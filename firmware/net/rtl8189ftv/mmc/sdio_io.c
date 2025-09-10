#define DEBUG

#include "type.h"
#include "sdio.h"
#include "error.h"
#include "debug.h"
#include "api.h"
#include "net/wireless/usr_cfg.h"
/*
 * The virtio configuration space is defined to be little-endian.  arm9 is
 * little-endian too, but it's nice to be explicit so we have these helpers.
 */
#define cpu_to_le16(v16) (v16)
#define cpu_to_le32(v32) (v32)
#define cpu_to_le64(v64) (v64)
#define le16_to_cpu(v16) (v16)
#define le32_to_cpu(v32) (v32)
#define le64_to_cpu(v64) (v64)

#define le32_to_cpup __le32_to_cpup
#define le16_to_cpup __le16_to_cpup

static __u16 __le16_to_cpup(const __le16 *p)
{
	return (__u16)*p;
}

static __u32 __le32_to_cpup(const __le32 *p)
{
	return (__u32)*p;
}

int sdio_enable_func(struct sdio_func *func)
{
	int ret;
	unsigned char reg;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IOEx, 0, &reg);
	if (ret != SD_OK)
		goto err;

	reg |= 1 << func->num;

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IOEx, reg, NULL);
	if (ret != SD_OK)
		goto err;

	while (1)
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IORx, 0, &reg);
		if (ret != SD_OK)
			goto err;
		if (reg & (1 << func->num))
			break;
	}

	p_dbg("SDIO: Enabled func %d\n", reg);

	return 0;

err:
	p_err("SDIO: Failed to enable device\n");
	return ret;
}

int sdio_disable_func(struct sdio_func *func)
{
	int ret;
	unsigned char reg;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IOEx, 0, &reg);
	if (ret == 0xff)
		goto err;

	reg &= ~(1 << func->num);

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IOEx, reg, NULL);
	if (ret == 0xff)
		goto err;

	return 0;

err:
	p_err("SDIO: Failed to disable device\n");
	return -EIO;
}

int sdio_claim_irq(struct sdio_func *func, void (*handler)(struct sdio_func *))
{
	int ret;
	unsigned char reg;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);
	if (ret == 0xff)
	{
		p_err("sdio_claim_irq err\n");
		return ret;
	}
	p_dbg("sdio_claim_irq0 :%x\n", reg);

	reg |= 1 << func->num;
	reg |= 0x01; /* Master interrupt enable */

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IENx, reg, NULL);
	if (ret == 0xff)
		p_err("sdio_claim_irq err1\n");

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);
	p_dbg("sdio_claim_irq1 :%x\n", reg);

	func->irq_handler = handler;

	/* Create init thread */
	if (xTaskCreate(sdio_irq_thread, "init", 2048, &bw_mmc_host, tskIDLE_PRIORITY + 21, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);

	// request_irq(IO_INT, BSP_INT_PRIO_THIRD, sdio_irq_handler);
	BSP_IntVecReg(IO_INT, 0, sdio_irq_handler, NULL);
	return ret;
}

int sdio_release_irq(struct sdio_func *func)
{
	int ret;
	unsigned char reg;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);
	if (ret == 0xff)
		return ret;

	reg &= ~(1 << func->num);

	/* Disable master interrupt with the last function interrupt */
	if (!(reg & 0xFE))
		reg = 0;

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IENx, reg, NULL);
	if (ret == 0xff)
		return ret;

	return 0;
}

void sdio_claim_host(struct sdio_func *func)
{
	os_down_sema(func->card->host->lock);
}

void sdio_release_host(struct sdio_func *func)
{
	os_up_sema(func->card->host->lock);
}

u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret)
{
	int ret;
	u8 val = 0;

	if (err_ret)
		*err_ret = 0;

	ret = mmc_io_rw_direct(0, func->num, addr, 0, &val);
	if (ret == 0xff)
	{
		if (err_ret)
			*err_ret = ret;
		return 0xFF;
	}

	return val;
}

void sdio_writeb(struct sdio_func *func, u8 b, unsigned int addr, int *err_ret)
{
	int ret;

	ret = mmc_io_rw_direct(1, func->num, addr, b, NULL);
	if (err_ret)
		*err_ret = ret;
	if (ret == 0xff)
		p_err("faild:%d", *err_ret);
}

static int sdio_io_rw_ext_helper(struct sdio_func *func, int write,
								 unsigned addr, int incr_addr, u8 *buf, unsigned size)
{
	unsigned remainder = size;
	unsigned max_blocks;
	int ret;

	if (func->card->cccr.multi_block)
	{
		max_blocks = 512;

		while (remainder > func->cur_blksize)
		{
			unsigned blocks;

			blocks = remainder / func->cur_blksize;

			if (blocks > max_blocks)
				blocks = max_blocks;
			size = blocks * func->cur_blksize;

			ret = mmc_io_rw_extended(write,
									 func->num, addr, incr_addr, buf,
									 blocks, func->cur_blksize);
			if (ret == 0xff)
				return ret;

			remainder -= size;
			buf += size;
			if (incr_addr)
				addr += size;
		}
	}
	/* Write the remainder using byte mode. */
	while (remainder > 0)
	{

		size = min(remainder, 512);

		ret = mmc_io_rw_extended(write, func->num, addr,
								 incr_addr, buf, 0, size);
		if (ret == 0xff)
			return ret;

		remainder -= size;
		buf += size;
		if (incr_addr)
			addr += size;
	}
	return 0;
}

/**
 *	sdio_memcpy_toio - write a chunk of memory to a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to start writing to
 *	@src: buffer that contains the data to write
 *	@count: number of bytes to write
 *
 *	Writes to the address space of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,
					 void *src, int count)
{
	return sdio_io_rw_ext_helper(func, 1, addr, 1, src, count);
}

/**
 *	sdio_memcpy_fromio - read a chunk of memory from a SDIO function
 *	@func: SDIO function to access
 *	@dst: buffer to store the data
 *	@addr: address to begin reading from
 *	@count: number of bytes to read
 *
 *	Reads from the address space of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
int sdio_memcpy_fromio(struct sdio_func *func, void *dst,
					   unsigned int addr, int count)
{
	return sdio_io_rw_ext_helper(func, 0, addr, 1, dst, count);
}

/**
 *	sdio_readw - read a 16 bit integer from a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a 16 bit integer from the address space of a given SDIO
 *	function. If there is a problem reading the address, 0xffff
 *	is returned and @err_ret will contain the error code.
 */
u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret)
{
	int ret;

	if (err_ret)
		*err_ret = 0;

	ret = sdio_memcpy_fromio(func, func->tmpbuf, addr, 2);
	if (ret == 0xff)
	{
		if (err_ret)
			*err_ret = ret;
		return 0xFFFF;
	}

	return le16_to_cpup((__le16 *)func->tmpbuf);
}

/**
 *	sdio_writew - write a 16 bit integer to a SDIO function
 *	@func: SDIO function to access
 *	@b: integer to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a 16 bit integer to the address space of a given SDIO
 *	function. @err_ret will contain the status of the actual
 *	transfer.
 */
void sdio_writew(struct sdio_func *func, u16 b, unsigned int addr, int *err_ret)
{
	int ret;

	*(__le16 *)func->tmpbuf = cpu_to_le16(b);

	ret = sdio_memcpy_toio(func, addr, func->tmpbuf, 2);
	if (err_ret)
		*err_ret = ret;
}

/**
 *	sdio_readl - read a 32 bit integer from a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a 32 bit integer from the address space of a given SDIO
 *	function. If there is a problem reading the address,
 *	0xffffffff is returned and @err_ret will contain the error
 *	code.
 */
u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret)
{
	int ret;

	if (err_ret)
		*err_ret = 0;

	ret = sdio_memcpy_fromio(func, func->tmpbuf, addr, 4);
	if (ret == 0xff)
	{
		if (err_ret)
			*err_ret = ret;
		return 0xFFFFFFFF;
	}

	return le32_to_cpup((__le32 *)func->tmpbuf);
}

/**
 *	sdio_writel - write a 32 bit integer to a SDIO function
 *	@func: SDIO function to access
 *	@b: integer to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a 32 bit integer to the address space of a given SDIO
 *	function. @err_ret will contain the status of the actual
 *	transfer.
 */
void sdio_writel(struct sdio_func *func, u32 b, unsigned int addr, int *err_ret)
{
	int ret;

	*(__le32 *)func->tmpbuf = cpu_to_le32(b);

	ret = sdio_memcpy_toio(func, addr, func->tmpbuf, 4);
	if (err_ret)
		*err_ret = ret;
}

int sdio_set_block_size(struct sdio_func *func, unsigned blksz)
{
	int ret = 0;

	ret = mmc_io_rw_direct(1, 0,
						   SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE,
						   blksz & 0xff, NULL);
	if (ret == 0xff)
		goto end;

	ret = mmc_io_rw_direct(1, 0,
						   SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE + 1,
						   (blksz >> 8) & 0xff, NULL);

	func->cur_blksize = blksz;
	func->max_blksize = 512;

	if (ret == 0xff)
		goto end;
end:
	return ret;
}

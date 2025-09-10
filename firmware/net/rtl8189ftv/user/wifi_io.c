#include "type.h"
#include "api.h"
#include "sdio.h"
#include "error.h"
#include "wifi_io.h"
#include "debug.h"
#include "net/wireless/usr_cfg.h"

struct sdio_func *wifi_sdio_func;

struct sdio_func rtw_func = {
	.num = 1,
	.enable_timeout = 100,
};

struct mmc_card rtw_card = {
	.host = &bw_mmc_host,
	.func = &rtw_func,
};

struct mmc_host bw_mmc_host = {
	.card = &rtw_card,
};

// void sdio_claim_host(struct sdio_func*func)
//{
//	os_down_sema(&func->card->host->lock);
// }

// void sdio_release_host(struct sdio_func *func)
//{
//	os_up_sema(&func->card->host->lock);
// }

// u8 sdio_readb(struct sdio_func *func,unsigned int addr, int *err_ret)
//{
//	int ret;
//	u8 val = 0;

//	if (err_ret)
//		*err_ret = 0;

//	ret = mmc_io_rw_direct(0, func->num, addr, 0, &val);
//	if (ret) {
//		if (err_ret)
//			*err_ret = ret;
//		return 0xFF;
//	}

//	return val;
//}

// int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,
//	void *src, int count)
//{
//	return sdio_io_rw_ext_helper(func, 1, addr, 1, src, count);
// }

// int sdio_memcpy_fromio(struct sdio_func *func, void *dst,
//	unsigned int addr, int count)
//{
//	return sdio_io_rw_ext_helper(func, 0, addr, 1, dst, count);
// }

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
// u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret)
//{
//	int ret;

//	if (err_ret)
//		*err_ret = 0;
//
//	ret = sdio_memcpy_fromio(func, func->tmpbuf, addr, 4);
//	if (ret) {
//		if (err_ret)
//		*err_ret = ret;
//		return 0xFFFFFFFF;
//	}

//	return le32_to_cpup((__le32 *)func->tmpbuf);
//}

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
// void sdio_writel(struct sdio_func *func, u32 b, unsigned int addr, int *err_ret)
//{
//	int ret;

//	*(__le32 *)func->tmpbuf = cpu_to_le32(b);

//	ret = sdio_memcpy_toio(func, addr, func->tmpbuf, 4);
//	if (err_ret)
//		*err_ret = ret;
//}

// void sdio_writeb(struct sdio_func *func,u8 b, unsigned int addr, int *err_ret)
//{
//	int ret;

//	ret = mmc_io_rw_direct(1, func->num, addr, b, NULL);
//	if (err_ret)
//		*err_ret = ret;
//	if(ret)
//		p_err("faild:%d", *err_ret);
//}

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
// u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret)
//{
//	int ret;

//	if (err_ret)
//		*err_ret = 0;

//	ret = sdio_memcpy_fromio(func, func->tmpbuf, addr, 2);
//	if (ret) {
//		if (err_ret)
//			*err_ret = ret;
//		return 0xFFFF;
//	}

//	return le16_to_cpup((__le16 *)func->tmpbuf);
//}

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
// void sdio_writew(struct sdio_func *func, u16 b, unsigned int addr, int *err_ret)
//{
//
//	int ret;

//	*(__le16 *)func->tmpbuf = cpu_to_le16(b);

//	ret = sdio_memcpy_toio(func, addr, func->tmpbuf, 2);
//	if (err_ret)
//		*err_ret = ret;
//}

// int sdio_set_block_size(struct sdio_func *func,unsigned blksz)
//{
//	int ret = 0;

//	ret = mmc_io_rw_direct(1, 0,
//		SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE,
//		blksz & 0xff, NULL);
//	if (ret)
//		goto end;

//	ret = mmc_io_rw_direct(1, 0,
//		SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE + 1,
//		(blksz >> 8) & 0xff, NULL);

//	func->cur_blksize = blksz;
//	func->max_blksize = 512;

//	if (ret)
//		goto end;
// end:
//	return ret;
//}

/*
void sdio_claim_host(struct sdio_func*func)
{
	os_down_sema(&func->card->host->lock);
}

void sdio_release_host(struct sdio_func *func)
{
	os_up_sema(&func->card->host->lock);
}
*/
u32 return_addr()
{
	return rtw_card.host;
}

int sdio_bus_probe(void)
{
	int ret;
	// u32 temp =0;
	struct sdio_func *func = &rtw_func;
	func->card = &rtw_card;
	// printf("%x \n",func->card->host->lock);
	// temp = *((int *)0x215c444);
	//	printf("addr data:%x \n",temp);

	printf("sizeof struct mmc_card	:%d", sizeof(struct mmc_card));
	os_init_sema(&func->card->host->lock, 1);

	sdio_claim_host(func);

	ret = sdio_enable_func(func);
	if (ret)
	{
		sdio_disable_func(func);
		sdio_release_host(func);
		return -EIO;
	}

	sdio_release_host(func);

	sdio_set_block_size(func, 512);

	wifi_sdio_func = func;

	return 0;
}

int sdio_bus_remove(void)
{
	return 0;
}

SDIO_BUS_OPS rtw_sdio_bus_ops = {
	sdio_bus_probe,
	sdio_bus_remove,
	sdio_enable_func,
	sdio_disable_func,
	NULL,
	NULL,
	sdio_claim_irq,
	sdio_release_irq,
	sdio_claim_host,
	sdio_release_host,
	sdio_readb,
	sdio_readw,
	sdio_readl,
	sdio_writeb,
	sdio_writew,
	sdio_writel,
	sdio_memcpy_fromio,
	sdio_memcpy_toio,
};

void init_busops()
{
	rtw_sdio_bus_ops.bus_probe();
}

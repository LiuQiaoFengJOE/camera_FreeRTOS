#define DEBUG
#include "drivers/appodriver/sdio/sdc.h"
// #include "inc/hld/sdio/sdcard.h"
#include "type.h"
#include "sdio.h"
#include "api.h"
#include "debug.h"
#include <string.h>
#include "error.h"
#include "bsp.h"
// #include "inc/system/ax3268/interrupt.h"
#ifndef ENABLE
#define ENABLE (1)
#endif
#ifndef DISABLE
#define DISABLE (0)
#endif

void enable_irq(INT_SRC irq_id)
{
	u32 irq;
	irq = mfspr(SPR_PICMR);
	mtspr(SPR_PICMR, irq | BIT(irq_id));
}
void disable_irq(INT_SRC irq_id)
{
	u32 irq;
	irq = mfspr(SPR_PICMR);
	mtspr(SPR_PICMR, irq & ~BIT(irq_id));
}

static void sdc_wait_74_clks(void)
{
	volatile int i = 12000;

	while (i--)
		;
}

static void sdc_io_init(void)
{
	init_sdmutex();

	SD1_Init();
	sdc_wait_74_clks();
}

int SDC_WaitDAT0High(void)
{
	SD_Error errorstatus = SD_OK;

	if (SD1_WaitDAT0High() == false)
	{
		errorstatus = SD_ERROR;
		p_err("WaitDAT0High fail !!!\n");
	}

	return errorstatus;
}

static void reset_wifi(void)
{
	/*
	SDIO_PDN_LOW;
	SDIO_PDN_OUTPUT;

	msleep(50);
	SDIO_PDN_HIGH;
	msleep(50);
	 * */
}

int mmc_go_idle()
{
	SD_Error errorstatus = SD_OK;

	SD1_SendCMD(SDIO_GO_IDLE_STATE, 0, NO_RSP_CHECK_BUSY);
	// printf("%d\n",__LINE__);
	return (errorstatus);
}

/**
 * @brief  Converts the number of bytes in power of two and returns the
 *   power.
 * @param  NumberOfBytes: number of bytes.
 * @retval None
 */
/*
static uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes)
{
	uint8_t count = 0;

	while (NumberOfBytes != 1)
	{
		NumberOfBytes >>= 1;
		count++;
	}
	return (count);
}
*/

SD_Error SD_SelectDeselect(uint32_t addr)
{
	SD_Error errorstatus = SD_OK;

	SD1_SendCMD(SDIO_SEL_DESEL_CARD, addr, RSP_6B_CHECK_BUSY);

	// errorstatus = check_rsp1(SDIO_SEL_DESEL_CARD);

	if (errorstatus != SD_OK)
		p_err("SD_SelectDeselect err:%d\n", errorstatus);

	return (errorstatus);
}

static int mmc_send_relative_addr(u16 *rca)
{
	bool errorstatus = true;
	u32 resp;

	if (rca == 0)
		return false;

	errorstatus = SD1_SendCMD(3, 0x00, RESP_6);

	errorstatus = SD1_GetRsp(&resp);

	if (true != errorstatus)
	{
		deg("%s err\n", __func__);
	}
	else
	{
		*rca = resp >> 16;
	}

	return (errorstatus);
}

static int sdio_read_cccr(struct mmc_card *card)
{
	int ret;
	int cccr_vsn;
	unsigned char data;
	printf("**%d %x sizeof %d\n", __LINE__, return_addr(), sizeof(struct sdio_cccr));
	// memset(&card->cccr, 0, 12);
	memset(&card->cccr, 0, sizeof(struct sdio_cccr));

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_CCCR, 0, &data);
	if (ret != SD_OK)
	{
		printf("mmc_io_rw_direct\n");
		goto out;
	}

	cccr_vsn = data & 0x0f;
	p_dbg("SDIO_CCCR_CCCR:%x\n", cccr_vsn);

	if (cccr_vsn > SDIO_CCCR_REV_1_20)
	{
		p_err("unrecognised CCCR structure version %d\n", cccr_vsn);
		return SD_ERROR;
	}

	card->cccr.sdio_vsn = (data & 0xf0) >> 4;
	printf("**%d %x\n", __LINE__, return_addr());

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_CAPS, 0, &data);
	if (ret != SD_OK)
	{

		printf("2mmc_io_rw_direct\n");

		goto out;
	}
	p_dbg("SDIO_CCCR_CAPS:%x\n", data);

	if (data & SDIO_CCCR_CAP_SMB)
		card->cccr.multi_block = 1;
	if (data & SDIO_CCCR_CAP_LSC)
		card->cccr.low_speed = 1;
	if (data & SDIO_CCCR_CAP_4BLS)
		card->cccr.wide_bus = 1;

	if (cccr_vsn >= SDIO_CCCR_REV_1_10)
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_POWER, 0, &data);

		printf("sdio cccr power:%x\n", data);
		if (ret != SD_OK)
		{

			printf("3mmc_io_rw_direct\n");

			goto out;
		}
		if (data & SDIO_POWER_SMPC)
			card->cccr.high_power = 1;
	}

	if (cccr_vsn >= SDIO_CCCR_REV_1_20)
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_SPEED, 0, &data);

		p_dbg("sdio cccr speed:%x\n", data);
		if (ret != SD_OK)
		{

			goto out;
		}
		if (data & SDIO_SPEED_SHS)
			card->cccr.high_speed = 1;
	}

out:
	if (ret != SD_OK)
		p_err("sdio_read_cccr err\n");
	return ret;
}

static int mmc_sdio_switch_hs(int enable)
{
	int ret;
	u8 speed;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_SPEED, 0, &speed);
	if (ret != SD_OK)
	{
		p_err("mmc_sdio_switch_hs err:%d\n", ret);
		return ret;
	}
	if (enable)
		speed |= SDIO_SPEED_EHS;
	else
		speed &= ~SDIO_SPEED_EHS;

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_SPEED, speed, NULL);
	if (ret != SD_OK)
		p_err("mmc_sdio_switch_hs err1:%d\n", ret);
	else
		printf("mmc_sdio_switch_hs ok\n");

	return ret;
}

#if ENABLE_4BIT_DATA_BUS
static int sdio_enable_wide()
{
	/*
	int ret;
	u8 ctrl;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IF, 0, &ctrl);
	if (ret != SD_OK)
	{
		p_err("sdio_enable_wide err:%d\n", ret);
		return ret;
	}

	ctrl &= 0xfc;
	ctrl |= SDIO_BUS_WIDTH_4BIT;

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IF, ctrl, NULL);

	if (ret != SD_OK)
		p_err("sdio_enable_wide err1:%d\n", ret);
	else
		printf("sdio_enable_wide ok\n");

//	REG32(SDC0_CFG0) |= BIT(2);

	return ret;
	 * */
	return sdio_card_bus_4();
}
#endif

struct cis_tpl
{
	unsigned char code;
	unsigned char min_size;
	// tpl_parse_t *parse;
};

struct sdio_func_tuple
{
	unsigned char code;
	unsigned char size;
	unsigned char data[128];
};

static int sdio_read_cis(int f_n)
{
	int ret;
	struct sdio_func_tuple *this;
	unsigned int i, ptr = 0, tuple_cnt = 0;
	unsigned char tpl_code, tpl_link;

	/*
	 * Note that this works for the common CIS (function number 0) as
	 * well as a function's CIS * since SDIO_CCCR_CIS and SDIO_FBR_CIS
	 * have the same offset.
	 */
	for (i = 0; i < 3; i++)
	{
		unsigned char x;

		ret = mmc_io_rw_direct(0, f_n,
							   SDIO_FBR_BASE(f_n) + SDIO_FBR_CIS + i, 0, &x);
		if (ret == 0xFF)
			return ret;
		ptr |= x << (i * 8);
	}

	// p_dbg("read_cis,fn:%d,addr:%d\n", f_n, ptr);

	do
	{
		ret = mmc_io_rw_direct(0, f_n, ptr++, 0, &tpl_code);
		if (ret == 0xFF)
			break;

		/* 0xff means we're done */
		if (tpl_code == 0xff)
			break;

		/* null entries have no link field or data */
		if (tpl_code == 0x00)
			continue;

		ret = mmc_io_rw_direct(0, f_n, ptr++, 0, &tpl_link);
		if (ret == 0xFF)
			break;

		/* a size of 0xff also means we're done */
		if (tpl_link == 0xff)
			break;

		// p_info("tpl code:%x,size:%d\n", tpl_code, tpl_link);

		if (tuple_cnt > 9 || tpl_link > 128)
		{
			p_dbg("tuple_cnt over\n");
			break;
		}

		this = (struct sdio_func_tuple *)malloc(sizeof(*this) + tpl_link);
		if (!this)
			return -ENOMEM;

		for (i = 0; i < tpl_link; i++)
		{
			ret = mmc_io_rw_direct(0, f_n,
								   ptr + i, 0, &this->data[i]);
			if (ret == 0xFF)
				break;
		}

		dump_hex("cis", this->data, i);

		if (ret == 0xFF)
		{
			free(this);
			break;
		}
		if (tpl_code == CISTPL_VERS_1)
			p_dbg("%s\n", this->data + 2);

		free(this);

		ptr += tpl_link;
		tuple_cnt += 1;
	} while (ret);

	if (tpl_link == 0xff)
		ret = 0;

	return ret;
}

static int sdio_read_common_cis()
{
	return sdio_read_cis(0);
}

int sdio_init(void)
{
	int errorstatus = SD_OK;
	int ocr;
	u16 rca = 0;

	sdc_io_init();

again:

	// reset_wifi();

	errorstatus = sdio_reset(&bw_mmc_host);

	errorstatus = mmc_go_idle();
	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

	errorstatus = mmc_send_io_op_cond(0, (u32 *)&ocr);
	if (errorstatus != SD_OK)
	{

		printf("%d\n", __LINE__);
		goto out;
	}

	errorstatus = mmc_send_io_op_cond(ocr, (u32 *)&ocr);

	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

	errorstatus = mmc_send_relative_addr(&rca);
	printf("rca %x \n", rca);
	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

	errorstatus = SD_SelectDeselect(rca << 16);

	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

	errorstatus = sdio_read_cccr((struct mmc_card *)bw_mmc_host.card);

	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

	sdio_read_common_cis();

	errorstatus = mmc_sdio_switch_hs(ENABLE);
	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}

#if ENABLE_4BIT_DATA_BUS
	errorstatus = sdio_enable_wide();

	if (errorstatus != SD_OK)
	{
		printf("%d\n", __LINE__);
		goto out;
	}
#endif

	// 	REG32(SDC0_CFG0) &= ~ BIT(1); 						//send clk only when send cmd and data
	//	REG32(SDC0_BAUD) = APB_CLK/(2*24000000) - 1;
	SD1_ONLY_CMD_DATA_CLK_OUT;

	SD1_CLK_SET(24000000);

out:

	if (errorstatus != SD_OK)
	{
		p_err("SD_Init err:%d\n", errorstatus);
		msleep(1000);
		goto again;
	}
	printf("8189FTV sdio init ok%d\n", __LINE__);
	return errorstatus;
}

static void mmc_signal_sdio_irq(struct mmc_host *host)
{
	// host->ops->enable_sdio_irq(host, 0);
	// host->sdio_irq_pending = true;
	os_up_sema(&host->sdio_irq_sema);
}

static void sdio_clr_irq_pending(void)
{
	//	CLRB(REG32(PORTWKEN), 4+3);
	EXT_INT_CLR_PEND(IO_INT);
}

void sdio_irq_handler(void)
{
	if (EXT_INT_CHK_PEND(IO_INT))
	{
		sdio_clr_irq_pending();
		mmc_signal_sdio_irq(&bw_mmc_host);
	}
}

static int process_sdio_pending_irqs(struct mmc_host *host)
{
	struct sdio_func *func = host->card->func;

	/*
	 * Optimization, if there is only 1 function interrupt registered
	 * and we know an IRQ was signaled then call irq handler directly.
	 * Otherwise do the full probe.
	 */
	if (func)
	{
		sdio_claim_host(func);
		func->irq_handler(func);
		sdio_release_host(func);
		return 1;
	}

	return 0;
}

void sdio_irq_thread(void *data)
{
	struct mmc_host *host = data;
	int ret;

	os_init_sema(&host->sdio_irq_sema, 0);

	do
	{
		/*
		 * We claim the host here on drivers behalf for a couple
		 * reasons:
		 *
		 * 1) it is already needed to retrieve the CCCR_INTx;
		 * 2) we want the driver(s) to clear the IRQ condition ASAP;
		 * 3) we need to control the abort condition locally.
		 *
		 * Just like traditional hard IRQ handlers, we expect SDIO
		 * IRQ handlers to be quick and to the point, so that the
		 * holding of the host lock does not cover too much work
		 * that doesn't require that lock to be held.
		 */
		ret = process_sdio_pending_irqs(host);
		if (ret == 0)
			p_dbg("process sdio irq failed\n");
		// host->sdio_irq_pending = false;

		// host->ops->enable_sdio_irq(host, 1);
		os_down_sema(&host->sdio_irq_sema);
	} while (1);
}

void disable_sdio_int_ext(void)
{
#if ENABLE_4BIT_DATA_BUS
	disable_irq(IO_INT);
#endif
}

void enable_sdio_int_ext(void)
{
#if ENABLE_4BIT_DATA_BUS
	//	CLRB(REG32(PORTWKEN), 4 + 3);
	/*if(!CHKB(REG32(PA), 5)){
		if(!CHKB(REG32(PORTWKEN), 4 + 3)){
			mmc_signal_sdio_irq(&bw_mmc_host);
		}
	}*/
	enable_irq(IO_INT);
#endif
}

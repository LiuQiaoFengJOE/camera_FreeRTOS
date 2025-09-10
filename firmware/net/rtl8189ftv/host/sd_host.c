/* Filename:     sd.c
 * Version:      1.0
 * Author:
 * Started:
 * Target Soc:   arm(32bit)
 *
 * Description: driver for sd card module.
 *
 * Copyright (C) 2014 Buildwin Integration Co.Ltd, Zhuhai, China.
 * 		http://www.buildwin.com.cn
 * */

#include <type.h>

#include <api.h>
#include "sd_host.h"
#include "string.h"
#include "dma.h"
#include "debug.h"
#include "bsp.h"

/* Debug Macro Definitions */
/* Internel driver debug */
#define DEBUG_EN 1
/* debug level 0~3, value less, print info less */
#define DEBUG_LEVEL 3

#define Debug_Printf printf

/* Internel driver debug */
#if (DEBUG_EN == 1)
#define sd_dbg(l, x, arg...) \
	if (l <= DEBUG_LEVEL)    \
	Debug_Printf("[SD_DBG] " x, ##arg)
/* print when warnning happens */
#define sd_warn(x, arg...) Debug_Printf("[SD_WARN] " x, ##arg)
/* print when error happens */
#define sd_err(x, arg...) Debug_Printf("[SD_ERR] " x, ##arg)
/* print unconditional, for important info */
#define sd_ok(x, arg...) Debug_Printf("[SD_OK] " x, ##arg)
#else
#define sd_dbg(l, x, arg...)
#define sd_warn(x, arg...)
#define sd_err(x, arg...)
#define sd_ok(x, arg...)
#endif /* DEBUG_EN */

#define SETB(REG, BIT) ((REG) |= (1UL << (BIT)))
#define CLRB(REG, BIT) ((REG) &= (~(1UL << (BIT))))
#define XRLB(REG, BIT) ((REG) ^= (1UL << (BIT)))

#define USE_SD_ISR 0

typedef xSemaphoreHandle _sema;

static volatile bool g_bCardAbendFlag;		//
static volatile bool g_bCardOnlineFlag = 0; // SD¿¨ÔÚÏß±êÖ¾

struct sd_card card;
// static mutex_t sd_mutex;
static _sema sd_sema;

// SD_INFO g_tSdInfo;

bool sd_init(void);
bool sd_wait_data_high(void);
/*
 * SDC_CFG0
 * */
#define SDC_CFG0_FIFO_SEL 0x800
#define SDC_CFG0_FIFO_CLREN 0x400
#define SDC_CFG0_DIE 0x200
#define SDC_CFG0_CIE 0x100
#define SDC_CFG0_FIFO_IE 0x080
#define SDC_CFG0_CLK_AUTO_STOP 0x040
#define SDC_CFG0_CRC_STATUS 0x020
#define SDC_CFG0_8CLK_EN 0x010
#define SDC_CFG0_CLK_RISING 0x008
#define SDC_CFG0_4BIT_BUS 0x004
#define SDC_CFG0_KEEP_CLK 0x002
#define SDC_CFG0_CONTROLLER_EN 0x001

/*
 * SDC_CFG1
 * */
#define SDC_CFG1_FIFO_PNDCLR 0x400
#define SDC_CFG1_DPCLR 0x200
#define SDC_CFG1_CPCLR 0x100
/* for example: 6-bytes responses, without busy check */
#define SDC_CFG1_CKST_6RESP_NB 0x001
#define SDC_CFG1_CKST_NRESP_NB 0x002
#define SDC_CFG1_CKST_17RESP_NB 0x003
#define SDC_CFG1_CKST_NRESP_B 0x004
#define SDC_CFG1_CKST_6RESP_B 0x005
#define SDC_CFG1_CKST_17RESP_B 0x007

/*
 * note: SDC_CFG2, all bits are read-only.
 * */
#define SDC_CFG2_DPND 0x200
#define SDC_CFG2_CPND 0x100
#define SDC_CFG2_DCRCE 0x080
#define SDC_CFG2_RCRCE 0x040
#define SDC_CFG2_NRPS 0x020
#define SDC_CFG2_BUSY 0x010
#define SDC_CFG2_FIFOPND 0x008
#define SDC_CFG2_CRCS_MASK 0x007
#define SDC_CFG2_CRCS_FLASH_ERR 0x007
#define SDC_CFG2_CRCS_TRANS_ERR 0x005
#define SDC_CFG2_CRCS_NO_TRANS 0x002

#if USE_SD_ISR
static OS_SEM SDDataPend;

void sd_ISR(void)
{
	OS_ERR err;
	// clear pending
	REG32(SDC_CFG1) = SDC_CFG1_DPCLR;

	OSSemPost((OS_SEM *)&SDDataPend,
			  (OS_OPT)OS_OPT_POST_ALL,
			  (OS_ERR *)&err);
}
#endif

/*
 * send command to sd card.
 * note: no response from card, such as CMD0, use this function directly, otherwise, use
 * sd_send_command() which will check the validity of relative response.
.
 * */
static bool sd_send_command_do(unsigned char bCmd, unsigned int dwArgument,
							   unsigned char bResponse)
{
	unsigned int dwWait = 100000;

	REG32(SDC_INDEX) = (0x40 | bCmd); /* Fixed Command Format: 0b01xxxxxx */
	REG32(SDC_ARG) = dwArgument;
	REG32(SDC_CFG1) = SDC_CFG1_CPCLR;	 /* firstly clear command pending flag */
	REG32(SDC_CFG0) |= SDC_CFG0_8CLK_EN; /* send 8 clocks behind command */
	REG32(SDC_CFG1) = bResponse;		 /* kick start to send command */

	while (dwWait--)
	{ /* wait command pending */
		if (REG32(SDC_CFG2) & SDC_CFG2_CPND)
		{										  /* cmd has been sent */
			REG32(SDC_CFG0) &= ~SDC_CFG0_8CLK_EN; /* stop to send 8 clocks behind command successfully */
			return true;
		}
	}
	REG32(SDC_CFG0) &= ~SDC_CFG0_8CLK_EN; /* don't send 8 clocks after send command successfully */
	return false;
}

/*
 * send command to sd card, should check check the validity of relative response.
 * */
static bool sd_send_command(unsigned char bCmd, unsigned int dwArgument,
							unsigned char bResponse)
{
	if (!sd_send_command_do(bCmd, dwArgument, bResponse))
	{
		sd_err("send cmd%d error.\n", bCmd);
		return false;
	}

	if (REG32(SDC_CFG2) & SDC_CFG2_NRPS)
	{
		// sd_warn("cmd%d can't receive expected response.\n", bCmd);
		return false;
	}
	return true;
}

/*
 * send application command to card.
 * note: should send cmd55 firstly.
 * */
static bool sd_send_app_cmd(unsigned char bCmd, unsigned int dwArgument,
							unsigned char bResponse)
{
#define sd_send_app_op_cond sd_send_command(55, (card.wRca << 16), RESP_1)

	if (sd_send_app_op_cond)
	{
		sd_send_command_do(bCmd, dwArgument, bResponse);

		if (REG32(SDC_CFG2) & SDC_CFG2_NRPS)
		{
			sd_err("acmd%d can't receive response\n", bCmd);
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

/*
 * sets the block length (in bytes) for all following block commands (read, write, lock).
 * default block length is fixed to 512 bytes.
 * Note: In the case of a High Capacity SD Memory Card(SDHC), block length set by CMD16 command does
 * not affect the memory read and write commands. Always 512 Bytes fixed block length is used.
 * In both cases, if block length is set larger than 512Bytes, the card sets the BLOCK_LEN_ERROR bit.
 * */
#if 0
static bool sd_set_block_length(unsigned short wBlockLen) {
	return sd_send_command(16, wBlockLen, RESP_1);
}
#endif

/*
 * get the selected card's card-specific data (CSD).
 * */
static bool sd_get_csd(void)
{
	return sd_send_command_do(9, (card.wRca << 16), RESP_2);
}

/*
 * get card's capacity.
 * */
static void sd_get_card_capacity(void)
{
	unsigned short dwBlkLen, dwCsizeMult;
	unsigned short dwCsize;

	sd_get_csd();

	if (0x40 == ((REG32(SDC_RESP3) >> 24) & 0xff))
	{ // csd 2.0 SDHC or SDXC
		dwCsize = ((unsigned short)((REG32(SDC_RESP1) >> 24) & 0xff) << 8);
		dwCsize += (REG32(SDC_RESP1) >> 16) & 0xff;
		card.dwCapacity = ((unsigned int)(dwCsize + 1)) << 10;
	}
	else if (0x00 == ((REG32(SDC_RESP3) >> 24) & 0xff))
	{ // csd 1.0 or csd2.0 SC
		dwBlkLen = ((REG32(SDC_RESP2) >> 16) & 0xff) & 0x0f;
		dwCsize = (unsigned short)(REG32(SDC_RESP2) >> 0) & 0xff; // C_SIZE==[73:62]
		dwCsize <<= 2;
		dwCsize += ((unsigned short)(((REG32(SDC_RESP2) >> 8) & 0xff) & 0x03) << 10);
		dwCsize += ((((REG32(SDC_RESP1) >> 24) & 0xff) & 0xc0) >> 6);
		dwCsizeMult = ((((REG32(SDC_RESP1) >> 16) & 0xff) & 0x03) << 1); // dwCsizeMult=bit[49:47]
		dwCsizeMult += ((((REG32(SDC_RESP1) >> 8) & 0xff) & 0x80) >> 7);
		card.dwCapacity = ((unsigned int)(dwCsize + 1)) << (dwCsizeMult + dwBlkLen - 7);
	}
	sd_dbg(3, "card's capacity : %08x %dM\n", card.dwCapacity, card.dwCapacity * 512 / 1024 / 1024);
}

unsigned int sd_n_sectors(void)
{
	return card.dwCapacity;
}
/*
 *
 * */
static void sd_set_card_status(enum card_status status)
{

	if (g_bCardOnlineFlag)
	{
		sd_dbg(3, "SDC ONLINE, SET CARD STATUS\r\n");
		card.bCardStatus = status;
	}
	else
	{
		sd_dbg(3, "SDC DISONLINE\r\n");
		card.bCardStatus = DISONLINE;
	}
}

enum card_status sd_get_card_status(void)
{
	return card.bCardStatus;
}

/*
 * note: bRw, 1-->read lba. 0-->write lba.
 * */
void sd_start_transmission(unsigned int pDataBuf, unsigned int cnt, unsigned char bRW)
{

	REG32(SDC_DCNT) = (cnt << 16) | (cnt);

	if (bRW)
	{ /* read lba */
		dma_peri2mem(0, (1 << 11) | (0 << 7) | (1 << 1), (0 << 11) | (0 << 10), REG32(SDC0_DMA_TX_ADR), pDataBuf, (cnt + 3) / 4);
	}
	else
	{ /* write lba */
		dma_mem2peri(0, (0 << 11) | (0 << 7) | (1 << 1), (0 << 11) | (0 << 10), pDataBuf, REG32(SDC0_DMA_RX_ADR), (cnt + 3) / 4);
	}

	REG32(SDC_CFG1) = (bRW) ? 0x10 : 0x30; /* Kick-Start */
}

/*
 * wait for the data transmission complete.
 * */
void sd_wait_transmission_done(void)
{

#if USE_SD_ISR
	OS_ERR err;
	CPU_TS ts;
#if 0	
	OSSemPend((OS_SEM *)&SDDataPend,
			(OS_TICK )0,
			(OS_OPT  )OS_OPT_PEND_BLOCKING,
			(CPU_TS *)&ts,
			(OS_ERR *)&err);
#else
	OSSemPend((OS_SEM *)&SDDataPend,
			  (OS_TICK)500,
			  (OS_OPT)OS_OPT_PEND_BLOCKING,
			  (CPU_TS *)&ts,
			  (OS_ERR *)&err);
	if (OS_ERR_NONE == err)
	{
	}
	else if (OS_ERR_TIMEOUT == err)
	{
		dmac_channel_disable(0); // DMA disabl
		REG32(SDC_CFG1) = (SDC_CFG1_DPCLR);
		REG32(SDC_CFG0) &= ~(SDC_CFG0_CONTROLLER_EN);

		sd_err("SDDataPend  timeout\r\n");
	}
	else if (OS_ERR_PEND_WOULD_BLOCK == err)
	{
		sd_err("SDDataPend semaphore not available\r\n");
	}
	else
	{
		sd_err("SDDataPend  err %-08d\r\n", err);
	}
#endif
#else

	unsigned int delay_cnt = 1300000;

	while (!(REG32(SDC_CFG2) & SDC_CFG2_DPND))
	{ /* wait data transmission complete */
		delay_cnt--;
		if ((delay_cnt == 0) || (g_bCardAbendFlag))
		{
			g_bCardAbendFlag = 1;
			sd_err("transmission error.\n");
			return;
		}
	}

	/* clear data pending flag */
	REG32(SDC_CFG1) = SDC_CFG1_DPCLR;
#endif
	if (card.bLastOperation == OPT_READ)
	{
		if (REG32(SDC_CFG2) & SDC_CFG2_DCRCE)
		{ /* check crc */
			sd_err("read, crc error.\n");
			g_bCardAbendFlag = 1;
			return;
		}
		else
		{
			// Debug_Printf("read, crc good\r\n");
		}
	}
	else
	{
		if ((REG32(SDC_CFG2) & SDC_CFG2_CRCS_MASK) != SDC_CFG2_CRCS_NO_TRANS)
		{ /* check crc */
			g_bCardAbendFlag = 1;
			sd_err("write, crc error.\n");
			return;
		}
		if (!sd_wait_data_high())
		{
			return;
		}

		dmac_channel_disable(0); // DMA disable
	}
}

/*
 * check what version the card is.
 * SDHC/SDLC/MMC.
 * */
bool sd_check_card_version(void)
{

	unsigned int dwTry;

	for (dwTry = 0; dwTry < 100; dwTry++)
	{
		/* resets all cards to idle state */
		sd_send_command_do(0, 0x00000000, NO_RESP);

		if (!sd_send_command(8, 0x000001aa, RESP_7) || (0x000001aa != SDC_RESP0))
		{
			continue;
		}
		goto SD_VER20;
	}

	// SD_VER10:
	for (dwTry = 0; dwTry < 200; dwTry++)
	{
		if (!sd_send_app_cmd(41, 0x00ff8000, RESP_3))
		{
			continue;
		}
		if (REG32(SDC_RESP0) & 0x80000000)
		{
			card.bVersion = SD_10;
			sd_dbg(3, "card is sd v1.0.\n");
			return true;
		}
	}

	// MMC:
	for (dwTry = 0; dwTry < 200; dwTry++)
	{
		/* resets all cards to idle state */
		sd_send_command_do(0, 0x00000000, NO_RESP);
		if (!sd_send_command(1, 0x40ff8000, RESP_3))
		{
			continue;
		}
		if (REG32(SDC_RESP0) & 0x80000000)
		{
			card.bVersion = MMC;
			sd_dbg(3, "card is MMC.\n");
			return true;
		}
	}

	card.bVersion = NOT_SD;
	sd_dbg(3, "card is unkown.\n");
	return false;

SD_VER20:
	for (dwTry = 0; dwTry < 2000; dwTry++)
	{
		if (!sd_send_app_cmd(41, 0x40ff8000, RESP_3))
		{
			continue;
		}
		if (SDC_RESP0 & 0x80000000)
		{
			break;
		}
	}

	if (0xc0000000 == (REG32(SDC_RESP0) & 0xc0000000))
	{
		card.bVersion = SD_20_HC;
		card.bSdhc = true;
		sd_dbg(3, "card is sd v2.0 high capacity.\n");
		return true;
	}
	else if (0x80000000 == (SDC_RESP0 & 0xc0000000))
	{
		card.bVersion = SD_20_SC;
		sd_dbg(3, "card is sd v2.0 normal capacity.\n");
		return true;
	}
	else
	{
		card.bVersion = NOT_SD;
		sd_dbg(3, "card is unkown type.\n");
		return false;
	}
}

/*
 *
 * */
static void sd_read_multiple_block_do(unsigned int pDataBuf, unsigned int dwLba)
{
	unsigned char bTry = 5;

	/* prepare to receive data from card. */
	sd_start_transmission(pDataBuf, BLOCK_LENGTH, SD_RX);

	while (1)
	{

		if (g_bCardAbendFlag)
		{
			return;
		}

		if (sd_send_command(18, ((card.bSdhc) ? dwLba : (dwLba << 9)), RESP_1))
		{
			// sd_dbg(3, "cmd18 ok\n");
			return;
		}

		if (--bTry == 0)
		{
			sd_err("read, time out.\n");
			g_bCardAbendFlag = 1;
			return;
		}
	}
}

/*
 *
 * */
bool sd_stop_last_operation(void)
{

	unsigned char stop_timeout = 5;

	while (1)
	{
		if (g_bCardAbendFlag)
		{
			return false;
		}

		REG32(SDC_CFG0) |= SDC_CFG0_KEEP_CLK;

		sd_send_command_do(12, 0x00000000, RESP_1B);
		REG32(SDC_CFG0) &= ~SDC_CFG0_KEEP_CLK;

		if (!sd_wait_data_high())
		{
			return false;
		}

		/*  */
		sd_send_command_do(13, (card.wRca << 16), RESP_1);

		if ((((REG32(SDC_RESP0) >> 8) & 0xff) & 0x1e) == 0x08)
		{
			// sd_dbg(3, "stop last operation ok\n");
			return true;
		}

		if (--stop_timeout == 0)
		{
			sd_dbg(3, "stop last operation fail\n");
			g_bCardAbendFlag = 1;
		}
	}
}

/*
 * set the rate of sd clk.
 * */
static inline void sd_set_speed(unsigned int wSdClk)
{
	//	SDC_BAUD = APB_CLK / (2 * wSdClk) - 1;
}

#if 0
/*
 *
 * */
static void sd_write_set_lba(unsigned int dwLba) {
	sd_send_command_do(25, ((card.bSdhc) ? dwLba : (dwLba << 9)), RESP_1);
}
#endif

/*
 *
 * */
bool sd_read_multiple_block(unsigned int pDataBuf, unsigned int dwLba)
{

	if (card.bCardStatus == UNACTIVE)
	{
		sd_dbg(3, "read, card is not active, init again.\n");
		sdc_init();
	}

	if (card.bCardStatus != ACTIVED)
	{
		sd_dbg(3, "read, card is not actived.\n");
		return false;
		// sd_dbg(3, "SDC MULTIPLE SECTORS READ BUT CARD IS ACTIVED, SDC INITIALIZE AGAIN\r\n");
		// SdcInitialize();
	}

	if (!g_bCardOnlineFlag)
	{
		sd_dbg(3, "read, card disonline.\n");
		return false;
	}

	if (card.dwCapacity < dwLba)
	{
		dwLba = card.dwCapacity - 1;
	}

	if (card.bLastOperation == OPT_READ)
	{
		if (card.iPreLba == dwLba)
		{
			sd_start_transmission(pDataBuf, BLOCK_LENGTH, SD_RX);
		}
		else
		{
			sd_stop_last_operation();
			sd_read_multiple_block_do(pDataBuf, dwLba);
		}
	}
	else
	{
		if (card.bLastOperation == OPT_WRITE)
		{
			sd_stop_last_operation();
		}
		sd_read_multiple_block_do(pDataBuf, dwLba);
	}

	card.bLastOperation = OPT_READ;
	card.iPreLba = dwLba + 1;

	sd_wait_transmission_done();

	if (g_bCardAbendFlag)
	{
		sd_set_card_status(UNACTIVE);
		return false;
	}
	return true;
}

/*
 *
 * */
bool sd_write_multiple_block(unsigned int pDataBuf, unsigned int dwLba)
{

	if (card.bCardStatus == UNACTIVE)
	{
		sd_dbg(3, "write, card is not active, init again\n");
		sdc_init();
	}

	if (card.bCardStatus != ACTIVED)
	{
		sd_dbg(3, "write, card is not actived\n");
		return false;
	}

	if (card.bLastOperation == OPT_WRITE)
	{
		if (card.iPreLba != dwLba)
		{
			sd_stop_last_operation();
			// sd_write_set_lba(dwLba);
			sd_send_command_do(25, ((card.bSdhc) ? dwLba : (dwLba << 9)), RESP_1);
		}
	}
	else
	{
		if (card.bLastOperation == OPT_READ)
		{
			sd_stop_last_operation();
		}
		// sd_write_set_lba(dwLba);
		sd_send_command_do(25, ((card.bSdhc) ? dwLba : (dwLba << 9)), RESP_1);
	}
	if (!sd_wait_data_high())
	{
		return false;
	}

	sd_start_transmission(pDataBuf, BLOCK_LENGTH, SD_TX);

	card.bLastOperation = OPT_WRITE;
	card.iPreLba = dwLba + 1;

	//	g_tSdInfo.eCardState = SD_WRITE;
	//	g_tSdInfo.dwNextLBA = dwLba + 1;

	sd_wait_transmission_done();

	if (g_bCardAbendFlag)
	{
		sd_dbg(1, "sd error\n");
		sd_set_card_status(UNACTIVE);
		return false;
	}
	return true;
}

/*
 * read one lba.
 * */
bool sd_read(unsigned int pDataBuf, unsigned int dwLba, unsigned int dwLBANum)
{
	// OS_ERR err;
	// CPU_TS ts;

	unsigned char cnt;
	unsigned char bTry;
	bool res = true;

	os_down_sema(&sd_sema);
	/*
	OSMutexPend((OS_MUTEX *)&sd_mutex,
			(OS_TICK   )0,
			(OS_OPT    )OS_OPT_PEND_BLOCKING,
			(CPU_TS   *)&ts,
			(OS_ERR   *)&err);*/

	for (cnt = 0; cnt < dwLBANum; cnt++)
	{
		bTry = 5;
		while (bTry--)
		{
			res = sd_read_multiple_block((unsigned int)((u8 *)pDataBuf + 512 * cnt), dwLba + cnt);
			if (res)
			{
				sd_dbg(3, "sd read ok\n");
				break;
			}
		}
	}

	os_up_sema(&sd_sema);
	/*
	OSMutexPost((OS_MUTEX *)&sd_mutex,
			(OS_OPT    )OS_OPT_POST_NONE,
			(OS_ERR   *)&err);*/
	if (res)
	{
		return true;
	}
	sd_warn("sd read fail\n");
	sd_set_card_status(INVALID);
	return false;
}

/*
 * write one lba.
 * */
bool sd_write(unsigned int pDataBuf, unsigned int dwLba, unsigned int dwLBANum)
{
	// OS_ERR err;
	// CPU_TS ts;

	unsigned char cnt;
	unsigned char bTry;
	bool res = true;

	os_down_sema(&sd_sema);
	/*
	OSMutexPend((OS_MUTEX *)&sd_mutex,
			(OS_TICK   )0,
			(OS_OPT    )OS_OPT_PEND_BLOCKING,
			(CPU_TS   *)&ts,
			(OS_ERR   *)&err);*/

	for (cnt = 0; cnt < dwLBANum; cnt++)
	{
		bTry = 5;
		while (bTry--)
		{
			res = sd_write_multiple_block((unsigned int)((u8 *)pDataBuf + 512 * cnt), dwLba + cnt);
			if (res)
			{
				// sd_dbg(3, "sd write ok\n");
				break;
			}
		}
	}

	os_up_sema(&sd_sema);
	/*
	OSMutexPost((OS_MUTEX *)&sd_mutex,
			(OS_OPT    )OS_OPT_POST_NONE,
			(OS_ERR   *)&err);*/
	if (res)
	{
		return true;
	}
	sd_warn("sd write fail\n");
	sd_set_card_status(INVALID);
	return false;
}

/*
 * get the new relative address(RCA) of the card.
 * */
static bool sd_allocate_rca(void)
{
	/* get CID of all card */
	if (!sd_send_command(2, 0x00000000, RESP_2))
	{
		return false;
	}

	/* get rca */
	if (!sd_send_command(3, 0x00000000, RESP_6))
	{
		return false;
	}

	card.wRca = (RSP6_39_32 << 8) | RSP6_31_24;
	sd_dbg(3, "card's rca : 0x%04x.\n", card.wRca);

	/* get cid of selected card */
	sd_send_command_do(10, (card.wRca << 16), RESP_2);

	return true;
}

/*
 *
 * */
static void sdc_variable_initialize(void)
{
	card.bLastOperation = OPT_NONE;
	card.bCardType = UNUSABLE;
	g_bCardAbendFlag = 0;
	card.bSdhc = false;
	card.bVersion = NOT_SD;
	card.wRca = 0;
}

/*
 * toggles a card between the stand-by and transfer states or between the programming
 * and disconnect states. In both cases, the card is selected by its own relative address(RCA)
 * and gets deselected by any other address; address 0 deselects all. In the case that
 * the RCA equals 0, then the host may do one of the following:
 * - Use other RCA number to perform card de-selection.
 * - Re-send CMD3 to change its RCA number to other than 0 and then use CMD7 with RCA=0 for card deselection.
 * */
static bool sd_select_card(void)
{

	if (sd_send_command(7, (card.wRca << 16), RESP_1B) || !sd_wait_data_high())
	{
		return true;
	}
	return false;
}

/*
 * forces the card to stop transmission.
 * */
bool sd_stop_transmission(void)
{
	if (sd_send_command_do(12, 0x00000000, RESP_1B))
	{
		return true;
	}
	return false;
}

/*
 * enable sdc controller.
 * */
static inline void sd_controller_enable(void)
{
	REG32(SDC_CFG0) |= SDC_CFG0_CONTROLLER_EN;
}

/*
 * disable sdc controller.
 * */
static inline void sd_controller_disable(void)
{
	REG32(SDC_CFG0) &= ~SDC_CFG0_CONTROLLER_EN;
}

/*
 * set sd bus width.
 * */
static bool sd_set_bus_width(unsigned char bBusWidth)
{
	if (4 == bBusWidth)
	{
		sd_dbg(3, "4bit data bus\n");
		REG32(SDC_CFG0) |= SDC_CFG0_4BIT_BUS;
		if (!sd_send_app_cmd(6, 0x00000002, RESP_1))
		{
			sd_err("set 4bit data bus error.\n");
			return false;
		}
	}
	else
	{
		sd_dbg(3, "1bit data bus\n");
		REG32(SDC_CFG0) &= ~SDC_CFG0_4BIT_BUS;
	}
	return true;
}

/*
 *
 * */
void sd_insert_card(void)
{

	if (!g_bCardOnlineFlag)
	{
		card.bCardStatus = UNACTIVE;
		g_bCardAbendFlag = 0;
		g_bCardOnlineFlag = 1;
	}
}

/*
 *
 * */
void sd_remove_card(void)
{

	if (g_bCardOnlineFlag)
	{
		card.bCardStatus = DISONLINE;
		g_bCardAbendFlag = 1;
		g_bCardOnlineFlag = 0;
	}
}

/*
 *
 * */
static void sd_prepare()
{
	sd_dbg(1, "sd io init\n");
	REG32(PMAP_CFG0) &= ~(BIT(1) | BIT(2));
	REG32(PMAP_CFG0) |= (1 << 3) | (2 << 1); // PA15,PA14,PA13,PA12,PA11,PA10......

	/* set PA12(sdclk) pin as output, pull up all gpio */
#if (4 == SDC_BUS_WIDTH)
	REG32(PA_DIR) &= (BIT(15) | BIT(14) | BIT(13) | ~BIT(12) | BIT(11) | BIT(10)); // PA12=SDC0CLK
	REG32(PA_PU0) |= BIT(15) | BIT(14) | BIT(13) | BIT(12) | BIT(11) | BIT(10);
#else
	REG32(PA_DIR) &= (BIT(13) | ~BIT(12) | BIT(11)); // PA12=SDC0CLK
	REG32(PA_PU0) |= BIT(13) | BIT(12) | BIT(11);
#endif /* SDC_BUS_WIDTH */

	sd_controller_disable();

	/* sd clk = system clock / 2 * (SDBUAD+1) , SDBUAD range: 0~255 */
	REG32(SDC_BAUD) = 0xFF; // 234.375 KHz

	/* clear command and data interrupt pending flag */
	REG32(SDC_CFG1) = (SDC_CFG1_CPCLR | SDC_CFG1_DPCLR);

	/* set out sample edge, '1'-rise edge; '0'-fall edge; default is '0' */
	REG32(SDC_CFG0) &= ~(SDC_CFG0_CLK_RISING);

	sd_controller_enable();

	SETB(REG32(SDC0_CFG0), 1);
	volatile int i = 12000;
	while (i--)
		;
}

/*
 * inti sd controller, check what version the card is, active card.
 * */
bool sdc_init(void)
{
	// OS_ERR err;

	// sd_mutex = mutex_init("sd mutex");
	os_init_sema(&sd_sema, 1);
	/*OSMutexCreate((OS_MUTEX *)&sd_mutex,
				  (CPU_CHAR *)"Mutex",
				  (OS_ERR   *)&err);*/

	/*
	if(err != OS_ERR_NONE){
		sd_err("OSMutexCreate sd_mutex err=%x\r\n",err);
		//return false;
	}*/

	sd_dbg(1, "sdc init\n");
	sdc_variable_initialize();
	sd_prepare();

#if USE_SD_ISR
	OSSemCreate((OS_SEM *)&SDDataPend,
				(CPU_CHAR *)"SD Data",
				(OS_SEM_CTR)0,
				(OS_ERR *)&err);

	BSP_IntVectReg(BSP_INT_ID_SDC0, 0, sd_ISR); // °²ÑbISRKÔÊÔSÖÐà
	REG32(SDC_CFG0) |= SDC_CFG0_DIE;			// ÔÊÔSSDµþ®aÉúÖÐà
	sd_err("SDC_CFG0 = 0x%x \r\n" ,REG32(SDC_CFG0);
#endif

	sd_stop_transmission();

	if (!sd_check_card_version() || (NOT_SD == card.bVersion)) {
		sd_set_card_status(INVALID);
		return false;
	}

	if (!sd_allocate_rca()) {
		sd_err("allocate rca error\n");
		return false;
	}

	sd_get_card_capacity();

	if (!sd_select_card()) {
		sd_err("select card error\n");
		return false;
	}

	/* to save system power, close sd clk when no command or data transmission. */
	REG32(SDC_CFG0) &= ~SDC_CFG0_KEEP_CLK;

	if (g_bCardAbendFlag) {
		sd_err("SDC CARD ABEND\r\n");
		sd_set_card_status(INVALID);
		return false;
	}

	g_bCardOnlineFlag = 1;
	sd_set_card_status(ACTIVED);

	sd_set_bus_width(SDC_BUS_WIDTH);
     
	REG32(SDC_BAUD) = REG32(SDC__BAUD);  //12M

	return true;
}

bool sd_wait_data_high(void)
{
	int i = 0xffffff;

	REG32(SDC_CFG0) |= BIT(1); // keep clk output
	do
	{
		if (CHKB(REG32(SDC_CFG2), 4)) // wait sd not busy
		{
			REG32(SDC_CFG0) &= ~BIT(1); // clk output when send cmd or data
			return true;
		}
	} while (i--);

	REG32(SDC_CFG0) &= ~BIT(1); // clk output when send cmd or data
	sd_err("WaitDAT0High fail !!!\r\n");
	return false;
}

#if DEBUG_EN
void check_print(unsigned char *buf, unsigned int size)
{
	unsigned long i;

	for (i = 0; i < size; i++)
	{
		if (i % 16 == 0)
			Debug_Printf("\n");
		Debug_Printf("%02x ", *buf++);
	}
	Debug_Printf("\n");
}
#endif

void sd_test()
{
#if DEBUG_EN
	unsigned char buf[512];
	unsigned int i;

	sd_read((unsigned int)buf, 0, 0);

	Debug_Printf("read one lba:\n");
	check_print((unsigned char *)buf, sizeof(buf));

	for (i = 0; i < 512; i++)
	{
		buf[i] = i + 8;
	}
	Debug_Printf("write one lba:\n");
	check_print((unsigned char *)buf, sizeof(buf));
	sd_write((unsigned int)buf, 0x4000, 0);

	sd_stop_transmission();

#endif
}

/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                       AX32XX SYSTEM
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_sys.c
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "typedef.h"

#include "spr_defs.h"
#include "AX329x.h"
// #include "gpio_ops/gpio.h"
#include "sys.h"
#include "portmacro.h"

#define CACHE_LINE_SIZE 16
#define CACHE_L_ALIGN(x) ((const void *)(((x) % CACHE_LINE_SIZE) ? ((x) + CACHE_LINE_SIZE - ((x) % CACHE_LINE_SIZE)) : (x)))
#define CACHE_A_ALIGN(x) ((const void *)((__u32)(x) & (~(__u32)(CACHE_LINE_SIZE - 1))))

#define R_SYS_CLK PCON0
#define R_SYS_CLK1 PCON1
#define R_SYS_LDO LDOCON
#define R_SYS_RST RSTCON
#define R_DMA_CTRL0 NOC_REG0

#ifndef __ASSEMBLER__
extern u32 __bss_start, __bss_end;
extern u32 __sram_start, __sram_end;
#endif

extern int _sdram_remian_addr;
extern int __sdram_remain_size;
volatile u8 smph_dmacopy;
/*******************************************************************************
 * Function Name  : ax32xx_sysDelay
 * Description    : system delay ms
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysCpuMsDelay(int dtime)
{
	volatile int cnt = dtime * 0x1000;
	while (cnt--)
	{
		asm("l.nop");
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_sysCpuNopDelay
 * Description    : system delay nop
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysCpuNopDelay(int dtime)
{
	while (dtime--)
	{
		asm("l.nop");
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheInit
 * Description    : dcache initial
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheInit(void)
{
#if CFG_DCACHE_EN
	u32 cnt = 0;
	__sprclr(SPR_DCCR, 0);
	// invalidate 256 lines
	// for (cnt = 0; cnt <__CAHCHE_SIZE__/__LINE_SIZE__; cnt ++)
	{
		// mtspr(SPR_DCBIR, cnt << 4);
		mtspr(SPR_DCAIR, cnt << 4);
	}
	// enable D-Cache
	// debg("D-cache open\n");
	__sprset(SPR_DCCR, 0);
#else
	__sprclr(SPR_DCCR, 0);
#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_sysIcacheInit
 * Description    : icache initial
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysIcacheInit(void)
{
#if CFG_ICACHE_EN
	u32 cnt = 0;
	__sprclr(SPR_ICCR, 0);
	// invalidate 256 lines
	{
		// mtspr(SPR_ICBIR, cnt << 4);
		mtspr(SPR_ICAIR, cnt << 4);
	}
	__sprset(SPR_ICCR, 0);
	// debg("I-cache open\n");
#else
	__sprclr(SPR_ICCR, 0);
#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheWback
 * Description    : data cache write back
 * Input          : u32 addr : address
 *                  u32 size : size
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheWback(u32 addr, u32 size)
{
#if CFG_DCACHE_EN
	if (addr & BIT(31))
		return;
	if (addr < 0x44000)
		return;

	while (size)
	{
		u32 wb_size = min(size, (4 * 1024));
		u32 dccnt = ((addr + wb_size - 1) - (addr & ~0x0f)) / 16;
		SPR_DCCNT = dccnt;
		SPR_DCBWR = addr;
		addr += wb_size;
		size -= wb_size;
	}

#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheFlush
 * Description    : data cache flush
 * Input          : u32 addr : address
 *                  u32 size : size
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheFlush(u32 addr, u32 size)
{
#if CFG_DCACHE_EN
	if (addr & BIT(31))
		return;
	if (addr < 0x44000)
		return;

	while (size)
	{
		u32 flush_size = min(size, (4 * 1024));
		u32 dccnt = ((addr + flush_size - 1) - (addr & ~0x0f)) / 16;
		SPR_DCCNT = dccnt;
		SPR_DCBFR = addr;
		addr += flush_size;
		size -= flush_size;
	}

#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_sysBSSClear
 * Description    : clear system bss phase
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysBSSClear(void)
{
	u32 cnt, *dst;

	cnt = ((u32)(&__bss_end) - (u32)(&__bss_start)) / 4;
	dst = &__bss_start;
	while (cnt--)
		*dst++ = 0;
}
/*******************************************************************************
 * Function Name  : ax32xx_sysSRAMClear
 * Description    : clear system sram
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysSRAMClear(void)
{
	u32 cnt, *dst;

	cnt = ((u32)(&__sram_end) - (u32)(&__sram_start)) / 4;
	dst = &__sram_start;
	while (cnt--)
		*dst++ = 0;
}
/*******************************************************************************
 * Function Name  : ax32xx_sysLDOSet
 * Description    : set system ldo for power support
 * Input          : u8 ldo : ldo type
 *                  u8 sel : power sel
 *                  u8 en  : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LDO_P()
{
	printf("R_SYS_LDO:%x\n", R_SYS_LDO);
}

void ax32xx_sysLDOSet(u8 ldo, u8 sel, u8 en)
{

	switch (ldo)
	{
	case SYS_LDO_HSEN:
		if (en)
		{
			printf("ax32xx_sysLDOSet000000000:%x\n", sel);
			// R_SYS_LDO = (R_SYS_LDO & ~0x07)|(sel&0x07)|(1<<3);
			R_SYS_LDO = (R_SYS_LDO & ~0x1f) | (sel & 0x1f) | (1 << 5);
		}
		else
			// R_SYS_LDO &= ~(1<<3);
			R_SYS_LDO &= ~(1 << 5);
		break;
	case SYS_LDO_LSEN:
		if (en)
		{
			// R_SYS_LDO = (R_SYS_LDO & ~(0x07<<4))|((sel&0x07)<<4)|(1<<7);
			R_SYS_LDO = (R_SYS_LDO & ~(0x1f << 7)) | ((sel & 0x1f) << 7) | (1 << 12);
		}
		else
			R_SYS_LDO &= ~(1 << 12);
		break;
	case SYS_LDO_BGOE:
		if (en)
		{
			// R_SYS_LDO |= 1<<11;
			R_SYS_LDO |= 1 << 15;
			// R_SYS_LDO  = (R_SYS_LDO & ~(0x07<<12))|((sel&0x07)<<12);
			R_SYS_LDO = (R_SYS_LDO & ~(0x1f << 16)) | ((sel & 0x1f) << 16);
		}
		else
			// R_SYS_LDO &= ~(1<<11);
			R_SYS_LDO &= ~(1 << 15);
		break;
	case SYS_LDO_DDR:
		if (en)
			R_SYS_LDO |= 1 << 10; //?
		else
			R_SYS_LDO &= ~(1 << 10); //?
		break;
	case SYS_LDO_V1_2:
		//			R_SYS_LDO = (R_SYS_LDO & ~(0x03<<8))|((sel&0x03)<<8);
		R_SYS_LDO = (R_SYS_LDO & ~(0x03 << 13)) | ((sel & 0x03) << 13);
		break;
	case SYS_LDO_SDC:
		if (en)
			// R_SYS_LDO |= 1<<15;
			R_SYS_LDO |= 1 << 21;
		else
			// R_SYS_LDO &= ~(1<<15);
			R_SYS_LDO &= ~(1 << 21);
		break;
	}
}

/*******************************************************************************
 * Function Name  : ax32xx_sysClkSet
 * Description    : system clk set
 * Input          : u8 clk_type : clk type.SYS_CLK_E
 *                  u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysClkSet(u8 clk_type, u8 en)
{
	u32 CLKSFR = (u32)&R_SYS_CLK;
	u8 dev_id = clk_type;

	if (clk_type > 31)
	{
		CLKSFR = (u32)&R_SYS_CLK1;
		dev_id = clk_type - 32;
	}

	if (en)
		SFR_AT CLKSFR &= ~(1 << dev_id);
	else
		SFR_AT CLKSFR |= (1 << dev_id);
}

/*******************************************************************************
 * Function Name  : ax32xx_sysReset
 * Description    : system module reset
 * Input          : u8 clk_type : clk type.SYS_RESET_E
 *                  u8 en : 1-reset,0-normal
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysReset(u8 rst_type, u8 en)
{
	if (en)
		R_SYS_RST &= ~rst_type;
	else
		R_SYS_RST |= rst_type;
}

void BtB_memcpy(void *dst, void *src, int cnt)
{
	int count = 0;
	for (count = 0; count < cnt; count++)
	{
		*(u8 *)dst = *(u8 *)src;
		dst++;
		src++;
	}
}

void* dma_memcpy(void *dst,const void *src, int cnt)
{
#if 0
	cpu_memcpy(dst, src, cnt);
	dcache_writeback_region((u32)dst, cnt);

#else
	//void *a_p = NULL;
	//void *l_p = NULL;

	void *a_pd = NULL;
	void *l_pd = NULL;

	//u32 cnt_src_a;
	//u32 cnt_src_l;

	u32 cnt_dst_a;
	u32 cnt_dst_l;

	u32 dma_cnt;
	u8 i = 0;

	u8 *chche_dat;
	u8 *src_ck;
	u8 *src_dat;
	u8 *cache_dat_wb;
	u8 *dst_ck;

	// dst1 = dst;
	// src1 = src;
	portINIT_CRITICAL();

	if (cnt < 64)
	{
		BtB_memcpy(dst, src, cnt);
		dcache_writeback_region((u32)dst, cnt);
		return dst;
	}
#if 0
	if ((u32)src&0xf)
	{
		a_p = (u32)src + 16 - ((u32)src&0xf); // src dma start adr
		cnt_src_a = 16 - ((u32)src&0xf);
	}
	else
	{
		a_p = src;
		cnt_src_a = 0;
	}
	//	printf("a_p:%x  cnt_src_a:%d\n",a_p,cnt_src_a);

	if (((u32)src + cnt) &0xf)
	{
		l_p = (u32)src + cnt - (((u32)src + cnt)&0xf);
		cnt_src_l = ((u32)src + cnt)&0xf;
	}
	else
	{
		l_p = src + cnt;
		cnt_src_l = 0;
	}
#endif
	// dcache_writeback_region((u32)a_p,l_p - a_p);
	dcache_writeback_region((u32)src, cnt);

	// if(cnt == 154)
	//	Uart_PrintfBuf("src:\n", src, cnt);
	#if 0
	static u32 lastTime;
	if(os_time_get()-lastTime>100)
	{
		lastTime=os_time_get();
		printf("dst:0x%x cnt:%d\n",dst,cnt);
	}
#endif
	if ((u32)dst &0xf)
	{
		a_pd = (u32)dst + 16 - ((u32)dst &0xf); // dst dma start adr
		cnt_dst_a = 16 - ((u32)dst &0xf);
	}
	else
	{
		a_pd = dst;
		cnt_dst_a = 0;
	}
	//	printf("a_pd:%x cnt_dst_a:%d\n",a_pd,cnt_dst_a);

	if (((u32)dst + cnt) &0xf)
	{
		l_pd = (u32)dst + cnt - (((u32)dst + cnt) &0xf);
		cnt_dst_l = ((u32)dst + cnt)  & 0xf;
	}
	else
	{
		l_pd = (u32)dst + cnt;
		cnt_dst_l = 0;
	}
	//	printf("l_pd:%x	cnt_dst_l:%d\n",l_pd,cnt_dst_l);

	if (cnt_dst_a)
	{
		if (dst < 0x2000000)
		{
			src_ck = (u8 *)dst;
		}
		else
		{
			src_ck = (u8 *)dst + 0x80000000;
		}

		if (src < 0x2000000)
		{
			src_dat = (u8 *)src;
		}
		else
		{
			src_dat = (u8 *)src + 0x80000000;
		}

		chche_dat = (u8 *)src_ck; // cache
		cache_dat_wb = (u8 *)dst; // sdram
		for (i = 0; i < cnt_dst_a; i++)
		{
			cache_dat_wb[i] = src_dat[i];
			chche_dat[i] = src_dat[i];
		}
	}

	if (cnt_dst_l)
	{
		if (l_pd < 0x2000000)
		{
			src_ck = (u8 *)l_pd;
		}
		else
		{
			src_ck = (u8 *)l_pd + 0x80000000;
		}

		if (src < 0x2000000)
		{
			src_dat = (u8 *)src;
		}
		else
		{
			src_dat = (u8 *)src + 0x80000000;
		}

		chche_dat = (u8 *)src_ck;  // cache
		cache_dat_wb = (u8 *)l_pd; // sdram
		for (i = 0; i < cnt_dst_l; i++)
		{
			cache_dat_wb[i] = src_dat[i + cnt - cnt_dst_l];
			chche_dat[i] = src_dat[i + cnt - cnt_dst_l];
		}
	}

	dcache_flush_region((u32)a_pd, l_pd - a_pd);
	portENTER_CRITICAL();
	if(!(MCP0CON & BIT(1)))
	{
		portEXIT_CRITICAL();
		while (!(MCP0CON & BIT(1)))
			;
		portENTER_CRITICAL();
	}
	
	MCP0CON = BIT(3);
	MCP0CON |= BIT(0);	// en
	MCP0CON &= ~BIT(2); // no sir
	MCP0SCADDR = (u32)src + cnt_dst_a;
	MCP0TGADDR = (u32)a_pd;
	MCP0LENGTH = l_pd - a_pd - 1;
	portEXIT_CRITICAL();
	while (!(MCP0CON & BIT(1)))
		;

	return dst;
#endif
}

void ax32xx_first_mem_dma(void *dst, void *src, int cnt)
{
	dcache_writeback_region((u32)src, cnt);
	dcache_flush_region((u32)dst, cnt);

	MCP0CON = BIT(3);
	MCP0CON = BIT(0); // en
	// MCP0CON &= ~BIT(2);	//no sir
	MCP0SCADDR = (u32)src;
	MCP0TGADDR = (u32)dst;
	MCP0LENGTH = cnt - 1;
	while (!(MCP0CON & BIT(1)))
		;
}
void dma_memcpy1(void *dst, void *src, int cnt)
{
	void *a_pd = NULL;
	void *l_pd = NULL;
	u32 cnt_dst_a;
	u32 cnt_dst_l;
	u32 dma_cnt;
	u8 i = 0;
	u8 *chche_dat;
	u8 *src_ck;
	u8 *src_dat;
	u8 *cache_dat_wb;
	u8 *dst_ck;

	if (cnt < 64)
	{
		BtB_memcpy(dst, src, cnt);
		dcache_writeback_region((u32)dst, cnt);
		return;
	}
	//dcache_writeback_region((u32)src, cnt);
	if ((u32)dst &0xf)
	{
		a_pd = (u32)dst + 16 - ((u32)dst &0xf); // dst dma start adr
		cnt_dst_a = 16 - ((u32)dst &0xf);
	}
	else
	{
		a_pd = dst;
		cnt_dst_a = 0;
	}
	if (((u32)dst + cnt) &0xf)
	{
		l_pd = (u32)dst + cnt - (((u32)dst + cnt) &0xf);
		cnt_dst_l = ((u32)dst + cnt)  & 0xf;
	}
	else
	{
		l_pd = (u32)dst + cnt;
		cnt_dst_l = 0;
	}
	if (cnt_dst_a)
	{
		if (dst < 0x2000000)
		{
			src_ck = (u8 *)dst;
		}
		else
		{
			src_ck = (u8 *)dst + 0x80000000;
		}

		if (src < 0x2000000)
		{
			src_dat = (u8 *)src;
		}
		else
		{
			src_dat = (u8 *)src + 0x80000000;
		}

		chche_dat = (u8 *)src_ck; // cache
		cache_dat_wb = (u8 *)dst; // sdram
		for (i = 0; i < cnt_dst_a; i++)
		{
			cache_dat_wb[i] = src_dat[i];
			chche_dat[i] = src_dat[i];
		}
	}

	if (cnt_dst_l)
	{
		if (l_pd < 0x2000000)
		{
			src_ck = (u8 *)l_pd;
		}
		else
		{
			src_ck = (u8 *)l_pd + 0x80000000;
		}

		if (src < 0x2000000)
		{
			src_dat = (u8 *)src;
		}
		else
		{
			src_dat = (u8 *)src + 0x80000000;
		}

		chche_dat = (u8 *)src_ck;  // cache
		cache_dat_wb = (u8 *)l_pd; // sdram
		for (i = 0; i < cnt_dst_l; i++)
		{
			cache_dat_wb[i] = src_dat[i + cnt - cnt_dst_l];
			chche_dat[i] = src_dat[i + cnt - cnt_dst_l];
		}
	}

	MCP1CON = BIT(3);
	MCP1CON = BIT(0); // en
	MCP1SCADDR = (u32)src + cnt_dst_a;
	MCP1TGADDR = (u32)a_pd;
	MCP1LENGTH = l_pd - a_pd - 1;
	while (1)
	{
		if (MCP1CON & BIT(1))
		{
			MCP1CON |= BIT(3); // clear dma_copy pengding
			break;
		}
	}
}

void ax32xx_mcpy1_sdram2gram(void *dst, void *src, int cnt)
{
	dcache_writeback_region((u32)src, cnt);
	dcache_flush_region((u32)dst, cnt);

	if (cnt == 0)
		return;
	MCP1CON = BIT(3);
	MCP1CON = BIT(0); // en
	MCP1SCADDR = (u32)src;
	MCP1TGADDR = (u32)dst;
	MCP1LENGTH = cnt - 1;
	while (1)
	{
		if (MCP1CON & BIT(1))
		{
			MCP1CON |= BIT(3); // clear dma_copy pengding
			break;
		}
	}
}

void ax32xx_mcpy2_uram2gram(void *dst, void *src, int cnt)
{
	// dcache_writeback_region((u32)src,cnt);
	// dcache_flush_region((u32)dst,cnt);

	if (cnt == 0)
		return;
	MCP2CON = BIT(3);
	MCP2CON = BIT(0); // en
	MCP2SCADDR = (u32)src;
	MCP2TGADDR = (u32)dst;
	MCP2LENGTH = cnt - 1;
	while (1)
	{
		if (MCP2CON & BIT(1))
		{
			MCP2CON |= BIT(3); // clear dma_copy pengding
			break;
		}
	}
}

/*******************************************************************************
* Function Name  : ax32xx_sysInit
* Description    : initial system for platform using.
* Input          : u32 *saddr : sdram free start address
					  u32 *eaddr : sdram free end address
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysInit(u32 *saddr, u32 *eaddr)
{
	__LGIE_DIS__();
	__intvct_adr__(0X02000000);

	ax32xx_wdtEnable(0);
	__HGIE_DIS__();

	//-----dma set
	ax32xx_dmaHSWSet();
	ax32xx_dmaCH8Set();
	// ax32xx_dmaHSRSet();
	//-----cache set
	ax32xx_sysIcacheInit();

	//-----bss clear
	ax32xx_sysSRAMClear();
	ax32xx_sysBSSClear();

	ax32xx_sysDcacheInit();
	//-----system power
	R_SYS_LDO = (0 << 15) | // VDDSD en
				(3 << 12) | // bandgap voltage select
				(1 << 11) | // adkey bandgap enable
				(0 << 10);	// ddr bandgap enable
#if 1
	ax32xx_sysLDOSet(SYS_LDO_V1_2, SYS_VOL_V1_30, 1);
	// ax32xx_sysLDOSet(SYS_LDO_BGOE,SYS_VOL_V0_71,1);
	ax32xx_sysCpuMsDelay(50);
	ax32xx_sysLDOSet(SYS_LDO_LSEN, SYS_VOL_V1_8, 1);
	ax32xx_sysCpuMsDelay(50);
	printf("SYS_VOL_V3_1:%d\n", SYS_VOL_V3_1);
	ax32xx_sysLDOSet(SYS_LDO_HSEN, 22, 1);
	ax32xx_sysCpuMsDelay(50);
//	ax32xx_sysLDOSet(SYS_LDO_ADKEY,0,1);
#else

	// ax32xx_sysLDOSet(SYS_LDO_BGOE,SYS_VOL_V0_71,1);
	ax32xx_sysLDOSet(SYS_LDO_LSEN, SYS_VOL_V1_5, 1);
	ax32xx_sysCpuMsDelay(10);
	ax32xx_sysLDOSet(SYS_LDO_HSEN, SYS_VOL_V3_1, 1);
	ax32xx_sysCpuMsDelay(10);
	ax32xx_sysLDOSet(SYS_LDO_V1_2, SYS_VOL_V1_30, 1);
	ax32xx_sysCpuMsDelay(10);
	ax32xx_sysLDOSet(SYS_LDO_ADKEY, 0, 1);
#endif
	//-----system colck
	ax32xx_sysClkSet(SYS_CLK_DVP_CSI, 1);
	ax32xx_sysClkSet(SYS_CLK_JPGA, 1);
	ax32xx_sysClkSet(SYS_CLK_JPGB, 1);
	ax32xx_sysClkSet(SYS_CLK_RTC, 1);

	// 	fp_usb_isr = ax32xx_usbIRQHanlder;

	//----INT initial
	ax32xx_intInit();
	if (saddr)
		*saddr = (u32)&_sdram_remian_addr; // 0x58400000; // from 1M_sdram_remian_addr;//
	if (eaddr)
		*eaddr = ((u32)&_sdram_remian_addr) + ((u32)&__sdram_remain_size); //(0x58000000+SDRAM_SIZE); // end address

	__LGIE_EN__();
	__HGIE_EN__();
}
/*******************************************************************************
 * Function Name  : ax32xx_sysUninit
 * Description    : uninitial system .
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysUninit(void)
{
	//	ax32xx_sysLDOSet(SYS_LDO_BGOE,0,0);
	ax32xx_sysLDOSet(SYS_LDO_LSEN, 0, 0);
	ax32xx_sysLDOSet(SYS_LDO_HSEN, SYS_VOL_V3_1, 0);
	//-----system colck
	//    ax32xx_sysClkSet(SYS_CLK_CSI,0);
	//	ax32xx_sysClkSet(SYS_CLK_JPEG,0);
	//    ax32xx_sysClkSet(SYS_CLK_LCD,0);
	ax32xx_sysClkSet(SYS_CLK_ADC, 0);
	ax32xx_sysClkSet(SYS_CLK_AUADC, 0);
	ax32xx_sysClkSet(SYS_CLK_DAC, 0);
	ax32xx_sysClkSet(SYS_CLK_DE, 0);
	//	ax32xx_sysClkSet(SYS_CLK_IIC0,0);
	//	ax32xx_sysClkSet(SYS_CLK_IIC1,0);
	ax32xx_sysClkSet(SYS_CLK_SD0, 0);
	ax32xx_sysClkSet(SYS_CLK_SPI0, 0);
}

/*******************************************************************************
 * Function Name  : ax32xx_dmaChannelEnable
 * Description    : dma channel enable
 * Input          : u8 channel : dma channel
 *                  u8 en : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_dmaChannelEnable(u8 channel, u8 en)
{
	if (en)
		R_DMA_CTRL0 |= 1 << channel;
	else
		R_DMA_CTRL0 &= ~(1 << channel);
}

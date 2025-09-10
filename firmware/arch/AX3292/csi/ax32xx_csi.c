/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                         CSI DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_csi.c
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN CSI HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "basic_defs.h"
#include "AX329x.h"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
#include "gpio_ops/gpio.h"
#include "sys/sys.h"

#include "debug.h"
#include "board_config.h"

// todo
//extern uint32_t APB_CLK;
//#define PLL_CLK (APB_CLK * 2)

// todo : should be define by board
#define CSI_POS CSI_POS_HS_PE12_VS_PE13_MK_PE10_PK_PE7_DATA_PE0_PE11

// todo
#define IRQ_CSI 14

#define R_CSI_MODE CSIMODE
#define R_ISPMODE ISPMODE

#define R_ISPCTRL ISPCTRL
#define R_CSI_INTEN CSI_INTEN
#define R_CSI_INT CSI_INTEN
#define R_CSI_INTCLR CSI_INTR // CSI_INTCLR
#define R_CSI_INTSTATUS CSI_INTR
#define R_CSI_SIZE CSI_IMAGE_SIZE
#define R_CSI_LDMA_STRIDE CSI_I2L_STRD

#define R_CSI_INPUT_ADDR CSI_DVP_BADR
#define R_CSI_TEST_SIZE CSI_TEST_SIZE
#define R_CSI_TEST_SYNC CSI_TEST_BLANK

#define R_CSI_MJPEG_LINE CSI_I2J_LBC
#define R_CSI_MJPEG_SIZE CSI_I2J_LBS
#define R_CSI_MJPEG_YADDR CSI_I2J_YBADR
#define R_CSI_MJPEG_UVADDR CSI_I2J_UVBADR

#define R_CSI_LCD_YADDR CSI_I2L_YBADR
#define R_CSI_LCD_UVADDR CSI_I2L_UVBADR
#define R_CSI_LCD_CROPH CSI_I2L_HCROP
#define R_CSI_LCD_CROPV CSI_I2L_VCROP

#define R_CSI_LCD_SCALER_WTABLE0 ISP_LS_HTAB0
#define R_CSI_LCD_SCALER_WTABLE1 ISP_LS_HTAB1
#define R_CSI_LCD_SCALER_WTABLE2 ISP_LS_HTAB2
#define R_CSI_LCD_SCALER_WTABLE3 ISP_LS_HTAB3
#define R_CSI_LCD_SCALER_WTABLE4 ISP_LS_HTAB4
#define R_CSI_LCD_SCALER_WTABLE5 ISP_LS_HTAB5
#define R_CSI_LCD_SCALER_WTABLE6 ISP_LS_HTAB6
#define R_CSI_LCD_SCALER_WTABLE7 ISP_LS_HTAB7
#define R_CSI_LCD_SCALER_WTABLE8 ISP_LS_HTAB8
#define R_CSI_LCD_SCALER_WTABLE9 ISP_LS_HTAB9
#define R_CSI_LCD_SCALER_WTABLE10 ISP_LS_HTAB10
#define R_CSI_LCD_SCALER_WTABLE11 ISP_LS_HTAB11
#define R_CSI_LCD_SCALER_WTABLE12 ISP_LS_HTAB12
#define R_CSI_LCD_SCALER_WTABLE13 ISP_LS_HTAB13
#define R_CSI_LCD_SCALER_WTABLE14 ISP_LS_HTAB14
#define R_CSI_LCD_SCALER_WTABLE15 ISP_LS_HTAB15

#define R_CSI_LCD_SCALER_HTABLE0 ISP_LS_VTAB0
#define R_CSI_LCD_SCALER_HTABLE1 ISP_LS_VTAB1
#define R_CSI_LCD_SCALER_HTABLE2 ISP_LS_VTAB2
#define R_CSI_LCD_SCALER_HTABLE3 ISP_LS_VTAB3
#define R_CSI_LCD_SCALER_HTABLE4 ISP_LS_VTAB4
#define R_CSI_LCD_SCALER_HTABLE5 ISP_LS_VTAB5
#define R_CSI_LCD_SCALER_HTABLE6 ISP_LS_VTAB6
#define R_CSI_LCD_SCALER_HTABLE7 ISP_LS_VTAB7

#define R_CSI_DMA_MODE CSI_DMA_MODE

#define R_CSI_SCALER_SIZE ISP_LS_SIZE
#define R_CSI_SCALER_WSTEP ISP_LS_HSTP
#define R_CSI_SCALER_HSTEP ISP_LS_VSTP

#define R_MD_CTRL ISP_MD_PARA0
#define R_MD_XPOS ISP_MD_PARA1
#define R_MD_YPOS ISP_MD_PARA2

// void csi_printf()
//{
//	printf("CSIMODE:%x\nISPMODE:%x\nISPCTRL:%x\nCSI_INTEN:%x\nCSI_IMAGE_SIZE:%x\nCSI_I2L_STRD:%x\nCSI_DVP_BADR:%x\nCSI_DMA_MODE:%x\n",
//			CSIMODE,ISPMODE,ISPCTRL,CSI_INTEN,CSI_IMAGE_SIZE,CSI_I2L_STRD,CSI_DVP_BADR,CSI_DMA_MODE);
// }

volatile u32 csifcnt;
u32 hal_csi_fcnt_get(void)
{
	u32 cnt = csifcnt;
	csifcnt = 0;
	return cnt;
}

static u32 csi_errcnt;
static void (*ax32xxCSIISR[CSI_IRQ_MAX])(void);
void ax32xx_csiIRQHandler(void)
{
	volatile u32 intrcsi = R_CSI_INTSTATUS;
	int i;
	if (intrcsi & BIT(3))
	{
		csifcnt++;
	}
	for (i = 0; i < CSI_IRQ_ISP; i++)
	{
		if (intrcsi & (1 << i))
		{
			R_CSI_INTCLR = 1 << i;

			if (ax32xxCSIISR[i])
				ax32xxCSIISR[i]();
			csi_errcnt = 0;
		}
	}
	if (intrcsi & (0xf << 12))
	{
		R_CSI_INTCLR = 0xf << 12;

		if (ax32xxCSIISR[CSI_IRQ_ISP])
			ax32xxCSIISR[CSI_IRQ_ISP]();
		csi_errcnt = 0;
	}

	u32 error_mask = (7 << 16) | (1 << 11) | (1 << 9) | (7 << 4);
	if (intrcsi & error_mask)
	{
		// BIT(17): JE_LINE_BUFF_ERR
		// BIT(16): JE_CSI_ERR
		// BIT(11): SYNC_FIFO_ERR
		// BIT(9):  SACL_INTLBUFF_ERR
		// BIT(6):  LCD_INTLBUF_ERR
		// BIT(5):  MJP_JINTLBUFF_ERR
		// BIT(4):  SENSOR_SIZE_ERR
		debg("ax32xx: <CSI_ERR:%x>\n", intrcsi & error_mask);

		if (csi_errcnt++ > 100)
		{ // err cnt > 100 frame , must reset by WDT
			// csi_errcnt;
			LogPrint("csi err", 0);
			exception_trigger();
			while (1)
				;
		}
		if (intrcsi & BIT(4))
		{
			if (R_CSI_DMA_MODE & (1 << 24))
			{
				R_CSI_DMA_MODE &= ~(1 << 24); // ldma dis
				ax32xx_pipCounterReset();
				R_CSI_DMA_MODE |= (1 << 24); // ldma en
			}
		}
		if (intrcsi & BIT(6))
		{
			R_CSI_INTEN &= ~BIT(6);
			CSI_DMA_MODE &= ~BIT(24);

			R_CSI_INTCLR = BIT(6);
			ax32xx_pipCounterReset();

			CSI_DMA_MODE |= BIT(24);
			R_CSI_INTEN |= BIT(6);
		}
		if (intrcsi & BIT(5))
		{
			ax32xx_mjpegEncodeEnable(0);
			void hal_mjpegStart(void);
			ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT, hal_mjpegStart);
		}
	}
	R_CSI_INTCLR = intrcsi;
}
/*******************************************************************************
* Function Name  : ax32xx_csiISRRegiser
* Description    : csi isr register
* Input          :  u8 type : irq type.CSI_IRQ_E
					   void (*isr)(void) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiISRRegiser(u8 type, void (*isr)(void))
{
	printf("");
	if (type < CSI_IRQ_MAX)
		ax32xxCSIISR[type] = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_csiMJPEGFrameSet
* Description    : csi frame set for mjpeg
* Input          :  u32 mjpeg_y : y pixel address
					   u32 mjpeg_uv : uv pixel address
					   u32 lines     : lines
					   u32 width   : width
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiMJPEGFrameSet(u32 mjpeg_y, u32 mjpeg_uv, u32 lines, u32 width)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_MJPEG_LINE = lines & 0x7ff;
	R_CSI_MJPEG_SIZE = ((width + 0x1f) / 32) * 32 * lines;
	R_CSI_MJPEG_YADDR = mjpeg_y;
	R_CSI_MJPEG_UVADDR = mjpeg_uv;

	R_CSI_DMA_MODE &= ~CSI_OUTPUT_UVCEN;
	// R_CSI_DMA_MODE &= ~(7<<28);
	// R_CSI_DMA_MODE |= (CSI_BURST_32BYTE<<28);
	//R_CSI_DMA_MODE |= BIT(19);

	R_CSI_INTEN |= (1 << CSI_INT_EN) | (1 << CSI_JE_LB_OVERFLOW) | (1 << CSI_JE_CSI_ERR) | (1 << CSI_ASYC_FIRO_ERR) | (1 << CSI_JBUFF_ERR) | (1 << CSI_SEN_SIZE_ERR) | (1 << CSI_SEN_SIZE_STAT_INT) | (1 << CSI_JFRA_END_INT) | (1 << CSI_DVP_MEM_EMPTY) | (1 << CSI_STAT_CP_DONE) | (1 << CSI_WP_DET_INT) | (1 << CSI_AE_HIDT_INT);
	arch_local_irq_restore(flag);
	//  printf ("CSI_DMA_MODE=%X CSI_INTEN=%X CSIMODE2=%X\n", CSI_DMA_MODE, CSI_INTEN, CSIMODE2);
}
void ax32xx_csihv_tomjp_dma_cfg(u8 *Jy_buf, u8 *juv_buf, u16 line, u16 w)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	// CFG YUV cache
	CSI_I2W_LBC = line;
	CSI_I2W_LBS = (((w + 0x1f) / 32) * 32 * line); //__IMG_H__必须跟32B对其
	CSI_I2W_YBADR = (u32)Jy_buf;				   // jyuvbuf.y_buf;
	CSI_I2W_UVBADR = (u32)juv_buf;				   // jyuvbuf.uv_buf;
	//	debg("yadr:%x, uvadr: %x  line:%x\n",CSI_I2W_LBC,CSI_I2W_LBS,line);

	//
	CSI_DMA_MODE &= ~BIT(16); // uvc disable
	CSI_DMA_MODE &= ~(7 << 28);
	// CSI_DMA_MODE |= (CSI_BURST_32BYTE << 28)		//burst
	//|(1 << 31)
	; // csi jdma en
	// DMA EN
	CSI_INTEN |= BIT(31)   //_CSI_INT_EN_
				 | BIT(18) //_CSI_LBUF_INT_EN_
				 | BIT(17) //_CSI_ERR_INT_EN_
				 | BIT(11) // ASYC_FIFO_ERR_EN
				 //|BIT(6)//_LCDBUF_ER_INT_EN_
				 | BIT(9) //_J1BUF_ER_INT_EN_
				 | BIT(8) //_J1FRA_DOWN
				 | BIT(5) //_JBUF_ER_INT_EN_
				 | BIT(4) //_SENSER_SIZE_ER
				 | BIT(3) //_SENSER_RESOLIVING_TEST_EN
				 | BIT(0) //_JFRAME_END_INT_EN_
		;
	arch_local_irq_restore(flag);
	// debg("ladr:%x,%x\n",CSI_J1YDMA_ADR,CSI_J1UVDMA_ADR);
	// debg("CSI_JPACKET_SIZE:%x\n",CSI_JPACKET_SIZE);
	// printf ("CSI_DMA_MODE=%X CSI_INTEN=%X CSIMODE2=%X\n", CSI_DMA_MODE, CSI_INTEN, CSIMODE2);
}
void ax32xx_csi_only_FrameSet()
{
	u32 temp = 0;
	temp |= (1 << CSI_INT_EN) | (1 << CSI_JE_CSI_ERR) | (1 << CSI_ASYC_FIRO_ERR) | (1 << CSI_SEN_SIZE_ERR) | (1 << CSI_SEN_SIZE_STAT_INT) | (1 << CSI_DVP_MEM_EMPTY) | (1 << CSI_STAT_CP_DONE) | (1 << CSI_WP_DET_INT) | (1 << CSI_AE_HIDT_INT)

		;
	R_CSI_INTEN = temp;
}

/*******************************************************************************
* Function Name  : ax32xx_csiYUVFrameSet
* Description    : csi yuv buffer set for uvc
* Input          :  u32 yaddr0 : y pixel address 0
					   u32 yaddr1 : y pixel address 1
					   u32 size     : frame size
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiYUVFrameSet(u32 yaddr0, u32 yaddr1, u32 size)
{
#if 0
    R_CSI_LCD_YADDR = yaddr0;
	
	R_CSI_DMA_MODE &= ~(7 << 28);
	R_CSI_DMA_MODE &= ~(0x3fff << 0); //size clr
	
	R_CSI_DMA_MODE = (7 << 21)		//lcd burst
				|BIT(16)//_UVC_EN_
				|BIT(24)//_CSI_LDMA_EN_
				|BIT(17)//_CSI_LADR_RELOAD_
				|(size << 0 )
				;	
    R_CSI_LCD_YADDR = yaddr1;	
	
	R_CSI_INTEN = (1<<31)//_CSI_INT_EN_
				|(1<<17)//_CSI_LBUF_INT_EN_ 	
				|(1<<16)//_CSI_ERR_INT_EN_ 
				|(1<<11)//ASYC_FIFO_ERR_EN
				|(1<<6)//_LCDBUF_ER_INT_EN_		
				//|(1<<5)//_JBUF_ER_INT_EN_
				|(1<<4)//_SENSER_SIZE_ER
				|(1<<3)//_SENSER_RESOLIVING_TEST_EN
				|(1<<2)//_UVC_PACKET_INT_EN_
				|(1<<0)//_JFRAME_END_INT_EN_
				;
#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_csiIOConfig
 * Description    : csi io config set
 * Input          :  none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiIOConfig(void)
{
	ax32xx_gpioSFRSet(GPIO_MAP_CSI, CSI_POS);
#if CSI_POS == CSI_POS_HS_PE12_VS_PE13_MK_PE10_PK_PE7_DATA_PE0_PE11
	ax32xx_gpioDirSet(GPIO_PE, GPIO_PIN10, GPIO_OUTPUT); // mCLK
	ax32xx_gpioDirSet(GPIO_PE, /*GPIO_PIN0|GPIO_PIN1|*/ GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN14, GPIO_INPUT);
	ax32xx_gpioMapSet(GPIO_PE, /*GPIO_PIN0|GPIO_PIN1|*/ GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN14, GPIO_FUNC_SFR);
#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_csiInit
 * Description    : csi initial
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void Pcon_printf();

void ax32xx_csiInit(void)
{
	ax32xx_csiIOConfig();

	// CLKCON0 &= ~((1<<24) | (1<<25));//select  pclk form sensor

	ax32xx_sysClkSet(SYS_CLK_DVP_CSI, 1);

#if WIFI_SUPPORT_MODE == FLIGHT_MODE

	static u32 configed = 0;
	if (!configed)
	{
		configed = 1;
		R_CSI_LCD_SCALER_WTABLE0 = 0x2113fefe;
		R_CSI_LCD_SCALER_WTABLE1 = 0x1fefe13;
		R_CSI_LCD_SCALER_WTABLE2 = 0x2010fdff;
		R_CSI_LCD_SCALER_WTABLE3 = 0x1fdff17;
		R_CSI_LCD_SCALER_WTABLE4 = 0x220cfcff;
		R_CSI_LCD_SCALER_WTABLE5 = 0x1fc001a;
		R_CSI_LCD_SCALER_WTABLE6 = 0x1f09fc00;
		R_CSI_LCD_SCALER_WTABLE7 = 0xfc031d;
		R_CSI_LCD_SCALER_WTABLE8 = 0x2006fc00;
		R_CSI_LCD_SCALER_WTABLE9 = 0xfc061c;
		R_CSI_LCD_SCALER_WTABLE10 = 0x1d03fc00;
		R_CSI_LCD_SCALER_WTABLE11 = 0xfc091f;
		R_CSI_LCD_SCALER_WTABLE12 = 0x1a00fc01;
		R_CSI_LCD_SCALER_WTABLE13 = 0xfffc0c22;
		R_CSI_LCD_SCALER_WTABLE14 = 0x17fffd01;
		R_CSI_LCD_SCALER_WTABLE15 = 0xfffd1020;

		R_CSI_LCD_SCALER_HTABLE0 = 0x2113fefe;
		R_CSI_LCD_SCALER_HTABLE1 = 0x1fefe13;
		R_CSI_LCD_SCALER_HTABLE2 = 0x2010fdff;
		R_CSI_LCD_SCALER_HTABLE3 = 0x1fdff17;
		R_CSI_LCD_SCALER_HTABLE4 = 0x220cfcff;
		R_CSI_LCD_SCALER_HTABLE5 = 0x1fc001a;
		R_CSI_LCD_SCALER_HTABLE6 = 0x1f09fc00;
		R_CSI_LCD_SCALER_HTABLE7 = 0xfc031d;
	}

#endif
}
/*******************************************************************************
 * Function Name  : ax32xx_csiMclkSet
 * Description    : csi mclk set
 * Input          : u32 clk : clock
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiMclkSet(u32 clk)
{
	REG32(CLKCON2) &= ~(1 << 26);

	REG32(CLKCON2) |= (1 << 26);

	ax32xx_gpioSFRSet(GPIO_MAP_CSI, CSI_POS);
	ax32xx_gpioMapSet(GPIO_PE, GPIO_PIN10, GPIO_FUNC_SFR); // set mclk output
	ax32xx_gpioDirSet(GPIO_PE, GPIO_PIN10, GPIO_OUTPUT);

#if 0
    //USB PHY PLL
	USBCON0 |= 0xc0;//usb pll enable
	CLKCON0 |= BIT(5);
	
	CLKCON1 &= 0xffffe0ff ;
	CLKCON1 |= (240000000/clk - 1) << 8;  //senclk =pll/(div+1)
#else
	// SPLL
	CLKCON0 &= ~BIT(5);
	CLKCON1 &= 0xffffe0ff;
	CLKCON1 |= (PLL_CLK / clk - 1) << 8; // senclk =pll/(div+1)
#endif
}
/*******************************************************************************
* Function Name  : ax32xx_csiSyncSet
* Description    : csi sync set
* Input          : u8 hsync_en : hsync
					  u8 vsync_en : vsync
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSyncSet(u8 hsync_en, u8 vsync_en)
{
	R_CSI_MODE &= ~(CSI_MODE_VSYNC | CSI_MODE_HSYNC);
	if (hsync_en)
	{
		R_CSI_MODE |= CSI_MODE_HSYNC;
	}
	if (vsync_en)
	{
		R_CSI_MODE |= CSI_MODE_VSYNC;
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_csiPrioritySet
 * Description    : csi priority set
 * Input          : u8 type : type .CSI_POLARITY_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiPrioritySet(u8 plo)
{
	R_CSI_MODE &= ~(CSI_MODE_PRIORITY);
	R_CSI_MODE |= (plo & 0x03) * (CSI_MODE_PRIORITY / 3);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiTypeSet
 * Description    : csi type set
 * Input          : u8 type : type .CSI_TYPE_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiTypeSet(u8 type)
{
	R_CSI_MODE &= ~CSI_MODE_FORMAT;	 // 0xc0   数据格式
	R_CSI_MODE &= ~CSI_MODE_DATA_BW; // c00	数据宽度
	if (type < 3)
	{
		R_CSI_MODE |= type * (CSI_MODE_DATA_BW / 3);
	}
	else
	{
		R_CSI_MODE |= ((type - 1) & 0x3) * (CSI_MODE_FORMAT / 3);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiModeSet
* Description    : csi mode set
* Input          : u32 mode : mode flag
					  u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiModeSet(u32 mode, u8 en)
{
	if (en)
		R_CSI_MODE |= mode;
	else
		R_CSI_MODE &= ~mode;
}
/*******************************************************************************
 * Function Name  : ax32xx_pclk_fir_Set
 * Description    : pclk filter set
 * Input          : u8 en : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_pclk_fir_Set(u8 en)
{
	if (en)
	{
		CLKCON0 |= BIT(26); // DVP filter use DDRPLL
#if 0
		if((USB20CON0 &0xc0)!=0xc0){
			debg("USB PLL is not enable!!!");
			USB20CON0 |= 0x80;//osc enable
			USB20CON0 |= 0x40;//pll enable
			debg("Enable USB PLL for PCLK Filter!!!");
		}
#else
		// usb20_phyreset_as_filter();
#endif
		R_CSI_MODE |= en << 26;
	}
	else
		R_CSI_MODE &= ~(0xf << 26);
}
/*******************************************************************************
 * Function Name  : ax32xx_pclk_analog_Set
 * Description	 : pclk filter set
 * Input 		 : u8 en : step
 * Output		 : none
 * Return		 : none
 *******************************************************************************/
void ax32xx_pclk_analog_Set(u8 en)
{
	if (en)
	{
		R_CSI_MODE |= en << 22;
	}
	else
		R_CSI_MODE &= ~(0xf << 22);
}
/*******************************************************************************
 * Function Name  : ax32xx_pclk_inv_Set
 * Description    : pclk filter set
 * Input          : u8 en : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_pclk_inv_Set(u8 en)
{
	if (en)
	{
		R_CSI_MODE |= CSI_MODE_PCLK_INV;
	}
	else
		R_CSI_MODE &= ~CSI_MODE_PCLK_INV;
}
/*******************************************************************************
 * Function Name  : ax32xx_csi_clk_tun_Set
 * Description    : pclk tun set
 * Input          : u8 tun : tun val
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csi_clk_tun_Set(u8 tun)
{
	if (tun)
	{
		CLKTUN |= (1 << 31) | ((tun & 0x1f) << 26);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiMode2Set
* Description    : csi mode2 set
* Input          : u32 mode : mode flag
					  u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
// void ax32xx_csiMode2Set(u32 mode,u8 en)
//{
//	if(en)
//		R_CSI_MODE2 |= mode;
//	else
//		R_CSI_MODE2 &= ~mode;
//	//R_CSI_MODE2 |= BIT(7);
// }
/*******************************************************************************
* Function Name  : ax32xx_csiSizeSet
* Description    : csi size set
* Input          : u16 width : width
					  u16 height: height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSizeSet(u16 width, u16 height)
{
	R_CSI_SIZE = ((height - 1) << 16) | (width - 1);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiInputAddrSet
 * Description    : csi input from sdram addr set
 * Input          : u32 addr : input address
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiInputAddrSet(u32 addr)
{
	// R_CSI_INPUT_ADDR = addr;
	R_CSI_INPUT_ADDR = addr;
}
/*******************************************************************************
* Function Name  : ax32xx_csiTestModeSet
* Description    : csi test mode size set
* Input          : u16 width : width
					  u16 height: height
					  u16 hsync : hsync
					  u16 vsync : vsync
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiTestModeSet(u16 width, u16 height, u16 hsync, u16 vsync)
{
	R_CSI_MODE &= ~CSI_MODE_TEST;
	R_CSI_MODE |= (8 & 0xf) * (CSI_MODE_TEST / 15);
	R_CSI_TEST_SIZE = (height << 16) + width;
	R_CSI_TEST_SYNC = (vsync << 16) + hsync;
}
/*******************************************************************************
* Function Name  : ax32xx_csiLCDFrameSet
* Description    : csi set output frame addr.if any of the r address is NULL,do not care it.
* Input          : u32 yaddr : y pixel buffer addr
					  u32 uvaddr: uv pixel buffer addr.
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiDvpClkDivSet(u16 clk_div_mode)
{
	R_CSI_MODE &= ~CSI_MODE_DVP_PCLK_DIV;
	R_CSI_MODE |= (clk_div_mode & 0x3) * (CSI_MODE_DVP_PCLK_DIV / 3);
}

void ax32xx_csiLCDFrameSet(u32 yaddr, u32 uvaddr)
{
	//    printf("I2L:%x\n",yaddr);
	if (yaddr)
		R_CSI_LCD_YADDR = yaddr;
	if (uvaddr)
		R_CSI_LCD_UVADDR = uvaddr;
}
/*******************************************************************************
 * Function Name  : ax32xx_csiLCDDmaEnable
 * Description    : csi output for lcd control enable
 * Input          : u8 en : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiLCDDmaEnable(u8 en)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	if (en)
	{
		R_CSI_INTCLR = (1 << CSI_LBUFF_ERR) |		  // lbuff err
					   (1 << CSI_SEN_SIZE_ERR) |	  //
					   (1 << CSI_SEN_SIZE_STAT_INT) | //
					   (1 << CSI_LFRA_END_INT);		  // frame done
		R_CSI_INTEN |= (1 << CSI_INT_EN) |			  // int en
					   (1 << CSI_LINE_SCALER_ERR) |	  // line scaler err int en
					   (1 << CSI_LBUFF_ERR) |		  // lbuff err
					   (1 << CSI_SEN_SIZE_ERR) |	  //
					   (1 << CSI_SEN_SIZE_STAT_INT) | //
					   (1 << CSI_LFRA_END_INT);		  // frame done
		R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;			  // ldma en
	}
	else
	{
		R_CSI_DMA_MODE &= ~CSI_OUTPUT_LCDEN; // ldma en

		R_CSI_INTEN &= ~((1 << CSI_LINE_SCALER_ERR) |	// line scaler err int en
						 (1 << CSI_LBUFF_ERR) |			// lbuff err int en
						 (0 << CSI_SEN_SIZE_ERR) |		// sensor output size err int en
						 (0 << CSI_SEN_SIZE_STAT_INT) | // sensor output size statistics int en
						 (1 << CSI_LFRA_END_INT));		// frame done
		R_CSI_INTCLR = (1 << CSI_LINE_SCALER_ERR) |		// line scaler err pending
					   (1 << CSI_LBUFF_ERR) |			// lbuff err
					   (1 << CSI_SEN_SIZE_ERR) |		//
					   (1 << CSI_SEN_SIZE_STAT_INT) |	//
					   (1 << CSI_LFRA_END_INT);			// frame done
	}
	arch_local_irq_restore(flag);
}

void ax32xx_csiLCDDmaKick(void)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	if (R_CSI_DMA_MODE & CSI_OUTPUT_LDMAMANEN)
	{
		R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;
	}
	arch_local_irq_restore(flag);
}

/*******************************************************************************
* Function Name  : ax32xx_csiLCDScaler
* Description    : csi scaler for lcd
* Input          : u16 src_w : source width
				   u16 src_h : source height
				   u16 tar_w : target width
				   u16 tar_h : target height
				   u16 sx    : crop start x position
				   u16 sy    : crop start y position
				   u16 ex    : crop end x position
				   u16 ey    : crop end y position
				   u16 stride: warning:stride >= width
				   u32 * anti_lsawtooth   anti_lsawtooth para
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDScaler(u16 src_w, u16 src_h, u16 tar_w, u16 tar_h, u16 sx, u16 sy, u16 ex, u16 ey, u16 stride, u32 *anti_lsawtooth)
{
	//if (((ex - sx) < tar_w) || ((ey - sy) < tar_h)) // scaler can not do scaler up
	//	return;
	R_CSI_LCD_CROPH = (ex << 16) | sx;
	R_CSI_LCD_CROPV = (ey << 16) | sy;
	//printf("ax32xx_csiLCDScaler:======%x %x\n", (ex << 16) | sx, (ey << 16) | sy);

	ax32xx_ispModeSet(ISP_MODE_LCDCROP, 1);

	R_CSI_SCALER_SIZE = (tar_h << 16) | tar_w;
	R_CSI_SCALER_WSTEP = ((ex - sx) << 16) / (tar_w >> 1);
	R_CSI_SCALER_HSTEP = ((ey - sy) << 16) / (tar_h >> 1);

	static u32 configed = 0;
	if (!configed)
	{
		R_CSI_LCD_SCALER_WTABLE0 = anti_lsawtooth[0];
		R_CSI_LCD_SCALER_WTABLE1 = anti_lsawtooth[1];
		R_CSI_LCD_SCALER_WTABLE2 = anti_lsawtooth[2];
		R_CSI_LCD_SCALER_WTABLE3 = anti_lsawtooth[3];
		R_CSI_LCD_SCALER_WTABLE4 = anti_lsawtooth[4];
		R_CSI_LCD_SCALER_WTABLE5 = anti_lsawtooth[5];
		R_CSI_LCD_SCALER_WTABLE6 = anti_lsawtooth[6];
		R_CSI_LCD_SCALER_WTABLE7 = anti_lsawtooth[7];
		R_CSI_LCD_SCALER_WTABLE8 = anti_lsawtooth[8];
		R_CSI_LCD_SCALER_WTABLE9 = anti_lsawtooth[9];
		R_CSI_LCD_SCALER_WTABLE10 = anti_lsawtooth[10];
		R_CSI_LCD_SCALER_WTABLE11 = anti_lsawtooth[11];
		R_CSI_LCD_SCALER_WTABLE12 = anti_lsawtooth[12];
		R_CSI_LCD_SCALER_WTABLE13 = anti_lsawtooth[13];
		R_CSI_LCD_SCALER_WTABLE14 = anti_lsawtooth[14];
		R_CSI_LCD_SCALER_WTABLE15 = anti_lsawtooth[15]; //*/

		R_CSI_LCD_SCALER_HTABLE0 = anti_lsawtooth[16];
		R_CSI_LCD_SCALER_HTABLE1 = anti_lsawtooth[17];
		R_CSI_LCD_SCALER_HTABLE2 = anti_lsawtooth[18];
		R_CSI_LCD_SCALER_HTABLE3 = anti_lsawtooth[19];
		R_CSI_LCD_SCALER_HTABLE4 = anti_lsawtooth[20];
		R_CSI_LCD_SCALER_HTABLE5 = anti_lsawtooth[21];
		R_CSI_LCD_SCALER_HTABLE6 = anti_lsawtooth[22];
		R_CSI_LCD_SCALER_HTABLE7 = anti_lsawtooth[23];
		configed = 1;
	}

	R_CSI_LDMA_STRIDE = stride;

	//	R_CSI_SCALER_CTRL = (1<<2)|1;	// ls_en , ls_ks
	ax32xx_ispModeSet(ISP_MODE_LSEN, 1);
	R_ISPCTRL |= ISP_CTRL_LS_KS;
}
void csiLcdAnti_lsawtoothInit(void)
{
#define HBIC_TAB0    0xff043904
#define HBIC_TAB1    0xfc0c3800
#define HBIC_TAB2    0xfa1536fb
#define HBIC_TAB3    0xf91f2ff9
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf92f1ff9
#define HBIC_TAB6    0xfb3615fa
#define HBIC_TAB7    0x00380cfc
#define WBIC_TAB0    0x2a10fa01,0x0001fa10
#define WBIC_TAB1    0x280bfb01,0x0100fb15
#define WBIC_TAB2    0x2706fb01,0x0100fc1a
#define WBIC_TAB3    0x2502fc01,0x01fffd1f
#define WBIC_TAB4    0x2300fe01,0x01fe001f
#define WBIC_TAB5    0x1ffdff01,0x01fc0225
#define WBIC_TAB6    0x1afc0001,0x01fb0627
#define WBIC_TAB7    0x15fb0001,0x01fb0b28

#define WBIC_TAB   WBIC_TAB0,WBIC_TAB1,WBIC_TAB2,WBIC_TAB3,WBIC_TAB4,WBIC_TAB5,WBIC_TAB6,WBIC_TAB7
#define HBIC_TAB   HBIC_TAB0,HBIC_TAB1,HBIC_TAB2,HBIC_TAB3,HBIC_TAB4,HBIC_TAB5,HBIC_TAB6,HBIC_TAB7
	static u32 anti_lsawtooth[24]={WBIC_TAB,HBIC_TAB};
	static u32 configed = 0;
	if(!configed)
	{
		R_CSI_LCD_SCALER_WTABLE0 = anti_lsawtooth[0];
		R_CSI_LCD_SCALER_WTABLE1 = anti_lsawtooth[1];
		R_CSI_LCD_SCALER_WTABLE2 = anti_lsawtooth[2];
		R_CSI_LCD_SCALER_WTABLE3 = anti_lsawtooth[3];
		R_CSI_LCD_SCALER_WTABLE4 = anti_lsawtooth[4];
		R_CSI_LCD_SCALER_WTABLE5 = anti_lsawtooth[5];
		R_CSI_LCD_SCALER_WTABLE6 = anti_lsawtooth[6];
		R_CSI_LCD_SCALER_WTABLE7 = anti_lsawtooth[7];
		R_CSI_LCD_SCALER_WTABLE8 = anti_lsawtooth[8];
		R_CSI_LCD_SCALER_WTABLE9 = anti_lsawtooth[9];
		R_CSI_LCD_SCALER_WTABLE10= anti_lsawtooth[10];
		R_CSI_LCD_SCALER_WTABLE11= anti_lsawtooth[11];
		R_CSI_LCD_SCALER_WTABLE12= anti_lsawtooth[12];
		R_CSI_LCD_SCALER_WTABLE13= anti_lsawtooth[13];
		R_CSI_LCD_SCALER_WTABLE14= anti_lsawtooth[14];
		R_CSI_LCD_SCALER_WTABLE15= anti_lsawtooth[15];//*/
				
		R_CSI_LCD_SCALER_HTABLE0 = anti_lsawtooth[16];
		R_CSI_LCD_SCALER_HTABLE1 = anti_lsawtooth[17];
		R_CSI_LCD_SCALER_HTABLE2 = anti_lsawtooth[18];
		R_CSI_LCD_SCALER_HTABLE3 = anti_lsawtooth[19];
		R_CSI_LCD_SCALER_HTABLE4 = anti_lsawtooth[20];
		R_CSI_LCD_SCALER_HTABLE5 = anti_lsawtooth[21];
		R_CSI_LCD_SCALER_HTABLE6 = anti_lsawtooth[22];
		R_CSI_LCD_SCALER_HTABLE7 = anti_lsawtooth[23];
		configed = 1;
	}

}
void ax32xx_csiLCDScaler1(u16 src_w, u16 src_h, u16 tar_w, u16 tar_h, u16 sx, u16 sy, u16 ex, u16 ey, u16 stride)
{
	if (((ex - sx) < tar_w) || ((ey - sy) < tar_h)) // scaler can not do scaler up
		return;
	R_CSI_LCD_CROPH = (ex << 16) | sx;
	R_CSI_LCD_CROPV = (ey << 16) | sy;
	//printf("ax32xx_csiLCDScaler:======%x %x\n", (ex << 16) | sx, (ey << 16) | sy);
	ax32xx_ispModeSet(ISP_MODE_LCDCROP, 1);
	R_CSI_SCALER_SIZE = (tar_h << 16) | tar_w;
	R_CSI_SCALER_WSTEP = ((ex - sx) << 16) / (tar_w >> 1);
	R_CSI_SCALER_HSTEP = ((ey - sy) << 16) / (tar_h >> 1);
	R_CSI_LDMA_STRIDE = stride;
	//	R_CSI_SCALER_CTRL = (1<<2)|1;	// ls_en , ls_ks
	ax32xx_ispModeSet(ISP_MODE_LSEN, 1);
	R_ISPCTRL |= ISP_CTRL_LS_KS;
}

/*******************************************************************************
* Function Name  : ax32xx_csiScaler
* Description    : csi scaler down set for in line scaler
* Input          : u16 src_w : source width
					  u16 src_h : source height
					  u16 tar_w : target width
					  u16 tar_h : target height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiScaler(u16 src_w, u16 src_h, u16 tar_w, u16 tar_h, u16 sx, u16 sy, u16 ex, u16 ey)
{
	// R_CSI_DMA_MODE &= ~CSI_OUTPUT_MJPGEN;//关闭hv sacle
	// ISPMODE&= ~BIT(18); //SD_EN , wcrop_en
	
	ISPMODE &= ~ISP_MODE_JSCALER_SEL;
	// R_CSI_DMA_MODE &= ~CSI_OUTPUT_WIFIEN;//关闭hv sacle
	if ((tar_w < src_w) || (tar_h < src_h))
	{
		if ((sx > src_w) || (ex > src_w))
		{
			return;
		}
		if ((sy > src_h) || (ey > src_h))
		{
			return;
		}
		#if 0
		R_CSI_LCD_CROPH = (ex << 16) | sx;
		R_CSI_LCD_CROPV = (ey << 16) | sy;

		R_CSI_SCALER_SIZE = (tar_h << 16) | tar_w;
		R_CSI_SCALER_WSTEP = (src_w << 16) / (tar_w >> 1);
		R_CSI_SCALER_HSTEP = (src_h << 16) / (tar_h >> 1);
		//		R_CSI_SCALER_CTRL = (1<<2)|1;
		R_ISPMODE |= ISP_MODE_LSEN | ISP_MODE_JSCALER_SEL;
		R_ISPCTRL |= ISP_CTRL_LS_KS;
		#endif
		CSI_I2J_HCROP = (ex<<16)|sx;
		CSI_I2J_VCROP = (ey<<16)|sy;
		//R_CSI_SCALER_SIZE = (tar_h<<16)|tar_w;
		//R_CSI_SCALER_WSTEP = (src_w<<16)/(tar_w>>1);
		//R_CSI_SCALER_HSTEP = (src_h<<16)/(tar_h>>1);
		ax32xx_ispModeSet(ISP_MODE_JPEGCROP,1);
		// R_CSI_DMA_MODE |= CSI_OUTPUT_MJPGEN;
	}
}
void ax32xx_csi2JScalerDis(void)
{
	R_ISPMODE &= ~ISP_MODE_JSCALER_SEL;
	R_ISPMODE &= ~ISP_MODE_JPEGCROP;
}
bool ax32xx_hvScaler(u32 src_w, u32 src_h, u32 obj_w, u32 obj_h)
{
	u8 wsacle, hsacle;
	if ((src_w < obj_w) || (src_h < obj_h))
		return false;

	if ((src_w / obj_w) == 2)
	{ // w:2:1
		wsacle = 1;
	}
	else if ((src_w / obj_w) == 4)
	{ // w:4:1
		wsacle = 3;
	}
	else
	{
		return false;
	}

	if ((src_h / obj_h) == 2)
	{ // h:2:1
		hsacle = 1;
	}
	else if ((src_h / obj_h) == 4)
	{ // h:4:1
		hsacle = 3;
	}
	else if (((src_h * 2 / obj_h) == 3) && ((obj_h % 3) == 0))
	{ // h:3:2
		hsacle = 0;
	}
	else if (((src_h / obj_h) == 3) && ((obj_h % 3) == 0))
	{ // h:3:1
		hsacle = 2;
	}
	else
	{
		return false;
	}
	printf("wsacle,hsacle:%x,%x", wsacle, hsacle);
	{
	unsigned long flag;
	flag=arch_local_irq_save();
		R_CSI_DMA_MODE &= ~CSI_OUTPUT_MJPGEN; // 关闭hv sacle
		// CSI_I2W_HCROP &= ~(3<<11);
		// CSI_I2W_VCROP &= ~(3<<10);

		CSI_I2W_HCROP = (obj_w << 16)			  // end
						| ((wsacle & 0x03) << 11) // Horizontal Scale-Down steps
						| (0 << 0);				  // start

		CSI_I2W_VCROP = (obj_h << 16)			  // end
						| ((hsacle & 0x03) << 10) // Vertical Scale-Down steps
						| (0 << 0);				  // start
		ISPMODE |= BIT(19) /*|BIT(18)*/;		  // SD_EN , wcrop_en

		;
		R_CSI_DMA_MODE |= CSI_OUTPUT_WIFIEN; // 打开hv sacle
		arch_local_irq_restore(flag);
		debg("csi_hv_sacle... \n");
	}
	return true;
}
/*******************************************************************************
 * Function Name  : ax32xx_csiScalerCheck
 * Description    : csi sclaer to lcd check
 * Input          : none
 * Output         : none
 * Return         : 1 : scaler done
 *******************************************************************************/
s32 ax32xx_csiScalerCheck(void)
{
	if (R_ISPCTRL & ISP_CTRL_LS_KS)
		return 0;
	else
		return 1;
}
/*******************************************************************************
* Function Name  : ax32xx_csiOutputSet
* Description    : csi output mode set
* Input          : u32 mode : mode.CSI_OUTPUT_E
					  u8 en      : 0-disable,1-enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiOutputSet(u32 mode, u8 en)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	if (en)
		R_CSI_DMA_MODE |= mode;
	else
		R_CSI_DMA_MODE &= ~mode;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiOutputMJPEGBurst
 * Description    : csi output burst set to mjpeg
 * Input          : u8 burst : burst type, CSI_BURST_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
// 66666666
void ax32xx_csiOutputMJPEGBurst(u8 burst)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_DMA_MODE &= ~(7 << 28);
	R_CSI_DMA_MODE |= (burst & 7) << 28;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiOutputMJPEGScaler
 * Description    : csi output scaler set to mjpeg
 * Input          : u8 scaler : scaler type, CSI_LCDSCALER_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiOutputMJPEGScaler(u8 scaler)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_DMA_MODE &= ~(3 << 25);
	R_CSI_DMA_MODE |= (scaler & 3) << 25;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiOutputUVCPrority
 * Description    : csi output priotiy set
 * Input          : u8 priority : priority type, CSI_POLARITY_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiOutputUVCPriority(u8 priority)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_DMA_MODE &= ~(3 << 14);
	R_CSI_DMA_MODE |= (priority & 3) << 14;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiOutputLCDScaler
 * Description    : csi output scaler for lcd
 * Input          : u8 scaler : scaler type, CSI_LCDSCALER_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiOutputLCDScaler(u8 scaler)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_DMA_MODE &= ~(3 << 18);
	R_CSI_DMA_MODE |= (scaler & 3) << 18;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiOutputLCDBurst
 * Description    : csi output burst set to lcd
 * Input          : u8 burst : burst type, CSI_BURST_E
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiOutputLCDBurst(u8 burst)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	R_CSI_DMA_MODE &= ~(7 << 21);
	R_CSI_DMA_MODE |= (burst & 7) << 21;
	arch_local_irq_restore(flag);
}
/*******************************************************************************
 * Function Name  : ax32xx_csiEnable
 * Description    : csi enable set
 * Input          : u8 en : 0-disable,1-enable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_csiEnable(u8 en)
{
	if (en)
	{
		R_CSI_INTEN |= (1 << CSI_INT_EN);
		R_CSI_MODE |= CSI_MODE_ENABLE;
		ax32xx_intEnable(IRQ_CSI, 1);
	}
	else
	{
		R_CSI_INTEN &= ~(1 << CSI_INT_EN);
		R_CSI_MODE &= ~CSI_MODE_ENABLE;
		ax32xx_intEnable(IRQ_CSI, 0);
	}
}

/*******************************************************************************
 * Function Name  : ax32xx_mdEnable
 * Description    : motion dection enable set
 * Input          :  u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : none
 *******************************************************************************/
void ax32xx_mdEnable(u8 en)
{
	if (en)
	{
		R_CSI_INT |= (1 << 7);
		R_MD_CTRL |= 1;
	}
	else
	{
		R_CSI_INT &= ~(1 << 7);
		R_MD_CTRL &= ~1;
	}
}

/*
void ax32xx_csiPrint(void)
{
	deg_Printf("R_CSI_MODE = %x\n",R_CSI_MODE);
	deg_Printf("R_CSI_INTEN = %x\n",R_CSI_INTEN);
	deg_Printf("R_CSI_INTCLR = %x\n",R_CSI_INTCLR);
	deg_Printf("R_CSI_INTSTATUS = %x\n",R_CSI_INTSTATUS);
	deg_Printf("R_CSI_SIZE = %x\n",R_CSI_SIZE);
	deg_Printf("R_CSI_MODE2 = %x\n",R_CSI_MODE2);
	deg_Printf("R_CSI_INPUT_ADDR = %x\n",R_CSI_INPUT_ADDR);
	deg_Printf("R_CSI_TEST_SIZE = %x\n",R_CSI_TEST_SIZE);
	deg_Printf("R_CSI_TEST_SYNC = %x\n",R_CSI_TEST_SYNC);
	deg_Printf("R_CSI_MJPEG_SIZE = %x\n",R_CSI_MJPEG_SIZE);
	deg_Printf("R_CSI_MJPEG_YADDR = %x\n",R_CSI_MJPEG_YADDR);
	deg_Printf("R_CSI_MJPEG_UVADDR = %x\n",R_CSI_MJPEG_UVADDR);
	deg_Printf("R_CSI_LCD_YADDR = %x\n",R_CSI_LCD_YADDR);
	deg_Printf("R_CSI_LCD_UVADDR = %x\n",R_CSI_LCD_UVADDR);
	deg_Printf("R_CSI_LCD_CROPH = %x\n",R_CSI_LCD_CROPH);
	deg_Printf("R_CSI_LCD_CROPV = %x\n",R_CSI_LCD_CROPV);
	deg_Printf("R_CSI_DMA_MODE = %x\n",R_CSI_DMA_MODE);


}

*/

static u32 csi_j1dma_scale_cal(u32 src, u32 tar)
{
	u32 s = src * 2 / tar;
	if (s >= 8)
		s = 1 + 3;
	else if (s >= 6)
		s = 1 + 2;
	else if (s >= 4)
		s = 1 + 1;
	else if (s >= 3)
		s = 1 + 0;
	else
		s = 0;
	return s;
}

void csi_j1dma_scale_down(u32 src_w, u32 src_h, u32 obj_w, u32 obj_h)
{
	if (1)//(obj_w == 848)
	{		
		CSI_I2W_HCROP = ((((src_w - obj_w)/2/*192*/+obj_w) << 16)		   // end
						| (src_w - obj_w)/2/*192*/ << 0);			   // start

		CSI_I2W_VCROP = (((src_h - obj_h)/2+obj_h << 16)		   // end
						| (((src_h - obj_h)/2) << 0));			   // start

		ISPMODE &= ~BIT(19);				
		ISPMODE |= BIT(18);
	}
	else
	{
		u32 s_w = csi_j1dma_scale_cal(src_w, obj_w);
		u32 s_h = csi_j1dma_scale_cal(src_h, obj_h);
		u32 s = (s_w || s_h) ? 1 : 0;
		// printf("-------------------%d*%d  %d*%d--------%d-------------\n",src_w,src_h,obj_w,obj_h,s);
		s_w = (s_w - 1) & 3;
		s_h = (s_h - 1) & 3;
		// printf("s_w:%x  s_h:%x\n",s_w,s_h);
		//////////////////////////////////////
		//  R_CSI_DMA_MODE &= ~CSI_OUTPUT_MJPGEN;//关闭hv sacle
		// CSI_I2W_HCROP &= ~(3<<11);
		// CSI_I2W_VCROP &= ~(3<<10);

		CSI_I2W_HCROP = (obj_w << 16)		   // end
						| ((s_w & 0x03) << 11) // Horizontal Scale-Down steps
						| (0 << 0);			   // start

		CSI_I2W_VCROP = (obj_h << 16)		   // end
						| ((s_h & 0x03) << 10) // Vertical Scale-Down steps
						| (0 << 0);			   // start

		ISPMODE &= ~BIT(18);
		if (s)
		{
			ISPMODE |= BIT(19) /*|BIT(18)*/; // SD_EN , wcrop_en
		}
		else
		{
			ISPMODE &= ~BIT(19) /*|BIT(18)*/;
		}
	}
	//R_CSI_DMA_MODE |= CSI_OUTPUT_WIFIEN; // 打开hv sacle

	//////////////////////////////////////

#if 0
  CSI_J1WIN_CROPH = 
    (obj_w << 16) |
    (0     <<  0);
  CSI_J1WIN_CROPV =
    (obj_h << 16) |
    (0     <<  0);

  u32 csimode2 = CSIMODE2 & ~CSI_MODE2_SCALERD & ~((3<<10)|(3<<8)|(1<<12));
  CSIMODE2 = csimode2 |
    BIT(14) | // CSI_J1DMA_EN
    //BIT(13) | // CSI_J1DMA_CRITICAL
    (s<<12) | // SD_EN
    (s_h << 10) |
    (s_w <<  8);
#endif
}

void csi_jdma_scale_down(u32 src_w, u32 src_h, u32 obj_w, u32 obj_h)
{
	ax32xx_csiScaler(src_w, src_h, obj_w, obj_h, 0, 0, src_w, src_h);
}

void csi_jdma_stop(void)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	CSI_INTR = BIT(18)|BIT(17);
	CSI_INTEN &= ~(BIT(18)|BIT(17));
	CSI_DMA_MODE &= ~BIT(19);
	arch_local_irq_restore(flag);
}
void csi_jdma_start(void)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	CSI_INTR = BIT(18)|BIT(17);
	CSI_INTEN |= BIT(18)|BIT(17);
	CSI_DMA_MODE |= BIT(19);
	arch_local_irq_restore(flag);
}

void csi_j1dma_stop(void)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	CSI_DMA_MODE &= ~BIT(20);
	arch_local_irq_restore(flag);
}
void csi_j1dma_restart(void)
{
	unsigned long flag;
	flag=arch_local_irq_save();
	//CSI_INTR = BIT(18)|BIT(17);
	//CSI_INTEN |= BIT(18)|BIT(17);
	CSI_DMA_MODE |= BIT(20);
	arch_local_irq_restore(flag);
}

void csi_lcd_scaler(u16 src_w, u16 src_h, u16 tar_w, u16 tar_h, u16 sx, u16 sy, u16 ex, u16 ey, u16 stride)
{
	if (((ex - sx) < tar_w) || ((ey - sy) < tar_h)) // scaler can not do scaler up
		return;
	R_CSI_LCD_CROPH = (ex << 16) | sx;
	R_CSI_LCD_CROPV = (ey << 16) | sy;

	R_ISPMODE |= BIT(17);

	R_CSI_SCALER_SIZE = (tar_h << 16) | tar_w;
	R_CSI_SCALER_WSTEP = ((ex - sx) << 16) / (tar_w >> 1);
	R_CSI_SCALER_HSTEP = ((ey - sy) << 16) / (tar_h >> 1);

	R_CSI_LDMA_STRIDE = stride;

	ax32xx_ispModeSet(ISP_MODE_LSEN, 1);
	R_ISPCTRL |= ISP_CTRL_LS_KS;
}

void csi_enable(void)
{
	R_CSI_INTEN |= (1 << 31);
	R_CSI_MODE |= CSI_MODE_ENABLE;
}

int csi_enabled(void)
{
	return R_CSI_MODE & CSI_MODE_ENABLE;
}

void csi_disable(void)
{
	R_CSI_INTEN &= ~(1 << 31);
	R_CSI_MODE &= ~CSI_MODE_ENABLE;
	ISPMODE = 0;
}

void csi_disable2(void)
{
	R_CSI_INTEN &= ~(1 << 31);
	R_CSI_MODE &= ~CSI_MODE_ENABLE;
}

////////////////////////////////////////////////////////////////////////////////
// csi irq
typedef int (*isr_func)(unsigned int, void *, void *);

isr_func csiirq_vector_table[CSI_IRQ_MAX];
void *csiirq_dev_table[CSI_IRQ_MAX];

////////////////////////////////////////////////////////////////////////////////
int csi_isr(unsigned int irqno, void *d, void *regs);
void csiirq_register(uint32_t irq, isr_func isr, void *dev_id)
{
	irq_register(IRQ_CSI, 0, csi_isr, NULL);
	csiirq_vector_table[irq] = isr;
	csiirq_dev_table[irq] = dev_id;
	CSI_INTEN |= BIT(irq) | 0x80000000;
	printf("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, irq, isr);
}
void csiirq_unregister(uint32_t irq)
{
	ax32xx_intEnable(IRQ_CSI,0);
	CSI_INTEN&=~BIT(irq);
	csiirq_vector_table[irq] = NULL;
	csiirq_dev_table[irq] = NULL;
	ax32xx_intEnable(IRQ_CSI,1);
}

void csiirq_register_isp(isr_func isr, void *dev_id)
{
	csiirq_register(CSI_IRQ_ISP, isr, dev_id);
}

void csiirq_register_lcd_frame_end(isr_func isr, void *dev_id)
{
	csiirq_register(CSI_IRQ_LCD_FRAME_END, isr, dev_id);
}

void csiirq_register_j1dma_frame_end(isr_func isr, void *dev_id)
{
	printf("..............................j1dma..................dev_id:%x\n", dev_id);
	csiirq_register(CSI_IRQ_VXY_FRAME_EN, isr, dev_id);
}
void csiirq_unregister_j1dma_frame_end(void)
{
	csiirq_unregister(CSI_IRQ_VXY_FRAME_EN);
}
void csiirq_register_jdma_frame_end(isr_func isr, void *dev_id)
{
	//printf("...............................jdma.................dev_id:%x\n", dev_id);
	csiirq_register(CSI_IRQ_JPG_FRAME_END, isr, dev_id);
}
void csiirq_unregister_jdma_frame_end(void)
{
	csiirq_unregister(CSI_IRQ_JPG_FRAME_END);
}
void csiirq_register_mdt_action(isr_func isr, void *dev_id)
{
	csiirq_register(CSI_IRQ_MDT_MOTION_DET, isr, dev_id);
}
void csiirq_register_sen_state(isr_func isr, void *dev_id)
{
	csiirq_register(CSI_IRQ_SEN_STATE_INT, isr, dev_id);
}
void csiirq_unregister_sen_state(void)
{
	csiirq_unregister(CSI_IRQ_SEN_STATE_INT);
}


/*******************************************************************************
* Function Name  : ax32xx_mdInit
* Description    : motion dection enable set
* Input          :  U8 cnt : update pre cnt frame. // datasheet says
					  u8 num : block number
					  u8 pixel : pixel dc threashold
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdInit(u8 pixel, u8 num, u8 cnt)
{
	R_MD_CTRL = ((cnt & 0x1f) << 2) | ((num & 0x1ff) << 7) | (pixel << 16) | (1 << 24);
}
/*******************************************************************************
* Function Name  : ax32xx_mdXPos
* Description    : motion dection x position  set
* Input          :  u16 start : x start position
					   u16 end  : x end position
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdXPos(u16 start, u16 end)
{
	R_MD_XPOS = ((end << 16) | start);
}
/*******************************************************************************
* Function Name  : ax32xx_mdYPos
* Description    : motion dection y position set
* Input          :  u16 start : y start position
					   u16 end  : y end position
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_mdYPos(u16 start, u16 end)
{
	R_MD_YPOS = ((end << 16) | start);
}

u32 csi_num = 0;
////////////////////////////////////////////////////////////////////////////////
int csi_isr(unsigned int irqno, void *d, void *regs)
{
	static u32 lastTime;
	int i;
	uint32_t intrcsi = CSI_INTR;

	if (intrcsi & BIT(CSI_IRQ_SEN_STATE_INT))
	{
		csifcnt++;
	}

	for (i = 0; i < CSI_IRQ_ISP; ++i)
	{
		if (intrcsi & BIT(i))
		{
			// printf("csi isr:%d  %x\n",i,CSI_INTEN);
			if (csiirq_vector_table[i])
			{

#if 0	  	
	  	printf("CSIMODE:%x\n",CSIMODE);
		printf("ISPMODE:%x\n",ISPMODE);
	  	printf("CSI_DMA_MODE:%x\n",CSI_DMA_MODE);	
		printf("CSI_I2W_LBC:%x\n",CSI_I2W_LBC);
		printf("CSI_I2W_LBS:%x\n",CSI_I2W_LBS);

		
		printf("CSI_I2W_HCROP:%x\n",CSI_I2W_HCROP);	
		printf("CSI_I2W_VCROP:%x\n",CSI_I2W_VCROP);
#endif
				if (i == 0)
				{
					csi_num++;
				}

				csiirq_vector_table[i](i, csiirq_dev_table[i], regs);
			}

			csi_errcnt = 0;
		}
	}

	if (intrcsi & (0xF << 12))
	{
		//    CSI_INTCLR = 0xF<<12;
		//printf("isp:%x\n",intrcsi & (0xF<<12));
		if (csiirq_vector_table[CSI_IRQ_ISP])
		{
			// printf("csi isp: %x\n",csiirq_vector_table[CSI_IRQ_ISP]);
			csiirq_vector_table[CSI_IRQ_ISP](CSI_IRQ_ISP, csiirq_dev_table[CSI_IRQ_ISP], regs);
		}

		csi_errcnt = 0;
	}

	uint32_t error_mask = (7 << 17) | (1 << 11) | (1 << 8) | (7 << 4);
	if (intrcsi & error_mask)
	{
		// BIT(17): JE_LINE_BUFF_ERR
		// BIT(16): JE_CSI_ERR
		// BIT(11): SYNC_FIFO_ERR
		// BIT(9):  SACL_INTLBUFF_ERR
		// BIT(6):  LCD_INTLBUF_ERR
		// BIT(5):  MJP_JINTLBUFF_ERR
		// BIT(4):  SENSOR_SIZE_ERR
		if (csi_errcnt++ > 100)
		{
			printf("csi err\n");
			// todo error recover
		}
		if(os_time_get()-lastTime>=5000)
		{
			lastTime=os_time_get();
			printf("csi err:%x  CSI_SIZE_STAT: %x\n", intrcsi, CSI_SIZE_STAT);
		}
		if (intrcsi & BIT(CSI_IRQ_JBF_DMAWR_ERR))
		{
			//jpeg_jbuferr_deal();
		}
#if 0
    if (intrcsi & BIT(CSI_IRQ_SEN_STATE_ERR)) {
      if (CSI_DMA_MODE & BIT(24)) {
        CSI_DMA_MODE &= ~BIT(24);   // LDMA dis
        ax32xx_pipCounterReset ();
        CSI_DMA_MODE |=  BIT(24);   // LDMA en
      }
    }

    if (intrcsi & BIT(CSI_IRQ_LBF_DMAWR_ERR)) {
      CSI_INTEN &= ~BIT(CSI_IRQ_LBF_DMAWR_ERR);
      CSI_DMA_MODE &= ~BIT(24);   // LDMA dis
//      CSI_INTCLR = BIT(6);
      ax32xx_pipCounterReset ();
      CSI_DMA_MODE |=  BIT(24);   // LDMA en
      CSI_INTEN |= BIT(CSI_IRQ_LBF_DMAWR_ERR);
    }

    if (intrcsi & BIT(CSI_IRQ_JBF_DMAWR_ERR)) {
      ax32xx_mjpegEncodeEnable(0);
      extern int mjpeg_start_isr (unsigned int irqno, void *d, void *regs);
      csiirq_register (CSI_IRQ_SEN_STATE_INT, mjpeg_start_isr, 0);
    }
#endif
	}

	R_CSI_INTCLR = intrcsi;
	// printf ("\n#### csi_isr %08X %08X\n\n", intrcsi, CSI_INTEN);
	return 1;
}

void csi_dumpreg(void)
{
	printf("CSI %08X %08X %08X %08X\n", CSIMODE, ISPMODE, CSI_INTR, CSI_INTEN);
	printf("DMA %08X\n", CSI_DMA_MODE);
	printf("PRE_JPCON1 %08X  PRE_JPINTCON %08X  PRE_JPFLG_CLR %08X \n", PRE_JPCON1, PRE_JPINTCON, PRE_JPFLG_CLR);
	printf("MASK %08X %08X PCON %08X\n", CSI_INTEN, SPR_PICMR, PCON0);
}

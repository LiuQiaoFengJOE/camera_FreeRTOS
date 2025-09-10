/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                         AUADC DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_auadc.c
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUDIO ADC HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
// #include "AX3281.h"
// #include "basic_defs.h"
// #include "spr_defs.h"

#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"
#include "ax329x_irq.h"

// #include "gpio_ops/gpio.h"
#include "sys/sys.h"
#include "board_config.h"

#define IRQNO_AUADC 18

enum
{
	__50DB__ = 5,
	__60DB__ = 6,

};

/*
#define  R_AGC_STATUS     AUADCSTA
#define  R_AGC_CLEAR      AGCCFG
#define  R_AGC_CTRL0      AGCCFG
#define  R_AGC_CTRL1      AGCCFG    //AGCCON0
#define  R_AGC_CTRL2      AGCLVL    //AGCCON1
#define  R_AGC_CTRL3      AGCDRC
#define  R_AGC_ADDR       AUADCBADR
#define  R_AUADC_CTRL     AUADCCFG
*/

static void (*ax32xxAuadcISR0)(void);
static void (*ax32xxAuadcISR1)(void);

#define R_SYS_CLK PCON0
/*******************************************************************************
 * Function Name  : ax32xx_sysClkSet
 * Description    : system clk set
 * Input          : u8 clk_type : clk type.SYS_CLK_E
 *                  u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/

// void ax32xx_sysClkSet(u8 clk_type,u8 en)
//{
//	if(en)
//		R_SYS_CLK &=~(1<<clk_type);
//	else
//		R_SYS_CLK |= (1<<clk_type);
// }

// void ax32xx_intEnable(u8 irqNum,u8 en)
//{
//     if(en)
//		__sprset(SPR_PICMR,irqNum);
//	else
//		__sprclr(SPR_PICMR,irqNum);
// }

/*******************************************************************************
* Function Name  : ax32xx_int_priority
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
				   u8 level     : 1->high priority,0->low priority
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_int_priority(u8 irqNum, u8 level) // 这个似乎没有用到
{
	if (level)
		__sprset(SPR_PICPR, irqNum);
	else
		__sprclr(SPR_PICPR, irqNum);
}

///*******************************************************************************
//* Function Name  : ax32xx_auadcHalfIRQRegister
//* Description    : auadc hanlf irq regiser
//* Input          :void (*isr)(void) : isr call back
//* Output         : None
//* Return         : None
//*******************************************************************************/
void ax32xx_auadcHalfIRQRegister(void (*isr)(void)) // 这里是注册ADChalf中断，为这个中断
{													// 指定一个中断服务函数
	ax32xxAuadcISR0 = isr;
}

///*******************************************************************************
//* Function Name  : ax32xx_auadcHalfIRQRegister
//* Description    : auadc end irq regiser
//* Input          :void (*isr)(void) : isr call back
//* Output         : None
//* Return         : None
//*******************************************************************************/
void ax32xx_auadcEndIRQRegister(void (*isr)(void)) // 这里是注册ADCend中断，为这个中断
{												   // 指定一个中断服务函数
	ax32xxAuadcISR1 = isr;
}
/*******************************************************************************
 * Function Name  : ax32xx_auadcEnable
 * Description    : auadc enable
 * Input          :u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_auadcEnable(u8 en, u8 simGain, u8 digGain) // 使能\失能auADC，应该是用于中途开启关闭,set_running时候调用
{
	if (en)
	{
		AUADCCFG |= (1 << 2);	// RMDC EN
		AUADCCFG |= (0x1 << 3); // DMA EN
		// Init SRC filter SRAM
		AUADCSTA |= 0x1 << 31;
		while ((AUADCSTA & 0x4) == 0x0)
		{
			;
		}
		AUADCSTA = 0x1 << 10; // Clear SRAM flag
		AUADCCFG |= 0x1;	  // Kick start AUADC
		// Enable all AUADC interrupt flags
		AUADCSTA = 0x3 << 16; // half/full ie enable
#if 0
		AUADCGAIN = (0x78 << 24)	 //zcd_thd// dgain range 0~0x78 12db
				| (0x2	<< 18)	 //dec_step  (00~11:-0.1,-0.2,-0.3,-0.4 dB)
				| (0x2	<< 16)	//inc_step	(00~11:0.1,0.2,0.3,0.4 dB)
				| (digGain	<< 8)		//dgain (0~0x3c)
				| (simGain << 0);   //again (0~0xd)
#endif

		ax32xx_intEnable(IRQ_AUADC, 1);
	}
	else
	{
		AUADCSTA |= (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); // AGC INT PENDING CLEAR
		AUADCCFG &= ~(1 << 0);									 // AUADC_DIS
		AUADCCFG &= ~((1 << 3) | (1 << 2) | (1 << 1));			 //(DMA_DIS|RMDC_DIS|DGAIN_DIS)
		AGCCFG &= ~(1 << 0);									 // AGC DIS
		AUADCSTA |= (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); // AGC INT PENDING CLEAR

		ax32xx_intEnable(IRQ_AUADC, 0);
	}

	//	printf("GainSet:%X\r\n",R_AGC_CTRL2);
	//	printf("AGCCON1:%X\r\n",AGCCON1);
}

/*******************************************************************************
 * Function Name  : ax32xx_auadcAGCEnable
 * Description    : auadc agc enable
 * Input          :u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_auadcAGCEnable(u8 en)
{
	AGCLVL = (0x4000 << 16)			 // max_thd
			 | (0x3000 << 0);		 // min_thd
	AGCDRC = (0x0100 << 16)			 // vad_thd
			 | (0x7800 << 0);		 // drc_thd
	AGCCFG = (AGCCFG & (0xfff00000)) // again_mute
									 //   | (8 << 24)  //again_max
									 //   | (1 << 20)  //again_min
			 | (3 << 16) // vad_scnt
			 | (3 << 12) // agc_scnt
			 | (3 << 8)	 // mute_scnt
			 | (0 << 4)	 // mute_en
			 | (1 << 3)	 // drc_en
			 | (1 << 2)	 // zcd_en
			 | (1 << 1)	 // vad_en
			 | (1 << 0)	 // agc_en
		;
	AUADCCFG |= (1 << 1); // dgain en
	if (!en)
	{
		AGCCFG &= ~((1 << 3) | (1 << 0)); // drc_dis,agc_dis
		AUADCCFG &= ~(1 << 1);			  // DGAIN dis
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_auadcGainSet
 * Description    : auadc agc gain set
 * Input          :u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_auadcGainSet(u8 again, u8 again_mute, u8 again_max, u8 again_min)
{
	AUADCGAIN = (0x78 << 24)	// zcd_thd// dgain range 0~0x78 12db
				| (0x2 << 18)	// dec_step  (00~11:-0.1,-0.2,-0.3,-0.4 dB)
				| (0x2 << 16)	// inc_step	(00~11:0.1,0.2,0.3,0.4 dB)
				| (0x0 << 8)	// dgain (0~0x3c)
				| (again << 0); // again (0~0xd)
	AGCCFG &= ~(0xfff << 20);
	AGCCFG |= (again_mute << 28) | (again_max << 24) | (again_min << 20);
	AUADCSTA |= (0x1 << 30);
}

/*******************************************************************************
* Function Name  : ax32xx_auadcBufferSet
* Description    : auadc set frame buffer
* Input          :u32 addr : addr
					 u32 len   : buffer length
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcBufferSet(u32 addr, u32 len) // 这里应该是设置adc自动填充的buff
{
	ax32xx_sysDcacheFlush(addr, len);
	AUADCBADR = addr;
	AUADCDCNT = (len >> 2) - 1; /*AGCCON3 单位为word*/
	AUADCCFG |= (0 << 4);		// DMASK_SEL:DMA AUDIO DATA SELECTION.(0:0dB; 1:-6dB, 2: -12dB,  3: all data mask to 0)
	AUADCCFG |= (0x1 << 3);		// DMA EN
}
/*******************************************************************************
 * Function Name  : ax32xx_auadcInit
 * Description    : auadc intial
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
//extern uint32_t APB_CLK;
//#define PLL_CLK (APB_CLK * 2)
#define FPGA 0X0080
#define AX329X 0X6490
#define AX32_PLATFORM AX329X

void ax32xx_auadcInit(void)
{
#if (AX32_PLATFORM == FPGA)
	printf("auadc FPGA\n");
	CLKCON1 &= ~(BIT(27) | BIT(26));
	CLKCON1 |= (BIT(25) | BIT(24));
	CLKCON0 &= ~(BIT(9)); /*select pll0_out(96M)*/
	CLKCON0 |= BIT(8);
#else
#if 0
		CLKCON0 |= BIT(8); //USB PHY 96MHz
		CLKCON0 &=~BIT(9);
		CLKCON1 &=~(0xf<<24);
		CLKCON1 |= ((96000000/24000000 - 1)<<24);
#else
	printf("auadc board\n");
	CLKCON0 &= ~BIT(8); // SPLL
	CLKCON0 &= ~BIT(9);
	CLKCON1 &= ~(0xf << 24);
	CLKCON1 |= ((PLL_CLK / 24000000 - 1) << 24);
#endif
#endif
	ax32xx_sysClkSet(SYS_CLK_AUADC, 1);
	ax32xx_sysClkSet(SYS_CLK_DAC, 1);

	DACACON0 |= (1 << 12); // Preamp and Midamp enable

	DACACON0 |= (BIT(13) | // EN_VCM
				 BIT(11) | // MIC BIAS
				 BIT(10) | // EN_HP
				 BIT(4) |
				 BIT(3) | // EN_DAC
				 BIT(2) | // EN_AUDIOBIAS
				 BIT(1) | // EN_ANABIAS
				 BIT(0)); // EN_ABG
	//	AUADCCFG |= (0x1 << 14); // Release AUADC reset
}

/*******************************************************************************
 * Function Name  : ax32xx_auadcSetSampleSet
 * Description    : auadc set sample set
 * Input          : int sample_rate : sample rate
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_auadcSetSampleSet(int sample_rate) // 采样率设置
{
	AUADCCFG |= (0x1 << 14); // Release AUADC reset
	AUADCBAUD = 0x007c0002;	 // 14bit SARADC Fmax = 4.8MHz, FS = 192KHz
	AUADCCFG &= ~0xffff0000; // clear setting
	if (16000 == sample_rate)
	{
		// ADCBAUD = 0x00001005;
		// AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
		AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
		AUADCCFG |= (0x1 << 16); // Enable Filter 1
		AUADCCFG |= (0x1 << 21); // Set FILT2DN = 2/1
		AUADCCFG |= (0x1 << 17); // Enable Filter 2
		printf("- adc 16k\n");
	}
	else if (32000 == sample_rate)
	{
		// ADCBAUD = 0x00001005;
		// AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
		AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
		AUADCCFG |= (0x1 << 16); // Enable Filter 1
								 // AGCCON0  |= (2<<0);		//SR_SEL
		printf("- adc 32k\n");
	}
	else if (8000 == sample_rate)
	{
		// ADCBAUD = 0x00001005;
		// AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
		AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
		AUADCCFG |= (0x1 << 16); // Enable Filter 1
		AUADCCFG |= (0x3 << 21); // Set FILT2DN = 4/1
		AUADCCFG |= (0x1 << 17); // Enable Filter 2
								 //	AGCCON0  |= (3<<0); 	//SR_SEL
		printf("- adc 8k\n");
	}
}

typedef enum
{
	AUADC_IRQ_HALF = 0,
	AUADC_IRQ_END
} AUADC_IRQ_E;

typedef int (*isr_func)(unsigned int, void *, void *);

isr_func auadc_vector_table[2];
void *auadc_dev_table[2];

void auadc_irq(unsigned int irqno, void *d, void *regs);
void auadcirq_register(uint32_t irq, isr_func isr, void *dev_id)
{
	irq_register(IRQNO_AUADC, 0, auadc_irq, NULL);
	auadc_vector_table[irq] = isr;
	auadc_dev_table[irq] = dev_id;
	// CSI_INTEN |= BIT(irq) | 0x80000000;
	printf("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, IRQNO_AUADC, isr);
}

void auadc_register_half(isr_func isr, void *dev_id)
{
	auadcirq_register(AUADC_IRQ_HALF, isr, dev_id);
}

void auadc_register_end(isr_func isr, void *dev_id)
{
	auadcirq_register(AUADC_IRQ_END, isr, dev_id);
}
void auadc_irq_dis(void)
{
	irq_unregister(IRQNO_AUADC);
}
// 中断的时候会调用这个函数，接着在里面判断
void auadc_irq(unsigned int irqno, void *d, void *regs)
{						   // half中断还是end中断
	if (AUADCSTA & BIT(1)) // 1
	{
		if (auadc_vector_table[AUADC_IRQ_HALF])
			auadc_vector_table[AUADC_IRQ_HALF](AUADC_IRQ_HALF, auadc_dev_table[AUADC_IRQ_HALF], regs);
		AUADCSTA |= BIT(9); // HALF CLR
	}
	else if (AUADCSTA & BIT(0))
	{
		if (auadc_vector_table[AUADC_IRQ_END])
			auadc_vector_table[AUADC_IRQ_END](AUADC_IRQ_END, auadc_dev_table[AUADC_IRQ_END], regs);
		AUADCSTA |= BIT(8); // FULL CLR
	}
}

////中断注册
// void auadc_register(void)   //
//{
//	irq_register(IRQNO_AUADC, 0, auadc_irq, NULL);
//	auadc_dev_table
//	printf ("%s\n", __FUNCTION__);
// }

/*

//		printf("auadc half1\n");
//		if(use == 0)
//			{ax32xx_auadcBufferSet(auadc_buffB,8192);use = 1;}
//		else
//			{ax32xx_auadcBufferSet(auadc_buffA,8192);use = 0;}
		if(auadc_frame_half_isr) 					 //这两个函数指针在注册中断的时候分别指向了
			auadc_frame_half_isr(d, regs);					 //half中断和end中断；

	}
	else    //0
	{
//		printf("auadc end1\n");
//		ax32xx_auadcEnable(0);
//		test_flag = 1;
		if(auadc_frame_done_isr) 					 //这两个函数指针在注册中断的时候分别指向了
			auadc_frame_done_isr(d, regs);					 //half中断和end中断；

	}
	R_AGC_CLEAR = BIT(0);

u8 test_auadc = 1;
u8 test_flag  = 0;
u8 use = 0;
u8 auadc_buffA[8192];
u8 auadc_buffB[8192];

*/

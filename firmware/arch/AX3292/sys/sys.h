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
* File Name   : ax32xx_sys.h
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
#ifndef AX32XX_SYS_H
#define AX32XX_SYS_H

#ifndef __ASSEMBLER__
// extern int _isr_vactor_start; 		//引用ld文件变量
// extern int _isr_vactor_end; 		//引用ld文件变量
// extern int _isr_vactor_len; 		//引用ld文件变量

// extern int _upload_start;
// extern int _upload_end;
// extern int _upload_len;
// extern u32 __bss_start,__bss_end ;
extern volatile u32 SYSCLK;

#endif // #ifndef __ASSEMBLER__

enum
{
    SYS_MODE_SLEEP = 0,
    SYS_MODE_HOLD,    // 1
    SYS_MODE_IDLE,    // 2
    SYS_CLK_IROM,     // 3
    SYS_CLK_GRAM,     // 4
    SYS_CLK_ICACHE,   // 5
    SYS_CLK_DCACHE,   // 6
    SYS_CLK_DE,       // 7
    SYS_CLK_SDRAM,    // 8
    SYS_CLK_PIP,      // 9
    SYS_CLK_TIMER0,   // 10
    SYS_CLK_TIMER1,   // 11
    SYS_CLK_TIMER2,   // 12
    SYS_CLK_TIMER3,   // 13
    SYS_CLK_IIC0,     // 14
    SYS_CLK_MBIST,    // 15
    SYS_CLK_SD1,      // 16
    SYS_CLK_RTC,      // 17
    SYS_CLK_UART1,    // 18
    SYS_CLK_UART0,    // 19
    SYS_CLK_SPI0,     // 20
    SYS_CLK_SPI1,     // 21
    SYS_CLK_SD0,      // 22
    SYS_CLK_IIC1,     // 23
    SYS_CLK_AUADC,    // 24
    SYS_CLK_LCDC,     // 25
    SYS_CLK_JPGA,     // 26
    SYS_CLK_DVP_CSI,  // 27
    SYS_CLK_USB20,    // 28
    SYS_CLK_ADC,      // 29
    SYS_CLK_DAC,      // 30
    SYS_CLK_JPGB,     // 31
    SYS_CLK_USB11,    // 32
    SYS_CLK_OSDCMP,   // 33
    SYS_CLK_ROTATE,   // 34
    SYS_CLK_SPIF,     // 35
    SYS_CLK_MIPI_CSI, // 36
};

typedef enum
{
    DMA_BTUART = 0,    // 0
    DMA_JPGB_DRI,      // 1
    DMA_JPGA_UVC_DRI,  // 2
    DMA_ISP_WR_GRAM,   // 3
    DMA_AUDAC,         // 4
    DMA_AUADC,         // 5
    DMA_MEMCPY0,       // 6
    DMA_MEMCPY1,       // 7
    DMA_MEMCPY2,       // 8
    DMA_MEMCPY3,       // 9
    DMA_SPI0,          // 10
    DMA_SPI1,          // 11
    DMA_SD0,           // 12
    DMA_SD1,           // 13
    DMA_JPGA_TIMEINFO, // 14
    DMA_JPGB_TIMEINFO, // 15
    DMA_ICACHE,        // 16
    DMA_DCACHE,        // 17
    DMA_ISP_RD_DRAM,   // 18
    DMA_ISP_JDMA_WIFI, // 19
    DMA_JPGA_ENC_WR,   // 20
    DMA_JPGA_RD,       // 21
    DMA_ISP_JDMA,      // 22
    DMA_JPGB_ENC_WR,   // 23
    DMA_JPGB_DEC_WR,   // 24
    DMA_JPGB_RD,       // 25
    DMA_ISP_LDMA,      // 26
    DMA_DE,            // 27
    DMA_OSDCMP_WR,     // 28
    DMA_OSDCMP_RD,     // 29
    DMA_ROTATE_WR,     // 30
    DMA_ROTATE_RD,     // 31
} DMA_CH_E;

#define SYS_LDO_HSEN 0
#define SYS_LDO_LSEN 1
#define SYS_LDO_BGOE 2
#define SYS_LDO_V1_2 3
#define SYS_LDO_SDC 4
#define SYS_LDO_DDR 5

typedef enum
{
    SYS_VOL_V0_71 = 0,
    SYS_VOL_V0_74,
    SYS_VOL_V0_77,
    SYS_VOL_V0_80,
    SYS_VOL_V0_83,
    SYS_VOL_V0_86,
    SYS_VOL_V0_89,
    SYS_VOL_V0_92,

    SYS_VOL_V1_00 = 0,
    SYS_VOL_V1_20,
    SYS_VOL_V1_25,
    SYS_VOL_V1_30,

    SYS_VOL_V1_2 = 0, // 0
    SYS_VOL_V1_3,     // 1
    SYS_VOL_V1_4,     // 2
    SYS_VOL_V1_5,     // 3
    SYS_VOL_V1_6,     // 4
    SYS_VOL_V1_7,     // 5
    SYS_VOL_V1_8,     // 6
    SYS_VOL_V1_9,     // 7
    SYS_VOL_V2_0,     // 8
    SYS_VOL_V2_1,     // 9
    SYS_VOL_V2_2,     // 10
    SYS_VOL_V2_3,     // 11
    SYS_VOL_V2_4,     // 12
    SYS_VOL_V2_5,     // 13
    SYS_VOL_V2_6,     // 14
    SYS_VOL_V2_7,     // 15
    SYS_VOL_V2_8,     // 16
    SYS_VOL_V2_86,    // 17
    SYS_VOL_V2_9,     // 18
    SYS_VOL_V2_96,    // 19
    SYS_VOL_V3_0,     // 20 (default)
    SYS_VOL_V3_06,    // 21
    SYS_VOL_V3_1,     // 22
    SYS_VOL_V3_16,    // 23
    SYS_VOL_V3_2,     // 24
    SYS_VOL_V3_26,    // 25
    SYS_VOL_V3_3,     // 26
    SYS_VOL_V3_36,    // 27
    SYS_VOL_V3_4,     // 28
    SYS_VOL_V3_46,    // 29
    SYS_VOL_V3_5,     // 30
    SYS_VOL_V3_56,    // 31

    SYS_VOL_END
} SYS_LDO_E;

typedef enum
{
    SYS_RESET_JPEG = 0x01,
    SYS_RESET_CSI = 0x02,
    SYS_RESET_LCD = 0x04,
} SYS_RESET_E;

/*******************************************************************************
 * Function Name  : ax32xx_sysDelay
 * Description    : system delay ms
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysCpuMsDelay(int dtime);
/*******************************************************************************
 * Function Name  : ax32xx_sysCpuNopDelay
 * Description    : system delay nop
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysCpuNopDelay(int dtime);
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheInit
 * Description    : dcache initial
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheInit(void);
/*******************************************************************************
 * Function Name  : ax32xx_sysIcacheInit
 * Description    : icache initial
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysIcacheInit(void);
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheWback
 * Description    : data cache write back
 * Input          : u32 addr : address
 *                  u32 size : size
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheWback(u32 addr, u32 size);
/*******************************************************************************
 * Function Name  : ax32xx_sysDcacheFlush
 * Description    : data cache flush
 * Input          : u32 addr : address
 *                  u32 size : size
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysDcacheFlush(u32 addr, u32 size);
/*******************************************************************************
 * Function Name  : ax32xx_sysLDOSet
 * Description    : set system ldo for power support
 * Input          : u8 ldo : ldo type
 *                  u8 sel : power sel
 *                  u8 en  : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysLDOSet(u8 ldo, u8 sel, u8 en);
/*******************************************************************************
 * Function Name  : ax32xx_sysClkSet
 * Description    : system clk set
 * Input          : u8 clk_type : clk type.SYS_CLK_E
 *                  u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysClkSet(u8 clk_type, u8 en);
/*******************************************************************************
 * Function Name  : ax32xx_sysReset
 * Description    : system module reset
 * Input          : u8 clk_type : clk type.SYS_RESET_E
 *                  u8 en : 1-reset,0-normal
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysReset(u8 rst_type, u8 en);

/*******************************************************************************
* Function Name  : ax32xx_sysInit
* Description    : initial system for platform using.
* Input          : u32 *saddr : sdram free start address
                      u32 *eaddr : sdram free end address
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysInit(u32 *saddr, u32 *eaddr);
/*******************************************************************************
 * Function Name  : ax32xx_sysUninit
 * Description    : uninitial system .
 * Input          : none
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_sysUninit(void);
void ax32xx_mcpy1_sdram2gram(void *dst, void *src, int cnt);
void ax32xx_mcpy2_uram2gram(void *dst, void *src, int cnt);

#define memcpy dma_memcpy

#endif

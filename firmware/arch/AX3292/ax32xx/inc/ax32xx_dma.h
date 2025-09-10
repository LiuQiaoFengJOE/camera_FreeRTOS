/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                       AX32XX DMA
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_dma.h
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
#ifndef AX32XX_DMA_H
#define AX32XX_DMA_H

#if 0	
typedef enum
{
    DMA_BTUART = 0,    //0
    DMA_JPGB_DRI,      //1
    DMA_JPGA_UVC_DRI,  //2
    DMA_ISP_WR_GRAM,   //3
    DMA_AUDAC,         //4
    DMA_AUADC,         //5
    DMA_MEMCPY0,       //6
    DMA_MEMCPY1,       //7
    DMA_MEMCPY2,       //8
    DMA_MEMCPY3,       //9
    DMA_SPI0,          //10
    DMA_SPI1,          //11
    DMA_SD0,           //12
    DMA_SD1,           //13
    DMA_JPGA_TIMEINFO, //14
    DMA_JPGB_TIMEINFO, //15
    DMA_ICACHE,        //16
    DMA_DCACHE,        //17
    DMA_ISP_RD_DRAM,   //18
    DMA_ISP_JDMA_WIFI, //19
    DMA_JPGA_ENC_WR,   //20
    DMA_JPGA_RD,       //21
    DMA_ISP_JDMA,      //22
    DMA_JPGB_ENC_WR,   //23
    DMA_JPGB_DEC_WR,   //24
    DMA_JPGB_RD,       //25
    DMA_ISP_LDMA,      //26
    DMA_DE,            //27
    DMA_OSDCMP_WR,     //28
    DMA_OSDCMP_RD,     //29
    DMA_ROTATE_WR,     //30
    DMA_ROTATE_RD,     //31
}DMA_CH_E;
#endif


void ax32xx_dmaNocDefault(void);
void ax32xx_dmaNocPrehigh(void);
void ax32xx_dmaNocBackhigh(void);
void ax32xx_dmaNocPlayback(void);
/*******************************************************************************
 * Function Name  : ax32xx_dmaNocCfg
 * Description    :
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
//void ax32xx_dmaNocCfg(noc_cfg_t *p_cfg);
/*******************************************************************************
 * Function Name  : ax32xx_dmaChannelEnable
 * Description    : dma channel enable
 * Input          : u8 channel : dma channel
 *                  u8 en : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void ax32xx_dmaChannelEnable(u8 channel, u8 en);

#endif

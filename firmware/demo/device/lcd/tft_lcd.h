/****************************************************************************
**
 **                              DEVICE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DEVICE TFT-LCD HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : tft_lcd.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is TFT-LCD device file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  TFT_LCD_H
#define  TFT_LCD_H
#include "typedef.h"
#include "drv/lcd/lcdstruct.h"
#include "board_config.h"




#define  DEV_LCD_ILI9806E02         0
#define  DEV_LCD_ST7789S            1
#define  DEV_LCD_JL                 2
#define  DEV_LCD_ST7282             3  // ok 480*272
#define  DEV_LCD_ILI9806E1540       4
#define  DEV_LCD_KLD4301A           5
#define  DEV_LCD_SPFD5420           6  // ok
#define  DEV_LCD_R61509V            7  // ok
#define  DEV_LCD_ILI8961            8  // ok
#define  DEV_LCD_ILI9342C           9  // ok
#define  DEV_LCD_ILI9328            10 // ok
#define  DEV_LCD_HX8352B            11 // ok-400*240
#define  DEV_LCD_ST7789V            12
#define  DEV_LCD_HX8352C            13 // ok 400*240
#define  DEV_LCD_ILI9326G           14 // ok 400*240
#define  DEV_LCD_HX8357B          	15 // ok 480*320
#define  DEV_LCD_OTM4001A          	16 // ok 400*240
#define  DEV_LCD_OTA5182          	17
#define  DEV_LCD_ST7701FW1601     	18
#define	 DEV_LCD_ST7701WV1604		19
#define  DEV_LCD_lgdp4532			20
#define  DEV_LCD_iLi9225G			21// ok  220*176
#define  DEV_LCD_ST7701HSD495       22
#define  DEV_LCD_9503V				23
#define  DEV_LCD_ST7735S_SPI        24
#define  DEV_LCD_ST7789V_SPI		25
#define  DEV_LCD_ST7701S3002L0G		26
#define  DEV_LCD_9503				27
#define  DEV_LCD_NV3030B				28
#if AX329X_TYPE==3297
#define  DEV_LCD                    DEV_LCD_ST7789S// DEV_LCD_iLi9225G//DEV_LCD_ST7789S//
#else
#define  DEV_LCD                  DEV_LCD_ST7789V// DEV_LCD_9503// DEV_LCD_9503V//DEV_LCD_ST7701HSD495
#endif
#define LCD_OP_SECTION              __attribute__((section(".lcd.header.items")))//SECTION(".lcd.header.items")     
#define LCD_INTI_TAB_SECTION         //SECTION(".lcd_res.init_tab")

#define BEGIN_INIT_TAB              LCD_INTI_TAB_SECTION static u32 init_tab[][2] = {
#define END_INIT_TAB                {LCD_TAB_END,LCD_TAB_END},};

#define BEGIN_UNINIT_TAB            LCD_INTI_TAB_SECTION static u32 uninit_tab[][2] = {
#define END_UNINIT_TAB              {LCD_TAB_END,LCD_TAB_END},};

#define INIT_TAB_INIT               .init_table = init_tab, \
                                    .init_table_size = sizeof(init_tab),
#define UNINIT_TAB_INIT             .uninit_table = uninit_tab, \
                                    .uninit_table_size = sizeof(uninit_tab),

#define BCTAB0	   186789922UL
#define BCTAB1	   220475167UL
#define BCTAB2	   270937372UL
#define BCTAB3	   321399577UL
#define BCTAB4	   371861782UL
#define BCTAB5	   422258707UL
#define BCTAB6	   472720912UL
#define BCTAB7	   523117837UL

#define WBIC_TAB   WBIC_TAB0,WBIC_TAB1,WBIC_TAB2,WBIC_TAB3,WBIC_TAB4,WBIC_TAB5,WBIC_TAB6,WBIC_TAB7
#define HBIC_TAB   HBIC_TAB0,HBIC_TAB1,HBIC_TAB2,HBIC_TAB3,HBIC_TAB4,HBIC_TAB5,HBIC_TAB6,HBIC_TAB7
#define BCTAB      BCTAB0,BCTAB1,BCTAB2,BCTAB3,BCTAB4,BCTAB5,BCTAB6,BCTAB7


enum RGB_COLOR{
	RGB=0,
	RBG,
	BRG,
	GRB,
	GBR,
	BGR
};

/***************BITS of LCDDMOD0******************/

enum DATA_INV{//BIT[31]    data inversion mode
	DATA_NOT_INVER=0,
	DATA_INVER
};

enum COMPMODE{//BIT[26:24] data compress mode
	RGB888=0,
	RGB666,
	RGB565,
	RGB444,
	RGB453,
	RGB332
// others = reserved
};

enum FIFOMODE{//BIT[23:20]  FIFO mode for data organization
	MODE24BIT_P=0, // 24bit parallel mode 
	MODE24BIT_S,	// 24bit shift mode 
	MODE16BIT_R,	// 16bit red extend mode
	MODE16BIT_B,	// 16bit blue extend mode
	MODE16BIT_C,	// 16bit compact mode
	MODE16BIT_S,	//16bit shift mode
	MODE8BIT_S,	//8bit serial mode 
	MODE7			//mode 7,no description
// others = reserved
};

enum SDIR2{// BIT[17] output cycle 2 shift direction
	SDIR2_UP=0,
	SDIR2_DOWN
};

//BIT[16:12]:SCNT2,output cycle 2 shift bit count, 0 to 23,too many to list

enum SDIR1{// BIT[11] output cycle 1 shift direction
	SDIR1_UP=0,
	SDIR1_DOWN
};

//BIT[10:6]:SCNT2,output cycle 1 shift bit count,0 to 23,too many to list

enum SDIR0{// BIT[5] output cycle 0 shift direction
	SDIR0_UP=0,
	SDIR0_DOWN
};




#endif


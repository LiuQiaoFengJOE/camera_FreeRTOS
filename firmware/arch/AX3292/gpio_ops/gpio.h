/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                         GPIO DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_gpio.h
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN GPIO HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_GPIO_H
#define AX32XX_GPIO_H
#include "typedef.h"
typedef enum
{
	GPIO_PA = 0,
	GPIO_PB,
	//   GPIO_PC,   // no this port
	GPIO_PD,
	GPIO_PE,
	GPIO_PF,
	GPIO_PG,
	GPIO_PH,
	GPIO_PJ,
	GPIO_CH_MAX
} GPIO_CH_E;

#define GPIO_PIN0 0x00000001
#define GPIO_PIN1 0x00000002
#define GPIO_PIN2 0x00000004
#define GPIO_PIN3 0x00000008
#define GPIO_PIN4 0x00000010
#define GPIO_PIN5 0x00000020
#define GPIO_PIN6 0x00000040
#define GPIO_PIN7 0x00000080
#define GPIO_PIN8 0x00000100
#define GPIO_PIN9 0x00000200
#define GPIO_PIN10 0x00000400
#define GPIO_PIN11 0x00000800
#define GPIO_PIN12 0x00001000
#define GPIO_PIN13 0x00002000
#define GPIO_PIN14 0x00004000
#define GPIO_PIN15 0x00008000

typedef enum
{
	GPIO_OUTPUT = 0,
	GPIO_INPUT
} GPIO_DIR_E;

typedef enum
{
	GPIO_PULL_FLOATING = 0,
	GPIO_PULL_UP,
	GPIO_PULL_DOWN,
	GPIO_PULL_UPDOWN,

	GPIO_PULL_MAX
} GPIO_PULL_E;
typedef enum
{
	GPIO_PULLE_FLOATING = 0,
	GPIO_PULLE_UP,
	GPIO_PULLE_DOWN,
	GPIO_PULLE_UPDOWN
} GPIO_PULLE_E;

typedef enum
{
	GPIO_LOW = 0,
	GPIO_HIGH
} GPIO_DATA_E;
typedef enum
{
	GPIO_DRV_NORMAL = 0,
	GPIO_DRV_HIGH
} GPIO_DRV_E;
typedef enum
{
	GPIO_FUNC_GPIO = 0,
	GPIO_FUNC_SFR
} GPIO_FUNC_E;
typedef enum
{
	GPIO_DIGITAL_DIS = 0,
	GPIO_DIGITAL_EN
} GPIO_DIGITAL_E;

typedef enum
{
	GPIO_MAP_UARTTX1 = 0,
	GPIO_MAP_UARTRX1,
	GPIO_MAP_UARTTX0,
	GPIO_MAP_UARTRX0,
	GPIO_MAP_SPI1,
	GPIO_MAP_SPI0,
	GPIO_MAP_SD1,
	GPIO_MAP_SD0,
	GPIO_MAP_TMR3,
	GPIO_MAP_TMR2,
	GPIO_MAP_TMR1,
	GPIO_MAP_TMR0,
	GPIO_MAP_DLL,
	GPIO_MAP_XOSC32K,
	GPIO_MAP_EMI,
	GPIO_MAP_CSI,
	GPIO_MAP_LCD,
	GPIO_MAP_IIC1,
	GPIO_MAP_IIC0
} GPIO_MAP_E;

typedef enum
{
	GPIO_LED0_PA2 = 0,
	GPIO_LED1_PA3,
	GPIO_LED2_PA5,
	GPIO_LED3_PA9,
	GPIO_LED4_PB1,
	GPIO_LED5_PB5,
	GPIO_LED6_PB6,
	GPIO_LED7_PD0,
	GPIO_LED8_PD2,
	GPIO_LED9_PD3,
	GPIO_LED10_PD10,
	GPIO_LED11_PF0,
	GPIO_LED12_PF1,
	GPIO_LED13_PF2,
	GPIO_LED14_PF7,
	GPIO_LED15_PF8,
	GPIO_LED16_PF9,
	GPIO_LED17_PF11,
}GPIO_LED_E;

typedef enum
{
	GPIO_INT0_PA2 = 0,
	GPIO_INT1_PA3,
	GPIO_INT2_PA4,
	GPIO_INT3_PA5,
	GPIO_INT4_PA7,
	GPIO_INT5_PA9,
	GPIO_INT6_PB0,
	GPIO_INT7_PB1,
	GPIO_INT8_PB4,
	GPIO_INT9_PB5,
	GPIO_INT10_PB6,
	GPIO_INT11_PD3,
	GPIO_INT12_PD4,
	GPIO_INT13_PD8,
	GPIO_INT14_PD10,
	GPIO_INT15_PD12,
	GPIO_INT16_PD13,
	GPIO_INT17_PF0,
	GPIO_INT18_PF4,
	GPIO_INT19_PF8,
    GPIO_INT20_PF9,
    GPIO_INT21_PF10,
    GPIO_INT22_PF11,
    GPIO_INT23_PE0,
    GPIO_INT24_PE2,
    GPIO_INT25_PE4,
    GPIO_INT26_PE15,
    GPIO_INT_MAX
}GPIO_INT_E;


typedef enum
{
	UART0_POS_NONE = 0,
	UART0_POS_PA0, 
	UART0_POS_PA1, 
	UART0_POS_PA4,  //3
	UART0_POS_PA5,  //4
	UART0_POS_PA6,  //5
	UART0_POS_PA7,  //6
	UART0_POS_PA9,  //7
	UART0_POS_PA12, //8
	UART0_POS_PB0,  //9
	UART0_POS_PB1,  //10
	UART0_POS_PB5,  //11
	UART0_POS_PB6,  //12
	UART0_POS_PD1,  //13
	UART0_POS_PD2,  //14
	UART0_POS_PD3,  //15
	UART0_POS_PD7,  //16
	UART0_POS_PD10, //17
	UART0_POS_PF0,  //18
	UART0_POS_PF2,  //19
	UART0_POS_PF8,  //20
	UART0_POS_PF10, //21
	UART0_POS_PF11, //22
	UART0_POS_PE0,  //23
	UART0_POS_PE1,  //24
	UART0_POS_PE2,  //25
	UART0_POS_PE3,  //26
}UART0_POS_E;

typedef enum
{
	UART1_POS_NONE = 0,
	UART1_POS_PA4, 
	UART1_POS_PA5, 
	UART1_POS_PA6,  //3
	UART1_POS_PA7,  //4
	UART1_POS_PA9,  //5
	UART1_POS_PA10, //6
	UART1_POS_PA12, //7
	UART1_POS_PB0,  //8
	UART1_POS_PB4,  //9
	UART1_POS_PB5,  //10
	UART1_POS_PB6,  //11
	UART1_POS_PD0,  //12
	UART1_POS_PD8,  //13
	UART1_POS_PD9,  //14
	UART1_POS_PD10, //15
	UART1_POS_PF0,  //16
	UART1_POS_PF1,  //17
	UART1_POS_PF8,  //18
	UART1_POS_PF10, //19
	UART1_POS_PF11, //20
	UART1_POS_PE0,  //21
	UART1_POS_PE1,  //22
	UART1_POS_PE2,  //23
	UART1_POS_PE3,  //24
	UART1_POS_PE15, //25
}UART1_POS_E;

typedef enum
{
	TRIGGER_LEVEL_HIGH = 0,
	TRIGGER_LEVEL_LOW,
	TRIGGER_EDGE_RISING,
	TRIGGER_EDGE_FALLING
} EXT_INT_TRIGGER_E;

enum lcdc_group_e
{
	LCD_POS_NONE_,
	LCD_POS0,
	LCD_POS1,
	LCD_POS2,
	LCD_POS3,
	LCD_POS4,
	LCD_POS5,
	LCD_POS6,
	LCD_POS7,
	LCD_POS8,
	LCD_POS9,
	LCD_POS10,
	LCD_POS11,
	LCD_POS12,
	LCD_POS13,
	LCD_POS14,
};

enum csi_group_e
{
	CSI_POS_NONE_,
	CSI_POS0,
	CSI_POS1,
};

#define GPIO_DATA(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x00) << 2))))
#define GPIO_DIR(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x10) << 2))))
#define GPIO_PLU(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x20) << 2))))
#define GPIO_PLD(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x30) << 2))))
#define GPIO_MAP(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x40) << 2))))
#define GPIO_DRV(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x50) << 2))))
#define GPIO_HYS(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x60) << 2))))
#define GPIO_PEU(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x70) << 2))))
#define GPIO_PED(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0x80) << 2))))
#define GPIO_SFR(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0xD0) << 2))))
#define GPIO_DGL(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0xE0) << 2))))
#define GPIO_INT(a) (*((volatile __sfr unsigned long *)(SPRGROUP_SFR0 + ((a + 0xF0) << 2))))


#define bic_orr(reg, clr, set) (reg) = ((reg) & ~(clr)) | (set)

#define IIC0_POS_NONE 0
#define IIC0_POS_SCL_PE14_SDA_PE15 1
#define IIC0_POS_SCL_PE0_SDA_PE1 2
#define IIC1_POS_NONE 0
#define IIC1_POS_SCL_PA6_SDA_PA7 1
#define IIC1_POS_SCL_PD10_SDA_PD11 2

#define CCP0_POS_NONE 0
#define CCP0_POS_PA8 1
#define CCP0_POS_PD0 2
#define CCP0_POS_PF10 3

#define CCP1_POS_NONE 0
#define CCP1_POS_PA9 1
#define CCP1_POS_PD1 2
#define CCP1_POS_PF11 3

#define CCP2_POS_NONE 0
#define CCP2_POS_PA10 1
#define CCP2_POS_PD2 2
#define CCP2_POS_PF12 3

#define CCP3_POS_NONE 0
#define CCP3_POS_PB1 1
#define CCP3_POS_PD3 2
#define CCP3_POS_PF14 3

#define SPI1_POS_NONE 0
#define SPI1_POS_CLK_PD7_DO_PD6_DI_PD8 1
#define SPI1_POS_CLK_PA3_DO_PA4_DI_PA2 2
#define SPI1_POS_CLK_PD2_D2_PD1_DI_PD0 3

#define SPI0_POS_NONE 0
#define SPI0_POS_CLK_PA13_DO_PA12 1

#define SD0_POS_NONE 0
#define SD0_POS_CLK_PA3_CMD_PA2_D0_PA4_D1_PA5_D2_PA0_D3_PA1 1

#define SD1_POS_NONE 1
#define SD1_POS_CLK_PD9_CMD_PD8_D0_PD12_D1_PD13_D2_PD6_D3_PD7 1
#define SD1_POS_CLK_PD3_CMD_PD2_D0_PD4_D1_PD5_D2_PD0_D3_PD1 2

#define LCD_POS_NONE 0
#define LCD_POS_CS_PF13_WR_PF10_RS_PF14_TE_PF12_DATA_PF7_0 1
#define LCD_POS_CS_PF13_WR_PF10_RS_PF11_TE_PF12_DATA_PD15_6 2

#define CSI_POS_NONE 0
#define CSI_POS_HS_PE12_VS_PE13_MK_PE10_PK_PE7_DATA_PE0_PE11 2
/*******************************************************************************
 * Function Name  : ax32xx_gpioIRQHandler
 * Description    : gpio ext irq handler
 * Input          :
 * Output         : None
 * Return         : none
 *******************************************************************************/
void ax32xx_gpioIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_gpioDirSet
* Description    : set gpio direction
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : dir. ->GPIO_OUTPUT,GPIO_INPUT
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDirSet(u8 ch, u32 pin, u8 dir);
/*******************************************************************************
* Function Name  : ax32xx_gpioPullSet
* Description    : set gpio pull for 10k register
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : pull. ->GPIO_PULL_FLOATING,GPIO_PULL_UP,GPIO_PULL_DOWN,GPIO_PULL_UPDOWN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioPullSet(u8 ch, u32 pin, u8 pull);
void ax32xx_gpioPullSet2(u8 ch, u32 pin, u8 pull);
/*******************************************************************************
* Function Name  : ax32xx_gpioDrvSet
* Description    : set gpio drving
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : drv. ->GPIO_DRV_NORMAL,GPIO_DRV_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDrvSet(u8 ch, u32 pin, u8 drv);
/*******************************************************************************
* Function Name  : ax32xx_gpioDataSet
* Description    : set gpio output data set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : data. ->GPIO_LOW,GPIO_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDataSet(u8 ch, u32 pin, u8 data);
/*******************************************************************************
* Function Name  : ax32xx_gpioDataGrounpSet
* Description    : set gpio output data set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : data. ->GPIO_LOW,GPIO_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDataGrounpSet(u8 ch, u32 pin, u32 data);
/*******************************************************************************
* Function Name  : ax32xx_gpioDataGet
* Description    : set gpio input data get
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
* Output         : None
* Return         : ->GPIO_LOW,GPIO_HIGH
*******************************************************************************/
u8 ax32xx_gpioDataGet(u8 ch, u32 pin);
/*******************************************************************************
* Function Name  : ax32xx_gpioMapSet
* Description    : set gpio map set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : map. ->GPIO_FUNC_GPIO,GPIO_FUNC_SFR
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioMapSet(u8 ch, u32 pin, u8 map);
/*******************************************************************************
* Function Name  : ax32xx_gpioDigitalSet
* Description    : set gpio digital set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : digital. ->GPIO_DIGITAL_DIS,GPIO_DIGITAL_EN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDigitalSet(u8 ch, u32 pin, u8 digital);
/*******************************************************************************
* Function Name  : ax32xx_gpioHystersisSet
* Description    : set gpio hystersis set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : hystersis. 0->disable,1->enable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioHystersisSet(u8 ch, u32 pin, u8 hystersis);
/*******************************************************************************
* Function Name  : ax32xx_gpioLedPull
* Description    : set gpio pull for 300R led set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioLedPull(u8 ch, u32 pin, u8 pull);
/*******************************************************************************
* Function Name  : ax32xx_gpioSFRSet
* Description    : set gpio sfr function
* Input          : u8 type : function table,see : GPIO_MAP_E
					  u8 group: function group
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioSFRSet(u8 type, u8 group);
/*******************************************************************************
* Function Name  : ax32xx_gpioLedInit
* Description    : set gpio hardware led
* Input          : u8 ch : gpio channel->GPIO_LED0_PA8~GPIO_LED11_PF14
				   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
				   u8 soft : soft->0:soft control,1->hardware
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioLedInit(u8 led, u8 pull, u8 soft);
/*******************************************************************************
 * Function Name  : ax32xx_gpioINTCheck
 * Description    : set gpio ext irq
 * Input          : u8 int_no : GPIO_INT0_PA5~GPIO_INT15_15
 * Output         : None
 * Return         : u8 : 1->int occured,0->nothing
 *******************************************************************************/
u8 ax32xx_gpioINTCheck(u8 int_no);
/*******************************************************************************
 * Function Name  : ax32xx_gpioINTClear
 * Description    : clear gpio ext irq
 * Input          : u8 int_no : GPIO_INT0_PA5~GPIO_INT15_15
 * Output         : None
 * Return         : none
 *******************************************************************************/
void ax32xx_gpioINTClear(u8 int_no);
/*******************************************************************************
* Function Name  : ax32xx_gpioINTInit
* Description    : gpio ext int initial
* Input          : u8 int_no : GPIO_INT0_PA5~GPIO_INT15_15
					  u8 trigger :TRIGGER_LEVEL,TRIGGER_EDGE_RISING,TRIGGER_EDGE_FALLING
					  void (*isr)(void) : call back
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioINTInit(u8 int_no, u8 trigger, void (*isr)(void));

#endif

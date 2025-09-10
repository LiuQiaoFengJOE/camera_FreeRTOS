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
* File Name   : ax32xx_gpio.c
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
#include "gpio_ops/gpio.h"
#include "AX329x.h"

#define GPIO_ARG(ch, io)                     \
	{                                        \
		if ((ch > GPIO_PF) || (io > 0xffff)) \
			return;                          \
	}



/*******************************************************************************
* Function Name  : ax32xx_gpioSFRSet
* Description    : set gpio sfr function
* Input          : u8 type : function table,see : GPIO_MAP_E
					  u8 group: function group
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioSFRSet(u8 type, u8 group)
{
	switch (type)
	{
	case GPIO_MAP_UARTTX1:
		bic_orr(PMAPCON0, (0x1F << 15), (group & 0x1F) << 15);
		break;
	case GPIO_MAP_UARTRX1:
		bic_orr(PMAPCON0, (0x1F << 10), (group & 0x1F) << 10);
		break;
	case GPIO_MAP_UARTTX0:
		bic_orr(PMAPCON0, (0x1F << 5), (group & 0x1F) << 5);
		break;
	case GPIO_MAP_UARTRX0:
		bic_orr(PMAPCON0, (0x1F << 0), (group & 0x1F) << 0);
		break;

	case GPIO_MAP_SPI1:
		bic_orr(PMAPCON1, (0x7 << 22), (group & 0x7) << 22);
		break;
	case GPIO_MAP_SPI0:
		if (group > 2)
			group--;
		bic_orr(PMAPCON1, (0x3 << 20), (group & 0x3) << 20);
		break;
	case GPIO_MAP_SD1:
		bic_orr(PMAPCON1, (0x7 << 17), (group & 0x7) << 17);
		break;
	case GPIO_MAP_SD0:
		bic_orr(PMAPCON1, (0x1 << 16), (group & 0x1) << 16);
		break;
	case GPIO_MAP_TMR3:
		bic_orr(PMAPCON1, (0xF << 12), (group & 0xF) << 12);
		break;
	case GPIO_MAP_TMR2:
		bic_orr(PMAPCON1, (0xF << 8), (group & 0xF) << 8);
		break;
	case GPIO_MAP_TMR1:
		bic_orr(PMAPCON1, (0xF << 4), (group & 0xF) << 4);
		break;
	case GPIO_MAP_TMR0:
		bic_orr(PMAPCON1, (0xF << 0), (group & 0xF) << 0);
		break;

	case GPIO_MAP_DLL:
		bic_orr(PMAPCON2, (0x1 << 20), (group & 0x1) << 20);
		break;
	case GPIO_MAP_XOSC32K:
		bic_orr(PMAPCON2, (0x7 << 17), (group & 0x7) << 17);
		break;
	case GPIO_MAP_EMI:
		bic_orr(PMAPCON2, (0x7 << 14), (group & 0x7) << 14);
		break;
	case GPIO_MAP_CSI:
		bic_orr(PMAPCON2, (0x3 << 10), (group & 0x3) << 10);
		break;
	case GPIO_MAP_LCD:
		bic_orr(PMAPCON2, (0xF << 6), (group & 0xF) << 6);
		break;
	case GPIO_MAP_IIC1:
		bic_orr(PMAPCON2, (0x7 << 3), (group & 0x7) << 3);
		break;
	case GPIO_MAP_IIC0:
		bic_orr(PMAPCON2, (0x7 << 0), (group & 0x7) << 0);
		break;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_gpioDirSet
* Description    : set gpio direction
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : dir. ->GPIO_OUTPUT,GPIO_INPUT
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDirSet(u8 ch, u32 pin, u8 dir)
{
	GPIO_ARG(ch, pin);

	if (dir == GPIO_OUTPUT)
		GPIO_DIR(ch) &= ~(pin);
	else if (dir == GPIO_INPUT)
		GPIO_DIR(ch) |= pin;
}
/*******************************************************************************
* Function Name  : ax32xx_gpioPullSet
* Description    : set gpio pull for 10k register
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : pull. ->GPIO_PULL_FLOATING,GPIO_PULL_UP,GPIO_PULL_DOWN,GPIO_PULL_UPDOWN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioPullSet(u8 ch, u32 pin, u8 pull)
{
	GPIO_ARG(ch, pin);

	if (pull & GPIO_PULL_UP)
		GPIO_PLU(ch) |= (pin);
	else
		GPIO_PLU(ch) &= ~(pin);

	if ((pull & GPIO_PULL_DOWN))
		GPIO_PLD(ch) |= (pin);
	else
		GPIO_PLD(ch) &= ~(pin);
}

// 设置是否上拉10K
void ax32xx_gpioPullSet2(u8 ch, u32 pin, u8 pull)
{
	GPIO_ARG(ch, pin);

	if (pull & GPIO_PULL_UP)
		GPIO_PLU(ch) |= (pin);
	else
		GPIO_PLU(ch) &= ~(pin);
}

/*******************************************************************************
* Function Name  : ax32xx_gpioDrvSet
* Description    : set gpio drving
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : drv. ->GPIO_DRV_NORMAL,GPIO_DRV_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDrvSet(u8 ch, u32 pin, u8 drv)
{
	GPIO_ARG(ch, pin);

	if (drv)
		GPIO_DRV(ch) |= (pin);
	else
		GPIO_DRV(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioDataSet
* Description    : set gpio output data set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : data. ->GPIO_LOW,GPIO_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDataSet(u8 ch, u32 pin, u8 data)
{
	GPIO_ARG(ch, pin);

	if (data)
		GPIO_DATA(ch) |= pin;
	else
		GPIO_DATA(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioDataGrounpSet
* Description    : set gpio output data set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : data. ->GPIO_LOW,GPIO_HIGH
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDataGrounpSet(u8 ch, u32 pin, u32 data)
{
	GPIO_ARG(ch, pin);

	GPIO_DATA(ch) = data | (GPIO_DATA(ch) & (~(pin)));
}
/*******************************************************************************
* Function Name  : ax32xx_gpioDataGet
* Description    : set gpio input data get
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
* Output         : None
* Return         : ->GPIO_LOW,GPIO_HIGH
*******************************************************************************/
u8 ax32xx_gpioDataGet(u8 ch, u32 pin)
{
	if ((ch > GPIO_PG) || (pin > GPIO_PIN15))
		return 0;
	if (GPIO_DATA(ch) & (pin))
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_gpioMapSet
* Description    : set gpio map set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : map. ->GPIO_FUNC_GPIO,GPIO_FUNC_SFR
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioMapSet(u8 ch, u32 pin, u8 map)
{
	GPIO_ARG(ch, pin);

	if (map)
		GPIO_MAP(ch) |= pin;
	else
		GPIO_MAP(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioDigitalSet
* Description    : set gpio digital set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : digital. ->GPIO_DIGITAL_DIS,GPIO_DIGITAL_EN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioDigitalSet(u8 ch, u32 pin, u8 digital)
{
	GPIO_ARG(ch, pin);

	if (digital)
		GPIO_DGL(ch) |= pin;
	else
		GPIO_DGL(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioHystersisSet
* Description    : set gpio hystersis set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : hystersis. 0->disable,1->enable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioHystersisSet(u8 ch, u32 pin, u8 hystersis)
{
	GPIO_ARG(ch, pin);

	if (hystersis)
		GPIO_HYS(ch) |= pin;
	else
		GPIO_HYS(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioLedPull
* Description    : set gpio pull for 300R led set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioLedPull(u8 ch, u32 pin, u8 pull)
{
	GPIO_ARG(ch, pin);

	if (pull & GPIO_PULLE_UP)
		GPIO_PEU(ch) |= pin;
	else
		GPIO_PEU(ch) &= ~(pin);

	if (pull & GPIO_PULLE_DOWN)
		GPIO_PED(ch) |= pin;
	else
		GPIO_PED(ch) &= ~(pin);
}
/*******************************************************************************
* Function Name  : ax32xx_gpioLedInit
* Description    : set gpio hardware led
* Input          : u8 ch : gpio channel->GPIO_LED0_PA8~GPIO_LED11_PF14
				   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
				   u8 soft : soft->0:soft control,1->hardware
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_gpioLedInit(u8 led, u8 pull, u8 soft)
{
	if (pull & GPIO_PULLE_UP)
	{
		if (soft)
			GPIO_SFR(3) |= (1 << led);
		else
			GPIO_SFR(3) &= ~(1 << led);
	}
	if (pull & GPIO_PULLE_DOWN)
	{
		if (soft)
			GPIO_SFR(4) |= (1 << led);
		else
			GPIO_SFR(4) &= ~(1 << led);
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_gpioINTCheck
 * Description    : set gpio ext irq
 * Input          : u8 int_no : GPIO_INT0_PA5~GPIO_INT15_15
 * Output         : None
 * Return         : u8 : 1->int occured,0->nothing
 *******************************************************************************/
u8 ax32xx_gpioINTCheck(u8 int_no)
{
	if (GPIO_INT(3) & (1 << int_no))
		return 1;
	else
		return 0;
}
/*******************************************************************************
 * Function Name  : ax32xx_gpioINTClear
 * Description    : clear gpio ext irq
 * Input          : u8 int_no : GPIO_INT0_PA5~GPIO_INT15_15
 * Output         : None
 * Return         : none
 *******************************************************************************/
void ax32xx_gpioINTClear(u8 int_no)
{
	GPIO_INT(3) = 1 << (int_no + 16);
}

#define R_DAC_CTRL0 DACACON0
#define R_DAC_CTRL1 DACACON1
#define R_DAC_CTRL2 DACACON2
void ax32xx_dacHPSet(u8 en, u32 level)
{

	if (en)
	{
		level &= 0x07;
		R_DAC_CTRL0 |= (3 << 0);
		R_DAC_CTRL2 &= ~(1 << 8);
		R_DAC_CTRL0 = (R_DAC_CTRL0 & ~((7 << 27))) | (level << 27) | (1 << 10);
	}
	else
	{
		// R_DAC_CTRL0 &= ~(3<<0);
		R_DAC_CTRL0 &= ~(1 << 10);
		// R_DAC_CTRL2 &=~(1<<8);
		R_DAC_CTRL2 |= (1 << 8);
	}
}

void hal_gpioInit(u8 ch, u32 pin, u8 dir, u8 pull)
{
	ax32xx_gpioMapSet(ch, pin, GPIO_FUNC_GPIO);		  // set as gpio func
	ax32xx_gpioDrvSet(ch, pin, GPIO_DRV_NORMAL);	  // set normal drving
	ax32xx_gpioDirSet(ch, pin, dir);				  // set direction
	ax32xx_gpioLedPull(ch, pin, GPIO_PULLE_FLOATING); // set LED pull level,
	ax32xx_gpioPullSet(ch, pin, pull);				  // set pull level,
}
void hal_gpioEPullSet(u8 ch, u32 pin, u8 led_pull)
{
	ax32xx_gpioPullSet(ch, pin, GPIO_PULL_FLOATING); // set pull level,

	ax32xx_gpioLedPull(ch, pin, led_pull); // set pull level,
}
void hal_gpioWrite(u8 ch, u32 pin, u8 data)
{
	ax32xx_gpioDataSet(ch, pin, data);
}
u8 hal_gpioRead(u8 ch, u32 pin)
{
	return ax32xx_gpioDataGet(ch, pin);
}

/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                         IIC DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_iic.c
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN IIC HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"
#include "gpio_ops/gpio.h"
#include "sys/sys.h"

extern UINT32 APB_CLK; // defined by board

#define I2CREG(base, offset) (SFR_AT(base + (offset)*4))

#define IICXBAUD 0
#define IICXCON 1
#define IICXRDATA 2
#define IICXTDATA 3
#define IICXCMD 4
#define IICXSTA 5

typedef struct
{
    void *lock; // sys/drv/i2c.c要求controller首字必須為lock，其餘字段隨意
    u32 base_addr;
    u8 i2c_indx; // i2c 0 / i2c 1
    u8 group;
    u8 port;
    u32 data_pin;
    u32 clk_pin;
} i2c_controller;

i2c_controller i2c0_g0 =
    {
        .base_addr = (u32)&IIC0BAUD,
        .i2c_indx = GPIO_MAP_IIC0,
        .port = GPIO_PE,
        .data_pin = GPIO_PIN15,
        .clk_pin = GPIO_PIN13,
        .group = 3,
};

// todo
void *i2c_get_controller(int num)
{
    if (num == 0)
        return &i2c0_g0;
    return NULL;
}

#define get_addr(c) ((i2c_controller *)(c))->base_addr
#define get_port(c) ((i2c_controller *)(c))->port
#define get_data_pin(c) ((i2c_controller *)(c))->data_pin
#define get_clk_pin(c) ((i2c_controller *)(c))->clk_pin
#define get_i2c_indx(c) ((i2c_controller *)(c))->i2c_indx
#define get_i2c_group(c) ((i2c_controller *)(c))->group

/*******************************************************************************
 * Function Name  : ax32xx_iic0Init
 * Description    : iic 0 initial
 * Input          : u32 baudrate : iic0 baudrate
 * Output         : None
 * Return         : none
 *******************************************************************************/
void enable_i2c(u32 base)
{
    I2CREG(base, IICXCON) |= BIT(7);
}

void iicInit(i2c_controller *c, u32 baudrate)
{
    u32 base = get_addr(c);
    u32 port = get_port(c);
    u32 data_pin = get_data_pin(c);
    u32 clk_pin = get_clk_pin(c);
    u32 i2c_indx = get_i2c_indx(c);
    u32 group = get_i2c_group(c);

    ax32xx_gpioSFRSet(i2c_indx, group); //

    ax32xx_gpioMapSet(port, clk_pin, GPIO_FUNC_SFR);
    ax32xx_gpioMapSet(port, data_pin, GPIO_FUNC_SFR);

    ax32xx_gpioDataSet(port, clk_pin, GPIO_HIGH);
    ax32xx_gpioDataSet(port, data_pin, GPIO_HIGH);

    ax32xx_gpioDirSet(port, clk_pin, GPIO_OUTPUT);
    ax32xx_gpioDirSet(port, data_pin, GPIO_INPUT);

    ax32xx_gpioPullSet(port, clk_pin, GPIO_PULL_UP);
    ax32xx_gpioPullSet(port, data_pin, GPIO_PULL_UP);
    I2CREG(base, IICXBAUD) = (APB_CLK / ((5 * baudrate)) - 1);
    enable_i2c(base);
}

/*******************************************************************************
 * Function Name  : ax32xx_iic0Uninit
 * Description    : iic 0 uninitial
 * Input          :  none
 * Output         : None
 * Return         : none
 *******************************************************************************/

void iicUninit(i2c_controller *c)
{
    u32 base = get_addr(c);
    u32 i2c_indx = get_i2c_indx(c);
    ax32xx_gpioSFRSet(i2c_indx, 0);
    I2CREG(base, IICXCON) &= ~(1 << 7);
}

/*******************************************************************************
 * Function Name  : ax32xx_iic0Start
 * Description    : iic 0 send start singal
 * Input          :  none
 * Output         : None
 * Return         : none
 *******************************************************************************/

void iicStart(i2c_controller *c)
{
    u32 base = get_addr(c);
    I2CREG(base, IICXCMD) = (1 << 7);
}
/*******************************************************************************
 * Function Name  : ax32xx_iic0Stop
 * Description    : iic 0 send stop singal
 * Input          :  none
 * Output         : None
 * Return         : none
 *******************************************************************************/

void iicStop(i2c_controller *c)
{
    u32 base = get_addr(c);

    I2CREG(base, IICXCMD) = (1 << 6); // generate stop signal

    while (!(I2CREG(base, IICXSTA) & (1)))
    {
    };

    I2CREG(base, IICXCMD) = (1);
}
/*******************************************************************************
 * Function Name  : ax32xx_iic0Stop
 * Description    : iic 0 recv ack singal
 * Input          :  none
 * Output         : None
 * Return         : 0->ack,1->nack
 *******************************************************************************/

u8 iicRecvACK(i2c_controller *c)
{
    u32 base = get_addr(c);
    return (!(I2CREG(base, IICXSTA) & (1 << 7)));
}
/*******************************************************************************
 * Function Name  : ax32xx_iic0SendACK
 * Description    : iic 0 send ack singal
 * Input          :  none
 * Output         : None
 * Return         : none
 *******************************************************************************/

void iicSendACK(i2c_controller *c)
{
    u32 base = get_addr(c);
    I2CREG(base, IICXCMD) |= (1 << 3);
    //    while(!(IIC0STA & 1));
}
/*******************************************************************************
 * Function Name  : ax32xx_iic0SendByte
 * Description    : iic 0 send one byte
 * Input          :  u8 byte : byte
 * Output         : None
 * Return         : none
 *******************************************************************************/
void test_iic()
{
    PFFMAP &= ~BIT(1);
    PFDIR &= ~BIT(1);
    PORTF |= BIT(1);
}

void iicSendByte(i2c_controller *c, u8 byte)
{
    u32 base = get_addr(c);
    I2CREG(base, IICXTDATA) = byte;
    I2CREG(base, IICXCMD) |= (1 << 4);
    while (!(I2CREG(base, IICXSTA) & 1))
        ;
    I2CREG(base, IICXCMD) = 1;
}
/*******************************************************************************
 * Function Name  : ax32xx_iic0RecvByte
 * Description    : iic 0 recv one byte
 * Input          :  none
 * Output         : None
 * Return         : u8 byte : byte
 *******************************************************************************/

u8 iicRecvByte(i2c_controller *c)
{
    u32 base = get_addr(c);
    I2CREG(base, IICXCMD) = (1 << 3) | (1 << 5); //
    while (!(I2CREG(base, IICXSTA) & 1))
        ;
    I2CREG(base, IICXCMD) = 1;
    return (I2CREG(base, IICXRDATA) & 0xff);
}

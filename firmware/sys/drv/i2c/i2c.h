/****************************************************************************
**
**                         BUILDWIN HAL LAYER
** *   **                     THE APPOTECH HAL
**** **                         IIC HAL LAYER DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : hal_iic.h
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
#ifndef HAL_IIC_H
#define HAL_IIC_H
// typedef struct{
//     u32 base_addr;
//     u32 i2c_indx; // i2c 0 / i2c 1
//     u32 port;
//     u32 data_pin;
//     u32 clk_pin;
//     u32 group;
// }i2c_controller;
#define i2c_controller void *
void enable_i2c(u32 base);
void iicInit(i2c_controller *c, u32 baudrate);
void iicUninit(i2c_controller *c);
void iicStart(i2c_controller *c);
void iicStop(i2c_controller *c);
u8 iicRecvACK(i2c_controller *c);
void iicSendACK(i2c_controller *c);
void iicSendByte(i2c_controller *c, u8 byte);
u8 iicRecvByte(i2c_controller *c);

#endif

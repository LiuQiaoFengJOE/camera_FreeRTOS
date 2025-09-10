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
* File Name   : hal_iic.c
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
#include "basic_defs.h"
#include "osal/osal_mutex.h"
#include "i2c.h"

typedef struct
{
     struct mutex *lock;
} i2c_controller_base;
#define HAL_IIC0_LOCK() ({                                          \
     i2c_controller_base *ctrl = (i2c_controller_base *)controller; \
     osal_mutex_lock(ctrl->lock, -1);                               \
})
#define HAL_IIC0_UNLOCK() ({                                        \
     i2c_controller_base *ctrl = (i2c_controller_base *)controller; \
     osal_mutex_unlock(ctrl->lock);                                 \
})

#if HAL_CFG_EN_IIC1 > 0
static u8 halIIC1ShareFlag = 0;
#endif
#if 0
static volatile u8 halIIC0Lock = 0;
#define HAL_IIC0_LOCK() \
     if (halIIC0Lock)   \
     {                  \
          return;       \
     }                  \
     halIIC0Lock = 1;
#define HAL_IIC0_UNLOCK() halIIC0Lock = 0;
#endif

/*******************************************************************************
 * Function Name  : hal_iicInit
 * Description    : hal layer .iic0 initial
 * Input          : none
 * Output         : None
 * Return         : none
 *******************************************************************************/
void i2cInit(void *controller, u32 baudrate)
{
     i2c_controller_base *ctrl = (i2c_controller_base *)controller;
     ctrl->lock = osal_mutex_create();
     iicInit(controller, baudrate);

     iicStop(controller);
}
/*******************************************************************************
 * Function Name  : hal_iicUninit
 * Description    : hal layer .iic0 uninitial
 * Input          : none
 * Output         : None
 * Return         : none
 *******************************************************************************/
void i2cUninit(void *controller)
{
     iicUninit(controller);
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWriteData
* Description    : hal layer .iic0 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void i2c8bitAddrWriteData(void *controller, u8 slaveid, u8 addr, u8 data)
{
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid

     iicRecvACK(controller); // recv ack singal

     iicSendByte(controller, addr); // send address

     iicRecvACK(controller); // recv ack singal

     iicSendByte(controller, data);
     iicRecvACK(controller);

     iicStop(controller);

     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrReadData
* Description    : hal layer .iic0 read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic8bitAddrReadData(void *controller, u8 slaveid, u8 addr, u8 *data)
{
     u8 temp;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid
     iicRecvACK(controller);           // recv ack singal

     iicSendByte(controller, addr); // send address
     iicRecvACK(controller);        // recv ack singal

     iicStop(controller); // stop

     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid | 1); // send slaveid
     iicRecvACK(controller);               // recv ack singal

     temp = iicRecvByte(controller);
     //     ax32xx_iic1SendACK();

     iicStop(controller);

     if (data)
          *data = temp;
     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWrite
* Description    : hal layer .iic0 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void i2c8bitAddrWrite(void *controller, u8 slaveid, u8 addr, u8 *data, u8 len)
{
     int i;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid

     iicRecvACK(controller); // recv ack singal

     iicSendByte(controller, addr); // send address

     iicRecvACK(controller); // recv ack singal
     for (i = 0; i < len; i++)
     {
          iicSendByte(controller, data[i]); // send data
          iicRecvACK(controller);           // recv ack
     }

     iicStop(controller); // send stop singal
     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrRead
* Description    : hal layer .iic0 read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void i2c8bitAddrRead(void *controller, u8 slaveid, u8 addr, u8 *data, u8 len)
{
     int i;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid
     iicRecvACK(controller);           // recv ack singal

     iicSendByte(controller, addr); // send address
     iicRecvACK(controller);        // recv ack singal

     iicStop(controller); // stop

     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid | 1); // send slaveid
     iicRecvACK(controller);               // recv ack singal

     for (i = 0; i < len; i++)
     {
          data[i] = iicRecvByte(controller); // send data
                                             // iicSendACK();              // recv ack
     }

     iicStop(controller); // send stop singal
     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWriteData
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void i2c16bitAddrWriteData(void *controller, u8 slaveid, u16 addr, u8 data)
{
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid

     iicRecvACK(controller); // recv ack singal

     iicSendByte(controller, addr >> 8); // send address
     iicRecvACK(controller);             // recv ack singal

     iicSendByte(controller, addr & 0xff); // send address
     iicRecvACK(controller);               // recv ack singal

     iicSendByte(controller, data);
     iicRecvACK(controller);

     iicStop(controller);
     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrReadData
* Description    : hal layer .iic0 read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void i2c16bitAddrReadData(void *controller, u8 slaveid, u16 addr, u8 *data)
{
     u8 temp;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid
     iicRecvACK(controller);           // recv ack singal

     iicSendByte(controller, addr >> 8); // send address
     iicRecvACK(controller);             // recv ack singal

     iicSendByte(controller, addr & 0xff); // send address
     iicRecvACK(controller);               // recv ack singal

     iicStop(controller); // stop

     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid | 1); // send slaveid
     iicRecvACK(controller);               // recv ack singal

     temp = iicRecvByte(controller);
     //     ax32xx_iic1SendACK();

     iicStop(controller);

     if (data)
          *data = temp;
     HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWrite
* Description    : hal layer .iic0 write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void i2c16bitAddrWrite(void *controller, u8 slaveid, u16 addr, u8 *data, u8 len)
{
     int i;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid
     iicRecvACK(controller);           // recv ack singal

     iicSendByte(controller, addr >> 8); // send address
     iicRecvACK(controller);             // recv ack singal

     iicSendByte(controller, addr & 0xff); // send address
     iicRecvACK(controller);               // recv ack singal

     for (i = 0; i < len; i++)
     {
          iicSendByte(controller, data[i]); // send data
          iicRecvACK(controller);           // recv ack
     }

     iicStop(controller); // send stop singal
     HAL_IIC0_UNLOCK();
}

/*******************************************************************************
* Function Name  : hal_iic16bitAddrRead
* Description    : hal layer .iic0 read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void i2c16bitAddrRead(void *controller, u8 slaveid, u16 addr, u8 *data, u8 len)
{
     int i;
     HAL_IIC0_LOCK();
     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid); // send slaveid
     iicRecvACK(controller);           // recv ack singal

     iicSendByte(controller, addr >> 8); // send address
     iicRecvACK(controller);             // recv ack singal

     iicSendByte(controller, addr & 0xff); // send address
     iicRecvACK(controller);               // recv ack singal

     iicStop(controller); // stop

     iicStart(controller); // send Start singal

     iicSendByte(controller, slaveid | 1); // send slaveid
     iicRecvACK(controller);               // recv ack singal

     for (i = 0; i < len; i++)
     {
          data[i] = iicRecvByte(controller); // send data
                                             //    ax32xx_iic1SendACK();              // recv ack
     }

     iicStop(controller); // send stop singal
     HAL_IIC0_UNLOCK();
}

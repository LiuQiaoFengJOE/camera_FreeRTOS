#ifndef __CMOS_SENSOR_H__
#define __CMOS_SENSOR_H__

#include "basic_defs.h"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
#include "sys/sys.h"

#define Sensor_Op_t _Sensor_Adpt_
#define Sensor_Init_t _Sensor_Ident_

#define  	CMOS_USB_ONLINE_DBG      		1

#define CMOS_VGA_SUPPORT 1
#define CMOS_720P_SUPPORT 1
#define CMOS_ONLINE_DBG 0

#ifndef ALIGNED
#define ALIGNED(x) __attribute__((aligned(x)))
#endif

#ifndef SECTION
#define SECTION(s) __attribute__((section(s)))
#endif

// #define SENSOR_HEADER_SECTION              SECTION(".sensor_res.header")
// #define SENSOR_HEADER_ITEM_SECTION         SECTION(".sensor_res.header.items")/**/
//
// #define SENSOR_OP_SECTION                  SECTION(".sensor_res.struct")  static
// #define SENSOR_INIT_SECTION                SECTION(".sensor_res.init_tab")  static
//
// #define SENSOR_FUNC_SECTION                SECTION(".text.func")
// #define  _SEN_YGAMA_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static
// #define  _SEN_RGBGAMA_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static

#define SENSOR_HEADER_SECTION static
#define SENSOR_HEADER_COMMON SECTION(".sensor_res.header.common")
#define SENSOR_HEADER_ITEM_SECTION SECTION(".sensor.header.items") /**/
#define SENSOR_OP_SECTION static
#define SENSOR_INIT_SECTION static
#define SENSOR_FUNC_SECTION static
#define SENSOR_DATA_SECTION SECTION(".data") static
#define _SEN_YGAMA_TABLE_ static
#define _SEN_RGBGAMA_TABLE_ static
#define _SEN_LSC_TABLE_ static
#define _SEN_AWB_TABLE_ static

#define CMOS_INIT_LEN_MAX 4096

#define _SEN_YGAMA_CLASSES_ 8
#define _SEN_RGBGAMA_CLASSES_ 8
#define _YGAMA_STEPS_ 256
#define _RGB_GAMA_STEPS_ 256

#define DEV_SENSOR_BF2013 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF3703 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF3A03 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0307 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0308 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0309 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0328 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_HM1055 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_IT03A1 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_OV7670 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_OV7725 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SIV100B 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SIV120B 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SIV121DS 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF20A6 1 & CMOS_VGA_SUPPORT

#define DEV_SENSOR_SP1409 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_SC1045 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_GC1004 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_GC1054 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_GC1064 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_GC1084 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H42 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H62 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H63 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H63P 1 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H65 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_H7640 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_BF3016 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_OV9710 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_OV9732 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_NT99141 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_NT99142 0 & CMOS_720P_SUPPORT
#define DEV_SENSOR_SC2331	0 & CMOS_720P_SUPPORT

/*******************************************************************************
 * Function Name  : sensor_iic_enable
 * Description    : enable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_enable(void);
/*******************************************************************************
 * Function Name  : sensor_iic_disable
 * Description    : disable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_disable(void);
/*******************************************************************************
 * Function Name  : sensor_iic_write
 * Description    : senor iic  write
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_write(INT8U *data);
/*******************************************************************************
 * Function Name  : sensor_iic_read
 * Description    : senor iic  read
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
uint32_t sensor_iic_read(uint8_t *data);
/*******************************************************************************
* Function Name  : sensor_iic_info
* Description    : senor iic  slave set
* Input          : INT8U wid : write id
                      INT8U rid  : read id
                      INT8U awidth : addr width
                      INT8U dwidth : data width
* Output         : none
* Return         : none
*******************************************************************************/
void sensor_iic_info(INT8U wid, INT8U rid, INT8U awidth, INT8U dwidth);
/*******************************************************************************
 * Function Name  : sensorInit
 * Description    : initial cmos sensor
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensorInit(void *);
/*******************************************************************************
 * Function Name  : uart_to_iic
 * Description    :
 * Output         : None
 * Return         :
 *******************************************************************************/
INT8U *uart_to_iic(u8 *p);

/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug initial
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGInit(void);
/*******************************************************************************
 * Function Name  : sesnorDBGService
 * Description	  : sensor on line debug service.user should not call this function in isr
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sesnorDBGService(void);
/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug timeout check
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGTimeout(void);
/*******************************************************************************
 * Function Name  : sensorDBGRecv
 * Description	  : sensor on line debug recivce
 * Input		  : u8 data : data
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGRecv(u8 data);

void sensorGetRes(u32 *w,u32 *h);


#endif /* __CMOS_SENSOR_H__ */

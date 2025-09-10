/****************************************************************************
**
**                         BUILDWIN HAL LAYER
** *   **                  THE APPOTECH HAL
**** **                 ISP HAL LAYER DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : hal_isp.h
* Author      :
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN ISP HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef HAL_ISP_H
#define HAL_ISP_H

void isp_cfg(void);
void isp_process_check(void);
void isp_table_load(u32 gma_num, u32 contra_num, u32 rgbgma_num);
void isp_all_model_cfg(u32 isp_all_mod);
bool exposal_adj(u32 yloga_avg, u32 yloga, u32 *p_br, u32 exp_ext_mod);
// void awb_manual_gain_adapt(u32 awb_scene_mod,u32 *pbgain,u32 *pggain,u32 *prgain);
void awb_manual_gain_adapt(u32 awb_scene_mod);

void awb_scene_mod_set(u32 awb_scene_mod);
void sensor_compensate_exp_set(u32 compensate_mod);
u8 get_scen_light(void); // 0~255  0: dark  255:light
void sensor_frame_rate_adapt(u32 frequecy_mode, u32 frame_rate);
extern bool (*fp_sensor_exposure)(u32 yloga_avg, u32 yloga, u32 *p_br, u32 exp_ext_mod);
void uart_to_isp(u8 *p);

void isp_init(void);

int hal_ispFrameCnt(u8 clear);

void hal_ispService(void);
#define _EVSTEP_TAB_LEN_ 132 // 68
extern const u16 Ratio_of_Evstep[_EVSTEP_TAB_LEN_];

extern int _sensor_resource_start_addr;
#define SEN_RES_FLASH_ADDR(x) ((u32)(x) + ((u32)&_sensor_resource_start_addr))

#endif

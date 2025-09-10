/**@file board_config.h
 * @brief 板級配置文件
 * @details 根據PCB板的實際設計，對各模塊進行配置
 *
 * 相關的定義在../board_def.h文件中
 */

#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "../board_def.h"
#include "ax329x.h"

#define APB_CLK_CONFIG 144000000L
#define PLL_CLK (APB_CLK_CONFIG * 2)

// 定义在ax3292x.h,要匹配

#define AX3268 3268
#define AX3291 3291
#define AX3297 3297

#define BOARD_WIFI_RTL8189FTV 0
#define BOARD_WIFI_SV6030 1
#define BOARD_WIFI_ATBM 2
#define BOARD_WIFI_SV6152 3
#define BOARD_WIFI_SV6158 4
#define BOARD_WIFI_ZT9101  5

#define DVRUNNING 0 // 带屏
#define FLIGHT_MODE 1
#define USENSOR_MODE 2


#define WIFI_SUPPORT_VERSION BOARD_WIFI_ATBM//BOARD_WIFI_ZT9101
#define WIFI_SUPPORT_MODE DVRUNNING
#define BOARD_TYPE AX329X_TYPE // 定义在ax3292x.h


/* 飞控收发串口用btuart */
#define FLY_UARTTYPE UARTTYPE_BT
#define FLY_UARTGRP_TX BTUART_GRP_0
#define FLY_UARTGRP_RX BTUART_GRP_10
#define FLY_UARTBAUD 19200

/* Tick Timer */
#define TICKTIMER_TYPE TIMERTYPE_16BIT
#define TICKTIMER_SELECT TIMER16BIT_0
#define TICKTIMER_IRQNO TICKTIMER_SELECT

#define STARTUART 0
#define STARTRECLED 1
#define STARTRECLED1 2

#define LED_MODE STARTUART
#ifndef __ASSEMBLER__
extern struct spi_device spi0_device;
#endif

#define spidisk_device spi0_device

//下面两个宏互斥
#define DEBUG_WIFI_MODULE      0
#define FINSH_RX_ENABLE        0

// 是否使能相框
#define FRAME_ENABLE 1
#define LCD_EN  1

/////////////////////////////////////////led config//////////////////////////////////////////////////////
#define UVC_ERR_LED (BIT(0))
#define WIFI_ERR_LED (BIT(2))
#define SYS_ERR_LED (BIT(4))
#define SDIO_ERR_LED (BIT(6))

#define UVC_OK (BIT(1))
#define WIFI_OK (BIT(3))
#define SYS_OK (BIT(5))
#define SDIO_OK (BIT(7))

#define LED_ERR_UVC 0x321f40c8
#define LED_ERR_WIFI 0x321f40ff
#define LED_ERR_SYS 0x321f40c0
#define LED_ERR_SDIO 0x321f40ff


#endif /* __BOARD_CONFIG_H__ */

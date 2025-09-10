/**
 ******************************************************************************
 * @file    main.h
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    07-October-2011
 * @brief   This file contains all the functions prototypes for the main.c
 *          file.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* WLAN CONFIG ---------------------------------------------------------------*/
#define CONFIG_WLAN 1

/* Includes ------------------------------------------------------------------*/
#include "typedef.h"
#if !CONFIG_WLAN
#include "xxx.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

// Disable LCD when using ext RAM
// #define USE_LCD        /* enable LCD  */

/* Uncomment SERIAL_DEBUG to enables retarget of printf to serial port for debug purpose */
#define SERIAL_DEBUG
#define SERIAL_DEBUG_RX

/* Uncomment the line below if you will use the I2C peripheral as a Master */
// #define I2C_MASTER
// #define USE_PIR
// #define WDG_ENABLE
#if CONFIG_WLAN
#include <autoconf.h>
#define STA_MODE_SSID "wifi_wq"            /* Set SSID here */
#define AP_MODE_SSID "Buildwin_AX3268_FTV" /* Set SSID here */
#define AP_DEFAULT_CH 2
#define WLAN0_NAME "wlan0"
#define WLAN1_NAME "wlan1"
#define WPA_PASSPHRASE "12345678" /* Max 32 characters */
#define WEP40_KEY "12345"         /* 5 ASCII characters */
  // #ifdef CONFIG_CONCURRENT_MODE

#endif

/* MAC ADDRESS*/
#define MAC_ADDR0 02
#define MAC_ADDR1 00
#define MAC_ADDR2 00
#define MAC_ADDR3 00
#define MAC_ADDR4 00
#define MAC_ADDR5 00

/*Static IP ADDRESS*/
#define IP_ADDR0 192
#define IP_ADDR1 168
#define IP_ADDR2 1
#define IP_ADDR3 50

/*NETMASK*/
#define NETMASK_ADDR0 255
#define NETMASK_ADDR1 255
#define NETMASK_ADDR2 255
#define NETMASK_ADDR3 0

/*Gateway Address*/
#define GW_ADDR0 192
#define GW_ADDR1 168
#define GW_ADDR2 1
#define GW_ADDR3 1

/*******************************************/

/*Static IP ADDRESS*/
#define AP_IP_ADDR0 192
#define AP_IP_ADDR1 168
#define AP_IP_ADDR2 43
#define AP_IP_ADDR3 1

/*NETMASK*/
#define AP_NETMASK_ADDR0 255
#define AP_NETMASK_ADDR1 255
#define AP_NETMASK_ADDR2 255
#define AP_NETMASK_ADDR3 0

/*Gateway Address*/
#define AP_GW_ADDR0 192
#define AP_GW_ADDR1 168
#define AP_GW_ADDR2 43
#define AP_GW_ADDR3 1

  /*******************************************/
  /* MII and RMII mode selection, for STM324xG-EVAL Board(MB786) RevB ***********/
  // #define RMII_MODE  // User have to provide the 50 MHz clock by soldering a 50 MHz
  //  oscillator (ref SM7745HEV-50.0M or equivalent) on the U3
  //  footprint located under CN3 and also removing jumper on JP5.
  //  This oscillator is not provided with the board.
  //  For more details, please refer to STM3240G-EVAL evaluation
  //  board User manual (UM1461).

#define MII_MODE

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef MII_MODE
#define PHY_CLOCK_MCO
#endif

  /* STM324xG-EVAL jumpers setting
      +==========================================================================================+
      +  Jumper |       MII mode configuration            |      RMII mode configuration         +
      +==========================================================================================+
      +  JP5    | 2-3 provide 25MHz clock by MCO(PA8)     |  Not fitted                          +
      +         | 1-2 provide 25MHz clock by ext. Crystal |                                      +
      + -----------------------------------------------------------------------------------------+
      +  JP6    |          2-3                            |  1-2                                 +
      + -----------------------------------------------------------------------------------------+
      +  JP8    |          Open                           |  Close                               +
      +==========================================================================================+
    */

  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void Time_Update(void);
  void Delay(uint32_t nCount);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

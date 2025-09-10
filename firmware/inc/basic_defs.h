/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: basic_defs.h
*
* Description:
* Basic data types definition, it contains a number of generic type declarations and definitions.
*
* Author: CHEN<chen@buildwin.com.cn>
*
* Create date: June. 15, 2016
*
*********************************************************************************
*
* Revision history:
---------------------------------------------------------------------------------
Date                    Modification                 Revision            Name
---------------------------------------------------------------------------------
June. 15, 2016      Created                       V0.1                 CHEN

********************************************************************************/
/**
 * @File basic_defs.h
 * @brief Basic data types definition, it contains a number of generic type declarations and definitions
 */

#ifndef __BASIC_DEFS_H__
#define __BASIC_DEFS_H__

#include "typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef INT32 OSAL_ER;
  typedef void *OSAL_HDL;

#define OSAL_E_OK 0
#define OSAL_E_FAIL (-1)
#define OSAL_E_TIMEOUT (-2)

#define OSAL_INVALID_ID 0xFFFFFFFF
#define OSAL_WAIT_FOREVER_TIME 0xFFFFFFFF

  typedef void *PVOID;

#define NUM_N1 -1
#define NUM_0 0
#define NUM_1 1
#define NUM_2 2
#define NUM_3 3
  //
  // #define INT8_MIN (-128)
  // #define INT8_MAX ( 127)
  // #define UINT8_MAX ( 255)
  //
  // #define INT16_MIN (-32768)
  // #define INT16_MAX ( 32767)
  // #define UINT16_MAX ( 65535)
  //
  // #define INT32_MIN (-2147483648L)
  // #define INT32_MAX ( 2147483647L)
  // #define UINT32_MAX ( 4294967295UL)

  // RET for return code
  typedef INT32 RET_CODE;
#define RET_SUCCESS ((INT32)0)
#define RET_FAILURE ((INT32)(-1))
#define RET_BUSY ((INT32)(-2))
#define RET_STA_ERR ((INT32)(-3))
#define RET_STA_PLUGIN_NOT_EXIST ((INT32)(-4))

#define ERR_DEV_ERROR -5  /* Device work status error */
#define ERR_DEV_CLASH -6  /* Device clash for same device in queue */
#define ERR_QUEUE_FULL -7 /* Queue node count reached the max. val*/
#define ERR_NO_DEV -8     /* Device not exist on PCI */
#define ERR_FAILURE -9    /* Common error, operation not success */

#define ERR_PARA -20    /* Parameter is invalid */
#define ERR_ID_FULL -21 /* No more ID available */
#define ERR_ID_FREE -22 /* Specified ID isn't allocated yet */

  /*!
  @enum RET_ERR_STATUS
  @brief   Define the error status for return value
  */
  typedef enum
  {
    RET_NO_ERR = 0,              //!< No error
    RET_ERR_INVALID_PARA,        //!< Parameter is invalid
    RET_ERR_NOMEM,               //!< Memory alloc failed
    RET_ERR_UNKNOWN_DEV,         //!< Device name unknown
    RET_ERR_INITIALIZED,         //!< Device has been initialized
    RET_ERR_NO_FREE_HANDLES,     //!< Open handles reaches limitation, cannot open again
    RET_ERR_OPEN_HANDLE,         //!< At least one open handle
    RET_ERR_INVALID_HDL,         //!< Handle is not valid
    RET_ERR_FEATURE_UNAVAILABLE, //!< Feature not supported now
    RET_ERR_INT_INSTALL,         //!< Install interrupt failed
    RET_ERR_INT_UNINSTALL,       //!< Uninstall interrupt failed
    RET_ERR_TIMEOUT,             //!< Timeout
    RET_ERR_DEV_BUSY,            //!< Device is busy
    RET_ERR_SUSPENDED,           //!< Device is in standby state
    RET_ERR_INVALID_DATA,        //!< Parser error, should return positive value
    RET_ERR_OS_FAILED,           //!< Execute OS interface failed
    RET_ERR_CANT_EXECUTE,        //!< Can not execute the operation
    RET_ERR_NOT_AVAILABLE,       //!< Not available
    RET_ERR_EVENT_REGISTRATION,  //!< Event registration error

    RET_ERR_DEV_ERROR,  //!< Device work status error
    RET_ERR_DEV_CLASH,  //!< Device clash for same device in queue
    RET_ERR_QUEUE_FULL, //!< Queue node count reached the max. val
    RET_ERR_NO_DEV,     //!< Device not exist on PCI
    RET_ERR_MAX,
  } RET_ERR_STATUS;

#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

/* Device name definition */
#define OSAL_MAX_DEV_NAME 16 /* 15 characters with ending '\0' */
  typedef CHAR OSAL_DEV_NAME[OSAL_MAX_DEV_NAME];

  // union which converts two 8bit values into 16bit value
  typedef union
  {
    UINT16 uint_16;
    struct
    {
      UINT8 uint80; // LSB
      UINT8 uint81; // MSB
    } uint_8;
  } UINT16TO8;

  // union which converts two 16bit values into 32bit value.
  typedef union
  {
    UINT32 uint_32;
    struct
    {
      UINT16 low_16;
      UINT16 hi_16;
    } uint_16;
  } UINT32TO16;

// Support macros
#define itemsof(a) (sizeof(a) / sizeof(a[0]))

  typedef int SOCK_HDL;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif

#ifdef __cplusplus
}
#endif

#endif //__BASIC_DEFS_H__

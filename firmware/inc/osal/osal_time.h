/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2017 Copyright (C)
*
* File name: osal_time.h
*
* Description:
* Timer relative functions definition for Operating System Abstract Layer
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
 * @File osal_time.h
 * @brief Time relative functions definition for Operating System Abstract Layer
 */

#ifndef __OSAL_TIME_H__
#define __OSAL_TIME_H__

#include "typedef.h"

#include "asm/osal_time.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Time interface */
#define gettimeofday(x, y) do_gettimeofday(x)

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TIME_H__ */

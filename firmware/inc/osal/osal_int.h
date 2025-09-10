/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: osal_int.h
*
* Description:
* Interrupt relative functions definition for Operating System Abstract Layer
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
 * @File osal_int.h
 * @brief Interrupt  relative functions definition for Operating System Abstract Layer
 */

#ifndef __OSAL_INT_H__
#define __OSAL_INT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "os/freeRTOS/inc/FreeRTOS.h"
#include "os/freeRTOS/inc/semphr.h"
#include "os/freeRTOS/inc/task.h"
#include "os/freeRTOS/inc/timers.h"

    typedef void (*OSAL_ISR_PROC)(UINT);

/* Register the interrupt service routine */
#define osal_interrupt_reg_isr BSP_IntVecReg

/* Unregister the interrupt service routine */
#define osal_interrupt_unreg_isr(...) \
    do                                \
    {                                 \
    } while (0)

/* Enable the interrupt service routine */
#define osal_interrupt_enable vPortEnableInterrupts

/* Disable the interrupt service routine */
#define osal_interrupt_disable vPortDisableInterrupts

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_INT_H__ */

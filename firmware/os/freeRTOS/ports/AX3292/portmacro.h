/*
    FreeRTOS V6.1.1 - Copyright (C) 2011 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS books - available as PDF or paperback  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/
#ifndef PORTMACRO_H
#define PORTMACRO_H

// #include <api4os.h>
#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stddef.h>
#endif /* __ASSEMBLER__ */
// #include "spr_defs.h"
// #include "AX3278.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------
// Port specific definitions
//-----------------------------------------------------------
// Type definitions.
#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE unsigned portLONG
#define portBASE_TYPE long
    // #define portTickType    unsigned portLONG
    // #define portMAX_DELAY   (portTickType)0xffffffff

    typedef portSTACK_TYPE StackType_t;
    typedef long BaseType_t;
    typedef unsigned long UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
    typedef unsigned portSHORT TickType_t;
#define portMAX_DELAY (TickType_t)0xffff
#else
typedef unsigned portLONG TickType_t;
#define portMAX_DELAY (TickType_t)0xffffffff
#endif
// #if( configUSE_16_BIT_TICKS == 1 )
//         #effor "configUSE_16_BIT_TICKS must be 0"
// #endif

/*-----------------------------------------------------------*/
#define portSTACK_GROWTH -1
#define portTICK_PERIOD_MS ((TickType_t)1000 / configTICK_RATE_HZ)
// #define portTICK_RATE_MS                        ( (portTickType) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT 32
#define portCRITICAL_NESTING_IN_TCB 0
#define portINSTRUCTION_SIZE ((portSTACK_TYPE)4)
#define portNO_CRITICAL_SECTION_NESTING ((portSTACK_TYPE)0)

#define portYIELD_FROM_ISR() vTaskSwitchContext()
#define portYIELD() __asm__ __volatile__("l.sys 0x0")
#define portNOP() __asm__ __volatile__("l.nop")

/*
 * naked attribute is ignored or32-elf-gcc 4.5.1-or32-1.0rc1
 * use assemble routines in portasm.S
 */
// #define portDISABLE_INTERRUPTS()        ({unsigned int nf,fl=mfspr(SPR_SR); nf=fl & ~SPR_SR_LIEE; mtspr(SPR_SR, nf);})
// #define portENABLE_INTERRUPTS()         ({unsigned int nf,fl=mfspr(SPR_SR); nf=fl |  SPR_SR_LIEE; mtspr(SPR_SR, nf);})
#define portDISABLE_INTERRUPTS() arch_local_irq_disable()
#define portENABLE_INTERRUPTS() arch_local_irq_enable()

/*-----------------------------------------------------------*/
unsigned int __critical_flags;
// Critical section handling.
#define portINIT_CRITICAL() unsigned int __critical_flags
// switch supervisormode, disable tick interrupt and all external interrupt, switch back usermode
// #define portENTER_CRITICAL() __critical_flags = ({unsigned int nf,fl=mfspr(SPR_SR); nf=fl & ~SPR_SR_LIEE; mtspr(SPR_SR, nf); fl;})
#define portENTER_CRITICAL() __critical_flags = arch_local_irq_save()

// switch supervisormode, enable tick interrupt and all external interrupt, switch back usermode
// #define portEXIT_CRITICAL() mtspr(SPR_SR,__critical_flags)
#define portEXIT_CRITICAL() arch_local_irq_restore(__critical_flags)

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters) void vFunction(void *pvParameters)

#ifdef __cplusplus
}
#endif

// void vPortRestoreContext (void);
#define xPortStartScheduler vPortRestoreContext

// 有很多類似的宏可以定義，可以很方便在各個環節添加調試打印
// 這樣可以不改變freeRTOS的源碼
#define traceTASK_SWITCHED_IN() vApplicationTaskSwitchHook()
// #define traceMALLOC( pvAddress, uiSize ) printf("malloc %X size %X\n", pvAddress, uiSize)
#define traceTASK_CREATE(tcb) vApplicationTaskCreateHook(tcb)
// #define traceTASK_DELAY() printf("task delay\n");
// #define traceQUEUE_CREATE(queue) printf("traceQUEUE_CREATE %X\n", queue);
// #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() printf ("portCONFIGURE_TIMER_FOR_RUN_TIME_STATS\r\n");
// #define traceTASK_INCREMENT_TICK( xTickCount ) printf ("T")

#endif /* PORTMACRO_H */

/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2017 Copyright (C)
*
* File name: osal_alloc.h
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
 * @File osal_alloc.h
 * @brief Time relative functions definition for Operating System Abstract Layer
 */

#ifndef __ASM_ALLOC_H__
#define __ASM_ALLOC_H__

#ifdef __cplusplus
extern "C"
{
#endif

// #define osal_malloc(...) pvPortMalloc2(__VA_ARGS__,__LINE__,__FUNCTION__)
#define osal_malloc(...) pvPortMalloc(__VA_ARGS__)

#define osal_free(...) vPortFree(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __ASM_TIMER_H__ */

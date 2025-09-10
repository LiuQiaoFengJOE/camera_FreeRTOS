/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2017 Copyright (C)
*
* File name: osal_rand.h
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
 * @File osal_rand.h
 * @brief Time relative functions definition for Operating System Abstract Layer
 */

#ifndef __ASM_RAND_H__
#define __ASM_RAND_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define osal_random() xTaskGetTickCount()

#ifdef __cplusplus
}
#endif

#endif /* __ASM_RAND_H__ */

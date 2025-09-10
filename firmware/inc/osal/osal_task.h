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
Aug.   3, 2017      Modified                      V0.2                 HE

********************************************************************************/
/**
 * @File osal_task.h
 * @brief Task relative functions definition for Operating System Abstract Layer
 */

#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#include "basic_defs.h"

typedef UINT32 TASK_HDL;

/* Task Callback function */
typedef void (*OSAL_TSK_PROC)(void *p_param);

/*!
@struct OSAL_TASK
@brief Defines the task control data to create the task, which use to create the task
*/
typedef struct t_ctsk
{
  OSAL_TSK_PROC tsk_entry_func; //!< Task entry function
  UINT32 param;                 //!< Task entry function input parameter
  UINT32 tsk_prio;              //!< Task priority
  UINT32 stk_size;              //!< Task stack size
  OSAL_DEV_NAME name;           //!< Task name
} OSAL_TASK;

#include "asm/osal_task.h"
#include "debug.h"

static inline int kthread_should_stop(void)
{
  return 0;
}

// todo: stksize, prio, namefmt
static inline struct task_struct *kthread_run(int (*threadfn)(void *data), void *data, const char *namefmt, ...)
{
  return (struct task_struct *)osal_thread_create(threadfn, data, 10, 0, 4096, namefmt);
}

static inline void kthread_bind(struct task_struct *k, unsigned int cpu)
{
  p_err_miss;
}

#ifdef __cplusplus
extern "C"
{
#endif

  /* Task interface */

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_TASK_H__ */

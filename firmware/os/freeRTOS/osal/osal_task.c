/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: osal_task.c
*
* Description:
* Task relative functions implementation for OpenRISC CPU Architecture
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
 * @File osal_task.c
 * @brief Task relative functions implementation for OpenRISC CPU Architecture
 */

/*-----------------------------------------------------------------------*/
/*                        INCLUDE HEADER FILES                                                             */
/*-----------------------------------------------------------------------*/

#include "osal/osal_task.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "debug.h"

/************************************************************************************
 * @name osal_task_create
 * ----------------------------------------------------------------------------------
 * @brief Create the task thread
 *
 * Arguments:
 * @param[in] p_tsk_ctrl -  Pointer to the task control data structure
 * ----------------------------------------------------------------------------------
 * @return TASK_HDL
 * @retval NULL - Fail to create the task
 * @retval Not NULL - Succeed to create the task, return the task handle
 ***********************************************************************************/
TASK_HDL osal_task_create(OSAL_TASK *p_tsk_ctrl)
{
  BaseType_t x_ret = pdPASS;
  TaskHandle_t x_created_task = NULL;

  /* Check if the input pointer is valid */
  if (NULL == p_tsk_ctrl)
  {
    p_dbg("osal_task_create err \n");
    return (TASK_HDL)NULL;
  }

  /* Create the task thread based on	xTaskCreate interface of the FreeRTOS */
  x_ret = xTaskCreate(p_tsk_ctrl->tsk_entry_func, (char *)p_tsk_ctrl->name, p_tsk_ctrl->stk_size, (void *)p_tsk_ctrl->param,
                      p_tsk_ctrl->tsk_prio, &x_created_task);
  if (x_ret != pdPASS)
  {
    /* Create the task fail, not enough memory to  create the task  */
    return (TASK_HDL)NULL;
  }

  return (TASK_HDL)x_created_task;
}

/************************************************************************************
 * @name osal_task_exit
 * ----------------------------------------------------------------------------------
 * @brief Create the task thread
 *
 * Arguments:
 * @param[in] p_task_entry - Pointer to the task entry function
 * @param[in] p_param - Pointer to the task input parameter
 * @param[in] prio - The priority of the task
 * @param[out] p_task_hdl - Pointer to the task handle
 * @param[in] stk_size - The stack size of the task
 * @param[in] p_name - Pointer to the task name
 * ----------------------------------------------------------------------------------
 * @return TASK_HDL
 * @retval NULL - Fail to create the task
 * @retval Not NULL - Succeed to create the task, return the task handle
 ***********************************************************************************/
void osal_task_exit(void)
{
  TaskHandle_t x_cur_task = NULL;

  /* Return the handle of the calling task. */
  x_cur_task = xTaskGetCurrentTaskHandle();

  if (x_cur_task != NULL)
  {
    /* Delete the current task */
    vTaskDelete(x_cur_task);
  }
  else
  {
    p_dbg("osal_task_exit err\n");
  }
}

#if 1
/*
 * Delay a task for a given number of ticks.  The actual time that the
 * task remains blocked depends on the tick rate.  The constant
 * portTICK_PERIOD_MS can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 */
void osal_task_sleep(UINT32 ms)
{
  UINT32 ticks = (ms * configTICK_RATE_HZ) / 1000;
  vTaskDelay(ticks);
}
#endif

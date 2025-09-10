#ifndef __ASM_TASK_H__
#define __ASM_TASK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "FreeRTOS.h"
#include "task.h"

  /* Task interface */

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
  TASK_HDL osal_task_create(OSAL_TASK *p_tsk_ctrl);

#define osal_thread_myself() (TASK_HDL) xTaskGetCurrentTaskHandle()
#define osal_thread_delete(thread) \
  ({                               \
    vTaskDelete((void *)thread);   \
    0;                             \
  })
#define osal_thread_create(task, p_arg, prio, pbos, stk_size, name)                                                \
  ({                                                                                                               \
    void *xCreateTask;                                                                                             \
    (TASK_HDL)(xTaskCreate((TaskFunction_t)task, name, stk_size, p_arg, prio, &xCreateTask) ? xCreateTask : NULL); \
  })
#define osal_thread_create_withID(task, p_arg, prio, pbos, stk_size, name,id)                                \
		  ({																							   \
			void* xCreateTask;																			   \
			(TASK_HDL)(xTaskCreateWithID((TaskFunction_t)task, name, stk_size, p_arg, prio, &xCreateTask,id) ? xCreateTask : NULL); \
		  })

#ifdef __cplusplus
}
#endif

#endif /* __ASM_TASK_H__ */

/**@File osal_sema.h
 * @brief 用FreeRTOS的動態semaphore來實現RTOS的semaphore
 * @details
 * Linux的semaphore一般是靜態分配空間的：
 *  - 使用前調用sema_init函數對這個已分配的空間進行初始化。
 *  - 使用後無需釋放
 *
 * 另外有一套sema機制，是RTOS慣用的方式，其sema本身是一個指針：
 *  - 使用前分配空間同時初始化
 *  - 使用後要釋放空間
 *
 * 這裡實現的RTOS機制的sema。Linux的semaphore機制在osal_semaphore.h定義
 */

#ifndef __ASM_SEMA_H__
#define __ASM_SEMA_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C"
{
#endif

  static inline void *osal_sema_create(uint32_t val)
  {
    uint32_t max = val;
    if (!max)
      max = 1;
    return xSemaphoreCreateCounting(max, val);
  }

  static inline int osal_sema_capture(void *sema_hdl, uint32_t time_out)
  {
    return xSemaphoreTake(sema_hdl, time_out);
  }

  static inline int osal_sema_release(void *sema_hdl)
  {
    extern unsigned int os_int_nesting;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (os_int_nesting)
    {
      xSemaphoreGiveFromISR(sema_hdl, &xHigherPriorityTaskWoken);
      if (xHigherPriorityTaskWoken)
        vTaskSwitchContext();
    }
    else
    {
      xSemaphoreGive(sema_hdl);
      if (configUSE_PREEMPTION == 0)
        ; // portYIELD ();
    }
  }

  static inline void osal_sema_delete(void *sema_hdl)
  {
    vSemaphoreDelete(sema_hdl);
  }

#ifdef __cplusplus
}
#endif

#endif /* __ASM_SEMAPHORE_H__ */

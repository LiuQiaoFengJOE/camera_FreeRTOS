/**@File osal_semaphore.h
 * @brief 用FreeRTOS的靜態semaphore來實現Linux的semaphore
 * @details
 * Linux的semaphore一般是靜態分配空間的：
 *  - 使用前調用sema_init函數對這個已分配的空間進行初始化。
 *  - 使用後無需釋放
 *
 * 另外有一套sema機制，是RTOS慣用的方式，其sema本身是一個指針：
 *  - 使用前分配空間同時初始化
 *  - 使用後要釋放空間
 *
 * 這裡實現的Linux機制的semaphore。RTOS的sema機制在osal_sema.h中定義。
 */

#ifndef __ASM_SEMAPHORE_H__
#define __ASM_SEMAPHORE_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @class semaphore
   * @author Kaifan
   * @date 16/08/2017
   * @file osal_semaphore.h
   * @brief Linux semaphore container for FreeRTOS static semaphore
   * linux的semaphore祇有init，不會有fini。因此採用靜態分配是唯一選擇。
   * 採用分配的方式，會導致內存洩漏。
   *
   * @note FreeRTOS新版本雖然提供了靜態semaphore，但其max參數仍是大麻煩
   */
  struct semaphore
  {
    StaticSemaphore_t sema;
    int valid;
  };

  static inline void sema_init(struct semaphore *sem, int val)
  {
    int max = val ? val : 1;
    if (NULL == sem)
      return;

    //if (sem->valid)
     // printf("### WARNING: sema_init already!!\r\n");

    xSemaphoreCreateCountingStatic(max, val, &sem->sema); // todo: max == val ?
    sem->valid = 1;
  }

  static inline unsigned char down_interruptible(struct semaphore *sem)
  {
    if (!sem || !sem->valid)
      return -1;
    xSemaphoreTake(&sem->sema, -1); // todo: interrupt
    return 0;
  }

  // 带超时
  static inline unsigned char down2_interruptible(struct semaphore *sem, unsigned long timeout)
  {
    if (!sem || !sem->valid)
      return -1;
    unsigned char res;
    res = xSemaphoreTake(&sem->sema, timeout); // todo: interrupt
    return res;
  }

  static inline void up(struct semaphore *sem)
  {
    extern unsigned int os_int_nesting;
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (!sem || !sem->valid)
      return;
    if (os_int_nesting)
    {
      xSemaphoreGiveFromISR(&sem->sema, &xHigherPriorityTaskWoken);
      if (xHigherPriorityTaskWoken)
        vTaskSwitchContext();
    }
    else
    {
      xSemaphoreGive(&sem->sema);
      if (configUSE_PREEMPTION == 0)
        ; // portYIELD ();
    }
  }

#ifdef __cplusplus
}
#endif

#endif /* __ASM_SEMAPHORE_H__ */

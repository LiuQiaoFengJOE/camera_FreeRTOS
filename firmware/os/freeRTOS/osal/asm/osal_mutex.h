/**
 * @File osal_mutex.h
 * @brief 用FreeRTOS的semaphore來實現Linux的mutex
 */

#ifndef __ASM_MUTEX_H__
#define __ASM_MUTEX_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "osal/osal_alloc.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @class mutex
   * @author Kaifan
   * @date 16/08/2017
   * @file osal_mutex.h
   * @brief Linux mutex container for FreeRTOS static mutex
   * linux的mutex祇有init，不會有fini。因此採用靜態分配是唯一選擇。
   * 採用分配的方式，會導致內存洩漏。
   */
  struct mutex
  {
    StaticSemaphore_t mutex;
  };

  static inline void mutex_init(struct mutex *lock)
  {
    if (NULL == lock)
      return;

    xSemaphoreCreateMutexStatic(&lock->mutex);
  }

  static inline void mutex_lock(struct mutex *lock)
  {
    xSemaphoreTake(&lock->mutex, -1); // todo: interrupt
  }

  static inline void mutex_unlock(struct mutex *lock)
  {
    xSemaphoreGive(&lock->mutex);
    if (configUSE_PREEMPTION == 0)
      ; // portYIELD ();
  }

  static inline int mutex_lock_interruptible(struct mutex *lock)
  {
    return xSemaphoreTake(&lock->mutex, -1); // todo: interrupt
  }

  static inline void mutex_destroy(struct mutex *lock)
  {
  }

  ////////////////////////////////////////////////////////////////////////////////
  // osal_mutex
  static inline void *osal_mutex_create(void)
  {
    struct mutex *lock;
    lock = (struct mutex *)osal_malloc(sizeof(struct mutex));
    if (NULL == lock)
      return OSAL_INVALID_ID;

    mutex_init(lock);
    return lock;
  }

  static inline RET_CODE osal_mutex_lock(struct mutex *lock, UINT32 time_out)
  {
    mutex_lock(lock);
    return RET_SUCCESS;
  }

  static inline RET_CODE osal_mutex_unlock(struct mutex *lock)
  {
    mutex_unlock(lock);
    return RET_SUCCESS;
  }

  static inline void osal_mutex_delete(struct mutex *lock)
  {
    osal_free(lock);
  }

#ifdef __cplusplus
}
#endif

#endif /* __ASM_MUTEX_H__ */

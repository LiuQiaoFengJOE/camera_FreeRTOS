/**@file osal_wait.h
 * @brief Linux-like wait_queue functions definition for Operating System Abstract Layer
 * @details 在Linux中，wait_queue的概念是相當基礎的，而且相當靈活；
 * 在freeRTOS中與wait_queue類似的結構是Queue。
 *
 * 不要被freeRTOS的eventgroup的概念所迷惑，wait_event中的condition機制比EventBits靈活得多。
 *
 * 因此，在此使用freeRTOS的Queue來直接實現wait_queue_head_t。
 * freeRTOS的xQueueGenericReceive在timeout时，返回的也是0
 */

#ifndef __ASM_WAIT_H__
#define __ASM_WAIT_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef StaticQueue_t wait_queue_head_t;

  typedef struct
  {
  } wait_queue_t;

  static inline void init_waitqueue_head(wait_queue_head_t *queue)
  {
    xSemaphoreCreateBinaryStatic(queue);
  }
  
#define wait_event(wq) \
  ({\
	  xQueueGenericReceive( wq, NULL, -1, 0 );\
  })
#define wait_event_interruptible(wq, condition)    \
  ({                                               \
    int ret = 1;                                   \
    while (!(condition))                           \
      ret = xQueueGenericReceive(wq, NULL, -1, 0); \
    ret;                                           \
  })

#define wait_event_interruptible_exclusive(wq, condition) \
  ({                                                      \
    int ret = 1;                                          \
    while (!(condition))                                  \
      ret = xQueueGenericReceive(wq, NULL, -1, 0);        \
    ret;                                                  \
  })

#define wait_event_interruptible_timeout(wq, condition, timeout) \
  ({                                                             \
    int ret = 1;                                                 \
    while (!(condition) && ret)                                  \
      ret = xQueueGenericReceive(wq, NULL, timeout, 0);          \
    ret;                                                         \
  })

/* 当condition为真的时候，会返回
 * 当timeout到达时也会返回，不管此时condition为真为假都会返回
 * 返回0：表示timeout超时
 */
#define wait_event_timeout(wq, condition, timeout)      \
  ({                                                    \
    int ret = 1;                                        \
    while (!(condition) && ret)                         \
      ret = xQueueGenericReceive(wq, NULL, timeout, 0); \
    ret;                                                \
  })

#define wake_up(wq)                                                  \
  do                                                                 \
  {                                                                  \
    extern unsigned int os_int_nesting;                                  \
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;         \
    if (os_int_nesting)                                              \
    {                                                                \
      xQueueGenericSendFromISR(wq, 0, &xHigherPriorityTaskWoken, 0); \
      if (xHigherPriorityTaskWoken)                                  \
        vTaskSwitchContext();                                        \
    }                                                                \
    else                                                             \
      xQueueGenericSend(wq, 0, 0, 0);                                \
  } while (0)
#define wake_up_interruptible(x) wake_up(x)

#define wake_up_signal(wq, msg, timeout)                     \
  do                                                         \
  {                                                          \
    extern unsigned int os_int_nesting;                          \
    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE; \
    if (os_int_nesting)                                      \
    {                                                        \
      xQueueSendFromISR(wq, msg, &xHigherPriorityTaskWoken); \
      if (xHigherPriorityTaskWoken)                          \
        vTaskSwitchContext();                                \
    }                                                        \
    else                                                     \
      xQueueSend(wq, msg, timeout);                          \
  } while (0)
#define wake_up_signal_interruptible(x, msg, timeout) wake_up_signal(x, msg, timeout)

#ifdef __cplusplus
}
#endif

#endif /* __ASM_WAIT_H__ */

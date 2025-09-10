/**
 * @File osal_delay.h
 * @brief delay and sleep
 */

#ifndef __ASM_DELAY_H__
#define __ASM_DELAY_H__

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C"
{
#endif

  static inline void msleep(unsigned int msecs)
  {
    vTaskDelay(msecs / (portTICK_PERIOD_MS));
  }

  static inline void mdelay(unsigned int msecs)
  {
    vTaskDelay(msecs / (portTICK_PERIOD_MS));
  }

  static inline void udelay(unsigned int usecs)
  {
    vTaskDelay(usecs / (1000 * portTICK_PERIOD_MS));
  }

#ifdef __cplusplus
}
#endif

#endif /* __ASM_DELAY_H__ */

#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#include "os_cpu.h"
#include "api.h"
#include "lwip/err.h"

#define SYS_MBOX_NULL ((QueueHandle_t)NULL)
#define SYS_SEM_NULL ((SemaphoreHandle_t)NULL)
#define SYS_DEFAULT_THREAD_STACK_DEPTH configMINIMAL_STACK_SIZE

#ifndef SYS_MSG_T
#define SYS_MSG_T

typedef SemaphoreHandle_t sys_sem_t;
typedef SemaphoreHandle_t sys_mutex_t;
typedef QueueHandle_t sys_mbox_t;
typedef TaskHandle_t sys_thread_t;

#endif

#ifndef NULL
#define NULL 0
#endif

err_t sys_sem_new(sys_sem_t *sem, u8_t count);
void sys_sem_free(sys_sem_t *sem);
void sys_sem_signal(sys_sem_t *sem);

#endif /* __SYS_ARCH_H__ */

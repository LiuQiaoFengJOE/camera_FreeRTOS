#ifndef _API_H_
#define _API_H_

#include <stdlib.h>
#include <stdint.h>

#include "os/freeRTOS/inc/FreeRTOS.h"
#include "os/freeRTOS/inc/semphr.h"
#include "os/freeRTOS/inc/task.h"
#include "os/freeRTOS/inc/timers.h"

typedef SemaphoreHandle_t OS_EVENT;
typedef OS_EVENT wait_event_t;

#include "app_cfg.h"
#include "debug.h"
#include "atomic.h"
#include "mutex.h"
#include "wait.h"
#include "task.h"
#include "timer.h"
#include "memory.h"

#ifndef OS_FALSE
#define OS_FALSE 0u
#endif

#ifndef OS_TRUE
#define OS_TRUE 1u
#endif

#undef jiffies
#define jiffies xTaskGetTickCount()
#define enter_interrupt()
#define exit_interrupt(x)
#define OSStart() vTaskStartScheduler()
#define OSStatInit()
#define OSIntEnter()
#define OSIntExit()
#define OSTimeTick() vPreemptiveTick()

#if configROMHOOK
#define OSInit() rom_hook_init()
#define OSClearBss() vOSClearRomBSS()
#define OSInitHook() rom_malloc_hook_init()
#else
#define OSInit()
#define OSClearBss()
#define OSInitHook()
#endif

#endif

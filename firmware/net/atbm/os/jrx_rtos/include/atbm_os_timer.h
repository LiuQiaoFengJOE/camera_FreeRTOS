#ifndef ATBM_OS_TIMER_H
#define ATBM_OS_TIMER_H

#define TIMER_1ST_PARAM
//typedef TimerHandle_t T_hTimer;


typedef enum  {
	osTimerOnce	= 0,       ///< one-shot timer
	osTimerPeriodic	= 1        ///< repeating timer
} os_timer_type;


typedef atbm_void (*TIMER_CALLBACK)(atbm_void * CallRef);
//typedef void (*timer_handler_t)(void *arg);

#if 0
struct OS_TIMER_S
{
    atbm_uint32          TimerId;
    TIMER_CALLBACK  pCallback;
    atbm_void *          pCallRef;
	atbm_uint32			TimerHander;
	int 			bTimerStart;
};
#endif

struct OS_TIMER_S {
	TimerHandle_t handle;
	atbm_uint32 flag; //tick with timeout
	os_timer_type type;
	TIMER_CALLBACK func;
	unsigned long time_ms;
	void *arg;
};



typedef struct OS_TIMER_S OS_TIMER;

atbm_uint32   atbm_InitTimer(OS_TIMER *pTimer, TIMER_CALLBACK pCallback, atbm_void * CallRef);
atbm_uint32  atbm_StartTimer(OS_TIMER *pTimer, atbm_uint32 Interval/*ms*/);
atbm_uint32 atbm_CancelTimer(OS_TIMER *pTimer);
atbm_uint32 atbm_FreeTimer(OS_TIMER *pTimer);
atbm_void atbm_SleepMs(atbm_uint32 uiMiliSecond);

unsigned int atbm_GetOsTimeMs();
unsigned int atbm_GetOsTime();

//ATBM_BOOL atbm_TimeAfter(atbm_uint32 tickMs);
ATBM_BOOL atbm_TimeAfter(atbm_uint32 tickMs, atbm_uint32 intvMs);
//atbm_uint64 atbm_GetOsTimeMs();
atbm_uint32 atbm_TimerTickGet(atbm_void);
atbm_uint32 atbm_TimerMsToTick(atbm_uint32 ms);
atbm_uint32 atbm_TimerTickToMs(atbm_uint32 tick);
atbm_uint8 atbm_TimerIsActive(OS_TIMER *pTimer);
atbm_uint32 atbm_TimerGetExpiry(OS_TIMER *pTimer);
//atbm_uint32 msecs_to_jiffies(atbm_uint32 a);
atbm_void atbm_SleepMs(atbm_uint32 ms);
#define atbm_mdelay atbm_SleepMs
atbm_void atbm_wifi_ticks_timer_init(atbm_void);
#endif /* ATBM_OS_TIMER_H */

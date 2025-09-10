#include "api.h"
#include "debug.h"
#include "timer.h"
// #include "timers.h"
// #include "sys_misc.h"

#define OS_TMR_CFG_TICKS_PER_SEC 1000

static void WorkQueue_CommonCallBackFunction(OS_TMR *tmr)
{
	struct work_struct_legacy *work;

	work = pvTimerGetTimerID(tmr);
	timer_tasklet_callback(work);
}

OS_TMR *timer_setup(int time_val, int type, timer_callback_func callback, void *callback_arg)
{
	OS_TMR *tmr;
	void *pTimerID;

	if (time_val < 100)
		time_val = 100;

	time_val = time_val * OS_TMR_CFG_TICKS_PER_SEC / 1000;

	if ((callback == 0))
	{
		p_err("setup_timer err arg\n");
		return 0;
	}

	pTimerID = mem_malloc(sizeof(struct work_struct_legacy));
	if (!pTimerID)
	{
		p_err("setup timer malloc err\n");
		return 0;
	}

	init_work(pTimerID, callback, callback_arg);

	if (type)
	{
		p_dbg("repeat:%d\n", time_val);
		tmr = xTimerCreate("Timer", time_val, pdTRUE, pTimerID, WorkQueue_CommonCallBackFunction);
	}
	else
	{
		p_dbg("one short:%d\n", time_val);
		tmr = xTimerCreate("Timer", time_val, pdFALSE, pTimerID, WorkQueue_CommonCallBackFunction);
	}

	if (tmr == NULL)
	{
		p_err("setup_timer err\n");
	}

	return (OS_TMR *)tmr;
}

int timer_pending_api(OS_TMR *tmr)
{
	OS_TMR *tmr_t = (OS_TMR *)tmr;

	if (tmr_t == 0)
	{
		p_err("timer_pending_api tmr err");
		return 0;
	}

	if (xTimerIsTimerActive(tmr_t))
	{
		return 1;
	}

	return 0;
}

// expires ----n*ms
int mod_timer_api(OS_TMR *tmr, unsigned int expires)
{
	INT8U ret;
	OS_TMR *tmr_t = tmr;

	if (tmr_t == 0)
	{
		p_err("mod_timer_api tmr err");
		return -1;
	}

	if (expires < 100)
		expires = 100;

	expires = expires * configTICK_RATE_HZ / 1000;

	if (timer_pending_api(tmr) /* && (expires == tmr_t->OSTmrPeriod)*/)
		return -1;

	xTimerChangePeriod(tmr_t, expires, expires);

	ret = xTimerStart(tmr_t, 0);
	if (ret == pdTRUE)
	{
		return 0;
	}

	return -1;
}

int add_timer_api(OS_TMR *tmr)
{
	INT8U ret;
	OS_TMR *tmr_t = tmr;

	if (tmr_t == 0)
	{
		p_err("add_timer_api tmr err");
		return -1;
	}
	ret = xTimerStart(tmr_t, 0);
	if (ret == pdTRUE)
	{
		return 0;
	}

	return -1;
}

// Í£Ö¹¶¨Ê±Æ÷
int del_timer_api(OS_TMR *tmr)
{
	OS_TMR *tmr_t = tmr;

	if (tmr_t == 0)
	{
		p_err("del_timer_api tmr err");
		return 0;
	}

	if (timer_pending_api(tmr))
	{
		xTimerStop(tmr_t, 0);
		return 1;
	}
	xTimerStop(tmr_t, 0);

	return 0;
}

// ÊÍ·ÅÉ¾³ý¶¨Ê±Æ÷
int timer_free(OS_TMR *tmr)
{
	INT8U ret;
	OS_TMR *tmr_t = tmr;

	if (tmr->pvTimerID)
		free(tmr->pvTimerID);

	ret = xTimerDelete(tmr_t, 0);

	if (ret == pdTRUE)
		return 0;

	p_err("timer_free err %d", ret);
	return -1;
}

void sleep(uint32_t ms)
{
	vTaskDelay((ms * configTICK_RATE_HZ) / 1000);
}

#if 0
int gettimeofday( struct timeval *tv, struct timezone *tz )
{
	unsigned long os_msec;

	os_msec = os_time_get();
	tv->tv_sec = os_msec/1000;
	tv->tv_usec = (os_msec % 1000) * 1000;

	return 0;
}
#endif
#ifdef WIFI_RTL8189FTV_SUPPORT
static void os_delay(unsigned long ulCount)
{
	while (ulCount--)
		;
}

void os_udelay(unsigned int us)
{
	unsigned long nloops = us * (configCPU_CLOCK_HZ / 3000000);

	os_delay(nloops);
}

void os_msleep(unsigned int ms)
{
	vTaskDelay((ms * configTICK_RATE_HZ) / 1000);
}
#endif

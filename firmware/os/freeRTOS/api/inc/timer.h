#ifndef _TIMER_H_
#define _TIMER_H_

// #include "api.h"
#include "timers.h"
// #include "usr_cfg.h"
#ifdef WIFI_RTL8189FTV_SUPPORT
#define os_usleep(us) os_udelay(us)
#define msleep(ms) os_msleep(ms)
#endif
typedef xTIMER OS_TMR;
typedef void (*timer_callback_func)(void *parg);

OS_TMR *timer_setup(int time_val, int type, timer_callback_func callback, void *callback_arg);
int timer_pending_api(OS_TMR *tmr);
int mod_timer_api(OS_TMR *tmr, unsigned int expires);
int add_timer_api(OS_TMR *tmr);
int del_timer_api(OS_TMR *tmr);
int timer_free(OS_TMR *tmr);
void sleep(uint32_t ms);
unsigned int os_time_get(void);
void gettimeofday(struct timeval *tv, struct timezone *tz);

#endif

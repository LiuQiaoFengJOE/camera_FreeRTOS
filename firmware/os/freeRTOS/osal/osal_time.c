#include "osal/osal_task.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "drv/rtc/rtc.h"

#include "debug.h"

unsigned int os_time_get(void)
{
  return (unsigned int)(xTaskGetTickCount() * (1000uL / configTICK_RATE_HZ));
}

unsigned int time(unsigned int *t)
{
  unsigned int tm = xTaskGetTickCount() / configTICK_RATE_HZ;

  if (t)
    *t = tm;
  return tm;
}

/**
 * @brief 獲取以FAT目錄項時間格式所表示的時間
 * 所返回的32位小端數據：
 * 31:25 -- Year origin from the 1980，即陽曆年號-1980
 * 24:21 -- Month
 * 20:16 -- Day of the month
 * 15:11 -- Hour
 * 10:05 -- Minute
 * 04:00 -- Second / 2
 * 在Windows中GetLocalTime所返回的SYSTEMTIME格式，非常接近fattime格式
 * @return fattime
 */

extern HAL_RTC_T get_rtc_time();
unsigned int get_fattime(void)
{
  HAL_RTC_T nowTime = get_rtc_time();

  // todo: get a real time
  return ((nowTime.year - 1980) << 25) | (nowTime.month << 21) | (nowTime.day << 16) | (nowTime.hour << 11) | (nowTime.min << 5) |
         (nowTime.sec >> 1);
}

struct timeval
{
  long tv_sec; /* seconds */ // 实际是ms
  long tv_usec;              /* and microseconds */
};
void do_gettimeofday(struct timeval *tv)
{
#if 1
  TickType_t tick = xTaskGetTickCount();
  tv->tv_sec = tick / portTICK_PERIOD_MS;
  tv->tv_usec = tick % portTICK_PERIOD_MS;
#else
  TickType_t tick = os_time_get();
  tv->tv_sec = tick / 1000;
  tv->tv_usec = (tick % 1000) * 1000;
#endif
}

#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC
struct timespec
{
  long tv_sec;  /* seconds */
  long tv_nsec; /* nanoseconds */
};
#endif

struct timespec ns_to_timespec(s64 nsec)
{
  struct timespec ts;
  ts.tv_sec = nsec / 1000000L;
  ts.tv_nsec = nsec % 1000000L;
  return ts;
}

s64 ktime_get_with_offset(int offs)
{
  return xTaskGetTickCount();
}

unsigned long __msecs_to_jiffies(const unsigned int m)
{
  return m * portTICK_PERIOD_MS;
}

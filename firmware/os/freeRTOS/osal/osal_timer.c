#include "osal/osal_timer.h"
#include "FreeRTOS.h"
#include "timers.h"

static void timer_callback_wrapper(void *xTimer)
{
  struct timer_list *timer = (struct timer_list *)pvTimerGetTimerID(xTimer);
  timer->function(timer->data);
}

void osal_init_timer(struct timer_list *timer)
{
  timer->timer = NULL;
}

int osal_del_timer(struct timer_list *timer)
{
  if (!timer && !timer->timer)
    return 0;

  xTimerDelete(timer->timer, 0);
  timer->timer = NULL;

  return 0;
}

int osal_mod_timer(struct timer_list *timer, unsigned long expires)
{
  if (!timer)
    return 0;

  if (timer->timer)
    xTimerDelete(timer->timer, 0);

  timer->timer = xTimerCreate("TMR", expires, 0, timer, timer_callback_wrapper); // one shot timer
  xTimerStart(timer->timer, 0);
  return timer->timer != NULL;
}

int osal_auto_timer(struct timer_list *timer, unsigned long expires)
{
  if (!timer)
    return 0;

  if (timer->timer)
    xTimerChangePeriod(timer->timer, expires, 0);
  else
  {
    timer->timer = xTimerCreate("TMR", expires, 1, timer, timer_callback_wrapper);
    xTimerStart(timer->timer, 0);
  }

  return timer->timer != NULL;
}
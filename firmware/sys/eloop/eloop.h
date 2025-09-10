/**@file    eloop.h
 * @author  何凱帆
 * @brief   基於socket的event loop服務
 * @details 提供基於socket的event loop服務
 */

#ifndef _ELOOP_H_
#define _ELOOP_H_

#include "osal/osal_time.h"
#include "osal/osal_list.h"

#define EVT_HDL void *

#define EVENT_F_ENABLED 1
#define EVENT_F_REMOVE 2
#define EVENT_F_ONESHOT 4
#define EVENT_F_RUNNING 8

typedef void (*event_callback)(EVT_HDL e, void *d);
struct time_event
{
  time_ref fire;
  int ival;
};

struct fd_event
{
  SOCK_HDL fd;
  int write; // 0 = read, 1 = write
};

struct event
{
  struct list_head list;
  event_callback func;
  void *data;
  int type;
  int flags;
  union
  {
    struct time_event time;
    struct fd_event fd;
  } ev;
};

void eloop_resched_event(EVT_HDL ev, time_ref *tr);
void eloop_remove_event(EVT_HDL ev);
void eloop_set_event_interval(EVT_HDL ev, int msec);
void eloop_set_event_enabled(EVT_HDL ev, int enabled);
int eloop_get_event_enabled(EVT_HDL ev);

EVT_HDL eloop_add_timer(int msec, unsigned int flags, event_callback f, void *d);
EVT_HDL eloop_add_alarm(int msec, unsigned int flags, event_callback f, void *d);
EVT_HDL eloop_add_fd(SOCK_HDL fd, int write, unsigned int flags, event_callback f, void *d);
EVT_HDL eloop_add_always(unsigned int flags, event_callback f, void *d);

void eloop_exit(void);
void eloop_init(void);
void eloop_run(int single);
void eloop_wakeup(void);

void time_now(time_ref *tr);

#endif /* _ELOOP_H_ */

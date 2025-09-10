#ifndef __OSAL_WORKQUEUE_H__
#define __OSAL_WORKQUEUE_H__

#include "osal/osal_list.h"
#include "osal/osal_timer.h"

typedef int (*workfunc_t)(void *);
typedef void (*workqueue_daemon)(void *);

struct work_struct
{
  struct list_head entry;
  workfunc_t fun;
  void *data;
};

struct workqueue_struct
{
  unsigned int flags; /* I: WQ_* flags */
  void *task_id;
  wait_queue_head_t thread_event;
  struct list_head list;
  const char *name;
};

struct delayed_work
{
  struct work_struct work;
  struct timer_list timer;
};

extern void delayed_work_timer_fn(unsigned long __data);
extern struct workqueue_struct *system_wq;

#endif /* __OSAL_WORKQUEUE_H__ */

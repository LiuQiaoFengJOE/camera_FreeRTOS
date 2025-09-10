#ifndef _WAIT_H_
#define _WAIT_H_
#include "api.h"

#define WAIT_EVENT_TIMEOUT 1

wait_event_t init_event(void);
int wait_event_legacy(wait_event_t wq);

int wait_event_condition_legacy(wait_event_t wq, int *condition);

int wait_event_timeout_legacy(wait_event_t wq, unsigned int timeout);
void wake_up_legacy(wait_event_t wq);

void wake_up_condition_legacy(wait_event_t wq, int *condition);

void del_event(wait_event_t wq);
void clear_wait_event(wait_event_t wq);

#define init_waitqueue_head_legacy(x) *(x) = init_event()
#define del_waitqueue_head_legacy(x) del_event(x)
#define wait_event_interruptible(x, y) wait_event(x)
#define wait_event_interruptible_timeout(x, y, z) wait_event_timeout(x, z)
#define wake_up_interruptible wake_up

#endif

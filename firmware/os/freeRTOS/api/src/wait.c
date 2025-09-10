#include "api.h"
#include "debug.h"
#include "wait.h"

wait_event_t init_event()
{
	OS_EVENT *event = xSemaphoreCreateCounting(-1, 0);
	if (!event)
		p_err("init_waitqueue_head err");

	return (wait_event_t)event;
}

int wait_event_legacy(wait_event_t wq)
{
	if (wq == 0)
	{
		return -1;
	}
	if (xSemaphoreTake(wq, portMAX_DELAY) == pdTRUE)
	{
		return 0;
	}
	p_err("add_wait_queue err\n");
	return -1;
}

int wait_event_condition_legacy(wait_event_t wq, int *condition)
{
	OS_CPU_SR cpu_sr;

	OS_ENTER_CRITICAL();

	*condition = 1;
	if (wait_event_legacy(wq) == 0)
	{
		OS_EXIT_CRITICAL();
		return 0;
	}

	OS_EXIT_CRITICAL();
	return -1;
}

int wait_event_timeout_legacy(wait_event_t wq, unsigned int timeout)
{
	INT8U perr;
	uint32_t ticks;

	if (wq == 0)
	{
		p_err("wait_event_timeout wq err");
		return -1;
	}
	ticks = (timeout * configTICK_RATE_HZ) / 500;
	if (xSemaphoreTake(wq, ticks) == pdTRUE)
		return 0;

	// printf("xSemaphore_time out:%d\n",__LINE__);
	return 1; // timeout
}

void wake_up_legacy(wait_event_t wq)
{
	if (xSemaphoreGive(wq) != pdTRUE)
		p_err("wake_up err");
}
void wake_up_FromISR(wait_event_t wq)
{
	// printf("xSemaphore_isr:%d\n",__LINE__);
	if (xSemaphoreGiveFromISR(wq, NULL) != pdTRUE)
		p_err("wake_up_isr err");
}

void wake_up_condition_legacy(wait_event_t wq, int *condition)
{
	OS_CPU_SR cpu_sr;

	OS_ENTER_CRITICAL();

	if (*condition)
	{
		*condition = 0;
		wake_up_legacy(wq);
	}

	OS_EXIT_CRITICAL();
}

void wake_up_condition_FromISR(wait_event_t wq, int *condition)
{
	if (*condition)
	{
		*condition = 0;
		wake_up_FromISR(wq);
	}
}

void del_event(wait_event_t wq)
{
	vSemaphoreDelete(wq);
}

void clear_wait_event(wait_event_t wq)
{
	INT8U perr;

	if (wq == 0)
	{
		p_err("clear_wait_event wq err");
		return;
	}
	if (xQueueReset(wq) != pdTRUE)
		p_err("clear_wait_event err");

	return;
}

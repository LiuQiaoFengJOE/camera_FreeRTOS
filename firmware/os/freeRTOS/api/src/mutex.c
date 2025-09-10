#include "api.h"
#include "debug.h"

mutex_t mutex_init_legacy_atbm(const char *name)
{
	return (mutex_t)xSemaphoreCreateMutex();
}

int mutex_lock_legacy(mutex_t mutex)
{
	INT8U perr;

	perr = xSemaphoreTake(mutex, portMAX_DELAY);
	if (perr == pdPASS)
		return 0;

	p_err("mutex_lock err %d", perr);
	return -1;
}

int mutex_unlock_legacy(mutex_t mutex)
{
	INT8U perr;

	perr = xSemaphoreGive(mutex);
	if (perr == pdTRUE)
		return 0;

	p_err("mutex_unlock err %d", perr);
	return -1;
}

void mutex_destory(mutex_t mutex)
{
	vSemaphoreDelete(mutex);
}

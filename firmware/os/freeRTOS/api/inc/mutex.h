#ifndef _MUTEX_H_
#define _MUTEX_H_
#include "api.h"

typedef SemaphoreHandle_t mutex_t;
typedef SemaphoreHandle_t pthread_mutex_t;

typedef SemaphoreHandle_t pthread_mutex_t;

#define pthread_mutex_lock(mtx) mutex_lock_legacy(*mtx)
#define pthread_mutex_unlock(mtx) mutex_unlock_legacy(*mtx)

#define pthread_mutex_init(mtx, flg)         \
	{                                        \
		*mtx = mutex_init_legacy_atbm(NULL); \
	}

mutex_t mutex_init_legacy_atbm(const char *name);
int mutex_lock_legacy(mutex_t mutex);
int mutex_unlock_legacy(mutex_t mutex);
void mutex_destory(mutex_t mutex);
#define mutex_free mutex_destory

#endif

#include "typedef.h"
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <autoconf.h>
#include <osdep_service.h>

//----- ------------------------------------------------------------------
// Misc Function
//----- ------------------------------------------------------------------
// Attention: reentrance
#if 1
static uint32_t flags_stack[16];
static uint32_t flags_count = 0;
void save_and_cli()
{
	flags_stack[flags_count++] = arch_local_irq_save();
}

void restore_flags()
{
	arch_local_irq_restore(flags_stack[--flags_count]);
}
#else
static uint32_t __critical_flags;
static int flags_count = 0;
void save_and_cli()
{
	if (!flags_count)
		__critical_flags = arch_local_irq_save();
	++flags_count;
}

void restore_flags()
{
	--flags_count;
	if (!flags_count)
		arch_local_irq_restore(__critical_flags);
}
#endif

void cli()
{
	printf("##### ERROR ##### RTL8189FTV may be dead!\r\n");
	arch_local_irq_save();
}

void abort(void)
{
	printf("##### ERROR ##### RTL8189FTV may abort!\r\n");
}

unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	return simple_strtoul(nptr, endptr, base);
}

char *os_strdup(const char *string_copy_from)
{
	char *string_copy_to = NULL;
	string_copy_to = os_zalloc(strlen(string_copy_from) + 1);
	memcpy((void *)string_copy_to, string_copy_from, strlen(string_copy_from));
	string_copy_to[strlen(string_copy_from)] = '\0';
	return string_copy_to;
}

int os_get_random(unsigned char *buf, size_t len)
{
	// TODO implement it
	rtw_get_random_bytes(buf, len);
	return 0;
}

#if 0
__attribute__((weak))
int os_get_time(struct os_time *t){
	unsigned int tt = rtw_get_current_time();
	t->sec = (os_time_t) (tt / 1000);
	t->usec = (os_time_t) (tt % 1000)*1000;
	return 0;
}

int os_get_reltime(struct os_reltime *t){
	os_get_time((struct os_time *)t);
	return 0;
}
#endif

void *os_xqueue_create(unsigned long uxQueueLength, unsigned long uxItemSize)
{
	// printf ("** %s\r\n", __func__);
	return xQueueCreate(uxQueueLength, uxItemSize);
}

int os_xqueue_receive(xQueueHandle xQueue, void *const pvBuffer, unsigned long xSecsToWait)
{
	// printf ("** %s\r\n", __func__);
	return xQueueReceive((xQueueHandle)xQueue, pvBuffer, (portTickType)(xSecsToWait * configTICK_RATE_HZ));
}

void os_xqueue_delete(xQueueHandle xQueue)
{
	// printf ("** %s\r\n", __func__);
	vQueueDelete((xQueueHandle)xQueue);
}

int os_xqueue_send(xQueueHandle xQueue, const void *const pvItemToQueue, unsigned long xSecsToWait)
{
	// printf ("** %s\r\n", __func__);
	return xQueueSendToBack((xQueueHandle)xQueue, pvItemToQueue, (portTickType)(xSecsToWait * configTICK_RATE_HZ));
}

extern int min_free_heap_size;
u8 *os_malloc(u32 sz)
{
	int free_heap_size = rtw_getFreeHeapSize();
	// printf ("### %s %d bytes\r\n", __func__, sz);

	u8 *pbuf = _rtw_malloc(sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	add_mem_usage(&wpa_mem_table, pbuf, sz, &wpa_mem_used_num, MEM_MONITOR_FLAG_WPAS);
#else
	add_mem_usage(NULL, pbuf, sz, NULL, MEM_MONITOR_FLAG_WPAS);
#endif
	if (min_free_heap_size > free_heap_size)
		min_free_heap_size = free_heap_size;
	return pbuf;
}

void os_mfree(u8 *pbuf, u32 sz)
{
	// printf ("### %s %d bytes\r\n", __func__, sz);
	_rtw_mfree(pbuf, sz);
#if CONFIG_MEM_MONITOR & MEM_MONITOR_LEAK
	del_mem_usage(&wpa_mem_table, pbuf, &wpa_mem_used_num, MEM_MONITOR_FLAG_WPAS);
#else
	del_mem_usage(NULL, pbuf, NULL, MEM_MONITOR_FLAG_WPAS);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/* Not needed on 64bit architectures */
static unsigned int __div64_32(u64 *n, unsigned int base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	unsigned int high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base)
	{
		high /= base;
		res = (u64)high << 32;
		rem -= (u64)(high * base) << 32;
	}

	while ((u64)b > 0 && b < rem)
	{
		b = b + b;
		d = d + d;
	}

	do
	{
		if (rem >= b)
		{
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

/********************* os depended service ********************/

u8 *_freertos_malloc(u32 sz)
{
	// printf ("### %s %d bytes\r\n", __func__, sz);
	return pvPortMalloc(sz);
}

u8 *_freertos_zmalloc(u32 sz)
{
	// uint32_t r9 = ({uint32_t ret; asm("l.ori %0,r9,0":"=r"(ret));ret;});
	uint32_t r9 = ({uint32_t ret; asm("l.lwz %0,52(r1)":"=r"(ret));ret; });
	// printf ("** %s %X\r\n", __func__, r9);
	u8 *pbuf = _freertos_malloc(sz);

	if (pbuf != NULL)
		memset(pbuf, 0, sz);

	return pbuf;
}

void _freertos_mfree(u8 *pbuf, u32 sz)
{
	vPortFree(pbuf);
}

static void _freertos_memcpy(void *dst, void *src, u32 sz)
{
	memcpy(dst, src, sz);
}

static int _freertos_memcmp(void *dst, void *src, u32 sz)
{
	// under Linux/GNU/GLibc, the return value of memcmp for two same mem. chunk is 0
	// printf ("** %s\r\n", __func__);
	if (!(memcmp(dst, src, sz)))
		return 1;

	return 0;
}

static void _freertos_memset(void *pbuf, int c, u32 sz)
{
	memset(pbuf, c, sz);
}

static void _freertos_init_sema(_sema *sema, int init_val)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s\r\n", pcTaskGetName(NULL), __func__);
	*sema = xSemaphoreCreateCounting(0xffffffff, init_val); // Set max count 0xffffffff
}

static void _freertos_free_sema(_sema *sema)
{
	// printf ("** %s\r\n", __func__);
	if (*sema != NULL)
		vSemaphoreDelete(*sema);

	*sema = NULL;
}

static void _freertos_up_sema(_sema *sema)
{
	// printf ("** %s\r\n", __func__);
	xSemaphoreGive(*sema);
}

static u32 _freertos_down_sema(_sema *sema, u32 timeout)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s\r\n", pcTaskGetName(NULL), __func__);
	if (timeout == RTW_MAX_DELAY)
	{
		timeout = portMAX_DELAY;
	}
	else
	{
		timeout = rtw_ms_to_systime(timeout);
	}

	if (xSemaphoreTake(*sema, timeout) != pdTRUE)
	{
		return pdFALSE;
	}

	return pdTRUE;
}

static void _freertos_mutex_init(_mutex *pmutex)
{
	*pmutex = xSemaphoreCreateMutex();
	// printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __func__, *pmutex);
}

static void _freertos_mutex_free(_mutex *pmutex)
{
	// printf ("** %s\r\n", __func__);
	if (*pmutex != NULL)
		vSemaphoreDelete(*pmutex);

	*pmutex = NULL;
}

static void _freertos_mutex_get(_lock *plock)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __func__, *plock);
	while (xSemaphoreTake(*plock, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		DBG_ERR("\xA1\xBE%s\xA1\xBF%s(%p) failed, retry\n", pcTaskGetName(NULL), __FUNCTION__, plock);
}

static void _freertos_mutex_put(_lock *plock)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __func__, *plock);
	xSemaphoreGive(*plock);
}

static void _freertos_enter_critical(_lock *plock, _irqL *pirqL)
{
	*pirqL = arch_local_irq_save();
}

static void _freertos_exit_critical(_lock *plock, _irqL *pirqL)
{
	arch_local_irq_restore(*pirqL);
}

static int _freertos_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	int ret = 0;
	//  {uint32_t r9 = ({uint32_t ret; asm("l.ori %0,r9,0":"=r"(ret));ret;});
	//  {uint32_t r9 = ({uint32_t ret; asm("l.lwz %0,36(r1)":"=r"(ret));ret;});//32+4
	//  printf ("\xA1\xBE%s\xA1\xBF%s @%X\r\n", pcTaskGetName(NULL), __func__, r9);
	//  }

	while (xSemaphoreTake(*pmutex, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		printf("\n\r\xA1\xBE%s\xA1\xBF%s(%p) failed, retry\n", pcTaskGetName(NULL), __FUNCTION__, pmutex);
	// printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __func__, *pmutex);
	return ret;
}

static void _freertos_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __func__, *pmutex);
	xSemaphoreGive(*pmutex);
}

static void _freertos_spinlock_init(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
	*plock = xSemaphoreCreateMutex();
#endif
}

static void _freertos_spinlock_free(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
	if (*plock != NULL)
		vSemaphoreDelete(*plock);

	*plock = NULL;
#endif
}

static void _freertos_spinlock(_lock *plock)
{
#if USE_MUTEX_FOR_SPINLOCK
	while (xSemaphoreTake(*plock, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		DBG_ERR("\xA1\xBE%s\xA1\xBF%s(%p) failed, retry\n", pcTaskGetName(NULL), __FUNCTION__, plock);
#endif
	//	portDISABLE_INTERRUPTS();
}

static void _freertos_spinunlock(_lock *plock)
{
//	portENABLE_INTERRUPTS();
#if USE_MUTEX_FOR_SPINLOCK
	xSemaphoreGive(*plock);
#endif
}

static void _freertos_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
	*irqL = arch_local_irq_save();
#if USE_MUTEX_FOR_SPINLOCK
	while (xSemaphoreTake(*plock, 60 * 1000 / portTICK_RATE_MS) != pdTRUE)
		DBG_ERR("\xA1\xBE%s\xA1\xBF%s(%p) failed, retry\n", pcTaskGetName(NULL), __FUNCTION__, plock);
#endif
}

static void _freertos_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
#if USE_MUTEX_FOR_SPINLOCK
	xSemaphoreGive(*plock);
#endif
	arch_local_irq_restore(*irqL);
}

static int _freertos_init_xqueue(_xqueue *queue, const char *name, u32 message_size, u32 number_of_messages)
{
	printf("\xA1\xBE%s\xA1\xBF%s\r\n", pcTaskGetName(NULL), __func__);

	if ((*queue = xQueueCreate(number_of_messages, message_size)) == NULL)
	{
		return -1;
	}

	return 0;
}

static int _freertos_push_to_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	// printf ("** %s\r\n", __func__);
	if (timeout_ms == RTW_MAX_DELAY)
	{
		timeout_ms = portMAX_DELAY;
	}
	else
	{
		timeout_ms = rtw_ms_to_systime(timeout_ms);
	}

	if (xQueueSendToBack(*queue, message, timeout_ms) != pdPASS)
	{
		return -1;
	}

	return 0;
}

static int _freertos_pop_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s\r\n", pcTaskGetName(NULL), __func__);
	if (timeout_ms == RTW_WAIT_FOREVER)
	{
		timeout_ms = portMAX_DELAY;
	}
	else
	{
		timeout_ms = rtw_ms_to_systime(timeout_ms);
	}

	if (xQueueReceive(*queue, message, timeout_ms) != pdPASS)
	{
		return -1;
	}

	return 0;
}

static int _freertos_deinit_xqueue(_xqueue *queue)
{
	// printf ("** %s\r\n", __func__);
	int result = 0;

	if (uxQueueMessagesWaiting(queue))
	{
		result = -1;
	}
	vQueueDelete(*queue);
	return result;
}

static u32 _freertos_get_current_time(void)
{
	//  {uint32_t r9 = ({uint32_t ret; asm("l.lwz %0,36(r1)":"=r"(ret));ret;});//24+12
	//  printf ("\xA1\xBE%s\xA1\xBF%s @%X\r\n", pcTaskGetName(NULL), __func__, r9);
	//  }
	return xTaskGetTickCount(); // The count of ticks since vTaskStartScheduler was called.
}

static u32 _freertos_systime_to_ms(u32 systime)
{
	// printf ("** %s\r\n", __func__);
	return systime * portTICK_RATE_MS;
}

static u32 _freertos_systime_to_sec(u32 systime)
{
	// printf ("** %s\r\n", __func__);
	return systime / configTICK_RATE_HZ;
}

static u32 _freertos_ms_to_systime(u32 ms)
{
	// printf ("** %s\r\n", __func__);
	return ms / portTICK_RATE_MS;
}

static u32 _freertos_sec_to_systime(u32 sec)
{
	// printf ("** %s\r\n", __func__);
	return sec * configTICK_RATE_HZ;
}

static void _freertos_msleep_os(int ms)
{
	vTaskDelay(ms / portTICK_RATE_MS);
}

static void _freertos_mdelay_os(int ms)
{
	vTaskDelay(ms / portTICK_RATE_MS);
}

static void _freertos_usleep_os(int us)
{
#if defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A)
	DBG_ERR("%s: Please Implement micro-second delay\n", __FUNCTION__);
#else
	// #error "Please implement hardware dependent micro second level sleep here"
	_freertos_msleep_os(us / 1000);
#endif
}

static void _freertos_udelay_os(int us)
{
#if defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F10X_XL)
	// FreeRTOS does not provide us level delay. Use busy wait
	WLAN_BSP_UsLoop(us);
#elif defined(CONFIG_PLATFORM_8195A)
	RtlUdelayOS(us);
#else
	// #error "Please implement hardware dependent micro second level sleep here"
	_freertos_msleep_os(us / 1000);
#endif
}

static void _freertos_yield_os(void)
{
	//  {uint32_t r9 = ({uint32_t ret; asm("l.lwz %0,28(r1)":"=r"(ret));ret;});//24+4
	//  printf ("\xA1\xBE%s\xA1\xBF%s @%X\r\n", pcTaskGetName(NULL), __func__, r9);
	//  }

	taskYIELD();
}

//----- ------------------------------------------------------------------
// Timer Operation
//----- ------------------------------------------------------------------
void init_timer(struct timer_list *timer);
void mod_timer(struct timer_list *timer, u32 delay_time_ms);
void del_timer_sync(struct timer_list *timer);
void cancel_timer_ex(struct timer_list *timer);
static void _freertos_init_timer(_timer *ptimer, void *adapter, TIMER_FUN pfunc, void *cntx, const char *name)
{
	// printf ("** %s\r\n", __func__);
	ptimer->function = pfunc;
	ptimer->data = (u32)cntx;
	init_timer(ptimer);
}

static void _freertos_set_timer(_timer *ptimer, u32 delay_time_ms)
{
	// printf ("\xA1\xBE%s\xA1\xBF%s\r\n", pcTaskGetName(NULL), __func__);
	mod_timer(ptimer, delay_time_ms);
}

static u8 _freertos_cancel_timer_ex(_timer *ptimer)
{
	// printf ("** %s\r\n", __func__);
	cancel_timer_ex(ptimer);
	return 0;
}

static void _freertos_del_timer(_timer *ptimer)
{
	// printf ("** %s\r\n", __func__);
	del_timer_sync(ptimer);
}

static void _freertos_ATOMIC_SET(ATOMIC_T *v, int i)
{
	atomic_set(v, i);
}

static int _freertos_ATOMIC_READ(ATOMIC_T *v)
{
	return atomic_read(v);
}

static void _freertos_ATOMIC_ADD(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter += i;
	restore_flags();
}

static void _freertos_ATOMIC_SUB(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter -= i;
	restore_flags();
}

static void _freertos_ATOMIC_INC(ATOMIC_T *v)
{
	_freertos_ATOMIC_ADD(v, 1);
}

static void _freertos_ATOMIC_DEC(ATOMIC_T *v)
{
	_freertos_ATOMIC_SUB(v, 1);
}

static int _freertos_ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _freertos_ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _freertos_ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return _freertos_ATOMIC_ADD_RETURN(v, 1);
}

static int _freertos_ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return _freertos_ATOMIC_SUB_RETURN(v, 1);
}

static u64 _freertos_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;

	if (((n) >> 32) == 0)
	{
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	}
	else
		__rem = __div64_32(&(n), __base);

	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _freertos_arc4random(void)
{
	u32 res = xTaskGetTickCount();
	static unsigned long seed = 0xDEADB00B;
	seed = ((seed & 0x007F00FF) << 7) ^
		   ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
		   (res << 13) ^ (res >> 9);	// using the clock too!
	return (int)seed;
}

static int _freertos_get_random_bytes(void *buf, size_t len)
{
#if 1 // becuase of 4-byte align, we use the follow code style.
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *)buf;

	for (i = 0; i < count; i++)
	{
		lp[i] = _freertos_arc4random();
		len -= sizeof(unsigned int);
	}

	if (len > 0)
	{
		ranbuf = _freertos_arc4random();
		_freertos_memcpy(&lp[i], &ranbuf, len);
	}
	return 0;
#else
	unsigned long ranbuf, *lp;
	lp = (unsigned long *)buf;
	while (len > 0)
	{
		ranbuf = _freertos_arc4random();
		*lp++ = ranbuf; // this op need the pointer is 4Byte-align!
		len -= sizeof(ranbuf);
	}
	return 0;
#endif
}

static u32 _freertos_GetFreeHeapSize(void)
{
	return (u32)xPortGetFreeHeapSize();
}

static int _freertos_create_task(struct task_struct *ptask, const char *name,
								 u32 stack_size, u32 priority, thread_func_t func, void *thctx)
{
	thread_func_t task_func = NULL;
	void *task_ctx = NULL;
	int ret = 0;

	// printf ("** %s\r\n", __func__);
	ptask->task_name = name;
	ptask->blocked = 0;
	ptask->callback_running = 0;

	_freertos_init_sema(&ptask->wakeup_sema, 0);
	_freertos_init_sema(&ptask->terminate_sema, 0);
	// rtw_init_queue(&wq->work_queue);

	if (func)
	{
		task_func = func;
		task_ctx = thctx;
	}
	// else{
	//	task_func = freertos_wq_thread_handler;
	//	task_ctx = wq;
	// }
	priority += tskIDLE_PRIORITY + 19;
	// printk("/**************priority: %d************/",priority);
	if (stack_size < configMINIMAL_STACK_SIZE)
		stack_size = configMINIMAL_STACK_SIZE;
	ret = xTaskCreate(
		task_func,
		(const char *)name,
		stack_size,
		task_ctx,
		priority,
		&ptask->task);
	if (ret != pdPASS)
	{
		DBG_ERR("Create Task \"%s\" Failed! ret=%d\n", ptask->task_name, ret);
	}

	DBG_TRACE("Create Task \"%s\"\n", ptask->task_name);
	return ret;
}

static void _freertos_delete_task(struct task_struct *ptask)
{
	// printf ("** %s\r\n", __func__);
	if (!ptask->task)
	{
		DBG_ERR("_freertos_delete_task(): ptask is NULL!\n");
		return;
	}

	ptask->blocked = 1;

	_freertos_up_sema(&ptask->wakeup_sema);
	_freertos_down_sema(&ptask->terminate_sema, TIMER_MAX_DELAY);

	// rtw_deinit_queue(&wq->work_queue);
	_freertos_free_sema(&ptask->wakeup_sema);
	_freertos_free_sema(&ptask->terminate_sema);

	ptask->task = 0;

	DBG_TRACE("Delete Task \"%s\"\n", ptask->task_name);
}

void _freertos_wakeup_task(struct task_struct *ptask)
{
	// printf ("** %s\r\n", __func__);
	_freertos_up_sema(&ptask->wakeup_sema);
}

static void _freertos_thread_enter(char *name)
{
	DBG_INFO("\n\rRTKTHREAD %s\n", name);
}

static void _freertos_thread_exit(void)
{
	DBG_INFO("\n\rRTKTHREAD exit %s\n", __FUNCTION__);
	vTaskDelete(NULL);
}

_timerHandle _freertos_timerCreate(const signed char *pcTimerName,
								   osdepTickType xTimerPeriodInTicks,
								   u32 uxAutoReload,
								   void *pvTimerID,
								   TIMER_FUN pxCallbackFunction)
{
	// printf ("** %s\r\n", __func__);
	if (xTimerPeriodInTicks == TIMER_MAX_DELAY)
	{
		xTimerPeriodInTicks = portMAX_DELAY;
	}
	return xTimerCreate((const char *)pcTimerName, xTimerPeriodInTicks, uxAutoReload, pvTimerID, pxCallbackFunction);
}

u32 _freertos_timerDelete(_timerHandle xTimer,
						  osdepTickType xBlockTime)
{
	// printf ("** %s\r\n", __func__);
	return (u32)xTimerDelete(xTimer, xBlockTime);
}

u32 _freertos_timerIsTimerActive(_timerHandle xTimer)
{
	// printf ("** %s\r\n", __func__);
	return (u32)xTimerIsTimerActive(xTimer);
}

u32 _freertos_timerStop(_timerHandle xTimer,
						osdepTickType xBlockTime)
{
	// printf ("** %s\r\n", __func__);
	return (u32)xTimerStop(xTimer, xBlockTime);
}

u32 _freertos_timerChangePeriod(_timerHandle xTimer,
								osdepTickType xNewPeriod,
								osdepTickType xBlockTime)
{
	// printf ("** %s\r\n", __func__);
	if (xNewPeriod == 0)
		xNewPeriod += 1;
	return (u32)xTimerChangePeriod(xTimer, xNewPeriod, xBlockTime);
}

// for debug
void _freertos_enter_critical_bh(void)
{
	printf("%s %X\r\n", __FUNCTION__, ({uint32_t sp;asm("l.ori %0,r1,0":"=r"(sp));sp; }));
}

void _freertos_exit_critical_bh(void)
{
	printf("%s %X\r\n", __FUNCTION__, ({uint32_t sp;asm("l.ori %0,r1,0":"=r"(sp));sp; }));
}

const struct osdep_service_ops osdep_service = {
	_freertos_malloc,				// rtw_vmalloc
	_freertos_zmalloc,				// rtw_zvmalloc
	_freertos_mfree,				// rtw_vmfree
	_freertos_malloc,				// rtw_malloc
	_freertos_zmalloc,				// rtw_zmalloc
	_freertos_mfree,				// rtw_mfree
	_freertos_memcpy,				// rtw_memcpy
	_freertos_memcmp,				// rtw_memcmp
	_freertos_memset,				// rtw_memset
	_freertos_init_sema,			// rtw_init_sema
	_freertos_free_sema,			// rtw_free_sema
	_freertos_up_sema,				// rtw_up_sema
	_freertos_down_sema,			// rtw_down_sema
	_freertos_mutex_init,			// rtw_mutex_init
	_freertos_mutex_free,			// rtw_mutex_free
	_freertos_mutex_get,			// rtw_mutex_get
	_freertos_mutex_put,			// rtw_mutex_put
	_freertos_enter_critical,		// rtw_enter_critical
	_freertos_exit_critical,		// rtw_exit_critical
	_freertos_enter_critical_bh,	// rtw_enter_critical_bh
	_freertos_exit_critical_bh,		// rtw_exit_critical_bh
	_freertos_enter_critical_mutex, // rtw_enter_critical_mutex
	_freertos_exit_critical_mutex,	// rtw_exit_critical_mutex
	_freertos_spinlock_init,		// rtw_spinlock_init
	_freertos_spinlock_free,		// rtw_spinlock_free
	_freertos_spinlock,				// rtw_spin_lock
	_freertos_spinunlock,			// rtw_spin_unlock
	_freertos_spinlock_irqsave,		// rtw_spinlock_irqsave
	_freertos_spinunlock_irqsave,	// rtw_spinunlock_irqsave
	_freertos_init_xqueue,			// rtw_init_xqueue
	_freertos_push_to_xqueue,		// rtw_push_to_xqueue
	_freertos_pop_from_xqueue,		// rtw_pop_from_xqueue
	_freertos_deinit_xqueue,		// rtw_deinit_xqueue
	_freertos_get_current_time,		// rtw_get_current_time
	_freertos_systime_to_ms,		// rtw_systime_to_ms
	_freertos_systime_to_sec,		// rtw_systime_to_sec
	_freertos_ms_to_systime,		// rtw_ms_to_systime
	_freertos_sec_to_systime,		// rtw_sec_to_systime
	_freertos_msleep_os,			// rtw_msleep_os
	_freertos_usleep_os,			// rtw_usleep_os
	_freertos_mdelay_os,			// rtw_mdelay_os
	_freertos_udelay_os,			// rtw_udelay_os
	_freertos_yield_os,				// rtw_yield_os

	_freertos_init_timer,	   // rtw_init_timer
	_freertos_set_timer,	   // rtw_set_timer
	_freertos_cancel_timer_ex, // rtw_cancel_timer
	_freertos_del_timer,	   // rtw_del_timer

	_freertos_ATOMIC_SET,		 // ATOMIC_SET
	_freertos_ATOMIC_READ,		 // ATOMIC_READ
	_freertos_ATOMIC_ADD,		 // ATOMIC_ADD
	_freertos_ATOMIC_SUB,		 // ATOMIC_SUB
	_freertos_ATOMIC_INC,		 // ATOMIC_INC
	_freertos_ATOMIC_DEC,		 // ATOMIC_DEC
	_freertos_ATOMIC_ADD_RETURN, // ATOMIC_ADD_RETURN
	_freertos_ATOMIC_SUB_RETURN, // ATOMIC_SUB_RETURN
	_freertos_ATOMIC_INC_RETURN, // ATOMIC_INC_RETURN
	_freertos_ATOMIC_DEC_RETURN, // ATOMIC_DEC_RETURN

	_freertos_modular64,		// rtw_modular64
	_freertos_get_random_bytes, // rtw_get_random_bytes
	_freertos_GetFreeHeapSize,	// rtw_getFreeHeapSize

	_freertos_create_task, // rtw_create_task
	_freertos_delete_task, // rtw_delete_task
	_freertos_wakeup_task, // rtw_wakeup_task

	_freertos_thread_enter, // rtw_thread_enter
	_freertos_thread_exit,	// rtw_thread_exit

	_freertos_timerCreate,		  // rtw_timerCreate,
	_freertos_timerDelete,		  // rtw_timerDelete,
	_freertos_timerIsTimerActive, // rtw_timerIsTimerActive,
	_freertos_timerStop,		  // rtw_timerStop,
	_freertos_timerChangePeriod	  // rtw_timerChangePeriod
};

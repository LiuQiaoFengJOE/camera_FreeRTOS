#include "sys_arch.h"
#include "lwip/err.h"
#include "lwip/sys.h"
// #include "api.h"
// #include "app.h"

struct timeoutnode
{
	struct sys_timeouts timeouts;
	INT8U prio;
	struct timeoutnode *next;
};

struct timeoutnode nulltimeouts;

struct timeoutnode *timeoutslist;

const void *const pvNullPointer;

void sys_init(void)
{
	timeoutslist = &nulltimeouts;

	nulltimeouts.timeouts.next = NULL;

	nulltimeouts.next = NULL;
}

u32_t sys_jiffies(void)
{
	return jiffies;
}

u32_t sys_now(void)
{
	return jiffies * 10; // ���غ���
}

err_t sys_sem_new(sys_sem_t *pxSemaphore, u8_t ucCount)
{
	err_t xReturn = ERR_MEM;

	vSemaphoreCreateBinary((*pxSemaphore));

	if (*pxSemaphore != NULL)
	{
		if (ucCount == 0U)
		{
			xSemaphoreTake(*pxSemaphore, 1UL);
		}

		xReturn = ERR_OK;
	}

	if (xReturn)
		p_err("sys_sem_new err");

	return xReturn;
}

int sys_sem_valid(sys_sem_t *sem)
{
	if (*sem == NULL)
		return pdFALSE;

	return pdTRUE;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = NULL;
}
/*
;*****************************************************************************************************
;* �������� : sys_sem_free
;* ��    �� : ɾ��һ���ź�
;* �䡡	 �� : sem: �źž��
;*
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_sem_free(sys_sem_t *sem)
{
	vQueueDelete(*sem);
}

/*
;*****************************************************************************************************
;* �������� : sys_sem_signal
;* ��    �� : ����һ���ź�
;* �䡡	 �� : sem: �źž��
;*
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_sem_signal(sys_sem_t *sem)
{
	INT8U err;
	err = xSemaphoreGive(*sem);
	if (err != pdTRUE)
		Printf("sem_sig err:%d\n", err);
}

/*
;*****************************************************************************************************
;* �������� : sys_arch_sem_wait
;* ��    �� : �ȴ�һ���ź�
;* �䡡	 �� : sem: �źž��, timeout: �ȴ���ʱ��΢����
;*
;* �䡡	 �� : �ȴ����õ�΢����
;*****************************************************************************************************
;*/
u32_t sys_arch_sem_wait(sys_sem_t *pxSemaphore, u32_t ulTimeout)
{
	TickType_t xStartTime, xEndTime, xElapsed;
	unsigned long ulReturn;

	xStartTime = xTaskGetTickCount();

	if (ulTimeout != 0UL)
	{
		if (xSemaphoreTake(*pxSemaphore, ulTimeout / portTICK_PERIOD_MS) == pdTRUE)
		{
			xEndTime = xTaskGetTickCount();
			xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;
			ulReturn = xElapsed;
		}
		else
		{
			ulReturn = SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		xSemaphoreTake(*pxSemaphore, portMAX_DELAY);
		xEndTime = xTaskGetTickCount();
		xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

		if (xElapsed == 0UL)
		{
			xElapsed = 1UL;
		}

		ulReturn = xElapsed;
	}

	return ulReturn;
}

/*
;*****************************************************************************************************
;* �������� : sys_mbox_new
;* ��    �� : ����һ������
;* �䡡	 �� : size: ��������(ʵ�ʲ�������)
;*
;* �䡡	 �� : sys_mbox_t: ������
;*****************************************************************************************************
;*/
err_t sys_mbox_new(sys_mbox_t *pxMailBox, sys_sem_t *sem, int iSize)
{
	err_t xReturn = ERR_MEM;

	*pxMailBox = xQueueCreate(iSize, sizeof(void *));

	if (*pxMailBox != NULL)
	{
		xReturn = ERR_OK;
	}

	return xReturn;
}

int sys_mbox_valid(sys_mbox_t *pMbox)
{
	if (*pMbox == NULL)
		return pdFALSE;

	return pdTRUE;
}

void sys_mbox_set_invalid(sys_mbox_t *pMbox)
{
	*pMbox = NULL;
}
/*
;*****************************************************************************************************
;* �������� : sys_mbox_post
;* ��    �� : �����ʼ�������
;* �䡡	 �� : mbox: ������, msg: �ʼ�
;*
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_mbox_post(sys_mbox_t *pxMailBox, sys_sem_t *sem, void *pxMessageToPost)
{
	xQueueSendToBack(*pxMailBox, &pxMessageToPost, portMAX_DELAY);
}

/*
;*****************************************************************************************************
;* �������� : sys_mbox_trypost
;* ��    �� : ���Է����ʼ�������
;* �䡡	 �� : mbox: ������, msg: �ʼ�
;*
;* �䡡	 �� : ERROR: ERR_MEM | OK: ERR_OK
;*****************************************************************************************************
;*/

err_t sys_mbox_trypost(sys_mbox_t *pxMailBox, void *pxMessageToPost)
{
	err_t xReturn;

	if (xQueueSend(*pxMailBox, &pxMessageToPost, 0UL) == pdPASS)
	{
		xReturn = ERR_OK;
	}
	else
	{
		/* The queue was already full. */
		xReturn = ERR_MEM;
	}

	return xReturn;
}
/*
;*****************************************************************************************************
;* �������� : sys_arch_mbox_fetch
;* ��    �� : ������ȴ�һ���ʼ�
;* �䡡	 �� : mbox: ������, msg: �ʼ�, timeout: �ȴ���ʱ��΢����
;*
;* �䡡	 �� : �ȴ����õ�΢����
;*****************************************************************************************************
;*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *pxMailBox, sys_sem_t *sem, void **ppvBuffer, u32_t ulTimeOut)
{
	void *pvDummy;
	TickType_t xStartTime, xEndTime, xElapsed;
	unsigned long ulReturn;

	xStartTime = xTaskGetTickCount();

	if (NULL == ppvBuffer)
	{
		ppvBuffer = &pvDummy;
	}

	if (ulTimeOut != 0UL)
	{
		if (pdTRUE == xQueueReceive(*pxMailBox, &(*ppvBuffer), ulTimeOut / portTICK_PERIOD_MS))
		{
			xEndTime = xTaskGetTickCount();
			xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

			ulReturn = xElapsed;
		}
		else
		{
			/* Timed out. */
			*ppvBuffer = NULL;
			ulReturn = SYS_ARCH_TIMEOUT;
		}
	}
	else
	{
		xQueueReceive(*pxMailBox, &(*ppvBuffer), portMAX_DELAY);
		xEndTime = xTaskGetTickCount();
		xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

		if (xElapsed == 0UL)
		{
			xElapsed = 1UL;
		}

		ulReturn = xElapsed;
	}

	return ulReturn;
}

/*
;*****************************************************************************************************
;* �������� : sys_arch_mbox_tryfetch
;* ��    �� : ���Դ�����ȴ�һ���ʼ�
;* �䡡	 �� : mbox: ������, msg: �ʼ�
;*
;* �䡡	 �� : ERROR: SYS_MBOX_EMPTY | OK: 0
;*****************************************************************************************************
;*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *pxMailBox, void **ppvBuffer)
{
	void *pvDummy;
	unsigned long ulReturn;

	if (ppvBuffer == NULL)
	{
		ppvBuffer = &pvDummy;
	}

	if (pdTRUE == xQueueReceive(*pxMailBox, &(*ppvBuffer), 0UL))
	{
		ulReturn = ERR_OK;
	}
	else
	{
		ulReturn = SYS_MBOX_EMPTY;
	}

	return ulReturn;
}

/*
;*****************************************************************************************************
;* �������� : sys_mbox_free
;* ��    �� : ɾ��һ������
;* �䡡	 �� : mbox: ������
;*
;* �䡡	 �� : ��
;*****************************************************************************************************
;*/
void sys_mbox_free(sys_mbox_t *pxMailBox, sys_sem_t *sem)
{
	unsigned long ulMessagesWaiting;

	ulMessagesWaiting = uxQueueMessagesWaiting(*pxMailBox);
	configASSERT((ulMessagesWaiting == 0));

#if SYS_STATS
	{
		if (ulMessagesWaiting != 0UL)
		{
			/**/
		}
	}
#endif /* SYS_STATS */

	vQueueDelete(*pxMailBox);
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{

	return thread_create(thread,
						 arg,
						 prio,
						 0,
						 stacksize,
						 (char *)name);
}
#ifdef WIFI_RTL8189FTV_SUPPORT
struct timeoutlist
{
	struct sys_timeouts timeouts;
	xTaskHandle pid;
};

#define SYS_THREAD_MAX 6
static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];
static u16_t s_nextthread = 0;

struct sys_timeouts *sys_arch_timeouts(void)
{
	int i;
	xTaskHandle pid;
	struct timeoutlist *tl;

	pid = xTaskGetCurrentTaskHandle();

	for (i = 0; i < s_nextthread; i++)
	{
		tl = &(s_timeoutlist[i]);
		if (tl->pid == pid)
		{
			return &(tl->timeouts);
		}
	}
	// Error
	return NULL;
}
#endif
/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/

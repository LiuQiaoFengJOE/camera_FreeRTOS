#define DEBUG
#include "api.h"

static struct workqueue_struct_legacy *timer_tasklet = NULL;
TaskHandle_t pxCurrentTCB;

static void os_workqueue_thread(struct workqueue_struct_legacy *workqueue);

int thread_create(void (*task)(void *p_arg), void *p_arg, unsigned int prio, unsigned int *pbos, unsigned int stk_size, char *name)
{
	TaskHandle_t xCreatedTask;

	return xTaskCreate(task, name, stk_size, p_arg, prio, &xCreatedTask) == pdPASS ? xCreatedTask : NULL;
}

int thread_exit(TaskHandle_t xTaskToDelete)
{
	int ret = 0;

	vTaskDelete(xTaskToDelete);

	return ret;
}

// ·µ»ØÈÎÎñ×ÔÉíµÄIDºÅ
int thread_myself()
{
	return pxCurrentTCB;
}

void initList(PLIST_HEADER_LEGACY pList)
{
	pList->pHead = pList->pTail = NULL;
	pList->size = 0;
	return;
}

void insertTailList(PLIST_HEADER_LEGACY pList, PLIST_ENTRY_LEGACY pEntry)
{
	uint32_t cpu_sr;
	assert(pList);
	cpu_sr = arch_local_irq_save();
	pEntry->pNext = NULL;
	if (pList->pTail)
		pList->pTail->pNext = pEntry;
	else
		pList->pHead = pEntry;
	pList->pTail = pEntry;
	pList->size++;
	arch_local_irq_restore(cpu_sr);
	return;
}

PLIST_ENTRY_LEGACY removeHeadList(PLIST_HEADER_LEGACY pList)
{
	PLIST_ENTRY_LEGACY pNext;
	PLIST_ENTRY_LEGACY pEntry = 0;
	uint32_t cpu_sr;
	assert(pList);
	cpu_sr = arch_local_irq_save();
	pEntry = pList->pHead;
	if (pList->pHead != NULL)
	{
		pNext = pList->pHead->pNext;
		pList->pHead = pNext;
		if (pNext == NULL)
			pList->pTail = NULL;
		pList->size--;
	}
	else
	{

		pList->size = 0;
	}
	arch_local_irq_restore(cpu_sr);
	return pEntry;
}

int getListSize(PLIST_HEADER_LEGACY pList)
{
	return pList->size;
}

PLIST_ENTRY_LEGACY delEntryList(PLIST_HEADER_LEGACY pList, PLIST_ENTRY_LEGACY pEntry)
{
	PLIST_ENTRY_LEGACY pCurEntry;
	PLIST_ENTRY_LEGACY pPrvEntry;

	if (pList->pHead == NULL)
		return NULL;

	if (pEntry == pList->pHead)
	{
		pCurEntry = pList->pHead;
		pList->pHead = pCurEntry->pNext;

		if (pList->pHead == NULL)
			pList->pTail = NULL;

		pList->size--;
		return pCurEntry;
	}

	pPrvEntry = pList->pHead;
	pCurEntry = pPrvEntry->pNext;
	while (pCurEntry != NULL)
	{
		if (pEntry == pCurEntry)
		{
			pPrvEntry->pNext = pCurEntry->pNext;

			if (pEntry == pList->pTail)
				pList->pTail = pPrvEntry;

			pList->size--;
			break;
		}
		pPrvEntry = pCurEntry;
		pCurEntry = pPrvEntry->pNext;
	}

	return pCurEntry;
}

struct workqueue_struct_legacy *create_workqueue_legacy(const char *name, uint32_t stack_size, int prio)
{
	struct workqueue_struct_legacy *new_queue = (struct workqueue_struct_legacy *)mem_calloc(sizeof(struct workqueue_struct_legacy), 1);
	if (!new_queue)
		return NULL;
	// Printf("%d\n",__LINE__);
	new_queue->thread_event = init_event();
	if (!new_queue->thread_event)
		goto faild1;
	// Printf("%d\n",__LINE__);
	initList(&new_queue->list);
	new_queue->name = name;
	// Printf("->new_queue:%x\n",new_queue);
	new_queue->task_id = prio; // thread_createµ÷ÓÃµÄÊ±ºò¿ÉÄÜÂíÉÏ»áÇÐ»»µ½ÐÂÏß³Ì£¬ÕâÀïÔ¤ÏÈ¸³ºÃÖµ
	new_queue->task_id = thread_create((void (*)(void *))os_workqueue_thread,
									   new_queue,
									   prio,
									   NULL,
									   stack_size,
									   (char *)name);

	if (new_queue->task_id <= 0)
		goto faild;

	return new_queue;
faild:
	del_event(new_queue->thread_event);
	;
faild1:
	free(new_queue);
	taskYIELD();
	return NULL;
}

void destroy_workqueue_legacy(struct workqueue_struct_legacy *workqueue)
{
	if (!workqueue)
		return;
	p_dbg("destroy %s", workqueue->name);
	del_event(workqueue->thread_event);
	thread_exit(workqueue->task_id);
	free(workqueue);
}

/*
 *¹¤×÷Ïß³Ì
 *²Î¿¼linuxµÄworkqueue»úÖÆ
 *ÎªÒì²½ÊÂ¼þµÄ´¦ÀíÌá¹©Ò»ÖÖºÜºÃµÄ·½Ê½
 */
static void os_workqueue_thread(struct workqueue_struct_legacy *workqueue)
{
	int ret;
	uint32_t irq_flag;
	// Printf("<-new_queue:%x\n",workqueue);
	p_dbg("start workqueue %s id:%d", workqueue->name, workqueue->task_id);
	while (1)
	{
		ret = wait_event_timeout_legacy(workqueue->thread_event, 5000);
		if (ret != 0)
		{
			if (ret != WAIT_EVENT_TIMEOUT)
			{
				p_err("os_tasklet_thread wait err:%d, %x\n", ret, workqueue->thread_event);
				sleep(2000);
			}
			continue;
		}
		if (workqueue->list.size > 0)
		{

			while (workqueue->list.size)
			{
				PLIST_ENTRY_LEGACY entry = NULL;
				irq_flag = arch_local_irq_save();
				if (workqueue->list.size)
					entry = removeHeadList(&workqueue->list);
				arch_local_irq_restore(irq_flag);

				if (entry)
				{

					struct work_struct_legacy *work;
					work = (struct work_struct_legacy *)((char *)entry - offsetof(struct work_struct_legacy, entry));

					if (work->fun)
					{
						// printf("--isr---:%x\n",work->fun);
						work->fun(work->data);
						// printf("--------:%x\n\n",work->fun);
					}
				}
				else
				{
					p_err("no entry:%d\n", workqueue->list.size);
					break;
				}
			}
		}
	}
}

/*
 *¶¨Ê±Æ÷·þÎñÏß³Ì
 *Òòucos×Ô´øµÄ¶¨Ê±Æ÷º¯Êý²»ÔÊÐíÊ¹ÓÃÐÅºÅµÈ´ý²Ù×÷
 *ÕâÀïµ¥¶À½¨Ò»¸öÏß³Ì
 *×¢Òâ:ÒÑ¾­¶ÔucosÄÚºË×öÁËÒ»µãÐÞ¸Ä
 */
int init_timer_tasklet()
{
#ifdef WIFI_SV6030P_SUPPORT
	timer_tasklet = create_workqueue_legacy("timer_tasklet", TIMER_TASKLET_STACK_SIZE, 6 /*19TASK_TIMER_TASKLET_PRIO*/);
#else
	timer_tasklet = create_workqueue_legacy("timer_tasklet", TIMER_TASKLET_STACK_SIZE, 18 /*TASK_TIMER_TASKLET_PRIO*/);
#endif
	if (timer_tasklet)
		return 0;
	else
		return -1;
}
void init_work(struct work_struct_legacy *work, void *fun, void *data)
{
	work->fun = (void (*)(void *))fun;
	work->data = data;
}

int schedule_work_api(struct workqueue_struct_legacy *queue, struct work_struct_legacy *work)
{
	insertTailList(&queue->list, &work->entry);
	wake_up_legacy(queue->thread_event);
	return 0;
}

int schedule_work_FromISR(struct workqueue_struct_legacy *queue, struct work_struct_legacy *work)
{
	insertTailList(&queue->list, &work->entry);
	wake_up_FromISR(queue->thread_event);
	return 0;
}

int schedule_timer_tasklet(struct work_struct_legacy *work)
{
	int ret = -1;
	if (timer_tasklet)
		ret = schedule_work_api(timer_tasklet, work);
	return ret;
}

int schedule_timer_tasklet_FromISR(struct work_struct *work)
{
	int ret = -1;
	if (timer_tasklet)
		ret = schedule_work_FromISR(timer_tasklet, work);
	return ret;
}

void timer_tasklet_callback(struct work_struct_legacy *work)
{
	schedule_timer_tasklet(work);
}

void init_work_thread(void)
{
	init_timer_tasklet();
}

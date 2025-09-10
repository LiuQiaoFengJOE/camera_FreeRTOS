#include "eventTimer.h"
#include "osal/osal_wait.h"	 //xQueueHandle
#include "osal/osal_alloc.h" //osal_malloc  osal_free

/********************************软定时使用***************************************/

static xQueueHandle timer_Queue_HIGH = NULL;
static xQueueHandle timer_Queue_LOW = NULL;

xQueueHandle get_Timer_queue(u8 which)
{
	if (which == HIGH)
	{
		return timer_Queue_HIGH;
	}
	else
	{
		return timer_Queue_LOW;
	}
}

xQueueHandle init_timer_Queue(u8 which)
{
	if (which == HIGH)
	{
		timer_Queue_HIGH = xQueueCreate(20, sizeof(struct timerList));
		return timer_Queue_HIGH;
	}
	else
	{
		timer_Queue_LOW = xQueueCreate(20, sizeof(struct timerList));
		return timer_Queue_LOW;
	}
}

static struct timerList timer_List_LOW;
static struct timerList timer_List_HIGH;

struct timerList *get_Timer_List(u8 which)
{
	if (which == HIGH)
	{
		return &timer_List_HIGH;
	}
	else
	{
		return &timer_List_LOW;
	}
}
static void InitTList(struct timerList *TList)
{
	TList->next = TList;
	TList->head = TList;
	TList->timerEvent = NULL;
	TList->reponseEvent = NULL;
}

static void AddTimerList(struct timerList *TList, struct timerList *add)
{
	struct timerList *end;
	end = TList->head;
	TList->head = add;
	add->head = end;
	add->next = TList;
	end->next = add;
	printf("addList:%X\r\n", add);
}

static void RemoveTimerList(struct timerList *remove)
{
	struct timerList *next = remove->next;
	struct timerList *head = remove->head;
	next->head = head;
	head->next = next;
	printf("remove List:%X\r\n", remove);
}

/*****************************************************************************************************
timerEvent:定时执行事件
reponseEvent:每1ms都会执行一次,因此里面不要做耗时工作,主要是判断一些标志
						 里面还可以修改time的值,达到动态修改定时的值

timer:	>0  到达定时后,执行一次timerEvent事件,然后将该List移除
				<0	则不执行timerEvent,也不会移除该List事件,但可以通过reponseEvent来修改timer的值,从而达到退出

注意:如果timerQueue已经满了,会一直阻塞,直到该事件被发送,如果需要不阻塞,需要修改
		 不能再中断使用
*******************************************************************************************************/
void add_event_timerList(u8 which, timer_callback timerEvent, timer_callback reponseEvent, void *d, int timer)
{
	printf("start add_event_timerList:%d!\r\n", which);
	if (timerEvent)
	{
		printf("AtimerEvent:%X\r\n", timerEvent);
	}

	if (reponseEvent)
	{
		printf("AreponseEvent:%X\r\n", reponseEvent);
	}
	xQueueHandle timerQueue = get_Timer_queue(which);
	if (!timerQueue)
	{
		printf("timerQueue is not ready:%d\r\n", which);
		return;
	}
	struct timerList *Tlist = osal_malloc(sizeof(struct timerList));
	// printf("Tlist:%X\r\n",Tlist);
	struct timerCommon *common = osal_malloc(sizeof(struct timerCommon));
	// printf("common:%X\r\n",common);
	Tlist->common = common;
	Tlist->timerEvent = timerEvent;
	Tlist->reponseEvent = reponseEvent;
	Tlist->common->timer = timer;
	Tlist->common->startTimer_count = os_time_get();
	// printf("timer:%d\r\n",timer);
	Tlist->common->nowTimer = 0;
	Tlist->d = d;
	common->d = d;
	xQueueSend(timerQueue, Tlist, portMAX_DELAY);
	osal_free(Tlist);
	// osal_task_sleep(1);
}

void add_event_timerList_ISR(u8 which, timer_callback timerEvent, timer_callback reponseEvent, void *d, int timer)
{
	printf("start add_event_timerList!\r\n");
	struct timerList *Tlist = osal_malloc(sizeof(struct timerList));
	struct timerCommon *common = osal_malloc(sizeof(struct timerCommon));

	Tlist->common = common;
	xQueueHandle timerQueue = get_Timer_queue(which);
	Tlist->timerEvent = timerEvent;
	Tlist->reponseEvent = reponseEvent;
	Tlist->common->timer = timer;
	Tlist->common->countdown.countdownI = 0;
	printf("timer:%d\r\n", timer);
	Tlist->common->nowTimer = 0;
	Tlist->d = d;
	common->d = d;
	xQueueSendToBackFromISR(timerQueue, Tlist, portMAX_DELAY);
}

/**************************************************************************************
定时的线程,到达定时后,执行回调函数
注意:定时线程最好不要执行耗时的东西,否则可能时间会不准,至少在这个模式下是不准确的定时
			这里坚决不执行耗时任务
*************************************************************************************/
void event_timerList_thread_HIGH(void *arg)
{
	printf("event_timerList_thread_HIGH start!\r\n");

	long res = 0;
	unsigned int nowtimer_count;
	struct timerList *Tlist;
	struct timerList *Malloc_Tlist = NULL;
	struct timerList *excuteTlist = NULL;

	if (get_Timer_queue(HIGH))
	{
		printf("%s was already created!\n");
		return;
	}
	struct timerList *start_Tlist = get_Timer_List(HIGH);
	xQueueHandle timer_Queue = init_timer_Queue(HIGH);
	InitTList(start_Tlist);
	default_event_timer_HIGH();
	while (1)
	{
		// 接收队列信息,并且加入到链表,然后执行链表的事件
		for (;;)
		{
			if (!Malloc_Tlist)
			{
				Malloc_Tlist = osal_malloc(sizeof(struct timerList));
			}
			if (!Malloc_Tlist)
			{
				printf("Tlist malloc fail!\r\n");
				break;
			}
			res = xQueueReceive(timer_Queue, Malloc_Tlist, 0);
			if (res)
			{
				printf("add event Tlist!\r\n");
				printf("timerEvent:%X\r\n", Malloc_Tlist->timerEvent);
				printf("reponseEvent:%X\r\n", Malloc_Tlist->reponseEvent);
				AddTimerList(start_Tlist, Malloc_Tlist);
				Malloc_Tlist = NULL;
			}
			else
			{
				// osal_free(Malloc_Tlist);
				break;
			}
		}
		nowtimer_count = os_time_get();
		// 循环执行链表函数,如果reponseEvent与timerEvent都为空,说明已经结束
		for (Tlist = start_Tlist->next; Tlist != NULL && (Tlist->timerEvent != NULL || Tlist->reponseEvent != NULL);)
		{
			excuteTlist = Tlist;
			Tlist = Tlist->next;
			excuteTlist->common->nowTimer_count = nowtimer_count;
			excuteTlist->common->nowTimer = nowtimer_count - excuteTlist->common->startTimer_count;
			// 如果存在,则每一次都要执行
			if (excuteTlist->reponseEvent)
			{
				// printf("QreponseEvent:%X!\r\n",excuteTlist->reponseEvent);
				excuteTlist->reponseEvent(excuteTlist->common);
			}
			// excuteTlist->common->nowTimer ++;

			// 只有定时时间>0才去查询定时任务
			if (excuteTlist->common->timer >= 0)
			{
				if (excuteTlist->common->nowTimer >= excuteTlist->common->timer)
				{
					if (excuteTlist->timerEvent)
					{
						// printf("QtimerEvent:%X!\r\n",excuteTlist->timerEvent);
						excuteTlist->timerEvent(excuteTlist->d);
					}
					printf("excuteTlist->common->timer:%d\r\n", excuteTlist->common->timer);
					osal_free(excuteTlist->common);
					osal_free(excuteTlist);
					RemoveTimerList(excuteTlist);
					printf("remove List!\r\n");
				}
			}
		}
		osal_task_sleep(1);
	}
}

/**********************************************************
在这个timerList里面,允许做耗时的事情,但都是无关紧要的事情
就是这里的事情对程序其他部分基本上影响不大的事件
例如:电池检测、sd卡检测、自动关屏、自动关机等
现有固件中有超级耗时的timerList有:儿童相机的相框生成,最大耗时
有500ms,但在录像与拍照会等待相框生成完成后才会进行,因此如果
按键按得过快,理论上是会影响ui响应,但如果生成相框事件优先级
过高,也会影响到ui响应,太高的话,会先执行相框部分代码,后续如果
可以的话,继续优化
**********************************************************/
void event_timerList_thread_LOW(void *arg)
{
	printf("event_timerList_thread_LOW start!\r\n");
	long res = 0;
	unsigned int nowtimer_count;
	struct timerList *Tlist;
	struct timerList *Malloc_Tlist = NULL;
	struct timerList *excuteTlist = NULL;
	if (get_Timer_queue(LOW))
	{
		printf("%s was already created!\n");
		return;
	}
	struct timerList *start_Tlist = get_Timer_List(LOW);
	unsigned int time_list_low_num = 0;
	xQueueHandle timer_Queue = init_timer_Queue(LOW);
	InitTList(start_Tlist);
	// default_event_timer();
	default_event_timer_LOW();
	while (1)
	{
		// 接收队列信息,并且加入到链表,然后执行链表的事件
		for (;;)
		{
			if (!Malloc_Tlist)
			{
				Malloc_Tlist = osal_malloc(sizeof(struct timerList));
			}
			if (!Malloc_Tlist)
			{
				printf("Tlist malloc fail!\r\n");
				break;
			}
			res = xQueueReceive(timer_Queue, Malloc_Tlist, 0);
			if (res)
			{
				time_list_low_num++;
				printf("add event Tlist:%d!\r\n", time_list_low_num);
				printf("timerEvent:%X\r\n", Malloc_Tlist->timerEvent);
				printf("reponseEvent:%X\r\n", Malloc_Tlist->reponseEvent);

				AddTimerList(start_Tlist, Malloc_Tlist);
				Malloc_Tlist = NULL;
			}
			else
			{
				// osal_free(Malloc_Tlist);
				break;
			}
		}
		nowtimer_count = os_time_get();
		// 循环执行链表函数,如果reponseEvent与timerEvent都为空,说明已经结束
		for (Tlist = start_Tlist->next; Tlist != NULL && (Tlist->timerEvent != NULL || Tlist->reponseEvent != NULL);)
		{
			excuteTlist = Tlist;
			Tlist = Tlist->next;
			excuteTlist->common->nowTimer_count = nowtimer_count;
			excuteTlist->common->nowTimer = nowtimer_count - excuteTlist->common->startTimer_count;
			// 如果存在,则每一次都要执行
			if (excuteTlist->reponseEvent)
			{
				// printf("QreponseEvent:%X!\r\n",excuteTlist->reponseEvent);
				excuteTlist->reponseEvent(excuteTlist->common);
			}
			// excuteTlist->common->nowTimer ++;

			// 只有定时时间>0才去查询定时任务
			if (excuteTlist->common->timer >= 0)
			{
				if (excuteTlist->common->nowTimer >= excuteTlist->common->timer)
				{
					if (excuteTlist->timerEvent)
					{
						// printf("QtimerEvent:%X!\r\n",excuteTlist->timerEvent);
						excuteTlist->timerEvent(excuteTlist->d);
					}
					// printf("excuteTlist->common->timer:%d\r\n",excuteTlist->common->timer);
					osal_free(excuteTlist->common);
					osal_free(excuteTlist);
					RemoveTimerList(excuteTlist);
					time_list_low_num--;
					// printf("remove List!\r\n");
				}
			}
		}
		osal_task_sleep(1);
	}
}

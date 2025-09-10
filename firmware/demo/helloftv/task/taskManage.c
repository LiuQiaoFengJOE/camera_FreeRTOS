#include "app.h"
#include"sUI/sUI.h"
#include <stdarg.h>

static _osTaskInfor osTasks[OS_TASK_MAX];
QueueHandle_t getTaskMsgq(u32 id)
{
	if(id>=OS_TASK_MAX)
		return NULL;
	return osTasks[id].msgQ;
}

void osTasksInit(void)
{
	int i;
	for(i=0;i<OS_TASK_MAX;i++)
	{
		osTasks[i].msgQ=NULL;
	}
}
void createTaskByTaskInfor(osTaskInfor* infor,u32 size)
{
	u32 i,id;
	//memset(&osTasks,0,sizeof(osTasks));
	for(i=0;i<size;i++)
	{
		id=infor[i].id;
		osTasks[id].id=infor[i].id;
		osTasks[id].func=infor[i].func;
		osTasks[id].priority=infor[i].priority;
		osTasks[id].stackSize=infor[i].stackSize;
		osTasks[id].taskName=infor[i].taskName;
		osTasks[id].msgQSize=infor[i].msgQSize;
		if( osTasks[id].msgQSize)
		{
			printf("create msgQ for task[%s]\n",osTasks[id].taskName);
			osTasks[id].msgQ=osal_msgq_create ("none", osTasks[id].msgQSize);
			vQueueSetQueueNumber(osTasks[id].msgQ,id);
		}
		//init_waitqueue_head (&osTasks[id].wq);
		if(osTasks[id].func)
		{
			osTasks[id].taskHandle=osal_thread_create_withID(osTasks[id].func, NULL, osTasks[id].priority, 0, osTasks[id].stackSize, osTasks[id].taskName ,osTasks[id].id);
		}
	}
}

void createdTaskByFunc(OS_TASK_ID id,TaskFunction_t  taskFunc,void* parame,u32 priority,u16 stackSize,char* name,u32 msgQsize)
{
	if(taskFunc==NULL)
	{
		printf("task %d func is NULL\n",id);
		return;
	}
	if(id>=OS_TASK_MAX)
	{
		printf("addCreatedTask: id [%d] >= OS_TASK_MAX\n",id);
		return;
	}
	if(osTasks[id].taskHandle||osTasks[id].func)
	{
		printf("task %d has existed\n",id);
		return;
	}
	osTasks[id].id=id;
	osTasks[id].priority=priority;
	osTasks[id].stackSize=stackSize;
	osTasks[id].func=taskFunc;
	osTasks[id].taskName=name;
	osTasks[id].msgQSize=msgQsize;
	if( osTasks[id].msgQSize)
	{
		printf("create msgQ for task[%s]\n",osTasks[id].taskName);
		osTasks[id].msgQ=osal_msgq_create ("none", osTasks[id].msgQSize);
		vQueueSetQueueNumber(osTasks[id].msgQ,id);
	}
	//init_waitqueue_head (&osTasks[id].wq);
	osTasks[id].taskHandle=osal_thread_create_withID(osTasks[id].func, parame, osTasks[id].priority, 0, osTasks[id].stackSize, osTasks[id].taskName ,osTasks[id].id);
}

extern void taskUI(void* arg);
extern void taskScan(void* arg);
#if GAME_FUNC_EN
extern void taskGame(void* arg);
#endif
static osTaskInfor osTaskInfors[]=
{
	{OS_TASK_UI, taskUI, 24, 2*1024, "taskUI",20},
	{OS_TASK_SCAN,taskScan, 15, 2*1024, "taskScan" ,20},
#if GAME_FUNC_EN
	{OS_TASK_GAME,taskGame, 4, 1*1024, "taskgame" ,20},
#endif
	//{OS_TASK_ISP,taskISP, 12, 8192, "taskISP" ,0},
	//{OS_TASK_STORAGE,taskSorage, 18, 8192, "taskSorage" ,20},
};


void createTasks(void)
{
	createTaskByTaskInfor(&osTaskInfors, sizeof(osTaskInfors)/sizeof(osTaskInfors[0]));
}

#define TASK_QUEUE_SIZE    30
TaskHandle_t taskQueue[TASK_QUEUE_SIZE];
u32 taskQLen=0;
int taskQueueEn=0;
void taskQueueEnable(int en)
{
	taskQueueEn=en;
	printf("taskQueueEn:%d\n",taskQueueEn);
}
void addToTaskQueue(TaskHandle_t hand)
{
	if(!taskQueueEn)
		return;
	if(taskQLen>=TASK_QUEUE_SIZE)
	{
		printf("task queue full:%d\n",taskQLen);
		return;
	}
	taskQueue[taskQLen++]=hand;
}
void showTaskFreeStak(void)
{
	int i;
	u32 freeStak;
	for(i=0;i<taskQLen;i++)
	{
		freeStak=uxTaskGetStackHighWaterMark(taskQueue[i]);
		printf("thread [%s] stack size:%d,use:%d,Prio:%d\n",pcTaskGetName(taskQueue[i]),uxTaskGetStackSize(taskQueue[i]),uxTaskGetStackSize(taskQueue[i])-freeStak,uxTaskPriorityGet(taskQueue[i]));
	}
}



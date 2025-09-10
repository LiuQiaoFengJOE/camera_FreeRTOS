#ifndef  __OS_TASK_MSG_H
#define  __OS_TASK_MSG_H
#include "typedef.h"
#include "osal/osal_task.h"
#include "osal/osal_wait.h"
#include "osal/osal_msgq.h"

typedef struct
{
	u32     id;
	TaskFunction_t  func;
	u32    priority;
	u16    stackSize;
	char*    taskName;
	u32        msgQSize;
}osTaskInfor;
typedef struct
{
	u32     id;
	TaskFunction_t  func;
	u32    priority;
	u16    stackSize;
	char*    taskName;
	u32        msgQSize;
	QueueHandle_t  msgQ;
	TaskHandle_t       taskHandle;
	//wait_queue_head_t   wq;
}_osTaskInfor;

typedef  void (*msgCallBackFunc)(void*);
typedef  void (*msgDoneFunc)(void*);
#define    MAX_MSG_PARAME          5
typedef struct  _OS_EXE_CLIENT_MSG
{
	u32   parame[MAX_MSG_PARAME];
	msgCallBackFunc  callbackFunc;
	msgDoneFunc         done;
	wait_queue_head_t   *wq;
	int  ret;
}OS_EXE_CLIENT_MSG;
typedef struct  _OS_WAIT_DONE_MSG
{
	u32 msgID;
	u32 *msgData;
	msgDoneFunc         done;
	wait_queue_head_t   *wq;
	int  ret;
}OS_WAIT_DONE_MSG;


int sendMsgFromIsr(uint32 id,uint32 msgType,uint32 msgData);
int sendMsg(uint32 id,uint32 msgType,uint32 msgData);
int sendMsgNonblock(uint32 id,uint32 msgType,uint32 msgData);
int recMsg(uint32 id,uint32 *msgType,uint32 *msgData);
int sendMsgAndWait(uint32 id,uint32 msgType,uint32 *msgData);
int sendMsgFrontAndWait(uint32 id,uint32 msgType,uint32 *msgData);
int executeMsgNoWaite(uint32 id,msgCallBackFunc func,uint32 argc,...);
int executeMsgAndWaite(uint32 id,msgCallBackFunc func,uint32 argc,...);
u32 getMsgQWaiting(uint32 id);
u32 getMsgQRemain(uint32 id);





#endif

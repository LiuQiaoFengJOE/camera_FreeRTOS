#ifndef ATBM_OS_THREAD_H
#define ATBM_OS_THREAD_H

#include "atbm_type.h"
#if 0

atbm_uint32 atbm_os_CreateTask(atbm_void (*taskproc)(atbm_void *param), atbm_thread_t *task_cfg, atbm_void *param);
ATBM_BOOL atbm_os_setTaskName(atbm_uint8 ubNewPriority, atbm_uint8 *pTaskName);
atbm_void atbm_os_startTask(atbm_void);
atbm_uint8 atbm_os_changeTaskPri(atbm_uint8 ubold_pri, atbm_uint8 ubnew_pri);
atbm_uint8 atbm_os_delTask(atbm_uint8 taskid);
atbm_uint8 atbm_os_suspendTask(atbm_uint8 taskid);
atbm_uint8 atbm_os_resumeTask(atbm_uint8 taskid);
atbm_void atbm_os_StartTask(atbm_void);
#endif

typedef int pAtbm_thread_t ;
/*priotry */
enum  ATBM_THREAD_PRIO
{
	WORK_TASK_PRIO = 19,
	BH_TASK_PRIO,
	TX_BH_TASK_PRIO=BH_TASK_PRIO,
    RX_BH_TASK_PRIO=BH_TASK_PRIO,
	ELOOP_TASK_PRIO,
	HIF_TASK_PRIO,
};

pAtbm_thread_t atbm_createThread(char *name ,atbm_void(*task)(atbm_void *p_arg),atbm_void *p_arg,int prio);

//pAtbm_thread_t atbm_createThread(atbm_void(*task)(atbm_void *p_arg),atbm_void *p_arg,int prio);
int atbm_stopThread(pAtbm_thread_t thread_id);
int atbm_ThreadStopEvent(pAtbm_thread_t thread_id);
//atbm_uint32 atbm_getCurThreadId(void);
#endif /* ATBM_OS_THREAD_H */

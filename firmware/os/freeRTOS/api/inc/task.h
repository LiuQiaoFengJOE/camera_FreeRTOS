#ifndef _TASK_H_
#define _TASK_H_
#include "api.h"

typedef struct _LIST_ENTRY_LEGACY
{
	struct _LIST_ENTRY_LEGACY *pNext;
} LIST_ENTRY_LEGACY, *PLIST_ENTRY_LEGACY;

typedef struct _LIST_HEADR_LEGACY
{
	PLIST_ENTRY_LEGACY pHead;
	PLIST_ENTRY_LEGACY pTail;
	UCHAR size;
} LIST_HEADER_LEGACY, *PLIST_HEADER_LEGACY;

#if 0
void initList(
	PLIST_HEADER pList);

void insertTailList(
	PLIST_HEADER pList,
	PLIST_ENTRY pEntry);

PLIST_ENTRY removeHeadList(
	PLIST_HEADER pList);

int getListSize(
	PLIST_HEADER pList);

PLIST_ENTRY delEntryList(
	PLIST_HEADER pList,
	PLIST_ENTRY pEntry);
#endif

struct work_struct_legacy
{
	LIST_HEADER_LEGACY *pTaskletList;
	LIST_ENTRY_LEGACY entry;
	void (*fun)(void *data);
	void *data;
	OS_EVENT *event;
};

struct workqueue_struct_legacy
{
	unsigned int flags; /* I: WQ_* flags */
	int task_id;
	wait_event_t thread_event;
	LIST_HEADER_LEGACY list;
	const char *name;
};

#ifndef offsetof
#define offsetof(type, member) ((long)&((type *)0)->member)
#endif

int thread_create(void (*task)(void *p_arg), void *p_arg, unsigned int prio, unsigned int *pbos, unsigned int stk_size, char *name);

int thread_exit(TaskHandle_t thread_id);
int thread_myself(void);

struct workqueue_struct_legacy *create_workqueue_legacy(const char *name, uint32_t stack_size, int prio);
void destroy_workqueue_legacy(struct workqueue_struct_legacy *workqueue);
int init_workqueue(void);
void init_work(struct work_struct_legacy *work, void *fun, void *data);
int schedule_work_api(struct workqueue_struct_legacy *queue, struct work_struct_legacy *work);
int schedule_timer_tasklet(struct work_struct_legacy *work);
void timer_tasklet_callback(struct work_struct_legacy *work);
void init_work_thread(void);

#define queue_work_legacy schedule_work_api
#define queue_work_FromISR schedule_work_FromISR

#endif

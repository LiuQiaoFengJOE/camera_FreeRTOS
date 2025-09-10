#ifndef  __MQUEUE_
#define __MQUEUE_

#include "basic_defs.h"

typedef struct _queue
{
	void** q;
	u32  front;
	u32 rear;
	u32 size;
}queue;



int queueInit(queue *q,u32 size);
int queueUninit(queue *q);
int queueInput(queue *q,void* d);
int queueInputFront(queue *q,void* d);
int queueOutput(queue *q,void** d);
int queuePeek(queue *q,void** d);
int queueRealloc(queue *q,u32 size);
u32 queueSize(queue *q);
u32 queueValidData(queue *q);
int queueOutputRear(queue *q,void** d);
int queueFull(queue *q);



#endif


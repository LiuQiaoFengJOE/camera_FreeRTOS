#include "irqTimer.h"

#define  X_CFG_IRQTIMER_MAX     5

static IRQ_TIMER irqTimerPool[X_CFG_IRQTIMER_MAX];
void irqTimerInit(void)
{
	int i;

	for(i=0;i<X_CFG_IRQTIMER_MAX;i++)
	{
		irqTimerPool[i].func = NULL;
	}
}
IRQ_TIMER *irqTimerCreate(u32 msec,void (*func)(void *arg),void* arg)
{
	int i;
	uint32_t flags;
	
	if(msec == 0 || func == NULL)
		return NULL;

	flags=arch_local_irq_save();
    for(i=0;i<X_CFG_IRQTIMER_MAX;i++)
	{
		if(irqTimerPool[i].func==NULL)
		{
			irqTimerPool[i].func = func;
			irqTimerPool[i].arg = arg;			
			irqTimerPool[i].reload = msec;
			irqTimerPool[i].counter = msec;  // count down
			arch_local_irq_restore(flags);
			return &irqTimerPool[i];
		}
	}
	arch_local_irq_restore(flags);
	return NULL;
}

void irqTimerModTime(IRQ_TIMER *xwork,u32 msec)
{
	uint32_t flags;
	flags=arch_local_irq_save();
    if(xwork!= NULL)
    {
		xwork->reload = msec;
		xwork->counter = msec;
    }
	arch_local_irq_restore(flags);
}
void irqTimerModFunc(IRQ_TIMER *xwork,u32 msec,void (*func)(void *arg),void* arg)
{
	uint32_t flags;
	flags=arch_local_irq_save();
    if(xwork!= NULL)
    {
		xwork->func = func;
		xwork->arg = arg;
		xwork->reload = msec;
		xwork->counter = msec;
    }
	arch_local_irq_restore(flags);
}

int irqTimerDestory(IRQ_TIMER *xwork)
{
	uint32_t flags;
	flags=arch_local_irq_save();
    if(xwork!= NULL)
		xwork->func = NULL;
	arch_local_irq_restore(flags);
	return 0;
}

void irqTimerService(void)
{
	int i;

	for(i=0;i<X_CFG_IRQTIMER_MAX;i++)
	{
        if(irqTimerPool[i].func!=NULL)
        {
			irqTimerPool[i].counter--;
			if(irqTimerPool[i].counter <= 0)
			{
				irqTimerPool[i].func(irqTimerPool[i].arg);
				irqTimerPool[i].counter = irqTimerPool[i].reload;
			}
        }
	}
}




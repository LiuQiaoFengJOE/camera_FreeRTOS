#include "spr_defs.h"
#include "AX329x.h"
#include "ISR_Support.h"
#include "FreeRTOS.h"
#include "task.h"
#include "linux/irqflags.h"
#include "board_config.h"

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152) ||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)

unsigned long vPortDisableInterruptsSave(void)
{
	// unsigned long flags;

	// CPSR_Save_0 = mfspr(SPR_SR);
	// mtspr (SPR_SR, CPSR_Save_0 & ~SPR_SR_IEE);

	// CPSR_Save_0 = mfspr(SPR_PICMR);
	// mtspr (SPR_PICMR, CPSR_Save_0 & __KEEP_INT__);

	// flags = mfspr(SPR_PICMR);
	// mtspr (SPR_PICMR, flags & __KEEP_INT__);

	unsigned long flags;
	local_irq_save(flags);
	return flags;
}

void vPortEnableInterruptsRestore(unsigned long flags)
{
	// mtspr (SPR_SR, CPSR_Save_0);

	// mtspr (SPR_PICMR, CPSR_Save_0);
	// mtspr (SPR_PICMR, flags);
	local_irq_restore(flags);
	return;
}

#endif

typedef int (*isr_func)(unsigned int, void *, void *);
isr_func __attribute__((weak)) irq_vector_table[33];
void *__attribute__((weak)) irq_dev_table[33];
unsigned int __attribute__((weak)) os_int_nesting;

portSTACK_TYPE *pxPortInitialiseStack(portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters)
{
	int i;
	unsigned portLONG uTaskSR = mfspr(SPR_SR); // mfspr(SPR_ESR_BASE);
	unsigned portLONG uTaskMR = mfspr(SPR_PICMR);
	uTaskSR |= (SPR_SR_SM | SPR_SR_IEE | SPR_SR_LIEE | SPR_SR_EPH); // Not User mode, All External interupt enable
	uTaskMR |= (1 << 0) | (1 << 14) | (1 << 16);					// en timer0
	printf("init stack %X\n", pxTopOfStack);
	/* 大部分值都是沒有意義的，直接用個循環去初始化成下標值就可以了 */
	for (i = ISR_STACK_FRAME_WORDS; i; --i)
		*(--pxTopOfStack) = (portSTACK_TYPE)i;

	pxTopOfStack[FRAME_POS_EPC] = (portSTACK_TYPE)pxCode;
	pxTopOfStack[FRAME_POS_ESR] = (portSTACK_TYPE)uTaskSR;
	pxTopOfStack[FRAME_POS_EMK] = (portSTACK_TYPE)uTaskMR;
	pxTopOfStack[FRAME_POS_CNT] = (portSTACK_TYPE)0;
	pxTopOfStack[FRAME_POS_CSR] = (portSTACK_TYPE)0;
	pxTopOfStack[FRAME_POS_REG(3)] = (portSTACK_TYPE)pvParameters;
	return pxTopOfStack;
}

void vTaskDelete(void *);
/*-----------------------------------------------------------*/
void vPortEndScheduler(void)
{
	vTaskDelete(NULL);
}

static const char except_name[][8] = {
	"NONE", "RESET", "BUSERR", "DPF",
	"IPF", "LPINT", "ALIGN", "ILLEGAL",
	"HPINT", "DT_MISS", "IT_MISS", "RANGE",
	"SYSCALL", "FLOAT", "TRAP","BP"};

static const char stk_regname[][4] = INIT_REGNAMES;
void do_EXCEPTION(unsigned long eno, portSTACK_TYPE *stk)
{
	unsigned long i,j;

	eno >>= 4; // 異常號是其地址，每個異常入口處有16字節
	eno &= 15;
	printf("exception %s\r\n", except_name[eno]);
	printf("   EPCR = %lX\r\n", SPR_EPCR);
	//printf("   EPCR1 = %lX\r\n", SPR_EPCR1);
	//printf("   EPCR2 = %lX\r\n", SPR_EPCR2);
	printf("   SP = %lX\r\n", stk);
	for (i = 0; i < ISR_STACK_FRAME_REGS; ++i)
	{
		printf(" %4s = %lX\r\n", stk_regname[i], stk[i]);
	}
	printf("stack:\n");
	for (i = ISR_STACK_FRAME_WORDS; i <= ISR_STACK_FRAME_WORDS+50; ++i)
	{
		printf("[%lX] = %lX\n",&stk[i], stk[i]);
	}
	while (1)
		;
	// todo: dump task stack
}


//==================任务创建宏定义，便于修改==================
static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];		 // 空闲任务任务堆栈
static StaticTask_t IdleTaskTCB;								 // 空闲任务控制块
static StackType_t TimerTaskStack[configTIMER_TASK_STACK_DEPTH]; // 定时器服务任务堆栈
static StaticTask_t TimerTaskTCB;								 // 定时器服务任务控制块

//==================任务创建函数==================

// 获取空闲任务地任务堆栈和任务控制块内存
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
								   StackType_t **ppxIdleTaskStackBuffer,
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;
	*ppxIdleTaskStackBuffer = IdleTaskStack;
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

// 获取定时器服务任务的任务堆栈和任务控制块内存
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
									StackType_t **ppxTimerTaskStackBuffer,
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer = &TimerTaskTCB;
	*ppxTimerTaskStackBuffer = TimerTaskStack;
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*-----------------------------------------------------------*/
/* copy from freeRTOS/tasks.c */
typedef struct
{
	volatile StackType_t *pxTopOfStack; /*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

#if (portUSING_MPU_WRAPPERS == 1)
	xMPU_SETTINGS xMPUSettings; /*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
#endif

	ListItem_t xStateListItem;																									   /*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
	ListItem_t xEventListItem;																									   /*< Used to reference a task from an event list. */
	UBaseType_t uxPriority;																										   /*< The priority of the task.  0 is the lowest priority. */
	StackType_t *pxStack;																										   /*< Points to the start of the stack. */
	char pcTaskName[configMAX_TASK_NAME_LEN]; /*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

#if (portSTACK_GROWTH > 0)
	StackType_t *pxEndOfStack; /*< Points to the end of the stack on architectures where the stack grows up from low memory. */
#endif

#if (portCRITICAL_NESTING_IN_TCB == 1)
	UBaseType_t uxCriticalNesting; /*< Holds the critical section nesting depth for ports that do not maintain their own count in the port layer. */
#endif

#if (configUSE_TRACE_FACILITY == 1)
	UBaseType_t uxTCBNumber;  /*< Stores a number that increments each time a TCB is created.  It allows debuggers to determine when a task has been deleted and then recreated. */
	UBaseType_t uxTaskNumber; /*< Stores a number specifically for use by third party trace code. */
#endif

#if (configUSE_MUTEXES == 1)
	UBaseType_t uxBasePriority; /*< The priority last assigned to the task - used by the priority inheritance mechanism. */
	UBaseType_t uxMutexesHeld;
#endif

#if (configUSE_APPLICATION_TASK_TAG == 1)
	TaskHookFunction_t pxTaskTag;
#endif

#if (configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0)
	void *pvThreadLocalStoragePointers[configNUM_THREAD_LOCAL_STORAGE_POINTERS];
#endif

#if (configGENERATE_RUN_TIME_STATS == 1)
	uint32_t ulRunTimeCounter; /*< Stores the amount of time the task has spent in the Running state. */
#endif

#if (configUSE_NEWLIB_REENTRANT == 1)
	/* Allocate a Newlib reent structure that is specific to this task.
	Note Newlib support has been included by popular demand, but is not
	used by the FreeRTOS maintainers themselves.  FreeRTOS is not
	responsible for resulting newlib operation.  User must be familiar with
	newlib and must provide system-wide implementations of the necessary
	stubs. Be warned that (at the time of writing) the current newlib design
	implements a system-wide malloc() that must be provided with locks. */
	struct _reent xNewLib_reent;
#endif

#if (configUSE_TASK_NOTIFICATIONS == 1)
	volatile uint32_t ulNotifiedValue;
	volatile uint8_t ucNotifyState;
#endif

/* See the comments above the definition of
tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE. */
#if (tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0)
	uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the task is a statically allocated to ensure no attempt is made to free the memory. */
#endif

#if (INCLUDE_xTaskAbortDelay == 1)
	uint8_t ucDelayAborted;
#endif

} freeRTOS_TCB;
/*-----------------------------------------------------------*/

void vApplicationTaskCreateHook(freeRTOS_TCB *tcb)
{
	portSTACK_TYPE *pxTopOfStack = tcb->pxTopOfStack;
	printf("create task %2d '%s'\r\n", tcb->uxPriority, tcb->pcTaskName);
#if 0
	printf(" entry PC  %X\r\n"
		   " stack top %X\r\n"
		   " stack cur %X\r\n"
#if (portSTACK_GROWTH > 0)
		   " stack end %X\r\n"
#endif
		   ,
		   *pxTopOfStack, tcb->pxStack, pxTopOfStack
#if (portSTACK_GROWTH > 0)
		   ,
		   tcb->pxEndOfStack
#endif
	);
#endif
}

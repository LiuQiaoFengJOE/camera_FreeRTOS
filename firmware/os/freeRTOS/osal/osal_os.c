#include "osal/osal_wait.h"
#include "osal/osal_workqueue.h"

// extern struct workqueue_struct *system_wq;
// extern void tasklet_startup(void);

extern void vTaskStartScheduler(void);
extern void vTaskSwitchContext(void);
// extern struct workqueue_struct *alloc_workqueue(const char *fmt, unsigned int flags, int max_active,...);

void osal_os_start()
{
  irq_register(32, 0, vTaskSwitchContext, NULL);

  //  tasklet_startup();
  //  system_wq = alloc_workqueue( "sys_wq", 0, 0 );

  vTaskStartScheduler();
}

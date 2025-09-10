#ifndef ATBM_OS_SPINLOCK_H
#define ATBM_OS_SPINLOCK_H
#include "atbm_type.h"
#include "api.h"
#include "os/freeRTOS/inc/FreeRTOS.h"
#include "os/freeRTOS/inc/queue.h"
//#include "linux_spinlock.h"

typedef   QueueHandle_t atbm_spinlock_t;
//typedef   atbm_uint8 atbm_spinlock_t;

/*spin lock*/
//#define atbm_spin_lock_init(x)
void atbm_spin_lock_init(atbm_spinlock_t *plock);

//#define atbm_spin_lock(x) 
void atbm_spin_lock(atbm_spinlock_t *plock);

//#define atbm_spin_unlock(x) 
void atbm_spin_unlock(atbm_spinlock_t *plock);

#define atbm_spin_lock_bh(x) 
//void atbm_spin_lock_bh(atbm_spinlock_t *plock);

#define atbm_spin_unlock_bh(x)
//void atbm_spin_unlock_bh(atbm_spinlock_t *plock);


atbm_void atbm_spin_lock_irqsave(atbm_spinlock_t       *x, unsigned long *f);
atbm_void atbm_spin_unlock_irqrestore(atbm_spinlock_t *x, unsigned long f);

//#define atbm_disable_irq()                  drv_irq_store()
//#define atbm_enable_irq(f)                  drv_irq_restore(f)
#endif /* ATBM_OS_SPINLOCK_H */


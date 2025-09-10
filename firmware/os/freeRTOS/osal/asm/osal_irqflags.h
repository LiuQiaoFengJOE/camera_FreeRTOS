/**
 * @File osal_delay.h
 * @brief delay and sleep
 */

#ifndef __ASM_DELAY_H__
#define __ASM_DELAY_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern unsigned long arch_local_irq_save(void);
    extern arch_local_irq_restore(unsigned long flags);

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_IRQFLAGS_H__ */

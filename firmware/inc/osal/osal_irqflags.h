/**
 * @File osal_irqflags.h
 * @brief Architecture defined irq
 */

#ifndef __OSAL_IRQFLAGS_H__
#define __OSAL_IRQFLAGS_H__

#include "asm/osal_irqflags.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define local_irq_save(flags) ({ flags = arch_local_irq_save(); })
#define local_irq_restore(flags) ({ arch_local_irq_restore(flags); })

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_IRQFLAGS_H__ */

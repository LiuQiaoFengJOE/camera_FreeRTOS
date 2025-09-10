/**
 * @File irqflags.h
 * @brief Architecture defined irq
 */

#ifndef __ASM_IRQFLAGS_H__
#define __ASM_IRQFLAGS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ARCH_IRQ_DISABLED 0x00
#define ARCH_IRQ_ENABLED (SPR_SR_LIEE)

    unsigned long arch_local_save_flags(void);
    void arch_local_irq_restore(unsigned long flags);
    unsigned long arch_local_irq_save();
    void arch_local_irq_enable();
    void arch_local_irq_disable();
    int arch_irqs_disabled_flags(unsigned long flags);
    int arch_irqs_disabled();

#ifdef __cplusplus
}
#endif

#endif /* __ASM_IRQFLAGS_H__ */

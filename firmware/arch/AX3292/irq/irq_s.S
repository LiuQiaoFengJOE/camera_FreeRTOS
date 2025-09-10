#include <or1k-asm.h>

#include "spr_defs.h"
#include "AX329x.h"

        .macro          defunc name
        .global         \name
        .type           \name, "function"
\name:
        .endm

        .macro          endfunc name
        .endm


        .text

// TODO: should only handle the interrupt-enable-bit

defunc  arch_local_save_flags
OR1K_DELAYED(
OR1K_INST(
        l.mfspr         r11, r0, SPR_SR
),
OR1K_INST(
        l.jr            r9
)
)
endfunc arch_local_save_flags

defunc  arch_local_irq_save
defunc  arch_local_irq_disable
        l.mfspr         r11, r0, SPR_SR
        l.movhi         r3, hi(~SPR_SR_LIEE)
        l.ori           r3, r3, lo(~SPR_SR_LIEE)
        l.and           r3, r11, r3
defunc  arch_local_irq_restore
OR1K_DELAYED(
OR1K_INST(
        l.mtspr         r0, r3, SPR_SR
),
OR1K_INST(
        l.jr            r9
)
)
endfunc arch_local_irq_restore
endfunc arch_local_irq_disable
endfunc arch_local_irq_save


defunc  arch_local_irq_enable
        l.mfspr         r11, r0, SPR_SR
        l.ori           r3, r11, SPR_SR_LIEE
OR1K_DELAYED(
OR1K_INST(
        l.mtspr         r0, r3, SPR_SR
),
OR1K_INST(
        l.jr            r9
)
)
endfunc arch_local_irq_enable


defunc  arch_irqs_disabled_flags
        l.andi          r11, r3, SPR_SR_LIEE
OR1K_DELAYED(
OR1K_INST(
        l.xori          r3, r3, SPR_SR_LIEE
),
OR1K_INST(
        l.jr            r9
)
)
endfunc arch_irqs_disabled_flags





#if 0
defunc  __raw_local_irq_save
        l.mfspr         r11, r0, SPR_SR
        l.movhi         r3, hi(~SPR_SR_LIEE)
        l.ori           r3, r3, lo(~SPR_SR_LIEE)
        l.and           r3, r11, r3
defunc  raw_local_irq_restore
OR1K_DELAYED(
OR1K_INST(
        l.mtspr         r0, r3, SPR_SR
),
OR1K_INST(
        l.jr            r9
)
)
endfunc raw_local_irq_restore
endfunc __raw_local_irq_save

defunc  raw_local_irq_enable
        l.mfspr         r11, r0, SPR_SR
        l.ori           r3, r11, SPR_SR_LIEE
OR1K_DELAYED(
OR1K_INST(
        l.mtspr         r0, r3, SPR_SR
),
OR1K_INST(
        l.jr            r9
)
)
endfunc raw_local_irq_enable
#endif


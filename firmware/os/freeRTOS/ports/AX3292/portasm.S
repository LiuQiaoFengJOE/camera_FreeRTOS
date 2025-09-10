#include <or1k-asm.h>

#include "spr_defs.h"
#include "AX329x.h"

#include "ISR_Support.h"

        .macro          defunc name
        .global         \name
        .type           \name, "function"
\name:
        .endm

        .macro          endfunc name
        .endm

        .macro          weakfunc name
        .weak           \name
        .global         \name
        .type           \name, "function"
\name:
        .endm


        .section        .vector.entry, "ax"

////////////////////////////////////////////////////////////////////////////////
defunc  OS_Int_Switch
        portSAVE_CONTEXT

        l.movhi         r14, hi(os_int_nesting)
        l.ori           r14, r14, lo(os_int_nesting)

        l.movhi         r3, hi(irq_dev_table)
        l.ori           r3, r3, lo(irq_dev_table)

        l.lwz           r16, 0(r14)

        l.movhi         r11, hi(irq_vector_table)
        l.ori           r11, r11, lo(irq_vector_table)

        l.addi          r16, r16, 1

        l.add           r3, r9, r3
        l.add           r11, r9, r11

        l.sw            0(r14), r16

        l.lwz           r4, 0(r3)
        l.lwz           r11, 0(r11)
        l.srli          r3, r9, 2

        l.addi          r16, r16, -1
OR1K_DELAYED(
OR1K_INST(
        l.ori           r5, r1, 0
),
OR1K_INST(
        l.jalr          r11
)
)

        l.sw            0(r14), r16

defunc  vPortRestoreContext
#if 0
    l.movhi     r3, hi(pxCurrentTCB)
    l.ori       r3, r3, lo(pxCurrentTCB)
    l.lwz       r3, 0x00(r3)
    l.lwz       r3, 0x00(r3)
    l.lwz       r3, 0x00(r3)
    l.ori       r4, r0, 1
    l.jal       Uart_Put_Hex
    l.movhi     r3, hi(pxCurrentTCB)
    l.ori       r3, r3, lo(pxCurrentTCB)
    l.lwz       r3, 0x00(r3)
    l.ori       r4, r0, 1
    l.jal       Uart_Put_Hex
#endif
        portRESTORE_CONTEXT
endfunc vPortRestoreContext

endfunc OS_Int_Switch


////////////////////////////////////////////////////////////////////////////////
defunc  exception_entry
        portSAVE_CONTEXT
        l.ori           r3, r9, 0x00
OR1K_DELAYED(
OR1K_INST(
        l.ori           r4, r1, 0x00
),
OR1K_INST(
        l.jal           do_EXCEPTION
)
)
        portRESTORE_CONTEXT
        l.rfe
        l.nop
endfunc exception_entry


////////////////////////////////////////////////////////////////////////////////
// define some weak function here
weakfunc    vApplicationIdleHook
weakfunc    vApplicationTickHook
weakfunc    vApplicationStackOverflowHook
weakfunc    vApplicationTaskSwitchHook
// all weakfuncs can use the same empty function
OR1K_DELAYED_NOP(
OR1K_INST(
        l.jr            r9
)
)


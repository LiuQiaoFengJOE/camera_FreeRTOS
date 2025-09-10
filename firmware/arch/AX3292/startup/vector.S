/**@file             vector.S
 * @author           Kaifan He
 * @date             2016/06/15
 * @brief            AX3278 exception vector
 * @details
 *
 * The exception vector can only locate at internal sram address at 0x40000.
 * So the startup program maybe need to copy the vectors to sram.
 *
 * Vector item size are 16-bytes. The first 4 vectors are almost no used,
 * so they can be used as the entry addresses. Each entry address is 4-bytes long.
 *
 * The interrupt entry address is OS_Trap_Int_Comm, it sets IRQn to r9.
 * Yield uses Syscall, its treated as the 32th interrupt.
 */

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


        .section        .text.reset, "ax"
////////////////////////////////////////////////////////////////////////////////
defunc  except_vector
        /* The first two exception are no use */
        EXCEPTION       NONE
//        .align          16
//        l.j             _reset
        // EXCEPTION       RESET
        .align          16
defunc except_reset
OR1K_DELAYED_NOP(
OR1K_INST(
        l.j             _reset
)
)
        // needed BUSERR but not hpint/itlb/dtlb
        EXCEPTION       BUSERR
        EXCEPTION       DPF
        EXCEPTION       IPF
        EXCEPT_INT      OS_Trap_Int_Comm
        EXCEPTION       ALIGN
        EXCEPTION       ILLEGAL
        EXCEPTION       HPINT
        EXCEPTION       DTLBMISS
        EXCEPTION       ITLBMISS
        EXCEPTION       RANGE
        EXCEPT_INT      OS_Int_Switch
        EXCEPTION       FLOAT
        EXCEPTION       TRAP
        EXCEPTION       BREAKPOINT
endfunc except_vector

        .section        .reset, "ax"
////////////////////////////////////////////////////////////////////////////////
defunc  OS_Trap_Int_Comm
// todo: clear pending of SPR_PICVR
// maybe it can be clear before read
        l.mfspr         r9, r0, SPR_PICVR
        l.andi          r9, r9, 0x1F
OR1K_DELAYED(
OR1K_INST(
        l.slli          r9, r9, 2
),
OR1K_INST(
        l.j             OS_Int_Switch
)
)
endfunc OS_Trap_Int_Comm

// todo
////////////////////////////////////////////////////////////////////////////////
_reset:
        // clear WDT


        // stack
        l.movhi         r1, hi(_stack_base)
        l.ori           r1, r1, lo(_stack_base)

        // copy exception vector if need

        // bss
        // loop 16B
#define ___CLEAR_16B___
        l.movhi         r3, hi(_bss_start)
        l.ori           r3, r3, lo(_bss_start)
        l.movhi         r4, hi(_bss_end)
        l.ori           r4, r4, lo(_bss_end)
0:
#ifdef ___CLEAR_16B___
        l.addi          r3, r3, 16
        l.sw            -16(r3), r0
        l.sw            -12(r3), r0
        l.sw            -8(r3), r0
#else
        l.addi          r3, r3, 4
#endif
        l.sfgeu         r3, r4
OR1K_DELAYED(
OR1K_INST(
        l.sw            -4(r3), r0
),
OR1K_INST(
        l.bnf           0b
)
)


OR1K_DELAYED_NOP(
OR1K_INST(
        l.jal           _init
)
)

OR1K_DELAYED_NOP(
OR1K_INST(
        l.j             .
)
)


//l.j xTaskCreate
//l.j vTaskSwitchContext

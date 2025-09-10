/**@file boost.S
 * @brief boost入口函數支持庫
 * @details 提供boost入口函數實現
 */

#include <or1k-asm.h>

        .macro      PUB_FUNC name
        .global     \name
\name:
        .endm

        .macro      PUB_LABEL name
        .global     \name
\name:
        .endm

        .macro      PUB_VAL name, val
        .global     \name
        .equ        \name, \val
        .endm


        .macro      defunc,num
        .section    .text.boostentry.\num
PUB_FUNC            boostentry\num
OR1K_DELAYED_NOP (
OR1K_INST(
        l.jal       boost\num
)
)
        l.movhi     r4, hi(find_load_file)
        l.ori       r4, r4, lo(find_load_file)
        l.lwz       r3, 0(r4)
OR1K_DELAYED (
OR1K_INST(
        l.ori       r4, r11, 0
),
OR1K_INST(
        l.jr        r3
)
)
        .endm

        .irp        num,0,1,2,3,4,5,6,7,8,9
        defunc      \num
        .endr

        .irp        i,1,2,3,4,5,6,7,8,9
        .irp        j,0,1,2,3,4,5,6,7,8,9
        defunc      \i\j
        .endr
        .endr

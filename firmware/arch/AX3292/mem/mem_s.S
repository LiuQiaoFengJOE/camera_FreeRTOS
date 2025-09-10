/**@file             mem_s.S
 * @author           Kaifan He
 * @date             2017/12/06
 * @brief            AX3268 memory Operation
 * @details
 * 
 */

#include <or1k-asm.h>

        .macro          defunc name
        .global         \name
        .type           \name, "function"
\name:
        .endm

        .macro          endfunc name
        .endm


        .section        .text.memset, "ax"

////////////////////////////////////////////////////////////////////////////////
// void memset (dst, val, bytes)
/* or1k newlib的memset由於優化選項不高，內存操作過多，
 * 導致個別FT測試failure的芯片在memset發生異常。
 *
 * 這段彙編程序盡量避免引發異常的指令組合，用於對比測試。
 * 這段彙編程序的流程，仍然是參照newlib的C程序流程來做的。
 * 把下面defunc memset的memset改為其他名稱，可令連接器使用newlib庫的memset
 */
defunc  memset
        // 不夠4字節，按字節置值
        l.add           r11, r3, r0
        l.sfltui        r5, 4
OR1K_DELAYED(
OR1K_INST(
        l.andi          r4, r4, 0xFF
),
OR1K_INST(
        l.bf            9f
)
)
        /* 一定超過3字節了 */
        l.slli          r6, r4, 8
        l.andi          r7, r3, 1
        l.or            r4, r6, r4
        l.add           r7, r3, r7
        l.sb            0(r3), r4
        l.slli          r6, r4, 16
        l.addi          r7, r7, 2
        l.addi          r8, r0, -4
        l.sh            -2(r7), r4
        l.and           r7, r8, r7
        l.or            r4, r6, r4
        /* 經過上面處理，r7已經移動到了4字節對齊的地方
         * 原本對齊：已寫入的3字節白寫了
         * 原本2對齊：已寫入2字節，並且r7 - r3 = 2
         * 原本1對齊：
         *   4 + 1  ：已寫入3字節，並且r7 - r3 = 3
         *   4 + 3  ：已寫入1字節，並且r7 - r3 = 1
         */
        l.sub           r6, r7, r3
        l.addi          r3, r7, -16 /* 下面的循環多加了16 */
        l.sub           r5, r5, r6
OR1K_DELAYED(
OR1K_INST(
        l.sfltui        r5, 16
),
OR1K_INST(
        l.j             1f
)
)
        /* 下面按16字節置值 */
0:
        l.addi          r5, r5, -16
        l.sw            0(r3), r4
        l.sw            4(r3), r4
        l.sfltui        r5, 16
        l.sw            8(r3), r4
        l.sw            12(r3), r4
1:
OR1K_DELAYED(
OR1K_INST(
        l.addi          r3, r3, 16
),
OR1K_INST(
        l.bnf           0b
)
)

        /* 再按4字節置值 */
OR1K_DELAYED(
OR1K_INST(
        l.sfltui        r5, 4
),
OR1K_INST(
        l.j             3f
)
)
2:
        l.addi          r5, r5, -4
        l.sw            -4(r3), r4
        l.sfltui        r5, 4
3:
OR1K_DELAYED(
OR1K_INST(
        l.addi          r3, r3, 4
),
OR1K_INST(
        l.bnf           2b
)
)

        /* 最後剩下的不足4字節 */
OR1K_DELAYED(
OR1K_INST(
        l.addi          r3, r3, -4  /* 上面的循環多加了4 */
),
OR1K_INST(
        l.j             9f
)
)
        /* 包括一開始就不足4字節，一並在此處理 */
8:
        l.sb            -1(r3), r4
9:
        l.sfeqi         r5, 0
        l.addi          r5, r5, -1
OR1K_DELAYED(
OR1K_INST(
        l.addi          r3, r3, 1
),
OR1K_INST(
        l.bnf           8b
)
)

OR1K_DELAYED_NOP(
OR1K_INST(
        l.jr            r9
)
)
endfunc memset

#include <or1k-asm.h>

#define R9INSTK      -4(r1)
#define R6INSTK      -8(r1)
#define OUTLENINSTK -12(r1)

        .section        .text.startup, "ax"

        .global         _start
_start:

        l.sw            R9INSTK, r9
OR1K_DELAYED(
OR1K_INST(
        l.sw            R6INSTK, r6
),
OR1K_INST(
        l.jal           0f
)
)

.Lentry:  .long         0
.Lin:     .long         0
.Linlen:  .long         0
.Lout:    .long         0
.Loutlen: .long         0

          .equ          OFFSET_OUTLEN_ENTRY, (.Loutlen - .Lentry)
          .equ          OFFSET_IN_OUTLEN,    (.Lin     - .Loutlen)
          .equ          OFFSET_INLEN_OUTLEN, (.Linlen  - .Loutlen)
          .equ          OFFSET_OUT_OUTLEN,   (.Lout    - .Loutlen)

0:
        /* r9現在指向了entry */
        l.addi          r6, r9, OFFSET_OUTLEN_ENTRY
        l.sw            OUTLENINSTK, r6
        l.addi          r1, r1, -16

        l.lwz           r3, OFFSET_IN_OUTLEN(r6)
        l.lwz           r4, OFFSET_INLEN_OUTLEN(r6)
        l.lwz           r5, OFFSET_OUT_OUTLEN(r6)
OR1K_DELAYED(
OR1K_INST(
        l.ori           r7, r0, 0
),
OR1K_INST(
        l.jal           lzo1x_decompress
)
)

//        l.lwz           r6, 16+R6INSTK
//OR1K_DELAYED(
//OR1K_INST(
//        l.ori           r3, r11, 0
//),
//OR1K_INST(
//        l.jalr          r6
//)
//)

        l.addi          r1, r1, 16
        l.sfeqi         r11, 0
        l.lwz           r4, OUTLENINSTK

OR1K_DELAYED(
OR1K_INST(
        l.lwz           r9, R9INSTK
),
OR1K_INST(
        l.bnf           0f  /* 出錯，用棧上的r9 */
)
)

        /* 解壓沒錯，則用目標地址 */
        l.lwz           r9, -OFFSET_OUTLEN_ENTRY(r4)

0:
OR1K_DELAYED(
OR1K_INST(
        l.lwz           r6, R6INSTK
),
OR1K_INST(
        l.jr            r9
)
)

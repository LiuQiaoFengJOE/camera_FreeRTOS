#include <or1k-asm.h>

#define ENABLE_CACHE 1

#define R9INSTK      -4(r1)
#define R6INSTK      -8(r1)
#define OUTLENINSTK -12(r1)
#define R11INSTK    -16(r1)

        .section        .text.startup, "ax"

        .global         _start
_start:

#if 1

        l.sw            R9INSTK, r9
OR1K_DELAYED(
OR1K_INST(
        l.sw            R6INSTK, r6
),
OR1K_INST(
        l.jal           0f
)
)

.Linlen:  .long         0
// boost
.Lrambase:.long         0
.Lentry:  .long         0
.Lfile:   .skip         12

.Lziped_str:
          .asciz        "ent"
          .asciz        "src"
          .asciz        "len"
          .asciz        "dst"
          .asciz        "len"
.global ziped_str
.set ziped_str, .Lziped_str - 12

0:
        /* r4作為參數指向了_start
         * r9指向了.Linlen
         * 但其實兩者祇相距12字節
         */
        l.lwz           r3, 12(r4)  // inlen      r3--->  0x7f0a0
        l.sw            OUTLENINSTK, r4 // todo 改一下宏名    
        l.movhi         r5, hi(_start)
        l.ori           r5, r5, lo(_start)
        l.sub           r3, r4, r3  // ziped
        l.sub           r4, r4, r5
OR1K_DELAYED(
OR1K_INST(
        l.addi          r1, r1, -16
),
OR1K_INST(
        l.jal           do_unzip
)
)

        l.lwz           r9, 16 + R9INSTK
OR1K_DELAYED(
OR1K_INST(
        l.addi          r1, r1, 16
),
OR1K_INST(
        l.jr            r9
)
)


#else

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
OR1K_DELAYED(
OR1K_INST(
        l.addi          r1, r1, -16
),
OR1K_INST(
        l.jal           wdt_disable
)
)

#if ENABLE_CACHE
OR1K_DELAYED_NOP(
OR1K_INST(
        l.jal           icache_init
)
)
OR1K_DELAYED_NOP(
OR1K_INST(
        l.jal           dcache_init
)
)
#endif
        l.lwz           r6, 16+OUTLENINSTK

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

        l.sw            16+R11INSTK, r11
#if ENABLE_CACHE
        // 因為還要調用其他函數，故將返回值保護一下
OR1K_DELAYED(
OR1K_INST(
        l.sw            16+R11INSTK, r11
),
OR1K_INST(
        l.jal           dcache_writeback_all
)
)
OR1K_DELAYED_NOP(
OR1K_INST(
        l.jal           dcache_disable
)
)
OR1K_DELAYED_NOP(
OR1K_INST(
        l.jal           icache_disable
)
)
        l.lwz           r11, 16+R11INSTK
#endif

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

#endif

#if 0
.global dcache_disable
dcache_disable:
        l.addi          r4, r0, -9
        l.mfspr         r3, r0, 17
        l.and           r3, r3, r4
        l.mtspr         r0, r3, 17

        l.addi          r3, r0, 0
0:
        l.sfltui        r3, 511*4
        l.mtspr         r0, r3, 6148
        l.bf            0b
        l.addi          r3, r3, 4

        l.jr            r9
        l.nop
#endif


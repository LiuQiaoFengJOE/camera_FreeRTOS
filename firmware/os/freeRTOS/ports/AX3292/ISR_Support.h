#ifndef ISR_SUPPORT__H_
#define ISR_SUPPORT__H_

/* Define TCB Stack Frame */
#define ISR_STACK_FRAME_WORDS (32 + 32 + 1)
#define FRAME_POS_REG(r) r
#define FRAME_POS_EPC 0
#define FRAME_POS_ESR 1
#define FRAME_POS_EMK 33
#define FRAME_POS_CNT 34
#define FRAME_POS_CSR 35
#define ISR_STACK_FRAME_REGS 36

#define INIT_REGNAMES {\
  "PC",   "SR",   "R2",   "R3",\
  "R4",   "R5",   "R6",   "R7",\
  "R8",   "R9",   "R10",  "R11",\
  "R12",  "R13",  "R14",  "R15",\
  "R16",  "R17",  "R18",  "R19",\
  "R20",  "R21",  "R22",  "R23",\
  "R24",  "R25",  "R26",  "R27",\
  "R28",  "R29",  "R30",  "R31",\
  "DMY",  "EMK",  "CNT",  "CSR"\
}

#ifdef __ASSEMBLER__
.macro      l.push  n
    l.sw        4*FRAME_POS_REG(\n)(r1), r\n
    .endm

    .macro      l.pop   n
    l.lwz       r\n, 4*FRAME_POS_REG(\n)(r1)
    .endm

    .macro      l.push2  r, f
    l.sw        4*\r (r1), \f
    .endm

    .macro      l.pop2   r, t
    l.lwz       \t, 4*\r (r1)
    .endm

    .macro      preSAVE_CONTEXT entry
    /* make rooms in stack  */
    l.addi      r1, r1, - ISR_STACK_FRAME_WORDS*4
    /* early save r3-r5
     * these are clobber registers for pushing spr
     */
    l.push      3
    l.j         \entry
    l.push      4
    .endm
  
  /* portSAVE_CONTEXT */
    .macro      portSAVE_CONTEXT
    /* make rooms in stack  */
    l.addi      r1, r1, - ISR_STACK_FRAME_WORDS*4
    /* early save r3-r5
     * these are clobber registers for pushing spr
     */
    l.push      3
    l.push      4
    l.push      5
    /* save SPR_ESR_BASE, SPR_EPCR_BASE  */
    l.mfspr     r3, r0, SPR_ESR//SPR_ESR_BASE	//sr
    l.mfspr     r4, r0, SPR_EPCR//SPR_EPCR_BASE	//pc
    l.mfspr		  r5, r0, SPR_PICMR
    
    l.push2     FRAME_POS_ESR, r3
    l.push2     FRAME_POS_EPC, r4
    l.push2     FRAME_POS_EMK, r5

    l.mfspr     r4, r0, SPR_SR
    l.ori		    r4, r4, 0x04
    l.mtspr     r0, r4, SPR_SR    

//    l.movhi     r4, hi(uxCriticalNesting)
//    l.movhi     r5, hi(critical_cpsr_save)
//    l.ori       r4, r4, lo(uxCriticalNesting)
//    l.ori       r5, r5, lo(critical_cpsr_save)
//    l.lwz       r3, 0(r4)
//    l.lwz       r4, 0(r5)
//    l.push2     FRAME_POS_CNT, r3
//    l.push2     FRAME_POS_CSR, r4
    /* save context */
//    l.push      9
    l.push      2
    l.push      6
    l.push      7
    l.push      8
    l.push      10
    l.push      11
    l.push      12
    l.push      13
    l.push      14
    l.push      15
    l.push      16
    l.push      17
    l.push      18
    l.push      19
    l.push      20
    l.push      21
    l.push      22
    l.push      23
    l.push      24
    l.push      25
    l.push      26
    l.push      27
    l.push      28
    l.push      29
    l.push      30
    l.push      31
    /* save the top of stack in TCB */
    l.movhi     r3, hi(pxCurrentTCB)
    l.ori       r3, r3, lo(pxCurrentTCB) 
    l.lwz       r3, 0x00(r3)
    l.sw        0x00(r3), r1
    .endm
  
  /* portRESTORE_CONTEXT */
    .macro      portRESTORE_CONTEXT
    
    l.mfspr     r4, r0, SPR_SR
    l.andi		  r4, r4, 0xFFFFFFFB
    l.mtspr     r0, r4, SPR_SR
  
    l.movhi     r3, hi(pxCurrentTCB)
    l.ori       r3, r3, lo(pxCurrentTCB) 
    l.lwz       r3, 0x00(r3)
    l.lwz       r1, 0x00(r3)
    /* restore context */
    l.pop       9
    l.pop       2
    l.pop       6
    l.pop       7
    l.pop       8
    l.pop       10
    l.pop       11
    l.pop       12
    l.pop       13
    l.pop       14
    l.pop       15
    l.pop       16
    l.pop       17
    l.pop       18
    l.pop       19
    l.pop       20
    l.pop       21
    l.pop       22
    l.pop       23
    l.pop       24
    l.pop       25
    l.pop       26
    l.pop       27
    l.pop       28
    l.pop       29
    l.pop       30
    l.pop       31
    /* restore SPR_ESR_BASE, SPR_EPCR_BASE  */
//    l.pop2      FRAME_POS_CNT, r3
//    l.movhi     r4, hi(uxCriticalNesting)
//    l.movhi     r5, hi(critical_cpsr_save)
//    l.ori       r4, r4, lo(uxCriticalNesting)
//    l.ori       r5, r5, lo(critical_cpsr_save)
//    l.sw        0(r4), r3
//    l.pop2      FRAME_POS_CSR, r4
//    l.sw        0(r5), r4

    l.pop2      FRAME_POS_ESR, r3
    l.pop2      FRAME_POS_EPC, r4
    l.pop2      FRAME_POS_EMK, r5
    l.mtspr     r0, r3, SPR_ESR//SPR_ESR_BASE        del wq
    l.mtspr     r0, r4, SPR_EPCR//SPR_EPCR_BASE
    //l.mtspr     r0, r5, SPR_PICMR

    /* restore clobber register  */
    l.pop       3
    l.pop       4
    l.pop       5
    l.addi      r1, r1, ISR_STACK_FRAME_WORDS*4
    l.rfe
    l.nop
    .endm

/* Pre-push r9 and call, so the new r9 can tell us which exception */
    .macro      EXCEPTION n
    .align      16
    l.sw        4*(FRAME_POS_REG(9) - ISR_STACK_FRAME_WORDS) (r1), r9
    l.jal       exception_entry
    l.nop
    l.nop
    .endm

/* Pre-push r9, and then the r9 can be used to get IRQn */
/* r9 pre-set 4*32, means that it is an Syscall.
 * So the Syscall can direct jump to irq handler.
 * If the true interrupt ocurrs, true IRQn should be set to r9 before jump to irq handler.
 */
    .macro      EXCEPT_INT int_entry
    .align      16
    l.sw        4*(FRAME_POS_REG(9) - ISR_STACK_FRAME_WORDS) (r1), r9
    l.addi      r9, r0, 4*32
    l.j         \int_entry
    l.nop
    .endm
#endif /* __ASSEMBLER__ */

#endif /* ISR_SUPPORT__H_ */

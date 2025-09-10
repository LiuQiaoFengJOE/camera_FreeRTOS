#ifndef __SPR__H
#define __SPR__H
/* spr_defs.h -- Defines OR1K architecture specific special-purpose registers
   Copyright (C) 1999 Damjan Lampret, lampret@opencores.org

This file is part of OpenRISC 1000 Architectural Simulator.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* This file is also used by microkernel test bench. Among
others it is also used in assembly file(s). */

/* Definition of special-purpose registers (SPRs) */

/*
 * Bit definitions for the Version Register
 *
 */
#define SPR_VR_VER 0xffff0000 /* Processor version */
#define SPR_VR_REV 0x0000003f /* Processor revision */

/*
 * Bit definitions for the Unit Present Register
 *
 */
#define SPR_UPR_UP 0x00000001    /* UPR present */
#define SPR_UPR_DCP 0x00000002   /* Data cache present */
#define SPR_UPR_ICP 0x00000004   /* Instruction cache present */
#define SPR_UPR_DMP 0x00000008   /* Data MMU present */
#define SPR_UPR_IMP 0x00000010   /* Instruction MMU present */
#define SPR_UPR_OB32P 0x00000020 /* ORBIS32 present */
#define SPR_UPR_OB64P 0x00000040 /* ORBIS64 present */
#define SPR_UPR_OF32P 0x00000080 /* ORFPX32 present */
#define SPR_UPR_OF64P 0x00000100 /* ORFPX64 present */
#define SPR_UPR_OV32P 0x00000200 /* ORVDX32 present */
#define SPR_UPR_OV64P 0x00000400 /* ORVDX64 present */
#define SPR_UPR_DUP 0x00000800   /* Debug unit present */
#define SPR_UPR_PCUP 0x00001000  /* Performance counters unit present */
#define SPR_UPR_PMP 0x00002000   /* Power management present */
#define SPR_UPR_PICP 0x00004000  /* PIC present */
#define SPR_UPR_TTP 0x00008000   /* Tick timer present */
#define SPR_UPR_SRP 0x00010000   /* Shadow registers present */
#define SPR_UPR_RES 0x00fe0000   /* ORVDX32 present */
#define SPR_UPR_CUST 0xff000000  /* Custom units */

/*
 * Bit definitions for the Supervision Register
 *
 */
#define SPR_SR_CID 0xf0000000  /* Context ID */
#define SPR_SR_FO 0x00008000   /* Fixed one */
#define SPR_SR_EPH 0x00004000  /* Exception Prefixi High */
#define SPR_SR_DSX 0x00002000  /* Delay Slot Exception */
#define SPR_SR_OVE 0x00001000  /* Overflow flag Exception */
#define SPR_SR_OV 0x00000800   /* Overflow flag */
#define SPR_SR_CY 0x00000400   /* Carry flag */
#define SPR_SR_F 0x00000200    /* Condition Flag */
#define SPR_SR_CE 0x00000100   /* CID Enable */
#define SPR_SR_LEE 0x00000080  /* Little Endian Enable */
#define SPR_SR_IME 0x00000040  /* Instruction MMU Enable */
#define SPR_SR_DME 0x00000020  /* Data MMU Enable */
#define SPR_SR_ICE 0x00000010  /* Instruction Cache Enable */
#define SPR_SR_DCE 0x00000008  /* Data Cache Enable */
#define SPR_SR_IEE 0x00000004  /* Interrupt Exception Enable */
#define SPR_SR_TEE 0x00000002  /* Tick timer Exception Enable */
#define SPR_SR_LIEE 0x00000002 /* Tick timer Exception Enable */
#define SPR_SR_SM 0x00000001   /* Supervisor Mode */

/*
 * Bit definitions for the Data MMU Control Register
 *
 */
#define SPR_DMMUCR_P2S 0x0000003e         /* Level 2 Page Size */
#define SPR_DMMUCR_P1S 0x000007c0         /* Level 1 Page Size */
#define SPR_DMMUCR_VADDR_WIDTH 0x0000f800 /* Virtual ADDR Width */
#define SPR_DMMUCR_PADDR_WIDTH 0x000f0000 /* Physical ADDR Width */

/*
 * Bit definitions for the Instruction MMU Control Register
 *
 */
#define SPR_IMMUCR_P2S 0x0000003e         /* Level 2 Page Size */
#define SPR_IMMUCR_P1S 0x000007c0         /* Level 1 Page Size */
#define SPR_IMMUCR_VADDR_WIDTH 0x0000f800 /* Virtual ADDR Width */
#define SPR_IMMUCR_PADDR_WIDTH 0x000f0000 /* Physical ADDR Width */

/*
 * Bit definitions for the Data TLB Match Register
 *
 */
#define SPR_DTLBMR_V 0x00000001   /* Valid */
#define SPR_DTLBMR_PL1 0x00000002 /* Page Level 1 (if 0 then PL2) */
#define SPR_DTLBMR_CID 0x0000003c /* Context ID */
#define SPR_DTLBMR_LRU 0x000000c0 /* Least Recently Used */
#define SPR_DTLBMR_VPN 0xfffff000 /* Virtual Page Number */

/*
 * Bit definitions for the Data TLB Translate Register
 *
 */
#define SPR_DTLBTR_CC 0x00000001  /* Cache Coherency */
#define SPR_DTLBTR_CI 0x00000002  /* Cache Inhibit */
#define SPR_DTLBTR_WBC 0x00000004 /* Write-Back Cache */
#define SPR_DTLBTR_WOM 0x00000008 /* Weakly-Ordered Memory */
#define SPR_DTLBTR_A 0x00000010   /* Accessed */
#define SPR_DTLBTR_D 0x00000020   /* Dirty */
#define SPR_DTLBTR_URE 0x00000040 /* User Read Enable */
#define SPR_DTLBTR_UWE 0x00000080 /* User Write Enable */
#define SPR_DTLBTR_SRE 0x00000100 /* Supervisor Read Enable */
#define SPR_DTLBTR_SWE 0x00000200 /* Supervisor Write Enable */
#define SPR_DTLBTR_PPN 0xfffff000 /* Physical Page Number */
#define DTLB_PR_NOLIMIT (SPR_DTLBTR_URE | \
                         SPR_DTLBTR_UWE | \
                         SPR_DTLBTR_SRE | \
                         SPR_DTLBTR_SWE)
/*
 * Bit definitions for the Instruction TLB Match Register
 *
 */
#define SPR_ITLBMR_V 0x00000001   /* Valid */
#define SPR_ITLBMR_PL1 0x00000002 /* Page Level 1 (if 0 then PL2) */
#define SPR_ITLBMR_CID 0x0000003c /* Context ID */
#define SPR_ITLBMR_LRU 0x000000c0 /* Least Recently Used */
#define SPR_ITLBMR_VPN 0xfffff000 /* Virtual Page Number */

/*
 * Bit definitions for the Instruction TLB Translate Register
 *
 */
#define SPR_ITLBTR_CC 0x00000001  /* Cache Coherency */
#define SPR_ITLBTR_CI 0x00000002  /* Cache Inhibit */
#define SPR_ITLBTR_WBC 0x00000004 /* Write-Back Cache */
#define SPR_ITLBTR_WOM 0x00000008 /* Weakly-Ordered Memory */
#define SPR_ITLBTR_A 0x00000010   /* Accessed */
#define SPR_ITLBTR_D 0x00000020   /* Dirty */
#define SPR_ITLBTR_SXE 0x00000040 /* User Read Enable */
#define SPR_ITLBTR_UXE 0x00000080 /* User Write Enable */
#define SPR_ITLBTR_PPN 0xfffff000 /* Physical Page Number */
#define ITLB_PR_NOLIMIT (SPR_ITLBTR_SXE | \
                         SPR_ITLBTR_UXE)

/*
 * Bit definitions for Data Cache Control register
 *
 */
#define SPR_DCCR_EW 0x000000ff /* Enable ways */

/*
 * Bit definitions for Insn Cache Control register
 *
 */
#define SPR_ICCR_EW 0x000000ff /* Enable ways */

/*
 * Bit definitions for Data Cache Configuration Register
 *
 */
#define SPR_DCCFGR_NCW 0x00000007
#define SPR_DCCFGR_NCS 0x00000078
#define SPR_DCCFGR_CBS 0x00000080
#define SPR_DCCFGR_CWS 0x00000100
#define SPR_DCCFGR_CCRI 0x00000200
#define SPR_DCCFGR_CBIRI 0x00000400
#define SPR_DCCFGR_CBPRI 0x00000800
#define SPR_DCCFGR_CBLRI 0x00001000
#define SPR_DCCFGR_CBFRI 0x00002000
#define SPR_DCCFGR_CBWBRI 0x00004000

#define SPR_DCCFGR_NCW_OFF 0
#define SPR_DCCFGR_NCS_OFF 3
#define SPR_DCCFGR_CBS_OFF7

/*
 * Bit definitions for Instruction Cache Configuration Register
 *
 */
#define SPR_ICCFGR_NCW 0x00000007
#define SPR_ICCFGR_NCS 0x00000078
#define SPR_ICCFGR_CBS 0x00000080
#define SPR_ICCFGR_CCRI 0x00000200
#define SPR_ICCFGR_CBIRI 0x00000400
#define SPR_ICCFGR_CBPRI 0x00000800
#define SPR_ICCFGR_CBLRI 0x00001000

#define SPR_ICCFGR_NCW_OFF 0
#define SPR_ICCFGR_NCS_OFF 3
#define SPR_ICCFGR_CBS_OFF7

/*
 * Bit definitions for Debug Control registers
 *
 */
#define SPR_DCR_DP 0x00000001 /* DVR/DCR present */
#define SPR_DCR_CC 0x0000000e /* Compare condition */
#define SPR_DCR_SC 0x00000010 /* Signed compare */
#define SPR_DCR_CT 0x000000e0 /* Compare to */

/* Bit results with SPR_DCR_CC mask */
#define SPR_DCR_CC_MASKED 0x00000000
#define SPR_DCR_CC_EQUAL 0x00000001
#define SPR_DCR_CC_LESS 0x00000002
#define SPR_DCR_CC_LESSE 0x00000003
#define SPR_DCR_CC_GREAT 0x00000004
#define SPR_DCR_CC_GREATE 0x00000005
#define SPR_DCR_CC_NEQUAL 0x00000006

/* Bit results with SPR_DCR_CT mask */
#define SPR_DCR_CT_DISABLED 0x00000000
#define SPR_DCR_CT_IFEA 0x00000020
#define SPR_DCR_CT_LEA 0x00000040
#define SPR_DCR_CT_SEA 0x00000060
#define SPR_DCR_CT_LD 0x00000080
#define SPR_DCR_CT_SD 0x000000a0
#define SPR_DCR_CT_LSEA 0x000000c0

/*
 * Bit definitions for Debug Mode 1 register
 *
 */
#define SPR_DMR1_CW0 0x00000003  /* Chain watchpoint 0 */
#define SPR_DMR1_CW1 0x0000000c  /* Chain watchpoint 1 */
#define SPR_DMR1_CW2 0x00000030  /* Chain watchpoint 2 */
#define SPR_DMR1_CW3 0x000000c0  /* Chain watchpoint 3 */
#define SPR_DMR1_CW4 0x00000300  /* Chain watchpoint 4 */
#define SPR_DMR1_CW5 0x00000c00  /* Chain watchpoint 5 */
#define SPR_DMR1_CW6 0x00003000  /* Chain watchpoint 6 */
#define SPR_DMR1_CW7 0x0000c000  /* Chain watchpoint 7 */
#define SPR_DMR1_CW8 0x00030000  /* Chain watchpoint 8 */
#define SPR_DMR1_CW9 0x000c0000  /* Chain watchpoint 9 */
#define SPR_DMR1_CW10 0x00300000 /* Chain watchpoint 10 */
#define SPR_DMR1_ST 0x00400000   /* Single-step trace*/
#define SPR_DMR1_BT 0x00800000   /* Branch trace */
#define SPR_DMR1_DXFW 0x01000000 /* Disable external force watchpoint */

/*
 * Bit definitions for Debug Mode 2 register
 *
 */
#define SPR_DMR2_WCE0 0x00000001 /* Watchpoint counter 0 enable */
#define SPR_DMR2_WCE1 0x00000002 /* Watchpoint counter 0 enable */
#define SPR_DMR2_AWTC 0x00001ffc /* Assign watchpoints to counters */
#define SPR_DMR2_WGB 0x00ffe000  /* Watchpoints generating breakpoint */

/*
 * Bit definitions for Debug watchpoint counter registers
 *
 */
#define SPR_DWCR_COUNT 0x0000ffff /* Count */
#define SPR_DWCR_MATCH 0xffff0000 /* Match */

/*
 * Bit definitions for Debug stop register
 *
 */
#define SPR_DSR_RSTE 0x00000001  /* Reset exception */
#define SPR_DSR_BUSEE 0x00000002 /* Bus error exception */
#define SPR_DSR_DPFE 0x00000004  /* Data Page Fault exception */
#define SPR_DSR_IPFE 0x00000008  /* Insn Page Fault exception */
#define SPR_DSR_TTE 0x00000010   /* iTick Timer exception */
#define SPR_DSR_AE 0x00000020    /* Alignment exception */
#define SPR_DSR_IIE 0x00000040   /* Illegal Instruction exception */
#define SPR_DSR_IE 0x00000080    /* Interrupt exception */
#define SPR_DSR_DME 0x00000100   /* DTLB miss exception */
#define SPR_DSR_IME 0x00000200   /* ITLB miss exception */
#define SPR_DSR_RE 0x00000400    /* Range exception */
#define SPR_DSR_SCE 0x00000800   /* System call exception */
#define SPR_DSR_SSE 0x00001000   /* Single Step Exception */
#define SPR_DSR_TE 0x00002000    /* Trap exception */

/*
 * Bit definitions for Debug reason register
 *
 */
#define SPR_DRR_RSTE 0x00000001  /* Reset exception */
#define SPR_DRR_BUSEE 0x00000002 /* Bus error exception */
#define SPR_DRR_DPFE 0x00000004  /* Data Page Fault exception */
#define SPR_DRR_IPFE 0x00000008  /* Insn Page Fault exception */
#define SPR_DRR_TTE 0x00000010   /* Tick Timer exception */
#define SPR_DRR_AE 0x00000020    /* Alignment exception */
#define SPR_DRR_IIE 0x00000040   /* Illegal Instruction exception */
#define SPR_DRR_IE 0x00000080    /* Interrupt exception */
#define SPR_DRR_DME 0x00000100   /* DTLB miss exception */
#define SPR_DRR_IME 0x00000200   /* ITLB miss exception */
#define SPR_DRR_RE 0x00000400    /* Range exception */
#define SPR_DRR_SCE 0x00000800   /* System call exception */
#define SPR_DRR_TE 0x00001000    /* Trap exception */

/*
 * Bit definitions for Performance counters mode registers
 *
 */
#define SPR_PCMR_CP 0x00000001    /* Counter present */
#define SPR_PCMR_UMRA 0x00000002  /* User mode read access */
#define SPR_PCMR_CISM 0x00000004  /* Count in supervisor mode */
#define SPR_PCMR_CIUM 0x00000008  /* Count in user mode */
#define SPR_PCMR_LA 0x00000010    /* Load access event */
#define SPR_PCMR_SA 0x00000020    /* Store access event */
#define SPR_PCMR_IF 0x00000040    /* Instruction fetch event*/
#define SPR_PCMR_DCM 0x00000080   /* Data cache miss event */
#define SPR_PCMR_ICM 0x00000100   /* Insn cache miss event */
#define SPR_PCMR_IFS 0x00000200   /* Insn fetch stall event */
#define SPR_PCMR_LSUS 0x00000400  /* LSU stall event */
#define SPR_PCMR_BS 0x00000800    /* Branch stall event */
#define SPR_PCMR_DTLBM 0x00001000 /* DTLB miss event */
#define SPR_PCMR_ITLBM 0x00002000 /* ITLB miss event */
#define SPR_PCMR_DDS 0x00004000   /* Data dependency stall event */
#define SPR_PCMR_WPE 0x03ff8000   /* Watchpoint events */

/*
 * Bit definitions for the Power management register
 *
 */
#define SPR_PMR_SDF 0x0000000f  /* Slow down factor */
#define SPR_PMR_DME 0x00000010  /* Doze mode enable */
#define SPR_PMR_SME 0x00000020  /* Sleep mode enable */
#define SPR_PMR_DCGE 0x00000040 /* Dynamic clock gating enable */
#define SPR_PMR_SUME 0x00000080 /* Suspend mode enable */

/*
 * Bit definitions for PICMR
 *
 */
#define SPR_PICMR_IUM 0xfffffffc /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 *
 */
#define SPR_PICPR_IPRIO 0xfffffffc /* Interrupt priority */

/*
 * Bit definitions for PICSR
 *
 */
#define SPR_PICSR_IS 0xffffffff /* Interrupt status */

/*
 * Bit definitions for Tick Timer Control Register
 *
 */
#define SPR_TTCR_PERIOD 0x0fffffff /* Time Period */
#define SPR_TTMR_PERIOD SPR_TTCR_PERIOD
#define SPR_TTMR_IP 0x10000000 /* Interrupt Pending */
#define SPR_TTMR_IE 0x20000000 /* Interrupt Enable */
#define SPR_TTMR_RT 0x40000000 /* Restart tick */
#define SPR_TTMR_SR 0x80000000 /* Single run */
#define SPR_TTMR_CR 0xc0000000 /* Continuous run */
#define SPR_TTMR_M 0xc0000000  /* Tick mode */

/*
 * l.nop constants
 *
 */
#define NOP_NOP 0x0000          /* Normal nop instruction */
#define NOP_EXIT 0x0001         /* End of simulation */
#define NOP_REPORT 0x0002       /* Simple report */
#define NOP_PRINTF 0x0003       /* Simprintf instruction */
#define NOP_REPORT_FIRST 0x0400 /* Report with number */
#define NOP_REPORT_LAST 0x03ff  /* Report with number */
                                // extern unsigned long mfspr(unsigned long spr);
#define ALIGNED(x) __attribute__((aligned(x)))

#define mfspr(spr) (spr) // add wq
#define mtspr(spr, val) \
   do                   \
   {                    \
      spr = val;        \
   } while (0)

// spr set or clr bit define
// #define __sprset(spr,n)	do{long val; val = mfspr(spr); val |=  (1 << n); mtspr(spr,val);}while(0)
// #define __sprclr(spr,n)	do{long val; val = mfspr(spr); val &= ~(1 << n); mtspr(spr,val);}while(0)
#define __sprset(spr, n) mtspr(spr, mfspr(spr) | (1 << n))
#define __sprclr(spr, n) mtspr(spr, mfspr(spr) & ~(1 << n))

#define __gie_dis() __sprclr(SPR_SR, 2)
#define __gie_en() __sprset(SPR_SR, 2)
#define __intvct_mask() __sprclr(SPR_SR, 14)
#define __intvct_sram() __sprset(SPR_SR, 14)

// #include "int/int.h"
#define __KEEP_INT__ ((1 << 14) | (1 << 16)) // CSI_INT:14,JPEG_INT:16
/*
#define __gie_dis() do{u32 val; val = mfspr(SPR_SR); val &= ~(1<<2); mtspr(SPR_SR,val);}while(0)
#define __gie_en()  do{u32 val; val = mfspr(SPR_SR); val |= (1<<2); mtspr(SPR_SR,val);}while(0)

#define __intvct_sram()  do{u32 val; val = mfspr(SPR_SR); val |= (1<<14); mtspr(SPR_SR,val);}while(0)
*/
#endif
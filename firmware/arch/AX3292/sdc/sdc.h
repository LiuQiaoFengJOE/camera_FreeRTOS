#ifndef __SD1_H
#define __SD1_H

#include "typedef.h"
#include "mmc/core.h"

#ifndef _SD_H_

#define SD_CLK 20000000 //
#define SD_HS_CLK 30000000

#define SD_SAMPLE_EDGE 0 // 0:fall; 1:rise
#define FWS(FunctionWithSD)  \
    do                       \
    {                        \
        if (!FunctionWithSD) \
            return FALSE;    \
    } while (0)

// typedef volatile __sfr unsigned long *pSFR;
typedef unsigned int pSFR;
#define sdreg(base, offset) (SFR_AT(base + (offset)*4))

#define SDxCON0 0
#define SDxCON1 1
#define SDxBAUD 2
#define SDxCADR 3
#define SDxDADR 4
#define SDxDCNT 5
#define SDxIDX 6
#define SDxARG 7

#define SD_CLOCK_BASE SYS_CHIP_CLOCK /*APB_CLK*/
#define SD_BAUDRATE 125000           // 750000
#define SD_INITBAUD 250000           // 300000    //400000
#define SD_NORMAL_WR 16000000        // 12000000
#define SD_MAP_CON PMAPCON0
#define SD_MAP_POS 4
#define SD_MAP_MSK 15

#define OFFS_DIR 0x10
#define OFFS_PU 0x20
#define OFFS_PD 0x28

#define WIDTH_CRCS 3
#define BIT_CRCS 21
#define BIT_BUSY 20
#define BIT_NRPS 19
#define BIT_CCRCE 18
#define BIT_CPND 17
#define BIT_DCRCE 16
#define BIT_DPND 15

#define BIT_ELYCLK 31

#define BIT_DIE 9
#define BIT_CIE 8

#define BIT_SDCSEN 6
#define BIT_SPCRCSTA 5
#define BIT_8CKE 4
#define BIT_ORISE 3
#define BIT_DW4 2
#define BIT_SDCKE 1
#define BIT_SDE 0

// SDCCON1
#define BIT_CPCLR 17
#define BIT_DPCLR 15
#define WIDTH_DKST 2
#define BIT_DKST 4
#define WIDTH_CKST 3
#define BIT_CKST 0

#define GET_SD_R1() SD1ARG        // SD_REG[OFFS_RESP6B]
#define GET_SD_R7_CMDIDX() SD1IDX // SD_REG[OFFS_RESP1]
// #define GET_SD_CSD1()       GPARAM(SD_RESP1)//SD_REG[OFFS_RESP1]
// #define GET_SD_CSD2()       GPARAM(SD_RESP2)//SD_REG[OFFS_RESP2]
#define CHK_SD_DCRCE() SD1CON0 &BIT(BIT_DCRCE)

// to be deleted
// #define NO_RESP BIT(BIT_CKST+1)  //send cmd. without receive response; without check busy
// #define RESP_1  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
// #define RESP_1B (BIT(BIT_CKST+2) | BIT(BIT_CKST+0))  //send cmd. receive 6 byte response; with check busy
// #define RESP_2  (BIT(BIT_CKST+1) | BIT(BIT_CKST+0))  //send cmd. receive 17 byte response; without check busy
// #define RESP_3  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
// #define RESP_6  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
// #define RESP_7  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
// #define RESP_4	RESP_6
// #define RESP_5	RESP_1

#define soft_delay(t)             \
    do                            \
    {                             \
        volatile u32 i = (t) / 2; \
        while (i--)               \
            ;                     \
    } while (0)

#endif /*#ifndef _SD_H_*/

#define SD1_CLK 20000000
#define SD1_HS_CLK 30000000
#define SD1_SAMPLE_EDGE 0 // 0:fall; 1:rise

#define SD1_KEEP_CLK_OUT SDCON0 |= BIT(BIT_SDCKE)
#define SD1_ONLY_CMD_DATA_CLK_OUT SDCON0 &= ~BIT(BIT_SDCKE)
#define SD1_SET_4_LINE SDCON0 |= BIT(BIT_DW4)
#define SD1_SET_1_LINE SDCON0 &= ~BIT(BIT_DW4)
#define SD1_CMD_CRC_ERR_PEND() ((SDCON0 & BIT(13)) == BIT(13))
#define SD1_READ_CRC_ERR_PEND() ((SDCON0 & BIT(11)) == BIT(11))  /*读卡时如果是CRC ERR，返回TRUE，无ERR返回FLASE*/
#define SD1_WRITE_CRC_ERR_PEND() ((SDCON0 & 0x30000) != 0x20000) /*写卡时CRC ERR返回TURE*/
#define SD1_CLK_SET(x) SDBAUD = SD_CLOCK_BASE / (x)-1

#ifndef soft_delay
#define soft_delay(t)             \
    do                            \
    {                             \
        volatile u32 i = (t) / 2; \
        while (i--)               \
            ;                     \
    } while (0)
#endif

#endif

#include <or1k-asm.h>

#define FATVER 28

.macro      PUB_FUNC name
        .global     \name
\name:
        .endm

        .macro      PUB_LABEL name
        .global     \name
\name:
        .endm


#define PLL_CLOCK               50000
#define COMBO_CLOCK             120000000
#define UART_BAUDRATE           115200
#define SD_BAUDRATE             6000000

#define OTHER_SEC_ADDR          bpb_ram_base

#define BUS1                    0
#define REMAP                   0
#define JTAG_EN                 0
#define SD4                     1
#define INIT_PLL_RUN            0
#define CLKTUN_EN               1
#define SDRAM_EN                1


#define NO_CHKSUM               1
#define INVALID_KEY             0
#define CFG_FUNC                1

#define USE_UART0               3
#ifndef DBG_EN
#define DBG_EN                  0
#endif
/*
CLKTUN
[31:30] SDOTU_S : SDRAM Data output clock tune clock output select
  00 -- SDR_CLK PAD feedback clock
  01 -- SDR_CLK output clock
  10 -- SDR_CLK tuning output clock
  11 -- AHB bus clock
[29] -- SDOTUEN : SDRAM Data output clock tune function enable
[28] -- SDOTUP : SDRAM Data output clock tuning polarity
   0 -- SDRAM clock tune backwards
   1 -- SDRAM clock tune forwards
[27:24] SDOTUC
  [28:24] = {if(steps>0)--steps; steps&0x1F;}
[21] -- CSITUEN : CSI clock tune function enable
[20] -- CSITUP : CSI clock tuning polarity
[19:16] CSICTU
  [20:16] = {if(steps>0)--steps; steps&0x1F;}
[13] -- SCOTUEN : SDRAM Clock output tune function enable
[12] -- SCOTUP : SDRAM Clock output tuning polarity
   0 -- SDRAM clock tune backwards
   1 -- SDRAM clock tune forwards
[11:8] SDOCTU
  [12: 8] = {if(steps>0)--steps; steps&0x1F;}
[7:6] -- SDITU_S : SDRAM Data input clock tune clock input select
  00 -- SDR_CLK PAD feedback clock
  01 -- SDR_CLK output clock
  10 -- SDR_CLK tuning output clock
  11 -- AHB bus clock
[5]  -- SDITUEN : SDRAM Data input clock tune function enable
[4]  -- SDITUP : SDRAM Data input clock tuning polarity
   0 -- SDRAM clock tune backwards
   1 -- SDRAM clock tune forwards
[3:0] SDICTU
  [ 4: 0] = {if(steps>0)--steps; steps&0x1F;}
*/
// 相應Value為0是，其CLKTUN功能是否Disable
#define SDOTUDIS                0
#define SCOTUDIS                0
#define SDITUDIS                0
#define CSITUDIS                1

#define SDOTU_S                 1
#define SDITU_S                 1

#define SDOTUC                  -16
#define SDOCTU                  -16
#define SDITUC                  -16
#define CSICTU                  0

#if SDOTUC==0
# define SDOTUEN                (1 & ~SDOTUDIS)
# define SDOTUCV                0
#elif SDOTUC>0
# define SDOTUEN                1
# define SDOTUCV                ((SDOTUC-1)&0x1F)
#else
# define SDOTUEN                1
# define SDOTUCV                (SDOTUC&0x1F)
#endif

#if SDOCTU==0
# define SCOTUEN                (1 & ~SCOTUDIS)
# define SDOCTUV                0
#elif SDOCTU>0
# define SCOTUEN                1
# define SDOCTUV                ((SDOCTU-1)&0x1F)
#else
# define SCOTUEN                1
# define SDOCTUV                (SDOCTU&0x1F)
#endif

#if SDITUC==0
# define SDITUEN                (1 & ~SDITUDIS)
# define SDITUCV                0
#elif SDITUC>0
# define SDITUEN                1
# define SDITUCV                ((SDITUC-1)&0x1F)
#else
# define SDITUEN                1
# define SDITUCV                (SDITUC&0x1F)
#endif

#if CSICTU==0
# define CSITUEN                (1 & ~CSITUDIS)
# define CSICTUV                0
#elif CSICTU>0
# define CSITUEN                1
# define CSICTUV                ((CSICTU-1)&0x1F)
#else
# define CSITUEN                1
# define CSICTUV                (CSICTU&0x1F)
#endif

#define  SDR_CFG0_VAL      0x83
#define  SDR_CFG1_VAL      0x20735f//0x28737f，需要修改为0x20735f。  873那三个是用来配置CBR的时间间隔的。这个是根据每个SDRAM不一样进行配置的


//clk_sel: 00:fb_clk; 01:sdr_clko; 1x: sdr_clk
//              clk_sel    enable    forward    step
//#define SDRITUN ((2<<6)  | (1<<5)  | (1<<4)   | (3<<0))
//#define SCOTUN  ((2<<14) | (1<<13) | (1<<12)) | (0x8<<8)
//#define SDOTUN            ((1<<21) | (1<<20)  | (3<<16))
#define SDRITUN ((1<<6)  | (1<<5)  | (0<<4)   | (3<<0))
#define SCOTUN  ((2<<14) | (1<<13) | (1<<12)) | (0x8<<8)
#define SDOTUN            ((1<<21) | (0<<20)  | (5<<16))

#if 1
# define CLKTUN_VAL     SDRITUN|SCOTUN|SDOTUN 
#else
# define CLKTUN_VAL             (SDOTU_S<<30)|(SDOTUEN<<29)|(SDOTUCV<<24) | \
                                              (CSITUEN<<21)|(CSICTUV<<16) | \
                                              (SCOTUEN<<13)|(SDOCTUV<< 8) | \
                                (SDITU_S<< 6)|(SDITUEN<< 5)|(SDITUCV<< 0)
#endif


#define SDRAM_SIZE              0x2000000

// 注：80pin封裝不能開JTAG_EN

#if REMAP
#define __sdram 0x00000000
#else
#define __sdram 0x2000000
#endif

#define SYS_CFG_VAL             (BUS1<<3)|(REMAP<<2)|(JTAG_EN<<0)//|(SDRAM_EN<<7)

#define FLAG_BYTE               (CFG_FUNC<<0) | (INVALID_KEY<<1) | (NO_CHKSUM<<2)


#define FT_FAT16                16
#define FT_FAT32                28
#define FT_exFAT                32

################################################################################
/* 定義一些與FAT類型相關的操作和參數
 */
#if (FATVER == FT_FAT16)      //////////////////////////////// FAT16
# define OTHER_SEC     1
#elif (FATVER == FT_FAT32)    //////////////////////////////// FAT32
# define OTHER_SEC     2
#elif (FATVER == FT_exFAT)    //////////////////////////////// exFAT
# define OTHER_SEC     1

#else
#error "Error FATVER"
#endif



#if CPU_BIG_ENDIAN
    #define BLDR_VER       0x00000200
    #define CHECKSUM       0x67452301
    #define MAGICKEY       0x67452301
    #define SPI_DMA_SHIFT  0
    #define SPINAND_CMD    0x0FC01303
#else
    #define BLDR_VER       0x00020000
    #define CHECKSUM       0x01234567 
    #define MAGICKEY       0x01234567
    #define SPI_DMA_SHIFT  0
    #define SPINAND_CMD    0x0313C00F
#endif

        .section    .text_dbr, "ax"
        .global     __true_start
__true_start:
        .long       BLDR_VER
        .ascii      "BLDR"
        .byte       0x00	// Should be Check Sum of the sector
        .byte       (flash_param - __true_start) / 16
        .byte       FLAG_BYTE

#if (FATVER == FT_FAT16)
        .align      4
.L_alphapp_sys:         .asciz  "ALPHA   BIN" // 40 -- 64
#endif

        .align      16, 0
.L_param:
flash_param:
////////////////////////////////////////////////////////////////////////////////
        .ascii      "0123456789ABCDEF"

        .long       OTHER_SEC_ADDR            // 另一個扇區的內存地址
        .long       OTHER_SEC                 // 另一個扇區在1扇區
        .long       1
        .long       dbr_entry

        .long       CHECKSUM                          // Check Sum of App
        .long       MAGICKEY                          // Magic 1
        .long       SPI_DMA_SHIFT                     // SPI DMA & Page Size Shift
        .long       SPINAND_CMD                       // SPI Nand Command
        .long       COMBO_CLOCK / (SD_BAUDRATE) - 1 // SD Baud

////////////////////////////////////////////////////////////////////////////////
// 配置函數 __OR1K__
#if CFG_FUNC
#include "cfg_hlp.inc"

config_begin:
.Lconfig_begin:
CONFIG_BEGIN


//////////////////////////////
        return0
        return0
        return0
CONFIG_END
#endif




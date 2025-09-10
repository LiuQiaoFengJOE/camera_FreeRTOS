#include "typedef.h"

#include "spr_defs.h"
#include "AX329x.h"

// 兩個jpeg控制器的中斷號不一樣
#define IRQNO_JPGA 16
#define IRQNO_JPGB 22

// 狀態和錯誤標誌
#define JPCIRQ_OUT_PACK_FULL 0
#define JPCIRQ_IN_PACK_EMPTY 1
#define JPCIRQ_DATA_ERR 3
#define JPCIRQ_JDRST 5

#define JPCIRQ_FRAME_DONE 6
#define JPCIRQ_ENC_OUT_FULL 7
#define JPCIRQ_DEC_HUFF_ERR 8
#define JPCIRQ_DEC_DCTIMEROUT 13

#define JPCIRQ_MAX 14

// 兩個jpeg控制器寄存器定義是一樣的
#define JPxCON1 0x000
#define JPxCON2 0x001
#define JPxCON3 0x002
#define JPxSIZE0 0x003
#define JPxOBAS 0x004
#define JPxOYADR 0x004
#define JPxOEND 0x005
#define JPxOUVADR 0x005
#define JPxOPTR 0x006
#define JPxOLTA 0x007
#define JPxIBAS 0x008
#define JPxIYADR 0x008
#define JPxIEND 0x009
#define JPxIUVADR 0x009
#define JPxIPTR 0x00a
#define JPxUBIT 0x00b
#define JPxHCNT 0x00c
#define JPxDRI 0x00d
#define JPxBSHADDR 0x00e
#define JPxINTCON 0x00f
#define JPxFLG 0x010
#define JPxLS 0x011
#define JPxES 0x012
#define JPxCOM 0x013
#define JPxTINF 0x014
#define JPxT0SIZE 0x015
#define JPxT1SIZE 0x016
#define JPxT2SIZE 0x017
#define JPxT0POS 0x018
#define JPxT1POS 0x019
#define JPxT2POS 0x01a
#define JPxT0BASE 0x01b
#define JPxT1BASE 0x01c
#define JPxT2BASE 0x01d
#define JPxSIZE1 0x01e
#define JPxXSCALE 0x01f
#define JPxYSCALE 0x020
#define JPxBSCCOEF0 0x021
#define JPxBSCCOEF1 0x022
#define JPxBSCCOEF2 0x023
#define JPxBSCCOEF3 0x024
#define JPxBSCCOEF4 0x025
#define JPxBSCCOEF5 0x026
#define JPxBSCCOEF6 0x027
#define JPxBSCCOEF7 0x028

// AX3292与AX3281区别
/******************************/
#define JPxBSCCOEF10 0x031
#define JPxBSCCOEF11 0x032
#define JPxBSCCOEF12 0x033
#define JPxBSCCOEF13 0x034
#define JPxBSCCOEF14 0x035
#define JPxBSCCOEF15 0x036
#define JPxBSCCOEF16 0x037
#define JPxBSCCOEF17 0x038

#define LINE_JPxBSCCOEF0 0x039
#define LINE_JPxBSCCOEF1 0x03a
#define LINE_JPxBSCCOEF2 0x03b
#define LINE_JPxBSCCOEF3 0x03c
#define LINE_JPxBSCCOEF4 0x03d
#define LINE_JPxBSCCOEF5 0x03e
#define LINE_JPxBSCCOEF6 0x03f
#define LINE_JPxBSCCOEF7 0x040

#define UVJPxBSCCOEF0 0x041
#define UVJPxBSCCOEF1 0x042
#define UVJPxBSCCOEF2 0x043
#define UVJPxBSCCOEF3 0x044
#define UVJPxBSCCOEF4 0x045
#define UVJPxBSCCOEF5 0x046
#define UVJPxBSCCOEF6 0x047
#define UVJPxBSCCOEF7 0x048
#define UVJPxBSCCOEF10 0x049
#define UVJPxBSCCOEF11 0x04a
#define UVJPxBSCCOEF12 0x04b
#define UVJPxBSCCOEF13 0x04c
#define UVJPxBSCCOEF14 0x04d
#define UVJPxBSCCOEF15 0x04e
#define UVJPxBSCCOEF16 0x04f
#define UVJPxBSCCOEF17 0x050
/*************************************/

#define JPxCON1_DCTIMER 0x051
#define JPxCON1_DEBLOCK 0x052

#define JPxELINECNT 0x029
#define JPxDCXCROP 0x02a
#define JPxDCYCROP 0x02b
#define JPxFLG_CLR 0x02c
#define JPxPAUSECNT 0x02d
#define JPxDCYBASE 0x02e
#define JPxDCUVBASE 0x02f
#define JPxDMAOUTX32 0x030

#define CCOEF0 0x00017F00
#define CCOEF1 0xFF0E7CF7
#define CCOEF2 0xFC2170F3
#define CCOEF3 0xF8365FF3
#define CCOEF4 0xF54B4BF5
#define CCOEF5 0xF35F36F8
#define CCOEF6 0xF37021FC
#define CCOEF7 0xF77C0EFF

#define CCTAB0 186789922UL
#define CCTAB1 220475167UL
#define CCTAB2 270937372UL
#define CCTAB3 321399577UL
#define CCTAB4 371861782UL
#define CCTAB5 422258707UL
#define CCTAB6 472720912UL
#define CCTAB7 523117837UL

// decode table
#define DQTOFF 0x400
#define DQTSIZE 0x40
#define MINCODEOFF 0x500
#define MINCODESIZE 0x20
#define VALPTROFF 0x580
#define VALPTRSIZE 0x10
#define HUFFDCOFF 0x5C0
#define HUFFDCSIZE 0x0C
#define HUFFACOFF 0x5D8
#define HUFFACSIZE 0xA2

#define BIC_TAB00 0x40000000 // 0
#define BIC_TAB01 0x3ffa02ff // 1
#define BIC_TAB02 0x39f604ff // 2
#define BIC_TAB03 0x32f504ff // 3
#define BIC_TAB04 0x28f504ff // 4
#define BIC_TAB05 0x1df703ff // 5
#define BIC_TAB06 0x12fa0200 // 6
#define BIC_TAB07 0x08fd0100 // 7
#define BIC_TAB10 0x00000000 // 10
#define BIC_TAB11 0x0001fd08 // 11
#define BIC_TAB12 0x0002fa12 // 12
#define BIC_TAB13 0xff03f71d // 13
#define BIC_TAB14 0xff04f528 // 14
#define BIC_TAB15 0xff04f532 // 15
#define BIC_TAB16 0xff04f639 // 16
#define BIC_TAB17 0xff02fa3f // 17

#define BIC_UVTAB00 0x40000000 // 0
#define BIC_UVTAB01 0x3ffa02ff // 1
#define BIC_UVTAB02 0x39f604ff // 2
#define BIC_UVTAB03 0x32f504ff // 3
#define BIC_UVTAB04 0x28f504ff // 4
#define BIC_UVTAB05 0x1df703ff // 5
#define BIC_UVTAB06 0x12fa0200 // 6
#define BIC_UVTAB07 0x08fd0100 // 7
#define BIC_UVTAB10 0x00000000 // 10
#define BIC_UVTAB11 0x0001fd08 // 11
#define BIC_UVTAB12 0x0002fa12 // 12
#define BIC_UVTAB13 0xff03f71d // 13
#define BIC_UVTAB14 0xff04f528 // 14
#define BIC_UVTAB15 0xff04f532 // 15
#define BIC_UVTAB16 0xff04f639 // 16
#define BIC_UVTAB17 0xff02fa3f // 17

#define BIC_LINETAB00 0x00004000 // 0
#define BIC_LINETAB01 0x00063dfd // 1
#define BIC_LINETAB02 0xfe0f37fc // 2
#define BIC_LINETAB03 0xfd192ffb // 3
#define BIC_LINETAB04 0xfc2424fc // 4
#define BIC_LINETAB05 0xfb2f19fd // 5
#define BIC_LINETAB06 0xfc370ffe // 6
#define BIC_LINETAB07 0xfd3d0600 // 7

const int alpha_table_luma[16] = {0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}; // 列出一些有可能的配置
const int belta_table_luma[16] = {0, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, 10, 11, 12, 13, 16};
const int c_table_luma[16] = {0, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 8};
const int alpha_table_chroma[16] = {0, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 18, 20, 25, 30, 40};
const int belta_table_chroma[16] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
const int c_table_chroma[16] = {0, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 8};
#define INDEX_LUMA 3 // select cofigure para
#define INDEX_CHROMA 3
u32 block_scaler_tab[3][8]; // 0 :all lcd   1:half lcd  2:small window

typedef int (*isr_func)(unsigned int, void *, void *);
typedef unsigned int pSFR;
typedef struct jpeg_controller
{
  pSFR regbase;
  uint8_t *table_base;
  uint8_t irqno;
  uint8_t clken;
  isr_func isr[JPCIRQ_MAX];
  void *dev[2][JPCIRQ_MAX];
  uint32_t component; // 緩存分量編碼
  uint8_t jpeg_num;   // 记录对应的位置
} jpeg_controller;

static struct jpeg_controller jpg0_controller = {
    .regbase = (pSFR)&PRE_JPCON1,
    .table_base = 0x64000,
    .irqno = IRQNO_JPGA,
    .clken = 26,
};
static struct jpeg_controller jpg1_controller = {
    .regbase = (pSFR)&BACK_JPCON1,
    .table_base = 0x66000,
    .irqno = IRQNO_JPGB,
    .clken = 31,
};
struct jpeg_controller *jpeg_controllers[2] = {
    &jpg0_controller,
    &jpg1_controller};

#define jpreg(base, offset) (SFR_AT(base + (offset)*4))
#define get_jpreg(c) ((struct jpeg_controller *)(c))->regbase

typedef volatile __sfr unsigned long *pREG;
static inline pREG jp_reg(pSFR base, uint32_t offset)
{
  return (pREG)(base + offset * 4);
}

extern UINT32 APB_CLK; // defined by board

////////////////////////////////////////////////////////////////////////////////
static const int jpeg_zigzag_table[] =
    {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63};

static const uint8_t jpeg_quant_table_80[] = {
    0x06, 0x04, 0x04, 0x06, 0x0a, 0x10, 0x14, 0x18,
    0x05, 0x05, 0x06, 0x08, 0x0a, 0x17, 0x18, 0x16,
    0x06, 0x05, 0x06, 0x0a, 0x10, 0x17, 0x1c, 0x16,
    0x06, 0x07, 0x09, 0x0c, 0x14, 0x23, 0x20, 0x19,
    0x07, 0x09, 0x0f, 0x16, 0x1b, 0x2c, 0x29, 0x1f,
    0x0a, 0x0e, 0x16, 0x1a, 0x20, 0x2a, 0x2d, 0x25,
    0x14, 0x1a, 0x1f, 0x23, 0x29, 0x30, 0x30, 0x28,
    0x1d, 0x25, 0x26, 0x27, 0x2d, 0x28, 0x29, 0x28,

    0x07, 0x07, 0x0a, 0x13, 0x28, 0x28, 0x28, 0x28,
    0x07, 0x08, 0x0a, 0x1a, 0x28, 0x28, 0x28, 0x28,
    0x0a, 0x0a, 0x16, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x13, 0x1a, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,         
};

static const uint8_t jpeg_quant_table_75[] = {
    0x09, 0x06, 0x06, 0x09, 0x0d, 0x16, 0x1d, 0x22,
    0x07, 0x07, 0x08, 0x0b, 0x0f, 0x20, 0x22, 0x1f,
    0x08, 0x07, 0x09, 0x0d, 0x16, 0x20, 0x27, 0x1f,
    0x08, 0x0a, 0x0c, 0x10, 0x1d, 0x31, 0x2d, 0x23,
    0x0a, 0x0c, 0x15, 0x1f, 0x26, 0x3d, 0x3a, 0x2b,
    0x0d, 0x14, 0x1f, 0x24, 0x2d, 0x3a, 0x3f, 0x34,
    0x1b, 0x24, 0x2c, 0x31, 0x3a, 0x44, 0x43, 0x39,
    0x28, 0x34, 0x35, 0x37, 0x3f, 0x38, 0x3a, 0x37,

    0x0a, 0x0a, 0x0d, 0x1a, 0x37, 0x37, 0x37, 0x37,
    0x0a, 0x0c, 0x0f, 0x25, 0x37, 0x37, 0x37, 0x37,
    0x0d, 0x0f, 0x1f, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x1a, 0x25, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
    0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37,
};

static const uint8_t jpeg_quant_table_65[] = {
    0x0b, 0x08, 0x07, 0x0b, 0x11, 0x1c, 0x24, 0x2b,
    0x08, 0x08, 0x0a, 0x0d, 0x12, 0x29, 0x2a, 0x27,
    0x0a, 0x09, 0x0b, 0x11, 0x1c, 0x28, 0x30, 0x27,
    0x0a, 0x0c, 0x0f, 0x14, 0x24, 0x3d, 0x38, 0x2b,
    0x0d, 0x0f, 0x1a, 0x27, 0x30, 0x4c, 0x48, 0x36,
    0x11, 0x19, 0x27, 0x2d, 0x39, 0x49, 0x4f, 0x40,
    0x22, 0x2d, 0x37, 0x3d, 0x48, 0x55, 0x54, 0x47,
    0x32, 0x40, 0x43, 0x45, 0x4e, 0x46, 0x48, 0x45, 

    0x0c, 0x0d, 0x11, 0x21, 0x45, 0x45, 0x45, 0x45,
    0x0d, 0x0f, 0x12, 0x2e, 0x45, 0x45, 0x45, 0x45,
    0x11, 0x12, 0x27, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x21, 0x2e, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
    0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45,
};

static const uint8_t jpeg_quant_table_50[] = {
16,11,15,23,28,24,22,25, 
13,16,22,21,25,21,26,41, 
28,25,24,26,27,51,37,39, 
31,42,60,53,63,52,59,53, 
58,57,66,74,94,80,66,80, 
89,72,58,59,83,112,84,90, 
98,101,106,107,106,65,80,116, 
123,117,105,123,99,107,117,111,

17,18,18,24,21,24,47,26, 
26,47,99,66,56,66,99,111, 
99,99,99,99,99,99,111,115, 
99,99,99,99,99,111,115,120, 
99,99,99,99,111,115,121,122, 
99,99,99,111,115,120,123,123, 
99,99,111,115,121,123,124,124, 
99,111,115,120,122,123,124,125,
};


static const uint8_t jpeg_quant_table_low[] = {
    27,  18,  40,  37,  43,  68,  88, 105, // 30%
    25,  47,  42,  35,  47, 100,  87,  97, 
    38,  22,  42,  42,  70,  98, 120,  98, 
    27,  35,  40,  52,  88, 148, 138, 108, 
    37,  43,  65,  97, 133, 187, 177, 133, 
    45,  62,  95, 110, 140, 178, 193, 165, 
    85, 110, 133, 150, 177, 205, 205, 178, 
    123, 157, 163, 168, 195, 175, 195, 185, 

    34,  36,  48,  94, 198, 222, 198, 198, // 22%
    36,  42,  52, 132, 198, 198, 222, 198, 
    48,  52, 112, 198, 198, 230, 198, 222, 
    94, 132, 198, 198, 240, 198, 230, 246, 
    198, 198, 230, 198, 244, 240, 242, 248, 
    198, 222, 198, 242, 246, 230, 248, 244, 
    198, 198, 230, 246, 222, 198, 240, 246, 
    198, 222, 198, 198, 222, 230, 248, 250, 
};

static const uint8_t *jpeg_quant_table[] = {
    jpeg_quant_table_80,
    jpeg_quant_table_75,
    jpeg_quant_table_65,
    jpeg_quant_table_50,
    jpeg_quant_table_low};

static const uint8_t jpeg_huffman_table[] = {
    // DC Huffman table 0 TC //addr=0x20080 //len=16
    0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    // 0x0, 0x1, 0x5, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // DC Huffman table 1 TC //addr=0x20090 //len=16
    0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x0, 0x1, 0x5, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // AC Huffman table 0 TC //addr=0x200A0 //len=16
    0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d,
    // 0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7d,

    // AC Huffman table 1 TC //addr=0x200B0  //len=16
    0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77,
    // 0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77,

    // DC Huffman table 0 HN //addr=0x200C0 //len=28
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    // 0x03,0x0,0x01,0x02,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0,0x0,0x0,0x0,
    // 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,

    // DC Huffman table 1 HN //addr=0x200DC  //len=28
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    // 0x01,0x0,0x02,0x03,
    // 0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    // 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,

    // AC Huffman table 0 HN //addr=0x200F8  //len=178
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
    0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
    0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // AC Huffman table 1 HN //addr=0x201AA //len=178
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
    0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
    0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
    0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // reserved //addr=0x2025C //len=0x1A4 = 13*32+4
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,

    // AC Huffman code table0  //addr= 0x20400  //len=0x200
    0xa, 0x30, 0x0, 0x10, 0x1, 0x10, 0x4, 0x20, 0xb, 0x30, 0x1a, 0x40, 0x78, 0x60, 0xf8, 0x70,
    0xf6, 0x93, 0x82, 0xff, 0x83, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xc, 0x30, 0x1b, 0x40, 0x79, 0x60, 0xf6, 0x81, 0xf6, 0xa7, 0x84, 0xff, 0x85, 0xff,
    0x86, 0xff, 0x87, 0xff, 0x88, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1c, 0x40, 0xf9, 0x70, 0xf7, 0x93, 0xf4, 0xbf, 0x89, 0xff, 0x8a, 0xff, 0x8b, 0xff,
    0x8c, 0xff, 0x8d, 0xff, 0x8e, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3a, 0x50, 0xf7, 0x81, 0xf5, 0xbf, 0x8f, 0xff, 0x90, 0xff, 0x91, 0xff, 0x92, 0xff,
    0x93, 0xff, 0x94, 0xff, 0x95, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3b, 0x50, 0xf8, 0x93, 0x96, 0xff, 0x97, 0xff, 0x98, 0xff, 0x99, 0xff, 0x9a, 0xff,
    0x9b, 0xff, 0x9c, 0xff, 0x9d, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7a, 0x60, 0xf7, 0xa7, 0x9e, 0xff, 0x9f, 0xff, 0xa0, 0xff, 0xa1, 0xff, 0xa2, 0xff,
    0xa3, 0xff, 0xa4, 0xff, 0xa5, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7b, 0x60, 0xf6, 0xbf, 0xa6, 0xff, 0xa7, 0xff, 0xa8, 0xff, 0xa9, 0xff, 0xaa, 0xff,
    0xab, 0xff, 0xac, 0xff, 0xad, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xfa, 0x70, 0xf7, 0xbf, 0xae, 0xff, 0xaf, 0xff, 0xb0, 0xff, 0xb1, 0xff, 0xb2, 0xff,
    0xb3, 0xff, 0xb4, 0xff, 0xb5, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf8, 0x81, 0xc0, 0xef, 0xb6, 0xff, 0xb7, 0xff, 0xb8, 0xff, 0xb9, 0xff, 0xba, 0xff,
    0xbb, 0xff, 0xbc, 0xff, 0xbd, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf9, 0x81, 0xbe, 0xff, 0xbf, 0xff, 0xc0, 0xff, 0xc1, 0xff, 0xc2, 0xff, 0xc3, 0xff,
    0xc4, 0xff, 0xc5, 0xff, 0xc6, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xfa, 0x81, 0xc7, 0xff, 0xc8, 0xff, 0xc9, 0xff, 0xca, 0xff, 0xcb, 0xff, 0xcc, 0xff,
    0xcd, 0xff, 0xce, 0xff, 0xcf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf9, 0x93, 0xd0, 0xff, 0xd1, 0xff, 0xd2, 0xff, 0xd3, 0xff, 0xd4, 0xff, 0xd5, 0xff,
    0xd6, 0xff, 0xd7, 0xff, 0xd8, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xfa, 0x93, 0xd9, 0xff, 0xda, 0xff, 0xdb, 0xff, 0xdc, 0xff, 0xdd, 0xff, 0xde, 0xff,
    0xdf, 0xff, 0xe0, 0xff, 0xe1, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf8, 0xa7, 0xe2, 0xff, 0xe3, 0xff, 0xe4, 0xff, 0xe5, 0xff, 0xe6, 0xff, 0xe7, 0xff,
    0xe8, 0xff, 0xe9, 0xff, 0xea, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xeb, 0xff, 0xec, 0xff, 0xed, 0xff, 0xee, 0xff, 0xef, 0xff, 0xf0, 0xff, 0xf1, 0xff,
    0xf2, 0xff, 0xf3, 0xff, 0xf4, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf9, 0xa7, 0xf5, 0xff, 0xf6, 0xff, 0xf7, 0xff, 0xf8, 0xff, 0xf9, 0xff, 0xfa, 0xff, 0xfb, 0xff,
    0xfc, 0xff, 0xfd, 0xff, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // AC Huffman code table 1 //addr= 0x20600 //len=0x200
    0x0, 0x10, 0x1, 0x10, 0x4, 0x20, 0xa, 0x30, 0x18, 0x40, 0x19, 0x40, 0x38, 0x50, 0x78, 0x60,
    0xf4, 0x81, 0xf6, 0x93, 0xf4, 0xbf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0x30, 0x39, 0x50, 0xf6, 0x70, 0xf5, 0x81, 0xf6, 0xa7, 0xf5, 0xbf, 0x88, 0xff,
    0x89, 0xff, 0x8a, 0xff, 0x8b, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1a, 0x40, 0xf7, 0x70, 0xf7, 0x93, 0xf6, 0xbf, 0xc2, 0xef, 0x8c, 0xff, 0x8d, 0xff,
    0x8e, 0xff, 0x8f, 0xff, 0x90, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1b, 0x40, 0xf8, 0x70, 0xf8, 0x93, 0xf7, 0xbf, 0x91, 0xff, 0x92, 0xff, 0x93, 0xff,
    0x94, 0xff, 0x95, 0xff, 0x96, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3a, 0x50, 0xf6, 0x81, 0x97, 0xff, 0x98, 0xff, 0x99, 0xff, 0x9a, 0xff, 0x9b, 0xff,
    0x9c, 0xff, 0x9d, 0xff, 0x9e, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3b, 0x50, 0xf9, 0x93, 0x9f, 0xff, 0xa0, 0xff, 0xa1, 0xff, 0xa2, 0xff, 0xa3, 0xff,
    0xa4, 0xff, 0xa5, 0xff, 0xa6, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x79, 0x60, 0xf7, 0xa7, 0xa7, 0xff, 0xa8, 0xff, 0xa9, 0xff, 0xaa, 0xff, 0xab, 0xff,
    0xac, 0xff, 0xad, 0xff, 0xae, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7a, 0x60, 0xf8, 0xa7, 0xaf, 0xff, 0xb0, 0xff, 0xb1, 0xff, 0xb2, 0xff, 0xb3, 0xff,
    0xb4, 0xff, 0xb5, 0xff, 0xb6, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf9, 0x70, 0xb7, 0xff, 0xb8, 0xff, 0xb9, 0xff, 0xba, 0xff, 0xbb, 0xff, 0xbc, 0xff,
    0xbd, 0xff, 0xbe, 0xff, 0xbf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf7, 0x81, 0xc0, 0xff, 0xc1, 0xff, 0xc2, 0xff, 0xc3, 0xff, 0xc4, 0xff, 0xc5, 0xff,
    0xc6, 0xff, 0xc7, 0xff, 0xc8, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf8, 0x81, 0xc9, 0xff, 0xca, 0xff, 0xcb, 0xff, 0xcc, 0xff, 0xcd, 0xff, 0xce, 0xff,
    0xcf, 0xff, 0xd0, 0xff, 0xd1, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf9, 0x81, 0xd2, 0xff, 0xd3, 0xff, 0xd4, 0xff, 0xd5, 0xff, 0xd6, 0xff, 0xd7, 0xff,
    0xd8, 0xff, 0xd9, 0xff, 0xda, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xfa, 0x81, 0xdb, 0xff, 0xdc, 0xff, 0xdd, 0xff, 0xde, 0xff, 0xdf, 0xff, 0xe0, 0xff,
    0xe1, 0xff, 0xe2, 0xff, 0xe3, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf9, 0xa7, 0xe4, 0xff, 0xe5, 0xff, 0xe6, 0xff, 0xe7, 0xff, 0xe8, 0xff, 0xe9, 0xff,
    0xea, 0xff, 0xeb, 0xff, 0xec, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xe0, 0xdf, 0xed, 0xff, 0xee, 0xff, 0xef, 0xff, 0xf0, 0xff, 0xf1, 0xff, 0xf2, 0xff,
    0xf3, 0xff, 0xf4, 0xff, 0xf5, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xfa, 0x93, 0xc3, 0xef, 0xf6, 0xff, 0xf7, 0xff, 0xf8, 0xff, 0xf9, 0xff, 0xfa, 0xff, 0xfb, 0xff,
    0xfc, 0xff, 0xfd, 0xff, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // DC Huffman code table 0 //addr=0x20800  //len=32
    0x0, 0x10, 0x2, 0x20, 0x3, 0x20, 0x4, 0x20, 0x5, 0x20, 0x6, 0x20, 0xe, 0x30, 0x1e, 0x40,
    0x3e, 0x50, 0x7e, 0x60, 0xfe, 0x70, 0xfe, 0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    //  0x2, 0x20, 0x3, 0x20, 0x4, 0x20, 0x0, 0x10, 0x5, 0x20, 0x6, 0x20, 0xe, 0x30, 0x1e, 0x40,
    //  0x3e, 0x50, 0x7e, 0x60, 0xfe, 0x70, 0xfe, 0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // DC Huffman code table 1 //addr=0x20820  //len=32
    0x0, 0x10, 0x1, 0x10, 0x2, 0x10, 0x6, 0x20, 0xe, 0x30, 0x1e, 0x40, 0x3e, 0x50, 0x7e, 0x60,
    0xfe, 0x70, 0xfe, 0x81, 0xfe, 0x93, 0xfe, 0xa7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    //   0x2, 0x20, 0x0, 0x10, 0x3, 0x20, 0x4, 0x20, 0x5, 0x20, 0x6, 0x20, 0xe, 0x30, 0x1e, 0x40,
    // 0x3e, 0x50, 0x7e, 0x60, 0xfe, 0x70, 0xfe, 0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

void jpeg_table_init(uint8_t *base, uint32_t qt)
{
  uint8_t *quant_table;
  uint8_t *dataptr;
  uint32_t i;

  if (qt > 4)
    qt = 4;

  quant_table = jpeg_quant_table[qt];

  // QT0
  dataptr = base + 0x000;
  for (i = 0; i < 0x40; ++i)
    dataptr[i] = quant_table[jpeg_zigzag_table[i]];

  // QT1
  dataptr = base + 0x040;
  for (i = 0; i < 0x40; ++i)
    dataptr[i] = quant_table[jpeg_zigzag_table[i]];

  // Huffman
  dataptr = base + 0x080;
  for (i = 0; i < 0x380; ++i)
    dataptr[i] = jpeg_huffman_table[i];
  dataptr = base + 0x800;
  for (i = 0; i < 0x440; ++i)
    dataptr[i] = jpeg_huffman_table[i + 0x380];
}

////////////////////////////////////////////////////////////////////////////////
static cpu_nop_delay(uint32_t dtime)
{
  while (dtime--)
    asm("l.nop");
}

////////////////////////////////////////////////////////////////////////////////
void jpgenc_linebuf_init(void *controller, uint32_t ybase, uint32_t uvbase)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  jpreg(base, JPxELINECNT) = 0;
  jpreg(base, JPxIBAS) = ybase;
  jpreg(base, JPxIEND) = uvbase;
}

void jpgenc_outbuf_init(void *controller, uint32_t start, uint32_t end)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  jpreg(base, JPxOBAS) = start;
  jpreg(base, JPxOEND) = end | (3 << 28);
}

void jpgenc_outbuf_printf(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  printf("s:%x e:%x\n", jpreg(base, JPxOBAS), jpreg(base, JPxOEND));
}
void jpgenc_set_scaler(void *controller, uint32_t yuv_w, uint32_t yuv_h, uint32_t jpg_w, uint32_t jpg_h)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  if (yuv_w > jpg_w)
    yuv_w = jpg_w;
  if (yuv_h > jpg_h)
    yuv_h = jpg_h;

  jpreg(base, JPxSIZE1) = (yuv_w << 16) | yuv_h;
  jpreg(base, JPxSIZE0) = (jpg_w << 16) | jpg_h;

  jpreg(base, JPxXSCALE) =
      ((yuv_w / jpg_w) << 16) |                                               // 整數部分
      (uint16_t)((yuv_w << 16) / jpg_w + (((yuv_w << 16) % jpg_w) ? 1 : 0)) | // 小數部分
      (0 << 18);                                                              // div n. 編碼時不需要
  jpreg(base, JPxYSCALE) =
      ((yuv_h / jpg_h) << 16) |                                               // 整數部分
      (uint16_t)((yuv_h << 16) / jpg_h + (((yuv_h << 16) % jpg_h) ? 1 : 0)) | // 小數部分
      (0 << 18);                                                              // div n. 編碼時不需要
}

uint32_t jpgenc_last_addr(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  return jpreg(base, JPxOLTA);
}

void jpgenc_mode_start(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  *jp_reg(base, JPxCON1) |= BIT(8); // JERUN
  *jp_reg(base, JPxELINECNT) = 8;   // why firstly set 8 ???
  *jp_reg(base, JPxELINECNT) = 800; // why not the true line-count?
}

void jpgenc_mode_stop(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  pREG con1 = jp_reg(base, JPxCON1);

  //*jp_reg(base, JPxCON1) |= BIT(9); // JESTOP
  *con1 |= BIT(9);
  *jp_reg(base, JPxINTCON) &= ~BIT(11); // why timeinfo ?
  //*jp_reg(base, JPxCON1) |= BIT(9); // JESTOP
  *con1 |= BIT(9);
  *jp_reg(base, JPxFLG_CLR) = ~0;
}

void jpeg_jbuferr_deal()
{
  unsigned long flag;
flag=arch_local_irq_save();

  CSI_DMA_MODE &= ~(1 << 19);
  PRE_JPCON1 |= BIT(9);
  PRE_JPCON1 |= BIT(8);
  CSI_DMA_MODE |= (1 << 19);
  arch_local_irq_restore(flag);
}
#define sfr_reg(b, o) *(volatile __sfr unsigned int *)(base + o * 4)
void sfr_array_error_test(unsigned int *pbase)
{
  unsigned int base = *pbase;
  sfr_reg(base, 0) |= (1 << 8);
  sfr_reg(base, 41) = 8;
  sfr_reg(base, 41) = 800;
}

static int jpg_reset_end = 0;
void jpgModeReset(void)
{
	RSTCON &= ~BIT(0); // reset mjpeg modlule
    cpu_nop_delay(10);
    RSTCON |= BIT(0);
}
void jpgenc_mode_init(void *controller, uint32_t qt, int manual)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  // todo : 兩個jpeg模塊不能分開reset，這段代碼應該拉出去
  if (!jpg_reset_end)
  {
    RSTCON &= ~BIT(0); // reset mjpeg modlule
    cpu_nop_delay(10);
    RSTCON |= BIT(0);
    jpg_reset_end = 1;
  }

  PCON0 &= ~BIT(jpc->clken);

  jpeg_table_init(jpc->table_base, qt);

  // TODO : TABLE INIT

  /*
  jpreg(base, JPxBSCCOEF0) = CCOEF0;
  jpreg(base, JPxBSCCOEF1) = CCOEF1;
  jpreg(base, JPxBSCCOEF2) = CCOEF2;
  jpreg(base, JPxBSCCOEF3) = CCOEF3;
  jpreg(base, JPxBSCCOEF4) = CCOEF4;
  jpreg(base, JPxBSCCOEF5) = CCOEF5;
  jpreg(base, JPxBSCCOEF6) = CCOEF6;
  jpreg(base, JPxBSCCOEF7) = CCOEF7;
  */

  jpreg(base, JPxBSCCOEF0) = BIC_TAB00;
  jpreg(base, JPxBSCCOEF1) = BIC_TAB01;
  jpreg(base, JPxBSCCOEF2) = BIC_TAB02;
  jpreg(base, JPxBSCCOEF3) = BIC_TAB03;
  jpreg(base, JPxBSCCOEF4) = BIC_TAB04;
  jpreg(base, JPxBSCCOEF5) = BIC_TAB05;
  jpreg(base, JPxBSCCOEF6) = BIC_TAB06;
  jpreg(base, JPxBSCCOEF7) = BIC_TAB07;
  jpreg(base, JPxBSCCOEF10) = BIC_TAB10;
  jpreg(base, JPxBSCCOEF11) = BIC_TAB11;
  jpreg(base, JPxBSCCOEF12) = BIC_TAB12;
  jpreg(base, JPxBSCCOEF13) = BIC_TAB13;
  jpreg(base, JPxBSCCOEF14) = BIC_TAB14;
  jpreg(base, JPxBSCCOEF15) = BIC_TAB15;
  jpreg(base, JPxBSCCOEF16) = BIC_TAB16;
  jpreg(base, JPxBSCCOEF17) = BIC_TAB17;

  jpreg(base, LINE_JPxBSCCOEF0) = BIC_LINETAB00;
  jpreg(base, LINE_JPxBSCCOEF1) = BIC_LINETAB01;
  jpreg(base, LINE_JPxBSCCOEF2) = BIC_LINETAB02;
  jpreg(base, LINE_JPxBSCCOEF3) = BIC_LINETAB03;
  jpreg(base, LINE_JPxBSCCOEF4) = BIC_LINETAB04;
  jpreg(base, LINE_JPxBSCCOEF5) = BIC_LINETAB05;
  jpreg(base, LINE_JPxBSCCOEF6) = BIC_LINETAB06;
  jpreg(base, LINE_JPxBSCCOEF7) = BIC_LINETAB07;

  jpreg(base, UVJPxBSCCOEF0) = BIC_UVTAB00;
  jpreg(base, UVJPxBSCCOEF1) = BIC_UVTAB01;
  jpreg(base, UVJPxBSCCOEF2) = BIC_UVTAB02;
  jpreg(base, UVJPxBSCCOEF3) = BIC_UVTAB03;
  jpreg(base, UVJPxBSCCOEF4) = BIC_UVTAB04;
  jpreg(base, UVJPxBSCCOEF5) = BIC_UVTAB05;
  jpreg(base, UVJPxBSCCOEF6) = BIC_UVTAB06;
  jpreg(base, UVJPxBSCCOEF7) = BIC_UVTAB07;
  jpreg(base, UVJPxBSCCOEF10) = BIC_UVTAB10;
  jpreg(base, UVJPxBSCCOEF11) = BIC_UVTAB11;
  jpreg(base, UVJPxBSCCOEF12) = BIC_UVTAB12;
  jpreg(base, UVJPxBSCCOEF13) = BIC_UVTAB13;
  jpreg(base, UVJPxBSCCOEF14) = BIC_UVTAB14;
  jpreg(base, UVJPxBSCCOEF15) = BIC_UVTAB15;
  jpreg(base, UVJPxBSCCOEF16) = BIC_UVTAB16;
  jpreg(base, UVJPxBSCCOEF17) = BIC_UVTAB17;

  uint32_t jpXcon1 = jpreg(base, JPxCON1) & ~(0xF << 23) & ~(0x3 << 19) & ~(BIT(10));
  jpXcon1 |=
      BIT(13) | // enchold_en
      BIT(22) | // dctidct
      BIT(0) |  // jeodmaen
      BIT(11) | // JESTSYNCEN
      BIT(14) | // WR_CRITICAL_EN
      BIT(15) | // RD_CRITICAL_EN
      (manual << 10);
  jpreg(base, JPxCON1) = jpXcon1;
  jpreg(base, JPxINTCON) = BIT(31)  // JPEG_INT_EN
                           | BIT(6) // FRAMEDONE_INT_EN
                           | BIT(4) // JEOLDARD_INT_EN
                           //|BIT(JEO_FULL_INT_EN)
                           | BIT(2) // DMA_OBERROR_INT_EN
                           | BIT(0) // JPO_DMA_INT_EN
      //|BIT(11)//JPTINF0_INTEN
      //|BIT(10)//JPTINF1_INTEN
      //|BIT(9)//JPTINF2_INTEN
      ;
  /*jpCON1 BIT 10
   * 0:DMA input addr using csi internal
   * 1:DMA input addr using user setting just JPIBAS/JPIEND
   */
  jpreg(base, JPxFLG_CLR) = ~0;
  jpreg(base, JPxHCNT) = (768 << 16) | (600);
}
void jpgenc_quality_set(void *controller, uint32_t quality)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  jpreg(base, JPxCON1) = (jpreg(base, JPxCON1) & ~(0xF << 23)) | (quality << 23);
}
uint32_t jpgenc_quality_get(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  return (jpreg(base, JPxCON1)>>23)&0xF;
}

void jpgenc_quality_adj(void *controller,u32 jsize,u32 min_size,u32 max_size)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  //static s8 tq;
  if(jsize==0)
  	return ;
  s8 q = (jpreg(base, JPxCON1) >> 23) & 0x0f;
  if(q == 0)
  	q = 8;
  if(jsize > max_size)
  	q++;
  if(jsize < min_size)
  	q--;
  if(q < 0x03)
  	q=0x03;
  if(q > 0x0f)
  	q=0x0f;
  jpreg(base, JPxCON1) = (jpreg(base, JPxCON1) & ~(0xF << 23)) | (q << 23);
}
u8 jpgenc_quality_adj1(void *controller,u32 jsize,u32 min_size,u32 max_size,u8 q)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  //static s8 tq;
  if(jsize==0)
  	return ;
  if(q == 0)
  	q = 8;
  if(jsize > max_size)
  	q++;
  if(jsize < min_size)
  	q--;
  if(q < 0x03)
  	q=0x03;
  if(q > 0x0f)
  	q=0x0f;
  jpreg(base, JPxCON1) = (jpreg(base, JPxCON1) & ~(0xF << 23)) | (q << 23);
  return q;
}

void jpgenc_extra_hdr8(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  jpreg(base, JPxCON1) = (jpreg(base, JPxCON1) & ~(0x3 << 19)) | (1 << 19);
}

////////////////////////////////////////////////////////////////////////////////
void jpgdec_inbuf_init(void *controller, uint32_t start, uint32_t end, int cont, uint32_t end_frame)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  if (end_frame == 1)
  {
    end = 0;
  }

  if (cont)
  {
    jpreg(base, JPxIBAS) = start;
    jpreg(base, JPxIEND) = end | BIT(29);
    return;
  }
  // 按Byte對齊的Buffer，要用按4-Byte對齊的方式配置JPxIBAS，但JPxUBIT可以設置32-bit的起始
  jpreg(base, JPxIBAS) = start & ~3;
  jpreg(base, JPxIEND) = end | BIT(29);
  jpreg(base, JPxUBIT) = (start & 3) * 8; // 一個Byte是8-bit
}

void jpgdec_outbuf_init(void *controller, uint32_t start, uint32_t end)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  jpreg(base, JPxOBAS) = start;
  jpreg(base, JPxOEND) = end;
}

void jpgdec_mode_start(void *controller,u8 yuv422)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  jpreg(base, JPxFLG_CLR) = BIT(8) | BIT(5);

  jpreg(base, JPxCON1) |=
      (1 << 29) | // decoder
      (0 << 28) | // encoder
      (0 << 27) | // encoder
      (0 << 23) | // encoder quantize select
      (0 << 22) | // codec DCT/IDCT select, 0:IDCT
      (0 << 19) | // encoder add 8/28 byte AVI chunk header before jpg header
      (0 << 18) | // encoder generate JPEG head en
      (0 << 17) | // encoder DRI enable
      ((!!yuv422)<<16) | // decoder yuv420/yuv422 select
      (0 << 15) | // dma read critical en
      (0 << 14) | // dma write critical en
      (0 << 13) | // encoder hold en
      (0 << 12) | // encoder
      (0 << 11) | // encoder sync to csi frame start
      (0 << 10) | // encoder manual mode en
      (0 << 9) |  // encoder stop and reset
      (0 << 8) |  // encoder start
      (0 << 7) |  // decoder stop and reset
      (1 << 6) |  // decoder start
      (1 << 5) |  // decoder end-marker select
      (0 << 4) |  // encoder output auto reload en
      (0 << 3) |  // encoder
      (0 << 2) |  // decoder output DMA (no block-scaler) en <! JUST FOR BACK_JPEG>
      (1 << 1) |  // input DMA en
      (1 << 0);   // output DMA en
}

void jpgdec_mode_stop(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  jpreg(base, JPxCON1) |= BIT(7);
  jpreg(base, JPxCON1) = 0;

  jpreg(base, JPxFLG_CLR) =

      (1 << 13) | // time-info
      (1 << 12) | // time-info
      (1 << 11) | // time-info
      (1 << 10) | // time-info
      (1 << 9) |  // time-info
      (1 << 8) |  // decoder huffman err
      (1 << 7) |  // encoder
      (1 << 6) |  // codec frame done
      (1 << 5) |  // decoder DRI err
      (1 << 4) |  // encoder
      (1 << 3) |  // output buffer err
      (1 << 2) |  // decoder DMA (no block-scaler) frame done
      (0 << 1) |  // decoder input dma resume en
      (0 << 0);   // encoder output dma resume en

  jpreg(base, JPxFLG_CLR) = ~0;
}

void jpgdec_mode_init(void *controller, uint32_t yuvfmt)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  // todo : 兩個jpeg模塊不能分開reset，這段代碼應該拉出去
  if (!jpg_reset_end)
  {
    RSTCON &= ~BIT(0); // reset mjpeg modlule
    cpu_nop_delay(10);
    RSTCON |= BIT(0);
    jpg_reset_end = 1;
  }

  PCON0 &= ~BIT(jpc->clken);

  jpreg(base, JPxCON1) =   //(jpreg(base, JPxCON1) & ~(1<<16)) |
      (0 << 30) |          // deblock en
      (1 << 29) |          // decoder
      (0 << 28) |          // encoder
      (0 << 27) |          // encoder
      (0 << 23) |          // encoder quantize select
      (0 << 22) |          // codec DCT/IDCT select, 0:IDCT
      (0 << 19) |          // encoder add 8/28 byte AVI chunk header before jpg header
      (0 << 18) |          // encoder generate JPEG head en
      (0 << 17) |          // encoder DRI enable
      ((!!yuvfmt) << 16) | // decoder yuv420/yuv422 select
      (0 << 15) |          // dma read critical en
      (0 << 14) |          // dma write critical en
      (0 << 13) |          // encoder hold en
      (0 << 12) |          // encoder
      (0 << 11) |          // encoder sync to csi frame start
      (0 << 10) |          // encoder manual mode en
      (1 << 9) |           // encoder stop and reset
      (0 << 8) |           // encoder start
      (0 << 7) |           // decoder stop and reset
      (0 << 6) |           // decoder start
      (1 << 5) |           // decoder end-marker select
      (0 << 4) |           // encoder output auto reload en
      (0 << 3) |           // encoder
      (0 << 2) |           // decoder output DMA (no block-scaler) en <! JUST FOR BACK_JPEG>
      (1 << 1) |           // input DMA en
      (1 << 0);            // output DMA en

  jpreg(base, JPxIPTR) = 0;
  jpreg(base, JPxINTCON) =
      (1 << 31) | // codec jpeg int en
      (1 << 12) | // time-info
      (0 << 11) | // time-info
      (0 << 10) | // time-info
      (0 << 9) |  // time-info
      (1 << 8) |  // decoder huffman err int en
      (0 << 7) |  // encoder
      (1 << 6) |  // codec frame done int en
      (1 << 5) |  // decoder DRI err int en
      (0 << 4) |  // encoder reload output addr int en
      (0 << 3) |  // output buffer error int en
      (0 << 2) |  // decoder DMA (no block-scaler) frame done int en
      (1 << 1) |  // decode DMA input int en, input one packet, one interrupt
      (0 << 0);   // encod DMA output int en, output one packet, one interrupt

  jpreg(base, JPxFLG_CLR) =
      (1 << 11) | // time-info
      (1 << 10) | // time-info
      (1 << 9) |  // time-info
      (1 << 8) |  // decoder huffman err
      (1 << 7) |  // encoder
      (1 << 6) |  // codec frame done
      (1 << 5) |  // decoder DRI err
      (1 << 4) |  // encoder
      (1 << 3) |  // output buffer err
      (1 << 2) |  // decoder DMA (no block-scaler) frame done
      (0 << 1) |  // decoder input dma resume en
      (0 << 0);   // encoder output dma resume en

  jpreg(base, JPxBSCCOEF0) = BIC_TAB00;
  jpreg(base, JPxBSCCOEF1) = BIC_TAB01;
  jpreg(base, JPxBSCCOEF2) = BIC_TAB02;
  jpreg(base, JPxBSCCOEF3) = BIC_TAB03;
  jpreg(base, JPxBSCCOEF4) = BIC_TAB04;
  jpreg(base, JPxBSCCOEF5) = BIC_TAB05;
  jpreg(base, JPxBSCCOEF6) = BIC_TAB06;
  jpreg(base, JPxBSCCOEF7) = BIC_TAB07;
  jpreg(base, JPxBSCCOEF10) = BIC_TAB10;
  jpreg(base, JPxBSCCOEF11) = BIC_TAB11;
  jpreg(base, JPxBSCCOEF12) = BIC_TAB12;
  jpreg(base, JPxBSCCOEF13) = BIC_TAB13;
  jpreg(base, JPxBSCCOEF14) = BIC_TAB14;
  jpreg(base, JPxBSCCOEF15) = BIC_TAB15;
  jpreg(base, JPxBSCCOEF16) = BIC_TAB16;
  jpreg(base, JPxBSCCOEF17) = BIC_TAB17;

  jpreg(base, LINE_JPxBSCCOEF0) = BIC_LINETAB00;
  jpreg(base, LINE_JPxBSCCOEF1) = BIC_LINETAB01;
  jpreg(base, LINE_JPxBSCCOEF2) = BIC_LINETAB02;
  jpreg(base, LINE_JPxBSCCOEF3) = BIC_LINETAB03;
  jpreg(base, LINE_JPxBSCCOEF4) = BIC_LINETAB04;
  jpreg(base, LINE_JPxBSCCOEF5) = BIC_LINETAB05;
  jpreg(base, LINE_JPxBSCCOEF6) = BIC_LINETAB06;
  jpreg(base, LINE_JPxBSCCOEF7) = BIC_LINETAB07;

  jpreg(base, UVJPxBSCCOEF0) = BIC_UVTAB00;
  jpreg(base, UVJPxBSCCOEF1) = BIC_UVTAB01;
  jpreg(base, UVJPxBSCCOEF2) = BIC_UVTAB02;
  jpreg(base, UVJPxBSCCOEF3) = BIC_UVTAB03;
  jpreg(base, UVJPxBSCCOEF4) = BIC_UVTAB04;
  jpreg(base, UVJPxBSCCOEF5) = BIC_UVTAB05;
  jpreg(base, UVJPxBSCCOEF6) = BIC_UVTAB06;
  jpreg(base, UVJPxBSCCOEF7) = BIC_UVTAB07;
  jpreg(base, UVJPxBSCCOEF10) = BIC_UVTAB10;
  jpreg(base, UVJPxBSCCOEF11) = BIC_UVTAB11;
  jpreg(base, UVJPxBSCCOEF12) = BIC_UVTAB12;
  jpreg(base, UVJPxBSCCOEF13) = BIC_UVTAB13;
  jpreg(base, UVJPxBSCCOEF14) = BIC_UVTAB14;
  jpreg(base, UVJPxBSCCOEF15) = BIC_UVTAB15;
  jpreg(base, UVJPxBSCCOEF16) = BIC_UVTAB16;
  jpreg(base, UVJPxBSCCOEF17) = BIC_UVTAB17;

  jpreg(base, JPxCON1_DEBLOCK) = ((c_table_chroma[INDEX_CHROMA] & 0xf) << 28) |      // c_chroma
                                 ((c_table_luma[INDEX_LUMA] & 0xf) << 24) |          // c_luma
                                 ((belta_table_chroma[INDEX_CHROMA] & 0x3f) << 18) | // belta_chroma
                                 ((alpha_table_chroma[INDEX_CHROMA] & 0x3f) << 12) | // alpha_chroma
                                 ((belta_table_luma[INDEX_LUMA] & 0x3f) << 6) |      // belta_luma
                                 (alpha_table_luma[INDEX_LUMA] & 0x3f);              // alpha_luma
}

/*******************************************************************************
 * Function Name  : ax32xx_mjpegDecodeDCTimeSet
 * Description    : jpeg decode dc time over set
 * Input          : u8 en,u32 src_w,u32 src_h
 * Output         : none
 * Return         : none
 *******************************************************************************/
#define COMBO_CLOCK 96000000 // 48000000

void jpgdec_DecodeDCTimeSet(void *controller, u8 en, u32 src_w, u32 src_h)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  if (
      !en)
  {
    jpreg(base, JPxINTCON) &= ~(1 << 13);       // dc time over int dis
    jpreg(base, JPxCON1_DCTIMER) &= ~(1 << 31); // DC TIMER CHECK dis
    return;
  }

  int time_count = 0; // ms
  if (src_h <= 720)
  {
    time_count = 100*2;
  }
  else if (src_h <= 1080)
  {
    time_count = 200*2;
  }
  else if (src_h <= 2160)
  {
    time_count = 400*2;
  }
  else
  {
    time_count = 600*2;
  }

  if (en)
  {
    // R_MJPEG_INTEN |= (1<<13) ; //dc time over int en
    jpreg(base, JPxINTCON) |= (1 << 13);
    jpreg(base, JPxCON1_DCTIMER) = (1 << 31) |                                                  // DC TIMER CHECK EN
                                   (((time_count * (COMBO_CLOCK / 1000)) & (0x7fffffff)) << 0); // dc timer count : 45ms
  }
}

void printDecode(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  /**********************
  jpreg(base, JPxCON2)   = jpcon2;
  jpreg(base, JPxSIZE0)  = jpsize0;
  jpreg(base, JPxSIZE1)  = jpsize1;
  jpreg(base, JPxXSCALE) = jpxscale;
  jpreg(base, JPxYSCALE) = jpyscale;

  ***********************/
  printf("JPxCON1:%X\r\n", jpreg(base, JPxCON1));
  printf("JPxIPTR:%X\r\n", jpreg(base, JPxIPTR));
  printf("JPxINTCON:%X\r\n", jpreg(base, JPxINTCON));
  printf("JPxFLG_CLR:%X\r\n", jpreg(base, JPxFLG_CLR));

  printf("JPxCON2:%X\r\n", jpreg(base, JPxCON2));
  printf("JPxSIZE0:%X\r\n", jpreg(base, JPxSIZE0));
  printf("JPxSIZE1:%X\r\n", jpreg(base, JPxSIZE1));
  printf("JPxXSCALE:%X\r\n", jpreg(base, JPxXSCALE));
  printf("JPxYSCALE:%X\r\n", jpreg(base, JPxYSCALE));

  printf("JPxBSCCOEF0:%X\r\n", jpreg(base, JPxBSCCOEF0));
  printf("JPxBSCCOEF1:%X\r\n", jpreg(base, JPxBSCCOEF1));
  printf("JPxBSCCOEF2:%X\r\n", jpreg(base, JPxBSCCOEF2));
  printf("JPxBSCCOEF3:%X\r\n", jpreg(base, JPxBSCCOEF3));
  printf("JPxBSCCOEF4:%X\r\n", jpreg(base, JPxBSCCOEF4));
  printf("JPxBSCCOEF5:%X\r\n", jpreg(base, JPxBSCCOEF5));
  printf("JPxBSCCOEF6:%X\r\n", jpreg(base, JPxBSCCOEF6));
  printf("JPxBSCCOEF7:%X\r\n", jpreg(base, JPxBSCCOEF7));

  printf("JPxDRI:%X\r\n", jpreg(base, JPxDRI));
  printf("JPxCOM:%X\r\n", jpreg(base, JPxCOM)); // jpc->table_base

  printf("jpc->table_base:%X\r\n", jpc->table_base);
}

void ax32xx_mpegPrint(void)
{
#define debgreg(x) printf(#x "=%X\n", x)
  debgreg(BACK_JPCON1);
  debgreg(BACK_JPCON2);
  debgreg(BACK_JPCON3);
  debgreg(BACK_JPSIZE0);
  debgreg(BACK_JPOBAS);
  debgreg(BACK_JPOYADR);
  debgreg(BACK_JPOEND);
  debgreg(BACK_JPOUVADR);
  debgreg(BACK_JPOPTR);
  debgreg(BACK_JPOLTA);
  debgreg(BACK_JPIBAS);
  debgreg(BACK_JPIYADR);
  debgreg(BACK_JPIEND);
  debgreg(BACK_JPIUVADR);
  debgreg(BACK_JPIPTR);
  debgreg(BACK_JPUBIT);
  debgreg(BACK_JPHCNT);
  debgreg(BACK_JPDRI);
  debgreg(BACK_JPBSHADDR);
  debgreg(BACK_JPINTCON);
  debgreg(BACK_JPFLG);
  debgreg(BACK_JPLS);
  debgreg(BACK_JPES);
  debgreg(BACK_JPCOM);
  debgreg(BACK_JPTINF);
  debgreg(BACK_JPT0SIZE);
  debgreg(BACK_JPT1SIZE);
  debgreg(BACK_JPT2SIZE);
  debgreg(BACK_JPT0POS);
  debgreg(BACK_JPT1POS);
  debgreg(BACK_JPT2POS);
  debgreg(BACK_JPT0BASE);
  debgreg(BACK_JPT1BASE);
  debgreg(BACK_JPT2BASE);
  debgreg(BACK_JPSIZE1);
  debgreg(BACK_JPXSCALE);
  debgreg(BACK_JPYSCALE);
  debgreg(BACK_JPDCXCROP);
  debgreg(BACK_JPDCYCROP);
  debgreg(BACK_JPFLG_CLR);
  debgreg(BACK_JPPAUSECNT);
  debgreg(BACK_JPDCYBASE);
  debgreg(BACK_JPDCUVBASE);
  debgreg(BACK_JPDMAOUTX32);
}

s32 ax32xx_mjpegDecodeScalerCal(u32 jpg_w, u32 yuv_w)
{
  const u8 pre_xscale_table[] = {0, 0, 1, 2, 2};
  const u8 pre_xscale_table1[] = {0, 1, 2, 2, 3, 3, 3, 3, 3};
  u32 res = 0;
  u32 prescale0 = 0, prescale1 = 0, temp0 = jpg_w, dst_w4 = yuv_w * 4;

  if (jpg_w > 1920)
  {
    prescale0 = pre_xscale_table[(jpg_w + 1920 - 1) / 1920];
    temp0 = (jpg_w + (1 << prescale0) - 1) / (1 << prescale0);
  }

  if (temp0 >= dst_w4)
  {
    prescale1 = pre_xscale_table1[temp0 / dst_w4];
  }

  temp0 = (prescale0 + prescale1);
  res = (temp0 << 16) | ((jpg_w + (1 << temp0) - 1) / (1 << temp0));

  return res;
}
/*******************************************************************************
 * Function Name  : ax32xx_mjpegDecodeSetSize
 * Description    : jpeg decode size set
 * Input          : u32 jpg_w : source width
 *                  u32 jpg_h : source height
 *                  u32 yuv_w : target width
 *                  u32 yuv_h : target height
 *                  u32 stride: 0:disable stride function
 *                             -1:don't change stride
 *                              other:target stride,for dma0
 * Output         : none
 * Return         : none
 *******************************************************************************/
int32_t jpgdec_set_scaler(void *controller, uint32_t jpg_w, uint32_t jpg_h, uint32_t yuv_w, uint32_t yuv_h, uint32_t stride)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  static u32 sum0 = 0;
  static u32 jpcon2, jpsize0, jpsize1, jpxscale, jpyscale;

  u32 pre_xscale, scale_jpg_width;
  u32 xi, xf, yi, yf;

  if (jpg_w > 4032)
    return -1;

  // 避免重复计算
  u32 sum = jpg_w + jpg_h + yuv_w + yuv_h;
  if (sum0 != sum)
  {
    sum0 = sum;
    // 避免prescale
    if ((jpg_w / 4) == yuv_w)
    {
      // 保存原始尺寸
      jpcon2 = (1 << 28) |
               (jpg_w << 16) |
               (jpg_h << 0);
      jpg_w -= 2;
    }
    else
      jpcon2 = 0;

    pre_xscale = ax32xx_mjpegDecodeScalerCal(jpg_w, yuv_w);
    scale_jpg_width = pre_xscale & 0xffff;
    xi = scale_jpg_width / yuv_w;
    xf = (((scale_jpg_width % yuv_w) << 16) + yuv_w - 1) / yuv_w;
    yi = jpg_h / yuv_h;
    yf = (((jpg_h % yuv_h) << 16) + yuv_h - 1) / yuv_h;

    pre_xscale >>= 16;

    jpsize0 = (jpg_w << 16) | // decoder src width
              (jpg_h << 0);   // decoder src height

    jpsize1 = (yuv_w << 16) | // decoder dst width
              (yuv_h << 0);   // decoder dst height

    jpxscale = (pre_xscale << 18) | // x scale div
               (xi << 16) |         // integrate scale
               (xf << 0);           // fractional of scale

    jpyscale = (yi << 16) | // integrate scale
               (yf << 0);   // fractional of scale
  }
  {
    jpreg(base, JPxCON2) = jpcon2;
    jpreg(base, JPxSIZE0) = jpsize0;
    jpreg(base, JPxSIZE1) = jpsize1;
    jpreg(base, JPxXSCALE) = jpxscale;
    jpreg(base, JPxYSCALE) = jpyscale;
  }

  if (stride != 0xffffffff)
  {
    //    	printf("stride:%x\n",stride);
    jpreg(base, JPxDMAOUTX32) = (jpg_w << 16) |      // DMA1 stride
                                ((!!stride) << 12) | // decoder yuv420 manual stride en
                                (stride << 0);       // decoder stride of yuv420 output
  }
  return 0;
}

static void jpgdec_table_copy(uint8_t *dst, uint8_t *src, int len)
{
  while (len--)
    *dst++ = *src++;
}

void jpgdec_copy_dqt(void *controller, uint32_t idx, uint8_t *table)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  uint8_t *dqt = jpc->table_base + DQTOFF + DQTSIZE * idx;
  jpgdec_table_copy(dqt, table, DQTSIZE);
}

void jpgdec_copy_dht(void *controller, uint32_t idx, uint8_t *table)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  uint8_t *valptr, *dht;
  uint8_t *mincode;
  int len;
  int counter, i;
  uint8_t order, c;
  uint16_t code, mc;

  if (idx == 0)
    dht = jpc->table_base + HUFFDCOFF + HUFFDCSIZE * 0;
  if (idx == 1)
    dht = jpc->table_base + HUFFDCOFF + HUFFDCSIZE * 1;
  if (idx == 2)
    dht = jpc->table_base + HUFFACOFF + HUFFACSIZE * 0;
  if (idx == 3)
    dht = jpc->table_base + HUFFACOFF + HUFFACSIZE * 1;
  mincode = jpc->table_base + MINCODEOFF + MINCODESIZE * idx;
  valptr = jpc->table_base + VALPTROFF + VALPTRSIZE * idx;
  len = (idx & 2) ? HUFFACSIZE : HUFFDCSIZE;

  counter = 0;
  code = 0;
  for (i = 0, order = 15; i < 16; ++i)
  {
    c = *table++;
    mc = code >> order;
    mincode[i * 2 + 0] = (mc >> 0) & 0xFF;
    mincode[i * 2 + 1] = (mc >> 8) & 0xFF;
    valptr[i] = counter;
    counter += c;
    code += c << order;
    --order;
  }
  jpgdec_table_copy(dht, table, len);
}

void jpgdec_set_dri(void *controller, uint32_t dri)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  jpreg(base, JPxDRI) = dri;
}

void jpgdec_code_component(void *controller, uint32_t id, uint32_t dc_table, uint32_t ac_table, uint32_t qt, uint32_t blocks_per_mcu_h, uint32_t blocks_per_mcu_v)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  uint32_t component = jpc->component & ~(0x3FF << (id * 10));
  uint32_t code =
      (ac_table << 9) |
      (dc_table << 8) |
      (qt << 6) |
      (blocks_per_mcu_v << 3) |
      (blocks_per_mcu_h << 0);
  component |= code << (id * 10);
  jpc->component = component;
}

void jpgdec_set_component_code(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  // printf ("COMPONENT CODE %08X\r\n", jpc->component);
  jpreg(base, JPxCOM) = jpc->component;
}

static void jpgdec_dump_dqt(void *controller, uint32_t idx)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  uint8_t *dqt;
  int i;

  dqt = jpc->table_base + DQTOFF + DQTSIZE * idx;
  printf("DQT %d @%08X\r\n", idx, dqt);
  for (i = 0; i < DQTSIZE; ++i)
  {
    printf(" %02X", dqt[i]);
    if ((i & 7) == 7)
      printf("\r\n");
  }
}

void jpegdec_eprintf()
{
  printf("BACK_JPIPTR:%x  %x\n", BACK_JPIPTR, BACK_JPINTCON);
}

static void jpgdec_dump_dht(void *controller, uint32_t idx)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  uint8_t *dht, *valptr;
  uint8_t *mincode;
  int i, len;
  if (idx == 0)
    dht = jpc->table_base + HUFFDCOFF + HUFFDCSIZE * 0;
  if (idx == 1)
    dht = jpc->table_base + HUFFDCOFF + HUFFDCSIZE * 1;
  if (idx == 2)
    dht = jpc->table_base + HUFFACOFF + HUFFACSIZE * 0;
  if (idx == 3)
    dht = jpc->table_base + HUFFACOFF + HUFFACSIZE * 1;
  mincode = jpc->table_base + MINCODEOFF + MINCODESIZE * idx;
  valptr = jpc->table_base + VALPTROFF + VALPTRSIZE * idx;
  len = (idx & 2) ? HUFFACSIZE : HUFFDCSIZE;

  printf("DHT %d @%08X %08X %08X\r\n mincode ", idx, mincode, valptr, dht);
  for (i = 0; i < 16; ++i)
    printf("%02X%02X ", mincode[i * 2 + 1], mincode[i * 2 + 0]);
  printf("\r\n  valptr ");
  for (i = 0; i < 16; ++i)
    printf("  %02X ", valptr[i]);
  printf("\r\n");
  for (i = 0; i < len; ++i)
  {
    printf(" %02X", dht[i]);
    if ((i & 15) == 15)
      printf("\r\n");
  }
  printf("\r\n");
}

void jpgdec_dump_table(void *controller)
{
  jpgdec_dump_dqt(controller, 0);
  jpgdec_dump_dqt(controller, 1);
  jpgdec_dump_dht(controller, 0);
  jpgdec_dump_dht(controller, 1);
  jpgdec_dump_dht(controller, 2);
  jpgdec_dump_dht(controller, 3);
}

////////////////////////////////////////////////////////////////////////////////
static int jpeg_isr(uint32_t irq, void *arg, void *reg);
extern void *jpg_rtp;
extern void *jpg_rec;
void jpgirq_register(void *controller, uint32_t irq, isr_func isr, void *dev_id)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);

  //irq_register(jpc->irqno, 0, jpeg_isr, jpc); // todo : 可以在此檢查是否被註冊成不同的controller

  jpc->isr[irq] = isr;
 #if 0
  if (jpg_rtp == dev_id)
  { // 第0号stream注册为rtp的
    jpc->dev[0][irq] = dev_id;
    jpc->jpeg_num = 0;
    printf("jpc rtp irq:%d\tnum:%d\n", irq, jpc->jpeg_num);
  }
  else // 第1号stream注册为rec的
  {
    jpc->dev[1][irq] = dev_id;
    jpc->jpeg_num = 1;
    printf("jpc rec irq:%d\tnum:%d\n", irq, jpc->jpeg_num);
  }
#else
  jpc->dev[0][irq] = dev_id;
  jpc->jpeg_num = 0;
#endif
  jpreg(base, JPxINTCON) |= BIT(irq) | 0x80000000;
  //printf("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, irq, isr);
}

void jpgirq_unregister(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  irq_unregister(jpc->irqno);
}
void jpgirqRegister(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  irq_register(jpc->irqno, 0, jpeg_isr, jpc);
}

void jpgirq_register_frame_done(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_FRAME_DONE, isr, dev_id);
}

void jpgirq_register_out_pack_full(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_OUT_PACK_FULL, isr, dev_id);
}

void jpgirq_register_data_err(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_DATA_ERR, isr, dev_id);
}

void jpgirq_register_in_pack_empty(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_IN_PACK_EMPTY, isr, dev_id);
}

void jpgirq_register_dec_huff_err(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_DEC_HUFF_ERR, isr, dev_id);
}

void jpgirq_register_JDRST(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_JDRST, isr, dev_id);
}

void jpgirq_register_DCTIMEROUT(void *controller, isr_func isr, void *dev_id)
{
  jpgirq_register(controller, JPCIRQ_DEC_DCTIMEROUT, isr, dev_id);
}

////////////////////////////////////////////////////////////////////////////////

#define jr(x) jpreg(base, JPx##x)
void jpg_dumpreg(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  printf("irqno:%d\n", jpc->irqno);
  printf("JPG\n INTCON = %08X  CON1   = %08X\n", jr(INTCON), jr(CON1));
  printf(" SIZE0  = %08X  SIZE1  = %08X\n", jr(SIZE0), jr(SIZE1));
  printf(" OBAS   = %08X  OEND   = %08X\n", jr(OBAS), jr(OEND));
  printf(" OPTR   = %08X  OLTA   = %08X\n", jr(OPTR), jr(OLTA));
  printf(" IBAS   = %08X  IEND   = %08X\n", jr(IBAS), jr(IEND));
  printf(" IPTR   = %08X  UBIT   = %08X\n", jr(IPTR), jr(UBIT));
  printf(" DRI    = %08X  BSHA   = %08X\n", jr(DRI), jr(BSHADDR));
  printf(" XSCALE = %08X  YSCALE = %08X\n", jr(XSCALE), jr(YSCALE));
  printf(" COM    = %08X  INTFLG = %08X\n", jr(COM), jr(FLG));
  printf(" CON2    = %08X\n", jr(CON2));
  printf(" PICMR = %08X\n", SPR_PICMR);
}

void jpg_dumpreg2(void *controller)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  printf(" IBAS   = %08X  IEND   = %08X\n", jr(IBAS), jr(IEND));
}

extern u8 jpeg_mode_rec;
extern u8 jpeg_mode_rtp;

////////////////////////////////////////////////////////////////////////////////
u32 mode_rec_rtp = 0;
u32 jpeg_isr_tick = 0;
u32 jpeg_mode_tmp;
extern u8 manual_for_jpeg;
//uint32_t jpeg_isr_for_mode3(void *arg, void *reg, uint32_t clr, int i);
extern u8 mode_for_wifi_and_tf;

typedef void (*jpg_isr_sleep)(uint32_t t);

// 非中断模式查询
int no_jpg_isr(void *controller, jpg_isr_sleep jpg_sleep)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  uint32_t pending = jpreg(base, JPxFLG); // todo 與上JPxINTCON
  int times = 0;
  int res = 0;

  while (!(pending & BIT(JPCIRQ_FRAME_DONE))) // JPCIRQ_DEC_DCTIMEROUT
  {
    if (pending & BIT(JPCIRQ_JDRST))
    {
      printf("%s JDRST err\r\n", __FUNCTION__);
      break;
    }
    if (pending & BIT(JPCIRQ_DEC_DCTIMEROUT))
    {
      printf("%s timeout err\r\n", __FUNCTION__);
      break;
    }
    if (pending & BIT(JPCIRQ_DEC_HUFF_ERR))
    {
      printf("%s huf err\r\n", __FUNCTION__);
      break;
    }
    if (pending & BIT(JPCIRQ_IN_PACK_EMPTY))
    {
      printf("%s inputbuf empty err\r\n", __FUNCTION__);
      break;
    }

    jpg_sleep(3);
    pending = jpreg(base, JPxFLG);
  }
  //printf("pending:%X\r\n", pending);
  res = (pending & BIT(JPCIRQ_FRAME_DONE));
  jpreg(base, JPxFLG_CLR) = ~0;
  // printf("res:%d\t%d\t%d\r\n",res,pending,BIT(JPCIRQ_FRAME_DONE));
  return res;
}
int no_jpgEnc_isr(void *controller, jpg_isr_sleep jpg_sleep)
{
  jpeg_controller *jpc = (jpeg_controller *)controller;
  pSFR base = get_jpreg(jpc);
  uint32_t pending = jpreg(base, JPxFLG); // todo 與上JPxINTCON
  int times = 0;
  int res = 0;

  while (!(pending & BIT(JPCIRQ_FRAME_DONE))) // JPCIRQ_DEC_DCTIMEROUT
  {
    if (pending & BIT(JPCIRQ_OUT_PACK_FULL))
    {
      printf("%s packet full err\r\n", __FUNCTION__);
      break;
    }
    if (pending & BIT(JPCIRQ_DATA_ERR))
    {
      printf("%s data err\r\n", __FUNCTION__);
      break;
    }

    jpg_sleep(3);
    pending = jpreg(base, JPxFLG);
  }
  //printf("pending:%X\r\n", pending);

  res = (pending & BIT(JPCIRQ_FRAME_DONE));
  jpreg(base, JPxFLG_CLR) = ~0;
  // printf("res:%d\t%d\t%d\r\n",res,pending,BIT(JPCIRQ_FRAME_DONE));
  return res;
}
uint32_t jpgPendFlg;

uint32_t jpegPending(void *arg)
{
	//pSFR base = get_jpreg(arg);
	return jpgPendFlg;//jpreg(base, JPxFLG);
}

static int jpeg_isr(uint32_t irq, void *arg, void *reg)
{
  jpeg_controller *jpc = (jpeg_controller *)arg;
  pSFR base = get_jpreg(jpc);
  uint32_t pending = jpreg(base, JPxFLG); // todo 與上JPxINTCON
  int i;
  uint32_t bi;
  uint32_t clr = 0;
  jpgPendFlg=pending;

  if (pending & BIT(JPCIRQ_FRAME_DONE))
    pending &= ~(BIT(JPCIRQ_OUT_PACK_FULL) | BIT(JPCIRQ_IN_PACK_EMPTY));

  bi = 1;
  for (i = 0; i < JPCIRQ_MAX; ++i, bi <<= 1)
  {
    if ((pending & bi) && jpc->isr[i])
    {
      clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg);
    }
  }

  // 暂时清除这个pending
  if (pending & BIT(JPCIRQ_JDRST))
  {
    // printf("JDRST pending:%X\r\n",pending);
    // printf("clr:%X\r\n",clr);
    // jpreg(base, JPxCON1) |=(1<<7);
    // jpreg(base, JPxCON1) =0;
    jpreg(base, JPxFLG_CLR) = ~0;
  }

  //if (!clr)
  {
    jpreg(base, JPxFLG_CLR) = ~0; // pending必须在设置输入输出地址之后，否则地址设置会失败
    // printf("@");
  }
}

u8 get_rec_pixel();

#if 0
uint32_t jpeg_isr_for_mode3(void *arg, void *reg, uint32_t clr, int i)
{
  jpeg_controller *jpc = (jpeg_controller *)arg;
  pSFR base = get_jpreg(jpc);

  if (manual_for_jpeg) // 分辨率一样
  {
    if (i == JPCIRQ_FRAME_DONE)
    {
      //		  printf("isr:%x  src:%x reg:%x\n",jpc->isr[i],jpc->dev[mode_rec_rtp][i],reg);
    }
    clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg); // 0:wifi,1:rec
  }
  else // 分辨率不一样
  {
    // printf("jpc->irqno:%d  %x\n",jpc->irqno,jpc->isr[i] );
    if (get_rec_pixel())
    { // rec是否720P
      if (jpc->irqno == IRQNO_JPGB)
      {
        clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg); // rec为720P的话，则这里表示rec用自动模式，用CSI0,
                                                                            // WIFI就是VGA，用到CSI1--->JPG1
      }
      else if (jpc->irqno == IRQNO_JPGA)
      {
        clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg); // 自动模式，为REC，所以这里是dev[1]
      }
    }
    else
    {
      //	      printf("j: %d\n",jpc->irqno);
      if (jpc->irqno == IRQNO_JPGB)
      {
        // printf("jpg:%x  %x\n",jpc->isr[i],jpc->dev[1][i]);
        clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg);
      }
      else if (jpc->irqno == IRQNO_JPGA)
      {
        clr |= jpc->isr[i](jpc->jpeg_num, jpc->dev[jpc->jpeg_num][i], reg); // 自动模式，为WIFI，所以这里为dev[0]
      }
    }
  }

  return clr;
}
#endif

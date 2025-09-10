/****************************************************************************
**
**                         BUILDWIN AX32XX MCU LAYER
** *   **                     THE APPOTECH HAL
**** **                       AX32XX MJPEG
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : ax32xx_mjpeg.c
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "AX329x.h"
#include "mjpeg.h"
#define IRQ_JPGB 22
#define IRQ_JPGA 16
#define R_MJPEG_OUT_SADDR BACK_JPOBAS
#define R_MJPEG_OUT_EADDR BACK_JPOEND
#define R_MJPEG_IN_SADDR BACK_JPIBAS
#define R_MJPEG_IN_EADDR BACK_JPIEND
// #define  R_MJPEG_CTRL
#define R_MJPEG_CTRL1 BACK_JPCON1
#define R_MJPEG_CTRL2 BACK_JPCON2
#define R_MJPEG_INTEN BACK_JPINTCON
#define R_MJPEG_INTCLR BACK_JPFLG_CLR
#define R_MJPEG_INTSTATUS BACK_JPFLG
#define R_MJPEG_SIZE BACK_JPSIZE0
#define R_MJPEG_SIZE1 BACK_JPSIZE1
#define R_MJPEG_XSCALER BACK_JPXSCALE
#define R_MJPEG_YSCALER BACK_JPYSCALE
#define R_MJPEG_HEIGHT BACK_JPHCNT
#define R_MJPEG_BIT BACK_JPUBIT
#define R_MJPEG_OUTPTR BACK_JPOPTR
#define R_MJPEG_OUTLOAD BACK_JPOLTA
#define R_MJPEG_INPTR BACK_JPIPTR
#define R_MJPEG_DRI BACK_JPDRI
#define R_MJPEG_DADDR BACK_JPBSHADDR
#define R_MJPEG_LINES BACK_JPLS
#define R_MJPEG_DTATS BACK_JPES
#define R_MJPEG_TABLE BACK_JPCOM
#define R_MJPEG_DMA1YBASE BACK_JPDCYBASE
#define R_MJPEG_DMA1UVBASE BACK_JPDCUVBASE
#define R_MJPEG_STRIDE BACK_JPDMAOUTX32

#define R_MJPEG_CCOEF0 BACK_JPBSCCOEF0
#define R_MJPEG_CCOEF1 BACK_JPBSCCOEF1
#define R_MJPEG_CCOEF2 BACK_JPBSCCOEF2
#define R_MJPEG_CCOEF3 BACK_JPBSCCOEF3
#define R_MJPEG_CCOEF4 BACK_JPBSCCOEF4
#define R_MJPEG_CCOEF5 BACK_JPBSCCOEF5
#define R_MJPEG_CCOEF6 BACK_JPBSCCOEF6
#define R_MJPEG_CCOEF7 BACK_JPBSCCOEF7

#define R_MJPEGA_OUT_SADDR PRE_JPOBAS
#define R_MJPEGA_OUT_EADDR PRE_JPOEND
#define R_MJPEGA_IN_SADDR PRE_JPIBAS
#define R_MJPEGA_IN_EADDR PRE_JPIEND
// #define  R_MJPEG_CTRL
#define R_MJPEGA_CTRL1 PRE_JPCON1
#define R_MJPEGA_CTRL2 PRE_JPCON2
#define R_MJPEGA_INTEN PRE_JPINTCON
#define R_MJPEGA_INTCLR PRE_JPFLG_CLR
#define R_MJPEGA_INTSTATUS PRE_JPFLG
#define R_MJPEGA_SIZE PRE_JPSIZE0
#define R_MJPEGA_SIZE1 PRE_JPSIZE1
#define R_MJPEGA_XSCALER PRE_JPXSCALE
#define R_MJPEGA_YSCALER PRE_JPYSCALE
#define R_MJPEGA_HEIGHT PRE_JPHCNT
#define R_MJPEGA_BIT PRE_JPUBIT
#define R_MJPEGA_OUTPTR PRE_JPOPTR
#define R_MJPEGA_OUTLOAD PRE_JPOLTA
#define R_MJPEGA_INPTR PRE_JPIPTR
#define R_MJPEGA_DRI PRE_JPDRI
#define R_MJPEGA_DADDR PRE_JPBSHADDR
#define R_MJPEGA_LINES PRE_JPLS
#define R_MJPEGA_DTATS PRE_JPES
#define R_MJPEGA_TABLE PRE_JPCOM
#define R_MJPEGA_DMA1YBASE PRE_JPDCYBASE
#define R_MJPEGA_DMA1UVBASE PRE_JPDCUVBASE
#define R_MJPEGA_STRIDE PRE_JPDMAOUTX32

#define R_MJPEGA_CCOEF0 PRE_JPBSCCOEF0
#define R_MJPEGA_CCOEF1 PRE_JPBSCCOEF1
#define R_MJPEGA_CCOEF2 PRE_JPBSCCOEF2
#define R_MJPEGA_CCOEF3 PRE_JPBSCCOEF3
#define R_MJPEGA_CCOEF4 PRE_JPBSCCOEF4
#define R_MJPEGA_CCOEF5 PRE_JPBSCCOEF5
#define R_MJPEGA_CCOEF6 PRE_JPBSCCOEF6
#define R_MJPEGA_CCOEF7 PRE_JPBSCCOEF7

#define CCOEF0 0x00017F00
#define CCOEF1 0xFF0E7CF7
#define CCOEF2 0xFC2170F3
#define CCOEF3 0xF8365FF3
#define CCOEF4 0xF54B4BF5
#define CCOEF5 0xF35F36F8
#define CCOEF6 0xF37021FC
#define CCOEF7 0xF77C0EFF
/*
#define  CCTAB0	   170012706UL
#define  CCTAB1	   220474911UL
#define  CCTAB2	   270937372UL
#define  CCTAB3	   321399833UL
#define  CCTAB4	   355084821UL
#define  CCTAB5	   422324243UL
#define  CCTAB6	   472720912UL
#define  CCTAB7	   523117581UL
*/
#define CCTAB0 186789922UL
#define CCTAB1 220475167UL
#define CCTAB2 270937372UL
#define CCTAB3 321399577UL
#define CCTAB4 371861782UL
#define CCTAB5 422258707UL
#define CCTAB6 472720912UL
#define CCTAB7 523117837UL
#define __sprclr(spr, n) spr &= ~BIT(n) // mtspr(spr,mfspr(spr) & ~(1 << n))
#define __sprset(spr, n) spr |= BIT(n)	// mtspr(spr,mfspr(spr) | (1 << n))
void sysCpuNopDelay(int dtime)
{
	while (dtime--)
	{
		asm("l.nop");
	}
}

void ax32xx_intEnable(u8 irqNum, u8 en)
{
	if (en)
		__sprset(SPR_PICMR, irqNum);
	else
		__sprclr(SPR_PICMR, irqNum);
}

static volatile u32 jpegErrStatus = 0, jpegFlag = 0;
extern void jpegA_table_init(bool tp);

static void (*ax32xxMJPEGA_EncodeISR)(int flag);
static void (*ax32xxMJPEGB_EncodeISR)(int flag);
static void (*ax32xxMJPEGDecodeISR)(int flag);

void ax32xx_mjpegEA_ncodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGA_EncodeISR = isr;
}
void ax32xx_mjpegEB_ncodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGB_EncodeISR = isr;
}

void ax32xx_mjpegDecodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGDecodeISR = isr;
}

void mjpeg_clr_pend(u32 bit)
{
	R_MJPEGA_INTCLR = bit;
}

u32 mjpeg_irq_statu()
{
	u32 statu = 0;
	u32 jflg = R_MJPEGA_INTSTATUS;
	//  printf("%x\n",jflg);

	if (jflg & BIT(7))
	{
		printf("data full\n");
	}
	if (jflg & BIT(6))
	{
		statu |= MJPEG_FRAME_DONE;
	}
	else if (jflg & BIT(0))
	{
		statu |= MJPEG_OUT_PACK_FULL;
	}
	if (jflg & BIT(3))
	{
		statu |= MJPEG_DATA_ERR;
		// R_MJPEGA_INTCLR |= jflg;
	}

	return statu;
}

/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeEnable
 * Description    : jpeg encode enable
 * Input          : u8 ne : 1-enable,0-disable
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeEnable(u8 en)
{
	if (en)
	{
		R_MJPEGA_CTRL1 |= BIT(8); // mjpeg run
	}
	else
	{
		R_MJPEGA_CTRL1 |= BIT(9);	  // mjpeg stop
		R_MJPEGA_INTEN &= ~(1 << 11); // MJP_INIT_EN
		R_MJPEGA_CTRL1 |= BIT(9);	  // mjpeg stop
		R_MJPEGA_INTCLR = 0xffffffff; // clear all int flag
	}
}

/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeBufferSet
 * Description    : jpeg encode set buffer
 * Input          : u32 saddr : start address
 *                  u32 eddr : end address
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeBufferSet(u32 saddr, u32 eaddr)
{
	dcache_flush_region(saddr, eaddr - saddr);

	R_MJPEGA_OUT_SADDR = saddr;
	R_MJPEGA_OUT_EADDR = (eaddr | (3 << 28));
	// R_MJPEGA_OUT_EADDR &= ~(BIT(28) | BIT(29));
}

void jpeg_run()
{
	PRE_JPCON1 |= BIT(8); // JERUN
	PRE_JPELINECNT = 8;
	PRE_JPELINECNT = 800;
}

/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeSizeSet
 * Description    : jpeg encode size set
 * Input          : u32 src_w : source width
 *                  u32 src_h : source height
 *                  u32 tar_w : target width
 *                  u32 tar_h : target height
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeSizeSet(u32 src_w, u32 src_h, u32 tar_w, u32 tar_h)
{
	if (src_w > tar_w)
		src_w = tar_w;
	if (src_h > tar_h)
		src_h = tar_h;
	R_MJPEGA_SIZE1 = (src_w << 16) | src_h;
	R_MJPEGA_SIZE = (tar_w << 16) | tar_h;

	R_MJPEGA_XSCALER = (0 << 18)			   // div n(ecdoe not use)
					   | (src_w / tar_w << 16) // intergser
					   | ((u16)(src_w * 65536 / tar_w + (bool)(src_w * 65536 % tar_w)) << 0);

	R_MJPEGA_YSCALER = (src_h / tar_h << 16) // intergser
					   | ((u16)(src_h * 65536 / tar_h + (bool)(src_h * 65536 % tar_h)) << 0);
}
/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeQuilitySet
 * Description    : jpeg encode quility set
 * Input          : u8 qulity
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeQuilitySet(u8 qulity)
{
	R_MJPEGA_CTRL1 &= ~(0x0f << 23);
	R_MJPEGA_CTRL1 |= (qulity & 0x0f) << 23;
}

/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeQadj
* Description    : jpeg encode quality ajust
* Input          : u32 jsize : last jpeg size
					  u32 min_size : limited min size
					  u32 max_size: limited max size
* Output         : none
* Return         : none
*******************************************************************************/
volatile u32 javgsize;
void ax32xx_mjpegEncodeQadj(u32 jsize, u32 min_size, u32 max_size)
{
	static s8 tq;
	if (jsize == 0)
		return;
	javgsize += jsize;
	s8 q = (R_MJPEGA_CTRL1 >> 23) & 0x0f;
	if (q == 0)
	{ // q = 0 equ q = 8
		q = 8;
	}
	if (jsize > max_size)
	{
		q++;
	}
	if (jsize < min_size)
	{
		q--;
	}
	// q -> (JPEG_Q_27 ~ JPEG_Q_81)
	if (q < 0x03)
	{ // JPEG_Q_81
		q = 0x03;
	}
	if (q > 0x0f)
	{ // JPEG_Q_27
		q = 0x0f;
	}
	if (tq != q)
	{
		tq = q;
		// printf("Q,S:%x,%dK\n",q, jsize/1024);
		u8 sbuf[] = {'Q', '=', '0', '0', '\n', '\0'};
		sbuf[2] = q / 10 + '0';
		sbuf[3] = q % 10 + '0';
		// LogPrint(sbuf,0);
	}
	ax32xx_mjpegEncodeQuilitySet(q);
}
/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeInfoSet
 * Description    : jpeg encode info mode set
 * Input          : u8 mode
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeInfoSet(u8 mode)
{
	R_MJPEGA_CTRL1 &= ~(3 << 19);
	R_MJPEGA_CTRL1 |= (mode & 3) << 19;
}

/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeLoadAddrGet
 * Description    : jpeg encode get load addr
 * Input          : u8 mode
 * Output         : none
 * Return         : none
 *******************************************************************************/
u32 mjpegEncodeLoadAddrGet(void)
{
	return R_MJPEGA_OUTLOAD;
}

void mjpeg_inlinebuf_init(u8 *ybuf, u8 *uvbuf)
{
	u32 i;
	PRE_JPELINECNT = 0;
	PRE_JPIBAS = (u32)ybuf;
	PRE_JPIEND = (u32)uvbuf;
}
/*******************************************************************************
 * Function Name  : ax32xx_mjpegEncodeInit
 * Description    : jpeg encode initial
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void mjpegEncodeInit(bool manual, bool tp)
{
	RSTCON &= ~BIT(0); // reset mjpeg modlule
	sysCpuNopDelay(10);
	RSTCON |= BIT(0);  // normal set mjpeg
	PCON0 &= ~BIT(26); // enable mjpeg clk

	jpegA_table_init(tp);

	R_MJPEGA_CCOEF0 = CCOEF0;
	R_MJPEGA_CCOEF1 = CCOEF1;
	R_MJPEGA_CCOEF2 = CCOEF2;
	R_MJPEGA_CCOEF3 = CCOEF3;
	R_MJPEGA_CCOEF4 = CCOEF4;
	R_MJPEGA_CCOEF5 = CCOEF5;
	R_MJPEGA_CCOEF6 = CCOEF6;
	R_MJPEGA_CCOEF7 = CCOEF7;

	R_MJPEGA_CTRL1 &= ~(0x0f << 23);
	R_MJPEGA_CTRL1 &= ~(0x03 << 19);
	R_MJPEGA_CTRL1 = 0		   // BIT(27)//JEOAUTOLD_EN
					 | BIT(13) // ENCHOLD_EN
					 | BIT(22) // DCTIDCT
					 | BIT(0)  // JEODMAEN
					 | BIT(11) // JESTSYNCEN
					 | BIT(14) // WR_CRITICAL_EN
					 | BIT(15) // RD_CRITICAL_EN
		;
	R_MJPEGA_INTEN = BIT(31)  // JPEG_INT_EN
					 | BIT(6) // FRAMEDONE_INT_EN
					 | BIT(4) // JEOLDARD_INT_EN
					 //|BIT(JEO_FULL_INT_EN)
					 | BIT(2)  // DMA_OBERROR_INT_EN
					 | BIT(0)  // JPO_DMA_INT_EN
					 | BIT(11) // JPTINF0_INTEN
					 | BIT(10) // JPTINF1_INTEN
					 | BIT(9)  // JPTINF2_INTEN
		;
	/*jpCON1 BIT 10
	 * 0:DMA input addr using csi internal
	 * 1:DMA input addr using user setting just JPIBAS/JPIEND
	 */
	(manual) ? (PRE_JPCON1 |= BIT(10)) : (PRE_JPCON1 &= ~BIT(10));
	R_MJPEGA_INTCLR = -1;
	R_MJPEGA_HEIGHT = (768 << 16) | (600);
}
#if 0
void ax32xx_mjpB_reset(void)
{
	PCON0 &= ~BIT(31);			//JPEG  clk enable
	(RSTCON) &= ~BIT(2);
	sysCpuNopDelay(30);
	(RSTCON) |= BIT(2);
	sysCpuNopDelay(30);
}

/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeScalerCal
* Description    : jpeg decode scaler caculate
* Input          : u32 src_w : source width
*                  u32 tar_w : source height
* Output         : none
* Return         : none
*******************************************************************************/

s32 ax32xx_mjpegDecodeScalerCal(u32 src_w,u32 tar_w)
{
	const u8 pre_xscale_table[] = {0,0,1,2,2};
	const u8 pre_xscale_table1[] = {0,1,2,2,3,3,3,3,3};
	u32 res = 0;
	u32 prescale0 = 0,prescale1 = 0,temp0 = src_w,dst_w4 = tar_w * 4;
	
	if(src_w > 1920)
	{
		prescale0 = pre_xscale_table[(src_w + 1920 - 1)/1920];
		temp0 = (src_w + (1 << prescale0) - 1)/(1 << prescale0);
	}
	
	if(temp0 >= dst_w4)
	{
		prescale1 = pre_xscale_table1[temp0 / dst_w4];
	}	
	
	temp0 = (prescale0 + prescale1);
	res = (temp0 << 16)|((src_w + (1 << temp0) - 1)/(1 << temp0));
	
	return res;	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeSetSize
* Description    : jpeg decode size set
* Input          : u32 src_w : source width
*                  u32 src_h : source height
*                  u32 tar_w : target width
*                  u32 tar_h : target height
*                  u32 stride: 0:disable stride function
*                             -1:don't change stride
*                              other:target stride,for dma0
* Output         : none
* Return         : none
*******************************************************************************/
s32 ax32xx_mjpegDecodeSetSize(u32 src_w,u32 src_h,u32 tar_w,u32 tar_h,u32 stride)
{
    static u32 sum0 = 0;
    static u32 jpcon2,jpsize0,jpsize1,jpxscale,jpyscale;
    
	u32 pre_xscale,scale_src_width;
	u32 xi,xf,yi,yf;
	
	if(src_w > 4000)
		return -1;

    //避免重复计算
    u32 sum = src_w + src_h + tar_w + tar_h;
    if(sum0 != sum)
    {
        sum0 = sum;
        //避免prescale
        if((src_w / 4) == tar_w)
        {
            //保存原始尺寸
            R_MJPEG_CTRL2 =
            jpcon2 = (1<<28)|
                     (src_w<<16)|
                     (src_h<<0);
            src_w -= 2;
        }
        else
            R_MJPEG_CTRL2 = jpcon2 = 0;

        pre_xscale = ax32xx_mjpegDecodeScalerCal(src_w,tar_w);
        scale_src_width = pre_xscale & 0xffff;
        xi = scale_src_width / tar_w;
        xf = (((scale_src_width % tar_w)<<16) + tar_w - 1) / tar_w;
        yi = src_h / tar_h;
        yf = (((src_h % tar_h)<<16) + tar_h - 1) / tar_h;
        
        pre_xscale >>= 16;
        
        R_MJPEG_SIZE   = (src_w<<16)|//decoder src width
                         (src_h<<0); //decoder src height

        R_MJPEG_SIZE1  = (tar_w<<16)|//decoder dst width
                         (tar_h<<0); //decoder dst height

        R_MJPEG_XSCALER= (pre_xscale<<18)|//x scale div
                         (xi<<16)|//integrate scale
                         (xf<<0); //fractional of scale

        R_MJPEG_YSCALER= (yi<<16)|//integrate scale
                         (yf<<0); //fractional of scale
        
        jpsize0  = R_MJPEG_SIZE;
        jpsize1  = R_MJPEG_SIZE1;
        jpxscale = R_MJPEG_XSCALER;
        jpyscale = R_MJPEG_YSCALER;
    }
    else
    {
        R_MJPEG_CTRL2   = jpcon2;
        R_MJPEG_SIZE    = jpsize0;
        R_MJPEG_SIZE1   = jpsize1;
        R_MJPEG_XSCALER = jpxscale;
        R_MJPEG_YSCALER = jpyscale;
    }
    
    if(stride != 0xffffffff)
    {
        R_MJPEG_STRIDE = (src_w<<16)|//DMA1 stride
                         ((!!stride)<<12)|//decoder yuv420 manual stride en
                         (stride<<0); //decoder stride of yuv420 output
    }
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeOutputSet
* Description    : jpeg decode output buffer set
* Input          : u32 yaddr : yaddress
*                  u32 uvaddr: uv address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeOutputSet(u32 yaddr,u32 uvaddr)
{
	R_MJPEG_OUT_SADDR = yaddr;
	R_MJPEG_OUT_EADDR = uvaddr;
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInputSet
* Description    : jpeg decode input set
* Input          : u32 saddr : start address
*                  u32 eaddr : end address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInputSet(u32 saddr,u32 eaddr)
{
	R_MJPEG_IN_SADDR = saddr&(~3);
	R_MJPEG_IN_EADDR = eaddr;
	R_MJPEG_BIT      = (saddr&3)<<3;
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeDriSet
* Description    : jpeg decode dri set
* Input          : u32 dri
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeDriSet(u32 dri)
{
	R_MJPEG_DRI = dri;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCompressSet
* Description    : jpeg decode compress set
* Input          : u32 table : compress value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeCompressSet(u32 table)
{
	R_MJPEG_TABLE = table;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInitTable
* Description    : jpeg decode huffman table
* Input          : u32 tab : table address
*                  u32 len : reserved,do not care this value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInitTable(u32 tab,u32 len)
{
	u8 *dst = (u8 *)0x66400;
	u8 *src = (u8 *)tab;
	u32 i;
	
	for(i=0;i<796;i++)
	{
		*dst++ = *src++;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_mjepgDecodeInit
* Description    : jpeg decode initial
* Input          : u8 yuvfmt : 0:yuv420,1:yuv422
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInit(u8 yuvfmt)
{
//    ax32xx_sysReset(SYS_RESET_JPGB,1); // reset mjpeg modlule
//    ax32xx_sysCpuNopDelay(10);
//    ax32xx_sysReset(SYS_RESET_JPGB,0); // reset mjpeg modlule
	PCON0 &= ~BIT(31);
	R_MJPEG_CTRL1 =  (R_MJPEG_CTRL1 & ~(1<<16))|
                     (1<<29)|//decoder del line(just for 422)
                     (0<<28)|//encoder
                     (0<<27)|//encoder
                     (0<<23)|//encoder quantize select
                     (0<<22)|//codec DCI/IDCT select,0:IDCT
                     (0<<19)|//encoder add 8/28 byte AVI chunk header before .jpg header
                     (0<<18)|//encoder generate JPEG head en
                     (0<<17)|//encoder DRI enable
                     ((!!yuvfmt)<<16)|//decoder yuv420/yuv422 select
                     (0<<15)|//dma read critical en
                     (0<<14)|//dma write critical en
                     (0<<13)|//encoder hold en
                     (0<<12)|//encoder
                     (0<<11)|//encoder sync to csi frame start
                     (0<<10)|//encoder manual mode en
                     (0<<9)| //encoder stop and reset
                     (0<<8)| //encoder start
                     (0<<7)| //decoder stop and reset
                     (0<<6)| //decoder start
                     (1<<5)| //decoder end-marker select
                     (0<<4)| //encoder output auto reload en
                     (0<<3)| //encoder
                     (0<<2)| //decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
                     (1<<1)| //input DMA en
                     (1<<0); //output DMA en
	R_MJPEG_INPTR   = 0;
	R_MJPEG_INTEN   = (1<<31)|//codec jpeg int en
                      (0<<11)|//time-info int en
                      (0<<10)|//time-info int en
                      (0<<9)| //time-info int en
                      (1<<8)| //decoder huffman err int en
                      (0<<7)| //encoder
                      (1<<6)| //codec frame done int en
                      (1<<5)| //decoder DRI err int en
                      (0<<4)| //encoder reload output addr int en
                      (0<<3)| //output buffer error int en
                      (0<<2)| //decoder DMA(no block-scaler) frame done int en
                      (0<<1)| //decode DMA input int en,input one packet,one interrupt
                      (0<<0); //encode DMA output int en,output one packet,one interrupt
	
    R_MJPEG_INTCLR = (1<<11)|//time-info
                     (1<<10)|//time-info
                     (1<<9)| //time-info
                     (1<<8)| //decoder huffman err flag
                     (1<<7)| //encoder
                     (1<<6)| //codec frame done flag
                     (1<<5)| //decoder DRI err flag
                     (1<<4)| //encoder
                     (1<<3)| //output buffer error
                     (1<<2)| //decoder DMA(no block-scaler) frame done
                     (0<<1)| //decoder input dma resume en
                     (0<<0); //encoder output dma resume en
				
	R_MJPEG_CCOEF0 = CCTAB0;
	R_MJPEG_CCOEF1 = CCTAB1;
	R_MJPEG_CCOEF2 = CCTAB2;
	R_MJPEG_CCOEF3 = CCTAB3;
	R_MJPEG_CCOEF4 = CCTAB4;
	R_MJPEG_CCOEF5 = CCTAB5; 
	R_MJPEG_CCOEF6 = CCTAB6;
	R_MJPEG_CCOEF7 = CCTAB7;	
    jpegFlag = 1;
    jpegErrStatus = 0;		
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeEnable
* Description    : jpeg decode enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeEnable(u8 en)
{
	if(en)
	{
		R_MJPEG_CTRL1 |= (1<<29)|//decoder
                         (0<<28)|//encoder
                         (0<<27)|//encoder
                         (0<<23)|//encoder quantize select
                         (0<<22)|//codec DCI/IDCT select,0:IDCT
                         (0<<19)|//encoder add 8/28 byte AVI chunk header before .jpg header
                         (0<<18)|//encoder generate JPEG head en
                         (0<<17)|//encoder DRI enable
                         (0<<16)|//decoder yuv420/yuv422 select
                         (0<<15)|//dma read critical en
                         (0<<14)|//dma write critical en
                         (0<<13)|//encoder hold en
                         (0<<12)|//encoder
                         (0<<11)|//encoder sync to csi frame start
                         (0<<10)|//encoder manual mode en
                         (0<<9)| //encoder stop and reset
                         (0<<8)| //encoder start
                         (0<<7)| //decoder stop and reset
                         (1<<6)| //decoder start
                         (1<<5)| //decoder end-marker select
                         (0<<4)| //encoder output auto reload en
                         (0<<3)| //encoder
                         (0<<2)| //decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
                         (1<<1)| //input DMA en
                         (1<<0); //output DMA en
		jpegFlag = 1;
		ax32xx_intEnable(IRQ_JPGB,1);	
        	
	}
	else
	{
		ax32xx_intEnable(IRQ_JPGB,0);	
	}
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStop
* Description    : jpeg decode stop
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeStop(void)
{
	R_MJPEG_CTRL1 |= (1<<7);
	R_MJPEG_CTRL1  = 0;
    
    R_MJPEG_INTCLR = (1<<11)|//time-info
                     (1<<10)|//time-info
                     (1<<9)| //time-info
                     (1<<8)| //decoder huffman err flag
                     (1<<7)| //encoder
                     (1<<6)| //codec frame done flag
                     (1<<5)| //decoder DRI err flag
                     (1<<4)| //encoder
                     (1<<3)| //output buffer error
                     (1<<2)| //decoder DMA(no block-scaler) frame done
                     (0<<1)| //decoder input dma resume en
                     (0<<0); //encoder output dma resume en
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeQDTCfg
* Description    : jpeg decode config qdt table
* Input          : u8 *qdt[3] : table
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeQDTCfg(u8 *qdt[2])
{
	u8 i;
	u8 *dst = (u8 *)0x66400;

	for(i=0;i<2;i++)
	{
		memcpy(dst,qdt[i],64);
		dst+=64;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStatus
* Description    : jpeg decode get status
* Input          : none
* Output         : none
* Return         : s32 : 1-busy,0-idle
*******************************************************************************/
s32 ax32xx_mjpegDecodeStatus(void)
{
	//printf("R_MJPEG_INTSTATUS&:%x\n",R_MJPEG_INTSTATUS & (3 << 16));
	if(R_MJPEG_INTSTATUS&(3<<16))
		return 1;  // busy
	else
		return 0;  // idle
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCheck
* Description    : jpeg decode error check
* Input          : none
* Output         : none
* Return         : s32 : 0-no error.other-error
*******************************************************************************/
s32 ax32xx_mjpegDecodeCheck(void)
{
	s32 temp;
	temp = jpegErrStatus;
	jpegErrStatus = 0;
	return temp;
}

/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeODma1Cfg
* Description    : config output dma1(no block-scaler),call this function before init jpeg-decoder
*                  ax32xx_mjpegDecodeStop will disable DMA1
* Input          : u8 * y_addr
*                  u8 * uv_addr
*                  bool dma_en
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeODma1Cfg(u8 * y_addr,u8 * uv_addr,bool dma_en)
{
    R_MJPEG_DMA1YBASE   = (u32)y_addr;
    R_MJPEG_DMA1UVBASE  = (u32)uv_addr;
    
    if(!dma_en)
    {
        //disable dma1
        R_MJPEG_CTRL1   &=~(1<<2);
        R_MJPEG_INTEN   &=~(1<<2);
        R_MJPEG_INTCLR   = (1<<2);
    }
    else
    {
        if(ax32xx_mjpegDecodeStatus())
        {
        }
        else
        {
            R_MJPEG_CTRL1 |= (1<<2);//decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
            R_MJPEG_INTEN |= (1<<2);
        }
    }
}
const u32 bicCtab[]=
{
	0x00017F00, 
	0xFF0E7CF7, 
	0xFC2170F3, 
	0xF8365FF3, 
	0xF54B4BF5, 
	0xF35F36F8, 
	0xF37021FC, 
	0xF77C0EFF,
};
/*******************************************************************************
* Function Name  : ax32xx_mjpegIRQHandler
* Description    : jpeg irq handler
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegBIRQHandler(void)
{
	volatile u32 jflg = R_MJPEG_INTSTATUS;
//--------encode status--------------------	
	if(jpegFlag==0)
	{
		if(ax32xxMJPEGB_EncodeISR)//hal_mjpB_isr
			ax32xxMJPEGB_EncodeISR(jflg);
	}
	else
	{
		ax32xx_mjpegDecodeStop();
		mjpBcodec_sacle_dwon(jflg);
        //huffman err
        if(jflg & (1<<8))
        {
            jpegErrStatus = 1<<6;
            printf("mjpBcodec: <huf err>\n");
        }
        if(jflg & (1<<5))
        {
            jpegErrStatus = 1<<5;
            printf("mjpBcodec: <dri err>\n");
        }

		if(ax32xxMJPEGDecodeISR) //deocode_done()
			ax32xxMJPEGDecodeISR(jflg);   
		
		
	}
	R_MJPEG_INTCLR = jflg;
}

/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void ax32xx_mjpB_Ctl_init(u8 mod, u8 quality, u32 src_w,  u32 src_h, u32 obj_w, u32 obj_h)
{
	ax32xx_mjpB_reset();
	//humfamn, quanta tab cfg
	ax32xx_mjpB_tab_init();
	
	//scale cfg
	if(src_w > obj_w){
		src_w = obj_w;
	}
	if(src_h > obj_h){
		src_h = obj_h;	
	}
	BACK_JPSIZE1 = (src_w << 16) | src_h;//src size
	BACK_JPSIZE0 = (obj_w << 16) | obj_h;//obj size
	
	BACK_JPXSCALE = (0 << 18)  //div n(ecdoe not use)
				|(src_w/obj_w << 16)//intergser
				|((u16)(src_w*65536 /obj_w + (bool)(src_w*65536 %obj_w))  << 0);
	
	BACK_JPYSCALE = (src_h/obj_h << 16)//intergser
				|((u16)(src_h*65536 /obj_h + (bool)(src_h*65536 %obj_h))  << 0);
				
	BACK_JPBSCCOEF0 = bicCtab[0];
	BACK_JPBSCCOEF1 = bicCtab[1];
	BACK_JPBSCCOEF2 = bicCtab[2];
	BACK_JPBSCCOEF3 = bicCtab[3];
	BACK_JPBSCCOEF4 = bicCtab[4];
	BACK_JPBSCCOEF5 = bicCtab[5];
	BACK_JPBSCCOEF6 = bicCtab[6];
	BACK_JPBSCCOEF7 = bicCtab[7];
	//printf("> Jsacle: %x,%x,%x,%x\n",JPSIZE,JPSIZE1,JPXSCALE,JPYSCALE);
	
	BACK_JPCON1 &= ~(0x0f << 23);//Q
	BACK_JPCON1 &= ~(0x03 << 19);//jpg & mjpg mod
	

	BACK_JPCON1 = 0//BIT(27)//JEOAUTOLD_EN
				|BIT(13)//ENCHOLD_EN
				//|BIT(JPBITSCHRLD_EN)
				//|BIT(JPBTSCH_EN)
				|((quality & 0x0f)<< 23)
				|BIT(22)//DCTIDCT
				//|BIT(10)//ENCODST
				//|BIT(INFOMODE)
				//|BIT(HEADERSKIP)
				|((mod & 0x03) << 19)//INFMOD:(0:no 1:8byte, 2:28buye)
				|BIT(0)//JEODMAEN
				//|BIT(DRISKIP)
				|BIT(11)//JESTSYNCEN
				|BIT(14)//WR_CRITICAL_EN
				|BIT(15)//RD_CRITICAL_EN
				|BIT(10) //JPENCMAN (only manual)
				;

	//int cfg			
	BACK_JPINTCON = BIT(31)//JPEG_INT_EN
				|BIT(6)//FRAMEDONE_INT_EN
				|BIT(4)//JEOLDARD_INT_EN
				//|BIT(JEO_FULL_INT_EN)
				|BIT(2)//DMA_OBERROR_INT_EN
				|BIT(0)//JPO_DMA_INT_EN
				|BIT(11)//JPTINF0_INTEN 
				|BIT(10)//JPTINF1_INTEN 
				|BIT(19)//JPTINF2_INTEN 
				;
	//clr all pending
	BACK_JPFLG_CLR = -1;
	
	BACK_JPHCNT = (768 << 16) //high
			|(600 << 0) //low
			;
	//printf("BACK_JPCON1:%x,%x\n",BACK_JPCON1,BACK_JPINTCON);
}
void ax32xx_mjpB_Linebuf_cfg(u8* ybuf, u8 *uvbuf)
{
	BACK_JPELINECNT = 0;
	BACK_JPIBAS = (u32)ybuf;
	BACK_JPIEND = (u32)uvbuf;
	
	//printf("Jline:%x,%x\n",BACK_JPIBAS,BACK_JPIEND);
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpB_dma_cfg(u32 s, u32 e)
{
	BACK_JPOBAS = s ;
	BACK_JPOEND = (e | (0x03 << 28));
	
}
void ax32xx_mjpB_stop(void)
{
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	BACK_JPINTCON &= ~BIT(11); 		//MJP_INIT_EN
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	
	BACK_JPFLG_CLR = ~0;				//MJP_INT_PENDING	
	ax32xx_intEnable(IRQ_JPGB,0);		
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpB_on(void)
{
	BACK_JPCON1 |= BIT(8); //JERUN
	BACK_JPELINECNT = 8;
	BACK_JPELINECNT = 800;	
	ax32xx_intEnable(IRQ_JPGB,1);
}
#endif
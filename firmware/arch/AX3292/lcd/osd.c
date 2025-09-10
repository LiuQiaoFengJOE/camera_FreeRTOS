#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"

#include "gpio_ops/gpio.h"
#include "sys/sys.h"
#include "osd.h"

////////////////////////////////////////////////////////////////////////////////
#define IRQNO_DE 21
////////////////////////////////////////////////////////////////////////////////

#define R_DE_DISPLAY_CTRL DE_DISPLAY_CTRL
#define R_DE_FRAME_CTRL DE_FRAME_CTRL
#define R_DE_CSC DE_CSC_BCS_CFG
#define R_DE_CRITICAL DE_CRITICAL_CTRL
#define R_DE_INT DE_INT_CTRL
#define R_DE_STATUS DE_STATUS
#define R_DE_SCRENN_SIZE DE_SCREEN_CFG
#define R_DE_BACKGROUND DE_BK_CFG

#define R_VIDEO_Y_ADDR DE_VIDEO_Y_FB_CFG
#define R_VIDEO_Y_STRIDE DE_VIDEO_Y_STRIDE_CFG
#define R_VIDEO_UV_ADDR DE_VIDEO_UV_FB_CFG
#define R_VIDEO_UV_STRIDE DE_VIDEO_UV_STRIDE_CFG
#define R_VIDEO_SIZE DE_VIDEO_SIZE_CFG
#define R_VIDEO_POS DE_VIDEO_COOR_CFG
#define R_VIDEO_SCALE DE_VIDEO_SCALE_COF_CFG

#define R_OSD_FBADDR0 DE_OSD0_FB_CFG
#define R_OSD_SIZE0 DE_OSD0_SIZE_CFG
#define R_OSD_COOR0 DE_OSD0_COOR_CFG
#define R_OSD_STRIDE0 DE_OSD0_STRIDE_CFG
#define R_OSD_PALETTE0 0x50000
#define R_OSD_FBADDR1 DE_OSD1_FB_CFG
#define R_OSD_SIZE1 DE_OSD1_SIZE_CFG
#define R_OSD_COOR1 DE_OSD1_COOR_CFG
#define R_OSD_STRIDE1 DE_OSD1_STRIDE_CFG
#define R_OSD_PALETTE1 (R_OSD_PALETTE0 + 2014)
#define R_OSD_ALPHA DE_OSD_ALPHA_CFG
#define OSD_PALETTE_SIZE 1024

static void (*ax32xx_deISR)(void);

/*******************************************************************************
 * Function Name  : ax32xx_deWait
 * Description    : de wait
 * Input          :  none
 * Output         : None
 * Return         :
 *******************************************************************************/
void ax32xx_deWait(void)
{
	volatile int i = 0xf000;
	while ((R_DE_FRAME_CTRL & BIT(1)) != 0)
	{
		if ((R_DE_STATUS & BIT(9)) != 0)
			break;
		i--;
		if (i == 0)
			break;
	}
}

/*******************************************************************************
 * Function Name  : ax32xx_deUpdate
 * Description    : de update
 * Input          :  none
 * Output         : None
 * Return         :
 *******************************************************************************/
void ax32xx_deUpdate(void)
{
	R_DE_FRAME_CTRL |= (1 << 1);
}
/*******************************************************************************
 * Function Name  : ax32xx_deGetUpdateStatus
 * Description    : get de update status
 * Input          :  none
 * Output         : bit 1 status
 * Return         :
 *******************************************************************************/
u8 ax32xx_deGetUpdateStatus(void)
{
	return (R_DE_FRAME_CTRL & (1 << 1)) >> 1;
}
/*******************************************************************************
 * Function Name  : ax32xx_deIRQHandler
 * Description    : initial de irq handler
 * Input          :  none
 * Output         : None
 * Return         :
 *******************************************************************************/
void ax32xx_deIRQHandler(void)
{
	if (DE_STATUS & BIT(0))
	{
		DE_STATUS &= ~BIT(0);
		if (ax32xx_deISR)
			ax32xx_deISR();
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_deISRRegister
 * Description    : register de isr
 * Input          :  void (*isr)(void) : call back
 * Output         : None
 * Return         :
 *******************************************************************************/
void ax32xx_deISRRegister(void (*isr)(void))
{
	ax32xx_deISR = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_deInit
* Description    : initial de for video/osd0/osd1 layers
* Input          :  u16 width : display width
					   u16 height: display height
* Output         : None
* Return         :
*******************************************************************************/
void ax32xx_deInit(u16 width, u16 height)
{
	ax32xx_sysClkSet(SYS_CLK_DE, 1);
	//	ax32xx_deWait();

	R_DE_CRITICAL = (0 << 16) | // osd arbit number
					(0 << 10) | // critical en
					(0 << 9) |	// critical mode
					(15 << 0);	// critical burst cycle

	R_DE_INT = (0 << 0); // frame end int en

	R_DE_STATUS = (0 << 9) | // de busy flag
				  (0 << 0);	 // frame end pending

	R_DE_SCRENN_SIZE = ((height - 1) << 16) | ((width - 1) << 0); // screen width
	R_DE_DISPLAY_CTRL = 0;

	//	ax32xx_deUpdate();

	R_DE_FRAME_CTRL = (1 << 1) | // double-buffer register status
					  (1 << 0);	 // de en

	disable_irq(IRQNO_DE);
}

void ax32xx_Dereinit(u16 width, u16 height)
{
	R_DE_SCRENN_SIZE = ((height - 1) << 16) | ((width - 1) << 0); // screen width
	disable_irq(IRQNO_DE);
}

/*******************************************************************************
* Function Name  : ax32xx_deSetBackground
* Description    : set video layer background color
* Input          :  u8 red : red value
					   u8 green:green value
					   u8 blue  : blue value
* Output         : None
* Return         :
*******************************************************************************/
void ax32xx_deSetBackground(u8 red, u8 green, u8 blue)
{
	//    ax32xx_deWait();
	R_DE_BACKGROUND = (red << 16) | (green << 8) | (blue << 0);
	//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_deSetCSC
* Description    : set video layer csc effect
* Input          :  u16 brightness : brightness
					   u16 saturation:saturation
					   u16 contrast  : contrast
* Output         : None
* Return         :
*******************************************************************************/
void ax32xx_deSetCSC(u16 brightness, u16 saturation, u16 contrast)
{
	//	ax32xx_deWait();
	R_DE_CSC = (brightness << 16) | ((saturation & 0x0f) << 8) | ((contrast & 0x0f) << 0); // contrast
	R_DE_DISPLAY_CTRL &= ~(1 << 12);													   // 改
	//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetDither
* Description    : set video layer color dither width
* Input          :  u8 red : red color width
					   u8 green:green color width
					   u8 blue  : blue color width
* Output         : None
* Return         :
*******************************************************************************/
void ax32xx_videoSetDither(u8 red, u8 green, u8 blue)
{
	//	ax32xx_deWait();
	R_DE_DISPLAY_CTRL &= ~(0x0f << 15);
	R_DE_DISPLAY_CTRL |= ((((red == 5) || (red == 6)) << 18) |
						  ((red != 5) << 17) |
						  ((green != 5) << 16) |
						  ((blue != 5) << 15)); // dither B width
	//	ax32xx_deUpdate();
}

static const u32 def_args[] = {
	0x40000000, 0,									// Y_WTAB0
	0x40000000, 0,									// Y_WTAB1
	0x40000000, 0,									// Y_WTAB2
	0x40000000, 0,									// Y_WTAB3
	0x40000000, 0,									// Y_WTAB4
	0x40000000, 0,									// Y_WTAB5
	0x40000000, 0,									// Y_WTAB6
	0x40000000, 0,									// Y_WTAB7
	0x00004000, 0xff083efb, 0xfc123af8, 0xfa1e31f7, // YHTAB0~3
	0xf82729f8, 0xf7311efa, 0xf83a12fc, 0xfb3e08ff, // YHTAB4~7
	0x40000000, 0,									// UV_WTAB0
	0x40000000, 0,									// UV_WTAB1
	0x40000000, 0,									// UV_WTAB2
	0x40000000, 0,									// UV_WTAB3
	0x40000000, 0,									// UV_WTAB4
	0x40000000, 0,									// UV_WTAB5
	0x40000000, 0,									// UV_WTAB6
	0x40000000, 0,									// UV_WTAB7
};

void ax32xx_osdSetUpscaler(u16 src_w, u16 src_h, u16 dst_w, u16 dst_h, u32 *args)
{
	if (!args)
		args = (u32 *)def_args;

	DE_YWTABLE0L_CFG = args[0];
	DE_YWTABLE0H_CFG = args[1];
	DE_YWTABLE1L_CFG = args[2];
	DE_YWTABLE1H_CFG = args[3];
	DE_YWTABLE2L_CFG = args[4];
	DE_YWTABLE2H_CFG = args[5];
	DE_YWTABLE3L_CFG = args[6];
	DE_YWTABLE3H_CFG = args[7];
	DE_YWTABLE4L_CFG = args[8];
	DE_YWTABLE4H_CFG = args[9];
	DE_YWTABLE5L_CFG = args[10];
	DE_YWTABLE5H_CFG = args[11];
	DE_YWTABLE6L_CFG = args[12];
	DE_YWTABLE6H_CFG = args[13];
	DE_YWTABLE7L_CFG = args[14];
	DE_YWTABLE7H_CFG = args[15];
	DE_YHTABLE0_CFG = args[16];
	DE_YHTABLE1_CFG = args[17];
	DE_YHTABLE2_CFG = args[18];
	DE_YHTABLE3_CFG = args[19];
	DE_YHTABLE4_CFG = args[20];
	DE_YHTABLE5_CFG = args[21];
	DE_YHTABLE6_CFG = args[22];
	DE_YHTABLE7_CFG = args[23];
	DE_UVWTABLE0L_CFG = args[24];
	DE_UVWTABLE0H_CFG = args[25];
	DE_UVWTABLE1L_CFG = args[26];
	DE_UVWTABLE1H_CFG = args[27];
	DE_UVWTABLE2L_CFG = args[28];
	DE_UVWTABLE2H_CFG = args[29];
	DE_UVWTABLE3L_CFG = args[30];
	DE_UVWTABLE3H_CFG = args[31];
	DE_UVWTABLE4L_CFG = args[32];
	DE_UVWTABLE4H_CFG = args[33];
	DE_UVWTABLE5L_CFG = args[34];
	DE_UVWTABLE5H_CFG = args[35];
	DE_UVWTABLE6L_CFG = args[36];
	DE_UVWTABLE6H_CFG = args[37];
	DE_UVWTABLE7L_CFG = args[38];
	DE_UVWTABLE7H_CFG = args[39];

	DE_WSTEP_CFG = (src_w << 16) / (dst_w);
	DE_HSTEP_CFG = (src_h << 16) / (dst_h);
	DE_VIDEO_SIZE_CFG = ((src_h - 1) << 16) |  // video height
						((src_w - 1) << 0);	   // video width
	DE_SCA_SRCSIZE_CFG = ((src_h - 1) << 16) | // src height
						 ((src_w - 1) << 0);   // src width
	DE_SCA_DSTSIZE_CFG = ((dst_h - 1) << 16) | // dst height
						 ((dst_w - 1) << 0);   // dst width
	DE_DISPLAY_CTRL |= (1 << 7);			   // upscaler en
}

/*******************************************************************************
* Function Name  : ax32xx_videoSetScale
* Description    : set video layer scale
* Input          :  u8 tap0 : tap0
					   u8 tap1 : tap1
					   u8 tap2 : tap2
					   u8 tap3 : tap3
* Output         : None
* Return         :
*******************************************************************************/
void ax32xx_videoSetScale(u8 tap0, u8 tap1, u8 tap2, u8 tap3)
{
	//	ax32xx_deWait();
	R_VIDEO_SCALE = (tap3 << 24) | // tap3 0xfa
					(tap2 << 16) | // tap2 0x26
					(tap1 << 8) |  // tap1 0x26
					(tap0 << 0);   // tap0 0xfa
	R_DE_DISPLAY_CTRL |= 1 << 13;
	//	ax32xx_deUpdate();
}
/*******************************************************************************
 * Function Name  : ax32xx_videoSetScanMode
 * Description    : set video layer scan mode.scan mode is rotate
 * Input          :  u8 scan_mode : scan mode
 * Output         : None
 * Return         :
 *******************************************************************************/
void ax32xx_videoSetScanMode(u8 scan_mode)
{
	if (scan_mode < 7)
	{
		//		ax32xx_deWait();
		R_DE_DISPLAY_CTRL &= ~(7 << 8);
		R_DE_DISPLAY_CTRL |= (scan_mode & 7) << 8;
		//		ax32xx_deUpdate();
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGetYAddr
 * Description    : set video layer current y buffer addr
 * Input          :
 * Output         : None
 * Return         : buffer addr
 *******************************************************************************/
u32 ax32xx_videoGetYAddr(void)
{
	return (R_VIDEO_Y_ADDR & 0xffffff);
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGetUVAddr
 * Description    : set video layer current uv buffer addr
 * Input          :
 * Output         : None
 * Return         : buffer addr
 *******************************************************************************/
u32 ax32xx_videoGetUVAddr(void)
{
	return (R_VIDEO_UV_ADDR & 0xffffff);
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetAddr
* Description    : set video layer buffer addr
* Input          :  u32 y_addr : ystride
					   u32 uv_addr : uv_addr
					   u8 update  : 1-update now,0-update late
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetAddr(u32 y_addr, u32 uv_addr, u8 update)
{
	//	if(update)
	//		ax32xx_deWait();
	R_VIDEO_Y_ADDR = (y_addr << 0); // video y addr,64byte align

	R_VIDEO_UV_ADDR = (uv_addr << 0); // video uv addr ,64byte align

	//	if(update)
	//	   ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetStride
* Description    : set video layer stride
* Input          :  u32 ystride : ystride
					   u32 uvstride : uvstride
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetStride(u32 ystride, u32 uvstride)
{
	//	ax32xx_deWait();
	DE_VIDEO_Y_STRIDE_CFG = ((ystride >> 2) << 0);	 // video y stride,unit:word
	DE_VIDEO_UV_STRIDE_CFG = ((uvstride >> 2) << 0); // video uv stride
	//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetSize
* Description    : set video layer size
* Input          :  u16 width : width
					   u16 height : height
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetSize(u16 width, u16 height)
{
	//	ax32xx_deWait();
	R_VIDEO_SIZE = ((height - 1) << 16) | ((width - 1) << 0); // video width
	//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetPosition
* Description    : set video position
* Input          :  s16 x : x
					   s16 y : y
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetPosition(s16 x, s16 y)
{
	//	ax32xx_deWait();
	R_VIDEO_POS = (y << 16) | (x << 0); // video start-x
	//	ax32xx_deUpdate();
}

void print_videoPosition()
{
	printf("R_VIDEO_POS:%X\n", R_VIDEO_POS);
}
/*******************************************************************************
 * Function Name  : ax32xx_videoEnable
 * Description    : video enable
 * Input          :  u8 en: 0-disable,1-enable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_videoEnable(u8 en)
{
	//	ax32xx_deWait();
	if (en)
		R_DE_DISPLAY_CTRL |= 1;
	else
		R_DE_DISPLAY_CTRL &= ~1;
	//	ax32xx_deUpdate();
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGammaRed
 * Description    : de video gamma table for red color
 * Input          :  u32 lut : table addr
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_videoGammaRed(u32 lut)
{
	//	memcpy((u32 *)(0x50800 ),(u32 *)lut,64*4);
	u32 *src, *dst, i;

	dst = (u32 *)0x50800;
	src = (u32 *)lut;
	for (i = 0; i < 128; i++)
	{
		*dst++ = *src++;
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGammaRed
 * Description    : de video gamma table for green color
 * Input          :  u32 lut : table addr
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_videoGammaGreen(u32 lut)
{
	//	memcpy((u32 *)(0x50800 + 64*4),(u32 *)lut,64*4);
	u32 *src, *dst, i;

	dst = (u32 *)(0x50800 + 4 * 128);
	src = (u32 *)lut;
	for (i = 0; i < 128; i++)
	{
		*dst++ = *src++;
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGammaRed
 * Description    : de video gamma table for blue color
 * Input          :  u32 lut : table addr
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_videoGammaBlue(u32 lut)
{
	//	memcpy((u32 *)(0x50800 + 64*8),(u32 *)lut,64*4);
	u32 *src, *dst, i;

	dst = (u32 *)(0x50800 + 8 * 128);
	src = (u32 *)lut;
	for (i = 0; i < 128; i++)
	{
		*dst++ = *src++;
	}
}

/*******************************************************************************
 * Function Name  : ax32xx_deSetGAMA
 * Description    : set GAMA table by index
 * Input          :  u8 gamma_index :gamma table index
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_deSetGAMA(u32 gred, u32 ggreen, u32 gblue)
{
	if ((!gred) || (!ggreen) || (!gblue))
		return;
	ax32xx_videoGammaRed(gred);
	ax32xx_videoGammaGreen(ggreen);
	ax32xx_videoGammaBlue(gblue);

	ax32xx_videoGammaEnable(1);
}
/*******************************************************************************
 * Function Name  : ax32xx_videoGammaEnable
 * Description    : enable de video gamma table
 * Input          :  u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_videoGammaEnable(u8 en)
{
	//	ax32xx_deWait();
	if (en)
		R_DE_DISPLAY_CTRL |= (1 << 14);
	else
		R_DE_DISPLAY_CTRL &= ~(1 << 14);
	//	ax32xx_deUpdate();
}

/*******************************************************************************
* Function Name  : ax32xx_osdEnable
* Description    : osd enable
* Input          : osd : osd id,OSD0,OSD1
					  u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdEnable(u8 osd, u8 en)
{
	if (osd >= OSD_MAX)
		return -1;
	//	ax32xx_deWait();
	if (en)
	{
		DE_DISPLAY_CTRL |= 1 << (osd + 1);
	}
	else
	{
		DE_DISPLAY_CTRL &= ~(1 << (osd + 1));
	}
	//    ax32xx_deUpdate();
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetSize
* Description    : osd set size
* Input          : osd : osd id,OSD0,OSD1
					  u16 width : width
					  u16 height: height
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetSize(u8 osd, u16 width, u16 height)
{
	if (osd >= OSD_MAX)
		return -1;

	//	ax32xx_deWait();
	switch (osd)
	{
	case OSD0:
		R_OSD_SIZE0 = ((height - 1) << 16) | (width - 1);
		R_OSD_STRIDE0 = (width * height) - 1;
		break;
	case OSD1:
		R_OSD_SIZE1 = ((height - 1) << 16) | (width - 1);
		R_OSD_STRIDE1 = (width * height) - 1;
		break;
	}

	//    ax32xx_deUpdate();
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetPosition
* Description    : osd set position
* Input          : osd : osd id,OSD0,OSD1
					  u16 x : x
					  u16 y: y
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetPosition(u8 osd, s16 x, s16 y)
{
	if (osd >= OSD_MAX)
		return -1;

	//	ax32xx_deWait();
	switch (osd)
	{
	case OSD0:
		R_OSD_COOR0 = (y << 16) | (x);
		break;
	case OSD1:
		R_OSD_COOR1 = (y << 16) | (x);
		break;
	}

	//   ax32xx_deUpdate();
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetPalette
* Description    : osd set palette
* Input          : osd : osd id,OSD0,OSD1
					  u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
static void cpu_memcpy4(u32 *dst, u32 *src, u32 bytes)
{
	for (; bytes; bytes -= 4)
		*dst++ = *src++;
}

s32 ax32xx_osdSetPalette(u8 osd, u32 addr)
{
	if (osd >= OSD_MAX)
		return -1;

	switch (osd)
	{
	case OSD0:
		cpu_memcpy4((u32 *)R_OSD_PALETTE0, (u32 *)addr, OSD_PALETTE_SIZE);
		break;
	case OSD1:
		cpu_memcpy4((u32 *)R_OSD_PALETTE1, (u32 *)addr, OSD_PALETTE_SIZE);
		break;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetAddr
* Description    : osd set fb addr
* Input          : osd : osd id,OSD0,OSD1
					  u32 addr : addr
* Output         : None
* Return         :
*******************************************************************************/
s32 ax32xx_osdSetAddr(u8 osd, u32 addr)
{
	if (osd >= OSD_MAX)
		return -1;

	switch (osd)
	{
	case OSD0:
		R_OSD_FBADDR0 = addr;
		break;
	case OSD1:
		R_OSD_FBADDR1 = addr;
		break;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetAlpha
* Description    : osd set set alpha type
* Input          : osd : osd id,OSD0,OSD1
					  u8 type : ALPHA_NORMAL,ALPHA_GLOBAL
					  u8 value : gblobal alpha value
* Output         : None
* Return         :
*******************************************************************************/
s32 ax32xx_osdSetAlpha(u8 osd, u8 type, u8 value)
{
	u32 v;
	if (osd >= OSD_MAX)
		return -1;

	v = R_OSD_ALPHA & ~(0x2f << (osd * 16));
	value |= ((type & 1) << 5);
	R_OSD_ALPHA = v | ((value & 0x2f) << (osd * 16));

	return 0;
}
/*
void ax32xx_osdPrint(void)
{
	debg("R_OSD_FBADDR0=%x\n",R_OSD_FBADDR0);
	debg("R_OSD_SIZE0= %x\n",R_OSD_SIZE0);
	debg("R_OSD_COOR0= %x\n",R_OSD_COOR0);
	debg("R_OSD_STRIDE0= %x\n",R_OSD_STRIDE0);
	debg("R_OSD_PALETTE0= %x\n",R_OSD_PALETTE0);
	debg("R_OSD_FBADDR1= %x\n",R_OSD_FBADDR1);
	debg("R_OSD_SIZE1= %x\n",R_OSD_SIZE1);
	debg("R_OSD_COOR1= %x\n",R_OSD_COOR1);
	debg("R_OSD_STRIDE1= %x\n",R_OSD_STRIDE1);
	debg("R_OSD_PALETTE1= %x\n",R_OSD_PALETTE1);
	debg("R_OSD_ALPHA= %x\n",R_OSD_ALPHA);
	debg("DE_DISPLAY_CTRL= %x\n",DE_DISPLAY_CTRL);

}

*/
void ax32xx_osd_video_Print(void)
{
	printf("R_DE_CRITICAL=%x\n", R_DE_CRITICAL);
	printf("R_DE_INT= %x\n", R_DE_INT);
	printf("R_DE_STATUS= %x\n", R_DE_STATUS);
	printf("R_DE_SCRENN_SIZE= %x\n", R_DE_SCRENN_SIZE);
	printf("R_DE_DISPLAY_CTRL= %x\n", R_DE_DISPLAY_CTRL);
	printf("R_DE_FRAME_CTRL= %x\n", R_DE_FRAME_CTRL);
	printf("DE_VIDEO_Y_STRIDE_CFG= %x\n", DE_VIDEO_Y_STRIDE_CFG);
	printf("DE_VIDEO_UV_STRIDE_CFG= %x\n", DE_VIDEO_UV_STRIDE_CFG);
	printf("R_VIDEO_Y_ADDR= %x\n", R_VIDEO_Y_ADDR);
	printf("R_VIDEO_UV_ADDR= %x\n", R_VIDEO_UV_ADDR);
	printf("R_VIDEO_SIZE= %x\n", R_VIDEO_SIZE);
	printf("R_VIDEO_POS= %x\n", R_VIDEO_POS);
}

void lcdc_set_palette(u32 addr)
{
	ax32xx_osdSetPalette(0, addr);
}

uint32_t lcdc_de_doublebuffer(void)
{
	return ax32xx_deGetUpdateStatus();
}

uint32_t lcdc_de_status(void)
{
	return DE_STATUS;
}

void lcdc_de_update(void)
{
	R_DE_FRAME_CTRL |= (1 << 1);
}

/*******************************************************************************
* Function Name  : lcdc_osd_enable
* Description    : osd enable
* Input          : osd : osd id,OSD0,OSD1
					  u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 lcdc_osd_enable(u8 osd, u8 en)
{
	ax32xx_osdEnable(osd, en);
}

// extern u8 test_photo[59136];

void lcdc_set_fbuf(uint32_t layer, void *y, void *uv)
{
	//  printf("layer:%x addr:%x\n",layer,y);

	if (layer)
		ax32xx_osdSetAddr(layer - 1, (uint32_t)y & ~0x1F);
	else
	{
		// y = test_photo;
		// uv = test_photo+38280;

		//      Uart_PrintfBuf(":\n",y,32);
		ax32xx_videoSetAddr(y, uv, 0); // todo uvbuffer
	}
}

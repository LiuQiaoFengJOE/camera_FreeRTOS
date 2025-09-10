/****************************************************************************
**
**                         BUILDWIN HAL LAYER
** *   **                   THE APPOTECH HAL
**** **                 TINFO HAL LAYER DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : hal_watermark.h
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TINFO HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "hal_watermark.h"



__RAM_USE__ ramuse __attribute__((section(".sram_comm")));

#define Tbdri ramuse.Tbdri
#define tminf_font ramuse.tminf_font
#define tminfB_font ramuse.tminfB_font



static HAL_WATERMARK_T hal_watermark_ctrl[MJPEG_TIMEINFO_MAX] =
	{
		{
			.width = TIME0_W,
			.height = TIME0_H,
			.layer = MJPEG_TIMEINFO_LAYER0,
			.autoUpdate = 1,
			.font = 0,
			.ram = tminf_font.win0,
		},

		{
			.width = TIME1_W,
			.height = TIME1_H,
			.autoUpdate = 1,
			.font = 0,
			.layer = MJPEG_TIMEINFO_LAYER0,
			.ram = tminf_font.win1,
		},

		{
			.width = TIME2_W,
			.height = TIME2_H,
			.autoUpdate = 1,
			.font = 0,
			.layer = MJPEG_TIMEINFO_LAYER2,
			.ram = tminf_font.win2,
		}};
static u32 HAL_Y, HAL_U, HAL_V;
static s32 HAL_B_Y_POS_ADJ;
static u16 wmMode = 0;
static u16 wmx = 0;
static u16 wmy = 0;
void hal_watermarkSetPos(u16 mode, u16 x, u16 y)
{
	wmx = x;
	wmy = y;
	wmMode = mode;
}

void hal_watermarkGetPos(u16 *mode, u16 *x, u16 *y)
{
	if ((x == NULL) || (y == NULL))
		return;
	*mode = wmMode;
	*x = wmx;
	*y = wmy;
}
/*******************************************************************************
 * Function Name  : hal_watermarkISR
 * Description    : hal watermark isr
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void hal_watermarkCallBack(void)
{
	int i;

	for (i = 0; i < MJPEG_TIMEINFO_MAX; i++)
	{
		if (hal_watermark_ctrl[i].stat && hal_watermark_ctrl[i].en && hal_watermark_ctrl[i].callback)
			hal_watermark_ctrl[i].callback();
	}
}

static void hal_watermark_date_CallBack(void)
{
	if (hal_watermark_ctrl[WATERMARK_DATE].autoUpdate)
	{
		videoRecordWatermark(&hal_watermark_ctrl[WATERMARK_DATE]);
	}
}

void videoRecordWatermark(void *watermark)
{
	INT8U *buffer;
	INT16U w, h;

	char string[20];
	HAL_WATERMARK_T *ctrl = watermark;
	buffer = (INT8U *)hal_watermarkRam2(watermark, &w, &h);

	if (buffer && w && h)
	{
		DATE_TIME_T *rtcTime = hal_rtcTimeGet();
		hal_rtcTime2StringExt(string, rtcTime);

		string[19] = 0;
		// printf("string:%s\r\n",string);
		streamSprintf(string, buffer, w, ctrl->font);
	}
}

/*******************************************************************************
 * Function Name  : hal_watermarkInit
 * Description    : hal watermark init
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 hal_watermarkInit(void)
{
	int i;

	for (i = 0; i < MJPEG_TIMEINFO_MAX; i++)
	{
		hal_watermark_ctrl[i].stat = 0;
		hal_watermark_ctrl[i].en = 0;
		hal_watermark_ctrl[i].callback = NULL;
		ax32xx_mjpegATimeinfoEnable(i, 0);
		ax32xx_mjpegBTimeinfoEnable(i, 0);
	}
	HAL_Y = 0xff;
	HAL_U = 0x80;
	HAL_V = 0x80;
	//	ax32xx_mjpegTimeinfoColor(0xff,0x80,0x80);  // set default color

	hal_rtcCallBackRegister(hal_watermarkCallBack);

	return 0;
}

s32 hal_watermarkInit2(void)
{
	int i;

	for (i = 0; i < MJPEG_TIMEINFO_MAX; i++)
	{
		hal_watermark_ctrl[i].stat = 0;
		hal_watermark_ctrl[i].en = 0;
		hal_watermark_ctrl[i].callback = NULL;
		hal_watermark_ctrl[i].autoUpdate = 0;
	}
	HAL_Y = 0xff;
	HAL_U = 0x80;
	HAL_V = 0x80;
	hal_rtcCallBackRegister(hal_watermark_date_CallBack);
	return 0;
}

/*******************************************************************************
 * Function Name  : hal_watermarkClose
 * Description    : hal watermark close stream
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 hal_watermarkClose(s32 layer)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;

	if (hal_watermark_ctrl[layer].en)
	{
		ax32xx_mjpegATimeinfoEnable(layer, 0); // disable mjpeg timeinfo
		ax32xx_mjpegBTimeinfoEnable(layer, 0); // disable mjpeg timeinfo
	}
	hal_watermark_ctrl[layer].stat = 0;

	return 0;
}
/*******************************************************************************
 * Function Name  : hal_watermarkClear
 * Description    : hal watermark clear
 * Input          :  s32 layer : layer
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 hal_watermarkClear(s32 layer)
{
	int j;

	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;
	for (j = 0; j < (hal_watermark_ctrl[layer].width * hal_watermark_ctrl[layer].height / 8); j++)
		hal_watermark_ctrl[layer].ram[j] = 0;
	return 0;
}
/*******************************************************************************
 * Function Name  : hal_watermarkOpen
 * Description    : hal watermark open stream
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 hal_watermarkOpen(void)
{
	int i;

	for (i = 0; i < MJPEG_TIMEINFO_MAX; i++)
	{
		if (hal_watermark_ctrl[i].stat == 0)
		{
			if ((hal_watermark_ctrl[i].width == 0) || (hal_watermark_ctrl[i].height == 0)) // can not be used
				continue;
			hal_watermark_ctrl[i].stat = 1;
			hal_watermark_ctrl[i].en = 0;
			hal_watermark_ctrl[i].callback = NULL;
			hal_watermarkClear(i);

			return i;
		}
	}

	return -1;
}

// 重新修改
void *hal_watermarkOpen2(u8 which)
{
	if (which < MJPEG_TIMEINFO_MAX)
	{
		return &hal_watermark_ctrl[which];
	}
	printf("%s:NULL\r\n", __FUNCTION__);
	return NULL;
}

/*******************************************************************************
* Function Name  : hal_watermarkColor
* Description    : hal watermark color
* Input          : u8 layer : timeinfo layer
* Input		   : u32 y : y-byte
				u32 u : u-bye
				u32 v : v-byte
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkColor(s32 layer, u8 y, u8 u, u8 v)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;

	HAL_Y = y;
	HAL_U = u;
	HAL_V = v;
	//	ax32xx_mjpegTimeinfoColor(HAL_Y,HAL_U,HAL_V);  // set default color

	return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkAddr
* Description    : hal watermark addr
* Input          : u8 layer : timeinfo layer
					  void *addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkAddr(s32 layer, void *addr)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;
	//	ax32xx_mjpegTimeinfoAddr(layer,(u32)addr);   // can not use user ram

	return -1;
}
/*******************************************************************************
* Function Name  : hal_watermarkSize
* Description    : hal watermark size
* Input          : u8 layer : timeinfo layer
					  u32 width    : width
					  u32 height   : height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkSize(s32 layer, u32 width, u32 height)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;

	hal_watermark_ctrl[layer].width = width;
	hal_watermark_ctrl[layer].height = height;
	//    ax32xx_mjpegTimeinfoSize(layer,width,height);

	return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkPosition
* Description    : hal watermark psotion
* Input          : u8 layer : timeinfo layer
					  s32 x    : x position
					  s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkPosition(s32 layer, s32 x, s32 y)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;
	hal_watermark_ctrl[layer].x = x;
	hal_watermark_ctrl[layer].y = y;
	//	ax32xx_mjpegTimeinfoPos(layer,x,y);
	return 0;
}

s32 hal_watermarkPosition2(void *watermark, s32 x, s32 y)
{
	HAL_WATERMARK_T *ctrl = watermark;
	if (!ctrl)
		return -1;
	ctrl->x = x;
	ctrl->y = y;
	//	ax32xx_mjpegTimeinfoPos(layer,x,y);
	return 0;
}

/*******************************************************************************
* Function Name  : hal_watermarkCallback
* Description    : hal watermark callback register
* Input          : s32 layer : layer
					  void (*callback)(void) : callback
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkCallbackRegister(s32 layer, void (*callback)(void))
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;
	hal_watermark_ctrl[layer].callback = callback;
	return 0;
}

s32 hal_watermarkCallbackRegister2(void *watermark, void (*callback)(void))
{
	HAL_WATERMARK_T *ctrl = watermark;
	ctrl->callback = callback;
	return 0;
}

/*******************************************************************************
* Function Name  : hal_watermarkRam
* Description    : hal watermark ram get
* Input          : s32 layer : layer
					  u32 width    : width
					  u32 height   : height
* Output         : None
* Return         : None
*******************************************************************************/
void *hal_watermarkRam(s32 layer, INT16U *width, INT16U *height)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return NULL;
	if ((hal_watermark_ctrl[layer].width == 0) || (hal_watermark_ctrl[layer].height == 0))
		return NULL;

	if (width)
		*width = hal_watermark_ctrl[layer].width;
	if (height)
		*height = hal_watermark_ctrl[layer].height;
	return (void *)hal_watermark_ctrl[layer].ram;
}

void *hal_watermarkRam2(void *watermark, INT16U *width, INT16U *height)
{
	HAL_WATERMARK_T *ctrl = watermark;
	if (!watermark)
		return NULL;
	if ((ctrl->width == 0) || (ctrl->height == 0))
		return NULL;

	if (width)
		*width = ctrl->width;
	if (height)
		*height = ctrl->height;
	return ctrl->ram;
}

/*******************************************************************************
* Function Name  : hal_watermarkEnable
* Description    : hal watermark enable
* Input          : u8 layer : timeinfo layer
					  u8 en : enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkEnable(s32 layer, u8 en)
{
	if ((layer < 0) || (layer >= MJPEG_TIMEINFO_MAX))
		return -1;
	if (en)
	{
		if ((hal_watermark_ctrl[layer].width == 0) || (hal_watermark_ctrl[layer].height == 0)) // this layer can not be used
			return -1;
		ax32xx_mjpegATimeinfoPos(layer, hal_watermark_ctrl[layer].x, hal_watermark_ctrl[layer].y);
		ax32xx_mjpegBTimeinfoPos(layer, hal_watermark_ctrl[layer].x, /*hal_watermark_ctrl[layer].y*/ HAL_B_Y_POS_ADJ - 32);

		ax32xx_mjpegATimeinfoSize(layer, hal_watermark_ctrl[layer].width, hal_watermark_ctrl[layer].height);
		ax32xx_mjpegBTimeinfoSize(layer, hal_watermark_ctrl[layer].width, hal_watermark_ctrl[layer].height);

		ax32xx_mjpegATimeinfoAddr(layer, (u32)hal_watermark_ctrl[layer].ram);
		ax32xx_mjpegBTimeinfoAddr(layer, (u32)hal_watermark_ctrl[layer].ram);

		ax32xx_mjpegBTimeinfoColor(HAL_Y, HAL_U, HAL_V);
		ax32xx_mjpegATimeinfoColor(HAL_Y, HAL_U, HAL_V); // set default color
	}
	hal_watermark_ctrl[layer].en = en;
	ax32xx_mjpegATimeinfoEnable(layer, en);
	ax32xx_mjpegBTimeinfoEnable(layer, en);

	return 0;
}

/*******************************************************************************
* Function Name  : hal_watermarkEnable
* Description    : hal watermark enable
* Input          : u8 layer : timeinfo layer
				   u8 en : enable
				   u8 which: which mjpeg(mjpegA/mjpegB)
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkEnable2(void *watermark, u8 en, u8 which)
{
	HAL_WATERMARK_T *ctrl = watermark;
	if (!ctrl)
		return -1;
	if (en)
	{
		if ((ctrl->width == 0) || (ctrl->height == 0)) // this layer can not be used
			return -1;
		if (which == 0)
		{
			ax32xx_mjpegATimeinfoPos(ctrl->layer, ctrl->x, ctrl->y);
			ax32xx_mjpegATimeinfoSize(ctrl->layer, ctrl->width, ctrl->height);
			ax32xx_mjpegATimeinfoAddr(ctrl->layer, ctrl->ram);
			ax32xx_mjpegATimeinfoColor(HAL_Y, HAL_U, HAL_V); // set default color
		}
		else if (which == 1)
		{
			ax32xx_mjpegBTimeinfoPos(ctrl->layer, ctrl->x, ctrl->y);
			ax32xx_mjpegBTimeinfoSize(ctrl->layer, ctrl->width, ctrl->height);
			ax32xx_mjpegBTimeinfoAddr(ctrl->layer, ctrl->ram);
			ax32xx_mjpegBTimeinfoColor(HAL_Y, HAL_U, HAL_V);
		}
		videoRecordWatermark(ctrl);
		//printf("font:%d\n",ctrl->font&0xf);
	}
	ctrl->en = en;
	if (which == 0)
	{
		ax32xx_mjpegATimeinfoEnable(ctrl->layer, en);
	}
	else if (which == 1)
	{
		ax32xx_mjpegBTimeinfoEnable(ctrl->layer, en);
	}

	return 0;
}

void hal_watermarkBPos_Adjust(s32 y)
{
	HAL_B_Y_POS_ADJ = y;
}

void hal_set_watermarkAutoUpdate(void *watermark, u8 value)
{
	HAL_WATERMARK_T *ctrl = watermark;
	ctrl->autoUpdate |= value;
	//printf("watermark autoUpdate:0x%x\n",value);
}
void hal_clear_watermarkAutoUpdate(void *watermark, u8 value)
{
	HAL_WATERMARK_T *ctrl = watermark;
	ctrl->autoUpdate &= (~value);
	//printf("watermark autoUpdate:0x%x\n",value);
}

u8 hal_get_watermarkAutoUpdate(void *watermark)
{
	HAL_WATERMARK_T *ctrl = watermark;
	u8 value;
	value = ctrl->autoUpdate;
	return value;
}

// 设置字号
void hal_set_watermark_font(void *watermark, u32 width, u32 height)
{
	static INT32U font_last = 0;
	HAL_WATERMARK_T *ctrl = watermark;
	font_last = streamFont(width, height);
	if (ctrl->font != font_last)
	{
		ctrl->font = font_last;
		HAL_WATERMARK_T *ctrl = watermark;
		memset(ctrl->ram, 0x00, ctrl->width * ctrl->height / 8);
	}
}

// 是否开启日期标签

static u8 watermark_setting = 0;
u8 get_watermark_setting()
{
	// printf("%s:%d\r\n",__FUNCTION__,watermark_setting);
	return watermark_setting;
}

void set_watermark_setting(u8 value)
{
	printf("set_watermark_setting:%d\r\n", value);
	watermark_setting = value;
}

u8 hal_get_watermark_en(void *watermark)
{
	HAL_WATERMARK_T *ctrl = watermark;
	u8 value;
	value = ctrl->en;
	return value;
}

void hal_set_watermark_en(void *watermark, u8 en)
{
	HAL_WATERMARK_T *ctrl = watermark;
	ctrl->en = en;
}

/****************************************************************************
**
**                         BUILDWIN HAL LAYER
** *   **                  THE APPOTECH HAL
**** **                 AUADC HAL LAYER DRIVER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : hal_auadc.h
* Author      : Mark.Douglas
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUADC HARDWARE LAYER.
*
*
* History     :
* 2017-02-27  :
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "hal_auadc.h"

#define PCM_SHEET_SIZE HAL_CFG_PCM_BUFFER_SIZE // 8192L
#define PCM_ITEM_NUM HAL_CFG_PCM_BUFFER_NUM	   // AVI_CBUF_SIZE/AVI_SHEET_SIZE
#define PCM_BUFFER_SIZE (HAL_CFG_PCM_BUFFER_SIZE * HAL_CFG_PCM_BUFFER_NUM)
#define PCM_REC_TYPE_AVI 0
#define PCM_REC_TYPE_RAW 1
#define PCM_REC_TYPE_UAC 2

typedef struct AUADC_CTRL_S
{
	u8 halPcmType;
	u8 dropFlag;
	u8 volume;
	s32 tadcsync;
	s32 tadcsync_next;

	u32 pcmbuf;
	u32 pcmsize;
	u32 curBuffer;
	u32 curLen;
	Stream_Head_T auds;
	Stream_Node_T auadcNode[PCM_ITEM_NUM];

} AUADC_CTRL_T;
static AUADC_CTRL_T auadcCtrl, auadcCtrlB;
static bool auadc_back; // true : back recording is on,need to copy auadc
extern bool uac_ready(void);
u8 auadcbuff[PCM_SHEET_SIZE];
static volatile s32 tpcmsync;
static INT32U curBuffer_temp;
/*******************************************************************************
 * Function Name  : hal_auadcInit
 * Description    : hal layer.auadc initial
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void hal_audc_33ms_syncnt_clr(void)
{
	tpcmsync = 0;
}
void hal_audc_stamp_cal(u32 framesize)
{
	tpcmsync += framesize / (16 * 2);
}
s32 hal_audc_stamp_out(void)
{
	s32 jcnt = tpcmsync / 33;
	s32 algn = tpcmsync / 1000;
	jcnt -= (algn * 1000) / 33 - algn * 30;
	return jcnt;
}
s32 hal_audc_stamp_next(void)
{
	s32 jcnt = (tpcmsync + (PCM_SHEET_SIZE / (16 * 2))) / 33;
	s32 algn = (tpcmsync + (PCM_SHEET_SIZE / (16 * 2))) / 1000;
	jcnt -= (algn * 1000) / 33 - algn * 30;
	return jcnt;
}
void hal_auadcInit(void)
{
	hal_audc_33ms_syncnt_clr();
	ax32xx_auadcInit();

	auadcCtrl.pcmbuf = 0;
	auadcCtrl.pcmsize = 0;
	auadcCtrlB.pcmbuf = 0;
	auadcCtrlB.pcmsize = 0;
}
/*******************************************************************************
 * Function Name  : hal_auadcMemInit
 * Description    : hal layer.adadc memory initial
 * Input          :
 * Output         : None
 * Return         : int
 *******************************************************************************/
int hal_auadcMemInit(void)
{
	//	hal_audc_33ms_syncnt_clr();
	if (auadcCtrl.pcmbuf == 0)
	{
		auadcCtrl.pcmbuf = (u32)osal_malloc(PCM_BUFFER_SIZE);
		if (auadcCtrl.pcmbuf == 0)
			return -1;
		auadcCtrl.pcmsize = PCM_BUFFER_SIZE;
		debg("HAL : [AUADC]<INFO> adcbuffer addr = 0x%x,size = 0x%x\n", auadcCtrl.pcmbuf, auadcCtrl.pcmsize);
	}
	auadc_back = false;
	return 0;
}
int hal_auadcMemInitB(void)
{
	//	hal_audc_33ms_syncnt_clr();
	if (auadcCtrlB.pcmbuf == 0)
	{
		auadcCtrlB.pcmbuf = (u32)hal_sysMemMalloc(PCM_BUFFER_SIZE, 64);
		if (auadcCtrlB.pcmbuf == 0)
			return -1;
		auadcCtrlB.pcmsize = PCM_BUFFER_SIZE;
		debg("HAL : [AUADC_B]<INFO> adcbuffer addr = 0x%x,size = 0x%x\n", auadcCtrlB.pcmbuf, auadcCtrlB.pcmsize);
	}
	return 0;
}
/*******************************************************************************
 * Function Name  : hal_auadcMemUninit
 * Description    : hal layer.adadc memory uninitial
 * Input          :
 * Output         : None
 * Return         : int
 *******************************************************************************/
int hal_auadcMemUninit(void)
{
	if (auadcCtrl.pcmbuf)
		osal_free((void *)auadcCtrl.pcmbuf);
	auadcCtrl.pcmbuf = 0;

	return 0;
}
int hal_auadcMemUninitB(void)
{
	auadc_back = false;
	if (auadcCtrlB.pcmbuf)
		hal_sysMemFree((void *)auadcCtrlB.pcmbuf);
	auadcCtrlB.pcmbuf = 0;

	return 0;
}
/*******************************************************************************
 * Function Name  : hal_auadcBufferSetAVI
 * Description    : hal layer.adadc bufer change for avi
 * Input          : QDT_PCM  *p : queue
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void hal_auadcBufferSet(u8 type, u32 buffer, u32 len)
{
	u32 addr, size;

	if ((!buffer) || (!len))
		return;

	if (type == PCM_REC_TYPE_AVI)
	{
		addr = buffer + 8;
		size = len - 8;
	}
	else
	{
		addr = buffer;
		size = len;
	}

	ax32xx_auadcBufferSet(addr, size);
}
/*******************************************************************************
 * Function Name  : hal_auadcCallBackHalf
 * Description    : hal layer.adadc half flag callback
 * Input          : none
 * Output         : None
 * Return         : none
 *******************************************************************************/
static void hal_auadcCallBackHalf(void)
{
	curBuffer_temp = auadcCtrl.curBuffer;
	u32 buffer = hal_streamMalloc(&auadcCtrl.auds, PCM_SHEET_SIZE);
	if (buffer)
	{
		auadcCtrl.dropFlag = 0;
		auadcCtrl.curBuffer = buffer;
	}
	else
	{
		auadcCtrl.dropFlag = 1;
		buffer = auadcCtrl.curBuffer;
		// sysLog("D:V");
		debg("HAL : [AUADC]<WARNING> drop\n");
	}
	hal_auadcBufferSet(auadcCtrl.halPcmType, auadcCtrl.curBuffer, auadcCtrl.curLen);
	// debg("Line:%d\tfunc:%s\n",__LINE__,__FUNCTION__);
}
/*******************************************************************************
* Function Name  : hal_auadcCallBackEnd
* Description    : hal layer.adadc end flag callback
* Input          : none
* Output         : None
* Return         : none
mjpegEncCtrl.p_mjp = hal_mjpegQueueDel(mjpegEncCtrl.pjchain);	//clr jpg que
hal_mjpegQueueFree(mjpegEncCtrl.cjchain);						//free chain
mjpegEncCtrl.cjchain = hal_mjpegQueueMalloc(__TYP_MJP__);		//new qdt
hal_mjpegEncodeBufferSet(mjpegEncCtrl.cjchain); 				//dma cfg
mjpegEncCtrl.pjchain = mjpegEncCtrl.cjchain->plink; 			//pachain

*******************************************************************************/
void hal_auadcCallBackEnd(void)
{
	hal_audc_stamp_cal(PCM_SHEET_SIZE);
	auadcCtrl.tadcsync = hal_audc_stamp_out();
	auadcCtrl.tadcsync_next = hal_audc_stamp_next();
	// debg("Line:%d\tfunc:%s\n",__LINE__,__FUNCTION__);
	if (auadcCtrl.dropFlag == 0)
	{
		hal_streamIn(&auadcCtrl.auds, auadcCtrl.curBuffer, auadcCtrl.curLen, auadcCtrl.tadcsync, auadcCtrl.tadcsync_next);
		if (auadc_back)
		{
			auadcCtrlB.curBuffer = hal_streamMalloc(&auadcCtrlB.auds, PCM_SHEET_SIZE);
			if (auadcCtrlB.curBuffer)
			{
				// deg_Printf("b:%x\n",curBuffer_temp);
				ax32xx_mcpy1_sdram2gram((void *)auadcCtrlB.curBuffer, (void *)curBuffer_temp, PCM_SHEET_SIZE);
				hal_streamIn(&auadcCtrlB.auds, auadcCtrlB.curBuffer, auadcCtrlB.curLen, auadcCtrl.tadcsync, auadcCtrl.tadcsync_next);
			}
		}
	}
}
/*******************************************************************************
 * Function Name  : hal_auadcStartForWAV
 * Description    : hal layer.auadc start for wav using
 * Input          : _PCM_FRQ_: senclk frequency
 * Output         : None
 * Return         : None
 *******************************************************************************/
int hal_auadcStartForWAV(_PCM_FRQ_ frq, int volume)
{
	auadcCtrl.halPcmType = PCM_REC_TYPE_RAW;
	auadcCtrl.dropFlag = 0;
	hal_audc_33ms_syncnt_clr();
	/*if(hal_auadcMemInit()<0)
	{
		debg("HAL : [AUADC]<ERROR> memory malloc fial.\n");
		return -1;
	}*/
	auadcCtrl.volume = 100; // volume;
	hal_streamInit(&auadcCtrl.auds, auadcCtrl.auadcNode, PCM_ITEM_NUM, (u32)auadcCtrl.pcmbuf, PCM_BUFFER_SIZE);

	auadcCtrl.curBuffer = hal_streamMalloc(&auadcCtrl.auds, PCM_SHEET_SIZE);
	if (auadcCtrl.curBuffer == 0)
		return -1;
	auadcCtrl.curLen = PCM_SHEET_SIZE;
	hal_auadcBufferSet(auadcCtrl.halPcmType, auadcCtrl.curBuffer, auadcCtrl.curLen);

	ax32xx_auadcSetSampleSet(frq);

	ax32xx_auadcHalfIRQRegister(hal_auadcCallBackHalf);
	ax32xx_auadcEndIRQRegister(hal_auadcCallBackEnd);
	// ax32xx_auadcEnable(1);
#if 0
    ax32xx_auadcAGCEnable(1);
#else
	ax32xx_auadcGainSet(0x8);
#endif
	return 0;
}

/*******************************************************************************
 * Function Name  : hal_auadcStartForAVI
 * Description    : hal layer.auadc start for avi using
 * Input          : _PCM_FRQ_: senclk frequency
 * Output         : None
 * Return         : None
 *******************************************************************************/
int hal_auadcStartForAVI(_PCM_FRQ_ frq, int volume)
{
	u32 memaddr;
	auadcCtrl.halPcmType = PCM_REC_TYPE_AVI;
	auadcCtrl.dropFlag = 0;
	hal_audc_33ms_syncnt_clr();
	/*if(hal_auadcMemInit()<0)
	{
		debg("HAL : [AUADC]<ERROR> memory malloc fial.\n");
		return -1;
	}*/
	auadcCtrl.volume = volume;
	memaddr = auadcCtrl.pcmbuf;
	/*if(volume==0)
	{
		auadcCtrl.pcmsize-=PCM_SHEET_SIZE;
		memaddr+=PCM_SHEET_SIZE;
		memset((void *)auadcCtrl.pcmbuf,0,HAL_CFG_PCM_BUFFER_SIZE);
	}*/
	memset((void *)auadcbuff, 0, PCM_SHEET_SIZE);
	hal_streamInit(&auadcCtrl.auds, auadcCtrl.auadcNode, PCM_ITEM_NUM, (u32)memaddr, auadcCtrl.pcmsize);
	auadcCtrl.curBuffer = hal_streamMalloc(&auadcCtrl.auds, PCM_SHEET_SIZE);
	if (auadcCtrl.curBuffer == 0)
		return -1;
	curBuffer_temp = auadcCtrl.curBuffer;
	auadcCtrl.curLen = PCM_SHEET_SIZE;
	hal_auadcBufferSet(auadcCtrl.halPcmType, auadcCtrl.curBuffer, auadcCtrl.curLen);

	ax32xx_auadcSetSampleSet(frq);

	ax32xx_auadcHalfIRQRegister(hal_auadcCallBackHalf);
	ax32xx_auadcEndIRQRegister(hal_auadcCallBackEnd);
	ax32xx_auadcEnable(1, 0, 0);
#if 0
	ax32xx_auadcAGCEnable(1);
#else
	ax32xx_auadcGainSet(0x8);
#endif
	return 0;
}
int hal_auadcStartForAVIB(_PCM_FRQ_ frq, int volume)
{
	u32 memaddr;
	auadcCtrlB.halPcmType = PCM_REC_TYPE_AVI;
	// auadcCtrl.dropFlag = 0;
	// hal_audc_33ms_syncnt_clr();
	/*if(hal_auadcMemInit()<0)
	{
		debg("HAL : [AUADC]<ERROR> memory malloc fial.\n");
		return -1;
	}*/
	auadcCtrlB.volume = volume;
	memaddr = auadcCtrlB.pcmbuf;
	/*if(volume==0)
	{
		auadcCtrl.pcmsize-=PCM_SHEET_SIZE;
		memaddr+=PCM_SHEET_SIZE;
		memset((void *)auadcCtrl.pcmbuf,0,HAL_CFG_PCM_BUFFER_SIZE);
	}*/
	// memset((void *)auadcbuff,0,PCM_SHEET_SIZE);
	hal_streamInit(&auadcCtrlB.auds, auadcCtrlB.auadcNode, PCM_ITEM_NUM, (u32)memaddr, auadcCtrlB.pcmsize);
	// auadcCtrlB.curBuffer = hal_streamMalloc(&auadcCtrlB.auds,PCM_SHEET_SIZE);
	// if(auadcCtrlB.curBuffer==0)
	//	return -1;
	auadcCtrlB.curLen = PCM_SHEET_SIZE;
	auadc_back = true;
	// hal_auadcBufferSet(auadcCtrl.halPcmType,auadcCtrl.curBuffer,auadcCtrl.curLen);

	// ax32xx_auadcSetSampleSet(frq);

	// ax32xx_auadcHalfIRQRegister(hal_auadcCallBackHalf);
	// ax32xx_auadcEndIRQRegister(hal_auadcCallBackEnd);
	// ax32xx_auadcEnable(1);
#if 0
	//ax32xx_auadcAGCEnable(1);
#else
	// ax32xx_auadcGainSet(0x8);
#endif
	return 0;
}
/*******************************************************************************
* Function Name  : hal_auadcBufferGet
* Description    : hal layer.get pcm raw data addr & length
* Input          : u32 *addr : addr
					  u32 *len : length
* Output         : None
* Return         : int : 0: data ready
*******************************************************************************/
void *hal_auadcBufferGet(u32 *addr, u32 *len, s32 *syncnt, s32 *syncnt_next)
{
	u32 buffer, size;
	s32 sync, sync_next;
	*syncnt = 0;
	*syncnt_next = 0;
	if (hal_streamOut(&auadcCtrl.auds, &buffer, &size, &sync, &sync_next) < 0)
		return NULL;
	if (auadcCtrl.volume == 0)
	{
		// buffer = auadcCtrl.pcmbuf;
		buffer = (u32)auadcbuff;
		size = PCM_SHEET_SIZE;
	}
	if (addr)
		*addr = buffer;
	if (len)
		*len = size;
	if (syncnt)
		*syncnt = sync;
	if (syncnt_next)
		*syncnt_next = sync_next;
	return ((void *)buffer);
}
void *hal_auadcBufferfree(void)
{
	hal_streamfree(&auadcCtrl.auds);
	return NULL;
}
void *hal_auadcBufferGetB(u32 *addr, u32 *len, s32 *syncnt, s32 *syncnt_next)
{
	u32 buffer, size;
	s32 sync, sync_next;
	*syncnt = 0;
	*syncnt_next = 0;
	if (hal_streamOut(&auadcCtrlB.auds, &buffer, &size, &sync, &sync_next) < 0)
		return NULL;
	if (auadcCtrlB.volume == 0)
	{
		// buffer = auadcCtrl.pcmbuf;
		buffer = (u32)auadcbuff;
		size = PCM_SHEET_SIZE;
	}
	if (addr)
		*addr = buffer;
	if (len)
		*len = size;
	if (syncnt)
		*syncnt = sync;
	if (syncnt_next)
		*syncnt_next = sync_next;
	return ((void *)buffer);
}
void *hal_auadcBufferfreeB(void)
{
	hal_streamfree(&auadcCtrlB.auds);
	return NULL;
}

void hal_auadcStart(void)
{
	ax32xx_auadcEnable(1, 0, 0);
}
/*******************************************************************************
 * Function Name  : hal_auadcStop
 * Description    : hal layer.auadc stop
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void hal_auadcStop(void)
{
	ax32xx_auadcEnable(0, 0, 0);

	// hal_auadcMemUninit();		//取消buf释放

	// hal_auadcMemUninitB();
}

/*******************************************************************************
 * Function Name  : hal_auadcBufferRelease
 * Description    : hal layer.set pcm raw data addr & length
 * Input          : void *buffer : buffer addr
 * Output         : None
 * Return         : none
 *******************************************************************************/
void hal_auadcBufferRelease(void *buffer)
{
	if (buffer)
	{
		hal_auadcBufferfree();
	}
}
void hal_auadcBufferReleaseB(void *buffer)
{
	if (buffer)
	{
		hal_auadcBufferfreeB();
	}
}

bool hal_adcBuffer_prefull(void)
{
	s32 size;
	size = hal_stream_size(&auadcCtrl.auds);
	if (size < 0)
		return false;
	if (size > (auadcCtrl.pcmsize * 3 / 4))
		return true;
	return false;
}
bool hal_adcBuffer_halffull(void)
{
	return false;
	s32 size;
	size = hal_stream_size(&auadcCtrl.auds);
	if (size < 0)
		return false;
	if (size > (auadcCtrl.pcmsize / 4))
		return true;
	return false;
}
void hal_adc_volume_set(int volume)
{
	auadcCtrl.volume = volume;
}
void hal_adc_volume_setB(int volume)
{
	auadcCtrlB.volume = volume;
}

void hal_test()
{
	if (hal_auadcMemInit() < 0)
	{
		printf("hal_auadcMemInit fail\r\n");
		return;
	}
	else
	{
		printf("hal_malloc success!!!\r\n");
	}
	ax32xx_auadcInit();
	// hal_auadcStartForWAV(ADC_SAMPLEREATE_16000,100);
}

volatile u8 *pcm_buf;
#define __PCM_PLOAD__ (__PCM_SAMPL__ / 1000) * 2

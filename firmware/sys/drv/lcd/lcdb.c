/**@file lcdb.c
 * @brief driver for lcd with double buffer registers
 */
#include "ax329x_soc_drv.h"

#include "stream/stream.h"
#include "osal/osal_alloc.h"
#include "drv/lcd/lcdstruct.h"
#include "debug.h"
#include "sysCommon/commonInclude.h"
#include "osal/osal_task.h"
#include "lcdframe.h"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
#include "lcdrotate.h"
#include "task/app.h"
#include "drv/jpeg/jpeg.h"

////////////////////////////////////////////////////////////////////////////////
static u32 lcdLastUpDateTime=0;
static u32 PIP_W;
static u32 PIP_H;
static u32 PIP_TOP;
#define Y_TOP(fr) (PIP_TOP*fr->stride)
#define UV_TOP(fr) (PIP_TOP*fr->stride>>1)
int lcdTimeOutCnt;

#define SHOW_DEFAULT   0
#define SHOW_FRONT     1
#define SHOW_USB1      2
#define SHOW_USB2      3
#define SHOW_BIG_USB1      4
#define SHOW_BIG_USB2      5
static u32 usbRes[SHOW_BIG_USB2+1][2];
static struct lcd_infors lcdInfors;
int lcdGetRotate(void);
void csiLcdRestart1(struct frame_s *f);
void usbJpgDecCheck(void *d);

int lcdDeBusy(void)
{
	if(lcdc_de_status() & BIT(9))
	{
		printf("de busy\n");
	}
	//if((lcdInfors.lcdpanel->lcd_bus_type&&(lcdc_de_status() & BIT(9))) ||(lcdInfors.lcdpanel->lcd_bus_type==0&&lcdc_de_doublebuffer()))
	if(lcdc_de_doublebuffer()&&((lcdc_de_status() & BIT(9))==0))
		return 1;
	return 0;
}

int lcdTimeOutUpdateEn(void)
{
	if(os_time_get()<lcdLastUpDateTime)
		lcdLastUpDateTime=os_time_get();
	if(os_time_get()-lcdLastUpDateTime>=500)
		return 1;
	return 0;
}
void lcdModeUsbResInit(void)
{	
	u16 w,h;
	lcdGetVideoRes(&w,&h);
	PIP_W=(w/3)&(~0xf);
	PIP_H=PIP_W*3/4;
	PIP_TOP=((25*h)/320)&(~0x1);
	usbRes[SHOW_DEFAULT][0]=PIP_W;
	usbRes[SHOW_DEFAULT][1]=PIP_H;
	usbRes[SHOW_FRONT][0]=32;
	usbRes[SHOW_FRONT][1]=24;
	usbRes[SHOW_USB1][0]=w;
	usbRes[SHOW_USB1][1]=h;
	usbRes[SHOW_USB2][0]=w;
	usbRes[SHOW_USB2][1]=h;
	usbRes[SHOW_BIG_USB1][0]=w;
	usbRes[SHOW_BIG_USB1][1]=h;
	usbRes[SHOW_BIG_USB2][0]=w;
	usbRes[SHOW_BIG_USB2][1]=h;
	printf("PIP_W:%d,PIP_H:%d\n",PIP_W,PIP_H);
}
void lcdTimerNull(void *d)
{
}
void lcdShowModeChange(void)
{
	if(getUsbCameraConnect()||getUsb2CameraConnect())
	{
		if(lcdInfors.showMode==SHOW_DEFAULT)
			lcdInfors.showModeSw=SHOW_FRONT;
		else if(lcdInfors.showMode==SHOW_FRONT&&getUsbCameraConnect())
			lcdInfors.showModeSw=SHOW_USB1;
		else if(lcdInfors.showMode==SHOW_FRONT&&getUsb2CameraConnect())
			lcdInfors.showModeSw=SHOW_USB2;
		else if(lcdInfors.showMode==SHOW_USB1&&getUsb2CameraConnect())
			lcdInfors.showModeSw=SHOW_USB2;
		else if(lcdInfors.showMode==SHOW_USB2&&getUsbCameraConnect())
			lcdInfors.showModeSw=SHOW_BIG_USB1;		
		else if(lcdInfors.showMode==SHOW_USB1&&getUsbCameraConnect())
			lcdInfors.showModeSw=SHOW_BIG_USB1;	
		else if(lcdInfors.showMode==SHOW_BIG_USB1&&getUsb2CameraConnect())
			lcdInfors.showModeSw=SHOW_BIG_USB2;		
		else if(lcdInfors.showMode==SHOW_USB2&&getUsb2CameraConnect())
			lcdInfors.showModeSw=SHOW_BIG_USB2;	
		else
			lcdInfors.showModeSw=SHOW_DEFAULT;
	}
}
void lcdShowAsternMode(bool show)
{
	if (show)
	{
		if (api_astern2_status())
			lcdInfors.showModeSw=SHOW_USB2;//SHOW_USB2;
		else
			lcdInfors.showModeSw=SHOW_USB1;//SHOW_USB2;
	}
	else
		lcdInfors.showModeSw=SHOW_DEFAULT;
}

u32 getCurInputSource(void)
{
	if(curFuncWifi())
		return packerGetCurInput();
	if(lcdInfors.showModeSw)
		return (lcdInfors.showModeSw-1);
	return 0;
}
static u8 lastLevel=0xff;
void lcd_set_crop_csi_to_lcd(u8 crop_level)
{
	
	u8 levels[] = {100,80,60,40};
	if(lastLevel==crop_level)
		return;
	lastLevel=crop_level;
	if(crop_level<sizeof(levels)/sizeof(levels[0]))
	{
		_Sensor_Adpt_ *sensor = sensor_adapt_get();
        u16 csi_w,csi_h,crop_w,crop_h;
        u32 l = levels[crop_level];
        
        crop_w = (sensor->pixelw * l / 100) & ~1;
        crop_h = (sensor->pixelh * l / 100) & ~1;
		
        lcdInfors.crop_x= (sensor->pixelw - crop_w) / 2;
        lcdInfors.crop_w = lcdInfors.crop_x + crop_w;
        lcdInfors.crop_y = (sensor->pixelh - crop_h) / 2;
        lcdInfors.crop_h = lcdInfors.crop_y + crop_h;
		lcdInfors.crop_set=1;
		printf("lcd:%d,x:%d,y:%d,x1:%d,y1:%d\n",crop_level,lcdInfors.crop_x,lcdInfors.crop_w,lcdInfors.crop_y,lcdInfors.crop_h);
	}
}
void getsensorCropArg(u32 *x,u32 *y,u32 *x1,u32 *y1)
{
    u8 levels[] = {100,80,60,40};

	if(lastLevel<sizeof(levels)/sizeof(levels[0]))
	{
		_Sensor_Adpt_ *sensor = sensor_adapt_get();
        u16 csi_w,csi_h,crop_w,crop_h;
        u32 l = levels[lastLevel];
        
        crop_w = ((sensor->pixelw-320) * l / 100) & ~1;
        crop_h = (sensor->pixelh * l / 100) & ~1;
		
        *x= (sensor->pixelw - crop_w) / 2;
        *x1 = *x + crop_w;
        *y = (sensor->pixelh - crop_h) / 2;
        *y1 = *y + crop_h;
		// printf("sensor:%d,x:%d,y:%d,x1:%d,y1:%d\n",lastLevel,lcdInfors.crop_x,lcdInfors.crop_w,lcdInfors.crop_y,lcdInfors.crop_h);
	}
}
void getLcdCropArg(u32 *x,u32 *y,u32 *x1,u32 *y1)
{
	if(x)
    	*x=lcdInfors.crop_x;
    if(x1)
    	*x1=lcdInfors.crop_w;
    if(y)
    	*y=lcdInfors.crop_y;
    if(y1)
    	*y1=lcdInfors.crop_h;
}

static void lcdFrameCfg(struct lcd_infors *lcdInfors,disp_frame_t * showFrame)
{
	if(showFrame->frame_type==FTYPE_VIDEO_DISPLAY)
	{
		if(lcdInfors->x!=showFrame->posX||lcdInfors->y!=showFrame->posY||lcdInfors->w!=showFrame->w||lcdInfors->h!=showFrame->h||lcdInfors->scan_mode!=showFrame->scan_mode)
		{
			lcdInfors->x=showFrame->posX;
			lcdInfors->y=showFrame->posY;
			lcdInfors->w=showFrame->w;
			lcdInfors->h=showFrame->h;
			lcdInfors->scan_mode=showFrame->scan_mode;
			ax32xx_videoSetSize(lcdInfors->w, lcdInfors->h);
			ax32xx_videoSetStride(showFrame->stride, showFrame->stride); // todo
			ax32xx_videoSetScanMode(showFrame->scan_mode);
			ax32xx_videoSetPosition(lcdInfors->x, lcdInfors->y);
			//ax32xx_osdSetUpscaler(lcdInfors->h, lcdInfors->w, lcdInfors->h, lcdInfors->w, NULL);
			ax32xx_osdSetUpscaler(lcdInfors->w, lcdInfors->h, lcdInfors->w, lcdInfors->h, NULL);
			printf("lcd video x:%d,y:%d,w:%d,h:%d,stride:%d,scan:%d\n",lcdInfors->x,lcdInfors->y,lcdInfors->w,lcdInfors->h,showFrame->stride,showFrame->scan_mode);
		}
	}
	else if(showFrame->frame_type==FTYPE_OSD_DISPLAY)
	{
		if(lcdInfors->osdx!=showFrame->posX||lcdInfors->osdy!=showFrame->posY||lcdInfors->osdw!=showFrame->w||lcdInfors->osdh!=showFrame->h)
		{
			lcdInfors->osdx=showFrame->posX;
			lcdInfors->osdy=showFrame->posY;
			lcdInfors->osdw=showFrame->w;
			lcdInfors->osdh=showFrame->h;
			
			ax32xx_osdSetPosition(0,lcdInfors->osdx, lcdInfors->osdy);
			ax32xx_osdSetSize(0,lcdInfors->osdw,lcdInfors->osdh);
			ax32xx_osdSetAlpha(0, 0, 0);
			lcdc_osd_enable(0, 1);
			printf("lcd osd x:%d,y:%d,w:%d,h:%d,scan:%d\n",lcdInfors->osdx,lcdInfors->osdy,lcdInfors->osdw,lcdInfors->osdh,showFrame->scan_mode);
		}
	}
}
static void lcdFrameEndIsr(struct lcd_infors *lcdInfors)
{
#if 1
	uint32_t stride;
	disp_frame_t * ready=lcdInfors->ready;
	static u32 lastTime;
	static u32 frameReadyCnt;
	if(ax32xx_deGetUpdateStatus())
	{
		frameReadyCnt++;
		//printf("frame ready:%d\n",frameReadyCnt);
	}
	if(os_time_get()-lastTime>20030)
	{
		lastTime=os_time_get();
		printf("lcd frame end:0X%X,0X%X,%d,%d\n",ready,lcdInfors->displaying,frameReadyCnt,lcdTimeOutCnt);
	}
	
	if(ready==NULL)
	{
		goto END;
	}
	if(ready==lcdInfors->displaying)
	{
		lcdInfors->ready=NULL;
		printf("display frame err:0X%X\n\n",ready);
		goto END;
	}
	lcddev_t *panel = &lcdInfors->lcdpanel;

	lcdFrameCfg(lcdInfors,ready);
	lcdc_set_fbuf(0, ready->_y_addr,ready->_uv_addr);
	if(lcdInfors->osdUpdate)
	{
		lcdInfors->osdUpdate=0;
		lcdc_set_fbuf(1, lcdInfors->osdDisplaying->y_addr,NULL);
	}
	lcdc_de_update();
	lcdframeFree(lcdInfors->displaying);
	lcdInfors->displaying=ready;
	lcdInfors->ready=NULL;
	lcdLastUpDateTime=os_time_get();
	return;
END:
	if(lcdInfors->osdUpdate)
	{
		lcdInfors->osdUpdate=0;
		lcdc_set_fbuf(1, lcdInfors->osdDisplaying->y_addr,NULL);
		lcdc_de_update();
		lcdLastUpDateTime=os_time_get();
	//	printf("$");
	}
	else
	{
		//if(lcdInfors->lcdpanel->te_mode==1)
		if(lcdInfors->lcdpanel->lcd_bus_type == LCD_BUS_I80)
		{
			lcdc_de_update();
			lcdLastUpDateTime=os_time_get();
		}
	}
	
#endif
}

void lcdSetVideoBuffer(disp_frame_t * showFrame)
{
	static u8 first=1;
	if(showFrame==NULL)
		return;
//	static u32 lastTime;
//	if(os_time_get()-lastTime>20030)
//	{
//		lastTime=os_time_get();
//		printf("de ready:0X%X,0X%X\n",showFrame,lcdInfors.displaying);
//	}
	lcdcIrqEn(0);
	if(lcdInfors.ready)
		lcdframeFree(lcdInfors.ready);
	lcdInfors.ready=showFrame;
	lcdcIrqEn(1);
}

void lcdDealVideBuff(disp_frame_t * frame)
{
	static u32 lastTime;
	static u32 dealVidCnt=0;
	static u32 dropVidCnt=0;
	lcdInfors.curInputData = frame;
    if(frame->scan_mode == LCD_ROTATE_90)
    {
        disp_frame_t * dst = lcdframeMalloc(FTYPE_VIDEO_DISPLAY);
        if(dst)
        {
        	//printf("R");
            lcdRotateAdd(ROTATE_CH0,frame,dst);
			dealVidCnt++;
        }
        else
        {
        	dropVidCnt++;
			if(os_time_get()-lastTime>20000)
			{
				lastTime=os_time_get();
				printf("dealVide:%d,drop:%d,TO:%d\n",dealVidCnt,dropVidCnt,lcdTimeOutCnt);
				dropVidCnt=dealVidCnt=0;
			}
        	void (*priv_func)(struct frame_s *);
			priv_func=frame->priv_func;
            lcdframeFree(frame);
			if(priv_func)
				priv_func(NULL);
        }
    }
    else
    {
    	void (*priv_func)(struct frame_s *);
		priv_func=frame->priv_func;
        lcdSetVideoBuffer(frame);
		if(priv_func)
			priv_func(NULL);
    }
}

void lcdOsdEn(u8 en)
{
	lcdc_osd_enable(0, en);
}
u8* getOsdBuff(void)
{
#if LCD_EN
	return lcdInfors.osdDrawFrame->y_addr;
#else
	return NULL;
#endif
}
void lcdOsdShow(void)
{
#if LCD_EN
	static u32 lastTime=0;
	u32 interval=os_time_get()-lastTime;
	if(interval<30)
	{
		if(interval<10)
			osal_task_sleep(30-interval);
		else
			osal_task_sleep(30);
	}
	lastTime=os_time_get();
	disp_frame_t * showFrame=lcdInfors.osdDrawFrame;

	dcache_writeback_region(showFrame->y_addr,showFrame->buf_size);
	lcdFrameCfg(&lcdInfors,showFrame);

	lcdInfors.osdDrawFrame=lcdInfors.osdDisplaying;
	lcdInfors.osdDisplaying=showFrame;
	lcdInfors.osdUpdate=1;
	lcdInfors.osdCopyEn=1;
#endif
}

void lcdForceOsdShow(void)
{
#if LCD_EN
	disp_frame_t * showFrame=lcdInfors.osdDrawFrame;

	dcache_writeback_region(showFrame->y_addr,showFrame->buf_size);
	lcdFrameCfg(&lcdInfors,showFrame);

	lcdInfors.osdDrawFrame=lcdInfors.osdDisplaying;
	lcdInfors.osdDisplaying=showFrame;
	lcdInfors.osdUpdate=1;
	lcdInfors.osdCopyEn=1;
#endif
}

void lcdOsdDeal(void)
{
#if LCD_EN
	if(lcdInfors.osdCopyEn==0)
		return;
	while(lcdInfors.osdUpdate)
	{
		osal_task_sleep(10);
	}
	dma_memcpy(lcdInfors.osdDrawFrame->y_addr,lcdInfors.osdDisplaying->y_addr,lcdInfors.osdDisplaying->buf_size);
#endif	
}
void lcdOsdClear(u8 color)
{
#if LCD_EN
	memset(lcdInfors.osdDisplaying->y_addr,color,lcdInfors.osdDisplaying->buf_size);
	dcache_writeback_region(lcdInfors.osdDisplaying->y_addr,lcdInfors.osdDisplaying->buf_size);
	if(!lcdDeBusy())
	{
		//lcdc_set_fbuf(1, lcdInfors.osdDisplaying->y_addr,NULL);
		lcdFrameCfg(&lcdInfors,lcdInfors.osdDisplaying);
	}
	lcdInfors.osdUpdate=1;
	osal_task_sleep(50);
#endif
}

void lcdDeUpdata(void *d)
{
	static u32 lastTime;
	if(os_time_get()-lastTime>20000)
	{
		lastTime=os_time_get();
		printf("lcdTimeOutCnt:%d\n",lcdTimeOutCnt);
	}
	if(lcdTimeOutUpdateEn())
	{
		lcdTimeOutCnt++;
		if(lcdInfors.osdUpdate)
		{
			lcdInfors.osdUpdate=0;
			lcdc_set_fbuf(1, lcdInfors.osdDisplaying->y_addr,NULL);
		}
		lcdc_de_update();
		if(lcdInfors.lcdpanel->lcd_bus_type == LCD_BUS_I80)
			ax32xx_lcdKick();
		lcdLastUpDateTime=os_time_get();
	}
}
extern struct frame *getUsbJpgFrame(void);
extern int jpgDecFrameEx(struct frame *f,uint8_t *yuvBUf,uint8_t *uvBUf,uint32_t w,uint32_t h,uint32_t stride,imageDecDone done);
void csiLcdRestart(void)
{
	if(getLcdDebugEn())
	{
		printf("++++++%s,---%d\n\n",__FUNCTION__,__LINE__);
	}
	while(lcdInfors.preview==NULL)
	{
		printf("%s,%d,preview NULL\n",__FUNCTION__,__LINE__);
	}
	lcdframeVideoCfg(lcdInfors.preview,lcdInfors.lcdpanel->video_x,lcdInfors.lcdpanel->video_y,lcdInfors.lcdpanel->video_w,lcdInfors.lcdpanel->video_h);
    ax32xx_csiLCDFrameSet((u32)lcdInfors.preview->y_addr,(u32)lcdInfors.preview->uv_addr);
	ax32xx_csiLCDDmaKick();
}
void lcdJpgShowTimer(void)
{
	irqTimerModFunc(lcdInfors.lcdTimer,10000000, lcdTimerNull, NULL);
	lcdInfors.preview->priv_func=csiLcdRestart1;
	disp_frame_t * temp=lcdInfors.preview;
	lcdInfors.preview = NULL;
	addPicStickerB(temp->y_addr);
	animationRun(temp->y_addr);
	lcdDealVideBuff(temp);
}
void usb2JpgDecDone(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsb2QueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsb2ManualInput(lcdInfors.curFrame);
	}
	else
	{
		//printf("@%d\n", picComUsb2QueueValidData());
		usb2CheckFrameSend(lcdInfors.curFrame);
	}
	lcdInfors.curFrame=NULL;
	if(lcdInfors.showMode==SHOW_DEFAULT)
	{
		irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
	}
	else if(lcdInfors.showMode==SHOW_FRONT)
	{
		csiLcdRestart();
	}
	else if(lcdInfors.showMode==SHOW_BIG_USB2)
	{
	    ax32xx_csiLCDFrameSet((u32)lcdInfors.preview->y_addr,(u32)lcdInfors.preview->uv_addr);
		ax32xx_csiLCDDmaKick();
	}
	else
	{
		irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
	}
}
void usb2JpgDecDoneStartCsi(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsb2QueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsb2ManualInput(lcdInfors.curFrame);
	}
	else
	{
		//printf("@%d\n", picComUsb2QueueValidData());
		usb2CheckFrameSend(lcdInfors.curFrame);
	}
	lcdInfors.curFrame=NULL;
	//csiLcdRestart();
    ax32xx_csiLCDFrameSet((u32)lcdInfors.preview->y_addr,(u32)lcdInfors.preview->uv_addr);
	ax32xx_csiLCDDmaKick();

}
void usb1JpgDecDoneStartCsi(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsbQueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsbManualInput(lcdInfors.curFrame);
	}
	else
	{
		usb1CheckFrameSend(lcdInfors.curFrame);
	}
	lcdInfors.curFrame=NULL;
	//csiLcdRestart();
    ax32xx_csiLCDFrameSet((u32)lcdInfors.preview->y_addr,(u32)lcdInfors.preview->uv_addr);
	ax32xx_csiLCDDmaKick();

}
void usb1JpgDecDone(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsbQueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsbManualInput(lcdInfors.curFrame);
	}
	else
	{
		usb1CheckFrameSend(lcdInfors.curFrame);
	}
	lcdInfors.curFrame=NULL;
	if(lcdInfors.showMode==SHOW_DEFAULT)
	{
		irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
	}
	else if(lcdInfors.showMode==SHOW_FRONT)
	{
		csiLcdRestart();
	}
	else if (lcdInfors.showMode==SHOW_BIG_USB1)
	{
		ax32xx_csiLCDFrameSet((u32)lcdInfors.preview->y_addr,(u32)lcdInfors.preview->uv_addr);
		ax32xx_csiLCDDmaKick();
	}
	else
	{
		irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
	}
}

void usb2JpgDecPic(void *d)
{
	disp_frame_t * ready = lcdInfors.preview;
	irqTimerModFunc(lcdInfors.lcdTimer,10000000, lcdTimerNull, NULL);
	lcdInfors.curFrame=getUsb2JpgFrame();
	if(lcdInfors.curFrame)
	{
		if(lcdInfors.showMode==SHOW_DEFAULT)
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb2JpgDecDone)>=0)
				return;
		}
		else if(lcdInfors.showMode==SHOW_BIG_USB1)
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb2JpgDecDoneStartCsi)>=0)
				return;
		}
		else
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2JpgDecDone)>=0)
				return;
		}
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
	}
	if(lcdInfors.showMode<=SHOW_FRONT)
		csiLcdRestart();
	else
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
}
void usb1DecDoneStartUsb2DecTimer(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsbQueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsbManualInput(lcdInfors.curFrame);
	}
	else
	{
		usb1CheckFrameSend(lcdInfors.curFrame);
	}
	lcdInfors.curFrame=NULL;
	irqTimerModFunc(lcdInfors.lcdTimer,1, usb2JpgDecPic, NULL);
}
void usb1JpgDecPic(void *d)
{
	disp_frame_t * ready = lcdInfors.preview;
	irqTimerModFunc(lcdInfors.lcdTimer,10000000, lcdTimerNull, NULL);
	lcdInfors.curFrame=getUsbJpgFrame();
	if(lcdInfors.curFrame)
	{
		if(lcdInfors.showMode==SHOW_DEFAULT)
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb1JpgDecDone)>=0)
				return;
		}
		else if(lcdInfors.showMode==SHOW_BIG_USB2)
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb1JpgDecDoneStartCsi)>=0)
				return;
		}
		else
		{
			if(jpgDecFrameEx(lcdInfors.curFrame,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1JpgDecDone)>=0)
				return;
		}
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
	}
	if(lcdInfors.showMode<=SHOW_FRONT)
		csiLcdRestart();
	else
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
}

void usb2DecDoneStartUsb1DecTimer(void *d)
{
	disp_frame_t * next = NULL;
	if(jpgDecSta()<0)
	{
		freeFrame(lcdInfors.curFrame);
		lcdInfors.curFrame=NULL;
		if(lcdInfors.showMode<=SHOW_FRONT)
			csiLcdRestart();
		else
			irqTimerModFunc(lcdInfors.lcdTimer,20, usbJpgDecCheck, NULL);
		return;
	}
	if (picComUsb2QueueValidData()<1)
	{
		//printf("=====%s,%d\n", __FUNCTION__, __LINE__);
		picComUsb2ManualInput(lcdInfors.curFrame);
	}
	else
	{
		//printf("@%d\n", picComUsb2QueueValidData());
		usb2CheckFrameSend(lcdInfors.curFrame);
	}	
	lcdInfors.curFrame=NULL;
	irqTimerModFunc(lcdInfors.lcdTimer,1, usb1JpgDecPic, NULL);
}
void usbJpgDecCheck(void *d)
{
	struct frame *f;
	if(lcdInfors.preview==NULL)
		return;
	disp_frame_t * ready = lcdInfors.preview;
	if(lcdInfors.showMode==SHOW_USB1)
	{
		PIP_TOP=0;
		if(!getUsbCameraConnect())
		{
			lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
			csiLcdRestart();
			return;
		}
		if(getUsb2CameraConnect())
		{
			f=getUsb2JpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb2DecDoneStartUsb1DecTimer)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		f=getUsbJpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1JpgDecDone)>=0)
				return ;
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}	
	else if(lcdInfors.showMode==SHOW_BIG_USB1)
	{
		PIP_TOP=0;
		if(!getUsbCameraConnect())
		{
			lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
			csiLcdRestart();
			return;
		}

		f=getUsbJpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(getUsb2CameraConnect())
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
					return ;
			}
			else
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1JpgDecDone)>=0)
					return ;
			}			
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		
		if(getUsb2CameraConnect())
		{
			f=getUsb2JpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				
				if(jpgDecFrameEx(f,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W, PIP_H,ready->stride,usb2JpgDecDoneStartCsi)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}
	else if(lcdInfors.showMode==SHOW_BIG_USB2)
	{
		PIP_TOP=0;
		if(!getUsb2CameraConnect())
		{
			lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
			csiLcdRestart();
			return;
		}

		f=getUsb2JpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(getUsbCameraConnect())
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2DecDoneStartUsb1DecTimer)>=0)
					return ;
			}
			else
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2JpgDecDone)>=0)
					return ;
			}			
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		
		if(getUsbCameraConnect())
		{
			f=getUsbJpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W, PIP_H,ready->stride,usb1JpgDecDoneStartCsi)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}	
	else if(lcdInfors.showMode==SHOW_USB2)
	{
		PIP_TOP=0;
		if(!getUsb2CameraConnect())
		{
			lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
			csiLcdRestart();
			return;
		}
		if(getUsbCameraConnect())
		{
			f=getUsbJpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		f=getUsb2JpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2JpgDecDone)>=0)
				return ;
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}
}
void csiLcdRestart1(struct frame_s *ff)
{
	disp_frame_t * ready;
	struct frame *f;
	_Sensor_Adpt_ *sensor;
	if(lcdInfors.previewEn==0)
	{
		while(1)// lyc debug
		{
			printf("lcd preview disable\n");
		}
	}
	lcdInfors.preview=lcdVideoFrameMalloc();
	while(lcdInfors.preview==NULL)
	{
		lcdInfors.preview=lcdVideoFrameMalloc();
		printf("lcdVideoFrameMalloc err\n");
	}
	ready = lcdInfors.preview;
RETRY:
	if 	(((lcdInfors.showMode==SHOW_BIG_USB1)|| (lcdInfors.showMode==SHOW_BIG_USB2)) && (lcdInfors.showModeSw<SHOW_BIG_USB1))
	{
		sensor = sensor_adapt_get();
		ax32xx_csiLCDScaler(sensor->pixelw, sensor->pixelh,
							lcdInfors.lcdpanel->video_w,lcdInfors.lcdpanel->video_h,
							lcdInfors.crop_x,lcdInfors.crop_y,lcdInfors.crop_w,lcdInfors.crop_h,
							(lcdInfors.lcdpanel->video_w + 31) & ~31,
							(u32 *)lcdInfors.lcdpanel->lcd_lsawtooth.anti_lsawtooth);
	}
	else if ((((lcdInfors.showModeSw==SHOW_BIG_USB1)&&(getUsbCameraConnect()))|| ((lcdInfors.showModeSw==SHOW_BIG_USB2) && (getUsb2CameraConnect()))) && (lcdInfors.showMode<SHOW_BIG_USB1))
	{
		sensor = sensor_adapt_get();
		ax32xx_csiLCDScaler(sensor->pixelw, sensor->pixelh,
							PIP_W,PIP_H,
							lcdInfors.crop_x,lcdInfors.crop_y,lcdInfors.crop_w,lcdInfors.crop_h,
							(lcdInfors.lcdpanel->video_w + 31) & ~31,
							(u32 *)lcdInfors.lcdpanel->lcd_lsawtooth.anti_lsawtooth);
	}
	
	lcdInfors.showMode=lcdInfors.showModeSw;
//RETRY:
	if(lcdInfors.showMode==SHOW_DEFAULT)
	{
		csiLcdRestart();
	}
	else if(lcdInfors.showMode==SHOW_FRONT)
	{
		if(getUsbCameraConnect()||getUsb2CameraConnect())
		{
			f=getUsbJpgFrame();
			if(f)
			{
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(getUsb2CameraConnect())
				{
					if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
						return ;
				}
				else
				{
					if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb1JpgDecDone)>=0)
						return ;
				}
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
			f=getUsb2JpgFrame();
			if(f)
			{
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb2JpgDecDone)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		csiLcdRestart();
	}
	else if(lcdInfors.showMode==SHOW_USB1)
	{
		PIP_TOP=0;
		if(!getUsbCameraConnect())
		{
			lcdInfors.showModeSw/*=lcdInfors.showMode*/=SHOW_DEFAULT;
			goto RETRY;
		}
		if(getUsb2CameraConnect())
		{
			f=getUsb2JpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb2DecDoneStartUsb1DecTimer)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		f=getUsbJpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1JpgDecDone)>=0)
				return ;
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}	
	else if(lcdInfors.showMode==SHOW_BIG_USB1)
	{
		PIP_TOP=0;
		if(!getUsbCameraConnect())
		{
			lcdInfors.showModeSw=/*lcdInfors.showMode=*/SHOW_DEFAULT;
			goto RETRY;
		}

		f=getUsbJpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(getUsb2CameraConnect())
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
					return ;
			}
			else
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb1JpgDecDone)>=0)
					return ;
			}
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}

		if(getUsb2CameraConnect())
		{
			f=getUsb2JpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W, PIP_H,ready->stride,usb2JpgDecDoneStartCsi)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}		
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}
	else if(lcdInfors.showMode==SHOW_BIG_USB2)
	{
		PIP_TOP=0;
		if(!getUsb2CameraConnect())
		{
			lcdInfors.showModeSw=/*lcdInfors.showMode=*/SHOW_DEFAULT;	
			goto RETRY;
		}

		f=getUsb2JpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(getUsbCameraConnect())
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2DecDoneStartUsb1DecTimer)>=0)
					return ;
			}
			else
			{
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2JpgDecDone)>=0)
					return ;
			}
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		
		if(getUsbCameraConnect())
		{
			f=getUsbJpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+PIP_W+Y_TOP(ready),ready->uv_addr+PIP_W+UV_TOP(ready),PIP_W, PIP_H,ready->stride,usb1JpgDecDoneStartCsi)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}

		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}
	else if(lcdInfors.showMode==SHOW_USB2)
	{
		PIP_TOP=0;
		if(!getUsb2CameraConnect())
		{
			lcdInfors.showModeSw=/*lcdInfors.showMode=*/SHOW_DEFAULT;
			goto RETRY;
		}
		if(getUsbCameraConnect())
		{
			f=getUsbJpgFrame();
			if(f)
			{	
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),32,24,ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
					return ;
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
		}
		f=getUsb2JpgFrame();
		if(f)
		{
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=f;
			if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),usbRes[lcdInfors.showMode][0],usbRes[lcdInfors.showMode][1],ready->stride,usb2JpgDecDone)>=0)
				return ;
			freeFrame(lcdInfors.curFrame);
			lcdInfors.curFrame=NULL;
		}
		irqTimerModFunc(lcdInfors.lcdTimer,50, usbJpgDecCheck, NULL);
	}
}
void lcdPrint(void)
{
	printf("preview:0x%x,read:0x%x,display:0x%x\n",lcdInfors.preview,lcdInfors.ready,lcdInfors.displaying);
}
void csiLcdFramEnd(unsigned int irqno, void *d, void *regs)
{
	disp_frame_t * ready = lcdInfors.preview;
    disp_frame_t * next = NULL;
	struct frame *f;
//	static u32 lastTime;
//	if(os_time_get()-lastTime>20030)
//	{
//		lastTime=os_time_get();
//		printf("csiLcdFramEnd\n");
//	}
	if(getLcdDebugEn())
	{
		printf("++++++%s,---%d\n\n",__FUNCTION__,__LINE__);
	}
	if(lcdInfors.previewEn==0)
	{
		while(1)// lyc debug
		{
			printf("lcd preview disable a\n");
		}
	}
	//printf("P:%d%d",lcdc_de_doublebuffer(),(lcdc_de_status()>>9)&1);
	if(!ready)
    {
        printf("lcd preview null\n");
        return;
    }
	if(lcdInfors.disp_drop_frame_num)
	{
		lcdInfors.disp_drop_frame_num--;
		csiLcdRestart();
		return;
	}
	if(lcdInfors.showMode==SHOW_DEFAULT)
	{
		if(getUsbCameraConnect()||getUsb2CameraConnect())
		{
			PIP_TOP=((25*ready->h)/320)&(~0x1);
			f=getUsbJpgFrame();
			if(f)
			{
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=f;
				if(getUsb2CameraConnect())
				{
					if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb1DecDoneStartUsb2DecTimer)>=0)
						return ;
				}
				else
				{
					if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb1JpgDecDone)>=0)
						return ;
				}
				freeFrame(lcdInfors.curFrame);
				lcdInfors.curFrame=NULL;
			}
			else if(getUsbCameraConnect()==0&&getUsb2CameraConnect())
			{
				f=getUsb2JpgFrame();
				if(f)
				{
					freeFrame(lcdInfors.curFrame);
					lcdInfors.curFrame=f;
					if(jpgDecFrameEx(f,ready->y_addr+Y_TOP(ready),ready->uv_addr+UV_TOP(ready),PIP_W,PIP_H,ready->stride,usb2JpgDecDone)>=0)
						return ;
					freeFrame(lcdInfors.curFrame);
					lcdInfors.curFrame=NULL;
				}
			}
			csiLcdRestart();
			return;
		}
	}
	else if((lcdInfors.showMode==SHOW_BIG_USB1) || (lcdInfors.showMode==SHOW_BIG_USB2))
	{
		irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
		return;
	}
	
	if(lcdInfors.crop_set)
	{
		lcdInfors.crop_set=0;
		_Sensor_Adpt_ *sensor = sensor_adapt_get();
		ax32xx_csiLCDScaler(sensor->pixelw, sensor->pixelh,
							lcdInfors.lcdpanel->video_w,lcdInfors.lcdpanel->video_h,
							lcdInfors.crop_x,lcdInfors.crop_y,lcdInfors.crop_w,lcdInfors.crop_h,
							(lcdInfors.lcdpanel->video_w + 31) & ~31,
							(u32 *)lcdInfors.lcdpanel->lcd_lsawtooth.anti_lsawtooth);
	}
	irqTimerModFunc(lcdInfors.lcdTimer,1, lcdJpgShowTimer, NULL);
}
int lcdDebugEn;
void lcdPreviewStart(void)
{
#if LCD_EN
	lcdDebugEn=0;
	static bool first=true;
	_Sensor_Adpt_ *sensor = sensor_adapt_get();
	lcdInfors.previewEn=1;
	lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
	csiirq_register_lcd_frame_end(csiLcdFramEnd, &lcdInfors);
	if(lcdInfors.preview)
	{
		lcdframeFree(lcdInfors.preview);
	}
	lcdInfors.preview=lcdVideoFrameMalloc();
	lcdframeVideoCfg(lcdInfors.preview,lcdInfors.lcdpanel->video_x,lcdInfors.lcdpanel->video_y,lcdInfors.lcdpanel->video_w,lcdInfors.lcdpanel->video_h);
	
	ax32xx_csiLCDScaler(sensor->pixelw, sensor->pixelh,
						lcdInfors.lcdpanel->video_w,lcdInfors.lcdpanel->video_h,
						lcdInfors.crop_x,lcdInfors.crop_y,lcdInfors.crop_w,lcdInfors.crop_h,
						(lcdInfors.lcdpanel->video_w + 31) & ~31,
						(u32 *)lcdInfors.lcdpanel->lcd_lsawtooth.anti_lsawtooth);
	ax32xx_csiOutputSet(CSI_OUTPUT_RELOAD, 0);
	ax32xx_csiOutputSet(CSI_OUTPUT_LDMAMANEN,1);
	ax32xx_csiLCDFrameSet(lcdInfors.preview->y_addr,lcdInfors.preview->uv_addr);
	ax32xx_csiLCDDmaEnable(1);
	csi_enable();
	if(first)
	{
		lcdInfors.disp_drop_frame_num=7;
		osal_task_sleep(500);
		first=false;
	}
	else
	{
		osal_task_sleep(100);
	}
#else
	csiLcdAnti_lsawtoothInit();
	csi_enable();
#endif
}
int getLcdDebugEn(void)
{
	//return lcdDebugEn;
	return 0;
}
void lcdPreviewStop(void)
{
#if LCD_EN
	uint32_t flags;
	flags=arch_local_irq_save();
	lcdDebugEn=1;
	lcdInfors.previewEn=0;
	ax32xx_csiLCDDmaEnable(0);
	lcdRotateStop();
	irqTimerModFunc(lcdInfors.lcdTimer,10000000, lcdTimerNull, NULL);
	jpgDecStop(0);
	freeFrame(lcdInfors.curFrame);
	lcdInfors.curFrame=NULL;
	if(lcdInfors.preview)
		lcdframeFree(lcdInfors.preview);
	lcdInfors.preview=NULL;
	if (functionCurrent() == FUNCTION_WIFI)
	{
		lcdframeFree(lcdInfors.displaying);
		lcdInfors.displaying=NULL;
	}
	if(lcdInfors.ready)
		lcdframeFree(lcdInfors.ready);
	lcdInfors.ready=NULL;
	arch_local_irq_restore(flags);
#endif
}

extern uint32_t _lcd_head_start[];
extern uint32_t _lcd_head_end[];
extern void lcdc_init(lcddev_t *p_lcddev,disp_frame_t * display, u32 res_isp_lcd);
void display_engine_init(struct lcd_infors *lcdInfors, u32 res_isp_lcd)
{
	//lcdc_caloff(&lcdInfors->lcdpanel, &lcdInfors->y_off, &lcdInfors->uv_off);
	if(lcdInfors->lcdpanel)
	{
		disp_frame_t * displaying=lcdframeMalloc(FTYPE_VIDEO_DISPLAY);
		if(displaying==NULL)
		{
			printf("display_engine_init, display frame malloc error\n");
		}
		lcdc_init(lcdInfors->lcdpanel,displaying, res_isp_lcd);
		lcdframeFree(displaying);
	}

}
int lcdParameCheck(lcddev_t *p_lcddev)
{
	INT16U lcdW=0,lcdH=0;
	lcdGetRes(&lcdW,&lcdH);
    if(!p_lcddev->video_w||!p_lcddev->video_h||p_lcddev->video_x>lcdW||p_lcddev->video_y>lcdH)
	{
		p_lcddev->video_x = 0;
		p_lcddev->video_y = 0;
		p_lcddev->video_w = lcdW;
	    p_lcddev->video_h = lcdH;
	}
    //if(!p_lcddev->vsrc_w) p_lcddev->vsrc_w = p_lcddev->video_w;
    //if(!p_lcddev->vsrc_h) p_lcddev->vsrc_h = p_lcddev->video_h;
	if(!p_lcddev->osd_w||!p_lcddev->osd_h||p_lcddev->osd_x>lcdW||p_lcddev->osd_y>lcdH)
	{
		p_lcddev->osd_x = p_lcddev->video_x;
		p_lcddev->osd_y = p_lcddev->video_y;
		p_lcddev->osd_w = p_lcddev->video_w;
    	p_lcddev->osd_h = p_lcddev->video_h;
	}
	if(p_lcddev->video_x+p_lcddev->video_w>lcdW)
		p_lcddev->video_w=lcdW-p_lcddev->video_x;
	if(p_lcddev->video_y+p_lcddev->video_h>lcdH)
		p_lcddev->video_h=lcdH-p_lcddev->video_y;
	if(p_lcddev->osd_x+p_lcddev->osd_w>lcdW)
		p_lcddev->osd_w=lcdW-p_lcddev->osd_x;
	if(p_lcddev->osd_y+p_lcddev->osd_h>lcdH)
		p_lcddev->osd_h=lcdH-p_lcddev->osd_y;
}
int lcdFrameBuffInit(void)
{
	u32 num=((uint32_t)_lcd_head_end-(uint32_t)_lcd_head_start)/sizeof(lcddev_t);
	printf("lcd sum:%d\n",num);
	lcdInfors.lcdpanel=NULL;
	if(num)
	{
		lcdInfors.lcdpanel=_lcd_head_start;
		printf("LCD panel name '%s'\n", lcdInfors.lcdpanel->name);
		lcdParameCheck(lcdInfors.lcdpanel);
		lcdframeInit();
	}
}
int lcdmcu_frame_end(unsigned int irqno, void *d, void *regs)
{
	struct lcd_infors *lcdInfors=d;
	static u32 lastTime;
	static u32 cnt;
	lcdFrameEndIsr(d);
	if(ax32xx_deGetUpdateStatus()==0)
	{
		cnt++;
		if(os_time_get()-lastTime>5030)
		{
			lastTime=os_time_get();
			printf("frame not ready:%d\n",cnt);
			cnt=0;
		}
	}
	if(lcdInfors->lcdpanel->te_mode!=1)
		ax32xx_lcdKick();
	return 0;
}
int lcdrgb_frame_end(unsigned int irqno, void *d, void *regs)
{
	static u32 lastTime;
	static u32 cnt;
	lcdFrameEndIsr(d);
	if(ax32xx_deGetUpdateStatus()==0)
	{
		cnt++;
		if(os_time_get()-lastTime>5030)
		{
			lastTime=os_time_get();
			printf("frame not ready:%d\n",cnt);
			cnt=0;
		}
	}
	return 0;
}
void display_photo(u16 id)
{
}
// 0:游戏 1:正常
void lcd_reconfig(u8 mode)
{
}
void lcdb_config(void *d)
{
}
u8 lcdGetScanMode(void)
{
	return lcdInfors.lcdpanel->scan_mode;
}
int lcdGetRotate(void)
{
	if(lcdInfors.lcdpanel->scan_mode == LCD_ROTATE_270 || lcdInfors.lcdpanel->scan_mode == LCD_ROTATE_90)
	{
		return 1;
	}
	return 0;
}
int lcdGetRes(u16 *w,u16 *h)
{
	if(lcdInfors.lcdpanel->scan_mode == LCD_ROTATE_270 || lcdInfors.lcdpanel->scan_mode == LCD_ROTATE_90)
	{
		if(w)
			*w=lcdInfors.lcdpanel->screen_h;
		if(h)
			*h=lcdInfors.lcdpanel->screen_w;
	}
	else
	{
		if(w)
			*w=lcdInfors.lcdpanel->screen_w;
		if(h)
			*h=lcdInfors.lcdpanel->screen_h;
	}
	return 0;
}
int lcdGetVideoRes(u16 *w,u16 *h)
{
	if(w)
		*w=lcdInfors.lcdpanel->video_w;
	if(h)
		*h=lcdInfors.lcdpanel->video_h;
	return 0;
}
int lcdGetVideoPos(u16 *x,u16 *y)
{
	if(x)
		*x=lcdInfors.lcdpanel->video_x;
	if(y)
		*y=lcdInfors.lcdpanel->video_y;
	return 0;
}
int lcdGetOsdRes(u16 *w,u16 *h)
{
	if(w)
		*w=lcdInfors.lcdpanel->osd_w;
	if(h)
		*h=lcdInfors.lcdpanel->osd_h;
	return 0;
}
int lcdGetOsdPos(u16 *x,u16 *y)
{
	if(x)
		*x=lcdInfors.lcdpanel->osd_x;
	if(y)
		*y=lcdInfors.lcdpanel->osd_y;
	return 0;
}

char *LcdGetName(void)
{
	if(lcdInfors.lcdpanel == NULL)
		return ((char *)("NULL"));

	return lcdInfors.lcdpanel->name;
}

disp_frame_t *LcdGetVideoFrame(void)
{
	return lcdInfors.curInputData;

	//if(lcdInfors.preview == NULL)
	//	return NULL;

	//return lcdInfors.preview;
}
extern void lcd_bk_ctrl(u8 en);
void palette_load(u32 res_palette)
{
  uint32_t pal_addr;
  uint32_t *pal = (uint32_t *)osal_malloc(256 * 4); // lcdc_get_palette();
  int i;
  pal_addr = res_open(res_palette);
  res_read(pal_addr, pal, 256 * 4);
  dcache_writeback_region((u32)pal, 1024);
  lcdc_set_palette(pal);
  osal_free(pal);
}
void lcdDeTimeoutRegister(void)
{
	lcdLastUpDateTime=os_time_get();
	lcdInfors.lcdDeTimer=irqTimerCreate(40, lcdDeUpdata, NULL);
}
void lcdInit(u32 res_palette, u32 res_isp_lcd)
{
#if LCD_EN
	lcdInfors.displaying=NULL;
	lcdInfors.curFrame=NULL;
	lcdInfors.lcdTimer=NULL;
	lcdInfors.ready=NULL;
	lcdInfors.showModeSw=lcdInfors.showMode=SHOW_DEFAULT;
	lcdInfors.osdCopyEn=0;
	lcdRotateInit();
	display_engine_init(&lcdInfors, res_isp_lcd);
	if(lcdInfors.lcdpanel->lcd_bus_type == LCD_BUS_I80)
		lcdcirq_register_data_end(lcdmcu_frame_end, &lcdInfors);
	else
		lcdcirq_register_frame_end(lcdrgb_frame_end, &lcdInfors);
	palette_load(res_palette);
	lcdModeUsbResInit();
	lcdInfors.lcdTimer=irqTimerCreate(10000000, lcdTimerNull, NULL);
	lcdInfors.osdDrawFrame=lcdOsdFrameMalloc();
	lcdInfors.osdDisplaying=lcdOsdFrameMalloc();
#endif
}
void logoShow(u32 id)
{
#if LCD_EN
	u32 id_addr = res_open(id);
	uint32_t jpgbufsize = res_size(id);
	INT16U lcdW=0,lcdH=0,cnt;
	lcdGetRes(&lcdW,&lcdH);
	//jpgbufsize += 64;
	printf("jpgbufsize:%d,lcdw:%d,lcdh:%d\r\n", jpgbufsize,lcdW,lcdH);
	void *jpgbuf = osal_malloc(jpgbufsize+64);
	if(jpgbuf==NULL)
	{
		printf("malloc jpgbuf:%d error\r\n", jpgbufsize);
		return;
	}
	res_read(id_addr, jpgbuf, jpgbufsize);
	dcache_writeback_region((u32)jpgbuf, jpgbufsize);
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		osal_free(jpgbuf);
		printf("malloc lcdframe error\r\n");
		return;
	}
	lcdframeVideoCfg(lcdFrame,0,0,lcdW,lcdH);
	not_isr_jpgdec2(lcdFrame->y_addr, jpgbuf, jpgbufsize, lcdW, lcdH);
	osal_free(jpgbuf);
	//printf("lcdFrame->scan_mode=%d\r\n",lcdFrame->scan_mode);
	cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	lcdDealVideBuff(lcdFrame);
	lcdOsdClear(0xf9/*R_COLOR_TRANSFER*/);
	lcdc_osd_enable(0, 1);
	lcdc_osd_enable(1, 0);
	osal_task_sleep(60);
	lcd_bk_ctrl(1);
	printf("logo[%d] show end\n",id);
#endif
}

void LivePreviewShow(char* name)
{
#if LCD_EN
	void *fp=osal_fopen(name, "r");
	uint32_t jpgbufsize = osal_fsize(fp);
	INT16U lcdW=0,lcdH=0,cnt;
	uint32_t count;
	
	lcdGetRes(&lcdW,&lcdH);
	//jpgbufsize += 64;
	printf("jpgbufsize:%d,lcdw:%d,lcdh:%d, name:%s\r\n", jpgbufsize,lcdW,lcdH, name);
	void *jpgbuf = osal_malloc(jpgbufsize+64);
	if(jpgbuf==NULL)
	{
		printf("malloc jpgbuf:%d error\r\n", jpgbufsize);
		return;
	}
	count=osal_fread(jpgbuf, 1, jpgbufsize, fp);
	printf ("==osal_fread count:%d\n", count);
	dcache_writeback_region((u32)jpgbuf, jpgbufsize);
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		osal_free(jpgbuf);
		printf("malloc lcdframe error\r\n");
		return;
	}
	lcdframeVideoCfg(lcdFrame,0,0,lcdW,lcdH);
	not_isr_jpgdec2(lcdFrame->y_addr, jpgbuf, jpgbufsize, lcdW, lcdH);
	osal_free(jpgbuf);
	//printf("lcdFrame->scan_mode=%d\r\n",lcdFrame->scan_mode);
	cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	lcdDealVideBuff(lcdFrame);
	//osal_task_sleep(60);
	//lcdc_osd_enable(0, 0);
	//lcdc_osd_enable(1, 0);
	//lcd_bk_ctrl(1);
	printf("logo[%d] show end\n",fp);
#endif
}



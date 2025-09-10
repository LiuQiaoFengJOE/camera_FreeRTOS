#include "stream/stream.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "osal/osal_wait.h"

#include "drv/lcd/lcdstruct.h"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
#include "debug.h"
#include "sysCommon/commonInclude.h"
#include "eloop/irqTimer.h"
#include "drv/jpeg/jpeg.h"
#define  CSI_SCALE_W   640
#define  CSI_SCALE_H   360
#define  USENSOR_DEC_W   240
#define  USENSOR_DEC_H   180
#define  START_FRAME_CNT   50
#define  MAX_SEND_INTERVAL    300
#define  UVC_TEMP_W   32
#define  UVC_TEMP_H   16

u8 uvcTempY[UVC_TEMP_W*UVC_TEMP_H];
u8 uvcTempUV[UVC_TEMP_W*UVC_TEMP_H/2];
u32 csiJpgInterval;
static u32 csiJpgInterval1=10;
void csiUsb2JpgDecPicTimer(void *d);
void csiUsb1JpgDecDone(void *d);
void csiUsb2JpgDecDone(void *d);
void csiUsb1JpgDecPicTimer(void *d);
void csiUsb1JpgDecPic(void *d);
extern int jpgDecFrameEx(struct frame *f,uint8_t *yuvBUf,uint8_t *uvBUf,uint32_t w,uint32_t h,uint32_t stride,imageDecDone done);

static int usbJpeDecEn=0;
static u32 inputNoSW=0;
static u32 curInputNo=0;
u32 packerGetCurInput(void)
{
	return curInputNo&0xf;
}
u32 pictureMix(void)
{
	if ((curInputNo == 3) ||(curInputNo == 4))
		return 1;
	return curInputNo?0:1;
}
void packerInputChange(void)
{
	if(getUsbCameraConnect()||getUsb2CameraConnect())
	{
		if(curInputNo==0)
			inputNoSW=0x10;
		else if(curInputNo==0x10&&getUsbCameraConnect())
			inputNoSW=0x1;
		else if(curInputNo==0x10&&getUsb2CameraConnect())
			inputNoSW=0x2;
		else if(curInputNo==0x1&&getUsb2CameraConnect())
			inputNoSW=0x2;
		else if(curInputNo==0x1&&getUsbCameraConnect())
			inputNoSW=0x3;
		else if(curInputNo==0x2&&getUsbCameraConnect())
			inputNoSW=0x3;
		else if(curInputNo==0x3&&getUsb2CameraConnect())
			inputNoSW=0x4;	
		else if(curInputNo==0x2&&getUsb2CameraConnect())
			inputNoSW=0x4;		
		else
			inputNoSW=0;
	}
	printf("packer input:%d\n",curInputNo);
}

void packerInputReset(void)
{
	inputNoSW=0;
}
void setCsiJpgInterval(u32 interval)
{
	if(interval<10||interval>100)
		return;
	csiJpgInterval1=interval;
}
struct csiScaleAndUvc
{
	struct stream_src_self self; // with a chunklist heap
	IRQ_TIMER *csiScaleAndUvcTimer;
	struct frame *curFrame;
	struct frame *curUvcFrame;
	u32 lastSendTime;
	u32 lastTime;
	u32 srcW,srcH;
  	u32 scaleW,scaleH;
	u32 startFrame;
	int stop;
};
void csiUsbNullTimer(void *d)
{
}
static void csiScaleAndUvcStart(void *arg)
{
	struct csiScaleAndUvc *pCsiUvc = (struct csiScaleAndUvc *)arg;
	u32 resw,resh;
	int ret;
	struct frame *f;
	
	if(usbJpeDecEn==0)
		return;
	if((pCsiUvc->curFrame!=NULL)&&(os_time_get()-pCsiUvc->lastSendTime<MAX_SEND_INTERVAL))
	{
		irqTimerModTime(pCsiUvc->csiScaleAndUvcTimer, 20);
		return;
	}
	freeFrame(pCsiUvc->curFrame);
	pCsiUvc->curFrame=streamNewFrame(pCsiUvc->self.output);
	if(pCsiUvc->curFrame==NULL)
	{
		irqTimerModTime(pCsiUvc->csiScaleAndUvcTimer, 20);
		return;
	}
	/*u32 resw,resh;
	resw=resh=0;
	resw=CSI_SCALE_W;
	resh=CSI_SCALE_H;
	if(pCsiUvc->scaleW!=resw||pCsiUvc->scaleH!=resh)
	{
		pCsiUvc->scaleW=resw;
		pCsiUvc->scaleH=resh;
	}*/
RETRY:	
	if (((curInputNo==3)||(curInputNo==4)) && (inputNoSW != 3) && (inputNoSW != 4))
	{
		//printf("scale0[%d,%d]->[%d,%d]\n",pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->scaleW,pCsiUvc->scaleH);
		ax32xx_csiLCDScaler1(pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->scaleW,pCsiUvc->scaleH,0,0,pCsiUvc->srcW,pCsiUvc->srcH,MALIG(pCsiUvc->scaleW,32));
	}
	else if ((((inputNoSW==3)&&(getUsbCameraConnect()))|| ((inputNoSW==4) && (getUsb2CameraConnect())))&& (curInputNo != 3) && (curInputNo != 4))
	{
		ax32xx_csiLCDScaler1(pCsiUvc->srcW,pCsiUvc->srcH,USENSOR_DEC_W,USENSOR_DEC_H,0,0,pCsiUvc->srcW,pCsiUvc->srcH,MALIG(pCsiUvc->scaleW,32));
	}
	curInputNo = inputNoSW;
	//packerSetCurInput(showMode);
	if (curInputNo == 3)
	{
		if(!getUsbCameraConnect())
		{
			inputNoSW=0;
			goto RETRY;
		}
		f=streamDestGetFrame(stream_self_get_input(&pCsiUvc->self, 0));
		if(f)
		{
			freeFrame(pCsiUvc->curUvcFrame);
			pCsiUvc->curUvcFrame=f;
			f->private=pCsiUvc;
			if(getUsb2CameraConnect())
			{
				ret=jpgDecFrameEx(f,pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH,pCsiUvc->scaleW,pCsiUvc->scaleH,MALIG(pCsiUvc->scaleW,32),csiUsb2JpgDecPicTimer);
				if(ret>=0)
					return 0;
			}
			else
			{
				ret=jpgDecFrameEx(f,pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH,pCsiUvc->scaleW,pCsiUvc->scaleH,MALIG(pCsiUvc->scaleW,32),csiUsb1JpgDecDone);
				if(ret>=0)
					return 0;
			}
			freeFrame(pCsiUvc->curUvcFrame);
			pCsiUvc->curUvcFrame=NULL;
		}
	}
	else if (curInputNo == 4)
	{
		if(!getUsb2CameraConnect())
		{
			inputNoSW=0;
			goto RETRY;
		}

		f=streamDestGetFrame(stream_self_get_input(&pCsiUvc->self, 1));
		if(f)
		{
			freeFrame(pCsiUvc->curUvcFrame);
			pCsiUvc->curUvcFrame=f;
			f->private=pCsiUvc;
			if(getUsbCameraConnect())
				ret=jpgDecFrameEx(f,pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH,pCsiUvc->scaleW,pCsiUvc->scaleH,MALIG(pCsiUvc->scaleW,32),csiUsb1JpgDecPicTimer);
			else
				ret=jpgDecFrameEx(f,pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH,pCsiUvc->scaleW,pCsiUvc->scaleH,MALIG(pCsiUvc->scaleW,32),csiUsb2JpgDecDone);
			if(ret>=0)
				return 0;
			freeFrame(pCsiUvc->curUvcFrame);
			pCsiUvc->curUvcFrame=NULL;
		}
	}
	else
	{
		ax32xx_csiLCDDmaKick();
		irqTimerModTime(pCsiUvc->csiScaleAndUvcTimer, getFrameInterval());
	}
}
static void csiScaleAndUvcSendFrame(void *arg)
{
	struct csiScaleAndUvc *src=(struct csiScaleAndUvc *)arg;
	irqTimerModFunc(src->csiScaleAndUvcTimer,csiJpgInterval1,csiScaleAndUvcStart,src);
	if(src->curFrame==NULL)
	{
		return;
	}
	src->curFrame->imageInfor.width=src->scaleW;
	src->curFrame->imageInfor.hight=src->scaleH;
	stream_dest_inturn(&src->self.output->s, src->curFrame);
	src->curFrame=NULL;
	csiJpgInterval=os_time_get()-src->lastSendTime;
	src->lastSendTime=os_time_get();
}
void csiUsb2JpgDecDone(void *d)
{
	struct frame *f=(struct frame *)d;
	struct csiScaleAndUvc *src=f->private;
	if(jpgDecSta()<0)
	{
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
		if(os_time_get()-src->lastSendTime<MAX_SEND_INTERVAL)
		{
			freeFrame(src->curFrame);
			src->curFrame=NULL;
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
		}
		else
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
		}
		return;
	}
	usb2CheckFrameSend(src->curUvcFrame);
	src->curUvcFrame=NULL;
	if ((curInputNo == 3) || (curInputNo == 4))
	{
		ax32xx_csiLCDDmaKick();
	}
	else
		irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
}
void csiUsb1JpgDecDone(void *d)
{
	struct frame *f=(struct frame *)d;
	struct csiScaleAndUvc *src=f->private;
	if(jpgDecSta()<0)
	{
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
		if(os_time_get()-src->lastSendTime<MAX_SEND_INTERVAL)
		{
			freeFrame(src->curFrame);
			src->curFrame=NULL;
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
		}
		else
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
		}
		return;
	}
	usb1CheckFrameSend(src->curUvcFrame);
	src->curUvcFrame=NULL;
	if ((curInputNo == 3) || (curInputNo == 4))
	{
		ax32xx_csiLCDDmaKick();
		//irqTimerModTime(pCsiUvc->csiScaleAndUvcTimer, getFrameInterval());
	}
	else
	{
		irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
	}
}

void csiUsb2JpgDecPic(void *d)
{
	struct csiScaleAndUvc *src=d;
	freeFrame(src->curUvcFrame);
	src->curUvcFrame=streamDestGetFrame(stream_self_get_input(&src->self, 1));
	if(src->curUvcFrame)
	{
		src->curUvcFrame->private=src;
		int ret;
		if(pictureMix())
			ret=jpgDecFrameEx(src->curUvcFrame,src->curFrame->buf.dat+USENSOR_DEC_W,src->curFrame->buf.dat+MALIG(src->scaleW,32)*src->scaleH+USENSOR_DEC_W,USENSOR_DEC_W,USENSOR_DEC_H,MALIG(src->scaleW,32),csiUsb2JpgDecDone);
		else
			ret=jpgDecFrameEx(src->curUvcFrame,uvcTempY,uvcTempUV,UVC_TEMP_W,UVC_TEMP_H,UVC_TEMP_W,csiUsb2JpgDecDone);
		if(ret>=0)
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,100000,csiUsbNullTimer,src);
			return 0;
		}
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
	}
	if(src->startFrame>=START_FRAME_CNT&&os_time_get()-src->lastSendTime<(MAX_SEND_INTERVAL/2))
	{
		freeFrame(src->curFrame);
		src->curFrame=NULL;
		irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
	}
	else
	{
		src->curFrame->imageInfor.width=src->scaleW;
		src->curFrame->imageInfor.hight=src->scaleH;
		stream_dest_inturn(&src->self.output->s, src->curFrame);
		src->curFrame=NULL;
		csiJpgInterval=os_time_get()-src->lastSendTime;
		src->lastSendTime=os_time_get();
		irqTimerModFunc(src->csiScaleAndUvcTimer,20,csiScaleAndUvcStart,src);
	}
}
void csiUsb2JpgDecPicTimer(void *d)
{
	struct frame *f=(struct frame *)d;
	struct csiScaleAndUvc *src=f->private;
	if(jpgDecSta()<0)
	{
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
		if(os_time_get()-src->lastSendTime<MAX_SEND_INTERVAL)
		{
			freeFrame(src->curFrame);
			src->curFrame=NULL;
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
		}
		else
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
		}
		return;
	}
	usb1CheckFrameSend(src->curUvcFrame);
	src->curUvcFrame=NULL;
	irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiUsb2JpgDecPic,src);
}

void csiUsb1JpgDecPic(void *d)
{
	struct csiScaleAndUvc *src=d;
	freeFrame(src->curUvcFrame);
	src->curUvcFrame=streamDestGetFrame(stream_self_get_input(&src->self, 0));
	if(src->curUvcFrame)
	{
		src->curUvcFrame->private=src;
		int ret;
		if(pictureMix())
			ret=jpgDecFrameEx(src->curUvcFrame,src->curFrame->buf.dat+USENSOR_DEC_W,src->curFrame->buf.dat+MALIG(src->scaleW,32)*src->scaleH+USENSOR_DEC_W,USENSOR_DEC_W,USENSOR_DEC_H,MALIG(src->scaleW,32),csiUsb1JpgDecDone);
		else
			ret=jpgDecFrameEx(src->curUvcFrame,uvcTempY,uvcTempUV,UVC_TEMP_W,UVC_TEMP_H,UVC_TEMP_W,csiUsb1JpgDecDone);
		if(ret>=0)
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,100000,csiUsbNullTimer,src);
			return 0;
		}
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
	}
	if(src->startFrame>=START_FRAME_CNT&&os_time_get()-src->lastSendTime<(MAX_SEND_INTERVAL/2))
	{
		freeFrame(src->curFrame);
		src->curFrame=NULL;
		irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
	}
	else
	{
		src->curFrame->imageInfor.width=src->scaleW;
		src->curFrame->imageInfor.hight=src->scaleH;
		stream_dest_inturn(&src->self.output->s, src->curFrame);
		src->curFrame=NULL;
		csiJpgInterval=os_time_get()-src->lastSendTime;
		src->lastSendTime=os_time_get();
		irqTimerModFunc(src->csiScaleAndUvcTimer,20,csiScaleAndUvcStart,src);
	}
}
void csiUsb1JpgDecPicTimer(void *d)
{
	struct frame *f=(struct frame *)d;
	struct csiScaleAndUvc *src=f->private;
	if(jpgDecSta()<0)
	{
		freeFrame(src->curUvcFrame);
		src->curUvcFrame=NULL;
		if(os_time_get()-src->lastSendTime<MAX_SEND_INTERVAL)
		{
			freeFrame(src->curFrame);
			src->curFrame=NULL;
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcStart,src);
		}
		else
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
		}
		return;
	}
	usb2CheckFrameSend(src->curUvcFrame);
	src->curUvcFrame=NULL;
	irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiUsb1JpgDecPic,src);
}

static int csiScaleDown(unsigned int irqno, void *d, void *regs)
{
#if 1
	struct frame *f;
	int ret;
	struct csiScaleAndUvc *src = (struct csiScaleAndUvc *)d;
	if(src->curFrame==NULL)
	{
		printf("%s err happen\n",__FUNCTION__);
		irqTimerModFunc(src->csiScaleAndUvcTimer,getFrameInterval(),csiScaleAndUvcStart,src);
		return 0;
	}
	if(src->startFrame<START_FRAME_CNT)
		src->startFrame++;
	//csi_jdma_stop();
	#if 1
	if(src->startFrame<4)
	{
		streamDestFreeFrames(stream_self_get_input(&src->self, 0),0);
		streamDestFreeFrames(stream_self_get_input(&src->self, 1),0);
	}
	if(src->startFrame>=5&&(getUsbCameraConnect()||getUsb2CameraConnect()))
	{
		if((packerGetCurInput()==0x3) || (packerGetCurInput()==0x4))
		{
			irqTimerModFunc(src->csiScaleAndUvcTimer,1,csiScaleAndUvcSendFrame,src);
			return;
		}
		else
		{
			f=streamDestGetFrame(stream_self_get_input(&src->self, 0));
			if(f)
			{
				freeFrame(src->curUvcFrame);
				src->curUvcFrame=f;
				f->private=src;
				if(getUsb2CameraConnect())
				{
					if(pictureMix())
						ret=jpgDecFrameEx(f,src->curFrame->buf.dat,src->curFrame->buf.dat+MALIG(src->scaleW,32)*src->scaleH,USENSOR_DEC_W,USENSOR_DEC_H,MALIG(src->scaleW,32),csiUsb2JpgDecPicTimer);
					else
						ret=jpgDecFrameEx(src->curUvcFrame,uvcTempY,uvcTempUV,UVC_TEMP_W,UVC_TEMP_H,UVC_TEMP_W,csiUsb2JpgDecPicTimer);
					if(ret>=0)
						return 0;
				}
				else
				{
					if(pictureMix())
						ret=jpgDecFrameEx(f,src->curFrame->buf.dat,src->curFrame->buf.dat+MALIG(src->scaleW,32)*src->scaleH,USENSOR_DEC_W,USENSOR_DEC_H,MALIG(src->scaleW,32),csiUsb1JpgDecDone);
					else
						ret=jpgDecFrameEx(src->curUvcFrame,uvcTempY,uvcTempUV,UVC_TEMP_W,UVC_TEMP_H,UVC_TEMP_W,csiUsb1JpgDecDone);
					if(ret>=0)
						return 0;
				}
				freeFrame(src->curUvcFrame);
				src->curUvcFrame=NULL;
			}
			else if(getUsbCameraConnect()==0&&getUsb2CameraConnect())
			{
				f=streamDestGetFrame(stream_self_get_input(&src->self, 1));
				if(f)
				{
					freeFrame(src->curUvcFrame);
					src->curUvcFrame=f;
					f->private=src;
					if(pictureMix())
						ret=jpgDecFrameEx(f,src->curFrame->buf.dat,src->curFrame->buf.dat+MALIG(src->scaleW,32)*src->scaleH,USENSOR_DEC_W,USENSOR_DEC_H,MALIG(src->scaleW,32),csiUsb2JpgDecDone);
					else
						ret=jpgDecFrameEx(src->curUvcFrame,uvcTempY,uvcTempUV,UVC_TEMP_W,UVC_TEMP_H,UVC_TEMP_W,csiUsb2JpgDecDone);
					if(ret>=0)
						return 0;
					freeFrame(src->curUvcFrame);
					src->curUvcFrame=NULL;
				}
			}
		}
		//printf("% usb decode err:0x%x\n\n\n\n\n\n",__FUNCTION__,f);
		//csi_jdma_start();
		if(src->startFrame>=START_FRAME_CNT&&os_time_get()-src->lastSendTime<MAX_SEND_INTERVAL)
		{
			ax32xx_csiLCDDmaKick();
			return 0;
		}
	}
	#endif
	src->curFrame->imageInfor.width=src->scaleW;
	src->curFrame->imageInfor.hight=src->scaleH;
	stream_dest_inturn(&src->self.output->s, src->curFrame);
	src->curFrame=NULL;
	irqTimerModFunc(src->csiScaleAndUvcTimer,10,csiScaleAndUvcStart,src);
	csiJpgInterval=os_time_get()-src->lastSendTime;
	src->lastSendTime=os_time_get();
#endif
	return 1;
}

static void csiScaleAndUvcSetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct csiScaleAndUvc *pCsiUvc = s->self;
  printf("%s %s run:%d\r\n", __FUNCTION__, pCsiUvc->self.output->s.name, status);
  if(status==S_INPUT_RUNNING)
  {
  	u32 resw,resh;
	resw=resh=0;
	sensorGetRes(&resw,&resh);
	pCsiUvc->srcW=resw;
	pCsiUvc->srcH=resh;
	resw=resh=0;
	//getWifiVidRes(&resw,&resh);
	resw=CSI_SCALE_W;
	resh=CSI_SCALE_H;
	pCsiUvc->scaleW=resw;
	pCsiUvc->scaleH=resh;
	freeFrame(pCsiUvc->curFrame);
	pCsiUvc->curFrame=streamNewFrame(pCsiUvc->self.output);
	if(pCsiUvc->curFrame==NULL)
	{
		printf("% err,no frame\n",__FUNCTION__);
		return;
	}
	pCsiUvc->stop=0;
	pCsiUvc->startFrame=0;
	printf("scale0[%d,%d]->[%d,%d]\n",pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->scaleW,pCsiUvc->scaleH);
	//csi_jdma_scale_down(pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->scaleW,pCsiUvc->scaleH);
	ax32xx_csiLCDScaler1(pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->scaleW,pCsiUvc->scaleH,0,0,pCsiUvc->srcW,pCsiUvc->srcH,MALIG(pCsiUvc->scaleW,32));
	dcache_flush_region((u32)pCsiUvc->curFrame->buf.dat,(MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH*3)/2);
	//ax32xx_csiMJPEGFrameSet(pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH,pCsiUvc->scaleH,MALIG(pCsiUvc->scaleW,32));
	//csiirq_register_jdma_frame_end(csiScaleDown,pCsiUvc);
	///csi_jdma_start();
	csiirq_register_lcd_frame_end(csiScaleDown,pCsiUvc);
	ax32xx_csiOutputSet(CSI_OUTPUT_RELOAD, 0);
	ax32xx_csiOutputSet(CSI_OUTPUT_LDMAMANEN,1);
	ax32xx_csiLCDFrameSet((u32)pCsiUvc->curFrame->buf.dat,pCsiUvc->curFrame->buf.dat+MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH);
	ax32xx_csiLCDDmaEnable(1);
	
	csiLcdAnti_lsawtoothInit();
	csi_enable();
	
	pCsiUvc->lastSendTime=pCsiUvc->lastTime=os_time_get();
	if(pCsiUvc->csiScaleAndUvcTimer)
		printf("% err,csiScaleAndUvcTimer not null\n",__FUNCTION__);
	pCsiUvc->csiScaleAndUvcTimer = irqTimerCreate(100000, csiUsbNullTimer, pCsiUvc);
	usbJpeDecEn=1;
  }
  else if(status==S_INPUT_PAUSE)
  {
  	usbJpeDecEn=0;
  	pCsiUvc->stop=1;
	if (pCsiUvc->csiScaleAndUvcTimer)
		irqTimerDestory(pCsiUvc->csiScaleAndUvcTimer);
	pCsiUvc->csiScaleAndUvcTimer = NULL;
	//csi_jdma_stop();
	//csiirq_unregister_jdma_frame_end();
	ax32xx_csiLCDDmaEnable(0);
	jpgDecStop(0);
	freeFrame(pCsiUvc->curFrame);
	pCsiUvc->curFrame=NULL;
	freeFrame(pCsiUvc->curUvcFrame);
	pCsiUvc->curUvcFrame=NULL;
	streamDestFreeFrames(stream_self_get_input(&pCsiUvc->self, 0),0);
	streamDestFreeFrames(stream_self_get_input(&pCsiUvc->self, 1),0);
  }
}
static void csiScaleAndUvcStreamSetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct csiScaleAndUvc *pCsiUvc = s->self;
  if(status==S_INPUT_RUNNING)
  {
  	usbJpeDecEn=1;
  }
  else if(status==S_INPUT_PAUSE)
  {
  	usbJpeDecEn=0;
  }
}

static void csiScaleAndUvcConfig(void *d)
{
	u32 resw,resh;
	resw=resh=0;
	struct csiScaleAndUvc *pCsiUvc = (struct csiScaleAndUvc *)d;
	sensorGetRes(&pCsiUvc->srcW,&pCsiUvc->srcH);
	//getWifiVidRes(&resw,&resh);
	resw=CSI_SCALE_W;
	resh=CSI_SCALE_H;
	pCsiUvc->scaleW=resw;//pCsiUvc->srcW;
	pCsiUvc->scaleH=resh;//pCsiUvc->srcH;
	pCsiUvc->self.output->buf_num = 0;
	pCsiUvc->self.output->lst_size = 0;
	pCsiUvc->self.output->frame_num = 1;
	pCsiUvc->self.output->s.buf_size = (MALIG(pCsiUvc->scaleW,32)*pCsiUvc->scaleH*3)/2;
	printf("senW:%d,senH:%d ,size:%dKB\n", pCsiUvc->srcW,pCsiUvc->srcH,pCsiUvc->self.output->s.buf_size>>10);
}

static struct stream_src_ops csiScaleAndUvc_src_ops = {
    .bufsize = (frame_size_func)frame_bufsize,
	.set_status = csiScaleAndUvcStreamSetStatus,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};
static struct stream_self_ops csiScaleAndUvc_self_ops = {
    .getinfo = NULL,
    .set_status = csiScaleAndUvcSetStatus,
    .config = csiScaleAndUvcConfig,
    .collect = stream_self_collect_default,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};
static void uvcJpgSave(struct frame *input, void *d)
{
	struct stream_dest *dest = (struct stream_dest *)d;
	struct csiScaleAndUvc *pCsiUvc = (struct csiScaleAndUvc*)dest->self;
	if(pCsiUvc->stop)
	{
		freeFrame(input);
		return;
	}
	if(queueFull(&dest->mq))
	{
		freeFrame(input);
		return;
	}
	streamDestAddFrame(dest, input);
}
static void *start_block(void)
{
	struct csiScaleAndUvc *src = (struct csiScaleAndUvc *)osal_malloc(sizeof(struct csiScaleAndUvc));
	if (src == NULL)
	{
		p_err_mem;
		return NULL;
	}
	src->stop=1;
	src->startFrame=0;
	stream_src_self_init(&src->self, &csiScaleAndUvc_self_ops, NULL);
	return src;
}

static int end_block(void *d)
{
	struct csiScaleAndUvc *src = (struct csiScaleAndUvc *)d;
	struct stream_dest *input = stream_dest_new(uvcJpgSave, d,3);
  	stream_add_input(&src->self, input);
	input = stream_dest_new(uvcJpgSave, d,3);
  	stream_add_input(&src->self, input);
	src->curFrame = NULL;
	src->curUvcFrame=NULL;
	src->csiScaleAndUvcTimer=NULL;
	//csi_setup(); // todo
	return 0;
}

static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct csiScaleAndUvc *src = (struct csiScaleAndUvc *)d;
	struct stream_src *output;
	if (src->self.output)
	{
		p_err("vid-csi config twice");
		return -1;
	}
	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;
	//output->s.takeOver=1;
	output->s.get_framerate = NULL;
	output->ops = &csiScaleAndUvc_src_ops;
	src->self.output = output;
	return 0;
}

static int set_framenum(int num_tokens, struct token *tokens, void *d)
{
	struct csiScaleAndUvc *src = (struct csiScaleAndUvc *)d;
	if (!src->self.output)
	{
		p_err("vid-csi output should be config before framenum");
		return -1;
	}
	src->self.output->frame_num = tokens[1].v.num;
	return 0;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
	/* directive name, process function, min args, max args, arg types */
	{"output", set_output, 1, 1, {TOKEN_STR}},
	{"framenum", set_framenum, 1, 1, {TOKEN_NUM}},

	/* empty terminator -- do not remove */
	{NULL, NULL, 0, 0, {}}};

int vid_csiuvc_register(void)
{
	register_config_context("yuv", "csiuvc", start_block, end_block, config_statements);
	return 0;
}


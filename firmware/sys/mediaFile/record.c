#include "rtsp/rtsp.h"
#include "osal/osal_task.h"
#include "mediaFile/muxer.h"
#include "task/app.h"
#include "ax32xx_auadc.h"
#include "fatfs/fileOps.h"
#include "eloop/irqTimer.h"
#include "drv/jpeg/jpeg.h"
#include "systemSetting/systemSetting.h"
#include "eloop/eloop.h"
#include "drv/rtc/rtc.h"
#include "mediaFile/picIdentify.h"

#if SUPPORT_AUD_REC
#include "mediaFile/wav/wav.h"
#endif

#define SUBTITLES_BUF_LEN    (32*1024)
extern void setLocalRes(u32 w,u32 h);
extern u32 getLocalResW(void);
extern u32 getLocalResH(void);
extern int w_test_open(INT32U fd, UINT len);
extern void mediaMuxerSlowRecSet(u8 slow);
extern u8 mediaMuxerSlowRecGet(void);

typedef struct _mediaState
{
	int recording;
	uint32  recordingTime;
	uint32  audioFileSize;
	u32 timeLapseCnt;
	u32 photoBurstCnt;
	void *timeLapseEvent;
	void *photoBurstEvent;
	int takePhotoWorking;
	int clientFd;
	int curVid;
	int curVid1;
	int curVid2;
	int freeFrame;
	int reStart;
	char *lastPicName;
	u8   videorecRedLevel;
}mediaState;
mediaState mediaSta;
#define   FILE_FPS     30
#define   TAKE_PIC_INTERVAL    1



int usbCameraRecord(void)
{
	return getUsbCameraConnect();
}
int usb2CameraRecord(void)
{
	return getUsb2CameraConnect();
}
int curVidIsLock(void)
{
	if(mediaSta.recording==0)
		return 0;
	return fileIsLock(mediaSta.curVid);
}
int videoIsRecording(void)
{
	return mediaSta.recording;
}
int takePictrueIsWorking(void)
{
	return mediaSta.takePhotoWorking;
}
u32 getAudioRecTime(void)
{
	return 2*60;//60*60;//one hour
}
uint32 videoRecordGetTime(void)
{
	return mediaSta.recordingTime;
}
int8 videoRecordGetRedLevel( void )
{
	return mediaSta.videorecRedLevel;
}
int8 videoRecordSetRedLevel(s8 value)
{
	if (value>=0)
	{
		mediaSta.videorecRedLevel = value;
		mediaMuxerSlowRecSet(0);
	}
	else
	{
		mediaSta.videorecRedLevel = 0;
		mediaMuxerSlowRecSet(-value);
	}
}
u32 vidRecordRemainTime(void)
{
	u16 w=0,h=0;
	getVidRes(&w,&h);
	return (getSdFreeSizeMB()<<10)/(getFrameSizeByRes(w,h)>>10)/FILE_FPS;
}
u32 takePhotoRemainSum(void)
{
	u16 w=0,h=0;
	int fMgSum=getMaxFileSum()-getFileSum();
	getPhotoRes(&w,&h);
	int remainSum=(getSdFreeSizeMB()<<10)/(getFrameSizeByRes(w,h)>>10);
	if(fMgSum>remainSum)
		return remainSum;
	return fMgSum;
}

int getTakePhotoTime(void)
{
	return getTimeLapse()+getPhotoBurst()*TAKE_PIC_INTERVAL;
}
u32 getVidNeedSize(void)
{
	u16 w=0,h=0;
	u32 size;
	if(getVidRecLoopEn()==0)
		return 200;
	getVidRes(&w,&h);
	size=getFrameSizeByRes(w,h)*FILE_FPS*getVidRecTime()>>20;
	if(usbCameraRecord())
		size+=30*FILE_FPS*getVidRecTime()>>10;
	if(usb2CameraRecord())
		size+=30*FILE_FPS*getVidRecTime()>>10;
	return size;
}

void notiyClentCountdown(u32 cnt)
{
#if WIFI_EN
#if JR_APP_PROTOCOL
	int ret;
	if(mediaSta.clientFd!=0xffffffff)
	{
		struct tcpMessage *msg = take_photo_countdown(cnt);
		if (msg)
		{
			msg->fd=mediaSta.clientFd;
			ret = xQueueSend(get_sender_queue(), msg, 0);
			if (!ret)
				osal_free(msg->sendbuf);
			osal_free(msg);
		}
	}
#endif
#endif
}
void takePhotoBurstCnt(void *e,void *d)
{
	eloop_remove_event(mediaSta.photoBurstEvent);
	mediaSta.photoBurstEvent=NULL;
	sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_TAKE_PHOTO_START,0);
}
void takePhotoTimeLapseCnt(void *e,void *d)
{
	if(mediaSta.timeLapseCnt)
	{
		mediaSta.timeLapseCnt--;
		notiyClentCountdown(mediaSta.timeLapseCnt);
	}
	//printf("timeLapse:%d\n",mediaSta.timeLapseCnt);
	if(mediaSta.timeLapseCnt==0)
	{
		eloop_remove_event(mediaSta.timeLapseEvent);
		mediaSta.timeLapseEvent=NULL;
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_TAKE_PHOTO_START,0);
	}
}
int clientTaketPhotoStart(int fd)
{
	uint32_t flags;
	flags = arch_local_irq_save();
	if(mediaSta.takePhotoWorking)
	{
		arch_local_irq_restore(flags);
		printf("takePhoto Working\n");
		return -1;
	}
	mediaSta.takePhotoWorking=1;
	arch_local_irq_restore(flags);
	if(mediaSta.timeLapseEvent)
	{
		while(1)
		{
			printf("timeLapseEvent err\n");
			osal_task_sleep(1000);
		}
	}
	mediaSta.clientFd=fd;
	mediaSta.timeLapseCnt=getTimeLapse();
	mediaSta.photoBurstCnt=getPhotoBurst();
	printf("timeLapse:%d,photoBurst:%d\n",mediaSta.timeLapseCnt,mediaSta.photoBurstCnt);
	if(mediaSta.timeLapseCnt)
	{
		notiyClentCountdown(mediaSta.timeLapseCnt);
		mediaSta.timeLapseEvent=eloop_add_timer(1000, 0, takePhotoTimeLapseCnt, NULL);
		if(mediaSta.timeLapseEvent)
			return 0;
		mediaSta.takePhotoWorking=0;
		return -1;
	}
	sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_TAKE_PHOTO_START,0);
	return 0;
}
int localTaketPhotoStart(void)
{
	return clientTaketPhotoStart(0xffffffff);
}

////////////////////////////////////////////////////////////////////////////////
struct sssRecord { // 繼承自stream_self
	struct stream_src_self self;  // with a chunklist heap
	IRQ_TIMER  *recordTimer;
	mediaMuxer* muxer;
	mediaStream* videoStream;
	mediaStream* audioStream;
	mediaMuxer* muxer1;
	mediaStream* videoStream1;
	mediaStream* audioStream1;
	mediaMuxer* muxer2;
	mediaStream* videoStream2;
	mediaStream* audioStream2;
	void *audFD;
	void *photoFD;
	void *photoFD1;
	void *photoFD2;
#if SUBTITLES_SUPPORT		
	void *usb1Fd;
	void *usb2Fd;
	HAL_RTC_T lastTime;
	uint32 lastIdx;
	u32 wmLen;
	u8 wmBuff[SUBTITLES_BUF_LEN+100];
#endif	
	uint32 recordingTime;
	uint32 recording1Time;
	uint32 recording2Time;
	uint32 showTime;
	u8  getThumbnail;
};

int writeWMark(struct sssRecord* src)
{
#if SUBTITLES_SUPPORT
	int i, count,len;
	char *timeStr;
	HAL_RTC_T wmTime;
	HAL_RTC_T axisStart, axisEnd;
	HAL_RTC_T currT = get_rtc_time();
	
	if (((NULL == src->usb1Fd)&&(NULL == src->usb2Fd))/*(|| (src->lastTime.totalsec>= currT.totalsec)*/)
	{
		return 0;
	}
	if (mediaSta.videorecRedLevel != 0)
	{
		if (src->lastTime.totalsec>= src->recordingTime/1000)
		{
			//printf("++%d,%d++", src->lastTime.totalsec, src->recordingTime);
			return 0;
		}

		timeStr = hal_rtcTime2String(&currT);

		hal_rtcTime(src->lastIdx, &axisStart);
		hal_rtcTime((src->lastIdx+1), &axisEnd);
		
		len=sprintf(&src->wmBuff[src->wmLen], "%d\n%02d:%02d:%02d --> %02d:%02d:%02d\n%s\n\n", src->lastIdx+1, \
			axisStart.hour, axisStart.min, axisStart.sec, \
			axisEnd.hour, axisEnd.min, axisEnd.sec, timeStr);
		src->wmLen+=len;
		if(src->wmLen>=SUBTITLES_BUF_LEN)
		{
			if(src->usb1Fd)
				osal_fwrite(src->wmBuff,1,src->wmLen,src->usb1Fd);
			if(src->usb2Fd)
				osal_fwrite(src->wmBuff,1,src->wmLen,src->usb2Fd);
			src->wmLen=0;
		}
		src->lastIdx++;
		src->lastTime.totalsec = src->recordingTime/1000;//currT;
	}
	else
	{
		if (src->lastTime.totalsec>= currT.totalsec)
			return 0;
		
		count = (currT.totalsec-src->lastTime.totalsec);
		if(count>3)
			count=1;
		for (i=1; i<=count; i++)
		{
			hal_rtcTime(src->lastTime.totalsec+i, &wmTime);
			wmTime.totalsec = src->lastTime.totalsec+i;
			timeStr = hal_rtcTime2String(&wmTime);

			hal_rtcTime(src->lastIdx, &axisStart);
			hal_rtcTime((src->lastIdx+1), &axisEnd);
			
			len=sprintf(&src->wmBuff[src->wmLen], "%d\n%02d:%02d:%02d --> %02d:%02d:%02d\n%s\n\n", src->lastIdx+1, \
				axisStart.hour, axisStart.min, axisStart.sec, \
				axisEnd.hour, axisEnd.min, axisEnd.sec, timeStr);
			src->wmLen+=len;
			if(src->wmLen>=SUBTITLES_BUF_LEN)
			{
				if(src->usb1Fd)
					osal_fwrite(src->wmBuff,1,src->wmLen,src->usb1Fd);
				if(src->usb2Fd)
					osal_fwrite(src->wmBuff,1,src->wmLen,src->usb2Fd);
				src->wmLen=0;
			}
			src->lastIdx++;
			src->lastTime = wmTime;
		}
	}
	return count;
#endif
}


//USER_WAV_T audsrc;
int writFrame(void* fp,struct frame* f)
{
	uint32 count=0;
	struct list_head* pos;
	struct frame_buf* buf;
	uint32  writeLen=0;
	uint32  remainDataLen=f->dataLen;
	uint32 bufsize = f->stream->ops->bufsize(f);
	u8 head;
	if(fp==NULL)
		return 0;
	if(!list_empty(&(f->buflist)))
	{		
		list_for_each(pos, &f->buflist)
		{
			buf = list_entry(pos, struct frame_buf, list);
			if(remainDataLen>bufsize)
			{
				writeLen=bufsize;
				remainDataLen=remainDataLen-bufsize;
			}
			else
			{
				writeLen=remainDataLen;
				remainDataLen=0;
			}
			if(count==0)
			{
				if(buf->dat[0]==0xff)
					head=0;
				else
					head=8;
				count=osal_fwrite(buf->dat+head,1,writeLen-head,fp);
				count+=head;
			}
			else
				count+=osal_fwrite(buf->dat,1,writeLen,fp);
			if(remainDataLen==0)
			{
				return count;
			}
		}
	}
	else
	{
		count=osal_fwrite(buf->dat+8,1,writeLen-8,fp);
	}
	return count;
}
int writeThumbnailFrame(void* fp,struct frame* f)
{
	uint32 count=0;
	struct list_head* pos;
	struct frame_buf* buf;
	struct frame* thumbFrame;
	uint32  writeLen=0;
	uint32  remainDataLen=f->dataLen;
	u32 picLen;
	u8 head;
	uint32 bufsize = f->stream->ops->bufsize(f);
	u8 thumbnailPos[8] = {0xFF, 0xE1, 0x00, 0x06};
	if(fp==NULL||f==NULL||f->next==NULL)
		return 0;
	thumbFrame=f->next;
	if(!list_empty(&(f->buflist)))
	{		
		list_for_each(pos, &f->buflist)
		{
			buf = list_entry(pos, struct frame_buf, list);
			if(remainDataLen>bufsize)
			{
				writeLen=bufsize;
				remainDataLen=remainDataLen-bufsize;
			}
			else
			{
				writeLen=remainDataLen;
				remainDataLen=0;
			}
			if(count==0)
			{
				if(buf->dat[0]==0xff)
					head=0;
				else
					head=8;
				count=osal_fwrite(buf->dat+head,1,20,fp);
				picLen=f->dataLen-head+8;
				memcpy(thumbnailPos+4, &picLen,4);
				count+=osal_fwrite(thumbnailPos,1,8,fp);
				count+=osal_fwrite(buf->dat+head+20,1,writeLen-head-20,fp);
			}
			else
				count+=osal_fwrite(buf->dat,1,writeLen,fp);
			if(remainDataLen==0)
			{
				//return count;
				break;
			}
		}
	}
	else
	{
		printf("not support this buff\n");
		return 0;
	}
	writFrame(fp,thumbFrame);
	return count;
}

void dealVideoRecordTime(void *arg)
{
#if 1
	struct sssRecord* src=(struct sssRecord*)arg;
	if(videoIsRecording())
	{
		if (mediaSta.videorecRedLevel != 0)
		{
			mediaSta.recordingTime=src->recordingTime/1000;
			sendMsgNonblock(OS_TASK_UI,SYS_TIME_UPDATE, src->recordingTime/1000);
		}
		else
		{
			if (mediaMuxerSlowRecGet() == 0)
			{
				if(src->showTime*1000>src->recordingTime)
					irqTimerModTime(src->recordTimer,1100);
				else if(src->showTime*1000<src->recordingTime)
					irqTimerModTime(src->recordTimer,900);
				else
					irqTimerModTime(src->recordTimer,1000);
			}

			src->showTime++;
			if(src->audFD)
			{
				if(src->showTime>getAudioRecTime())
					src->showTime=0;
			}
			else
			{
				if(src->showTime>getVidRecTime())
					src->showTime=0;
			}
			mediaSta.recordingTime=src->showTime;
			sendMsgNonblock(OS_TASK_UI,SYS_TIME_UPDATE, src->showTime);
		}
	}
#endif
}

void recordUnintMsg(void)
{
	//sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
	sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_UNINIT,0);
}
void vidRecordStop(struct sssRecord* src)
{
	int delSmallFile=0;
	struct stream_dest* streamDestVid0=stream_self_get_input(&src->self, 0);
	struct stream_dest* streamDestAud=stream_self_get_input(&src->self, 1);
	struct stream_dest* streamDestVid1=stream_self_get_input(&src->self, 2);
	struct stream_dest* streamDestVid2=stream_self_get_input(&src->self, 3);
	if(src->recordTimer)
		irqTimerDestory(src->recordTimer);
	src->recordTimer=NULL;
	if(mediaSta.takePhotoWorking==0)
	{
		streamSetPause(streamDestVid0);
		streamDestFreeFrames(streamDestVid0,0);
	}
	streamSetPause(streamDestVid1);
	streamDestFreeFrames(streamDestVid1,0);
	streamSetPause(streamDestVid2);
	streamDestFreeFrames(streamDestVid2,0);
	streamSetPause(streamDestAud);
	streamDestFreeFrames(streamDestAud,0);
	if(src->videoStream&&videoStreamGetTime(src->videoStream)<500)
	{
		delSmallFile=1;
		printf("video time short %d,delelet file %d\n",videoStreamGetTime(src->videoStream),mediaSta.curVid);
	}
	mediaMuxerClosure(src->muxer);
	if(delSmallFile&&mediaSta.curVid>=0)
		delDateFile(AVI_FILE,mediaSta.curVid);
	mediaSta.curVid=-1;
	
	delSmallFile=0;
	if(src->videoStream1&&videoStreamGetTime(src->videoStream1)<500)
	{
		delSmallFile=1;
		printf("video time short %d,delelet file %d\n",videoStreamGetTime(src->videoStream1),mediaSta.curVid1);
	}
	mediaMuxerClosure(src->muxer1);
	if(delSmallFile&&mediaSta.curVid1>=0)
		delDateFile(AVI_FILE,mediaSta.curVid1);
	mediaSta.curVid1=-1;

	delSmallFile=0;
	if(src->videoStream2&&videoStreamGetTime(src->videoStream2)<500)
	{
		delSmallFile=1;
		printf("video time short %d,delelet file %d\n",videoStreamGetTime(src->videoStream2),mediaSta.curVid2);
	}
	mediaMuxerClosure(src->muxer2);
	if(delSmallFile&&mediaSta.curVid2>=0)
		delDateFile(AVI_FILE,mediaSta.curVid2);
	mediaSta.curVid2=-1;
	
	src->muxer=NULL;
	src->videoStream=NULL;
	src->audioStream=NULL;
	src->muxer1=NULL;
	src->videoStream1=NULL;
	src->audioStream1=NULL;
	src->muxer2=NULL;
	src->videoStream2=NULL;
	src->audioStream2=NULL;
	mediaSta.recordingTime=0;
	mediaSta.recording=0;
#if SUBTITLES_SUPPORT	
	if (src->usb1Fd)
	{
		if(src->wmLen)
			osal_fwrite(src->wmBuff,1,src->wmLen,src->usb1Fd);
		sdFreeSizeAdj(osal_fsize(src->usb1Fd),0);
		osal_fclose(src->usb1Fd);
		src->usb1Fd = NULL;
	}
	if (src->usb2Fd)
	{
		if(src->wmLen)
			osal_fwrite(src->wmBuff,1,src->wmLen,src->usb2Fd);
		sdFreeSizeAdj(osal_fsize(src->usb2Fd),0);
		osal_fclose(src->usb2Fd);
		src->usb2Fd = NULL;
	}	
	src->wmLen=0;
#endif	
	sdCheckEnSet(1);
	user_action(MSG_ID_RECORD_VIDEO);
}
#if SUPPORT_AUD_REC
void audRecordStop(struct sssRecord* src)
{
	wavEncodeEnd(src->audFD);
	if(src->audFD)
	{
		sdFreeSizeAdj(osal_fsize(src->audFD),0);
		osal_fclose(src->audFD);
	}
	src->audFD=NULL;
	if(src->recordTimer)
		irqTimerDestory(src->recordTimer);
	src->recordTimer=NULL;
	mediaSta.recording = 0;
	mediaSta.recordingTime=0;
}
#endif
void recordUninit(struct sssRecord* src)
{
	struct stream_dest* streamDestVid0=stream_self_get_input(&src->self, 0);
	vidRecordStop(src);
	streamSetPause(streamDestVid0);
	streamDestFreeFrames(streamDestVid0,0);
	if(src->audFD)
	{
		sdFreeSizeAdj(osal_fsize(src->audFD),0);
		osal_fclose(src->audFD);
	}
	src->audFD=NULL;
	if(src->photoFD)
	{
		sdFreeSizeAdj(osal_fsize(src->photoFD),0);
		osal_fclose(src->photoFD);
	}
	if(src->photoFD1)
	{
		sdFreeSizeAdj(osal_fsize(src->photoFD1),0);
		osal_fclose(src->photoFD1);
	}
	if(src->photoFD2)
	{
		sdFreeSizeAdj(osal_fsize(src->photoFD2),0);
		osal_fclose(src->photoFD2);
	}	
	src->photoFD=NULL;
	src->photoFD1=NULL;
	src->photoFD2=NULL;
	uint32_t flags;
	flags = arch_local_irq_save();
	if(mediaSta.timeLapseEvent)
		eloop_remove_event(mediaSta.timeLapseEvent);
	mediaSta.timeLapseEvent=NULL;
	if(mediaSta.photoBurstEvent)
		eloop_remove_event(mediaSta.photoBurstEvent);
	arch_local_irq_restore(flags);
	mediaSta.photoBurstEvent=NULL;
	mediaSta.clientFd=0xffffffff;
	mediaSta.takePhotoWorking=0;
	mediaSta.audioFileSize = 0;
}
void vidRecordStart(struct sssRecord* src)
{
	u16 resW,resH;
	int nameLen;
	getVidRes(&resW,&resH);
	mediaSta.curVid=-1;
	mediaSta.curVid1=-1;
	mediaSta.curVid2=-1;
	mediaSta.reStart=0;
	src->muxer=mediaMuxerAllocate(createVidName(&mediaSta.curVid));
	src->videoStream=mediaMuxerAllocateStream(src->muxer, MEDIA_VIDEO);
	videoStreamInit(src->videoStream, resW, resH, FILE_FPS);
	mediaStreamSetCodecName(src->videoStream, "JPEG");
	src->audioStream=mediaMuxerAllocateStream(src->muxer, MEDIA_AUDIO);
	audioStreamInit(src->audioStream,ADC_SAMPLEREATE_16000,1);
	mediaStreamSetCodecName(src->audioStream, "L16");
	mediaMuxerWriteHeader(src->muxer);
	if(src->muxer)
		mediaSta.recording=1;
	else
		mediaSta.recording=0;
	src->recordingTime = 0;
#if SUBTITLES_SUPPORT
	src->lastTime = get_rtc_time();
	if (mediaSta.videorecRedLevel != 0)
		src->lastTime.totalsec = 0;
	src->lastIdx = 0;
	src->wmLen=0;
#endif
	if(usbCameraRecord())
	{
		char *pName;
		resW=0;
		resH=0;
		usbCameraRes(&resW,&resH);
#if SUBTITLES_SUPPORT
		pName = createVidName(&mediaSta.curVid1);
		src->muxer1=mediaMuxerAllocate(pName);
		if(src->muxer1&&getVidWaterMarkEn())
		{
			nameLen=strlen(pName);
			pName[nameLen-3]='s';
			pName[nameLen-2]='r';
			pName[nameLen-1]='t';
			src->usb1Fd=osal_fopen(pName, "w");
		}
#else
		src->muxer1=mediaMuxerAllocate(createVidName(&mediaSta.curVid1));
#endif
		src->videoStream1=mediaMuxerAllocateStream(src->muxer1, MEDIA_VIDEO);
		videoStreamInit(src->videoStream1, resW, resH, FILE_FPS);
		mediaStreamSetCodecName(src->videoStream1, "JPEG");
		src->audioStream1=mediaMuxerAllocateStream(src->muxer1, MEDIA_AUDIO);
		audioStreamInit(src->audioStream1,ADC_SAMPLEREATE_16000,1);
		mediaStreamSetCodecName(src->audioStream1, "L16");
		mediaMuxerWriteHeader(src->muxer1);
	}
	if(usb2CameraRecord())
	{
		char *pName;
		resW=0;
		resH=0;
		usbCameraRes(&resW,&resH);
#if SUBTITLES_SUPPORT
		pName = createVidName(&mediaSta.curVid2);
		src->muxer2=mediaMuxerAllocate(pName);
		if(src->muxer2&&getVidWaterMarkEn())
		{
			nameLen=strlen(pName);
			pName[nameLen-3]='s';
			pName[nameLen-2]='r';
			pName[nameLen-1]='t';
			src->usb2Fd=osal_fopen(pName, "w");
		}
#else		
		src->muxer2=mediaMuxerAllocate(createVidName(&mediaSta.curVid2));
#endif
		src->videoStream2=mediaMuxerAllocateStream(src->muxer2, MEDIA_VIDEO);
		videoStreamInit(src->videoStream2, resW, resH, FILE_FPS);
		mediaStreamSetCodecName(src->videoStream2, "JPEG");
		src->audioStream2=mediaMuxerAllocateStream(src->muxer2, MEDIA_AUDIO);
		audioStreamInit(src->audioStream2,ADC_SAMPLEREATE_16000,1);
		mediaStreamSetCodecName(src->audioStream2, "L16");
		mediaMuxerWriteHeader(src->muxer2);
	}
	if(mediaSta.recording)
		sdCheckEnSet(0);
	audioPresentationTimeReset();
}
#if SUPPORT_AUD_REC
void audRecordStart(struct sssRecord* src)
{
	mediaSta.recording = 1;
	mediaSta.audioFileSize = 0;
	src->showTime=0;
	changeDir(WAV_FILE);
	if(src->audFD)
	{
		sdFreeSizeAdj(osal_fsize(src->audFD),0);
		osal_fclose(src->audFD);
	}
	src->audFD=NULL;
	src->audFD=osal_fopen(CREATE_AUD_NEW, "w");
	wavEncodeParse(src->audFD);
}
#endif
static u32 recordFileSize;
void vidRecordRestart(void)
{
	//mediaSta.reStart=1;
	printf("sd free:%dMB,file:%dMB\n",getSdFreeSizeMB(),recordFileSize);
}
void recordFileThread(void* d)  // thread
{
	u32 msgType,msgData,lastTime,needSize,fileSize,temp,maxFileSize,maxAudTime;
	u16 resW,resH;
	int writeVidFrames,sartDebug=0;
	u8 tCount, tCount1, tCount2;
	u8 audioFlag;
	struct frame *f,*fvid1,*fvid2;
	OS_WAIT_DONE_MSG *msg;
#if SUPPORT_AUD_REC
	USER_WAV_T audsrc;
#endif
	struct sssRecord* src = (struct sssRecord*)d;
	struct stream_dest* streamDestVid0=stream_self_get_input(&src->self, 0);
	struct stream_dest* streamDestAud=stream_self_get_input(&src->self, 1);
	struct stream_dest* streamDestVid1=stream_self_get_input(&src->self, 2);
	struct stream_dest* streamDestVid2=stream_self_get_input(&src->self, 3);
	//f_mkdir("1:/DCIM");
	printf("task record enter,0x%x:%d,0x%x:%d,0x%x:%d,0x%x:%d\n",streamDestVid0,streamDestVid0->socknum,streamDestAud,streamDestAud->socknum,streamDestVid1,streamDestVid1->socknum,streamDestVid2,streamDestVid2->socknum);
	while (1) 
	{
		if(recMsg(OS_TASK_VID_REC,&msgType,&msgData)<=0)
		{
			printf("OS_TASK_VID_REC recv msg error!!!!!!\n\n");
			continue;
		}
		switch(msgType)
		{
		case TASK_MSG_DATA_INPUT:
			if(get_sd_status()==0)
			{
				recordUninit(src);
				SysCtrl.sdcard = SDC_STAT_NULL;
				//printf("sd null\n");
				if(mediaSta.recording)
				{
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_ERROR, 0);
					sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_NULL);
				}
				continue;
			}
			if(src->audFD)
			{
				f=streamDestGetFrame(streamDestAud);
				fvid1=NULL;
				fvid2=NULL;
			}
			else
			{
				if (videoStreamGetTime(src->audioStream)<getVidRecTime()*1000)
					f=streamDestGetFrame(streamDestAud/*streamDestVid0*/);
				if(f==NULL)
					f=streamDestGetFrame(streamDestVid0/*streamDestAud*/);
				
				fvid1=streamDestGetFrame(streamDestVid1);
				fvid2=streamDestGetFrame(streamDestVid2);
			}
			if(f==NULL&&fvid1==NULL&&fvid2==NULL)
				continue;
			if ((src->photoFD!=NULL) || (src->photoFD1!=NULL) || (src->photoFD2!=NULL))
			{
					if(f && f->mediaType==MEDIA_VIDEO && src->photoFD && (resW<=4032))
				{
					writFrame(src->photoFD, f);
					sdFreeSizeAdj(osal_fsize(src->photoFD),0);
					osal_fclose(src->photoFD);
					src->photoFD=NULL;
				}

				if(fvid1 && src->photoFD1)
				{
					writFrame(src->photoFD1, fvid1);
					sdFreeSizeAdj(osal_fsize(src->photoFD1),0);
					osal_fclose(src->photoFD1);
					src->photoFD1=NULL;
				}
				
				if(fvid2&& src->photoFD2)
				{
					writFrame(src->photoFD2, fvid2);
					sdFreeSizeAdj(osal_fsize(src->photoFD2),0);
					osal_fclose(src->photoFD2);
					src->photoFD2=NULL;
				}
				if((src->photoFD==NULL) && (src->photoFD1==NULL) &&(src->photoFD2==NULL))
				{
					notiyClentCountdown(0);
					if(mediaSta.photoBurstCnt)
						mediaSta.photoBurstCnt--;
					if(mediaSta.photoBurstCnt)
					{
						if(mediaSta.photoBurstEvent)
							eloop_remove_event(mediaSta.photoBurstEvent);
						mediaSta.photoBurstEvent=eloop_add_timer(TAKE_PIC_INTERVAL*1000, 0, takePhotoBurstCnt, NULL);
					}
					else
					{
						if(configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)
						{
							int rr;
							PicAiRet_T *aiContent;
#if 1//pic identify						
	                        if(SysCtrl.photoMode==PHOTO_IDCONTENT)
	                        {
	                        	if (R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
	                        		SysCtrl.ailangid = PICID_LANGID_SCH;
								else
									SysCtrl.ailangid = PICID_LANGID_ENG;

								//ai start
								rr = picIdtfyStart(mediaSta.lastPicName);
								if (rr == 0)
								{
									picIdtfySetLangid(SysCtrl.ailangid);
									picIdtfySetUserRole(SysCtrl.aiuserRole);
									sendMsgNonblock(OS_TASK_UI, VID_EVENT_LIVE_PREVIEW, 0);
									sendMsgNonblock(OS_TASK_UI,VID_EVENT_IDENTIFY_FINISH,0);
									aiContent = picIdtfyContentGet();
									requestAudioDownload(aiContent);
								}
								else
								{
									printf("==picIdtfyStart err, ret:%d\n", rr);
								}
							}
							else if(SysCtrl.photoMode==PHOTO_SMALL_MODE)
	                        {
	                        	/*if (R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
	                        		SysCtrl.ailangid = PICID_LANGID_SCH;
								else
									SysCtrl.ailangid = PICID_LANGID_ENG;*/

								rr = picIdtfySwStart(mediaSta.lastPicName);
								if (rr == 0)
								{
									sendMsgNonblock(OS_TASK_UI, VID_EVENT_LIVE_PREVIEW, 0);
									aiContent = picIdtfySwContentGet();
									sendMsgNonblock(OS_TASK_UI,VID_EVENT_IDENTIFY_FINISH,(uint32)aiContent);
								}
								else
								{
									printf("==picIdtfySwStart err, ret:%d\n", rr);
								}
								
							}
							else if (SysCtrl.photoMode==PHOTO_AI_CHAT)
							{
	                        	if (R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
	                        		SysCtrl.ailangid = PICID_LANGID_SCH;
								else
									SysCtrl.ailangid = PICID_LANGID_ENG;
									
								rr = picIdtfyStart(mediaSta.lastPicName);
								if (rr == 0)
								{
									picIdtfySetLangid(SysCtrl.ailangid);
									picIdtfySetUserRole(SysCtrl.aiuserRole);
									sendMsgNonblock(OS_TASK_UI,VID_EVENT_IDENTIFY_FINISH,(uint32)aiContent);
								}
								else
								{
									printf("==picIdtfy AI_CHAT err, ret:%d\n", rr);
								}
								
							}
#endif			
#if 1//ljftoon
	                        else if(SysCtrl.photoMode==PHOTO_LIFTOON)
	                        {
								rr = ljftoonStart(mediaSta.lastPicName);
								if (rr == 0)
								{
									aiContent = ljftoonGet();
									if ((aiContent != NULL)&&(aiContent->urlNum > 0))
									{
										rr = picAiDownloadFile(aiContent->url[0], CREATE_PIC_NEW);
										picAiReleaseAllRetult();
										sendMsgNonblock(OS_TASK_UI,VID_EVENT_LIVE_PREVIEW,0);
									}
									else
									{
										printf("===%s,%d, aiContent is NULL\n", __FUNCTION__, __LINE__);
									}
								}
								else
								{
									printf("==ljftoonStart err, ret:%d\n", rr);
								}
							}
#endif

#if 1 //ljf
	                        else if(SysCtrl.photoMode==PHOTO_LIF)
	                        {
								rr = ljfStart(mediaSta.lastPicName);
								if (rr == 0)
								{
									ljfSetNetType(SysCtrl.ainetType);
									aiContent = ljfGet();
									if ((aiContent != NULL)&&(aiContent->urlNum > 0))
									{
										rr = picAiDownloadFile(aiContent->url[0], CREATE_PIC_NEW);
										picAiReleaseAllRetult();
										sendMsgNonblock(OS_TASK_UI,VID_EVENT_LIVE_PREVIEW,0);
									}
									else
									{
										printf("==%s,%d, aiContent is NULL\n", __FUNCTION__, __LINE__);
									}
								}
								else
								{
									printf("==ljfStart err, ret:%d\n", rr);
								}
							}
#endif
															
								mediaSta.lastPicName = NULL;
						}
						
						mediaSta.clientFd=0xffffffff;
						mediaSta.takePhotoWorking=0;
						saveFileInfor(JPG_FILE);			
					}
					printf("take photo end\n");
				//}
					if(src->muxer==NULL)
					{
						streamSetPause(streamDestVid0);
						streamDestFreeFrames(streamDestVid0,0);

						streamSetPause(streamDestVid1);
						streamDestFreeFrames(streamDestVid1,0);

						streamSetPause(streamDestVid2);
						streamDestFreeFrames(streamDestVid2,0);
						
						freeFrame(f);
						f=NULL;
						freeFrame(fvid1);
						fvid1=NULL;
						freeFrame(fvid2);
						fvid2=NULL;
						continue;
					}
				}
				if(src->muxer==NULL)
				{
					freeFrame(f);
					f=NULL;
					freeFrame(fvid1);
					fvid1=NULL;
					freeFrame(fvid2);
					fvid2=NULL;
				}
				continue;
			}
			writeVidFrames=6;
			while(f||fvid1||fvid2)
			{
#if SUPPORT_AUD_REC			
				if(src->audFD)
				{
					osal_fwrite(f->buf.dat+8, 1, f->buf.len-8, src->audFD);
					wavEncodeAddSize(f->buf.len-8);
				}
				else
#endif				
				{
					if (mediaSta.videorecRedLevel != 0)
					{
						if ((f && f->mediaType==MEDIA_VIDEO && (tCount++<mediaSta.videorecRedLevel)) 
							|| (f && f->mediaType==MEDIA_AUDIO && audioFlag>2))
						{
							//printf("@");
							freeFrame(f);
							f=NULL;
						}
						if (fvid1 && (tCount1++<mediaSta.videorecRedLevel) && (src->recording1Time+100>=src->recordingTime))
						{
							//printf("#");
							freeFrame(fvid1);
							fvid1=NULL;
						}
						if (fvid2 && (tCount2++<mediaSta.videorecRedLevel) && (src->recording2Time+100>=src->recordingTime))
						{
							//printf("$\n");
							freeFrame(fvid2);
							fvid2=NULL;
						}
					}

					lastTime=os_time_get();
					if(fvid1&&fvid1->mediaType==MEDIA_VIDEO&&src->videoStream1)
					{
						//printf("wr usb cam frame\n");
						tCount1 = 0;
						mediaStreamWritePacket(src->videoStream1,fvid1);				
						writeWMark(src);
					}
					if(fvid2&&fvid2->mediaType==MEDIA_VIDEO&&src->videoStream2)
					{
						//printf("wr usb cam frame\n");
						tCount2 = 0;
						mediaStreamWritePacket(src->videoStream2,fvid2);				
						writeWMark(src);
					}
					//if(f->mediaType==MEDIA_VIDEO&&src->videoStream&&(src->getThumbnail==0||(src->getThumbnail&&f->next)))
					if(f&&f->mediaType==MEDIA_VIDEO&&src->videoStream)
					{
						src->getThumbnail=0;
						tCount = 0;
						mediaStreamWritePacket(src->videoStream,f);
					}
					else if(f&&f->mediaType==MEDIA_AUDIO&&src->audioStream)
					{
						mediaStreamWritePacket(src->audioStream,f);
						mediaStreamWritePacket(src->audioStream1,f);
						mediaStreamWritePacket(src->audioStream2,f);
						audioFlag++;
					}	
					if(os_time_get()-lastTime>800)
						printf("write time:%d\n",os_time_get()-lastTime);
				}
				freeFrame(f);
				f=NULL;
				freeFrame(fvid1);
				fvid1=NULL;
				freeFrame(fvid2);
				fvid2=NULL;
				if(get_sd_status()==0)
					break;
				writeVidFrames--;
				if(writeVidFrames>0)
				{
					if(src->audFD)
					{
						if(writeVidFrames>1)
							f=streamDestGetFrame(streamDestAud);
					}
					else
					{				
						if(writeVidFrames>1)
							f=streamDestGetFrame(streamDestVid0);
						fvid1=streamDestGetFrame(streamDestVid1);
						fvid2=streamDestGetFrame(streamDestVid2);
						if((f==NULL) && (videoStreamGetTime(src->audioStream)<getVidRecTime()*1000))
							f=streamDestGetFrame(streamDestAud);
					}
				}
				else
				{
					break;
					//writeVidFrames=6;
					//f=streamDestGetFrame(streamDestAud);
					//if(f==NULL)
					//	f=streamDestGetFrame(streamDestVid0);
				}
				
			}
#if SUPPORT_AUD_REC			
			if(src->audFD)
			{
				src->recordingTime = wavEncodeGetDatasize()/8000/4 * 1000;
				if(src->recordingTime >= /*getVidRecTime()*/getAudioRecTime() * 1000)
				{
					streamDestFreeFrames(streamDestAud,0);
					wavEncodeEnd(src->audFD);
					if(src->audFD)
					{
						sdFreeSizeAdj(osal_fsize(src->audFD),0);
						osal_fclose(src->audFD);
					}
					src->audFD=NULL;
					mediaSta.recording = 0;
					mediaSta.recordingTime=0;
					audRecordStart(src);
					if(mediaSta.recording)
						sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_RESTART, 0);
					else
					{
						audRecordStop(src);
						sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
					}
				}
				break;
			}
#endif			
			src->recordingTime=videoStreamGetTime(src->videoStream);
			src->recording1Time=videoStreamGetTime(src->videoStream1);
			src->recording2Time=videoStreamGetTime(src->videoStream2);
			maxFileSize=fileSize=mediaMuxerGetCurSize(src->muxer)>>20;
			temp=mediaMuxerGetCurSize(src->muxer1)>>20;
			fileSize+=temp;
			if(maxFileSize<temp)
				maxFileSize=temp;
			temp=mediaMuxerGetCurSize(src->muxer2)>>20;
			fileSize+=temp;
			if(maxFileSize<temp)
				maxFileSize=temp;
			maxAudTime=videoStreamGetTime(src->audioStream);
			if(videoStreamGetTime(src->audioStream1)>maxAudTime)
				maxAudTime=videoStreamGetTime(src->audioStream1);
			if(videoStreamGetTime(src->audioStream2)>maxAudTime)
				maxAudTime=videoStreamGetTime(src->audioStream2);
			recordFileSize=fileSize;
			if((mediaMuxerSlowRecGet()==0 && src->recordingTime>=getVidRecTime()*1000)||\
				maxAudTime>=getVidRecTime()*2000||\
				maxFileSize>=(4*1024-100)||fileSize+10>=getSdFreeSizeMB()||mediaSta.reStart ||\
				(mediaMuxerSlowRecGet()>0 && src->showTime>=getVidRecTime()))
			{
				src->showTime = 0;
				mediaSta.reStart=0;
				streamDestFreeFrames(streamDestVid0,0);
				streamDestFreeFrames(streamDestAud,0);
				streamDestFreeFrames(streamDestVid1,0);
				streamDestFreeFrames(streamDestVid2,0);
				lastTime=os_time_get();
				mediaSta.freeFrame=1;
				printf("\nvideo record restart==>>\n");
				printf("file size:%dMB\n",fileSize);
				mediaMuxerClosure(src->muxer);
				src->muxer=NULL;
				src->videoStream=NULL;
				src->audioStream=NULL;
				mediaMuxerClosure(src->muxer1);
				src->muxer1=NULL;
				src->videoStream1=NULL;
				src->audioStream1=NULL;
				mediaMuxerClosure(src->muxer2);
				src->muxer2=NULL;
				src->videoStream2=NULL;
				src->audioStream2=NULL;
				#if SUBTITLES_SUPPORT
				if(src->usb1Fd)
				{
					if(src->wmLen)
						osal_fwrite(src->wmBuff,1,src->wmLen,src->usb1Fd);
					sdFreeSizeAdj(osal_fsize(src->usb1Fd),0);
					osal_fclose(src->usb1Fd);
					src->usb1Fd = NULL;
				}
				if(src->usb2Fd)
				{
					if(src->wmLen)
						osal_fwrite(src->wmBuff,1,src->wmLen,src->usb2Fd);
					sdFreeSizeAdj(osal_fsize(src->usb2Fd),0);
					osal_fclose(src->usb2Fd);
					src->usb2Fd = NULL;
				}
				src->wmLen=0;
				#endif
				changeDir(AVI_FILE);
				needSize=getVidNeedSize();
				printf("need size:%dMB\n",needSize);
				if(getVidRecLoopEn())
					needDeletFile(AVI_FILE,needSize);
				mediaSta.freeFrame=0;
				if(getSdFreeSizeMB()<needSize)
				{
					vidRecordStop(src);
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
					SysCtrl.sdcard = SDC_STAT_FULL;
					sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
					deviceState(DEVICE_STATUS_INSUFFICIENT_STORAGE);
					printf("sd no enough space for record\n");
					break;
				}
				getVidRes(&resW,&resH);
				if(getLocalResW()!=resW||getLocalResH()!=resH)
				{
					setLocalRes(resW,resH);
					streamSetPause(streamDestVid0);
					streamDestFreeFrames(streamDestVid0,0);
					streamSetRuning(streamDestVid0);
				}
				src->getThumbnail=1;
				thumbnailStart();
				vidRecordStart(src);
				if(mediaSta.recording)
				{
					if (src->muxer1)
						streamSetRuning(streamDestVid1);
					if (src->muxer2)
						streamSetRuning(streamDestVid2);
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_RESTART, 0);
				}
				else
				{
					vidRecordStop(src);
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
				}
				audioFlag = 0;
				/////////////////////////////////
				printf("video record restart use time:%d\n",os_time_get()-lastTime);
				//sendMsg(OS_TASK_VID_REC, MSG_REC_RESTART, 0);
				sartDebug=1;
			}
			else if(mediaSta.recording)
			{
				if(getSdFreeSizeMB()>fileSize)
				{
					u32 remainSize=getSdFreeSizeMB()-fileSize;
					if(getVidRecLoopEn()==0&&remainSize<=30)
					{
						vidRecordStop(src);
						sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
					}
				}
			}
			break;
		case TASK_MSG_VID_LOCK:
			if(src->muxer)
			{
				if(msgData&&!curVidIsLock())
				{
					mediaMuxerLockFlie(src->muxer,1);
					setFileLock(mediaSta.curVid,1);
					mediaMuxerLockFlie(src->muxer1,1);
					setFileLock(mediaSta.curVid1,1);
					mediaMuxerLockFlie(src->muxer2,1);
					setFileLock(mediaSta.curVid2,1);
					#if SUBTITLES_SUPPORT
					f_lock(src->usb1Fd,1);
					f_lock(src->usb2Fd,1);
					#endif
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_LOCK_FILE, 1);
				}
				else if(msgData==0&&curVidIsLock())
				{
					mediaMuxerLockFlie(src->muxer,0);
					setFileLock(mediaSta.curVid,0);
					mediaMuxerLockFlie(src->muxer1,0);
					setFileLock(mediaSta.curVid1,0);
					mediaMuxerLockFlie(src->muxer2,0);
					setFileLock(mediaSta.curVid2,0);
					#if SUBTITLES_SUPPORT
					f_lock(src->usb1Fd,0);
					f_lock(src->usb2Fd,0);
					#endif
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_LOCK_FILE, 0);
				}
			}
			break;
		case TASK_MSG_WAIT_DONE:
			msg=(OS_WAIT_DONE_MSG *)msgData;
			switch(msg->msgID)
			{
				case TASK_MSG_VID_REC_START:
					if(get_sd_status()==0||SysCtrl.sdcard==SDC_STAT_ERROR)
					{
						sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
						printf("sd card offline\n");
						break;
					}
					if(mediaSta.recording)
						break;
					changeDir(AVI_FILE);
					needSize=getVidNeedSize();
					printf("need size:%dMB\n",needSize);
					if(getVidRecLoopEn())
						needDeletFile(AVI_FILE,needSize);
					if(getSdFreeSizeMB()<needSize)
					{
						printf("sd no enough space for record\n");
						SysCtrl.sdcard = SDC_STAT_FULL;
						deviceState(DEVICE_STATUS_INSUFFICIENT_STORAGE);
						sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
						break;
					}
					src->getThumbnail=1;
					thumbnailStart();
					vidRecordStart(src);
					//////////////////////////////////////////////
					src->showTime=0;
					if(mediaSta.recording)
					{
						SysCtrl.sdcard = SDC_STAT_NORMAL;
						user_action(MSG_ID_RECORD_VIDEO);
						getVidRes(&resW,&resH);
						setLocalRes(resW,resH);
						if(getVidWaterMarkEn())
							localWaterMarkEn(1);
						else
							localWaterMarkEn(0);
						streamSetPause(streamDestVid0);
						streamDestFreeFrames(streamDestVid0,0);
						streamDestFreeFrames(streamDestAud,0);
						streamDestFreeFrames(streamDestVid1,0);
						streamDestFreeFrames(streamDestVid2,0);
						streamSetRuning(streamDestVid0);
						streamSetRuning(streamDestAud);
						if(usbCameraRecord())
							streamSetRuning(streamDestVid1);
						if(usb2CameraRecord())
							streamSetRuning(streamDestVid2);
						if(src->recordTimer)
						{
							irqTimerDestory(src->recordTimer);
							while(1)
							{
								printf("video record irq timer err\n");
							}
						}
						sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_START, 0);
						src->recordTimer=irqTimerCreate(1000,dealVideoRecordTime,src);
						audioFlag = 0;
					}
					printf("video record start\n");
					printf("heap remain size: %d [%dKB]\n",xPortGetFreeHeapSize(),xPortGetFreeHeapSize()>>10);
					break;
				case TASK_MSG_VID_REC_STOP:
					if(mediaSta.recording==0)
						break;
					vidRecordStop(src);
					saveFileInfor(AVI_FILE);
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
					mediaSta.videorecRedLevel = 0;
					printf("video record stop\n");
					break;	
				case TASK_MSG_TAKE_PHOTO_START:
					if(get_sd_status()==0||SysCtrl.sdcard==SDC_STAT_ERROR)
					{
						sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
						mediaSta.takePhotoWorking=0;
						printf("sd card offline\n");
						break;
					}
					if((src->photoFD!=NULL) || (src->photoFD1!=NULL) || (src->photoFD2!=NULL))
					{
						mediaSta.takePhotoWorking=0;
						printf("take photo ......\n");
						break;
					}
					#if 1
					changeDir(JPG_FILE);
					needDeletFile(JPG_FILE,20);
					if(getSdFreeSizeMB()<20)
					{
						mediaSta.takePhotoWorking=0;
						SysCtrl.sdcard = SDC_STAT_FULL;
						deviceState(DEVICE_STATUS_INSUFFICIENT_STORAGE);
						sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
						printf("sd no enough space for take photo\n");
						break;
					}
					#endif
					mediaSta.lastPicName = CREATE_PIC_NEW;
					src->photoFD=osal_fopen(mediaSta.lastPicName, "w");
					if((src->photoFD!=NULL))
					{
						if(mediaSta.recording==0)
						{
							if(curFuncPhoto()||takePhotoHighResEn())
								getPhotoRes(&resW,&resH);
							else
								getVidRes(&resW,&resH);
							setLocalRes(resW,resH);
								if ((resW>4032) || (resH>3024))
								{
									w_test_open(src->photoFD, 32*1024);
								}
							if(getPhotoWaterMarkEn())
								localWaterMarkEn(1);
							else
								localWaterMarkEn(0);
						}
						thumbnailStart();
						streamSetRuning(streamDestVid0);
						if (usbCameraRecord())
						{
							src->photoFD1=osal_fopen(CREATE_PIC_NEW, "w");
							streamSetRuning(streamDestVid1);
						}
						if (usb2CameraRecord())
						{
							src->photoFD2=osal_fopen(CREATE_PIC_NEW, "w");
							streamSetRuning(streamDestVid2);
						}
					}
					else
					{
						osal_fclose(src->photoFD);
						src->photoFD=NULL;
						osal_fclose(src->photoFD1);
						src->photoFD1=NULL;
						osal_fclose(src->photoFD2);
						src->photoFD2=NULL;
						mediaSta.takePhotoWorking=0;
					}
					printf("take photo start\n");
					break;
				case TASK_MSG_TAKE_PHOTO_FINISH:
					if (src->photoFD!=NULL)
					{
						sdFreeSizeAdj(osal_fsize(src->photoFD),0);
						osal_fclose(src->photoFD);
						if (msg->msgData)
						{
							printf("===PHOTO_FINISH, err\n");
						}
						src->photoFD=NULL;						
						if(src->photoFD==NULL)
						{
							notiyClentCountdown(0);
							if(mediaSta.photoBurstCnt)
								mediaSta.photoBurstCnt--;
							if(mediaSta.photoBurstCnt)
							{
								if(mediaSta.photoBurstEvent)
									eloop_remove_event(mediaSta.photoBurstEvent);
								mediaSta.photoBurstEvent=eloop_add_timer(TAKE_PIC_INTERVAL*1000, 0, takePhotoBurstCnt, NULL);
							}
							else
							{
								mediaSta.clientFd=0xffffffff;
								//mediaSta.takePhotoWorking=0;
								//saveFileInfor(JPG_FILE);
								mediaSta.takePhotoWorking=0;
								saveFileInfor(JPG_FILE);
							}
							printf("take photo end\n");
							mediaSta.takePhotoWorking = 0;
							if(src->muxer==NULL)
							{
								streamSetPause(streamDestVid0);
								streamDestFreeFrames(streamDestVid0,0);
							}
						}
					}
					break;
				#if SUPPORT_AUD_REC
				case TASK_MSG_AUD_REC_START:
					if(get_sd_status()==0)
					{
						sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, SDC_STAT_FULL);
						printf("sd card offline\n");
						break;
					}
					if(mediaSta.recording)
						break;
					changeDir(WAV_FILE);
					audsrc.channel = 1;
					audsrc.samplerate= ADC_SAMPLEREATE_16000;
				    audsrc.type = WAV_TYPE_PCM;
				    wavEncodeInit(&audsrc,&audsrc);
					audRecordStart(src);
					streamSetRuning(streamDestAud);
					printf("auadio record start\n");
					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_START, 0);
					src->recordTimer=irqTimerCreate(1000,dealVideoRecordTime,src);
					break;
				case TASK_MSG_AUD_REC_STOP:
					if(mediaSta.recording==0)
						break;
					streamSetPause(streamDestAud);
					streamDestFreeFrames(streamDestAud,0);
					audRecordStop(src);
					printf("auadio record stop\n");

					sendMsgNonblock(OS_TASK_UI,VID_EVENT_REC_STOP, 0);
					break;
				#endif
				case TASK_MSG_UNINIT:
					printf("record uninit\n");
					recordUninit(src);
					break;
				default:
					printf("recordFile,wait msg:%d\n",msg->msgID);
					break;
			}
			msg->done(msg);
			break;
		default:
			printf("recordFile,unknow msg:%d\n",msgType);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
static int wakeup_sender (struct frame *input, void *d)  // process frame
{
	struct stream_dest* streamDest=(struct stream_dest*)d;
	struct sssRecord *xchg = (struct sssRecord*)streamDest->self;	
	if(mediaSta.freeFrame)
	{
		freeFrame(input);
		return 0;
	}
	if(streamDestAddFrame(streamDest, input)<0)
	{
		return 0;
		//printf("add frame err,0x%x,0x%x,%d\n",streamDest,input,streamDest->socknum);
	}
	//uint32_t flags;
	//flags = arch_local_irq_save();
	sendMsgNonblock(OS_TASK_VID_REC,TASK_MSG_DATA_INPUT, 0);
	//arch_local_irq_restore(flags);
  return 0;
}
static void record_set_status (struct stream *s, S_INPUT_STATUS  status)
{
	struct sssRecord* src = (struct sssRecord*)s->self;
	struct stream_dest* streamDest;
	int i;
	while(1)
	{
		printf("OS_TASK_VID_REC not surport set status\n");
	}
	if(status==S_INPUT_PAUSE)
	{
		//sendMsgAndWait(OS_TASK_VID_REC,TASK_MSG_UNINIT,0);
	}
}
static struct stream_self_ops recordSelf_ops = {
  .set_status = record_set_status,
  .lock = arch_local_irq_save,
  .unlock = arch_local_irq_restore,
};
static struct stream_src_ops record_ops = {
	.stream_info = NULL,
	.bufsize = NULL,
	.free = (frame_free_func)NULL,
	.lock = arch_local_irq_save,
	.unlock = arch_local_irq_restore
};

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void* start_block(void)
{
	struct sssRecord* src = (struct sssRecord*)osal_malloc(sizeof(struct sssRecord));
	if(src == NULL) {
	p_err("rtp-sender start_block malloc error");
	return NULL;
	}
	mediaSta.curVid=-1;
	mediaSta.freeFrame=0;
	mediaSta.lastPicName = NULL;
	src->muxer=NULL;
	src->videoStream=NULL;
	src->audioStream=NULL;
	src->muxer1=NULL;
	src->videoStream1=NULL;
	src->audioStream1=NULL;
	src->muxer2=NULL;
	src->videoStream2=NULL;
	src->audioStream2=NULL;
	src->recordTimer=NULL;
	src->recordingTime=0;
	src->getThumbnail=0;
	src->audFD=NULL;
	src->photoFD=NULL;
	src->photoFD1=NULL;
	src->photoFD2=NULL;
#if SUBTITLES_SUPPORT	
	src->usb1Fd=NULL;
	src->usb2Fd=NULL;
#endif	
	stream_src_self_init (&src->self, &recordSelf_ops, NULL); // NULL heap
  return src;
}

// todo: 線程優先級定義
static int end_block(void* d)
{
  struct sssRecord *xchg = (struct sssRecord *)d;
  struct stream_dest *input = stream_dest_new(wakeup_sender, d,30);
  stream_add_input(&xchg->self, input);
  input = stream_dest_new(wakeup_sender, d,30);
  stream_add_input(&xchg->self, input);
  createdTaskByFunc(OS_TASK_VID_REC, recordFileThread,d,19,1024,"taskRecord",80);
  return 0;
}
static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct sssRecord *xchg = (struct sssRecord *)d;
	struct stream_src *output;

	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;

	// todo : 在output前進行的set_input，diffuse無效

	output->s.get_framerate = NULL;
	output->ops = &record_ops;
	xchg->self.output = output;
	return 0;
}
static int set_input(int num_tokens, struct token* tokens, void* d)
{
  struct sssRecord* xchg = (struct sssRecord*)d;
  struct stream_dest *input;

  if(!(input = stream_connect(tokens[1].v.str, (frame_deliver_func)wakeup_sender, d,30))) {
    p_err("record file: unable to connect to stream \"%s\"", tokens[1].v.str);
    return -1;
  }

  stream_add_input(&xchg->self, input);
  // note : 在output前進行的set_input，diffuse無效

  return 0;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
  /* directive name, process function, min args, max args, arg types */
  { "input", set_input, 1, 1, { TOKEN_STR } },
  {"output", set_output, 1, 1, {TOKEN_STR}},
  /* empty terminator -- do not remove */
  { NULL, NULL, 0, 0, {} }
};

int record_file_register(void)
{
  register_config_context("record", "file", start_block, end_block, config_statements);
  return 0;
}

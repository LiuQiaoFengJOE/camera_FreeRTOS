#include "rtsp/rtsp.h"
#include "osal/osal_task.h"
#include "debug.h"
#include "mediaFile/muxer.h"
#include "eloop/irqTimer.h"
#include "task/app.h"
#include "drv/lcd/lcdframe.h"
#include "drv/jpeg/jpeg.h"

struct sssPlayBack
{
	struct stream_src_self self;
	IRQ_TIMER *imageSyncTimer;
	queue vidQ;
	queue audQ;
	u32 audTime;
	u32 imageTime;
	u32 showTime;
	#if SUPPORT_AUD_ENC
	u32 audPlayOffset;
	#endif
	u32 play;
	u16 width;
	u16 hight;
	u16 fps;
};
#define CHECK_HEAD_FRAMES   30
struct sssPlayBack *playBackSta = NULL;
u32 getPlayStat(void)
{
	if (playBackSta == NULL)
		return 0;
	return playBackSta->play;
}
u32 getPlayImageWidth(void)
{
	if (playBackSta == NULL)
		return 0;
	return playBackSta->width;
}
u32 getPlayImageHight(void)
{
	if (playBackSta == NULL)
		return 0;
	return playBackSta->hight;
}

u32 frameID;
u32 audFrameID;
u32 audFrameAddr;
#define PLAY_IMAGE_BUFF_NUM 6
#define PLAY_AUDIO_BUFF_NUM 30

void imageSync(void *arg)
{
	struct sssPlayBack *src = (struct sssPlayBack *)arg;
	if (src->play == PLAY_BACK_PLAYING)
	{
		if(getCurPlaySpeed()!=1)
			irqTimerModTime(src->imageSyncTimer, 30);
		else if(src->audTime && src->audTime > src->imageTime)
			irqTimerModTime(src->imageSyncTimer, 20);
		else if (src->audTime && src->audTime < src->imageTime)
			irqTimerModTime(src->imageSyncTimer, 40);
		else
			irqTimerModTime(src->imageSyncTimer, 30);
		sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_IMAGE, 0);
	}
}
void *getPlayFrame(u8 **buff, u32 *size, void *argv)
{
	#if SUPPORT_AUD_ENC
	static uint32 audioFileOffset = 0;
	#endif
	struct frame *f;
	struct sssPlayBack *src = (struct sssPlayBack *)argv;
	if (src->play == PLAY_BACK_PLAYING)
	{
		if(queueValidData(&src->audQ)<=PLAY_AUDIO_BUFF_NUM-2)
			sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
		sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
		if (queueOutput(&src->audQ, &f) == 0)
		{
			struct list_head *cur_buffer;
			struct frame_buf *in_buff;
			if (f->buflist.next == &f->buflist)
			{
				freeFrame(f);
				*buff = NULL;
				*size = 0;
				printf("aud buff error,frame id:%d\n",f->id);
				return NULL;
			}
			cur_buffer = f->buflist.next;
			in_buff = list_entry(cur_buffer, struct frame_buf, list);
			// printf("get:%d,pos:%d,addr:0x%x,len:%d,buffAddr:0x%x\n",f->id,f->presentationTime,f->frame_staus,f->dataLen,f->dynamicBuff);
			if (f->dataLen == 0)
			{
				freeFrame(f);
				*buff = NULL;
				*size = 0;
				printf("aud len[0] error,frame id:\n", f->id);
				return NULL;
			}
			//printf("aud addr:0x%x,size:%d\n",in_buff->dat,f->dataLen);
			dcache_flush_region((u32)in_buff->dat, f->dataLen);
			*buff = in_buff->dat;
			*size = f->dataLen;
			src->audTime = f->timestamp;
			#if SUPPORT_AUD_ENC
			if(2 == getCurPlayFileType())
			{
				audioFileOffset += *size;
				src->showTime = audioFileOffset/32000;
				sendMsgNonblock(OS_TASK_UI,SYS_TIME_UPDATE,src->showTime);
			}
			#endif
#if 0
			if(src->audTime/1000!=src->showTime)
			{
				src->showTime=src->audTime/1000;
				printf("%dS ",src->showTime);
				sendMsgFromIsr(OS_TASK_UI,SYS_EVENT_TIME_UPDATE,src->showTime);
			}
#endif
			audFrameID = f->presentationTime;
			audFrameAddr = f->frame_staus;
			return f;
		}
		else
		{
			#if SUPPORT_AUD_ENC
			if(2 == getCurPlayFileType())
			{
				audioFileOffset = 0;
				sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_STOP,0);
			}
			#endif
			printf("aud play stop:%d, addr:0x%x\n", audFrameID, audFrameAddr);
		}
	}
	*buff = NULL;
	*size = 0;
	return NULL;
}
void curFrameDone(void *argv)
{
	if (argv == NULL)
		return;
	struct frame *f = (struct frame *)argv;
	// printf("free:%d\n",f->id);
	freeFrame(f);
}
extern int lcdGetVideoPos(u16 *x,u16 *y);
extern int lcdGetVideoRes(u16 *w,u16 *h);
int showVideoFrame(struct sssPlayBack *src,struct frame *f)
{
	int ret;
	if (f == NULL)
		return -1;
	disp_frame_t *lcdFrame = lcdVideoFrameMalloc();
	if (lcdFrame == NULL)
	{
		printf("lcdFrame is NULL\n");
		return -1;
	}
	u16 vidX,vidY,vidW,vidH;
	lcdGetVideoPos(&vidX,&vidY);
	lcdGetVideoRes(&vidW,&vidH);
	lcdframeVideoCfg(lcdFrame, vidX,vidY,vidW,vidH);
	//jpeCodecReset();
	if (jpgDecFrame(f, lcdFrame->y_addr, vidW, vidH,NULL) < 0)
	{
		printf("0err decode,frame time:%d\n", f->presentationTime);
		lcdframeFree(lcdFrame);
		return -1;
	}
	//printf("decode wait\n");
	do
	{
		osal_task_sleep(10);
		ret = jpgDecSta();
	} while (ret > 0);
	if (ret < 0)
	{
		printf("1err decode,frame time:%d\n", f->presentationTime);
		lcdframeFree(lcdFrame);
		return -1;
	}
	//printf("decode sec\n");
	src->imageTime = f->timestamp;
	lcdDealVideBuff(lcdFrame);
	osal_task_sleep(5);
	return 0;
}

void videoPlayThread(void *d) // thread
{
	u32 msgType, msgData, temp;
	OS_WAIT_DONE_MSG *msg;
	u32 count = 0;
	struct frame *f;
	struct sssPlayBack *src = (struct sssPlayBack *)d;
	struct stream_dest *streamDest = stream_self_get_input(&src->self, 0);
	printf("task record enter\n");
	while (1)
	{
		if (recMsg(OS_TASK_VIDEOPLAY, &msgType, &msgData) <= 0)
		{
			printf("OS_TASK_VIDEOPLAY recv msg error!!!!!!\n\n");
			continue;
		}
		switch (msgType)
		{
		case TASK_MSG_PLAY_IMAGE:
			if (src->play == PLAY_BACK_PLAYING)
			{
				if (queueOutput(&src->vidQ, &f) == 0)
				{
					if (f->dataLen)
					{
						if (showVideoFrame(src,f) != 0)
						{
							sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
						}
					}
					// dacPlay(f->audInfor.sampleRate,20,getPlayFrame,curFrameDone,src);
					//src->imageTime = f->timestamp;
					temp = queueValidData(&src->vidQ);
					if ((src->imageTime + 500) / 1000 != src->showTime)
					{
						src->showTime = (src->imageTime + 500) / 1000;
						printf("%d,%d-%dS ", queueValidData(&src->audQ), temp, src->showTime);
						sendMsgNonblock(OS_TASK_UI, SYS_TIME_UPDATE, src->showTime);
					}
					frameID = f->presentationTime;
					freeFrame(f);
					if ((f->timestamp == 0) && (getCurPlaySpeed()<0))
					{
						src->play = PLAY_BACK_PAUSE;
						if (src->imageSyncTimer)
							irqTimerDestory(src->imageSyncTimer);
						src->imageSyncTimer = NULL;
						sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYPAUSE, 0);
						playFastForward();
					}
					else
					{
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					}
					count = 0;
				}
				else if (count++<20)
				{
					if (count%2 ==0)
					{
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					}
				}
				else
				{
					if (queueValidData(&src->audQ) == 0)
					{
						printf("play stop:%d,id:%d\n", frameID,getCurTaskId());
						src->play = PLAY_BACK_STOP;
						src->showTime = 0;
						sendMsgNonblock(OS_TASK_UI, SYS_TIME_UPDATE, 0);
						if (src->imageSyncTimer)
							irqTimerDestory(src->imageSyncTimer);
						src->imageSyncTimer = NULL;
						sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_STOP,0);
						queueFreeFrames(&src->vidQ);
						queueFreeFrames(&src->audQ);
						sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
						sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
						if (queueOutput(&src->vidQ, &f) == 0)
						{
							showVideoFrame(src,f);
							freeFrame(f);
						}
						printf("send PLAYSTOP msg\n");
						sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTOP, 0);
					}
				}
			}
			break;
		case TASK_MSG_PLAY_AUDIO:
			if(queuePeek(&src->audQ, &f) == 0)
			{
				dacPlay(f->audInfor.sampleRate,getSoundVol(),getPlayFrame,curFrameDone,src);
			}
			break;
		#if SUPPORT_AUD_ENC
		case TASK_MSG_PLAY_AUD_STOP:
			src->play = PLAY_BACK_STOP;
			src->showTime = 0;
			sendMsgNonblock(OS_TASK_UI, SYS_TIME_UPDATE, 0);
			sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_STOP,0);
			queueFreeFrames(&src->audQ);
			sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
			sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTOP, 0);
			break;
		#endif
		case TASK_MSG_WAIT_DONE:
			msg = (OS_WAIT_DONE_MSG *)msgData;
			switch (msg->msgID)
			{
			case TASK_MSG_PLAY_START:
				temp = queueValidData(&src->vidQ);
				while (temp++ <= PLAY_IMAGE_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				temp = queueValidData(&src->audQ);
				while (temp++ <= PLAY_AUDIO_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);

				src->play = PLAY_BACK_PLAYING;
				if (src->imageSyncTimer)
				{
					irqTimerDestory(src->imageSyncTimer);
					while(1)
					{
						printf("play irq timer err\n");
					}
				}
				src->imageTime=0;
				src->imageSyncTimer = NULL;
				src->imageSyncTimer = irqTimerCreate(30, imageSync, src);
				sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_PLAY_STOP:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				sendMsgNonblock(OS_TASK_UI, SYS_TIME_UPDATE, 0);
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_STOP,0);
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTOP, 0);
				break;
			case TASK_MSG_PLAY_PAUSE:
				src->play = PLAY_BACK_PAUSE;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYPAUSE, 0);
				break;
			case TASK_MSG_PLAY_RESUME:
				temp = queueValidData(&src->vidQ);
				while (temp++ <= PLAY_IMAGE_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				temp = queueValidData(&src->audQ);
				while (temp++ <= PLAY_AUDIO_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				src->play = PLAY_BACK_PLAYING;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				src->imageSyncTimer = irqTimerCreate(30, imageSync, src);
				sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYRESUME, 0);
				break;
			case TASK_MSG_PLAY_NEXT:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_NEXT,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					src->width = f->imageInfor.width;
					src->hight = f->imageInfor.hight;
					src->fps = f->imageInfor.fps;
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_PLAY_PRE:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_PRE,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					src->width = f->imageInfor.width;
					src->hight = f->imageInfor.hight;
					src->fps = f->imageInfor.fps;
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;

			case TASK_MSG_PLAY_NEXT_JPG:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_NEXT_JPG,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					src->width = f->imageInfor.width;
					src->hight = f->imageInfor.hight;
					src->fps = f->imageInfor.fps;
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_PLAY_PRE_JPG:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_PRE_JPG,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					src->width = f->imageInfor.width;
					src->hight = f->imageInfor.hight;
					src->fps = f->imageInfor.fps;
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_REPLAY_CUR_FILE:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_CUR_FILE,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				temp=0;
				while(queueOutput(&src->vidQ, &f) == 0)
				{
					sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					src->width = f->imageInfor.width;
					src->hight = f->imageInfor.hight;
					src->fps = f->imageInfor.fps;
					if(showVideoFrame(src,f)>=0||temp++>=CHECK_HEAD_FRAMES)
					{
						freeFrame(f);
						break;
					}
					freeFrame(f);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
				
				
			case TASK_MSG_PLAY_FASTFORWARD:
				playFastForward();
				if(getCurPlaySpeed()==1)
				{
					sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_SEEKAUDIO,src->imageTime);
					queueFreeFrames(&src->audQ);
					temp=0;
					while (temp++ <= PLAY_AUDIO_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
					sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				}
				else
				{
					temp = queueValidData(&src->vidQ);
					while (temp++ <= PLAY_IMAGE_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					temp = queueValidData(&src->audQ);
					while (temp++ <= PLAY_AUDIO_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);		
				}
				break;
			case TASK_MSG_PLAY_FASTBACKWARD:
				playFastBackward();
				if(getCurPlaySpeed()==1)
				{
					sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_SEEKAUDIO,src->imageTime);
					queueFreeFrames(&src->audQ);
					temp=0;
					while (temp++ <= PLAY_AUDIO_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
					sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				}
				else
				{
					temp = queueValidData(&src->vidQ);
					while (temp++ <= PLAY_IMAGE_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
					temp = queueValidData(&src->audQ);
					while (temp++ <= PLAY_AUDIO_BUFF_NUM)
						sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
					sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				}
				break;
			#if SUPPORT_AUD_ENC
			case TASK_MSG_PLAY_AUD_START:
				temp = queueValidData(&src->audQ);
				while (temp++ <= PLAY_AUDIO_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				src->showTime = 0;
				src->play = PLAY_BACK_PLAYING;
				sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_PLAY_AUD_PAUSE:
				src->play = PLAY_BACK_PAUSE;
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYPAUSE, 0);
				break;
			case TASK_MSG_PLAY_AUD_RESUME:
				temp = queueValidData(&src->audQ);
				while (temp++ <= PLAY_AUDIO_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				src->play = PLAY_BACK_PLAYING;
				sendMsgNonblock(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUDIO, 0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYRESUME, 0);
				break;
			case TASK_MSG_PLAY_AUD_NEXT:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_AUD_NEXT,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			case TASK_MSG_PLAY_AUD_PRE:
				src->play = PLAY_BACK_STOP;
				src->showTime = 0;
				src->imageTime=0;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				queueFreeFrames(&src->audQ);
				streamSetRuning(streamDest);
				sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_READ_AUD_PRE,0);
				sendMsg(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
				sendMsgAndWait(OS_TASK_READ_FILE, 0,0);
				sendMsgNonblock(OS_TASK_UI, PLAY_EVENT_PLAYSTART, 0);
				break;
			#endif
			case TASK_MSG_UNINIT:
				src->play = PLAY_BACK_STOP;
				if (src->imageSyncTimer)
					irqTimerDestory(src->imageSyncTimer);
				src->imageSyncTimer = NULL;
				streamSetPause(streamDest);
				queueFreeFrames(&src->vidQ);
				queueFreeFrames(&src->audQ);
				break;
			default:
				printf("videoPlay,wait msg:%d\n", msg->msgID);
				break;
			}
			msg->done(msg);
			break;
		default:
			printf("videoPlay,unknow msg:%d\n", msgType);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
static int wakeup_videoPlay(struct frame *f, void *d) // process frame
{
	struct stream_dest *streamDest = (struct stream_dest *)d;
	struct sssPlayBack *src = (struct sssPlayBack *)streamDest->self;
	if (f->mediaType == MEDIA_VIDEO)
	{
		if (queueInput(&src->vidQ, f) < 0)
		{
			freeFrame(f);
		}
	}
	else
	{
		if (queueInput(&src->audQ, f) < 0)
		{
			freeFrame(f);
		}
	}
	return 0;
}

static struct stream_self_ops palyvideoSelf_ops = {
	.lock = arch_local_irq_save,
	.unlock = arch_local_irq_restore,
};

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void *start_block(void)
{
	struct sssPlayBack *xchg = (struct sssPlayBack *)osal_malloc(sizeof(struct sssPlayBack));
	if (xchg == NULL)
	{
		p_err("rtp-sender start_block malloc error");
		return NULL;
	}
	xchg->audTime = 0;
	xchg->showTime = 0;
	xchg->play = PLAY_BACK_STOP;
	xchg->imageSyncTimer = NULL;
	playBackSta = xchg;
	queueInit(&xchg->vidQ, 50);
	queueInit(&xchg->audQ, 50);
	stream_src_self_init(&xchg->self, &palyvideoSelf_ops, NULL); // NULL heap
	return xchg;
}

// todo: 線程優先級定義
static int end_block(void *d)
{
	// todo: 作些檢查
	// osal_thread_create(rtp_sender, d, 13, 0, 8192, "sender");
	createdTaskByFunc(OS_TASK_VIDEOPLAY, videoPlayThread, d, 12, 1024, "playBack", 50);
	return 0;
}

static int set_input(int num_tokens, struct token *tokens, void *d)
{
	struct sssPlayBack *xchg = (struct sssPlayBack *)d;
	struct stream_dest *input;

	if (!(input = stream_connect(tokens[1].v.str, (frame_deliver_func)wakeup_videoPlay, d, 0)))
	{
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
	{"input", set_input, 1, 1, {TOKEN_STR}},
	/* empty terminator -- do not remove */
	{NULL, NULL, 0, 0, {}}};

int paly_video_register(void)
{
	register_config_context("play", "video", start_block, end_block, config_statements);
	return 0;
}



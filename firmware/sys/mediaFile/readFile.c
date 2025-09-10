#include "rtsp/rtsp.h"
#include "osal/osal_task.h"
#include "mediaFile/muxer.h"
#include "mediaFile/demuxer.h"
#include "task/app.h"
#include "fatfs/fileOps.h"

struct sssReadfile {
  struct stream_src_self self;
  mediaDemuxerContext *demuxerContext;
  mediaStream *vidStream;
  mediaStream *audStream;
  int vidReadFinish;
  int speed;
  struct frame*  f;	
  void *subfp;
};
struct sssReadfile *psssReadfile;
u32 curPlayFileTotalTime(void)
{
	if(psssReadfile==NULL||psssReadfile->demuxerContext==NULL)
		return 0;
	int type = mediaDemuxerType(psssReadfile->demuxerContext);
	if(type==DEMUXER_AVI)
		return mediaDemuxerGetTotalTime(psssReadfile->vidStream);
	else
		return mediaDemuxerGetTotalTime(psssReadfile->audStream);
}
int curPlayFileIsVid(void)
{
	if(psssReadfile==NULL||psssReadfile->demuxerContext==NULL)
		return 0;
	int type=mediaDemuxerType(psssReadfile->demuxerContext);
	#if SUPPORT_AUD_ENC
	if(DEMUXER_AVI == type || DEMUXER_WAV == type)
	#else
	if(DEMUXER_AVI == type)
	#endif
		return 1;
	return 0;
}
int getCurPlayFileType(void)
{
	if(psssReadfile==NULL||psssReadfile->demuxerContext==NULL)
		return -1;
	return mediaDemuxerType(psssReadfile->demuxerContext);
}
int getCurPlayFileRes(u16 *w,u16*h)
{
	if(psssReadfile==NULL||psssReadfile->demuxerContext==NULL)
		return 1;
	return mediaDemuxerGetRes(psssReadfile->vidStream,w,h);
}
void setCurPlayFileLock(u8 lock)
{
	if(psssReadfile==NULL||psssReadfile->demuxerContext==NULL)
		return;
	mediaDemuxerFileLock(psssReadfile->demuxerContext,lock);
	setCurFileLock(lock);
	f_lock(psssReadfile->subfp,lock);	
	f_sync(psssReadfile->subfp);
}
void playFastForward(void)
{
	if(psssReadfile==NULL)
		return;
	if(psssReadfile->speed<0)
		psssReadfile->speed=1;
	else
		psssReadfile->speed++;
}
void playFastBackward(void)
{
	if(psssReadfile==NULL)
		return;
	if(psssReadfile->speed>1)
		psssReadfile->speed=1;
	else
	{
		if(psssReadfile->speed==1)
			psssReadfile->speed=-1;
		else
			psssReadfile->speed--;
	}
}
int getCurPlaySpeed(void)
{
	if(psssReadfile==NULL)
		return 1;
	return psssReadfile->speed;
}
u32 getCurPlayFileStreamEn(u32 no)
{
	return 1;
}
int getVidReadFinish(void)
{
	if(psssReadfile==NULL||psssReadfile->vidStream==NULL)
		return 1;
	return psssReadfile->vidReadFinish;
}
void readfileThread(void* d)  // thread
{
	u32 msgType,msgData,lastTime,i;
	OS_WAIT_DONE_MSG *msg;
	int ret;
	char *name;
	int nameLen;
	struct sssReadfile* src = (struct sssReadfile*)d;

	printf("task readfile enter\n");
	while (1) 
	{
		if(recMsg(OS_TASK_READ_FILE,&msgType,&msgData)<=0)
		{
			printf("OS_TASK_READ_FILE	 recv msg error!!!!!!\n\n");
			continue;
		}
		switch(msgType)
		{
		case TASK_MSG_READ_IMAGE:
			if(src->demuxerContext&&src->vidStream)
			{
				src->f=streamNewFrame(src->self.output); 
				if(src->f==NULL)
				{
					printf("image no frame\n");
					streamPrintFreeFrame(src->self.output);
				}
				else if((ret=mediaStreamRead(src->vidStream,src->f,src->speed))>=0)
				{
					stream_dest_inturn (&src->self.output->s, src->f);
				}
				else
				{
					//printf("1 read frame failed\n");
					if(ret==-2)
					{
						src->vidReadFinish=1;
					}
					freeFrame(src->f);
				}
			}
			break;
		case TASK_MSG_READ_AUDIO:
			if(src->demuxerContext&&src->audStream)
			{
				src->f=streamNewFrame(src->self.output); 
				if(src->f==NULL)
				{
					printf("audio no frame\n");
					streamPrintFreeFrame(src->self.output);
				}
				else if(mediaStreamRead(src->audStream,src->f,src->speed)>=0)
				{
					stream_dest_inturn (&src->self.output->s, src->f);
				}
				else
				{
					//printf("2 read frame failed\n");
					freeFrame(src->f);
				}
			}
			break;
		case TASK_MSG_WAIT_DONE:
			msg=(OS_WAIT_DONE_MSG *)msgData;
			switch(msg->msgID)
			{
				case TASK_MSG_READ_STOP:
					src->speed=1;
					src->vidReadFinish=0;
					mediaDemuxerSeek(src->demuxerContext,0);
					break;
				case TASK_MSG_READ_START:
					//streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=msg->msgData;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;
				case TASK_MSG_READ_NEXT:
					streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=FIND_NEXT_VID;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;
				case TASK_MSG_READ_PRE:
					streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=FIND_PRE_VID;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;
					
				case TASK_MSG_READ_NEXT_JPG:
					streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=FIND_NEXT_JPG;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;
				case TASK_MSG_READ_PRE_JPG:
					streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=FIND_PRE_JPG;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;

				case TASK_MSG_READ_CUR_FILE:
					streamPrintFreeFrame(src->self.output);
					changeDir(AVI_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					name=FIND_CUR_FILE;
					src->demuxerContext=mediaDemuxerCreateContext(name);
					src->vidStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_VIDEO);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					if(src->demuxerContext)
					{
						nameLen=strlen(name);
						name[nameLen-3]='s';
						name[nameLen-2]='r';
						name[nameLen-1]='t';
						src->subfp=osal_fopen(name, "r");
					}
					break;
					
				case TASK_MSG_READ_FASTFORWARD:
					if(src->speed<0)
						src->speed=1;
					else
						src->speed++;
					//printf("speed:%d\n",src->speed);
					break;
				case TASK_MSG_READ_FASTBACKWARD:
					if(src->speed>1)
						src->speed=1;
					else
					{
						if(src->speed==1)
							src->speed=-1;
						else
							src->speed--;
					}
					//printf("speed:%d\n",src->speed);
					break;
				case TASK_MSG_READ_SEEKAUDIO:
					mediaDemuxerSeekAudio(src->audStream,msg->msgData);
					printf("seek:%d\n",msg->msgData);
					break;
				#if SUPPORT_AUD_ENC
				case TASK_MSG_READ_AUD_NEXT:
					streamPrintFreeFrame(src->self.output);
					changeDir(WAV_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					src->demuxerContext=mediaDemuxerCreateContext(FIND_NEXT_WAV);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					break;
				case TASK_MSG_READ_AUD_PRE:
					streamPrintFreeFrame(src->self.output);
					changeDir(WAV_FILE);
					src->speed=1;
					src->vidReadFinish=0;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					src->demuxerContext=mediaDemuxerCreateContext(FIND_PRE_WAV);
					src->audStream=mediaDemuxerGetStreamByType(src->demuxerContext,MEDIA_AUDIO);
					mediaDemuxerSeek(src->demuxerContext,0);
					break;
				#endif
				case TASK_MSG_UNINIT:
					src->speed=1;
					src->vidReadFinish=1;
					if(src->demuxerContext)
						mediaDemuxerClosure(src->demuxerContext);
					src->demuxerContext= NULL;
					src->vidStream=NULL;
					src->audStream=NULL;
					osal_fclose(src->subfp);
					src->subfp=NULL;
					break;
				default:
					printf("readfile,wait msg:%d\n",msg->msgID);
					break;
			}
			msg->done(msg);
			break;
		default:
			printf("readfile,unknow msg:%d\n",msgType);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
static void readfile_set_status (struct stream *s, S_INPUT_STATUS  status)
{
	struct sssReadfile* src = (struct sssReadfile*)s->self;
	struct stream_dest* streamDest;
	int i;
	if(status==S_INPUT_PAUSE)
	{
		sendMsgAndWait(OS_TASK_READ_FILE, TASK_MSG_UNINIT,0);
	}
}
static struct stream_self_ops readfileSelf_ops = {
  .set_status = readfile_set_status,
  .lock = arch_local_irq_save,
  .unlock = arch_local_irq_restore,
};
static struct stream_src_ops readfile_ops = {
  .bufsize = (frame_size_func)frame_bufsize,
  .lock = arch_local_irq_save,
  .unlock = arch_local_irq_restore,
};


/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void* start_block(void)
{
  struct sssReadfile* src = (struct sssReadfile*)osal_malloc(sizeof(struct sssReadfile));
  if(src == NULL) {
    p_err("rtp-sender start_block malloc error");
    return NULL;
  }
  mediaDemuxerInit();
  psssReadfile=src;
  src->vidStream=NULL;
  src->audStream=NULL;
  src->demuxerContext= NULL;
  src->f=NULL;
  src->speed=1;
  src->vidReadFinish=0;
  src->subfp=NULL;
  stream_src_self_init (&src->self, &readfileSelf_ops, NULL); // NULL heap
  return src;
}

// todo: 線程優先級定義
static int end_block(void* d)
{
  // todo: 作些檢查
  //osal_thread_create(rtp_sender, d, 13, 0, 8192, "sender");
  createdTaskByFunc(OS_TASK_READ_FILE, readfileThread,d,13,1024,"readFile",50);
  return 0;
}

static int set_output(int num_tokens, struct token* tokens, void* d)
{
  struct sssReadfile* src = (struct sssReadfile*)d;
  struct stream_src *output;
  output = stream_src_new(tokens[1].v.str, d);
  if(!output)
    return -1;

  output->ops = &readfile_ops;
  src->self.output = output;
  return 0;
}
static int set_frameheap(int num_tokens, struct token *tokens, void *d)
{
  struct sssReadfile *src = (struct sssReadfile *)d;
  struct stream_src *output = src->self.output;

  if (NULL == output)
  {
    p_err("jpgencAuto: output must be specified before frameheap");
    return -1;
  }
  output->frame_num = tokens[1].v.num;
  output->s.buf_size = tokens[2].v.num;
  output->buf_num = tokens[3].v.num;
  output->lst_size = sizeof(struct frame_buf);
  return 0;
}


/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
  /* directive name, process function, min args, max args, arg types */
  {"output", set_output, 1, 1, { TOKEN_STR } },
  {"frameheap", set_frameheap, 3, 3, {TOKEN_NUM, TOKEN_NUM, TOKEN_NUM}},
  /* empty terminator -- do not remove */
  { NULL, NULL, 0, 0, {} }
};

int read_file_register(void)
{
  register_config_context("read", "file", start_block, end_block, config_statements);
  return 0;
}




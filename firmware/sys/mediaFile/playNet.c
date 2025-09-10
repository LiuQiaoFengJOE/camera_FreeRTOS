#include "rtsp/rtsp.h"
#include "osal/osal_task.h"
#include "debug.h"
#include "mediaFile/muxer.h"
#include "eloop/irqTimer.h"
#include "task/app.h"
#include "drv/lcd/lcdframe.h"
#include "drv/jpeg/jpeg.h"

struct sssPlayNet
{
	struct stream_src_self self;
	IRQ_TIMER *imageSyncTimer;
	struct stream_src *audioStream;
	queue vidQ;
	queue audQ;
	u32 audTime;
	u32 imageTime;
	u32 lastSendImageTime;
	u32 playTime;
	u32 running;
	u32 shutDownCnt;
	u32 firstFrame;
	u32 sendPicCnt;
};
static int playPause=1;
int playPauseSet(int en)
{
	playPause=en;
}
int getPlayPause(void)
{
	return playPause;
}
#define PLAY_INTERVAL     60
#define PLAY_BUFF_NUM   6
#define SEND_IMAGE_FIRST   1
void playNetSync(void *arg)
{
	static u32 lastTime;
	struct frame *f;
	u32 temp;
	if(playPause)
		return ;
	struct sssPlayNet *src = (struct sssPlayNet *)arg;
	if(getVidReadFinish())
	{
		if(queueValidData(&src->vidQ)==0&&queueValidData(&src->audQ)==0)
		{
			playPause=1;
			return ;
		}
	}
	
	if (src->running)
	{
		src->playTime+=PLAY_INTERVAL;
		while(src->playTime>=src->imageTime)
		{
			if(queueOutput(&src->vidQ, &f) == 0)
			{
				if(((f->timestamp-src->lastSendImageTime>=PLAY_INTERVAL)&&f->dataLen)||src->firstFrame)
				{
					src->imageTime=f->timestamp;
					src->lastSendImageTime=f->timestamp;
					f->timestamp*=90;
					src->firstFrame=0;
					#if SEND_IMAGE_FIRST
					if(src->sendPicCnt<20)
						src->sendPicCnt++;
					stream_dest_inturn(&src->self.output->s, f);
					#else
					if(src->sendPicCnt>=20)
					{
						if(os_time_get()-lastTime>3000)
						{
							lastTime=os_time_get();
							printf("send jpg vidQ:%d,audQ:%d,readMsg:%d\n",queueValidData(&src->vidQ),queueValidData(&src->audQ),getMsgQWaiting(OS_TASK_READ_FILE));
						}
						stream_dest_inturn(&src->self.output->s, f);
					}
					else
						freeFrame(f);
					#endif
					
				}
				else
				{
					src->imageTime=f->timestamp;
					freeFrame(f);
				}
			}
			else
				break;
		}
		temp = queueValidData(&src->vidQ);
		#if 0
		if(src->playTime>2000&&temp==0)
		{
			if(getVidReadFinish())
			{
				src->shutDownCnt++;
				if(src->shutDownCnt==3)
				{
					if (src->imageSyncTimer)
						irqTimerDestory(src->imageSyncTimer);
					src->imageSyncTimer = NULL;
					src->shutDownCnt=0;
					printf("drop conn\n");
					dropConnFromEloop();
					playPauseSet(1);
				}
			}
		}
		#endif
		if(getVidReadFinish()==0&&getMsgQWaiting(OS_TASK_READ_FILE)<8)
		{
			while (temp++ <= PLAY_BUFF_NUM)
				sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
		}
		//if(jrAppProtocol()) // modify for lingtong app
		//if(1)
		if(src->sendPicCnt>=10)
		{
			while(src->playTime>=src->audTime)
			{
				if(queueOutput(&src->audQ, &f) == 0)
				{
					src->audTime=f->timestamp;
					f->timestamp=f->presentationTime;
					//printf("send audio:%d\n",f->dataLen);
					#if SEND_IMAGE_FIRST
					if(src->sendPicCnt>=10)
						stream_dest_inturn (src->audioStream, f);
					else
						freeFrame(f);
					#else
					if(src->sendPicCnt<20)
						src->sendPicCnt++;
					stream_dest_inturn (src->audioStream, f);
					#endif
				}
				else
				{
					break;
				}
			}
			temp = queueValidData(&src->audQ);
			if(getMsgQWaiting(OS_TASK_READ_FILE)<8)
			{
				while (temp++ <= PLAY_BUFF_NUM)
					sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
static int wakeup_playNet(struct frame *f, void *d) // process frame
{
	struct stream_dest *streamDest = (struct stream_dest *)d;
	struct sssPlayNet *src = (struct sssPlayNet *)streamDest->self;
	if (f->mediaType == MEDIA_VIDEO)
	{
		if (queueInput(&src->vidQ, f) < 0)
		{
			freeFrame(f);
		}
	}
	else
	{
		//if(jrAppProtocol())
		if(1)
		{
			if (queueInput(&src->audQ, f) < 0) // modify for lingtong app
				freeFrame(f);
		}
		else
		{
			freeFrame(f);
		}
	}
	return 0;
}
static void playNet_set_status (struct stream *s, S_INPUT_STATUS  status)
{
	struct sssPlayNet* src = (struct sssPlayNet*)s->self;
	struct stream_dest* streamDest;
	if(status==S_INPUT_PAUSE&&src->running)
	{
		printf("play net stop!!!\n\n");
		playPauseSet(1);
		src->running=0;
		if (src->imageSyncTimer)
			irqTimerDestory(src->imageSyncTimer);
		src->imageSyncTimer = NULL;
		src->shutDownCnt=0;
		queueFreeFrames(&src->vidQ);
		queueFreeFrames(&src->audQ);
	}
	else if(status==S_INPUT_RUNNING&&src->running==0)
	{
		printf("play net start!!!\n\n");
		if(queueValidData(&src->vidQ))
			printf("video queue err:%d\n\n\n",queueValidData(&src->vidQ));
		if(queueValidData(&src->audQ))
			printf("audio queue err:%d\n\n\n",queueValidData(&src->audQ));
		queueFreeFrames(&src->vidQ);
		queueFreeFrames(&src->audQ);
		playPauseSet(0);
		src->sendPicCnt=0;
		src->shutDownCnt=0;
		src->playTime = 0;
		src->running=1;
		src->audTime = 0;
		src->imageTime = 0;
		src->lastSendImageTime=0;
		src->firstFrame=1;
		if (src->imageSyncTimer)
			irqTimerDestory(src->imageSyncTimer);
		src->imageSyncTimer = irqTimerCreate(PLAY_INTERVAL, playNetSync, src);
		sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_IMAGE, 0);
		sendMsgNonblock(OS_TASK_READ_FILE, TASK_MSG_READ_AUDIO, 0);
	}
}
static struct stream_self_ops playNetSelf_ops = {
	.set_status = playNet_set_status,
	.lock = arch_local_irq_save,
	.unlock = arch_local_irq_restore,
};
static struct stream_src_ops playNet_src_ops = {
  .bufsize = (frame_size_func)frame_bufsize,
  .free = NULL,  
  .lock = arch_local_irq_save,
  .unlock = arch_local_irq_restore,
};

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void *start_block(void)
{
	struct sssPlayNet *xchg = (struct sssPlayNet *)osal_malloc(sizeof(struct sssPlayNet));
	if (xchg == NULL)
	{
		p_err("rtp-sender start_block malloc error");
		return NULL;
	}
	xchg->audTime = 0;
	xchg->imageTime = 0;
	xchg->lastSendImageTime=0;
	xchg->playTime = 0;
	xchg->running = 0;
	xchg->audioStream=NULL;
	xchg->imageSyncTimer = NULL;
	queueInit(&xchg->vidQ, 30);
	queueInit(&xchg->audQ, 30);
	stream_src_self_init(&xchg->self, &playNetSelf_ops, NULL); // NULL heap
	return xchg;
}

// todo: 線程優先級定義
static int end_block(void *d)
{

	return 0;
}
static int set_output(int num_tokens, struct token* tokens, void* d)
{
  struct sssPlayNet* xchg = (struct sssPlayNet*)d;
  struct stream_src *output;

  output = stream_src_new(tokens[1].v.str, d);
  if(!output)
    return -1;
  output->s.get_framerate = NULL;
  output->ops = &playNet_src_ops;
  xchg->self.output = output;

  output = stream_src_new(tokens[2].v.str, d);
  if(!output)
    return -1;
  output->s.get_framerate = NULL;
  output->ops = &playNet_src_ops;
  xchg->audioStream=output;
  return 0;
}
static int set_input(int num_tokens, struct token *tokens, void *d)
{
	struct sssPlayNet *xchg = (struct sssPlayNet *)d;
	struct stream_dest *input;

	if (!(input = stream_connect(tokens[1].v.str, (frame_deliver_func)wakeup_playNet, d, 0)))
	{
		p_err("record file: unable to connect to stream \"%s\"", tokens[1].v.str);
		return -1;
	}

	stream_add_input(&xchg->self, input);
	// note : 在output前進行的set_input，diffuse無效

	return 0;
}
static int set_media(int num_tokens, struct token *tokens, void *d)
{
	struct sssPlayNet *src = (struct sssPlayNet *)d;
	struct stream_src *output = src->self.output;
	if (NULL == output)
	{
		p_err("sssPlayNet: output must be specified before media");
		return -1;
	}
	output->s.media = media_get_ops(tokens[1].v.str);
	if (output->s.media)
		stream_diffuse_media(&(output->s), output->s.media);
	else
		return -1;
	output=src->audioStream;
	if (NULL == output)
		return -1;
	output->s.media = media_get_ops(tokens[2].v.str);
	if (output->s.media)
		stream_diffuse_media(&(output->s), output->s.media);
	else
		return -1;
	return NULL == output->s.media;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
	/* directive name, process function, min args, max args, arg types */
	{"input", set_input, 1, 1, {TOKEN_STR}},
	{"output", set_output, 2, 2, { TOKEN_STR,TOKEN_STR } },
	{"media", set_media, 2, 2, {TOKEN_STR,TOKEN_STR}},
	/* empty terminator -- do not remove */
	{NULL, NULL, 0, 0, {}}};

int paly_net_register(void)
{
	register_config_context("play", "net", start_block, end_block, config_statements);
	return 0;
}



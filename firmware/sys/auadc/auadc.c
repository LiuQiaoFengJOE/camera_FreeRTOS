#include "auadc.h"
#include "stream/stream.h"

#define AUDIO_SAMPLE  ADC_SAMPLEREATE_16000
struct auadc_src
{
	struct stream_src_self self; // with a chunklist heap
	struct frame *curFrame;
	struct frame *nextFrame;
	u32 timeStamp;
	u8 simGain;
	u8 digGain;
	wait_queue_head_t wq;
	//EVT_HDL ev;
};

static void *auadcPCMThread(void *d)
{
	struct auadc_src *src = (struct auadc_src *)d;
	struct frame *f;
	uint32_t cnt = 0;
	while (1)
	{
		xQueueGenericReceive(&src->wq, NULL, -1, 0);
		cnt = 0;
	again:
		f = stream_next_frame(src->self.output);
		if (f)
		{
			// printf("auadc deal frame:%d\n",f->id);
			stream_dest_inturn(&src->self.output->s, f);
			cnt++;
			if (cnt < 5)
				goto again;
		}
	}
}

static int auadc_frame_half(uint32_t irq, void *d, void *reg)
{
	static u32 lastTime;
	struct auadc_src *src = (struct auadc_src *)d;
	u16 buf_size = src->self.output->s.buf_size;
	if (src->nextFrame)
	{
		printf("maybe some event excute long time!!!\n"); // 可能因为某个地方执行时间过长,导致半中断重复进来,这样可能会导致音频帧会少或者乱
		return 0;
	}
	src->nextFrame = streamNewFrame(src->self.output);
	if (NULL == src->nextFrame)
	{
		ax32xx_auadcBufferSet(src->curFrame->buf.dat + 8, buf_size - 8);
		if(os_time_get()-lastTime>5000)
		{
			lastTime=os_time_get();
			printf("aud no f\n");
		}
		return -1;
	}
	else
	{
		ax32xx_auadcBufferSet(src->nextFrame->buf.dat + 8, buf_size - 8);
	}
	return 0;
}

static u32 audioPresentationTime;
static u32 audioSampleLen;
void audioPresentationTimeReset(void)
{
	audioPresentationTime=0;
	audioSampleLen=0;
}
u32 audioPresentationTimeGet(void)
{
	return audioPresentationTime;
}
// 这里需要处理的是一个framebuff填充完了之后需要做的事情，
// 这里只需要将在auadc_half中断中申请到的frame进行分发就行了
static int auadc_frame_done(uint32_t irq, void *d, void *reg)
{
	u32 *p32;
	struct auadc_src *src = (struct auadc_src *)d;
	u16 buf_size = src->self.output->s.buf_size;

	if (NULL == src->curFrame)
	{
		printf("auadc curFrame is NULL!!!\n");
		src->timeStamp += ((buf_size - 8) >> 1);
		return -1;
	}
	if (NULL == src->nextFrame)
	{
		//printf("nextFrame is NULL,drop curFrame!!!\n");
		src->timeStamp += ((buf_size - 8) >> 1);
		stream_dest_inturn(&src->self.output->s,NULL);
		return -1;
	}
	src->timeStamp += ((buf_size - 8) >> 1);
	audioSampleLen+=((buf_size - 8) >> 1);
	audioPresentationTime=audioSampleLen/(AUDIO_SAMPLE/1000);
	src->curFrame->timestamp = src->timeStamp;
	//printf("audio timestamp:%d\n",src->timeStamp);
	//presentationTime = src->curFrame->presentationTime = src->timeStamp / 16;
	src->curFrame->mediaType = MEDIA_AUDIO;
	p32 = (u32 *)src->curFrame->buf.dat;
	p32[0] = 0x62773130; // 这里可以写进一些标志
	p32[1] = buf_size - 8;
	src->curFrame->dataLen = buf_size;
	src->curFrame->headLen=8;
	src->curFrame->presentationTime=os_time_get();
	frameAddFullListTail(src->curFrame);
	stream_dest_inturn(&src->self.output->s, src->curFrame);
	src->curFrame = src->nextFrame;
	src->nextFrame = NULL;
}

//extern void auadc_register_half(isr_func isr, void *dev_id);
//extern void auadc_register_end(isr_func isr, void *dev_id);

static void auadc_set_status(struct stream *s, S_INPUT_STATUS status)
{
	struct auadc_src *src = s->self; // todo more things
	u16 buf_size = src->self.output->s.buf_size;
	if (status == S_INPUT_RUNNING)
	{
		//streamPrintFreeFrame(src->self.output);
		//streamPrintFullFrame(src->self.output);
		audioPresentationTimeReset();
		auadc_register_half(auadc_frame_half, src);
		auadc_register_end(auadc_frame_done, src);
		if (src->curFrame == NULL)
			src->curFrame = streamNewFrame(s);
		if (NULL == src->curFrame)
		{
			p_err("Cannot alloc frame when starting auadc");
			return;
		}
		if (src->nextFrame)
			freeFrame(src->nextFrame);
		src->nextFrame = NULL;
		src->timeStamp = 0;
		// ax32xx_auadcInit();
		ax32xx_auadcSetSampleSet(AUDIO_SAMPLE);
		ax32xx_auadcGainSet(0x05, 0x05, 0x05, 0x05); // 设置AGAIN的范围
		ax32xx_auadcAGCEnable(1);					 // 使能AGC
		ax32xx_auadcBufferSet((src->curFrame->buf.dat + 8), buf_size - 8);
		ax32xx_auadcEnable(1,src->simGain,src->digGain); // 使能；
		printf("auadc_set_running :0x%x\n", src->curFrame->buf.dat);
	}
	else if (status == S_INPUT_PAUSE)
	{
		printf("auadc_set stop\n");
		ax32xx_auadcEnable(0,src->simGain,src->digGain);
		streamFreeFrames((struct stream_src *)s);
		freeFrame(src->curFrame);
		src->curFrame = NULL;
		freeFrame(src->nextFrame);
		src->nextFrame = NULL;
		//streamPrintFreeFrame(src->self.output);
		//streamPrintFullFrame(src->self.output);
	}
}

static struct stream_self_ops pcm_ops =
	{
		.set_status = auadc_set_status,
		.collect = stream_self_collect_default,
		.lock = arch_local_irq_save,
		.unlock = arch_local_irq_restore};

///////////////////////////////////////////////////////////////////////
static struct stream_src_ops auadc_ops =
	{
		.bufsize = (frame_size_func)frame_bufsize,
		.free = (frame_free_func)NULL, // TODO : 似乎不能在frame釋放時調用frame_buffer_free
		.lock = arch_local_irq_save,
		.unlock = arch_local_irq_restore};

static int start_block(void)
{
	// printf("start_block nothing todo now\n");
	struct auadc_src *xchg = (struct auadc_src *)osal_malloc(sizeof(struct auadc_src));
	if (xchg == NULL)
	{
		p_err("auadc start_block malloc error");
		return NULL;
	}
	xchg->curFrame = NULL;
	xchg->nextFrame = NULL;
	xchg->timeStamp = 0;
	xchg->simGain = 3;
	xchg->digGain = 5;
	//xchg->ev = NULL;
	//init_waitqueue_head(&xchg->wq);
	stream_src_self_init(&xchg->self, &pcm_ops, NULL);
	ax32xx_auadcInit();
	return xchg;
}
static int end_block(void *d)
{
	struct auadc_src *xchg = (struct auadc_src *)d;
	if (!xchg->self.output)
	{
		p_err("auadc: output should be config before end_block");
		return -1;
	}
	xchg->self.output->frame_num = 15;////最多的缓存帧数
	xchg->self.output->s.buf_size = 6*1024+8;
	//osal_thread_create(auadcPCMThread, d, 10, 0, 8192, "auadcPCM");
	return 0;
}
// 这里要创建这个源流所输出的流
// auadc采集数据之后再中断函数中将数据送到buffer里面
static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct auadc_src *xchg = (struct auadc_src *)d;
	struct stream_src *output;
	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;
	output->ops = &auadc_ops;
	xchg->self.output = output;
	return 0;
}
static int set_media(int num_tokens, struct token *tokens, void *d)
{
	struct auadc_src *src = (struct auadc_src *)d;
	struct stream_src *output = src->self.output;
	if (NULL == output)
	{
		p_err("auadc_manu: output must be specified before media");
		return -1;
	}
	output->s.media = media_get_ops(tokens[1].v.str);
	if (output->s.media)
		stream_diffuse_media(&(output->s), output->s.media);
	return NULL == output->s.media;
}
static int set_framenum(int num_tokens, struct token *tokens, void *d)
{
	struct auadc_src *src = (struct auadc_src *)d;
	if (!src->self.output)
	{
		p_err("auadc: output should be config before framenum");
		return -1;
	}
	src->self.output->frame_num = tokens[1].v.num;   //最多的缓存帧数
	src->self.output->s.buf_size = tokens[2].v.num;
	src->self.output->buf_num = 0;
	src->self.output->lst_size = 0;
	return 0;
}
static int set_gain(int num_tokens, struct token *tokens, void *d)
{
	struct auadc_src *src = (struct auadc_src *)d;
	src->simGain = tokens[1].v.num;
	src->digGain = tokens[2].v.num;
	return 0;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
	/* directive name, process function, min args, max args, arg types */
	{"framenum", set_framenum, 1, 2, {TOKEN_NUM, TOKEN_NUM}}, // 多添加了一个参数，控制buffer的大小
	{"output", set_output, 1, 1, {TOKEN_STR}},
	{"media", set_media, 1, 1, {TOKEN_STR}},
	{"gain", set_gain, 2, 2, {TOKEN_NUM, TOKEN_NUM}},
	/* empty terminator -- do not remove */
	{NULL, NULL, 0, 0, {}}};

int pcm_auadc_register(void)
{
	register_config_context("Input", "auadc", start_block, end_block, config_statements);
	return 0;
}


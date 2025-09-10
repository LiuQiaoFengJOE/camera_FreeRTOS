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


struct csiScale0
{
	struct stream_src_self self; // with a chunklist heap
	IRQ_TIMER *csiScale0Timer;
	struct frame *curFrame;
	u32 lastTime;
	u32 srcW,srcH;
  	u32 scaleW,scaleH;
};
static void csiScale0Start(void *arg)
{
	struct csiScale0 *pCsiScale0 = (struct csiScale0 *)arg;
	if(pCsiScale0->curFrame!=NULL)
	{
		irqTimerModTime(pCsiScale0->csiScale0Timer, 20);
		return;
	}
	pCsiScale0->curFrame=streamNewFrame(pCsiScale0->self.output);
	if(pCsiScale0->curFrame==NULL)
	{
		irqTimerModTime(pCsiScale0->csiScale0Timer, 20);
		return;
	}
	u32 resw,resh;
	resw=resh=0;
	getWifiVidRes(&resw,&resh);
	if(pCsiScale0->scaleW!=resw||pCsiScale0->scaleH!=resh)
	{
		pCsiScale0->scaleW=resw;
		pCsiScale0->scaleH=resh;
		csi_jdma_scale_down(pCsiScale0->srcW,pCsiScale0->srcH,pCsiScale0->scaleW,pCsiScale0->scaleH);
	}
	ax32xx_csiMJPEGFrameSet(pCsiScale0->curFrame->buf.dat,pCsiScale0->curFrame->buf.dat+MALIG(pCsiScale0->scaleW,32)*pCsiScale0->scaleH,pCsiScale0->scaleH,MALIG(pCsiScale0->scaleW,32));
	csi_jdma_start();
	irqTimerModTime(pCsiScale0->csiScale0Timer, getFrameInterval());
}
static int csiScale0FrameEnd(unsigned int irqno, void *d, void *regs)
{
	struct csiScale0 *src = (struct csiScale0 *)d;
	if(src->curFrame==NULL)
	{
		printf("%s err happen\n",__FUNCTION__);
		return 0;
	}
	csi_jdma_stop();
	src->curFrame->imageInfor.width=src->scaleW;
	src->curFrame->imageInfor.hight=src->scaleH;
	stream_dest_inturn(&src->self.output->s, src->curFrame);
	src->curFrame=NULL;
	return 1;
}
static void csiScale0SetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct csiScale0 *pCsiScale0 = s->self;
  printf("%s %s run:%d\r\n", __FUNCTION__, pCsiScale0->self.output->s.name, status);
  if(status==S_INPUT_RUNNING)
  {
  	u32 resw,resh;
	resw=resh=0;
	sensorGetRes(&resw,&resh);
	pCsiScale0->srcW=resw;
	pCsiScale0->srcH=resh;
	resw=resh=0;
	getWifiVidRes(&resw,&resh);
	pCsiScale0->scaleW=resw;
	pCsiScale0->scaleH=resh;
	freeFrame(pCsiScale0->curFrame);
	pCsiScale0->curFrame=streamNewFrame(pCsiScale0->self.output);
	if(pCsiScale0->curFrame==NULL)
	{
		printf("% err,no frame\n",__FUNCTION__);
		return;
	}
	printf("scale0[%d,%d]->[%d,%d]\n",pCsiScale0->srcW,pCsiScale0->srcH,pCsiScale0->scaleW,pCsiScale0->scaleH);
	csi_jdma_scale_down(pCsiScale0->srcW,pCsiScale0->srcH,pCsiScale0->scaleW,pCsiScale0->scaleH);
	dcache_flush_region((u32)pCsiScale0->curFrame->buf.dat,(MALIG(pCsiScale0->scaleW,32)*pCsiScale0->scaleH*3)/2);
	ax32xx_csiMJPEGFrameSet(pCsiScale0->curFrame->buf.dat,pCsiScale0->curFrame->buf.dat+MALIG(pCsiScale0->scaleW,32)*pCsiScale0->scaleH,pCsiScale0->scaleH,MALIG(pCsiScale0->scaleW,32));
	csiirq_register_jdma_frame_end(csiScale0FrameEnd,pCsiScale0);
	csi_jdma_start();
	pCsiScale0->lastTime=os_time_get();
	if(pCsiScale0->csiScale0Timer)
		printf("% err,csiScale0Timer not null\n",__FUNCTION__);
	pCsiScale0->csiScale0Timer = irqTimerCreate(getFrameInterval(), csiScale0Start, pCsiScale0);
  }
  else if(status==S_INPUT_PAUSE)
  {
  	if (pCsiScale0->csiScale0Timer)
		irqTimerDestory(pCsiScale0->csiScale0Timer);
	pCsiScale0->csiScale0Timer = NULL;
  	csiirq_unregister_jdma_frame_end();
	csi_jdma_stop();
	freeFrame(pCsiScale0->curFrame);
	pCsiScale0->curFrame=NULL;
  }
}

static void csiScale0Config(void *d)
{
	struct csiScale0 *pCsiScale0 = (struct csiScale0 *)d;
	sensorGetRes(&pCsiScale0->srcW,&pCsiScale0->srcH);
	pCsiScale0->scaleW=pCsiScale0->srcW;
	pCsiScale0->scaleH=pCsiScale0->srcH;
	pCsiScale0->self.output->buf_num = 0;
	pCsiScale0->self.output->lst_size = 0;
	pCsiScale0->self.output->frame_num = 1;
	pCsiScale0->self.output->s.buf_size = (MALIG(pCsiScale0->srcW,32)*pCsiScale0->srcH*3)/2;
	printf("senW:%d,senH:%d ,size:%dKB\n", pCsiScale0->srcW,pCsiScale0->srcH,pCsiScale0->self.output->s.buf_size>>10);
}

static struct stream_src_ops csiScale0_src_ops = {
    .bufsize = (frame_size_func)frame_bufsize,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};
static struct stream_self_ops csiScale0_self_ops = {
    .getinfo = NULL,
    .set_status = csiScale0SetStatus,
    .config = csiScale0Config,
    .collect = stream_self_collect_default,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};

static void *start_block(void)
{
	struct csiScale0 *src = (struct csiScale0 *)osal_malloc(sizeof(struct csiScale0));
	if (src == NULL)
	{
		p_err_mem;
		return NULL;
	}
	stream_src_self_init(&src->self, &csiScale0_self_ops, NULL);
	return src;
}

static int end_block(void *d)
{
	struct csiScale0 *src = (struct csiScale0 *)d;
	src->curFrame = NULL;
	src->csiScale0Timer=NULL;
	//csi_setup(); // todo
	return 0;
}

static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct csiScale0 *src = (struct csiScale0 *)d;
	struct stream_src *output;
	if (src->self.output)
	{
		p_err("vid-csi config twice");
		return -1;
	}
	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;
	output->s.get_framerate = NULL;
	output->ops = &csiScale0_src_ops;
	src->self.output = output;
	return 0;
}

static int set_framenum(int num_tokens, struct token *tokens, void *d)
{
	struct csiScale0 *src = (struct csiScale0 *)d;
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

int vid_csi0_register(void)
{
	register_config_context("yuv", "csi0", start_block, end_block, config_statements);
	return 0;
}


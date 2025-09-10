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


struct csiScale1
{
	struct stream_src_self self; // with a chunklist heap
	IRQ_TIMER *csiScale1Timer;
	struct frame *curFrame;
	u32 lastTime;
	u32 srcW,srcH;
  	u32 scaleW,scaleH;
};
static void csiScale1Start(void *arg)
{
	struct csiScale1 *pCsiScale1 = (struct csiScale1 *)arg;
	if(pCsiScale1->curFrame!=NULL)
	{
		irqTimerModTime(pCsiScale1->csiScale1Timer, 20);
		return;
	}
	pCsiScale1->curFrame=streamNewFrame(pCsiScale1->self.output);
	if(pCsiScale1->curFrame==NULL)
	{
		irqTimerModTime(pCsiScale1->csiScale1Timer, 20);
		return;
	}
	csi_j1dma_restart();
	irqTimerModTime(pCsiScale1->csiScale1Timer, getLocalFrameInterval());
}
static int csiScale1FrameEnd(unsigned int irqno, void *d, void *regs)
{
	struct csiScale1 *pCsiScale1 = (struct csiScale1 *)d;
	u32 resw,resh;
	if(pCsiScale1->curFrame==NULL)
	{
		printf("%s err happen\n",__FUNCTION__);
		return 0;
	}
	csi_j1dma_stop();
	pCsiScale1->curFrame->imageInfor.width=pCsiScale1->scaleW;
	pCsiScale1->curFrame->imageInfor.hight=pCsiScale1->scaleH;
	stream_dest_inturn(&pCsiScale1->self.output->s, pCsiScale1->curFrame);
	pCsiScale1->curFrame=NULL;
	#if 0
	pCsiScale1->curFrame=streamNewFrame(pCsiScale1->self.output);
	if(pCsiScale1->curFrame==NULL)
	{
		printf("% err,no frame\n",__FUNCTION__);
		return 0;
	}
	#endif
	return 0;
}
static void csiScale1SetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct csiScale1 *pCsiScale1 = s->self;
  printf("%s %s run:%d\r\n", __FUNCTION__, pCsiScale1->self.output->s.name, status);
  if(status==S_INPUT_RUNNING)
  {
  	u32 resw,resh;
	resw=resh=0;
	sensorGetRes(&resw,&resh);
	pCsiScale1->srcW=resw;
	pCsiScale1->srcH=resh;
	resw=getLocalResW();
	resh=getLocalResH();
	if(resw>pCsiScale1->srcW)
		resw=pCsiScale1->srcW;
	if(resh>pCsiScale1->srcH)
		resh=pCsiScale1->srcH;
	pCsiScale1->scaleW=resw;
	pCsiScale1->scaleH=resh;
	freeFrame(pCsiScale1->curFrame);
	pCsiScale1->curFrame=streamNewFrame(pCsiScale1->self.output);
	if(pCsiScale1->curFrame==NULL)
	{
		printf("% err,no frame\n",__FUNCTION__);
		return;
	}
	printf("scale1[%d,%d]->[%d,%d]\n",pCsiScale1->srcW,pCsiScale1->srcH,pCsiScale1->scaleW,pCsiScale1->scaleH);	
	csi_j1dma_scale_down(pCsiScale1->srcW,pCsiScale1->srcH,pCsiScale1->scaleW,pCsiScale1->scaleH);
	dcache_flush_region((u32)pCsiScale1->curFrame->buf.dat,(MALIG(pCsiScale1->scaleW,32)*pCsiScale1->scaleH*3)/2);
	ax32xx_csihv_tomjp_dma_cfg(pCsiScale1->curFrame->buf.dat,pCsiScale1->curFrame->buf.dat+MALIG(pCsiScale1->scaleW,32)*pCsiScale1->scaleH,pCsiScale1->scaleH,MALIG(pCsiScale1->scaleW,32));
	csiirq_register_j1dma_frame_end(csiScale1FrameEnd,pCsiScale1);
	csi_j1dma_restart();
	pCsiScale1->lastTime=os_time_get();
	if(pCsiScale1->csiScale1Timer)
		printf("% err,csiScale1Timer not null\n",__FUNCTION__);
	pCsiScale1->csiScale1Timer = irqTimerCreate(getLocalFrameInterval(), csiScale1Start, pCsiScale1);
  }
  else if(status==S_INPUT_PAUSE)
  {
  	if (pCsiScale1->csiScale1Timer)
		irqTimerDestory(pCsiScale1->csiScale1Timer);
	pCsiScale1->csiScale1Timer = NULL;
  	csiirq_unregister_j1dma_frame_end();
	csi_j1dma_stop();
	freeFrame(pCsiScale1->curFrame);
	pCsiScale1->curFrame=NULL;
  }
}

static void csiScale1Config(void *d)
{
	struct csiScale1 *pCsiScale1 = (struct csiScale1 *)d;
	sensorGetRes(&pCsiScale1->srcW,&pCsiScale1->srcH);
	pCsiScale1->scaleW=pCsiScale1->srcW;
	pCsiScale1->scaleH=pCsiScale1->srcH;
	pCsiScale1->self.output->buf_num = 0;
	pCsiScale1->self.output->lst_size = 0;
	pCsiScale1->self.output->frame_num = 1;
	pCsiScale1->self.output->s.buf_size = (MALIG(pCsiScale1->srcW,32)*pCsiScale1->srcH*3)/2;
	printf("senW:%d,senH:%d ,size:%dKB\n", pCsiScale1->srcW,pCsiScale1->srcH,pCsiScale1->self.output->s.buf_size>>10);
}

static struct stream_src_ops csiScale1_src_ops = {
    .bufsize = (frame_size_func)frame_bufsize,
	.set_status = csiScale1SetStatus,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};
static struct stream_self_ops csiScale1_self_ops = {
    .getinfo = NULL,
    .set_status = csiScale1SetStatus,
    .config = csiScale1Config,
    .collect = stream_self_collect_default,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};

static void *start_block(void)
{
	struct csiScale1 *src = (struct csiScale1 *)osal_malloc(sizeof(struct csiScale1));
	if (src == NULL)
	{
		p_err_mem;
		return NULL;
	}
	stream_src_self_init(&src->self, &csiScale1_self_ops, NULL);
	return src;
}

static int end_block(void *d)
{
	struct csiScale1 *src = (struct csiScale1 *)d;
	src->curFrame = NULL;
	src->csiScale1Timer=NULL;
	//csi_setup(); // todo
	return 0;
}

static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct csiScale1 *src = (struct csiScale1 *)d;
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
	output->ops = &csiScale1_src_ops;
	src->self.output = output;
	return 0;
}

static int set_framenum(int num_tokens, struct token *tokens, void *d)
{
	struct csiScale1 *src = (struct csiScale1 *)d;
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

int vid_csi1_register(void)
{
	register_config_context("yuv", "csi1", start_block, end_block, config_statements);
	return 0;
}


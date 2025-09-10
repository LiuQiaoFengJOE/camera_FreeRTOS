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
#include "task/app.h"


struct picCmb
{
	struct stream_src_self self; // with a chunklist heap
	struct frame *curFrame;
};
extern void jpgDecStop(int callDone);
static void picCmbStart(void *arg);
static void picCombineStart(struct frame *input, void *d);

struct picCmb *ppicCmb;

struct frame *getUsbJpgFrame(void)
{
	return streamDestGetFrame(stream_self_get_input(&ppicCmb->self, 0));
}
struct frame *getUsb2JpgFrame(void)
{
	return streamDestGetFrame(stream_self_get_input(&ppicCmb->self, 1));
}

u32 picComUsbQueueValidData(void)
{
	return queueValidData(&(stream_self_get_input(&ppicCmb->self, 0)->mq));
}
	
void picComUsbManualInput(struct frame *input)
{	
	struct stream_dest *dest = stream_self_get_input(&ppicCmb->self, 0);
	if(queueFull(&dest->mq)||!getUsbCameraConnect())
	{
		freeFrame(input);
		return;
	}
	streamDestAddFrame(dest, input);
}

u32 picComUsb2QueueValidData(void)
{
	return queueValidData(&(stream_self_get_input(&ppicCmb->self, 1)->mq));
}
	
void picComUsb2ManualInput(struct frame *input)
{
	struct stream_dest *dest = stream_self_get_input(&ppicCmb->self, 1);
	if(queueFull(&dest->mq)||!getUsb2CameraConnect())
	{
		freeFrame(input);
		return;
	}
	streamDestAddFrame(dest, input);	
}

void picComDeal(disp_frame_t * ready)
{
	if(ready==NULL)
		return;
	ppicCmb->curFrame=streamNewFrame(ppicCmb->self.output);
	if(ppicCmb->curFrame==NULL)
	{
		printf("pic combine malloc frame fail\n");
		return;
	}
	u16 w,h;
	lcdGetVideoRes(&w,&h);
	ppicCmb->curFrame->imageInfor.width=w;
	ppicCmb->curFrame->imageInfor.hight=h;
	ppicCmb->curFrame->buf.dat=ready->y_addr;
	stream_dest_inturn(&ppicCmb->self.output->s, ppicCmb->curFrame);
	ppicCmb->curFrame=NULL;
}

static void picCmbSetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct picCmb *pPicCmbine = s->self;
  printf("%s %s run:%d\r\n", __FUNCTION__, pPicCmbine->self.output->s.name, status);
  if(status==S_INPUT_RUNNING)
  {
  	
  }
  else if(status==S_INPUT_PAUSE)
  {
  	
  }
}
static void picCmbStreamSetStatus(struct stream *s, S_INPUT_STATUS status)
{
  struct picCmb *pPicCmbine = s->self;
  if(status==S_INPUT_RUNNING)
  {
	
  }
  else if(status==S_INPUT_PAUSE)
  {
  	
  }
}
static void picCmbConfig(void *d)
{
	struct picCmb *ppicCmb = (struct picCmb *)d;
	ppicCmb->self.output->frame_num = 1;
}
static void picCombineStart(struct frame *input, void *d)
{
	struct stream_dest *dest = (struct stream_dest *)d;
	struct picCmb *pPicCmbine = (struct picCmb*)dest->self;
	if(queueFull(&dest->mq))
	{
		freeFrame(input);
		return;
	}
	streamDestAddFrame(dest, input);
}
static struct stream_src_ops picCmb_src_ops = {
    .bufsize = (frame_size_func)frame_bufsize,
	.set_status = picCmbStreamSetStatus,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};
static struct stream_self_ops picCmb_self_ops = {
    .getinfo = NULL,
    .set_status = picCmbSetStatus,
    .config = picCmbConfig,
    .collect = stream_self_collect_default,
    .lock = arch_local_irq_save,
    .unlock = arch_local_irq_restore,
};

static void *start_block(void)
{
	struct picCmb *src = (struct picCmb *)osal_malloc(sizeof(struct picCmb));
	if (src == NULL)
	{
		p_err_mem;
		return NULL;
	}
	src->curFrame=NULL;
	ppicCmb=src;
	stream_src_self_init(&src->self, &picCmb_self_ops, NULL);
	return src;
}

static int end_block(void *d)
{
	struct picCmb *src = (struct picCmb *)d;
	struct stream_dest *input = stream_dest_new(picCombineStart, d,3);
  	stream_add_input(&src->self, input);
	input = stream_dest_new(picCombineStart, d,3);
  	stream_add_input(&src->self, input);
	return 0;
}

static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct picCmb *src = (struct picCmb *)d;
	struct stream_src *output;
	if (src->self.output)
	{
		p_err("vid-csi config twice");
		return -1;
	}
	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;
	output->s.takeOver=1;
	output->s.get_framerate = NULL;
	output->ops = &picCmb_src_ops;
	src->self.output = output;
	return 0;
}

static int set_framenum(int num_tokens, struct token *tokens, void *d)
{
	struct picCmb *src = (struct picCmb *)d;
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

int pic_combine_register(void)
{
	register_config_context("pic", "combine", start_block, end_block, config_statements);
	return 0;
}


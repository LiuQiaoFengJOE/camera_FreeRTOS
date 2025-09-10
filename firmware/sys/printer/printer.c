#include "stream/stream.h"

#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "debug.h"
#include "drv/lcd/lcdframe.h"
#include "task/app.h"

struct stream_printer
{ 
  struct stream_src_self self;
};
extern u8 jpgFramePrinter(struct frame* f);
void picPrinterThread(void* d)
{
	u32 msgType,msgData;
	OS_WAIT_DONE_MSG *msg;
	struct frame *f;
	struct stream_printer* src = (struct jpgencAuto*)d;
	struct stream_dest* streamDest=stream_self_get_input(&src->self, 0);
	while(1)
	{
		if(recMsg(OS_TASK_PRINTER,&msgType,&msgData)<=0)
		{
			printf("OS_TASK_PRINTER recv msg error!!!!!!\n\n");
			continue;
		}
		switch(msgType)
		{
		case TASK_MSG_DATA_INPUT:
			f = streamDestGetFrame(streamDest);
			if(f)
			{
				streamSetPause(streamDest);
				streamDestFreeFrames(streamDest,0);
				jpgFramePrinter(f);
			}
			break;
		case TASK_MSG_WAIT_DONE:
			msg=(OS_WAIT_DONE_MSG *)msgData;
			switch(msg->msgID)
			{
				case TASK_MSG_PRINTER_JPG:
					printer_init();
					streamSetRuning(streamDest);
				break;
				case TASK_MSG_UNINIT:
					streamSetPause(streamDest);
					streamDestFreeFrames(streamDest,0);
				break;
			}
			msg->done(msg);
			break;
		default:
			printf("pic printer,unknow msg:%d\n",msgType);
			break;
		}
	}	
}

static int wakeup_packer (struct frame *input, void *d)  // process frame
{
	struct stream_dest* streamDest=(struct stream_dest*)d;
	streamDestAddFrame(streamDest, input);
	sendMsgNonblock(OS_TASK_PRINTER,TASK_MSG_DATA_INPUT, 0);
	return 0;
}

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void *start_block(void)
{
  struct stream_printer *xchg = (struct stream_printer *)osal_malloc(sizeof(struct stream_printer));
  if (xchg == NULL)
  {
    p_err("picPrinter start_block malloc error");
    return NULL;
  }

  stream_src_self_init(&xchg->self, NULL, NULL);

  return xchg;
}

static int end_block(void *d)
{
  printf("picPrinter end_block!\r\n");

  struct stream_printer *xchg = (struct stream_printer *)d;
  struct stream_dest *input;
  // 祇需要一個input
  //input = stream_dest_new(wakeup_packer, d,5);
  //stream_add_input(&xchg->self, input);
  createdTaskByFunc(OS_TASK_PRINTER, picPrinterThread,d,29,1024,"taskPicPrinter",20);
  return 0;
}
static int set_input(int num_tokens, struct token* tokens, void* d)
{
  struct stream_printer* xchg = (struct stream_printer*)d;
  struct stream_dest *input;
  if(!(input = stream_connect(tokens[1].v.str, (frame_deliver_func)wakeup_packer, d,5))) {
    p_err("rtp-sender: unable to connect to stream \"%s\"", tokens[1].v.str);
    return -1;
  }
  stream_add_input(&xchg->self, input);
  return 0;
}
static int set_output(int num_tokens, struct token *tokens, void *d)
{

  struct stream_printer *xchg = (struct stream_printer *)d;
  struct stream_src *output;
  if (xchg->self.output)
  {
    p_err("picPrinter config twice");
    return -1;
  }

  output = stream_src_new(tokens[1].v.str, d);
  if (!output)
    return -1;

  xchg->self.output = output;

  return 0;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
    /* directive name, process function, min args, max args, arg types */
	{ "input", set_input, 1, 1, { TOKEN_STR } },
    {"output", set_output, 1, 1, {TOKEN_STR}},
    /* empty terminator -- do not remove */
    {NULL, NULL, 0, 0, {}}};

int pic_printer_register(void)
{
  register_config_context("pic", "printer", start_block, end_block, config_statements);
  return 0;
}


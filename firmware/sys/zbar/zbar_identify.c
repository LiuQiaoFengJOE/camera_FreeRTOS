#include "stream/stream.h"

#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "debug.h"
#include "drv/lcd/lcdframe.h"
#include "task/app.h"


#include <stdio.h>
#include "./zbar/inc/zbar.h"

#include "fatfs/ff.h"


typedef enum{
	ZBAR_OPT_WAIT = 0,
	ZBAR_OPT_DATA_PREPARE,
	ZBAR_OPT_DATA_READY,
	ZBAR_OPT_IDENTIFY_START,
	ZBAR_OPT_IDENTIFY_END,
	ZBAR_OPT_RESULT_PROCESS,
}E_ZBAR_OPT_STATUS;


typedef struct _zbar_identify_context
{ 
	uint8_t enable;

	struct stream_src_self self;

	//sensor 属性
	uint32_t src_width;
	uint32_t src_hight;
	uint8_t* buffer;
	uint32_t buffer_len;
	uint32_t data_len;
	uint32_t zbar_result_str[256];

	uint32_t identify_width;
	uint32_t identify_hight;
	uint32_t identify_buffer;
	uint32_t identify_buffer_len;

	volatile uint8_t setrun;
	volatile uint8_t runing;
	volatile uint8_t set_identify_run;
	volatile uint8_t opt_status;
}zbar_identify_context;

zbar_identify_context g_context={
.enable = 0,
.setrun = 0,
.runing = 0,
.set_identify_run = 0,
.opt_status = ZBAR_OPT_WAIT,
.buffer = NULL,
};


//fp = osal_fopen(name, "rb");
void save_frame_to_file(char*path,uint8_t*buffer,int buffer_len)
{
	FIL fp;

	printf("save_frame_to_file file:%s,len:%d\n",path,buffer_len);
	int ret = f_open(&fp,path, FA_WRITE | FA_CREATE_ALWAYS);
	 if(FR_OK != ret){
	 	printf("save file fail,ret:%d\n",ret);
		return;
	 }
	f_lseek(&fp,0);
	f_write(&fp,buffer,1,buffer_len);

	f_close(&fp);
}
int qrcode_identify(void*d)
{
	zbar_identify_context* context = (zbar_identify_context*)d;

	// 初始化扫描器和图像对象
	zbar_image_scanner_t *scanner = zbar_image_scanner_create();

	// 配置识别类型（启用全部符号）
	zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);	// 开启所有类型识别:ml-citation{ref="8" data="citationList"}

	zbar_image_t *image = zbar_image_create();

	//读取图像数据（需预处理为灰度格式 Y800）
	unsigned char *data = context->buffer;
	
	// 设置图像参数（假设已知宽高）
	zbar_image_set_format(image, *(unsigned int*)"Y800"); 
	zbar_image_set_size(image,context->identify_width, context->identify_hight);
	zbar_image_set_data(image, data, context->data_len, NULL);
	ax32xx_wdtClear();

	// 执行扫描
	int ret = zbar_scan_image(scanner, image);
	if(ret <= 0) {
		printf("qrcode identify none\n");
	}else{
		printf("qrcode identify success\n");
		// 遍历识别结果
		const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
		for(; symbol; symbol = zbar_symbol_next(symbol)) {
			const char *dec_text = zbar_symbol_get_data(symbol);
			if(dec_text != NULL){
				strcpy(context->zbar_result_str,dec_text);
				printf("identify text:%s\n", context->zbar_result_str);
				
				sendMsgNonblock(OS_TASK_UI,SYS_ZBAR_IDENTIFY_RESUALT,context->zbar_result_str);
			}
		}
	}

	zbar_image_destroy(image);
	zbar_image_scanner_destroy(scanner);

	return 0;
}

static int zbar_indentify(void *d) // process frame
{
	zbar_identify_context* context = (zbar_identify_context*)d;
	static char path[128];
	static int idx = 0;
	memset(path,0,128);

	int res = 0;
	if(ZBAR_OPT_DATA_READY == context->opt_status){
		printf("opt status:ZBAR_OPT_DATA_READY\n");
		
		context->opt_status = ZBAR_OPT_IDENTIFY_START;
		printf("opt status:ZBAR_OPT_IDENTIFY_START\n");

		res = qrcode_identify(d);
		
		context->opt_status = ZBAR_OPT_IDENTIFY_END;
		printf("opt status:ZBAR_OPT_IDENTIFY_END\n");
		
		context->opt_status = ZBAR_OPT_RESULT_PROCESS;
		printf("opt status:ZBAR_OPT_RESULT_PROCESS\n");
		
		sprintf(path,"1:zbar_test_%d.yuv",idx);
		printf("save yuv file:%s\n",path);
		save_frame_to_file(path,context->buffer,context->data_len);
		
	}				
	idx++;
	context->opt_status = ZBAR_OPT_WAIT;
	printf("opt status:ZBAR_OPT_WAIT\n");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
static int zbar_yuv_recv(void *d) // process frame
{
	zbar_identify_context* context = (zbar_identify_context*)d;
	csi_j1dma_stop();
	printf("==== zbar_yuv_recv buffer_len:%u====\r\n",context->buffer_len);
	context->data_len = context->identify_width*context->identify_hight;

	context->opt_status = ZBAR_OPT_DATA_READY;
	sendMsgNonblock(OS_TASK_ZBAR_IDENTIFY,TASK_MSG_ZBAR_IDENTIFY, 0);
	return 0;
}
static void  zbarIdentifyInit(zbar_identify_context *context)
{
	context->buffer_len = context->identify_width*context->identify_hight*3/2;
		printf("zbarIdentifyInit identify_width:%d,identify_hight:%d\r\n",g_context.identify_width,g_context.identify_hight);
		printf("==== zbarIdentifyInit buffer_len:%u====\r\n",context->buffer_len);
	
	context->buffer = osal_malloc(context->buffer_len);
	if(NULL != context->buffer){
		printf("==== zbarIdentifyInit osal_malloc success====\r\n");

	}
}
//中断回调,轻量级
static int zbarIdentifyStart(uint32_t irq, void *d, void *reg)  // process frame
{
	zbar_identify_context* context = (zbar_identify_context*)d;
	sendMsgNonblock(OS_TASK_ZBAR_IDENTIFY,TASK_MSG_DATA_INPUT, 0);
	printf("==== zbarIdentifyStart buffer_len:%u====\r\n",context->buffer_len);
	return 0;
}

void zbarIdentifySetStatus(void* d,S_INPUT_STATUS status)
{
	zbar_identify_context *context = d;
	if (status == S_INPUT_RUNNING)
	{
		printf("==== zbarIdentifySetStatus S_INPUT_RUNNING====\r\n");
		context->opt_status = ZBAR_OPT_DATA_PREPARE;
		zbarIdentifyInit(context);
		/////////CSI////////////////
		csiirq_register_j1dma_frame_end(zbarIdentifyStart,context);
		//内存清理,将原来在CPU缓存中的数据刷新到内存，以免在使用过程中DMA数据被CPU的缓冲数据污染
		dcache_flush_region((u32)context->buffer,(MALIG(context->identify_width,32)*context->identify_hight*3)/2);
		//DMA配置
		ax32xx_csihv_tomjp_dma_cfg(context->buffer,context->buffer+MALIG(context->identify_width,32)*context->identify_hight,
								   context->identify_hight,MALIG(context->identify_width,32));
		//缩放
		csi_j1dma_scale_down(context->src_width,context->src_hight,context->identify_width,context->identify_hight);
		csi_j1dma_restart();
		csi_enable();	
	}else if(status == S_INPUT_PAUSE){
		csi_j1dma_stop();
		if(context->buffer != NULL){
			osal_free(context->buffer);
		}
		context->buffer_len = 0;
		context->opt_status = ZBAR_OPT_WAIT;

		printf("==== zbarIdentifySetStatus S_INPUT_PAUSE====\r\n");
	}
}

void ZbarIdentifyThread(void *d) // thread
{
	u32 msgType, msgData, temp;
	OS_WAIT_DONE_MSG *msg;
	u32 count = 0;
	
	zbar_identify_context *context = (struct zbar_identify_context *)d;
	printf("ZbarIdentifyThread enter\n");
	context->runing = 1;
	while (1 == context->setrun){

		if (recMsg(OS_TASK_ZBAR_IDENTIFY, &msgType, &msgData) > 0){
			printf("OS_TASK_ZBAR_IDENTIFY recv msgType:%u\n",msgType);
			switch (msgType){
				case TASK_MSG_ZBAR_START:
				{	
					context->set_identify_run = 1;
					if(context->opt_status == ZBAR_OPT_WAIT){
						zbarIdentifySetStatus(d,S_INPUT_RUNNING);
					}else{
						printf("zbar identify in process,opt_status:%u\n",context->opt_status);	
					}
				}break;
				case TASK_MSG_ZBAR_STOP:
					zbarIdentifySetStatus(d,S_INPUT_PAUSE);
					context->set_identify_run = 0;
					break;
				case TASK_MSG_ZBAR_IDENTIFY:
				{
					zbar_indentify(d);
					context->opt_status = ZBAR_OPT_WAIT;
					zbarIdentifySetStatus(d,S_INPUT_PAUSE);
					//识别后释放资源，发送结果触发
					sendMsgNonblock(OS_TASK_UI,SYS_ZBAR_IDENTIFY_END,NULL);
				}break;
				case TASK_MSG_DATA_INPUT:
					context->opt_status = ZBAR_OPT_DATA_PREPARE;
					zbar_yuv_recv(d);
					break;
				default:
					printf("ZbarIdentify,unknow msgType:%d\n", msgType);
					break;
			}			
		}
		vTaskDelay(1);
	}
	vTaskDelete(NULL);
}

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void *start_block(void)
{
	printf("ZbarIdentify start_block!\r\n");
	zbar_identify_context *context = &g_context;
	sensorGetRes(&(g_context.src_width),&(g_context.src_hight));

	//缩放设置
	g_context.identify_width = 352;//g_context.src_width;
	g_context.identify_hight = 288;//g_context.src_hight;

	printf("ZbarIdentify src_width:%d,src_hight:%d\r\n",g_context.src_width,g_context.src_hight);


	stream_src_self_init(&context->self, NULL, NULL);
	printf("ZbarIdentify start_block,d:0x%x\r\n",context);

	return context;
}

static int end_block(void *d)
{
	printf("ZbarIdentify end_block,d:0x%x\r\n",d);

	zbar_identify_context *context = (struct zbar_identify_context *)d;
	context->setrun = 1;

	createdTaskByFunc(OS_TASK_ZBAR_IDENTIFY, ZbarIdentifyThread, d, 12, 1024, "ZbarIdentify", 50);

	return 0;
}

static int set_input(int num_tokens, struct token *tokens, void *d)
{
	zbar_identify_context *context = (zbar_identify_context *)d;
	return 0;
}

/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
    /* directive name, process function, min args, max args, arg types */
    {"input", set_input, 1, 1, {TOKEN_STR}},

    /* empty terminator -- do not remove */
    {NULL, NULL, 0, 0, {}}};

int zbar_steam_register(void)
{
  register_config_context("zbar", "identify", start_block, end_block, config_statements);
  return 0;
}

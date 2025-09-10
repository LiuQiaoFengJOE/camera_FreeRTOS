#include "usb_dev.h"
#include "osal/osal_alloc.h"
#include "usb_driver/usb_host/usb_host.h"
#include "task/app.h"
#include "usb/usb.h"

bool _usb_bulk_tx(u8 epx, u32 adr, u16 len,u16 blkszie)
{	
	//printf("usb_bulk_tx\n");
//	ClrIntSrc(_USB_INT_);//USBIE_DIS();
	ax32xx_intEnable(IRQ_USB20,0);
	
	u8 ret = true;
	u8 tepx = USB20_Index; //保存idex
    u16 txlen;
    u32 tx_addr = adr;//EP1_ADDR;
	
	
	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	volatile int i = 0x2ffff;
	//printf("u-i:%x\n",epx);
    while (len) {
        txlen = (len >= blkszie) ? blkszie : len;
		
		//*((u32 *)(&USB20E1TADR + 2 * (epx-1))) = tx_addr;
		__MSCTADR__ = adr;
		//USB20E3TADR = adr;
		//__U20ExTADR__(epx) = adr;
		
		USB20CON1 = BIT(13 + epx) | txlen;
		USB20_InCSR1 |= BIT(0);
        len -= txlen;
        tx_addr += txlen;
			
        i = 0x2fffff;
		while (USB20_InCSR1 & BIT(0))
		{			
			i--;
			if(i<=0)
			{
				//printf("msc btx tmo\n");
				ret = false;
				break;
			}
		}
    }
	__MSCTADR__ = (u32)adr; //复位地址
	//USB20E3TADR = (u32)adr; //复位地址
	//__U20ExTADR__(epx) = adr;
	USB20_Index=tepx;
	
//	SetIntSrc(_USB_INT_);//USBIE_EN();
	ax32xx_intEnable(IRQ_USB20,1);

	return ret;
}
bool _usb_bulk_rx(u8 epx, u32 adr, u16 len,u16 blkszie)
{	
	//printf("usb_bulk_rx\n");
//	ClrIntSrc(_USB_INT_);					//USBIE_DIS();
	ax32xx_intEnable(IRQ_USB20,0);
	u8 ret = true;
	u8 cnt = 0;
	u8 tepx = USB20_Index; //保存idex
	u8 error = false;
    u16 rxlen;
    u32 rx_addr = adr;//EP1_ADDR;

	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	//printf("u-i:%x\n",epx);
	volatile int i = 0xffffff;
    while (!error && len) 
	{
        rxlen = (len >= blkszie) ? blkszie : len;
		
        __MSCRADR__ = rx_addr;
		//USB20E3RADR = rx_addr;
		USB20_OutCSR1 = BIT(4);           //Flush FIFO, Clear RxRdy(接收下一个包)

        i = 0x2fffff;
		while (!error && !(USB20_OutCSR1 & BIT(0)))
		{
		    i--;
			if(i<=0)
			{
				ret = false;
				break;
			}		
		}
		
		cnt++;
		 
        len -= rxlen;
        rx_addr += rxlen;
    }

	USB20_Index = tepx;
//	SetIntSrc(_USB_INT_);//USBIE_EN();
	ax32xx_intEnable(IRQ_USB20,1);
	return ret;
}
bool _usb_iso_kick(u8 epx, u32 adr, u16 len)
{
//	ClrIntSrc(_USB_INT_);					//USBIE_DIS();	
	ax32xx_intEnable(IRQ_USB20,0);
	u8 tepx = USB20_Index; //保存idex
	//printf(":%x,%x,%d\n",epx,adr,len);
	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	//*((u32 *)(&USB20E1TADR + 2 * (epx-1))) = adr;
	if(2 == epx)
		USB20E2TADR = adr;
	if(3 == epx)
		USB20E3TADR = adr;
	if(4 == epx)
		USB20E4TADR = adr;
	//USB20E4TADR = adr;
	USB20CON1 = BIT(13 + epx) | len;

	USB20_InCSR1 |= BIT(7);
	if(USB20_InCSR1 & BIT(2)){
		USB20_InCSR1 &= ~BIT(2);
		//printf(">:%x\n",USB20_InCSR1);
	}
	USB20_InCSR1 |= BIT(0);//set bit txpktrdy

	USB20_Index = tepx;
//	SetIntSrc(_USB_INT_);//USBIE_EN()
    ax32xx_intEnable(IRQ_USB20,1);
	return true;
}
bool _iso_wait_txdown(u8 epx)
{
	volatile u32 tmout = 0xfffff;
	u8 tepx = USB20_Index; //保存idex
	USB20_Index = (epx & 0x7f);  
	while (1){
		if(!(--tmout)){
			printf("iso tout\n");
			USB20_Index = tepx;	
			return false;
		}
		if(!(USB20_InCSR1 & BIT(0))){
			USB20_Index = tepx;
			return true;
		}
	};//waitting iso paket
}

static u8 ftgl;
static u32 PTS;
static struct stream_dest* usbStreamDestVid;

#define  USBD_UVC_PKGSIZE		(996)
void uvcStart(u32 w,u32 h)
{
	ftgl = 0;
	PTS = -1;
	setLocalRes(w,h);
	streamSetRuning(usbStreamDestVid);
}
void uvcStop(void)
{
	streamSetPause(usbStreamDestVid);
}
static u8 uvcSaveBuff[USBD_UVC_PKGSIZE];
static u32 uvcSaveLen;
//extern u8 usbfifo[4096];
int usbSendBuff(u8 *fbuf,u32 flen)
{
	u8 *txfifo;
	u32 tlen;		
	//txbuf = _uvc_txbuf_[0];
	txfifo = _uvc_txfifo_;
	if(flen)
	{
		if(uvcSaveLen)
		{
			tlen=USBD_UVC_PKGSIZE-uvcSaveLen;
			if(tlen>flen)
				tlen=flen;
			memcpy(&uvcSaveBuff[uvcSaveLen], fbuf,tlen);
			flen -= tlen;
			fbuf += tlen;
			tlen+=uvcSaveLen;
			uvc_header_fill(ftgl, PTS, 0, txfifo);
			memcpy(txfifo+12, uvcSaveBuff,tlen);
			//dcache_writeback_region(txfifo, tlen+12);
			//ax32xx_mcpy1_sdram2gram(usbfifo,uvcSaveBuff, tlen);
			//ax32xx_mcpy2_uram2gram(txfifo+12, usbfifo, tlen);
			usb_uvciso_kick((u32)txfifo, tlen+12);
			if(!usb_uvciso_wait_txdown()) 
				return -1;
		}
		uvcSaveLen=0;
		while(flen)
		{
			if(flen >= USBD_UVC_PKGSIZE)
			{
				tlen=USBD_UVC_PKGSIZE;
				uvc_header_fill(ftgl, PTS, 0, txfifo);
				memcpy(txfifo+12, fbuf,tlen);
				//dcache_writeback_region(txfifo, tlen+12);
				//ax32xx_mcpy1_sdram2gram(usbfifo,fbuf, tlen);
				//ax32xx_mcpy2_uram2gram(txfifo+12, usbfifo, tlen);
				usb_uvciso_kick((u32)txfifo, tlen+12);
				if(!usb_uvciso_wait_txdown()) 
					return -1;
				flen -= tlen;
				fbuf += tlen;
			}
			else
			{
				uvcSaveLen=tlen=flen;
				memcpy(&uvcSaveBuff[0], fbuf,uvcSaveLen);
				flen -= tlen;
				fbuf += tlen;
			}
		}
		return 0;
	}
	if(uvcSaveLen)
	{
		uvc_header_fill(ftgl, PTS, 0, txfifo);
		memcpy(txfifo+12, uvcSaveBuff,uvcSaveLen);
		//dcache_writeback_region(txfifo, uvcSaveLen+12);
		//ax32xx_mcpy1_sdram2gram(usbfifo,uvcSaveBuff, uvcSaveLen);
		//ax32xx_mcpy2_uram2gram(txfifo+12, usbfifo, uvcSaveLen);
		usb_uvciso_kick((u32)txfifo, uvcSaveLen+12);
		if(!usb_uvciso_wait_txdown()) 
			return -1;
	}
	#if 1
	uvc_header_fill(ftgl & 0x01, PTS, 1, txfifo);
	//dcache_writeback_region(txfifo, 12);
	usb_uvciso_kick((u32)txfifo, 12);
	if(!usb_uvciso_wait_txdown()) 
		return -1;
	#endif
	return 0;
}
void sendMsgToUsbDev(void)
{
	if(usbDevMode())
		sendMsg(OS_TASK_USB_DEV,TASK_MSG_USBDEV_MSC, 0);
}
void uvcEnable(int en)
{
	if(en)
		sendMsg(OS_TASK_USB_DEV,TASK_MSG_USBDEV_UVC_ON, 0);
	else
		sendMsg(OS_TASK_USB_DEV,TASK_MSG_USBDEV_UVC_OFF, 0);
}
////////////////////////////////////////
struct sssUsbDev { // 繼承自stream_self
	struct stream_src_self self;
};
void usbSendFrame(struct frame *f)
{
	if(f==NULL)
		return;
	uint32 count=0;
	struct list_head* pos;
	struct frame_buf* buf;
	uint32	writeLen=0;
	uint32	remainDataLen=f->dataLen;
	uint32 bufsize = f->stream->ops->bufsize(f);
	u8 head;
	//printf("send frame:%d\n",bufsize);
	ftgl ^= 1; 
	PTS++;
	uvcSaveLen=0;
	if(!list_empty(&(f->buflist)))
	{		
		list_for_each(pos, &f->buflist)
		{
			if(!getUsbConnect())
				return;
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
				//count=osal_fwrite(buf->dat+head,1,writeLen-head,fp);
				if(usbSendBuff(buf->dat+head,writeLen-head)<0)
					return ;
				count+=writeLen;
			}
			else
			{
				if(usbSendBuff(buf->dat,writeLen)<0)
					return ;
				count+=writeLen;
			}
			if(remainDataLen==0)
			{
				goto SEND_END;
			}
		}
	}
	else
	{
		if(buf->dat[0]==0xff)
			head=0;
		else
			head=8;
		usbSendBuff(buf->dat+head,writeLen-head);
	}
SEND_END:
	usbSendBuff(NULL,0);
	return count;
}

void usbDevThread(void *d)
{
	u32 msgType,msgData;
	OS_WAIT_DONE_MSG *msg;
	struct frame *f;
	struct sssUsbDev* src = (struct sssUsbDev*)d;
	uvcSaveLen=0;
	usbStreamDestVid=stream_self_get_input(&src->self, 0);
	while (1)
	{
		if(recMsg(OS_TASK_USB_DEV,&msgType,&msgData)<=0)
		{
			printf("OS_TASK_USB_DEV recv msg error!!!!!!\n\n");
			continue;
		}
		switch(msgType)
		{
			case TASK_MSG_USBDEV_MSC:
				if(getUsbConnect())
					rbc_process();
				break;
			case TASK_MSG_DATA_INPUT:
				f=streamDestGetFrame(usbStreamDestVid);
				if(getUsbConnect())
					usbSendFrame(f);
				freeFrame(f);
				f=NULL;
				break;
			case TASK_MSG_USBDEV_UVC_ON:
				uvcStart(getUvcW(),getUvcH());
				break;
			case TASK_MSG_USBDEV_UVC_OFF:
				uvcStop();
				streamDestFreeFrames(usbStreamDestVid,0);
				break;
			case TASK_MSG_WAIT_DONE:
				msg=(OS_WAIT_DONE_MSG *)msgData;
				switch(msg->msgID)
				{
					case TASK_MSG_UNINIT:
					ax32xx_intEnable(IRQ_USB20,0);
					usb_phyreset_as_typeb();
					//usb_burst_reset();
					hal_usbdReset();
					streamSetPause(usbStreamDestVid);
					streamDestFreeFrames(usbStreamDestVid,0);
					break;
				}
				msg->done(msg);
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
static int wakeup_sender (struct frame *input, void *d)  // process frame
{
	struct stream_dest* streamDest=(struct stream_dest*)d;
	struct sssUsbDev *xchg = (struct sssUsbDev*)streamDest->self;		
	streamDestAddFrame(streamDest, input);
	uint32_t flags;
	flags = arch_local_irq_save();
	sendMsgNonblock(OS_TASK_USB_DEV,TASK_MSG_DATA_INPUT, 0);
	arch_local_irq_restore(flags);
  return 0;
}
static void usbDev_set_status (struct stream *s, S_INPUT_STATUS  status)
{
	struct sssUsbDev* src = (struct sssUsbDev*)s->self;
	struct stream_dest* streamDest;
	int i;
	while(1)
	{
		printf("OS_TASK_USB_DEV not surport set status\n");
	}
	if(status==S_INPUT_PAUSE)
	{
		//sendMsgAndWait(OS_TASK_USB_DEV,TASK_MSG_UNINIT,0);
	}
}
static struct stream_self_ops usbDevSelf_ops = {
  .set_status = usbDev_set_status,
  .lock = arch_local_irq_save,
  .unlock = arch_local_irq_restore,
};
static struct stream_src_ops usbDev_ops = {
	.stream_info = NULL,
	.bufsize = NULL,
	.free = (frame_free_func)NULL,
	.lock = arch_local_irq_save,
	.unlock = arch_local_irq_restore
};

/*********************** GLOBAL CONFIGURATION FUNCTIONS ***********************/
static void* start_block(void)
{
	struct sssUsbDev* src = (struct sssUsbDev*)osal_malloc(sizeof(struct sssUsbDev));
	if(src == NULL) {
	p_err("rtp-sender start_block malloc error");
	return NULL;
	}
	stream_src_self_init (&src->self, &usbDevSelf_ops, NULL); // NULL heap
  return src;
}

extern void usb_disk_write_thread(void *d);
static int end_block(void* d)
{
  // todo: 作些檢查
  //osal_thread_create(rtp_sender, d, 13, 0, 8192, "sender");
  struct sssUsbDev* xchg = (struct sssUsbDev*)d;
  struct stream_dest *input;
  input = stream_dest_new(wakeup_sender, d,60);
  stream_add_input(&xchg->self, input);
  createdTaskByFunc(OS_TASK_USB_DEV, usbDevThread,d,22,1024,"usb device",80);
  //osal_thread_create(usb_disk_write_thread, NULL, 23, 0, 1024, "usb_disk" );//u盘
  return 0;
}
static int set_output(int num_tokens, struct token *tokens, void *d)
{
	struct sssUsbDev *xchg = (struct sssUsbDev *)d;
	struct stream_src *output;

	output = stream_src_new(tokens[1].v.str, d);
	if (!output)
		return -1;

	// todo : 在output前進行的set_input，diffuse無效

	output->s.get_framerate = NULL;
	output->ops = &usbDev_ops;
	xchg->self.output = output;
	return 0;
}
static int set_input(int num_tokens, struct token* tokens, void* d)
{
  struct sssUsbDev* xchg = (struct sssUsbDev*)d;
  struct stream_dest *input;
//  if(!(input = stream_connect(tokens[1].v.str, (frame_deliver_func)wakeup_sender, d,60))) {
//    p_err("usbDev file: unable to connect to stream \"%s\"", tokens[1].v.str);
//    return -1;
//  }
//  stream_add_input(&xchg->self, input);
  return 0;
}


/*********************** GLOBAL CONFIGURATION DIRECTIVES **********************/
static struct statement config_statements[] = {
  /* directive name, process function, min args, max args, arg types */
  {"input", set_input, 1, 1, { TOKEN_STR } },
  {"output", set_output, 1, 1, {TOKEN_STR}},
  /* empty terminator -- do not remove */
  { NULL, NULL, 0, 0, {} }
};

int usb_device_register(void)
{
  register_config_context("usb", "device", start_block, end_block, config_statements);
  return 0;
}


#include "application.h"
#include "uiWin.h"
#include "menu.h"

#define PICSTICKER_SUM  4
static int picStickerjpg[PICSTICKER_SUM]={RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4};
static int picStickerLa[PICSTICKER_SUM]={RES_FRAMELISTA1,RES_FRAMELISTA2,RES_FRAMELISTA3,RES_FRAMELISTA4};
static int picStickerLb[PICSTICKER_SUM]={RES_FRAMELISTB1,RES_FRAMELISTB2,RES_FRAMELISTB3,RES_FRAMELISTB4};
typedef struct
{
	u32 index;
	u8 *pic;
	u32 picLen;
	u8 *lista;
	u32 listaLen;
	u8 *yuva;
	int frameaShow;
	u32 frameaW;
	u32 frameaH;
	u8 *listb;
	u32 listbLen;
	u8 *yuvb;
	int framebShow;
	u32 framebW;
	u32 framebH;
	QueueHandle_t mutex;
}_picSticker;
static _picSticker picSticker;

u8 *loadRes(u32 resID,u32 *len)
{
	u32 addr,size;
	u8 *buff;
	addr = res_open (resID); 
	size = res_size (resID);
	buff = (unsigned int*)osal_malloc(size);
	if(buff)
	{
		res_read (addr, buff, size);
		if(len)
			*len=size;
	}
	return buff;
}
void picStickerInit(void)
{
	picSticker.frameaShow=0;
	picSticker.framebShow=0;
	picSticker.pic=NULL;
	picSticker.lista=NULL;
	picSticker.listb=NULL;
	picSticker.yuvb=NULL;
	picSticker.index=0xffffffff;
	picSticker.framebW=0;
	picSticker.framebH=0;
	picSticker.mutex=xSemaphoreCreateMutex();
	lcdGetVideoRes(&picSticker.framebW,&picSticker.framebH);
	sensorGetRes(&picSticker.frameaW,&picSticker.frameaH);
	printf("frameaW:%d,frameaH:%d\n",picSticker.frameaW,picSticker.frameaH);
	printf("framebW:%d,framebH:%d\n",picSticker.framebW,picSticker.framebH);
}
void picStickerUninit(_picSticker *ppicStk)
{
	ppicStk->frameaShow=0;
	ppicStk->framebShow=0;
	if(ppicStk->pic)
		osal_free(ppicStk->pic);
	ppicStk->pic=NULL;
	if(ppicStk->lista)
		osal_free(ppicStk->lista);
	ppicStk->lista=NULL;
	if(ppicStk->listb)
		osal_free(ppicStk->listb);
	ppicStk->listb=NULL;
	if(ppicStk->yuvb)
		osal_free(ppicStk->yuvb);
	ppicStk->yuvb=NULL;
#if 0	
	if(ppicStk->yuva)
		osal_free(ppicStk->yuva);
#endif	
	ppicStk->yuva=NULL;
	ppicStk->index=0xffffffff;
}
void picStickerClose(void)
{
	picStickerUninit(&picSticker);
}
int loadPicSticker(_picSticker *ppicStk,u32 index)
{
	if(ppicStk->index==index)
	{
		if(ppicStk->lista||ppicStk->listb)
			return 1;
	}
	if(ppicStk->pic)
		osal_free(ppicStk->pic);
	ppicStk->pic=NULL;
	if(ppicStk->lista)
		osal_free(ppicStk->lista);
	ppicStk->lista=NULL;
	if(ppicStk->listb)
		osal_free(ppicStk->listb);
	ppicStk->listb=NULL;
	ppicStk->pic=loadRes(picStickerjpg[index],&ppicStk->picLen);
	ppicStk->lista=loadRes(picStickerLa[index],&ppicStk->listaLen);
	ppicStk->listaLen=ppicStk->listaLen>>2;
	ppicStk->listb=loadRes(picStickerLb[index],&ppicStk->listbLen);
	ppicStk->listbLen=ppicStk->listbLen>>2;
	printf("lista len:%d,listb len:%d\n",ppicStk->listaLen,ppicStk->listbLen);
	if(ppicStk->pic==NULL||ppicStk->lista==NULL||ppicStk->listb==NULL)
	{
		picStickerUninit(ppicStk);
		return -1;
	}
	ppicStk->index=index;
	return 0;
}

void picStickerLoop(void)
{
#if PIC_STICKER_EN
	u32 index;

	xSemaphoreTake(picSticker.mutex, portMAX_DELAY);
	index=picSticker.index+1;
	if(index==PICSTICKER_SUM)
	{
		picStickerUninit(&picSticker);
		goto END;
	}
	if(index>PICSTICKER_SUM)
		index=0;
	picStickerUninit(&picSticker);
	if(loadPicSticker(&picSticker,index)<0)
	{
		printf("loadPicSticker failed!!!\n");
		goto END;
	}
	if(picSticker.yuvb)
		osal_free(picSticker.yuvb);
	picSticker.yuvb=NULL;
	picSticker.yuvb = (unsigned int*)osal_malloc(picSticker.framebW*picSticker.framebH*3/2);
	if(picSticker.yuvb==NULL)
	{
		picStickerUninit(&picSticker);
		printf("malloc yuvb failed!!!\n");
		goto END;
	}
//	memset(picSticker.yuvb,0,(picSticker.framebW*picSticker.framebH*3/2));
	dcache_flush_region((u32)picSticker.yuvb, (picSticker.framebW*picSticker.framebH*3/2));
	
	printf("show pic:%d\n",picStickerjpg[picSticker.index]);
	not_isr_jpgdec2(picSticker.yuvb, picSticker.pic, picSticker.picLen, picSticker.framebW, picSticker.framebH);
	picSticker.framebShow=1;
	
#if 1
	picSticker.yuva = ((u32)getCurModeFreeBuffBase()+256)&(~0x3f);
	while(modeFreeSpace()<picSticker.frameaW*picSticker.frameaH*3/2+256)
	{
		printf("no memory for addPicStickerA!!!,%dKB\n\n",modeFreeSpace()>>10);
	}
#else
	if(picSticker.yuva)
		osal_free(picSticker.yuva);
	picSticker.yuva=NULL;
	
	picSticker.yuva = (unsigned int*)osal_malloc(picSticker.frameaW*picSticker.frameaH*3/2);
	if(picSticker.yuva==NULL)
	{
		picStickerUninit(&picSticker);
		printf("malloc yuva failed!!!\n");
		goto END;
	}
#endif	
	not_isr_jpgdec2(picSticker.yuva, picSticker.pic, picSticker.picLen, picSticker.frameaW, picSticker.frameaH);
	picSticker.frameaShow=1;
	if(picSticker.pic)
		osal_free(picSticker.pic);
	picSticker.pic=NULL;
END:
	xSemaphoreGive(picSticker.mutex);
	printf("heap remain size: %d [%dKB]\n",xPortGetFreeHeapSize(),xPortGetFreeHeapSize()>>10);
#endif
}

void addrPicSticker(u8 *dest,u8 *src,u32 *list,u32 listSize)
{
	u32 start,end,i;
	i=0;
	while(i<listSize) 
	{
		start = list[i++];
		end = list[i++];
		if(end>start)
		{
			//dma_memcpy1(dest+start,src+start,end-start);
			//ax32xx_mcpy1_sdram2gram(dest+start,src+start,end-start);
			memcpy(dest+start,src+start,end-start);
		}

	}
}
void addrPicSticker1(u8 *dest,u8 *src,u32 *list,u32 listSize)
{
	u32 start,end,i;
	i=0;
	while(i<listSize) 
	{
		start = list[i++];
		end = list[i++];
		if(end>start)
		{
			//dma_memcpy1(dest+start,src+start,end-start);
			//ax32xx_mcpy1_sdram2gram(dest+start,src+start,end-start);
			memcpy(dest+start,src+start,end-start);
		}

	}
}
void addPicStickerB(u8 *yBuff)
{
	if(picSticker.framebShow==0)
		return;
	addrPicSticker(yBuff,picSticker.yuvb,picSticker.listb,picSticker.listbLen);
	dcache_flush_region((u32)yBuff, (picSticker.framebW*picSticker.framebH*3/2));
}

void addPicStickerA(u8 *yBuff)
{
	xSemaphoreTake(picSticker.mutex, portMAX_DELAY);
	if(picSticker.frameaShow==0)
		goto END;
#if 1
	if (picSticker.yuva==NULL)
	{
		if(picSticker.pic)
			osal_free(picSticker.pic);
		picSticker.pic=loadRes(picStickerjpg[picSticker.index],&picSticker.picLen);

#if 1
		picSticker.yuva = ((u32)getCurModeFreeBuffBase()+256)&(~0x3f);
		while(modeFreeSpace()<picSticker.frameaW*picSticker.frameaH*3/2+256)
		{
			printf("no memory for addPicStickerA!!!,%dKB\n\n",modeFreeSpace()>>10);
		}
#else
		picSticker.yuva = (unsigned int*)osal_malloc(picSticker.frameaW*picSticker.frameaH*3/2);
		if(picSticker.yuva==NULL)
		{
			picStickerUninit(&picSticker);
			printf("malloc yuva failed!!!\n");
			goto END;
		}
#endif
		not_isr_jpgdec2(picSticker.yuva, picSticker.pic, picSticker.picLen, picSticker.frameaW, picSticker.frameaH);
		if(picSticker.pic)
			osal_free(picSticker.pic);
		picSticker.pic=NULL;
		picSticker.frameaShow=1;
		//dcache_writeback_region((u32)picSticker.yuva, (picSticker.frameaW*picSticker.frameaH*3/2));
	}
#endif	
	addrPicSticker1(yBuff,picSticker.yuva,picSticker.lista,picSticker.listaLen);
	dcache_flush_region((u32)yBuff, (picSticker.frameaW*picSticker.frameaH*3/2));
END:
	xSemaphoreGive(picSticker.mutex);

}

void freePicStickerA(void)
{
	xSemaphoreTake(picSticker.mutex, portMAX_DELAY);
	if (picSticker.frameaShow)
	{
#if 0	
		if(picSticker.yuva)
			osal_free(picSticker.yuva);
#endif		
		picSticker.yuva=NULL;
	//	picSticker.frameaShow=0;
	}
	xSemaphoreGive(picSticker.mutex);
}
int encAddPicSticker(void)
{
	return picSticker.frameaShow;
}









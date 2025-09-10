#include "task/app.h"
#include"fatfs/fileManage.h"
#include "fatfs/fileOps.h"
#include "drv/lcd/lcdframe.h"
#include "drv/jpeg/jpeg.h"

#include "uiThumbnallRes.c"

#define toTypeId(id)  ((id)+2)
u32 curItem = 0;
u32 curPage = 0;
u32 TotalPage = 0;

typedef struct{
u32 item;
u32 x;
u32 y;
u32 w;
u32 h;
}S_ITEM_P;
static void getItemParm(u32 item, u32 *x,u32 *y, u32 *w, u32 *h)
{
	S_ITEM_P iParm[THUMBNALL_NUM] = {
		{0, 0, 20, 210, 170},
		{1, 210, 20, 210, 170},
		{2, 420, 20, 210, 170},

		{3, 0, 192, 210, 170},
		{4, 210, 192, 210, 170},
		{5, 420, 192, 210, 170}
	};
		
	if (item>=THUMBNALL_NUM)
		return;
	
	*w = iParm[item].w;
	*h = iParm[item].h;

	*x = iParm[item].x;
	*y = iParm[item].y;
	//printf("---itemParm %d, [%d, %d, %d, %d]\n", item, *x, *y, *w, *h);
}

static void showImageThumb(winHandle handle,int indexStart,u32 num)
{
	INT32 i,j;
	
	char *name;
	INT32U size, pos;
	INT8U *buf = 0;
	int type;
	void *fp;
	INT8U app[12];
	INT16U cnt;
	u32 itemx;
	u32 itemy;
	u32 itemw;
	u32 itemh;
	
	disp_frame_t *lcdFrame = lcdVideoFrameMalloc();
	if (lcdFrame == NULL)
	{
		printf("lcdFrame is NULL\n");
		return -1;
	}
	u16 vidX,vidY,vidW,vidH;
	lcdGetVideoPos(&vidX,&vidY);
	lcdGetVideoRes(&vidW,&vidH);

	memset(lcdFrame->y_addr, 0x80, lcdFrame->w*lcdFrame->h*3/2);
	dcache_flush_region(lcdFrame->y_addr,lcdFrame->w*lcdFrame->h*3/2);
	lcdframeVideoCfg(lcdFrame, vidX,vidY,vidW,vidH);
	
	//printf("===%s,%d, idxStr:%d, num:%d, modeFreeSpace:%d\n", __FUNCTION__, __LINE__, indexStart, num, modeFreeSpace());

	for(i=indexStart;i<indexStart+num;i++)
	{
		name=getFileByIdx(i,&type);
		if (name == NULL)
		{
			printf("err! %s,%d\n", __FUNCTION__, __LINE__);
			return;
		}
		printf("=file %d: %s\n", i, name);
		fp = osal_fopen(name, "rb");
		if (fp == NULL)
		{
			printf("err! %s,%d\n", __FUNCTION__, __LINE__);
			return;
		}
		
		if(type==3)
		{
			size = osal_fsize(fp);
			pos=0;

			osal_fseek(fp, osal_fsize(fp)-sizeof(app));
			osal_fread(app, 1, sizeof(app), fp);
			if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
			{
				pos = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
				size = osal_fsize(fp) - pos;
			}
			else
			{
				osal_fseek(fp, 0);	
			}
		}
		else
			size = getAviFirstPic(fp, &pos);
		if (size == 0|| size>800*1024)
		{
			osal_fclose(fp);
			printf("err! %s,%d\n", __FUNCTION__, __LINE__);
			return ;
		}
		
		buf = ((u32)getCurModeFreeBuffBase()+128)&(~0x3f);
		osal_fseek(fp, pos);
		dcache_flush_region((u32)buf,size);
		osal_fread(buf, 1, size, fp);
		osal_fclose(fp);

		getItemParm(i-indexStart, &itemx, &itemy, &itemw, &itemh);
		not_isr_jpgdec3(lcdFrame->y_addr+(itemy*lcdFrame->stride)+itemx
			,lcdFrame->uv_addr+(itemy*lcdFrame->stride>>1)+itemx,lcdFrame->stride,buf,size,itemw,itemh);		
	}
	dcache_flush_region(lcdFrame->y_addr,lcdFrame->w*lcdFrame->h*3/2);
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	//printf("===%s,%d\n", __FUNCTION__, __LINE__);
	lcdDealVideBuff(lcdFrame);
	osal_task_sleep(60);
}


static void showImageThumbValid(winHandle handle)
{
	
}

static int thumbnallKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int type;
	char *name;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		name = getFileByIdx(curPage*THUMBNALL_NUM+curItem, &type);
		printf("curfile:%s, type:%d\n", name, type);
		functionStart(FUNCTION_PLAY_BACK,1);
	}
	return 0;
}

static int thumbnallKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 nexItem = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		nexItem = curItem>=THUMBNALL_NUM-1? 0:curItem+1;;
		if (THUMBNALL_NUM*curPage+nexItem<=getFileSum()-1)
		{
			thumbnallSelect(handle, curItem, 0);
			curItem = nexItem;
			thumbnallSelect(handle, curItem, 1);
		}		
	}
	return 0;
}
static int thumbnallKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fileNum,lastIndexStart,curIndexStart;
	uint32 keyState=KEY_STATE_INVALID;
	u32 nexItem = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		nexItem = curItem>0? curItem-1:THUMBNALL_NUM-1;
		if (THUMBNALL_NUM*curPage+nexItem<getFileSum()-1)
		{
			thumbnallSelect(handle, curItem, 0);
			curItem = nexItem;
			thumbnallSelect(handle, curItem, 1);
		}	
	}
	return 0;
}

static int thumbnallKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 num;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		curPage = curPage>=TotalPage-1?0:curPage+1;
		thumbnallShowPage(handle, curPage, TotalPage);
		if ((curPage+1)*THUMBNALL_NUM>getFileSum())
		{
			num = getFileSum() - curPage*THUMBNALL_NUM;
		}
		else
		{
			num = THUMBNALL_NUM;
		}
		showImageThumb(handle, curPage*THUMBNALL_NUM, num);
		
		thumbnallSelect(handle, curItem, 0);
		curItem = 0;
		thumbnallSelect(handle, curItem, 1);		
	}
	return 0;
}
static int thumbnallKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionChange();
	}
	return 0;
}
static int thumbnallSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}

static int thumbnallSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int thumbnallSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int thumbnallOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("thumbnall Open Win W:%d H:%d!!!\n",UNIT_W,UNIT_H);
	u32 num;
	u32 fIdx;
	
	changeDir(AVI_FILE);
	u32 sum=getFileSum();
	fIdx = getCurFileIdx(AVI_FILE);
	if (sum)
	{
		TotalPage = (sum-1)/THUMBNALL_NUM+1;
		curPage = fIdx/THUMBNALL_NUM;
		thumbnallShowPage(handle, curPage, TotalPage);
		
		if ((curPage+1)*THUMBNALL_NUM>getFileSum())
			num = getFileSum() - curPage*THUMBNALL_NUM;
		else
			num = THUMBNALL_NUM;
			
		showImageThumb(handle, curPage*THUMBNALL_NUM, num);		
        curItem = fIdx%THUMBNALL_NUM;
		thumbnallSelect(handle, curItem, 1);		
	}
	else
	{
		uiOpenWindow(&noFileWindow,0);
	}
	return 0;
}
static int thumbnallCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("thumbnall Close Win!!!\n");
	return 0;
}

static int thumbnallWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("thumbnall WinChild Open!!!\n");
	return 0;
}
static int thumbnallSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
  //  printf("----- curItem:%d curPage:%d  getFileSum()-1:%d----\r\n",curItem,curPage,getFileSum()-1);
	return 0;
}

msgDealInfor thumbnallMsgDeal[]=
{
	{SYS_OPEN_WINDOW,thumbnallOpenWin},
	{SYS_CLOSE_WINDOW,thumbnallCloseWin},
	{SYS_CHILE_OPEN,thumbnallWinChildOpen},
	{KEY_EVENT_OK,thumbnallKeyMsgOk},
	{KEY_EVENT_UP,thumbnallKeyMsgUp},
	{KEY_EVENT_DOWN,thumbnallKeyMsgDown},
	{KEY_EVENT_MENU,thumbnallKeyMsgMenu},
	{KEY_EVENT_MODE,thumbnallKeyMsgMode},
	{SYS_EVENT_SDC,thumbnallSysMsgSD},
	{SYS_EVENT_USB,thumbnallSysMsgUSB},
	{SYS_EVENT_BAT,thumbnallSysMsgBattery},
	{SYS_EVENT_1S,thumbnallSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(thumbnallWindow,thumbnallMsgDeal,thumbnallWin)




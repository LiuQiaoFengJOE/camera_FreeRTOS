#include "app.h"
#include "funcTakePhotoRes.c"
#include "mediaFile/demuxer.h"
#include "fatfs/fileOps.h"
#include "mediaFile/userInterface.h"
#include "image_watermark.h"
#include"mediaFile/picIdentify.h"
#include "wifiProc.h"
#define WATER_CHAR_GAP 0  //watermark character gap
#define ST_PIXEL_W    16	//res pic's w
#define ST_PIXEL_H	  32	//res pic's h

u8* small_pic_id_buf[20]; 

u32  small_pic_id[20]={RES_NUM_0,RES_NUM_1,RES_NUM_2,RES_NUM_3,RES_NUM_4,
								RES_NUM_5,RES_NUM_6,RES_NUM_7,RES_NUM_8,RES_NUM_9,
								RES_NUM_SLASH,RES_NUM_COLON,RES_NUM_BLANK,
							   };
static u8 showPhoto = 0;
uint32 lastPicRes = R_ID_STR_RES_1M;

void decodeEncodeTest(void)
{
	#define THUMBNAIL1_W  320
	#define THUMBNAIL1_H  240
	INT32 i,j;
#if 1
	char *name;
	u8 *yuvBuff;
	INT32U size, pos;
	INT8U *buf = 0;
	int type;
	void *fp,*fp1;
	changeDir(AVI_FILE);
	u32 time;
	time=os_time_get();
	u32 sum=getFileSum();
	for(i=0;i<sum;i++)
	{
		
		name=getFileByIdx(i,&type);
		if (name == NULL)
			return;
		fp = osal_fopen(name, "rb");
		if (fp == NULL)
			return;
		if(type==3)
		{
			size = osal_fsize(fp);
			pos=0;
		}
		else
			size = getAviFirstPic(fp, &pos);
		if (size == 0|| size>500*1024)
		{
			osal_fclose(fp);
			return;
		}
		buf = ((u32)getCurModeFreeBuffBase()+128)&(~0x3f);
		yuvBuff=buf;
		buf=((u32)(buf+THUMBNAIL1_W*THUMBNAIL1_H*3/2+256))&(~0x3f);
		osal_fseek(fp, pos);
		u32 time=os_time_get();
		dcache_flush_region((u32)buf,size);
		osal_fread(buf, 1, size, fp);
		osal_fclose(fp);
		printf("\njpg size:%dKB\n",size>>10);
		#if 1
		//fp1=osal_fopen(createJpgName(NULL), "w");
		//f_write(fp1, buf, size, &size);
		//osal_fclose(fp1);
		not_isr_jpgdec2(yuvBuff,buf,size,THUMBNAIL1_W,THUMBNAIL1_H);
		//jpgModeReset();
		size=not_isr_jpgEnc(yuvBuff,THUMBNAIL1_W,THUMBNAIL1_H,THUMBNAIL1_W,THUMBNAIL1_H,buf,buf+20*1024);
		printf("get thumbnail time:%d,size:%dKB\n",os_time_get()-time,size>>10);
		#endif
		
		printf("file write size:%d\n",size);
		fp=osal_fopen(createJpgName(NULL), "w");
		f_write(fp, buf, size, &size);
		osal_fclose(fp);
		printf("write size:%d\n",size);
	}
	printf("time:%d\n",os_time_get()-time);
#endif

}
#if 0
void showLastPic(void)
{
	void *fp;
	INT32U size, pos;
	void *jpgbuf;
	INT32U count;
#if LCD_EN
	changeDir(AVI_FILE);
	fp = osal_fopen(FIND_PRE_VID, "rb");
	if (fp == NULL)
		return;

	size = osal_fsize(fp);
	jpgbuf = osal_malloc(size+64);
	if(jpgbuf==NULL)
	{
		printf("malloc jpgbuf:%d error\r\n", size+64);
		return;
	}
	count = osal_fread(jpgbuf, 1, size, fp);
	printf("===osal_fread, count:%d\n", count);
	dcache_flush_region((u32)jpgbuf,size);
	osal_fclose(fp);
	printf("\njpg size:%dKB\n",size>>10);



	INT16U lcdW=0,lcdH=0,cnt;
	lcdGetRes(&lcdW,&lcdH);
	//jpgbufsize += 64;
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		osal_free(jpgbuf);
		printf("malloc lcdframe error\r\n");
		return;
	}
	lcdframeVideoCfg(lcdFrame,0,0,lcdW,lcdH);
	not_isr_jpgdec2(lcdFrame->y_addr, jpgbuf, size, lcdW, lcdH);
	osal_free(jpgbuf);
	//printf("lcdFrame->scan_mode=%d\r\n",lcdFrame->scan_mode);
	cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	lcdDealVideBuff(lcdFrame);
	osal_task_sleep(60);
	//lcdc_osd_enable(0, 0);
	//lcdc_osd_enable(1, 0);
	printf("aipic[%s] show end\n",mediaSta.aiPicName);
#endif
}
#endif
unsigned char g_take_zbar = 0;
static int photoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(g_take_zbar == 1){
			return -1;
		}
			
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}

		if((configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON) && (SysCtrl.photoMode != PHOTO_NORMAL))
		{
			if (takePictrueIsWorking()==1)
			{
				return 0;
			}
			else
			{
				if (get_wifi_statue(NULL, NULL) == 0)
				{
					uiOpenWindow(&tipsWindow,2,R_ID_STR_AI_PREPARING,3);
					return 0;
				}
				picAiInit();
			}

			configSet(CONFIG_ID_PRESLUTION,R_ID_STR_RES_1M);
		}	
		//decodeEncodeTest();
		//return 0;
		localTaketPhotoStart();
		videoTakePhotoSuccessShow(handle, 1);
		while(dacIsPlaying())
		{
			osal_task_sleep(5);
		}
		dacPlaySound(RES_TAKE_PHOTO_MUSIC,getSoundVol());
	}
	return 0;
}


static int photoZbarIdentifyEnd(winHandle handle,uint32 parameNum,uint32* parame)
{
	g_take_zbar = 0;
#if WIFI_EN
	printf("recv SYS_ZBAR_IDENTIFY_END from zbar.\n");
	functionStart(FUNCTION_WIFI,0);
#endif
	return 0;
}
static int photoZbarIdentify(winHandle handle,uint32 parameNum,uint32* parame)
{
	//char*wifi_string="WIFI:T:WPA;S:ipc_test;P:123456789;";
	char*wifi_string = (char*)(*parame);
	int ret = 0;
	dev_wifi_set wifi_set;
	if(wifi_string != NULL && 0 != strlen(wifi_string)){
		printf("recv zbar wifi_string:%s\n",wifi_string);
		//��o??��2?t?????a???��??��????:
		dev_wifi_set wifi_set;
		int ret = wifi_string_analyze(wifi_string,&wifi_set);
		if(ret == 0){
			wifi_init(&wifi_set);
		}
		
	}else{
		printf("parame verify err from zbar.\n");
	}

	return 0;
}

static int photoKeyMsgLongOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	#if 1
		if (takePictrueIsWorking()==0)
		{
			freePicStickerA();
		//	animationInit(LEFTBOTTOM2RIGHTUPPER);
		//	osal_task_sleep(50);
			sensor_change();
		}
    #endif
        
	}
	return 0;
}
static int photoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
#if 0	
		{	
			g_take_zbar = 1;
			printf("photoKeyMsgUp begin send OS_TASK_ZBAR_IDENTIFY,TASK_MSG_ZBAR_START\n");
			sendMsgNonblock(OS_TASK_ZBAR_IDENTIFY,TASK_MSG_ZBAR_START, 0);
		}	
#else	
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}
		if (SysCtrl.photoMode == PHOTO_NORMAL)
		{
			picStickerLoop();
		}
		else
		{
			//lcdShowModeChange();
	        SysCtrl.ailangid = SysCtrl.ailangid==PICID_LANGID_SCH?PICID_LANGID_ENG:PICID_LANGID_SCH;
	        printf("SysCtrl.ailangid:%d...\r\n",SysCtrl.ailangid);
			picIdtfySetLangid(SysCtrl.ailangid);
		}
//	#if WIFI_EN
//		functionStart(FUNCTION_WIFI,0);
//	#endif
#endif
	}
	return 0;
}

static int photoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;  
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}	
		printf("key down\n");

		if (SysCtrl.photoMode == PHOTO_NORMAL)
		{
			picStickerLoop();
		}
	}
	return 0;
}

static int photoKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;  
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(ax32xx_VDDGSENRead()){
			ax32xx_VDDGSENEnable(0);
			photoLedShow(handle,0);
		}else{
			ax32xx_VDDGSENEnable(1);
			photoLedShow(handle,1);
		}
	}
	return 0;
}

extern menu MENU(movie);
static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 i,j,lastTime,alignsize,size;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}	
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));
	}
	return 0;
}
static int photoKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}	
		functionStart(FUNCTION_MAINMENU2,0);
		//functionChange();
		//functionStart(FUNCTION_AUDIO_PLAYER,0);
	}
	return 0;
}

static u8 lastlevel=0;
static int photoKeyMsgZoomIn(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        SysCtrl.crop_level++;
        if(SysCtrl.crop_level>CROPLEVEMAX-1)
            SysCtrl.crop_level=0;        
        if (lastlevel!= SysCtrl.crop_level)
        {
            lastlevel = SysCtrl.crop_level;
            lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
        }
        photoZoomShow(handle, SysCtrl.crop_level);
	}
	return 0;
}
static int photoKeyMsgZoomOut(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        SysCtrl.crop_level--;
        if(SysCtrl.crop_level>CROPLEVEMAX-1)
            SysCtrl.crop_level=CROPLEVEMAX-1;              
        if (lastlevel!= SysCtrl.crop_level)
        {
            lastlevel = SysCtrl.crop_level;
            lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
        }
        photoZoomShow(handle, SysCtrl.crop_level);
	}
	return 0;
}

static int photoKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if (showPhoto == 1)
		{
			showPhoto = 0;
			ax32xx_csiLCDDmaEnable(1);
			lcdPreviewStart();
			return 0;
		}
		
		if(takePictrueIsWorking())
		{
			printf("zzy takePhotoWorking....\n");
			return 0;
		}		

		if((configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)&& (SysCtrl.photoMode!=PHOTO_NORMAL))
		{
			printf("===@@@@@@@@===##==wifi stop\n");
			picAiUnInit();
			//wifi_stop();
		}
		
		//printf("===sticker loop\n");
		//picStickerLoop();
		if(PHOTO_IDCONTENT==SysCtrl.photoMode)
			functionStart(FUNCTION_MAINMENU,0);
		else
			functionStart(FUNCTION_MAINMENU2,0);
	}
	return 0;
}
static int photoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoSDShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,2);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,2);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,2);
	else if ((SysCtrl.sdcard == SDC_STAT_NORMAL) && (SysCtrl.photoMode!=PHOTO_NORMAL))
	{
		if(configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)
		{
			start_wifi(1);
		}
	}
	return 0;
}

static int photoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}

static int photoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}

static int photoSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int photoTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/

	if(parameNum!=3)
	{
		printf("takephotoTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if((parame[2]==TOUCH_RELEASE) && (takePictrueIsWorking()==0))
	{
		if(parame[0]==PHOTO_EXE_MAIN_ID)
		{
			printf("==back to mainmenu\n");
			functionStart(FUNCTION_MAINMENU,0);
			//printf("===sticker loop\n");
			//picStickerLoop();
		}
		else if (parame[0]==PHOTO_MODE_ID)
		{
			if(SysCtrl.crop_level>0)
				SysCtrl.crop_level--;
			else
				SysCtrl.crop_level=6;
			
			lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
		}

		/*else if(parame[0]==VIDEO_EXE_RECORD_ID)
		{		
			printf("the event photo press\n");
			
			//decodeEncodeTest();
			//return 0;
			#if SUPPORT_PRINTER_PHOTO
			if(get_sd_status()==0||SysCtrl.sdcard==SDC_STAT_ERROR)
				setPrinterflag(1);
			else
				setPrinterflag(2);
			#endif
			localTaketPhotoStart();
			while(dacIsPlaying())
			{
				osal_task_sleep(5);
			}
			dacPlaySound(RES_TAKE_PHOTO_MUSIC,getSoundVol());
		}
		else if(parame[0]==VIDEO_CAPTURE_PHOTO_ID)
		{
			if(videoRecordGetStatus() == MEDIA_STAT_START)
			{
				winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),true);
				videoCapturePhoto();
			}
		}
		else if(parame[0]==VIDEO_EXE_LOCK_ID)
		{
			XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(KEY_EVENT_LOCK,KEY_PRESSED));
		}
		else if(parame[0]==VIDEO_EXE_MIC_ID)
		{
			XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(KEY_EVENT_MIC,KEY_PRESSED));
		}
		else if(parame[0]==VIDEO_EXE_LIG_ID)
		{
		}*/
	}
	return 0;
}


static int photoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("====photo Open Win!!!\n");
	if ((SysCtrl.sdcard == SDC_STAT_NORMAL) && (SysCtrl.photoMode!=PHOTO_NORMAL))
	{
		if(configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)
		{
			start_wifi(0);
		}
	}	
	mdSetEn(0);
	photoResolutionShow(handle);
	photoIrLedShow(handle);
	if (SysCtrl.photoMode!=PHOTO_NORMAL)
	{
		photoWifiShow(handle, 1, 1);
	}
	else
	{
		photoWifiShow(handle, 0, 1);
	}
	photoSDShow(handle);
	photoBaterryShow(handle);
	winSetVisible(winItem(handle,PHOTO_CAPTURE_PHOTO_ID),false);
	if(SysCtrl.timeStamp==1)
		winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	watermark_bmp2yuv_init(ST_PIXEL_W,ST_PIXEL_H,SM_PIC_ST_NUM); //place early
	lastPicRes = configGet(CONFIG_ID_PRESLUTION);
	printf("===lastPicRes:%d\n", lastPicRes);
	return 0;
}
static int photoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("photo Close Win!!!\n");
	
	picStickerClose();
	animationUnInit();
	watermark_buf_bmp2yuv_free();
	if((configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON) && (SysCtrl.photoMode != PHOTO_NORMAL))
	{
		configSet(CONFIG_ID_PRESLUTION,lastPicRes);
		printf("===%s,%d, %d, %d\n", __FUNCTION__, __LINE__, lastPicRes, configGet(CONFIG_ID_PRESLUTION));
	}
	return 0;
}
static int photoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("photo WinChild Close!!!\n");
	photoResolutionShow(handle);
	photoIrLedShow(handle);
	photoSDShow(handle);
	photoBaterryShow(handle);
	winSetVisible(winItem(handle,PHOTO_CAPTURE_PHOTO_ID),false);
	return 0;
}
static int photoWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("photo WinChild Open!!!\n");
	return 0;
}

static int photoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if (takePictrueIsWorking()==0)
	{
		videoTakePhotoSuccessShow(handle, 0);
	}

	if (SysCtrl.photoMode!=PHOTO_NORMAL)
		photoWifiShow(handle, 1, 0);
	if(SysCtrl.timeStamp==1)
		winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int photoSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	static uint32 lastTime = 0;
	if(lastTime==0)
	{
		lastTime=XOSTimeGet();
		printf("start time:%d    !!!!!!!!!!!!!!!!!!!!!!\n\n\n",lastTime);
		return 0;
	}
	if(XOSTimeGet()-lastTime>60*3*1000)
	{
		lastTime=XOSTimeGet();
		printf("end time:%d    !!!!!!!!!!!!!!!!!!!!!!\n\n\n",lastTime);
	}
#endif
}
static int photoSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	
}

static int photoSysMsgIdentifyFinish(winHandle handle,uint32 parameNum,uint32* parame)
{
	if (SysCtrl.photoMode == PHOTO_SMALL_MODE)
	{
		uiOpenWindow(&SmallModelWindow, 1, *parame);
		return;
	}
	else if (SysCtrl.photoMode == PHOTO_AI_CHAT)
	{
		functionStart(FUNCTION_AUDIO_RECORD,0);
		return;
	}

	videoTakePhotoSuccessShow(handle, 0);
	if (SysCtrl.ailangid == PICID_LANGID_SCH)
	{
		uiOpenWindow(&idContentUniWindow,1, *parame);
	}
	else
	{
		uiOpenWindow(&idContentWindow,1, *parame);
	}
	
}

static int photoSysMsgLivePreview(winHandle handle,uint32 parameNum,uint32* parame)
{
	ax32xx_csiLCDDmaEnable(0);
	lcdPreviewStop();
#if 0	
	uiOpenWindow(&livePreviewWindow,0 );
#else
	char *name;
	fileInforInit(AVI_FILE);
	changeDir(AVI_FILE);
	name=FIND_PRE_VID;
	LivePreviewShow(name);
	showPhoto = 1;
#endif
    //functionStart(FUNCTION_PLAY_BACK,0);
}

msgDealInfor takePhotoMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoOpenWin},
	{SYS_CLOSE_WINDOW,photoCloseWin},
	{SYS_CHILE_COLSE,photoWinChildClose},
	{SYS_CHILE_OPEN,photoWinChildOpen},
	{SYS_TOUCH_WINDOW,photoTouchWin},
	{KEY_EVENT_PHOTO,photoKeyMsgOk},
	//{KEY_EVENT_LONG_OK,photoKeyMsgLongOk},
	{KEY_EVENT_UP,photoKeyMsgUp},
	{KEY_EVENT_DOWN,photoKeyMsgDown},
	{KEY_EVENT_RIGHT,photoKeyMsgRight},
	{KEY_EVENT_SETTING,photoKeyMsgMenu},
	{KEY_EVENT_MODE,photoKeyMsgMode},
	{KEY_EVENT_ZOOMIN,photoKeyMsgZoomIn},
	{KEY_EVENT_ZOOMOUT,photoKeyMsgZoomOut},
	{SYS_EVENT_SDC,photoSysMsgSD},
	{SYS_EVENT_USB,photoSysMsgUSB},
	{SYS_EVENT_BAT,photoSysMsgBattery},
	//{SYS_EVENT_500MS,photoSysMsg500MS},
	{SYS_TIME_UPDATE,photoSysMsgUpdateTime},
	{VID_EVENT_IDENTIFY_FINISH,photoSysMsgIdentifyFinish},
    {VID_EVENT_LIVE_PREVIEW,photoSysMsgLivePreview},
	{SYS_EVENT_1S,photoSysMsg1S},
	{KEY_EVENT_POWER,photoKeyMsgPower},
	{SYS_ZBAR_IDENTIFY_RESUALT,photoZbarIdentify},
	{SYS_ZBAR_IDENTIFY_END,photoZbarIdentifyEnd},
	
	{EVENT_MAX,NULL},
};

WINDOW(takePhotoWindow,takePhotoMsgDeal,takePhotoWin)



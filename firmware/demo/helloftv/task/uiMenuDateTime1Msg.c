#include "touchPanel.h"

#include "uiMenuDateTime1Res.c"
#include "drv/rtc/rtc.h"

#define DATE_TIME_ITEM_MAX      8
#define DATE_TIME_STR_LEN       10
static char dateTime1Str[6][DATE_TIME_STR_LEN];
static uint32 dateTime1Data[6];
static unsigned char daysPerMon[12]={31,29,31,30,31,30,31,31,30,31,30,31};

char *getDateTimeStr(void)
{
	if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
		return "YY/MM/DD";
	else if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_MDY)
		return "MM/DD/YY";
	else
		return "DD/MM/YY";
}
u32 getDateIndex(u32 id)
{
	if(id==DATETIME_YEAR)
	{
		if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
			return 0;
		else
			return 2;
	}
	if(id==DATETIME_MON)
	{
		if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
			return 1;
		else if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_MDY)
			return 0;
		else
			return 1;
	}
	if(id==DATETIME_DAY)
	{
		if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
			return 2;
		else if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_MDY)
			return 1;
		else
			return 0;
	}
	return 0;
}
void dateTimeLoad(void)
{
	DATE_TIME_T *rtcTime;
	rtcTime = hal_rtcTimeGet();
	dateTime1Data[getDateIndex(DATETIME_YEAR)]=rtcTime->year;
	dateTime1Data[getDateIndex(DATETIME_MON)]=rtcTime->month;
	dateTime1Data[getDateIndex(DATETIME_DAY)]=rtcTime->day;
	dateTime1Data[3]=rtcTime->hour;
	dateTime1Data[4]=rtcTime->min;
	dateTime1Data[5]=rtcTime->sec;
	dateTime1StrUpdate(0);
	dateTime1StrUpdate(1);
	dateTime1StrUpdate(2);
	dateTime1StrUpdate(3);
	dateTime1StrUpdate(4);
	dateTime1StrUpdate(5);
}
void dateTimeSave(void)
{
	DATE_TIME_T rtcTime;
	rtcTime.year = dateTime1Data[getDateIndex(DATETIME_YEAR)];
	rtcTime.month =dateTime1Data[getDateIndex(DATETIME_MON)];
	rtcTime.day = dateTime1Data[getDateIndex(DATETIME_DAY)];
	rtcTime.hour = dateTime1Data[3];
	rtcTime.min = dateTime1Data[4];
	rtcTime.sec = dateTime1Data[5];		
	hal_rtcTimeSet(&rtcTime);
}

void dateTime1StrUpdate(uint32 num)
{
	if(num<6)
		intToStringFill0(&dateTime1Str[num],DATE_TIME_STR_LEN,dateTime1Data[num],2);
}
void dateTime1Increace(uint32 num)
{
	if(num>=6)
		return;
	dateTime1Data[num]+=1;
	if(num==getDateIndex(DATETIME_YEAR)&&dateTime1Data[num]>2050)
		dateTime1Data[num]=1990;
	if(num==getDateIndex(DATETIME_MON)&&dateTime1Data[num]>12)
		dateTime1Data[num]=1;
	if(num==getDateIndex(DATETIME_DAY)&&dateTime1Data[num]>31/*daysPerMon[dateTime1Data[num-1]-1]*/)
		dateTime1Data[num]=1;
	if(num==3&&dateTime1Data[num]>23)
		dateTime1Data[num]=0;
	if(num==4&&dateTime1Data[num]>59)
		dateTime1Data[num]=0;
	if(num==5&&dateTime1Data[num]>59)
		dateTime1Data[num]=0;
	dateTime1StrUpdate(num);
}
void dateTime1Decreace(uint32 num)
{
	if(num>=6)
		return;
	dateTime1Data[num]-=1;
	if(num==getDateIndex(DATETIME_YEAR)&&dateTime1Data[num]<1990)
		dateTime1Data[num]=2050;
	if(num==getDateIndex(DATETIME_MON)&&dateTime1Data[num]==0)
		dateTime1Data[num]=12;
	if(num==getDateIndex(DATETIME_DAY)&&dateTime1Data[num]==0)
		dateTime1Data[num]=31;//daysPerMon[dateTime1Data[num-1]-1];
	if(num==3&&dateTime1Data[num]>23)
		dateTime1Data[num]=23;
	if(num==4&&dateTime1Data[num]>59)
		dateTime1Data[num]=59;
	if(num==5&&dateTime1Data[num]>59)
		dateTime1Data[num]=59;
	dateTime1StrUpdate(num);
}

uint32 getDateTime1ResInfor(u16 id)
{
	printf("getDateTime1ResInfor:%d\n",id);
	if(id==DATETIME_YEAR)
		return (uint32)(&dateTime1Str[0]);
	else if(id==DATETIME_MON)
		return (uint32)(&dateTime1Str[1]);
	else if(id==DATETIME_DAY)
		return (uint32)(&dateTime1Str[2]);
	else if(id==DATETIME_HOUR)
		return (uint32)(&dateTime1Str[3]);
	else if(id==DATETIME_MIN)
		return (uint32)(&dateTime1Str[4]);
	else if(id==DATETIME_SEC)
		return (uint32)(&dateTime1Str[5]);
	else if(id==DATETIME_OK)
		return getDateTimeStr();
	//else if(id==DATETIME_CANCEL)
	//	return R_ID_STR_COM_CANCEL;
	else
		return INVALID_RES_ID;
	
}
static void dateTimeMoveIcon(winHandle handle,uint16 id)
{
#if 0
	uiRect pos;
	int16 x0,y0,x1,y1;
	pos.x0=0;
	pos.x1=0;
	pos.y0=0;
	pos.y1=0;
	winGetPos(winItem(handle,id),&pos);
	if(pos.x0==pos.x1)
		return;
	x0=pos.x0+((pos.x1-pos.x0-20)>>1);
	x1=x0+20;
	y0=pos.y0-20;
	y1=y0+20;
	winResize(winItem(handle,DATETIME_UP),x0,y0,x1,y1);
	y0=pos.y1;
	y1=y0+20;
	winResize(winItem(handle,DATETIME_DOWN),x0,y0,x1,y1);
#endif
}
static int dateTime1KeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint16 id;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	#if 1
		id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
		if(id<DATETIME_OK)
		{
			widgetManageNext(winItem(handle,DATETIME_WIDGET_MANAGE));
			id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
			dateTimeMoveIcon(handle,id);
		}
		else if(id==DATETIME_OK)
		{
			dateTimeSave();
			winDestroy(&handle);
		}
	#else
		dateTimeSave();
		winDestroy(&handle);
	#endif
		//else if(id==DATETIME_CANCEL)
		//	winDestroy(&handle);
	}
	return 0;
}
static int dateTime1KeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint16 id;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
		if(id<DATETIME_OK)
		{
			dateTime1Increace(id-DATETIME_YEAR);
			widgetManageUpdateCur(winItem(handle,DATETIME_WIDGET_MANAGE));
		}
		else
		{
			//widgetManagePre(winItem(handle,DATETIME_WIDGET_MANAGE));
			dateTimeSave();
			if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_DMY);
			else if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_DMY)
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_MDY);
			else
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_YMD);
			dateTimeLoad();
			widgetManageUpdateAll(winItem(handle,DATETIME_WIDGET_MANAGE));
			/*winUpdate(winItem(handle,DATETIME_YEAR));
			winUpdate(winItem(handle,DATETIME_MON));
			winUpdate(winItem(handle,DATETIME_DAY));
			winSetResid(winItem(handle,DATETIME_OK),strToResId(getDateTimeStr()));*/
		}
	}
	return 0;
}
static int dateTime1KeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint16 id;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
		if(id<DATETIME_OK)
		{
			dateTime1Decreace(id-DATETIME_YEAR);
			widgetManageUpdateCur(winItem(handle,DATETIME_WIDGET_MANAGE));
		}
		else
		{
		  //widgetManageNext(winItem(handle,DATETIME_WIDGET_MANAGE));
			dateTimeSave();
			if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_YMD)
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_MDY);
			else if(configGet(CONFIG_ID_DATEFORMAT)==R_ID_STR_COM_MDY)
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_DMY);
			else
				configSet(CONFIG_ID_DATEFORMAT,R_ID_STR_COM_YMD);
			dateTimeLoad();
			widgetManageUpdateAll(winItem(handle,DATETIME_WIDGET_MANAGE));
			/*winUpdate(winItem(handle,DATETIME_YEAR));
			winUpdate(winItem(handle,DATETIME_MON));
			winUpdate(winItem(handle,DATETIME_DAY));
			winSetResid(winItem(handle,DATETIME_OK),strToResId(getDateTimeStr()));*/
		}
	}
	return 0;
}
static int dateTime1KeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint16 id;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		widgetManagePre(winItem(handle,DATETIME_WIDGET_MANAGE));
		id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
		dateTimeMoveIcon(handle,id);
	}
	return 0;
}
static int dateTime1KeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint16 id;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		widgetManageNext(winItem(handle,DATETIME_WIDGET_MANAGE));
		id=widgetManageCurId(winItem(handle,DATETIME_WIDGET_MANAGE));
		dateTimeMoveIcon(handle,id);
	}
	return 0;
}

static int dateTime1KeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//dateTimeSave();
		winDestroy(&handle);
	}
	return 0;
}
static int dateTime1KeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int dateTime1OpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	resInfor infor;
	printf("dateTime1 Open Win!!!\n");
	dateTimeLoad();
	
#if 0
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_CENTER;
	infor.fontColor=R_COLOR_WHITE;
	infor.bgColor=R_COLOR_BLUE1;
	winSetSelectInfor(winItem(handle,DATETIME_WIDGET_MANAGE),&infor);
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_CENTER;
	infor.fontColor=R_COLOR_WHITE;
	infor.bgColor=R_COLOR_GRAY;
	winSetUnselectInfor(winItem(handle,DATETIME_WIDGET_MANAGE),&infor);
#else
	/*resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_CENTER;
	infor.fontColor=R_COLOR_WHITE;
	infor.bgColor=R_COLOR_BLUE1;
	winSetSelectInfor(winItem(handle,DATETIME_OK),&infor);
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_CENTER;
	infor.fontColor=R_COLOR_GREEN;
	infor.bgColor=R_COLOR_GRAY;
	infor.rimColor=R_COLOR_RED;
	winSetUnselectInfor(winItem(handle,DATETIME_OK),&infor);*/
#endif
	widgetManageUpdateAll(winItem(handle,DATETIME_WIDGET_MANAGE));
	widgetManageNext(winItem(handle,DATETIME_WIDGET_MANAGE));
	winSetResid(winItem(handle,DATETIME_OK),strToResId(getDateTimeStr()));
	dateTimeMoveIcon(handle,DATETIME_YEAR);
	return 0;
}
static int dateTime1CloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("dateTime1 Close Win!!!\n");
	return 0;
}
static int dateTime1WinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("dateTime1 WinChild Close!!!\n");
	return 0;
}
static int dateTime1SysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int dateTime1TouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("dateTime1, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		setNotplayKeySoundFlag(1);
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, KEY_PRESSED);
	}
	return 0;
}
static int dateTime1Sliding(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=1)
	{
		printf("playBackSlidRelease, parame num error %d\n",parameNum);
		return 0;
	}
	if(parame[0]==SLID_DIRECT_UP)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_DOWN, KEY_PRESSED);
	else if(parame[0]==SLID_DIRECT_DOWN)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_UP, KEY_PRESSED);
	
	return 0;
}



msgDealInfor dateTime1MsgDeal[]=
{
	{SYS_OPEN_WINDOW,dateTime1OpenWin},
	{SYS_CLOSE_WINDOW,dateTime1CloseWin},
	{SYS_CHILE_COLSE,dateTime1WinChildClose},
	{SYS_TOUCH_WINDOW,dateTime1TouchWin},
	{SYS_SLID_RELEASE,dateTime1Sliding},
	{KEY_EVENT_OK,dateTime1KeyMsgOk},
	{KEY_EVENT_LEFT,dateTime1KeyMsgUp},
	{KEY_EVENT_RIGHT,dateTime1KeyMsgDown},
	{KEY_EVENT_SETTING,dateTime1KeyMsgMenu},
	//{KEY_EVENT_RIGHT,dateTime1KeyMsgMode},
	{KEY_EVENT_POWER,dateTime1KeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(dateTime1Window,dateTime1MsgDeal,dateTime1Win)



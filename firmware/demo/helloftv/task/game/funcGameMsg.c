#include "../app.h"

#if GAME_FUNC_EN

#include "task/uiWin.h"

#include "funcGame.h"

static int gameKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_RIGHT,0));
	}
	return 0;
}

static int gameKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_UP,0));
	}
	return 0;
}

static int gameKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_DOWN,0));
	}
	return 0;
}

static int gameKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_LEFT,0));
	}
	return 0;
}

static int gameKeyReBack(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	
	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_TOUCH_BACK,0));
	#if 0
		functionStart(FUNCTION_GAME_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	#endif
	}
	return 0;
}

static int gameKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_OK,0));
	}
	return 0;
}

static int gameOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	sendMsgNonblock(OS_TASK_GAME,KEY_EVENT_OK, 0);
	return 0;
}

static int gameCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}



static int gameSysMsgGameTick(winHandle handle,uint32 parameNum,uint32* parame)
{
	static int times  = 0;
	times += 50;
	if(times>=sleeptime)
	{
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_GAME_TICK,0));	
		times = 0;
	}
	return 0;
}


static widgetCreateInfor gameWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	widgetEnd(),
};


msgDealInfor gameMsgDeal[]=
{
	{SYS_OPEN_WINDOW,gameOpenWin},
	{SYS_CLOSE_WINDOW,gameCloseWin},
	{KEY_EVENT_UP,gameKeyMsgUp},
	{KEY_EVENT_DOWN,gameKeyMsgDown},
	{KEY_EVENT_MENU,gameKeyMsgLeft},//left
	{KEY_EVENT_MODE,gameKeyMsgRight},//right
	{KEY_EVENT_POWER,gameKeyReBack},//返回
	{KEY_EVENT_OK,gameKeyMsgMode},
	//{KEY_EVENT_RETURN,gameKeyMsgExit},
	{SYS_EVENT_50MS,gameSysMsgGameTick},
	{EVENT_MAX,NULL},
};

WINDOW(gameWindow,gameMsgDeal,gameWin)

#endif

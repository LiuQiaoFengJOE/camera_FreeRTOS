#ifndef __UI_WIN_H
#define  __UI_WIN_H
#include"sUI/sUI.h"
#include "menu.h"
#include "task/app.h"
#include "hal_watermark/ascii.h"
#define JK_REMOVE

#define R_COLOR_TRANSFER   0xf9		
#define R_COLOR_TBLACK     0xf8
#define R_COLOR_GREEN      0xfd
#define R_COLOR_RED        0xfe
#define R_COLOR_WHITE      0xfb			
#define R_COLOR_BLACK      0xfa
#define R_COLOR_YELLOW     0xf7
#define R_COLOR_BLUE       0xfc
#define R_COLOR_DBLUE      0xf4
#define R_COLOR_BLUE1      0xf5
#define R_COLOR_BLUE2      0xf3
#define R_COLOR_ORANGE     0xfb
#define R_COLOR_GRAY       0xf6
#define R_COLOR_GRAY2      0xf2
#define R_COLOR_GRAY3      0xf1
#define SMALL_PANEL_SUPPORT  1


#define  TIPS_KEEP_SHOWING     0xffffffff
#define  TIPS_SD_NOT_INSERT    R_ID_STR_SDC_NULL
#define  TIPS_SD_FULL          R_ID_STR_SDC_FULL
#define  TIPS_SD_ERROR         R_ID_STR_SDC_ERROR
#define  TIPS_USENSOR_POWER_LOW  R_ID_STR_PWR_BACKLOW
#define  TIPS_POWER_LOW   R_ID_STR_PWR_LOW
#define  TIPS_NO_POWER   R_ID_STR_PWR_NO

typedef int (*msgDealFunc)(void* handle,uint32 parameNum,uint32* parame);
typedef struct _msgDealInfor
{
	uint32 msgId;
	msgDealFunc func;
}msgDealInfor;

typedef struct _uiWin
{
	msgDealInfor* msgDeal;
	widgetCreateInfor* widgetInfor;
	winHandle   handle;
	bool  repeateOpenSupport;
}uiWin;


extern uiWin wifiWindow;
extern uiWin videoRecordWindow;
extern uiWin photoEncodeWindow;
extern uiWin playbackWindow;
extern uiWin menuItemWindow;
extern uiWin menuOptionWindow;
extern uiWin tipsWindow;
extern uiWin tips1Window;
extern uiWin versionWindow;
extern uiWin dateTimeWindow;
extern uiWin menuUsbWindow;
extern uiWin noFileWindow;
extern uiWin formatWindow;
extern uiWin defaultWindow;
extern uiWin delCurWindow;
extern uiWin delAllWindow;
extern uiWin lockCurWindow;
extern uiWin gameMenuWindow;
extern uiWin gameWindow;
extern uiWin lockAllWindow;
extern uiWin unlockAllWindow;
extern uiWin unlockCurWindow;
extern uiWin asternWindow;
extern uiWin selfTestWindow;
extern uiWin audioRecordWindow;
extern uiWin audioPlayWindow;
extern uiWin thumbnallWindow;
extern uiWin slideShowWindow;
extern uiWin dateTime1Window;
extern uiWin usbDevWindow;
extern uiWin takePhotoWindow;
extern uiWin upgradeWindow;
extern uiWin mainMenuWindow;
extern uiWin mainMenu2Window;
extern uiWin mainMenu3Window;
extern uiWin audioPlayerWindow;
extern uiWin CreativeToolsWindow;
extern uiWin playBacEditkWindow;
extern uiWin photoscoreWindow;
extern uiWin idContentWindow;
extern uiWin idContentUniWindow;
extern uiWin RoleSwitchWindow;
extern uiWin SmallModelWindow;
//extern uiWin roleMenuWindow;
extern uiWin ljfNetTypeMenuWindow;
extern uiWin AppWindow;
void uiParentDealMsg(winHandle handle,uint32 parentMsg);
winHandle uiOpenWindow(uiWin* winInfor,uint32 argc,...);
winHandle uiOpenWinAndShow(uiWin* winInfor,uint32 argc,...);
bool windowIsOpen(uiWin* winInfor);


#define winItem         dialogItem
#define   WINDOW(name,msg,widget)        uiWin name={msg,widget,INVALID_HANDLE,false};
#define   MULTIWIN(name,msg,widget)        uiWin name={msg,widget,INVALID_HANDLE,true};



#endif


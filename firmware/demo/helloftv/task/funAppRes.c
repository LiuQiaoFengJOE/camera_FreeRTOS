#include"uiWin.h"

enum
{
    APP_CONNEC_ID,
    //APP_CONNECTWAIT_ID,
    //APP_CONNECTOK_ID,
	MAINMENU_MAX_ID
};
    
enum     
{   
	APP_DEPLOY_FALSE = -1,   
	APP_DEPLOY_CONNECTING = 1,        
    APP_DEPLOY_SUCCESS = 0,          
	APP_DEPLOY_STAT_MAX
};   
  
static widgetCreateInfor AppWin[] =
{

	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
    createStringIcon(APP_CONNEC_ID,      Rx(100), Ry(160), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
    //createStringIcon(APP_CONNECTOK_ID,        Rx(100), Ry(160), Rw(300), Rh(25),strToResId("connect OK!"),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	widgetEnd(),
};
  

static int AppWifiShow(winHandle handle, int stat)
{
	switch(stat)
	{
		case APP_DEPLOY_SUCCESS:
			winSetResid(winItem(handle,APP_CONNEC_ID),R_ID_STR_APP_CONNECTOK);
			break;
		case APP_DEPLOY_CONNECTING:
			winSetResid(winItem(handle,APP_CONNEC_ID),R_ID_STR_APP_CONNECTWAIT);
			break;
		default:
			winSetResid(winItem(handle,APP_CONNEC_ID),R_ID_STR_APP_CONNECTFALSE);
			break;
	}
	winSetVisible(winItem(handle,APP_CONNEC_ID),true);		
	return stat;
}



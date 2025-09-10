#include"task/uiWin.h"

enum
{
	ROLESWITCH_BATERRY_ID,
	ROLESWITCH_NAME,
	ROLESWITCH_ID1,
	ROLESWITCH_ID2,
	ROLESWITCH_ID3,
	ROLESWITCH_MAX_ID
};

static widgetCreateInfor RoleSwitchWin[] = {
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	createRect(ROLESWITCH_ID1,       Rx(12),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(ROLESWITCH_ID2,       Rx(12+100),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(ROLESWITCH_ID3,       Rx(12+100+100),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createStringIcon(ROLESWITCH_NAME,Rx(0), Ry(185), Rw(95),Rh(20),strToResId(R_ID_STR_DOCTOR),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	// createImageIcon(ROLESWITCH_BATERRY_ID,Rx(285), Ry(2), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

	widgetEnd(),
};

static void RoleNamePositionChange(winHandle hWin, int16 target_x, int16 target_y){
    uiRect pos;
    int16 x0, y0, x1, y1;
    pos.x0 = 0;
    pos.x1 = 0;
    pos.y0 = 0;
    pos.y1 = 0;
    winGetPos(hWin, &pos);
    if (pos.x0 == pos.x1)
        return;
    x0 = target_x;
    x1 = x0 + (pos.x1 - pos.x0);
    y0 = target_y;
    y1 = y0 + (pos.y1 - pos.y0);
    winResize(hWin, x0, y0, x1, y1);
}

static void RoleSwitchNameShow(winHandle handle, int show){
	if (show == 0||show == 3||show == 6||show == 9){
		printf("run in RoleSwitchNameShow1\n");
		if (show == 0){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),10,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_DOCTOR));
		}else if(show == 3){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),10,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_FLATTERER));
		}else if(show == 6){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),10,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_NEWTON));
		}else if(show == 9){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),10,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_POET));
		}
	}else if (show == 1||show == 4||show == 7||show == 10){
		printf("run in RoleSwitchNameShow2\n");
		if (show == 1){		
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),110,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_TEACHER));
		}else if(show == 4){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),110,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_SOCRATES));
		}else if(show == 7){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),110,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_DARWIN));
		}
	}else if (show == 2||show == 5||show == 8){
		printf("run in RoleSwitchNameShow3\n");
		if (show == 2){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),210,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_STORY));
		}else if(show == 5){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),210,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_DEBATER));
		}else if(show == 8){
			RoleNamePositionChange(winItem(handle,ROLESWITCH_NAME),210,185);
			winSetResid(winItem(handle,ROLESWITCH_NAME),strToResId(R_ID_STR_CHEF));
		}
	}
}

static void RoleSwitchStrShow(winHandle handle,bool show){
}

static void RoleSwitchBaterryShow(winHandle handle){
}
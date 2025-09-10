#include "app.h"
#include"uiWin.h"

void funcPhotoScoreOpen(uint32 arg)
{
	printf("main menu open\n");

	u32 size;
	u32 *ptr;
	wmm_trans_mode(&workmode, WM_NONE);
	lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
	lcdPreviewStart();
	uiOpenWindow(&photoscoreWindow,0);
}

void funcPhotoScoreClose(uint32 arg)
{
	lcdPreviewStop();
	printf("main menu exit.\n");
}
sysFunction functionPhotoScore=
{
	"Photo Score",
	NULL,
	funcPhotoScoreOpen,
	funcPhotoScoreClose,
};

///////////



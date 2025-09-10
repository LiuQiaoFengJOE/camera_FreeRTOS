#include "app.h"
#include"uiWin.h"


void funcAppOpen(void)
{
	printf("App open\n");
	wmm_trans_mode(&workmode, WM_NONE);
	uiOpenWindow(&AppWindow,0);	
}

void funcAppClose(uint32 arg)
{
	lcdPreviewStop();
	printf("App exit.\n");
}

sysFunction functionApp=
{
	"App",
	NULL,
	funcAppOpen,
	funcAppClose,
};

///////////



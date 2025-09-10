#include "app.h"
#include"uiWin.h"


void funcMainMenu2Open(uint32 arg)
{
	printf("main menu open, %d\n", arg);

	u32 size;
	u32 *ptr;
	wmm_trans_mode(&workmode, WM_NONE);
#if 0
	sysMemFree(get_workmode_heap());
	size=sysMemRemain();
	ptr=sysMemMalloc(size,4);
	workModeMemInit(ptr,size);
#endif	
	uiOpenWindow(&mainMenu2Window,arg);	
}


void funcMainMenu2Close(uint32 arg)
{
	lcdPreviewStop();
	printf("main menu exit.\n");
}
sysFunction functionMainMenu2=
{
	"Main menu",
	NULL,
	funcMainMenu2Open,
	funcMainMenu2Close,
};

///////////



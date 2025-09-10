#include "app.h"
#include"uiWin.h"


void funcMainMenuOpen(uint32 arg)
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
	uiOpenWindow(&mainMenuWindow,arg);	
}


void funcMainMenuClose(uint32 arg)
{
	lcdPreviewStop();
	printf("main menu exit.\n");
}
sysFunction functionMainMenu=
{
	"Main menu",
	NULL,
	funcMainMenuOpen,
	funcMainMenuClose,
};

///////////



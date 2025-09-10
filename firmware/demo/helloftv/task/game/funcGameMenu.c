#include "../app.h"

#if GAME_FUNC_EN

#include "task/uiWin.h"



void funcGameMenuOpen(uint32 arg)
{
	printf("game menu open\n");

//	hal_csiEnable(0);
//	dispLayerSetPIPMode(DISP_PIP_DISABLE);

	uiOpenWindow(&gameMenuWindow,0);
}

void funcGameMenuClose(uint32 arg)
{

	printf("game menu exit.\n");
}
sysFunction functionGameMenu=
{
	"Game menu",
	NULL,
	funcGameMenuOpen,
	funcGameMenuClose,
};
#endif


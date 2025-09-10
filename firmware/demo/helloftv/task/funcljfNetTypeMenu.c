#include "app.h"
#include "task/uiWin.h"

void funcljfNetTypeMenuOpen(uint32 arg)
{
	printf("game menu open\n");
	wmm_trans_mode(&workmode, WM_NONE);
	uiOpenWindow(&ljfNetTypeMenuWindow,0);
}

void funcljfNetTypeMenuClose(uint32 arg)
{

	printf("ljfNetType menu exit.\n");
}
sysFunction functionljfNetTypeMenu=
{
	"ljfNetType menu",
	NULL,
	funcljfNetTypeMenuOpen,
	funcljfNetTypeMenuClose,
};


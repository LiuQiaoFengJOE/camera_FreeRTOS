#include "app.h"
#include "task/uiWin.h"

void funcRoleMenuOpen(uint32 arg)
{
	printf("game menu open\n");

	uiOpenWindow(&roleMenuWindow,0);
}

void funcRoleMenuClose(uint32 arg)
{

	printf("RoleMenu exit.\n");
}
sysFunction functionRoleMenu=
{
	"Role menu",
	NULL,
	funcRoleMenuOpen,
	funcRoleMenuClose,
};


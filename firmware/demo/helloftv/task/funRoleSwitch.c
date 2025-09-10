#include "task/app.h"
#include "task/uiWin.h"

void funcRoleMenuOpen(uint32 arg){
	printf("Role Menu open\n");

	uiOpenWindow(&RoleSwitchWindow,0);
}

void funcRoleMenuClose(uint32 arg){

	printf("Role Menu exit.\n");
}

sysFunction functionRoleMenu={
	"Role Menu",
	NULL,
	funcRoleMenuOpen,
	funcRoleMenuClose,
};


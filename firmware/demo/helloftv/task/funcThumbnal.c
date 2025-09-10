#include "app.h"
#include"uiWin.h"
#include "mediaFile/demuxer.h"
#include "fatfs/fileOps.h"

void funcThumbnalOpen(uint32 arg1)
{
	wmm_trans_mode(&workmode, WM_NONE);
	if(arg1==0)
	{
		fileInforInit(AVI_FILE);
	}
	uiOpenWindow(&thumbnallWindow,0);
}


void funcThumbnalClose(uint32 arg)
{

}
sysFunction functionThumbnal=
{
	"thumbnal",
	NULL,
	funcThumbnalOpen,
	funcThumbnalClose,
};



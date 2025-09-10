#ifndef    _F_NAME_OPS_H
#define    _F_NAME_OPS_H

#if 1
#include"fatfs/fileManage.h"
#else
#include"fatfs/fileManage1.h"
#endif
#define getSdFreeSizeMB()  (getSdFreeSize()>>11)


#endif



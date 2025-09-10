#ifndef  _APPLICATION_H
#define  _APPLICATION_H

#include "task/app.h"
#include "touchPanel.h"

#define nv_open res_open
#define nv_size res_size
#define nv_read res_read

#define hal_sysMemMalloc(a, b) osal_malloc(a)
#define hal_sysMemFree(a) osal_free(a)
#define  deg_Printf printf


#endif



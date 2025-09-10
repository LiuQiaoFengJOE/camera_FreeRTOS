/*------------------------------------------------------------------------
 *  for platfrom port 
 *------------------------------------------------------------------------*/
#include "../inc/zbar_cfg.h"

void *hal_sysMemAlloc(u32 n,u32 s)
{
	 void *m;
	 m=hal_sysMemMalloc(n*s,32);
	 if(m)
	 {
		 memset(m,0,n*s);
		 return m;
	 }
	 else
	 {
		 return NULL;
	 }
}

void *hal_sysMemRealloc(void *ptr,u32 s)
{
	 void *m=NULL;
	 m=hal_sysMemMalloc(s,32);
	 if(m)
	 {
		 memcpy(m,ptr,s);
		 hal_sysMemFree(ptr);
		 return m;
	 }
	 else
	 {
		 deg_Printf("Realloc err\n");
		 return NULL;
	 }
}

/*
int se_snprintf(char *str, size_t size, const char *format, ...)
{
	return 0;
}
*/



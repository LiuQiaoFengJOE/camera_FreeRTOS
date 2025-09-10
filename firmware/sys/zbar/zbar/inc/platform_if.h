/*------------------------------------------------------------------------
 *  for platfrom port 
 *------------------------------------------------------------------------*/
#ifndef _PLATFORM_IF_H_
#define _PLATFORM_IF_H_
//#include "debug.h"
//#include <stdio.h>
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"


#include "sysCommon/commonInclude.h"


#undef assert
#define assert(...)

#undef zassert
#define zassert(...)

#undef zprintf
#define zprintf(...)

#undef dprintf
#define dprintf(...)

#undef dbprintf
#define dbprintf(...)


#undef sprintf
#define sprintf(...)	// not ok

#undef sprintf
#define  deg_Printf(...)  

//#undef snprintf
//#define snprintf	se_snprintf
#undef abort
#define abort()

#define XOSTimeGet xTaskGetTickCount
#undef malloc
#define malloc(n) osal_malloc(n)
#undef free
#define free(n) osal_free(n)

#undef calloc
#define calloc(n,s) osal_calloc(n,s)

#undef realloc
#define realloc(p,n) osal_realloc(p,n)	// not good 


#if 0

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif
#define ZBAR_LOG_LEVEL_ZPRINTF 3

#define zprintf(level, format, ...) do {                              				\
			if(ZBAR_LOG_LEVEL_ZPRINTF <= level) {									\
				printf(format,##__VA_ARGS__);					\
			}																		\
		} while(0)

	  
#define zassert(condition, retval, format, ...) do {   			\
					if(!(condition))								\
						return(retval); 							\
				} while(0)

#define XOSTimeGet xTaskGetTickCount
#define deg_Printf printf
#define dprintf zprintf

#define assert(x)	do { if (!(x)) abort(); } while (0)
#endif
	  
#endif



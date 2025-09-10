/**************************************************************************************************************
 * altobeam RTOS wifi hmac source code 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 *
 *  The source code contains proprietary information of AltoBeam, and shall not be distributed, 
 *  copied, reproduced, or disclosed in whole or in part without prior written permission of AltoBeam.
*****************************************************************************************************************/


#ifndef ATBM_OS_MEM_H
#define ATBM_OS_MEM_H
//#include "sys_MsWrapper_cus_os_mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "osal/os_drv.h"
#include "memory.h"
extern void *atbm_wifi_zalloc(unsigned int size);
//void *atbm_realloc(void *ptr, atbm_size_t size);
#define atbm_kmalloc(x,y) malloc(x)  
#define atbm_kzalloc(x,y) atbm_wifi_zalloc(x)            
#define atbm_kfree(x)   free(x)           
#define atbm_memcpy         memcpy 
#define atbm_memset         memset 
#define atbm_memmove  		memmove
#define atbm_memcmp 		memcmp 
#define atbm_calloc(x,y)	calloc(x,y) //drv_calloc(x,y)
#define atbm_realloc(x,y)   realloc(x,y)

#if 1
static inline void * atbm_realloc_array(void *ptr, size_t nmemb, size_t size)
{
	if (size && nmemb > (~(size_t) 0) / size)
		return NULL;
	return atbm_realloc(ptr, nmemb * size);
}

static inline void * atbm_memdup(const void *src, atbm_size_t len)
{
	void *r = (void *)atbm_kmalloc(len, GFP_KERNEL);

	if (r && src)
		atbm_memcpy(r, src, len);
	return r;
}
#endif

#endif /* ATBM_OS_MEM_H */

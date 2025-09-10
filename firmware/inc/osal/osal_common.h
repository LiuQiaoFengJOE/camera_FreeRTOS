/*****************************************************************************
*    Copyright (c) 2016 AppoTech Corp. All Rights Reserved
*    This source is confidential and is AppoTech's proprietary information.
*    This source is subject to AppoTech License Agreement, and shall not be
	 disclosed to unauthorized individual.
*    File: osal.h
*
*    Description:
*    This header file contains the definitions and declaration about  abstract os layer.
*
*    THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
	  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
	  PARTICULAR PURPOSE.
*****************************************************************************/

#ifndef __OSAL_COMMON_H__
#define __OSAL_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "type.h"

#define __FP__(f) \
	if (f)        \
	{             \
		(*f)();   \
	}

	typedef struct _BUFQDT QDT_BUF;
	struct _BUFQDT
	{
		u8 sta;
		u8 *pbuf;
		u32 buflen;
		struct _BUFQDT *last;
		struct _BUFQDT *next;
	};

	typedef struct
	{
		u8 semf;
		u8 *sf;
		u8 *ef;
		u8 *pcache;
		u32 cachelen;
		u32 cacheuse;
	} CACHE;

	typedef struct
	{
		CACHE c_inf;
		QDT_BUF *p_qdt;
		QDT_BUF *g_qdt;
	} C_STACK;

	u8 char_2_hex(u8 val);
	void c_stack_setup(C_STACK *c, QDT_BUF que[], u32 que_dep, u8 *linkbuf, u32 linksize);
	u8 *cache_pre_malloc(C_STACK *c, u32 mlen);
	bool cache_put(C_STACK *c, u8 *p, u32 len);
	bool cache_get(C_STACK *c, u8 **p, u32 *len);
	bool cache_free(C_STACK *c);

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_COMMON_H__ */

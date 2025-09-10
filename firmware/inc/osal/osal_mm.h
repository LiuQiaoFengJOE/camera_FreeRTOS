/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: osal_mm.h
*
* Description:
* Memory management relative functions definition for Operating System Abstract Layer
*
* Author: CHEN<chen@buildwin.com.cn>
*
* Create date: June. 15, 2016
*
*********************************************************************************
*
* Revision history:
---------------------------------------------------------------------------------
Date                    Modification                 Revision            Name
---------------------------------------------------------------------------------
June. 15, 2016      Created                       V0.1                 CHEN

********************************************************************************/
/**
 * @File osal_mm.h
 * @brief Memory management relative functions definition for Operating System Abstract Layer
 */

#ifndef __OSAL_MM_H__
#define __OSAL_MM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_defs.h>

    // #include <string.h>

#include <memory.h>

#define MALLOC malloc

#define FREE free

#ifdef MEMCPY
#undef MEMCPY
#endif
#define MEMCPY(dest, src, len) memcpy(dest, src, len)

#ifdef MEMCMP
#undef MEMCMP
#endif
#define MEMCMP(buf1, buf2, len) memcmp(buf1, buf2, len)

#ifdef MEMSET
#undef MEMSET
#endif
#define MEMSET(dest, c, len) memset(dest, c, len)

#ifdef MEMMOVE
#undef MEMMOVE
#endif
#define MEMMOVE(dest, src, len) memmove(dest, src, len)

#ifdef STRCPY
#undef STRCPY
#endif
#define STRCPY(dest, src) strcpy(dest, src)

#ifdef STRNCPY
#undef STRNCPY
#endif
#define STRNCPY(dest, src, len) strncpy(dest, src, len)

#ifdef STRCMP
#undef STRCMP
#endif
#define STRCMP(dest, src) strcmp(dest, src)

#ifdef STRLEN
#undef STRLEN
#endif
#define STRLEN(str) strlen(str)

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_MM_H__ */

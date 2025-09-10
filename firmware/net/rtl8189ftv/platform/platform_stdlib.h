/******************************************************************************
 *
 * Copyright(c) 2007 - 2014 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __PLATFORM_STDLIB_H__
#define __PLATFORM_STDLIB_H__

#if defined(CONFIG_PLATFORM_8195A)
#if defined(__IARSTDLIB__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "diag.h"

#define strsep(str, delim) _strsep(str, delim)
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag.h"
#include "strproc.h"
#include "basic_types.h"
#include "hal_misc.h"
#undef strlen
#undef strcmp
#undef atoi
#undef snprintf
#undef sprintf
#undef printf
#undef sscanf
#undef strtok
#undef strsep
#undef strstr
#undef memcmp
#undef memcpy
#undef memset

#define strlen(str) prvStrLen((const unsigned char *)str)
#define strcmp(str1, str2) prvStrCmp((const unsigned char *)str1, (const unsigned char *)str2)
#define atoi(str) prvAtoi(str)

#define snprintf DiagSnPrintf
#define sprintf(fmt, arg...) DiagSPrintf((u8 *)fmt, ##arg)
#define printf DiagPrintf

#define sscanf _sscanf
#define strtok(str, delim) _strsep(str, delim)
#define strsep(str, delim) _strsep(str, delim)
#define strstr(str1, str2) prvStrStr(str1, str2)
#define strcpy(dest, src) _strcpy(dest, src)
#define strncpy(dest, src, count) _strncpy(dest, src, count)
//
#define memcmp(dst, src, sz) _memcmp(dst, src, sz)
//
#define memset(dst, val, sz) my_memset(dst, val, sz)
//
#define memcpy(dst, src, sz) dma_memcpy(dst, src, sz) // my_memcpy(dst, src, sz)
#endif

//
// memory management
//
#define malloc pvPortMalloc
#define free vPortFree

#elif defined(USE_STM322xG_EVAL) || defined(USE_STM324xG_EVAL) || defined(STM32F10X_XL)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#endif

#endif //__PLATFORM_STDLIB_H__

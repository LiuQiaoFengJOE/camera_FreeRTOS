/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2017 Copyright (C)
*
* File name: osal_type.h
*
* Description:
* Timer relative functions definition for Operating System Abstract Layer
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
 * @File type.h
 * @brief Architecture defined type
 */

#ifndef __ASM_TYPE_H__
#define __ASM_TYPE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BYTE
#define BYTE uint8_t
#endif

#ifndef WORD
    typedef unsigned short WORD;
#endif

#ifndef DWORD
    typedef unsigned long DWORD;
#endif

#ifndef QWORD
    typedef unsigned long long QWORD;
#endif

#ifndef BOOL
    typedef int BOOL;
#endif

#ifndef INT8
    typedef signed char INT8;
#endif

#ifndef UINT8
    typedef unsigned char UINT8;
#endif

#ifndef INT16
    typedef short INT16;
#endif

#ifndef UINT16
    typedef unsigned short UINT16;
#endif

#ifndef INT32
    typedef int INT32;
#endif

#ifndef UINT32
    typedef unsigned int UINT32;
#endif

#ifndef UINT64
    typedef unsigned long long UINT64;
#endif

#ifndef INT64
    typedef long long INT64;
#endif

#ifndef WCHAR
    typedef unsigned short WCHAR;
#endif

#ifndef UCHAR
    typedef unsigned char UCHAR;
#endif

#ifndef CHAR
    typedef signed char CHAR;
#endif

#ifndef PUCHAR
    typedef unsigned char *PUCHAR;
#endif

#ifndef SHORT
    typedef short SHORT;
#endif

#ifndef INT
    typedef int INT;
#endif

#ifndef UINT
    typedef unsigned int UINT;
#endif

#ifndef LONG
    typedef long LONG;
#endif

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL ((void *)0)

#ifdef __cplusplus
}
#endif

#endif /* __ASM_TYPE_H__ */

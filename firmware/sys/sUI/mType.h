#ifndef  __MTYPE_H
#define  __MTYPE_H

#include "typedef.h"   
#include "debug.h"
#include "string.h"





#if 0
typedef enum
{
	false=0,
	true=!false
}bool;
#define  NULL          ((void *)0)
#endif

#define _ALIGN(n)

#define  toSmall(n)       ( ((n)>='A'&&(n)<='Z')?((n)-'A'+'a'):(n))
#define  toBig(n)           ( ((n)>='a'&&(n)<='z')?((n)-'a'+'A'):(n))

#define MakeTag(a,b,c,d) ((a)|((b)<<8)|((c)<<16)|((uint32)(d)<<24))

#define  MAX(a,b)       ((a)>(b)?(a):(b))
#define  MIN(a,b)        ((a)>(b)?(b):(a))
#define  CLIP(x,a,b)     MIN(MAX(x,a),b)

#define ARRY_LEN(a)    (sizeof(a)/sizeof(a[0]))

#endif


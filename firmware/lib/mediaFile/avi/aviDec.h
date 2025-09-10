#ifndef  __AVIDEC_H
#define  __AVIDEC_H
#include "avi.h"
#include "../demuxer.h"

#define AVI_SEEK_TYPE_NONE                 0
#define AVI_SEEK_TYPE_ODML                 1
#define AVI_SEEK_TYPE_IDX1                    2
typedef struct _aviMediaDemuxerContext
{
	mediaDemuxerContext  demuxerContext;
	uint8  using;
	uint8 aviSeekType;
	uint32 dwFlags;
	uint32 riffSize;
	uint32 moviListPos;
	uint32 moviListSize;
	uint32 idx1Pos;
	uint32 idx1Sum;
	aviIdx1Entry idx1[AVI_IDX1_NUM];
	uint32 idx1ValidNum;
	
}aviMediaDemuxerContext;


#endif

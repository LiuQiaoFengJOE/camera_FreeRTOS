#ifndef  __WAVDEC_H
#define  __WAVDEC_H
#include "wav.h"
#include "../demuxer.h"


typedef struct _wavMediaDemuxerContext
{
	mediaDemuxerContext  demuxerContext;
	uint8  using;
	uint8 wavSeekType;
	uint32 dwFlags;
	uint32 riffSize;
	uint32 moviListPos;
	uint32 moviListSize;
	uint32 idx1Pos;
	uint32 idx1Sum;
	wavIdx1Entry idx1[1024];
	uint32 idx1ValidNum;
	
}wavMediaDemuxerContext;

#endif

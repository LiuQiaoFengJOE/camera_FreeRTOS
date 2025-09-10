#ifndef  __STREAM_H
#define  __STREAM_H
#include "stream/stream.h"
#include "osal/osal_alloc.h"



#define      SYS_TIME_STAMP_BASE_DEN          1000


typedef struct _mediaCodec
{
	char* name;
}mediaCodec;


typedef struct _mediaStream
{
	mediaType type;
	void  *muxer;
	mediaCodec codec;
	uint8 streamNum;
	uint32 curTime;

	uint32 frameSum;
	uint16 width;
	uint16 height;
	uint16 fps;
	uint16 frameInterval;
	uint16 numChannels;
	uint32 sampleRate;
}mediaStream;
void mediaStreamInit(mediaStream* stream,mediaType type,uint8 streamNum);
void videoStreamInit(mediaStream* stream,uint16 width,uint16 height,uint16 fps);
void audioStreamInit(mediaStream* stream,uint32 sampleRate,uint16 numChannels);
char* mediaCodecGetName(mediaCodec* codec);
char* mediaStreamSetCodecName(mediaStream* stream,char* name);
uint32 videoStreamGetTime(mediaStream* stream);




#endif

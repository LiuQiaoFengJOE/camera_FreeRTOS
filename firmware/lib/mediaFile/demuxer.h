#ifndef  __DEMUXER_H
#define  __DEMUXER_H

#include "sUI/mType.h"
#include "fstream.h"
#include "mediaFile.h"
#include "stream/stream.h"
#include "task/app.h"


#define SUPPORT_AVI_DEMUXERS         2
#define MAX_STREAMS_OF_DEMUXER      2

#define  STREAM_SEEK_BY_FRAME             0
#define  STREAM_SEEK_BY_KEY_FRAME         1
#define  STREAM_SEEK_BY_SIZE              2

typedef struct _mediaDemuxer mediaDemuxer;

typedef struct _mediaDemuxerContext
{
	mediaDemuxer* demuxer;
	uint8 streams;
	mediaStream* stream[MAX_STREAMS_OF_DEMUXER];
	mediaFile file;
}mediaDemuxerContext;

typedef mediaDemuxerContext* (*demuxerReadHeader)(void *fp);
typedef int (*demuxerReadPacket)(mediaDemuxerContext* demuxerContext,struct frame* f,uint8 streamNum,int speed);
typedef uint32 (*demuxerStreamSeek)(mediaDemuxerContext* demuxerContext,uint8 streamNum,uint32 seek,uint8 seekType);
typedef uint32 (*demuxerClosure)(mediaDemuxerContext* demuxerContext);
typedef mediaStream* (*demuxerGetStream)(void);
typedef uint32 (*demuxerGetTotalTime)(mediaStream *stream);

typedef enum 
{
	DEMUXER_AVI=0,
	DEMUXER_JPG,
	DEMUXER_WAV,
	DEMUXER_MAX
}DEMUXER_TYPE;

typedef struct _mediaDemuxer
{
	char* name;
	int type;
	void (*init)(void);
	demuxerReadHeader  readHeader;//mediaDemuxerContext* (*readHeader)(mediaFile* file);
	demuxerReadPacket  readPacket;//void* (*readPacket)(mediaDemuxerContext* demuxerContext,uint8 streamNum);
	demuxerStreamSeek  streamSeek;//uint32 (*streamSeek)(mediaDemuxerContext* demuxerContext,uint8 streamNum,uint32 seek,uint8 seekType);
	demuxerClosure   closure;//uint32 (*closure)(mediaDemuxerContext* demuxerContext);
	demuxerGetStream    getStream;//mediaStream* (*getStream)(void);
	demuxerGetTotalTime getTime;
	struct _mediaDemuxer* next;
}mediaDemuxer;

void mediaDemuxerInit(void);
mediaStream* mediaDemuxerAllocateStream(mediaDemuxerContext* demuxerContext,mediaType type);
mediaStream* mediaDemuxerGetStream(mediaDemuxerContext* demuxerContext,uint8 streamNum);
mediaDemuxerContext* mediaDemuxerCreateContext(char* name);
void mediaDemuxerSeek(mediaDemuxerContext* demuxerContext,uint32 pts);
int mediaDemuxerReadStream(mediaDemuxerContext* demuxerContext,struct frame* f,uint8 streamNum,int speed);
mediaStream* mediaDemuxerGetStreamByType(mediaDemuxerContext* demuxerContext,mediaType type);
void mediaDemuxerClosure(mediaDemuxerContext* demuxerContext);
int mediaStreamRead(mediaStream* stream,struct frame* f,int speed);
u32 mediaDemuxerGetTotalTime(mediaStream* stream);
int mediaDemuxerType(mediaDemuxerContext* demuxerContext);
int mediaDemuxerGetRes(mediaStream* stream,u16 *w,u16* h);
void mediaDemuxerFileLock(mediaDemuxerContext* demuxerContext,u8 lock);
void mediaDemuxerSeekAudio(mediaStream* stream,uint32 pts);



#endif

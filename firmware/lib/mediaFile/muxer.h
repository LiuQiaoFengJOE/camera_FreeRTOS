#ifndef  __MUXER_H
#define  __MUXER_H
#include "sUI/mType.h"
#include "fstream.h"
#include "mediaFile.h"
#include "stream/stream.h"


#if 1//LITTLE_ENDIAN
#define   toLittleEndian16(n)    (n)
#define   toLittleEndian32(n)    (n)
#define   toLittleEndian64(n)    (n)
#else
#define   toLittleEndian16(n)    (((n)&0x00ff)<<8|((n)&0xff00)>>8)
#define   toLittleEndian32(n)    (toLittleEndian16((uint16)(n))<<16|toLittleEndian16((uint16)((n)>>16)))
#define   toLittleEndian64(n)    (toLittleEndian32((uint32)(n))<<32|toLittleEndian32((uint32)((n)>>32)))
#endif
//#define  toSmall(n)       ( ((n)>='A'&&(n)<='Z')?((n)-'A'+'a'):(n))
//#define  toBig(n)           ( ((n)>='a'&&(n)<='z')?((n)-'a'+'A'):(n))

#define MakeLTag(a,b,c,d) ((a)|((b)<<8)|((c)<<16)|((uint32)(d)<<24))/*little-endian*/
#define   MEDIA_TYPE(a1,a2,a3,a4)      ((uint32)(a1)+((uint32)(a2)<<8)+((uint32)(a3)<<16)+((uint32)(a4)<<24))
#define   MEDIA_TYPE_AVI      MEDIA_TYPE('A','V','I',0)
#define   MEDIA_TYPE_JPG      MEDIA_TYPE('J','P','G',0)
#define   MEDIA_TYPE_WAV      MEDIA_TYPE('W','A','V',0)



#define MUXER_BUFF_SIZE      (32*1024)
typedef struct _aviMuxerBuff
{
	uint8   buff[MUXER_BUFF_SIZE];
	uint32     ptr;
	uint32     maxSize;
}muxerBuff;


typedef struct _mediaMuxer
{
	char* name;
	uint8 streams;
	uint32  curTime;
	uint32  lastTime;
	uint32  writePic;
	uint32  reserved;
	mediaStream* stream[MEDIA_MAX];
	mediaFile  file;
	int32  (*writeHeader)(struct _mediaMuxer* muxer);
	uint32  (*writePacket)(struct _mediaMuxer* muxer,struct frame* f);
	int32  (*closure)(struct _mediaMuxer* muxer);
	uint32  (*getSize)(struct _mediaMuxer* muxer);
	void  (*lockFlie)(struct _mediaMuxer* muxer,u8 lock);
	mediaStream* (*getStream)(void);
}mediaMuxer;

#define MUXER_SIZE        sizeof(mediaMuxer)

mediaMuxer* mediaMuxerAllocate(char* name);
mediaStream* mediaMuxerAllocateStream(mediaMuxer* muxer,mediaType type);
mediaStream* mediaMuxerGetStream(mediaMuxer* muxer,uint8 streamNum);
void mediaMuxerWriteHeader(mediaMuxer* muxer);
void mediaMuxerClosure(mediaMuxer* muxer);
uint32 mediaMuxerGetCurSize(mediaMuxer* muxer);
void mediaStreamWritePacket(mediaStream* stream,struct frame* f);
void mediaMuxerLockFlie(mediaMuxer* muxer,u8 lock);
void mediaMuxerSlowRecSet(u8 slow);
u8 mediaMuxerSlowRecGet(void);


void muxerBuffInit(muxerBuff* buff,uint32 size);
void muxerBuffSyn(muxerBuff* buff,mediaFile* file,uint32 size);
void muxerBuffRead(muxerBuff* buff,mediaFile* file,uint32 size);
uint32 muxerBuffWrite(muxerBuff* buff,uint8* data,uint32 size);
uint32 muxerBuffWrite8(muxerBuff* buff,uint8 byte);
uint32 muxerBuffWriteL16(muxerBuff* buff,uint16 halfWord);
uint32 muxerBuffWriteL32(muxerBuff* buff,uint32 word);
uint32 muxerBuffWriteL64(muxerBuff* buff,uint64 word);
uint32 muxerBuffWriteLZeroWords(muxerBuff* buff,uint32 numWords);
uint32 muxerBuffWrite4ByteString(muxerBuff* buff,char const* str);
void muxerBuffSetL32(muxerBuff* buff,uint64 buffPosition, uint32 data);
uint32 tellBuff(muxerBuff* buff);
uint32 seekBuff(muxerBuff* buff,uint32 offset);
uint32 skipBuff(muxerBuff* buff,uint32 offset);




#endif

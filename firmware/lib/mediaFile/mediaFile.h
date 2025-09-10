#ifndef __MEDIAFILE_H
#define  __MEDIAFILE_H
#include "sUI/mType.h"


#define MEDIA_FILE_NUM       10

typedef struct _mediaFile
{
	void* fp;
	uint32  size;
}mediaFile;


#define MEDIA_FILE_SIZE          sizeof(mediaFile)

#define MEDIA_FILE_WRITE        (1<<0)
#define MEDIA_FILE_READ          (1<<1)
#if 1

uint32 mediaFileWrite(mediaFile* file,uint8* data,uint32 size);
uint32 mediaFileRead(mediaFile* file,uint8* data,uint32 size);
int32 mediaFileClose(mediaFile* file);
void* mediaFileCreate(mediaFile* file,char* name,uint32 mode);
uint32 mediaFileWrite8(mediaFile* file,uint8 byte);
uint32 mediaFileWriteL16(mediaFile* file,uint16 halfWord);
uint32 mediaFileWriteL32(mediaFile* file,uint32 word);
uint32 mediaFileWriteL64(mediaFile* file,uint64 word);
uint32 mediaFileWriteLZeroWords(mediaFile* file,uint32 numWords);
uint32 mediaFileWrite4ByteString(mediaFile* file,char const* str);
void mediaFileSetL32(mediaFile* file,uint64 filePosition, uint32 data);
uint32 fileSize(mediaFile* file);
uint32 tellFile(mediaFile* file);
uint32 seekFile(mediaFile* file,uint32 offset);
uint32 skipFile(mediaFile* file,uint32 offset);
uint8 mediaFileRead8(mediaFile* file);
uint16 mediaFileReadL16(mediaFile* file);
uint32 mediaFileReadL24(mediaFile* file);
uint32 mediaFileReadL32(mediaFile* file);
uint64 mediaFileReadL64(mediaFile* file);
void mediaFileLock(mediaFile* file,u8 lock);


#endif


#endif

#ifndef __AVI_H
#define  __AVI_H
#include "../muxer.h"

#define AVI_MAX_RIFF_SIZE       0x80000000LL

#define AVI_IDX1_NUM     (1024)
typedef struct _aviIdx1Entry
{
      uint32   dwChunkId;
      uint32   dwFlags;
      uint32   dwOffset;
      uint32   dwSize;
  }aviIdx1Entry;
#define AVI_IDX1_SIZE      sizeof(aviIdx1Entry)

#define AVI_SUPER_INDEX_NUM   256
#define AVI_SUPER_INDEX_SIZE      (8+2+1+1+4+4+4*3+16*AVI_SUPER_INDEX_NUM)
typedef struct _aviSuperIndex
{
	uint64 qwOffset;
	uint32 dwSize;
	uint32 dwDuration;
}aviSuperIndex;

typedef struct _aviStdIndex
{
	uint32 dwOffset;
	uint32 dwSize;
}aviStdIndex;
//#define AVI_STD_INDEX_SIZE      (1024*50)
#define AVI_STD_INDEX_NUM     (2000)

typedef struct _aviMediaStream
{
	mediaStream stream;
	uint32 aviChunkId;
	uint32 fAVIScale;
	uint32 fAVIRate;
	uint32 fAVISize;
	uint32 fAVICodecHandlerType;
	uint16 fWAVCodecTag;
	uint32 fHFrameCountPosition;
	uint32 frameSum;
	uint32 sIdxframeSum;
	uint32 streamSize;
	uint32 maxFrameSize;
	aviSuperIndex superIndex[AVI_SUPER_INDEX_NUM];
	uint32  superIndexKeyFrame[AVI_SUPER_INDEX_NUM];
	uint32  superIndexSize[AVI_SUPER_INDEX_NUM];
	uint32  superIndexSum;
	uint32  superIndexPosition;
	aviStdIndex stdIndex[AVI_STD_INDEX_NUM];
	uint32  stdIndexSum;
	aviStdIndex lastStdIndex;
	
	uint8  streamSeekType;
	uint32 stdIndexBuffFrameNumStart;
	uint32 stdIndexBuffFrameSizeStart;
	uint32 stdIndexBuffFrameSizeEnd;
	uint32 stdIndexBuffKeyFrameNumStart;
	uint32 stdIndexBuffKeyFrameSum;
	uint32 idx1End;
	int readPos;
	uint32 lastReadAddr;

	uint32 moviListPos;
	uint8   using;
}aviMediaStream;



typedef struct _aviMediaMuxer
{
	mediaMuxer muxer;
	uint32 fRIFFSizePosition;
	uint32 fAVIHMaxBytesPerSecondPosition;  /* XXX: not quite exact */
	uint32 fAVIHFrameCountPosition;
	uint32 fMoviSizePosition;
	uint32 fOdmlPosition;
	muxerBuff  muxBuff;
	uint32  nextLba;
	uint8   using;
}aviMediaMuxer;









mediaMuxer* aviMuxerGet(char* name);

#endif

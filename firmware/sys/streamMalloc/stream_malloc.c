#include "stream_malloc.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

static char *StreamMemoryPtr = NULL; // 可用空间指针buf头
static unsigned int stream_memory_space_size = 0;

static char *StreamMemoryHead = NULL; // 传入的空间buf头(原始buf)
static unsigned int StreamMemoryHeadSize = 0;

/***********************************************************************
一个内存申请函数,暂定提供给流使用,暂时不要在其他地方调用
特点:
1、以64字节对齐,因此申请的大小会自动扩大到64的倍数
2、不提供释放,因此这种空间申请适用在某个流模式中,当退出流时,整个空间由
		流机制将整个空间回收
3、这个提供的机制不是频繁去申请释放,而是提供给一些任务需要固定大的空间buf,
	 一般是在某个任务中申请后就不需要改动的
*************************************************************************/

/**********************************************************
arg: buf:用于申请总空间的buf
		 total_space_size:buf可用的大小
**********************************************************/
void stream_Malloc_space_init(void *buf, unsigned int total_space_size)
{
	void *src = buf;
	if (!buf || !total_space_size)
	{
		StreamMemoryHead = NULL;
		StreamMemoryPtr = NULL;
		stream_memory_space_size = 0;
		StreamMemoryHeadSize = 0;
		printf("%s buf is NULL:%d\n", __FUNCTION__, total_space_size);
		return;
	}
	StreamMemoryHead = buf;
	if ((unsigned int)buf % 64)
	{
		buf = ((unsigned int)src + 0x40) & ~(0x3f);
	}
	if ((unsigned int)buf - (unsigned int)src > total_space_size)
	{
		StreamMemoryHead = NULL;
		StreamMemoryPtr = NULL;
		stream_memory_space_size = 0;
		StreamMemoryHeadSize = 0;
		return;
	}
	StreamMemoryHeadSize = total_space_size;
	StreamMemoryPtr = buf;
	stream_memory_space_size = total_space_size - ((unsigned int)buf - (unsigned int)src);
	printf("%s src:%X\t%d\n", __FUNCTION__, src, total_space_size);
	printf("%s buf:%X\t%d\n", __FUNCTION__, StreamMemoryPtr, stream_memory_space_size);
}

/******************************************************
空间重新初始化,使用的buf以及size都是原来的
*******************************************************/
void stream_Malloc_space_init_again()
{
	stream_Malloc_space_init(StreamMemoryHead, StreamMemoryHeadSize);
}
/************************************************************************************
整个空间释放,前提是stream_malloc申请的空间没有任何任务调用,否则这里释放后有可能导致
stream_malloc申请的空间数据错乱
1、如果不需要释放,则传入参数为NULL,然后相关数据会被清空
2、这里传入的streamFree主要是为了如果不是流空间的话,需要主动去释放空间
*************************************************************************************/
void stream_all_space_free(Streamfree streamFree)
{
	if (streamFree)
	{
		streamFree(StreamMemoryHead);
	}
	StreamMemoryHead = NULL;
	StreamMemoryPtr = NULL;
	stream_memory_space_size = 0;
	StreamMemoryHeadSize = 0;
}

void *stream_malloc(unsigned int size)
{
	unsigned int malloc_size = size;
	char *returnPtr = StreamMemoryPtr;
	// 如果不是64byte对齐,则malloc_size进行64byte对齐
	if (size % 64)
	{
		malloc_size = (size + 0x40) & (~0x3f);
	}
	if (stream_memory_space_size < malloc_size)
	{
		printf("err remain:%d\t want to malloc:%d,err space:%d\n", stream_memory_space_size, malloc_size, malloc_size - stream_memory_space_size);
		return NULL;
	}
	StreamMemoryPtr += malloc_size;
	stream_memory_space_size -= malloc_size;
	printf("%s:%X\t%d\t%d\n", __FUNCTION__, StreamMemoryPtr, malloc_size, stream_memory_space_size);
	// 返回当前内存头
	printf("Stream space:%X\n", StreamMemoryPtr - returnPtr);
	return (void *)returnPtr;
}

// 暂定没有释放空间的函数
void stream_free(void *buf)
{
}

#ifndef _PCM_PLAY_FIFO_H_
#define _PCM_PLAY_FIFO_H_

#include "typedef.h"


#define MP3_PCM_FIFO_NUM 64

typedef struct _PCM_DATA_S
{
	u8 *buf;
	u32 len;
}pcm_data_t;

typedef struct _PCM_FIFO_S
{
    u16 max;
    u16 cnt;    
    u16 oput;
    u16 iput;
    pcm_data_t data[MP3_PCM_FIFO_NUM];
	pcm_data_t mute;
}pcm_info_t;

//初始化FIFO队列
pcm_info_t*  pcm_fifo_init(u16 pcm_fifo_num, u16 pcm_fifo_size);

//反初始化FIFO队列
void pcm_fifo_uninit(pcm_info_t* pcm_info);


//判断队列是否满
//==ret : 0: not full, 1: fifo full==
u8 pcm_fifo_is_full(pcm_info_t* pcm_info);

//获取队列数量
u16 pcm_fifo_get_cnt(pcm_info_t* pcm_info);


//数据入列
//fifo push 分为两部分，先start获取buf, 赋值完成后，finish设定长度
//这种方式可以避免多一个临时buf,及一次memcpy，非常适合对内容逐个赋值的情况
//==ret: 0:ok,  1, fifo full
u8 pcm_fifo_push_start(pcm_info_t* pcm_info, u32 *buf);
u8 pcm_fifo_push_finish(pcm_info_t* pcm_info, u16 len);

//数据出列
//==ret: 0:ok , 1: fifo null==
u8 pcm_fifo_pop(pcm_info_t* pcm_info, u32 *buf,u32 *len);

//重置缓冲区队列
void pcm_fifo_clear(pcm_info_t* pcm_info);

#if 1

typedef enum
{
	PCM_STATUS_STOP,
	PCM_STATUS_PLAY,
	PCM_STATUS_PAUSE,
}pcm_status_e;

//获取播放状态
pcm_status_e pcm_play_get_status();

//开始播放
int pcm_play_start(pcm_info_t* pcm_info, u16 volume, u16 samplerate);

//暂停播放
int pcm_play_pause();

//停止播放
int pcm_play_stop();
#endif

#endif

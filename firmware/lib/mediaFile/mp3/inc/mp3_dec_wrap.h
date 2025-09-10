#ifndef _MP3_DEC_WRAP_H_
#define _MP3_DEC_WRAP_H_

//初始化
int MP3_init();

//反初始化
int MP3_uninit();


//播放当前文件列表选择项目
int MP3_play();

//停止播放
int MP3_stop();

//获取播放状态//1, 播放中 //0, 停止
int MP3_is_playing();

//获取播放总时长
u32 MP3_get_totaltime();

//获取当前播放时间
u32 MP3_get_playtime();

//获取当前音量
int MP3_get_volume();

//增加音量,返回当前音量
int MP3_volume_add();

//降低音量,返回当前音量
int MP3_volume_dec();


#endif


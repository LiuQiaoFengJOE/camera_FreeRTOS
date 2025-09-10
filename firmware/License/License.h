#ifndef _LICENSE_H_
#define _LICENSE_H_

#include <stdint.h>
#include <stdbool.h>

//新的服务器获取字段用户ID
extern const char* userID;

//授权ID存放数组
extern char authIdBuffer[32];

//获取授权ID函数
//调用说明：建议将此函数放置在demo/helloftv/task/wifiProc.c当中
//当WIFI连接成功并获取到IP之后执行这个函数获取到内容授权ID，此ID有效期最长24小时，设定为每次开机获取一次
//拼接uthIdBuffer[32]的内容到服务器发送字段当中，新的服务器字段参考\sys\mediaFile\picIdentify.c
void GetAuthId(void);

//获取设备ID函数
char *getLicenseDevId(void);

//USB MSC指令函数
//放置在data in当中
void ACHandlerFunction(void);
void BCHandlerFunction(void);
void DCHandlerFunction(void);
void FCHandlerFunction(void);
//放置在data out当中
bool ADHandlerFunction(void);
bool ECHandlerFunction(void);

#endif /* LICENSE_H */
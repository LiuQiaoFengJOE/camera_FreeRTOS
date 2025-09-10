#ifndef __EVENTTIMER_H
#define __EVENTTIMER_H
#include "typedef.h"				 //
#include "sysCommon/commonInclude.h" //  LOW  HIGH

struct timerList;
struct timerCommon;
void event_timerList_thread_LOW(void *arg);
void event_timerList_thread_HIGH(void *arg);

// timer:-1 不定时	>0定时时间
// nowTimer:	记录已经定时的时间
struct timerCommon
{
	int timer;	  // 需要定时的时间
	int nowTimer; // 现在已经定时的时间
	unsigned int startTimer_count;
	unsigned int nowTimer_count;
	union // 倒计时保存变量
	{
		int countdownI;
		float countdownF;
	} countdown;
	void *d;
};
typedef void (*timer_callback)(void *d);
// 软定时的结构体
// func是回调函数
// common公共参数,例如定时时间等
// d	私有结构
struct timerList
{
	struct timerList *next;
	struct timerList *head;
	struct timerCommon *common;
	void *d;
	timer_callback timerEvent;
	timer_callback reponseEvent;
};

void event_timerList_thread(void *arg);
void add_event_timerList(u8 which, timer_callback timerEvent, timer_callback reponseEvent, void *d, int timer);
extern void default_event_timer_LOW();

#endif

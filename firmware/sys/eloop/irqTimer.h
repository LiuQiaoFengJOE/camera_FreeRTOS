/****************************************************************************
**     **                        X OS
 **   **            THE APPOTECH X OPERATION SYSTEM
  ** **                          X WORK QUEUE
   *** 
   ***               (C) COPYRIGHT 2016 BUILDWIN 
  ** **                   
 **   **     BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**     ** 
* File Name   : xwork.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 12/08/2016
* Description : 
*                 
*                 
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  XWORK_H
#define  XWORK_H

#include "typedef.h"


typedef struct X_Work_S
{
	int counter;
	u32 reload;
	void *arg;
	void (*func)(void* arg);
}IRQ_TIMER;

void irqTimerModTime(IRQ_TIMER *timer,u32 msec);
void irqTimerModFunc(IRQ_TIMER *xwork,u32 msec,void (*func)(void *arg),void* arg);
void irqTimerInit(void);
IRQ_TIMER *irqTimerCreate(u32 msec,void (*func)(void *arg),void* arg);
int irqTimerDestory(IRQ_TIMER *xwork);
void irqTimerService(void);


#endif


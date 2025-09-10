#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"

extern UINT32 APB_CLK; // defined by board
#define  R_TICK_CTRL         TTCTL
#define  R_TICK_COUNTER      TTCNT
#define  R_TICK_PRLOAD       TTPR
s32 ax32xx_timerTickStart(void)
{
	R_TICK_CTRL = 0;
	R_TICK_PRLOAD  = 0xffffffff;
	R_TICK_COUNTER = 0;
	
	R_TICK_CTRL = (1<<6)|
			(0<<5)|
			(0<<4)|
			(2<<0);
	return 0;
}

u32 ax32xx_timerTickCount(void)
{
	return R_TICK_COUNTER;
}
/**
 * @brief init timer0
 * @param psr [in] timer prescaler
 * @param pr  [in] timer period
 */

void timer0_init(UINT32 baud)
{
  UINT32 psr, pr;
  UINT32 clk_per_tick = APB_CLK / baud;

  psr = 16 - __builtin_clz(clk_per_tick);
  psr &= 7;
  pr = clk_per_tick >> psr;
  TMR0CON = (psr << 11) | (1 << 3) | (1 << 0);
  TMR0PR = pr;
  TMR0CNT = 0;
}

void timer0_clrpnd(void)
{
  TMR0CPND = BIT(5);
}

void timer1_init(UINT32 baud)
{
  UINT32 psr, pr;
  UINT32 clk_per_tick = APB_CLK / baud;

  psr = 16 - __builtin_clz(clk_per_tick);
  psr &= 7;
  pr = clk_per_tick >> psr;
  TMR1CON = (psr << 11) | (1 << 3) | (1 << 0);
  TMR1PR = pr;
  TMR1CNT = 0;
}

void timer1_clrpnd(void)
{
  TMR1CPND = BIT(5);
}

void timer2_init(UINT32 baud)
{
  UINT32 psr, pr;
  UINT32 clk_per_tick = APB_CLK / baud;

  psr = 16 - __builtin_clz(clk_per_tick);
  psr &= 7;
  pr = clk_per_tick >> psr;
  TMR2CON = (psr << 11) | (1 << 3) | (1 << 0);
  TMR2PR = pr;
  TMR2CNT = 0;
}

void timer2_clrpnd(void)
{
  TMR2CPND = BIT(5);
}

void timer2Start(void)
{
  #if 0
  TMR2PR = 0xffff;
  TMR2CNT = 0;
  TMR2CON = (7 << 11) | (1 << 3) | (1 << 0);
  #else
  ax32xx_timerTickStart();
  #endif
}
u32 timer2Cnt(void)
{
  #if 0
	return (TMR2CNT&0xffff);
  #else
  return ax32xx_timerTickCount();
  #endif
}


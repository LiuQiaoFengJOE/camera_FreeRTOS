#include <stdarg.h>

#include "typedef.h"
// #include "board_config.h"

#include "debug.h"
//void timer_work();
extern void ax32xx_wdtClear(void);
#if SUPPORT_RF_RECV
void timer_work();
#endif
int os_ticktimer_isr(uint32_t irq, void (*clr_pnd)(void), uint32_t *regs)
{
  clr_pnd();
  irqTimerService();
  //#if SUPPORT_RF_RECV
 //timer_work?¨®¨º?uart rx¨ºy?Y 3?¨ª?
 #if (!FINSH_RX_ENABLE)
  timer_work();
 #endif
  //#endif
  ax32xx_wdtClear();

  /* Increment the RTOS tick count, then look for the hightest priority
   * task that is ready to run. */
  if (xTaskIncrementTick())
    ;
  vTaskSwitchContext();
}

// void os_ticktimer_init(void)
//{
//   tick_timer_init( 1000 );
//   //SetIntSrc(_TMR0_INT_);
// }

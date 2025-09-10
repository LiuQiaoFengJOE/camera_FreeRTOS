#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"

void wdt_disable(void)
{
  WDTCON &= 0xFF0FF;
  WDTCON |= 0X00500;
}

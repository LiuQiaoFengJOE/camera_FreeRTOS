#include <stdio.h>
#if (PLATFORM == JIANRONG_RTOS_3268)
//#include "usr_cfg.h"
#endif
//#include "stddef.h"


#define __INLINE inline

#define  atbm_random()   rand()

#define synchronize_rcu()
#define  rcu_read_lock()
#define  rcu_read_unlock()
#define  TargetUsb_lmac_start()

atbm_uint32 atbm_os_random();
#define ZEROSIZE 0

char *atbm_strdup(const char *s);
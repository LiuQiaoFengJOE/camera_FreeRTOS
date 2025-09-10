#ifndef _ATOMIC_H_
#define _ATOMIC_H_
#include "api.h"

#include "os_cpu.h"

extern unsigned int os_int_nesting;
#define in_interrupt() (os_int_nesting)

#define atomic_t_ atomic

#define OS_CRITICAL_METHOD 3

typedef struct _atomic
{
	volatile unsigned char val;
} atomic;

#if 0
static inline void enter_critical()
{
	OSSchedLock();
}

static inline void exit_critical()
{
	OSSchedUnlock();
}
#endif

int atomic_test_set(atomic *at, int val);

#if 0
void atomic_set(atomic *at, int val);
int atomic_read(volatile atomic_t_ *v);
void atomic_add(int i, volatile atomic_t_ *v);
void atomic_sub(int i, volatile atomic_t_ *v);
int atomic_add_return(int i, atomic_t_ *v);
int  atomic_sub_return(int i, atomic_t_ *v);
#endif

#endif

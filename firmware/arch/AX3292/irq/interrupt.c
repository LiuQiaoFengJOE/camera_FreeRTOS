#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"

extern unsigned int except_vector[];

typedef int (*isr_func)(unsigned int, void *, void *);

isr_func irq_vector_table[33];
void *irq_dev_table[33];

unsigned int os_int_nesting = 0;

int unknown_interrupt(unsigned int irqno, void *d, uint32_t *regs)
{
  printf("unknown interrupt %d occured!\n", irqno);
}

#if 0
static void delay_1ms(u16 n)
{
    
	volatile u32 i=1000*n;
	while(i--)
	{
		//asm("nop");
	}
}
#endif

void irq_init(void)
{
  int i;

  arch_local_irq_disable(); // todo: shoule be local_irq_disable
  mtspr(SPR_PICMR, 0);
  mtspr(SPR_EVBA, except_vector);

#if 0
  PFBAUD = 0xa;    //Baud rate = system clock / (PFBAUD + 1 - 3), 3US<(1/ Baud rate)<5US
  PFCON = 9<<16;	//write encrypt release
  PFCON |= BIT(1);	//write mode
  LDOCON |= BIT(28); //enable vref 12V
  PFCON |= BIT(2);	//AVDDEN enable
  delay_1ms(2);	  //delay more than 1.2us
  PFCON |= BIT(0);	//cs enable
  
  PFDATA = 0x98765432; 
  while(!(PFCON&BIT(3)));
  PFDATA = 0x1d000000;
  while(!(PFCON&BIT(3)));
  PFDATA = 0xabcdef01;
  while(!(PFCON&BIT(3)));
  
  PFCON &= ~BIT(0);  //cs disable
  delay_1ms(2);	  //delay more than 1.2us
  PFCON &= ~BIT(2);  //AVDDEN disable
  PFCON &= ~BIT(1);  //read mode
#endif
  for (i = 0; i < 33; ++i)
    irq_vector_table[i] = unknown_interrupt;
}

void irq_register(uint32_t irq, uint32_t prio, isr_func isr, void *dev_id)
{
  irq_vector_table[irq] = isr;
  irq_dev_table[irq] = dev_id;
  //printf("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, irq, isr);

  if (irq < 32)
  {
    if (prio)
      mtspr(SPR_PICPR, mfspr(SPR_PICPR) | (1 << irq));
    else
      mtspr(SPR_PICPR, mfspr(SPR_PICPR) & ~(1 << irq));

    mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (1 << irq));
  }
}

void irq_unregister(uint32_t irq)
{
  if (irq < 32)
    mtspr(SPR_PICMR, mfspr(SPR_PICMR) & ~(1 << irq));
  irq_vector_table[irq] = NULL;
  irq_dev_table[irq] = NULL;
}

void enable_irq(uint32_t irq)
{
  mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (1 << irq));
}

void disable_irq(uint32_t irq)
{
  mtspr(SPR_PICMR, mfspr(SPR_PICMR) & ~(1 << irq));
}

u32 close_spr()
{

  u32 value = mfspr(SPR_PICMR);

  mtspr(SPR_PICMR, 1);
  return value;
}

u32 open_spr(u32 value)
{

  mtspr(SPR_PICMR, value);
  return value;
}

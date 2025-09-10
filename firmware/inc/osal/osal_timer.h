#ifndef __OSAL_TIMER_H__
#define __OSAL_TIMER_H__

/******************************************************************************/
/* Linux Timer */
struct timer_list
{
  void (*function)(unsigned long);
  unsigned long data;
  void *timer;
};

extern void osal_init_timer(struct timer_list *timer);
extern int osal_del_timer(struct timer_list *timer);
extern int osal_mod_timer(struct timer_list *timer, unsigned long expires);

#endif /* __OSAL_TIMER_H__ */

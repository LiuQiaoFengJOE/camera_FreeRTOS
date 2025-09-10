#ifndef __OSAL_TASKLET_H__
#define __OSAL_TASKLET_H__

struct tasklet_struct
{
  struct tasklet_struct *next;
  void (*func)(unsigned long);
  unsigned long data;
};

void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data);
void tasklet_schedule(struct tasklet_struct *t);
void tasklet_kill(struct tasklet_struct *t);

#define DECLARE_TASKLET(name, func, data) \
  struct tasklet_struct name = {NULL, func, data}

#define DECLARE_TASKLET_DISABLED(name, func, data) \
  struct tasklet_struct name = {NULL, func, data}

#endif /* __OSAL_TASKLET_H__ */

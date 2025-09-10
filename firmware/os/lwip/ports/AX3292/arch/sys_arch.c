#include "sys_arch.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdlib.h"

__attribute__((weak)) int errno;

// todo: define random
unsigned int LWIP_RAND() { return rand(); }
// #define LWIP_RAND random

/******************************************************************************/

void sys_init(void)
{
  printf("lwip %s\n", __FUNCTION__);
}

u32_t sys_jiffies(void)
{
  return xTaskGetTickCount();
}

/* Returns the current time in milliseconds */
u32_t sys_now(void)
{
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
  //  return xTaskGetTickCount() / (portTICK_PERIOD_MS * 10);
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  err_t xReturn = ERR_MEM;

  if (sem == NULL)
    printf("%s NULL pointer\n", __FUNCTION__);
  //  printf("%s %x %d\n", __FUNCTION__, sem, count);

  vSemaphoreCreateBinary((*sem));
  //*sem = xSemaphoreCreateBinary();
  //  printf("\xA1\xBE%s\xA1\xBF%s %x\n", pcTaskGetName(NULL), __FUNCTION__, *sem);

  if (*sem != NULL)
  {
    if (count == 0)
      xSemaphoreTake(*sem, 1);
    xReturn = ERR_OK;
  }
  else
    printf("%s ERROR\n", __FUNCTION__);

  return xReturn;
}

int sys_sem_valid(sys_sem_t *sem)
{
  return sem != NULL && *sem != NULL;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
  if (sem)
    *sem = NULL;
  else
    printf("%s NULL pointer\n", __FUNCTION__);
}

void sys_sem_free(sys_sem_t *sem)
{
  // printf ("%s %X\r\n", __FUNCTION__, *sem);
  if (sys_sem_valid(sem))
    vQueueDelete(*sem);
  else
    printf("%s NULL pointer\n", __FUNCTION__);
}

void sys_sem_signal(sys_sem_t *sem)
{
  // printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *sem);
  if (sys_sem_valid(sem))
    xSemaphoreGive(*sem);
  else
    printf("%s NULL pointer\n", __FUNCTION__);
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  TickType_t xStartTime, xEndTime, xElapsed;
  u32_t ulReturn;

  if (!sys_sem_valid(sem))
    printf("%s NULL pointer\n", __FUNCTION__);

  xStartTime = xTaskGetTickCount();

  if (timeout)
  {
    if (xSemaphoreTake(*sem, timeout / portTICK_PERIOD_MS) == pdTRUE)
    {
      xEndTime = xTaskGetTickCount();
      xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;
      ulReturn = xElapsed;
    }
    else
      ulReturn = SYS_ARCH_TIMEOUT;
  }
  else
  {
    xSemaphoreTake(*sem, portMAX_DELAY);
    xEndTime = xTaskGetTickCount();
    xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

    if (xElapsed == 0UL)
      xElapsed = 1UL;

    ulReturn = xElapsed;
  }
  // printf ("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *sem);
  return ulReturn;
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  err_t xReturn = ERR_MEM;

  if (mbox == NULL)
    printf("%s NULL pointer\n", __FUNCTION__);

  *mbox = xQueueCreate(size, sizeof(void *));
  printf("\xA1\xBE%s\xA1\xBF%s %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *mbox);

  if (*mbox)
    xReturn = ERR_OK;
  else
    printf("%S ERROR\n", __FUNCTION__);

  return xReturn;
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
  return mbox != NULL && *mbox != NULL;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
  if (mbox)
    *mbox = NULL;
  else
    printf("%s NULL pointer\n", __FUNCTION__);
}

void sys_mbox_free(sys_mbox_t *mbox)
{
  u32_t ulMessagesWaiting;

  if (!sys_mbox_valid(mbox))
    printf("%s NULL pointer\n", __FUNCTION__);

  ulMessagesWaiting = uxQueueMessagesWaiting(*mbox);
  configASSERT((ulMessagesWaiting == 0));

  vQueueDelete(*mbox);
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  //  printf ("\xA1\xBE%s\xA1\xBF%s %X %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *mbox, msg);
  if (sys_mbox_valid(mbox))
    xQueueSendToBack(*mbox, &msg, portMAX_DELAY);
  else
    printf("%s NULL pointer\n", __FUNCTION__);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  err_t xReturn;

  if (!sys_mbox_valid(mbox))
    printf("%s NULL pointer\n", __FUNCTION__);

  //  printf ("%s %X %X SP=%X\r\n", __FUNCTION__, *mbox, msg, ({uint32_t sp;asm("l.ori %0,r1,0":"=r"(sp));sp;}));
  //  printf ("\xA1\xBE%s\xA1\xBF%s %X %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *mbox, msg);
  if (pdPASS == xQueueSend(*mbox, &msg, 0UL)) // 0UL ticks to wait
    xReturn = ERR_OK;
  else
    xReturn = ERR_MEM;
  //  printf ("\xA1\xBE%s\xA1\xBF%s PICMR = %X\r\n", pcTaskGetName(NULL), __FUNCTION__, get_PICMR());

  return xReturn;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
  void *pvDummy;
  TickType_t xStartTime, xEndTime, xElapsed;
  u32_t ulReturn;

  if (!sys_mbox_valid(mbox))
    printf("%s NULL pointer\n", __FUNCTION__);

  //  if (timeout==0 || timeout==~0)
  //    printf ("\xA1\xBE%s\xA1\xBF%s timeout %X\r\n", pcTaskGetName(NULL), __FUNCTION__, timeout);

  xStartTime = xTaskGetTickCount();

  if (NULL == msg)
    msg = &pvDummy;

  if (timeout != 0UL)
  {
    if (pdTRUE == xQueueReceive(*mbox, msg, timeout / portTICK_PERIOD_MS))
    {
      xEndTime = xTaskGetTickCount();
      xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

      ulReturn = xElapsed;
    }
    else
    {
      *msg = NULL;
      ulReturn = SYS_ARCH_TIMEOUT;
    }
  }
  else
  {
    xQueueReceive(*mbox, msg, portMAX_DELAY);
    xEndTime = xTaskGetTickCount();
    xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

    if (xElapsed == 0UL)
      xElapsed = 1UL;

    ulReturn = xElapsed;
  }
  //  if (*msg)
  //    printf ("\xA1\xBE%s\xA1\xBF%s %X %X\r\n", pcTaskGetName(NULL), __FUNCTION__, *mbox, *(uint32_t*)msg);
  //  else
  //    printf ("\xA1\xBE%s\xA1\xBF%s %X timeout\r\n", pcTaskGetName(NULL), __FUNCTION__, *mbox);
  //  printf ("%s %X %X SP=%X\r\n", __FUNCTION__, *mbox, msg, ({uint32_t sp;asm("l.ori %0,r1,0":"=r"(sp));sp;}));

  return ulReturn;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  void *pvDummy;
  u32_t ulReturn;

  if (!sys_mbox_valid(mbox))
    printf("%s NULL pointer\n", __FUNCTION__);

  if (msg == NULL)
    msg = &pvDummy;

  if (pdTRUE == xQueueReceive(*mbox, &(*msg), 0UL)) // 0UL ticks to wait
    ulReturn = ERR_OK;
  else
    ulReturn = SYS_MBOX_EMPTY;

  return ulReturn;
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
  TaskHandle_t task;

  printf("%s %s\n", __FUNCTION__, name);
  if (pdPASS == xTaskCreate(thread, name, stacksize, arg, prio, &task))
    return (sys_thread_t)task;

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/* 貌似沒有用的東西 */
struct sys_timeouts
{
  struct sys_timeo *next;
};

struct timeoutlist
{
  struct sys_timeouts timeouts;
  xTaskHandle pid;
};

#define SYS_THREAD_MAX 6
static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];
static u16_t s_nextthread = 0;

struct sys_timeouts *sys_arch_timeouts(void)
{
  int i;
  xTaskHandle pid;
  struct timeoutlist *tl;

  pid = xTaskGetCurrentTaskHandle();

  for (i = 0; i < s_nextthread; i++)
  {
    tl = &(s_timeoutlist[i]);
    if (tl->pid == pid)
    {
      return &(tl->timeouts);
    }
  }
  // Error
  return NULL;
}

int sys_arch_timeout_exist(void (*tmr_hdl)(void *arg))
{
  struct sys_timeouts *timeouts;
  struct sys_timeo *t;

  timeouts = sys_arch_timeouts();

  for (t = timeouts->next; t != NULL; t = t->next)
    if (t->h == tmr_hdl)
      return 1;

  return 0;
}

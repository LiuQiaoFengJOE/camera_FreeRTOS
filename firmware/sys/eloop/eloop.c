/**@file    eloop.c
 * @author  何凱帆
 * @brief   基於socket的event loop服務
 * @details 提供基於socket的event loop服務
 *
 * 系統依賴：
 * - time
 * - socket
 * 這些依賴在plat-cfg.h的system time config和socket config中解決，單獨移植時可從中抽取
 *
 * 統一使用list.h來管理雙向鏈表
 */

#include "osal/osal_socket.h"
#include "osal/osal_alloc.h"
#include "osal/osal_time.h"

#include "eloop.h"

#include "debug.h"
#include "task/app.h"


#define ELOOP_WAKEUP_BY_LOOPBACK 0

#define EVENT_TIME 1
#define EVENT_FD 2
#define EVENT_ALWAYS 3
#define EVENT_SOFT_QUEUE 4

struct eloop_state
{
  struct list_head times;
  struct list_head fds;
  struct list_head always;
  struct event *wakeup;
  SOCK_HDL fdr;
  SOCK_HDL fdw;
  int end_loop;
};

static struct eloop_state els;

/*******************************************************************************
 * time
 ******************************************************************************/
int time_diff(time_ref *tr_start, time_ref *tr_end)
{
  return ((tr_end->tv_sec - tr_start->tv_sec) * 1000000 + tr_end->tv_usec - tr_start->tv_usec + 500) / 1000;
}

void do_gettime(struct timeval *tv, void *d)
{
#if 1
  unsigned long tick = os_time_get();
  tv->tv_sec = tick / 1000;
  tv->tv_usec = (tick % 1000) * 1000;
#else
  TickType_t tick = os_time_get();
  tv->tv_sec = tick / 1000;
  tv->tv_usec = (tick % 1000) * 1000;
#endif
}

int time_ago(time_ref *tr)
{
  time_ref now;

  do_gettime(&now, NULL);
  return time_diff(tr, &now);
}

void time_now(time_ref *tr)
{
  do_gettime((time_ref *)tr, NULL);
}

void time_add(time_ref *tr, int msec)
{
  tr->tv_sec += msec / 1000;
  tr->tv_usec += (msec % 1000) * 1000;
  if (tr->tv_usec >= 1000000)
  {
    tr->tv_usec -= 1000000;
    ++tr->tv_sec;
  }
}

void time_future(time_ref *tr, int msec)
{
  do_gettime(tr, NULL);
  time_add(tr, msec);
}

/*******************************************************************************
 * eloop
 ******************************************************************************/
static struct event *eloop_new_event(event_callback f, void *d)
{
  struct event *e;

  e = (struct event *)osal_malloc(sizeof(struct event));
  INIT_LIST_HEAD(&e->list);
  e->type = 0;
  e->flags = 0;
  e->func = f;
  e->data = d;
  return e;
}

static void eloop_strip_events(struct list_head *list)
{
  struct list_head *pos, *n;
  struct event *e;

  list_for_each_safe(pos, n, list)
  {
    e = list_entry(pos, struct event, list);
    if (e->flags & EVENT_F_REMOVE)
    {
      list_del_init(pos);
      osal_free(e); // todo: 要確定誰來負責釋放空間
    }
  }
}

void eloop_resched_event(EVT_HDL ev, time_ref *tr)
{
  struct event *e = (struct event *)ev;
  if (tr)
    e->ev.time.fire = *tr;
  else if (e->flags & EVENT_F_ENABLED)
    time_add(&e->ev.time.fire, e->ev.time.ival);
  else
    time_future(&e->ev.time.fire, e->ev.time.ival);

  e->flags &= ~EVENT_F_REMOVE;
  e->flags |= EVENT_F_ENABLED;
}

EVT_HDL eloop_add_timer(int msec, unsigned int flags, event_callback f, void *d)
{
  struct event *e;

  e = eloop_new_event(f, d);
  e->type = EVENT_TIME;
  e->flags = flags;
  e->ev.time.ival = msec;
  list_add(&e->list, &els.times);
  time_now(&e->ev.time.fire);
  eloop_resched_event(e, NULL);
  return e;
}

EVT_HDL eloop_add_alarm(int msec, unsigned int flags, event_callback f, void *d)
{
  struct event *e;

  e = eloop_new_event(f, d);
  e->type = EVENT_TIME;
  e->flags = flags | EVENT_F_ONESHOT;
  e->ev.time.ival = msec;
  list_add(&e->list, &els.times);
  time_now(&e->ev.time.fire);
  eloop_resched_event(e, NULL);
  return e;
}

EVT_HDL eloop_add_fd(SOCK_HDL fd, int write, unsigned int flags, event_callback f, void *d)
{
  struct event *e;

  e = eloop_new_event(f, d);
  e->type = EVENT_FD;
  e->flags = flags | EVENT_F_ENABLED;
  e->ev.fd.fd = fd;
  e->ev.fd.write = write;
  list_add(&e->list, &els.fds);
  return e;
}

EVT_HDL eloop_add_always(unsigned int flags, event_callback f, void *d)
{
  struct event *e;

  e = eloop_new_event(f, d);
  e->type = EVENT_ALWAYS;
  e->flags = flags | EVENT_F_ENABLED;
  list_add(&e->list, &els.always);
  return e;
}

void eloop_remove_event(EVT_HDL ev)
{
  struct event *e = (struct event *)ev;
  e->flags |= EVENT_F_REMOVE;
  e->flags &= ~(EVENT_F_RUNNING | EVENT_F_ENABLED);
}

void eloop_set_event_interval(EVT_HDL ev, int msec)
{
  struct event *e = (struct event *)ev;
  e->ev.time.ival = msec;
  if (e->flags & EVENT_F_ENABLED)
    eloop_resched_event(e, NULL);
}

void eloop_set_event_enabled(EVT_HDL ev, int enabled)
{
  struct event *e = (struct event *)ev;

  if (enabled)
    e->flags |= EVENT_F_ENABLED;
  else
    e->flags &= ~EVENT_F_ENABLED;
}

int eloop_get_event_enabled(EVT_HDL ev)
{
  struct event *e = (struct event *)ev;
  return e->flags & EVENT_F_ENABLED ? 1 : 0;
}

void eloop_exit(void)
{
  els.end_loop = 1;
}

int eloop_wakeup_init(void);
void eloop_init(void)
{
  INIT_LIST_HEAD(&els.times);
  INIT_LIST_HEAD(&els.fds);
  INIT_LIST_HEAD(&els.always);
  els.end_loop = 0;
  /*  */
  if (osal_socket_startup())
  {
    p_err("Socket startups error!");
    return;
  }
  // eloop_wakeup_init();
}

void eloop_run(int single)
{
  time_ref t, *st;
  struct event *e;
  int diff, nexttime = 0, highfd, ret;
  fd_set rfds, wfds;

  els.end_loop = 0;

  do
  {
    struct list_head *pos, *n;
    st = NULL;
    /* check how long the timeout should be */
    list_for_each_safe(pos, n, &els.times)
    {
      e = list_entry(pos, struct event, list);
      if (e->flags & EVENT_F_ENABLED)
      {
        diff = -time_ago(&e->ev.time.fire);
        if (diff < 5)
          diff = 0;
        if (!st || diff < nexttime)
          nexttime = diff;
        st = &t;
        e->flags |= EVENT_F_RUNNING;
      }
      else
        e->flags &= ~EVENT_F_RUNNING;
    }
    list_for_each_safe(pos, n, &els.always)
    {
      e = list_entry(pos, struct event, list);
      if (e->flags & EVENT_F_ENABLED)
      {
        st = &t;
        nexttime = 0;
        e->flags |= EVENT_F_RUNNING;
      }
      else
        e->flags &= ~EVENT_F_RUNNING;
    }

    if (st)
    {
      st->tv_sec = nexttime / 1000;
      st->tv_usec = (nexttime % 1000) * 1000;
    }
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    highfd = -1;
    /* This is all so ugly...  It should use poll() eventually. */
    list_for_each_safe(pos, n, &els.fds)
    {
      e = list_entry(pos, struct event, list);
      if (e->flags & EVENT_F_ENABLED)
      {
        FD_SET(e->ev.fd.fd, e->ev.fd.write ? &wfds : &rfds);
        if (e->ev.fd.fd > highfd)
          highfd = e->ev.fd.fd;
        e->flags |= EVENT_F_RUNNING;
      }
      else
        e->flags &= ~EVENT_F_RUNNING;
    }
#if WIFI_EN
    ret = select(highfd + 1, &rfds, &wfds, NULL, st);
#else
    osal_task_sleep(nexttime);
#endif
    list_for_each_safe(pos, n, &els.times)
    {
      e = list_entry(pos, struct event, list);
      if (!(e->flags & EVENT_F_RUNNING))
        continue;
      if (els.end_loop)
        break;
      diff = -time_ago(&e->ev.time.fire);
      if (diff < 5)
      {
        if (!(e->flags & EVENT_F_ONESHOT))
          eloop_resched_event(e, NULL);
        else
          e->flags |= EVENT_F_REMOVE;
        (*e->func)(e, e->data);
      }
    }
    list_for_each_safe(pos, n, &els.always)
    {
      e = list_entry(pos, struct event, list);
      if (!(e->flags & EVENT_F_RUNNING))
        continue;
      if (els.end_loop)
        break;
      if (e->flags & EVENT_F_ONESHOT)
        e->flags |= EVENT_F_REMOVE;
      (*e->func)(e, e->data);
    }
#if WIFI_EN
    if (ret > 0)

      list_for_each_safe(pos, n, &els.fds)
      {
        e = list_entry(pos, struct event, list);
        if (!(e->flags & EVENT_F_RUNNING))
          continue;
        if (els.end_loop)
          break;
        if (FD_ISSET(e->ev.fd.fd, e->ev.fd.write ? &wfds : &rfds))
        {
          if (e->flags & EVENT_F_ONESHOT)
            e->flags |= EVENT_F_REMOVE;
          (*e->func)(e, e->data);
        }
      }
#endif
    eloop_strip_events(&els.times);
    eloop_strip_events(&els.fds);
    eloop_strip_events(&els.always);
  } while (!els.end_loop && !single);
}

#if ELOOP_WAKEUP_BY_LOOPBACK
static void eloop_wakeup_read(EVT_HDL e, void *d)
{
  SOCK_HDL fd = (SOCK_HDL)d;
  struct sockaddr remote_addr;
  char buf[128];
  int len, retval;

  do
  {
    retval = sizeof(struct sockaddr);
    len = osal_socket_recvfrom(fd, buf, 128, 0, &remote_addr, &retval);
  } while (len == 128);
}

int eloop_wakeup_init(void)
{
  struct sockaddr_in addr;
  SOCK_HDL fd;
  ERR_STR strerr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(60000);

  if ((fd = osal_socket_open(SOCK_DGRAM)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("error creating listen socket: %s", strerr);
    return -1;
  }
  if (osal_set_reuseaddr(fd, 1) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_warn("ignoring error on setsockopt: %d %s", socket_errno, strerr);
  }
  if (osal_socket_block(fd, 0) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_info("error setting O_NONBLOCK on socket: %s", strerr);
  }

  if (osal_socket_bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("unable to bind to tcp socket: %s", strerr);
    osal_socket_close(fd);
    return -1;
  }

  els.fdr = fd;

  if ((els.fdw = osal_socket_open(SOCK_DGRAM)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("error creating wakeup socket: %s", strerr);
    osal_socket_close(fd);
    return -1;
  }

  els.wakeup = eloop_add_fd(fd, 0, 0, eloop_wakeup_read, (void *)fd);

  return 0;
}

void eloop_wakeup(void)
{
  struct sockaddr_in addr;
  char buf[4];
  addr.sin_family = AF_INET;
  addr.sin_port = htons(60000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  osal_socket_sendto(els.fdw, buf, 1, 0, (struct sockaddr *)&addr, sizeof addr);
}
#else  /* ELOOP_WAKEUP_BY_LOOPBACK */
// loopback機制不可用的話，採用定時事件來喚醒
static void eloop_wakeup_read(EVT_HDL e, void *d)
{
  printf("eloop wakeup\r\n");
}

int eloop_wakeup_init(void)
{
  EVT_HDL e;
  e = eloop_add_timer(5000, 0, eloop_wakeup_read, NULL);
}

void eloop_wakeup(void) {}
#endif /* ELOOP_WAKEUP_BY_LOOPBACK */

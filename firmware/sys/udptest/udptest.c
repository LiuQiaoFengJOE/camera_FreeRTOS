/**@file    udptest.c
 * @author  何凱帆
 * @brief   基於socket的UDP測試服務
 * @details 提供基於socket的UDP測試服務，用於測試UDP實際峰值帶寬
 */

#include "osal/osal_socket.h"

#include "debug.h"

#define UDP_BUFF_SIZE 1032
#define UDP_XMIT_SIZE 1024
struct udp_session
{
  uint8_t udp_buf[UDP_BUFF_SIZE]; // >= UDP_XMIT_SIZE
  struct sockaddr remote_addr;
  SOCK_HDL fd;
};

/*
 *cretate udp server, and set to can receive broadcast
 */
SOCK_HDL udptest_server_socket(uint16_t port)
{
  SOCK_HDL socket_c;
  struct sockaddr_in addr;
  ERR_STR strerr;

  p_dbg_enter;

  addr.sin_family = AF_INET;
  addr.sin_len = sizeof(struct sockaddr_in);
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htons(INADDR_ANY);

  socket_c = osal_socket_open(SOCK_DGRAM);
  if (socket_c == INVALID_SOCKET)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_err("error creating listen socket: %s", strerr);
    return socket_c;
  }

  if (osal_socket_bind(socket_c, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_err("unable to bind to udp socket: %s", strerr);
    osal_socket_close(socket_c);
    return INVALID_SOCKET;
  }

  p_dbg("udptest_server OK. socket %d\r\n", socket_c);

  return socket_c;
}

void udptest_loop(struct udp_session *sess)
{
  int len;
  int mem_free_low, mem_free_high;
  ERR_STR strerr;

  // todo : stop something

  mem_free_low = xPortGetFreeHeapSize() - (20 * 1024);
  mem_free_high = mem_free_low + (19 * 1024);

  p_dbg("udp testing ...\r\n");

  while (1)
  {
    len = osal_socket_sendto(sess->fd, sess->udp_buf, UDP_XMIT_SIZE, 0, &sess->remote_addr, sizeof(struct sockaddr));
    if (len == -1)
    {
      // osal_socket_strerr(strerr, socket_errno);
      // p_err ("send fatal err: fd=%d\r\nerrno %d %s", sess->fd, socket_errno, strerr);
      break;
    }
    if (len != UDP_XMIT_SIZE)
    {
      p_err("send err1:%d,%d\r\n", len, UDP_XMIT_SIZE);
      break;
    }

    if (xPortGetFreeHeapSize() < mem_free_low)
    {
      while (xPortGetFreeHeapSize() < mem_free_high)
      {
        vTaskDelay(10);
        p_dbg("s:%d ", xPortGetFreeHeapSize() / 1024);
      }
    }
  }

  p_dbg("udp end.\r\n");
}

void udptest_server(void *e, SOCK_HDL fd)
{
  struct udp_session *sess;
  int size, retval;
  uint8_t *addr;

  sess = (struct udp_session *)pvPortMalloc(sizeof(struct udp_session));
  if (!sess)
  {
    p_err("%s: malloc error\r\n", __func__);
    return;
  }

  p_dbg("udp server fd: %d\r\n", fd);

  retval = 16;
  size = osal_socket_recvfrom(fd, sess->udp_buf, UDP_BUFF_SIZE, MSG_DONTWAIT, &sess->remote_addr, (socklen_t *)&retval);
  addr = &((struct sockaddr_in *)&sess->remote_addr)->sin_addr.s_addr;
  p_dbg("         size: %d %s\r\n", size, sess->udp_buf);
  p_dbg("  client addr: %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);

  if (size > 0)
  {
    sess->fd = fd;
    udptest_loop(sess);
  }

  vPortFree(sess);
}

void udptest_thread(SOCK_HDL fd)
{
  time_ref t;
  while (1)
  {
    fd_set rfds;
    FD_ZERO(&rfds);
    t.tv_sec = 1;
    t.tv_usec = 1000;
    FD_SET(fd, &rfds);
    select(fd + 1, &rfds, NULL, NULL, &t);
    if (FD_ISSET(fd, &rfds))
      udptest_server(NULL, fd);
    else
      printf("select timeout %d\r\n", fd);
  }
}

void udptest_start(uint16_t port)
{
  SOCK_HDL fd;

  p_dbg("udptest_start port %d", port);

  fd = udptest_server_socket(port);
  if (fd == INVALID_SOCKET)
    return;

#if 1
  eloop_add_fd(fd, 0, 0, udptest_server, fd);
#else
  xTaskCreate(udptest_thread, "udp_test", 0x1000, fd, 10, NULL);
  fd = udptest_server_socket(port + 1);
  xTaskCreate(udptest_thread, "udp_test1", 0x1000, fd, 10, NULL);
#endif
}

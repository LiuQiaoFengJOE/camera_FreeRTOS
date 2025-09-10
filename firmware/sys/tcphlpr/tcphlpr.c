/**@file    tcphlpr.c
 * @author  何凱帆
 * @brief   tcp協議的一些基礎例程
 * @details 提供tcp協議的一些基礎例程
 *
 */

#include "osal/osal_socket.h"
#include "osal/osal_rand.h"
// #include "net/common/eloop.h"

#include "debug.h"

SOCK_HDL tcp_listen(int port)
{
  struct sockaddr_in addr;
  SOCK_HDL fd;
  ERR_STR strerr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(port);

  if ((fd = osal_socket_open(SOCK_STREAM)) < 0)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_err("error creating listen socket: %s\r\n", strerr);
    return INVALID_SOCKET;
  }
  if (osal_set_reuseaddr(fd, 1) < 0)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_warn("ignoring error on setsockopt: %s\r\n", strerr);
  }
  if (osal_socket_bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_err("unable to bind to tcp socket: %s\r\n", strerr);
    osal_socket_close(fd);
    return INVALID_SOCKET;
  }
  if (osal_socket_listen(fd, 5) < 0)
  {
    // osal_socket_strerr(strerr, socket_errno);
    // p_err("error when attempting to listen on tcp socket: %s\r\n", strerr);
    osal_socket_close(fd);
    return INVALID_SOCKET;
  }

  // eloop_add_fd( fd, 0, 0, conn_accept, (void*)fd );

  p_info("listening on tcp port %d\r\n", port);

  return fd;
}

void random_bytes(unsigned char *dest, int len)
{
  int i;
  for (i = 0; i < len; ++i)
    dest[i] = osal_random() & 0xFF;
}

void random_id(unsigned char *dest, int len)
{
  int i;

  for (i = 0; i < len / 2; ++i)
    sprintf((char *)dest + i * 2, "%02X", (unsigned int)(osal_random() & 0xFF));
  dest[i * 2] = 0;
}

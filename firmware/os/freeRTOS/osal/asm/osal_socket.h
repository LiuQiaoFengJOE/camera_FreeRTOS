/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2017 Copyright (C)
*
* File name: osal_socket.h
*
* Description:
* Timer relative functions definition for Operating System Abstract Layer
*
* Author: CHEN<chen@buildwin.com.cn>
*
* Create date: June. 15, 2016
*
*********************************************************************************
*
* Revision history:
---------------------------------------------------------------------------------
Date                    Modification                 Revision            Name
---------------------------------------------------------------------------------
June. 15, 2016      Created                       V0.1                 CHEN

********************************************************************************/
/**
 * @File osal_socket.h
 * @brief socket relative functions definition for Operating System Abstract Layer
 */

#ifndef __ASM_SOCKET_H__
#define __ASM_SOCKET_H__

#include "basic_defs.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef SOCK_HDL
#define SOCK_HDL int
#endif

#define socket_errno errno
#define socket_read_over() ((socket_errno == EWOULDBLOCK) || (socket_errno == ENOMEM))                            //
#define socket_send_over() ((socket_errno == EWOULDBLOCK) || (socket_errno == ENOMEM) || (socket_errno == EINTR)) //
#define socket_connecting() (socket_errno == EWOULDBLOCK)
#define socket_addrinuse() (socket_errno == EADDRINUSE)

    /* Win32的錯誤信息字符串需要分配空間
     * Linux則直接返回字符串指針
     * 通過下列配置來兼容
     */
    typedef char *ERR_STR;
#define osal_socket_strerr(str, n) str = lwip_strerr((n))
// Linux:
// typedef char *ERR_STR
// #define osal_socket_strerr(str,n) str=strerror(n)
// #define socket_read_over() errno==EAGAIN
// #define socket_connecting() (errno==EINPROGRESS)
// #define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define osal_socket_cleanup()
#define osal_socket_startup() ({ 0; })

    SOCK_HDL osal_socket_open(int type);
    void osal_socket_close(SOCK_HDL fd);
    int osal_set_tcp_nodelay(SOCK_HDL fd, int o);
    int osal_set_reuseaddr(SOCK_HDL fd, int o);
    int osal_socket_block(SOCK_HDL fd, int b);
    // int      osal_sendmsg         (SOCK_HDL fd, struct msghdr *msg, unsigned flags);

#define osal_socket_send(...) send(__VA_ARGS__)
#define osal_socket_sendto(...) sendto(__VA_ARGS__)
#define osal_socket_recv(...) recv(__VA_ARGS__)
#define osal_socket_recvfrom(...) recvfrom(__VA_ARGS__)
#define osal_socket_accept(...) accept(__VA_ARGS__)
#define osal_socket_listen(...) listen(__VA_ARGS__)
#define osal_socket_bind(...) bind(__VA_ARGS__)
#define osal_socket_connect(...) connect(__VA_ARGS__)

    void osal_socket_getmac(SOCK_HDL, char *);

#ifdef __cplusplus
}
#endif

#endif /* __ASM_SOCKET_H__ */

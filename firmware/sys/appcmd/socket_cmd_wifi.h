#ifndef __SOCKET_CMD_WIFI_H__
#define __SOCKET_CMD_WIFI_H__
#include "socket_cmd.h"

// Return code of socket cmd functions
#define SC_RET_SUCCESS 0
#define SC_RET_FAIL (-1)
#define SC_RET_NO_DATA (-2)
#define SC_RET_RECV_TIMEOUT (-3)

// Public APIs
extern INT32 socket_cmd_wifi_new_client_conn(jrxCmdSvr *ctl_blk);
extern INT32U socket_cmd_wifi_get_client_avail_tx_buf_size(jrxCmdSvr *ctl_blk);

#endif //__SOCKET_CMD_WIFI_H__

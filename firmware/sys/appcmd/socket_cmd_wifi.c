#include "stdio.h"
#include "string.h"
#include "socket_cmd_wifi.h"
#include "lwip/api.h"
#include "lwip/tcp.h"
#if 1

// =============================================================================
// Note: Wait for a new connection
// =============================================================================

INT32 socket_cmd_wifi_new_client_conn(jrxCmdSvr *ctl_blk)
{
  struct netconn *conn;
  conn = get_netconn(ctl_blk->server);
  if (!conn) {
  	printf("get conn err\n");
    return SC_RET_SUCCESS;
  }
	if(prvIsQueueEmpty(conn->acceptmbox))
		return SC_RET_FAIL;
	return SC_RET_SUCCESS;
}
// =============================================================================
// Note: Return the size of client's available tx buffer so that we know how much data to send
// =============================================================================
INT32U socket_cmd_wifi_get_client_avail_tx_buf_size(jrxCmdSvr *ctl_blk)
{
   struct netconn *conn;
   conn = get_netconn(ctl_blk->client);
   if (!conn) {
     return 0;
   }
	//printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	return conn->pcb.tcp->snd_buf;
}

#endif

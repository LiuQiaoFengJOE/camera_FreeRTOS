/**@file    rtsp.h
 * @author  何凱帆
 * @brief
 * @details
 */

#ifndef _RTSP_H_
#define _RTSP_H_

#include "tcphlpr/pmsg.h"
#include "stream/stream.h"
#include "osal/osal_socket.h"
#include "osal/osal_alloc.h"
#include "osal/osal_wait.h"
#include "board_config.h"
#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152)||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)
//#include "porting.h"
#include "asm/osal_time.h"
#endif
#include "eloop/eloop.h"

#define RTP_HDRSZ_INTERLEAVE 0

////////////////////////////////////////////////////////////////////////////////
#define PUT_16(p, v)            \
  ({                            \
    (p)[0] = ((v) >> 8) & 0xFF; \
    (p)[1] = (v)&0xFF;          \
  })
#define PUT_32(p, v)             \
  ({                             \
    (p)[0] = ((v) >> 24) & 0xFF; \
    (p)[1] = ((v) >> 16) & 0xFF; \
    (p)[2] = ((v) >> 8) & 0xFF;  \
    (p)[3] = (v)&0xFF;           \
  })
#define GET_16(p) (((p)[0] << 8) | (p)[1])
#define GET_32(p) (((p)[0] << 24) | ((p)[1] << 16) | ((p)[2] << 8) | (p)[3])

////////////////////////////////////////////////////////////////////////////////
// media
typedef int (*media_get_sdp_func)(char *dest, int len, int payload, int port, void *d);
typedef int (*media_get_payload_func)(int payload, void *d);
typedef int (*media_pack_func)(struct frame *r, struct stream_src_self *self, struct frame *f);
typedef int (*user_pack_func)(struct frame *r, void *d, struct frame *f);
#define DEMO_USER_PACK 0

// 客户userpack下的自己结构体
struct user
{
  void *picture; // 图片buf
  int len;       // 图片长度
  uint8_t flag;  // 是否为新图片,1	新图片	0:不变
};

struct media_ops
{
  media_get_sdp_func get_sdp;
  media_get_payload_func get_payload;
  media_pack_func pack;
  user_pack_func userpack;
};

struct media
{
  struct list_head list;
  char name[256];
  struct media_ops *ops;
};
int media_register(char *name, struct media_ops *ops);
media_pack_func media_get_pack_func(char *name);
struct media_ops *media_get_ops(char *name);
////////////////////////////////////////////////////////////////////////////////
// send item
/**@brief 發送項
 * @details
 * 一個發送項是一次網絡發送事務的數據。
 *
 * 這是幀緩衝struct frame_buf的一個子類。frame_buf定義了以下成員：
 * - struct list_head  list;
 * - struct chunklist *heap;
 * - uint8_t          *dat;
 * send_item要與之對應
 */
struct send_item
{
  struct list_head list;
  struct send_heap *heap;
  union
  {
    struct
    {
      unsigned char *dat;
      int off;
      int len;
    } buf;
    struct
    {
      struct eloop_sender *sender;
      struct list_head *pos; // dummy
      int len;
    } rtp;
  };
};

////////////////////////////////////////////////////////////////////////////////
// sender info
/**@brief sender狀態信息
 * @details
 * Sender需要維護一些狀態信息，這些信息通常需要跟下一級stream進行交互。
 * 下級stream通過stream_info接口獲取此sender_info的指針，然後可以返回信息給sender
 */
struct sender_info
{
  wait_queue_head_t wq; //< 等待隊列
  uint32_t pend_rtsp;   //< 記錄rtsp的請求
  uint32_t pend_rtp;    //< 記錄rtp請求

  fd_set wfds[2];  //< 下級stream將等待的socket填到這裏[1]
  SOCK_HDL highfd; //< 下級stream將等待的socket最大值填到這裡
};

////////////////////////////////////////////////////////////////////////////////
// conn
typedef int (*CONN_PARSE)(struct conn *c);
typedef int (*CONN_REQHDL)(struct req *req);
typedef void (*CONN_DROP)(void *c);
typedef int (*CONN_WRITE)(void *ep,struct frame* f);
typedef int (*CONN_FREE)(struct conn *c);
struct conn_proc
{
  struct list_head list;

  const char *name;

  CONN_PARSE parse_client_data;
  CONN_REQHDL handle_request;
  CONN_DROP conn_drop;
  CONN_WRITE conn_write;
  CONN_FREE conn_free;
};

void proto_plugin(struct conn_proc *proto);
void proto_register(struct conn_proc *proto);

struct req
{
  struct conn *conn;
  struct pmsg *req;
  struct pmsg *resp;
};

struct conn_state
{
  uint8_t tcp;
  uint8_t rtp;
  uint8_t rtcp;
  uint8_t rtsp;
};
#define CS_BUFFERING 4
#define CS_WAITING 2
#define CS_SENDING 1
#define CS_IDLE 0

#define CONN_PROTO_START 0
#define CONN_PROTO_RTSP 1
#define CONN_PROTO_HTTP 2
#define CONN_PROTO_SIP 3
#define RESP_MAXLEN 4 * 1024
#define SDP_MAXLEN 4 * 1024

struct conn
{

  struct stream_self self;
  struct stream_dest *input; // stream_self裏面有input，在這裡複製一份
  struct list_head list;

  // struct list_head list;
  SOCK_HDL fd;
  SOCK_HDL second_fd; /* QuickTime uses two TCP connections for tunneling */
  struct sockaddr_in client_addr;
  unsigned long client_mac[2];
  struct event *read_event;
  struct event* write_event;
  struct event *second_read_event; /* for second_fd */

  struct conn_proc *proc;
  void *proto;
  void *sess;
  struct conn *next;
  struct conn *prev;
  unsigned char chaddr[6];

  struct conn_state st;
  uint32_t tcp_off;

  unsigned char req_buf[RESP_MAXLEN + SDP_MAXLEN];
  int req_len;
  int resp_len;

  int drop;
  int writing;
  int drop_after_write;
  int base64_count; /* QuickTime uses base64 when tunneling over HTTP */
  uint32_t last_time;
};
#define resp_buf(c) c->req_buf
#define sdp_buf(c) (&c->req_buf[RESP_MAXLEN])

int conn_write(void *frame, struct stream_dest *dest);

int rtsp_claim_resp(struct conn *c);
#define rtsp_send_resp(c) c->st.rtsp = CS_WAITING

////////////////////////////////////////////////////////////////////////////////
// pending sender
uint32_t rtsp_pending_get(void *sender_stream, int sess);
void rtsp_pending_set(void *sender_stream, int sess);
void rtsp_pending_clr(void *sender_stream, int sess);
uint32_t rtp_pending_get(void *sender_stream, int track);
void rtp_pending_set(void *sender_stream, int track);
void rtp_pending_clr(void *sender_stream, int track);

////////////////////////////////////////////////////////////////////////////////
// location
// 提供open/setup/play/pause/teardown/close等操作
typedef struct session *(*open_func)(char *path, void *d);
typedef int (*get_sdp_func)(struct location *loc, char *dest, int *len, char *path);
typedef int (*setup_func)(struct session *s, int track, int type);
typedef void (*play_func)(struct session *s, double *start, char *base, int len);
typedef void (*pause_func)(struct session *s);
typedef void (*teardown_func)(struct session *s, struct rtp_endpoint *ep);
typedef void (*session_free_func)(struct session *sess);

struct rtsp_ops
{
  open_func open;
  get_sdp_func get_sdp;
  setup_func setup;
  play_func play;
  pause_func pause;
  teardown_func teardown;
  session_free_func free;
};

struct location
{
  struct list_head list;

  char path[256];
  char realm[128];
  char username[128];
  char password[128];

  struct rtsp_ops *ops; // 由source提供的rtsp操作
  void *src;            // 由source自己定義的數據，例如sender stream
};

struct location *loc_new(uint32_t extra);
void loc_set_source(struct location *loc, struct rtsp_ops *ops, void *src);
void loc_set_attr(struct location *loc, char *path, char *realm, char *username, char *password);

struct location *loc_find(char *path, int len);

////////////////////////////////////////////////////////////////////////////////
// endpoint
/* These should probably be an enum */
#define RTP_TRANS_NONE 0
#define RTP_TRANS_UDP 1
#define RTP_TRANS_INTER 2
#define MAX_INTERLEAVE_CHANNELS 8

struct rtp_endpoint
{
  struct session *sess;
  struct stream_dest *dest; // packer
  int payload;
  int max_data_size;
  unsigned int ssrc;
  unsigned int start_timestamp;
  unsigned int last_timestamp;
  int seqnum;
  int packet_count;
  int octet_count;

  EVT_HDL rtcp_send_event;
  int force_rtcp;
  unsigned char rtcp[56];
  time_ref last_rtcp_recv;
  uint32_t last_rtcp_send;

  struct frame *cur_frame;
  struct send_item *cur_item;
  int cur_off;

  int trans_type;
  union
  {
    struct
    {
      char sdp_addr[48];
      int sdp_port;
      SOCK_HDL rtp_fd;
      EVT_HDL rtp_event;
      SOCK_HDL rtcp_fd;
      EVT_HDL rtcp_event;
    } udp;
    struct
    {
      int rtp_chan;
      int rtcp_chan;
    } inter;
  } trans;
  uint8_t teardown;
};

////////////////////////////////////////////////////////////////////////////////
// session
#define MAX_TRACKS 2
struct session
{
  struct list_head list;
  struct conn *conn;
  char id[32];
  struct rtsp_ops *ops;
  struct timeval open_time;
  //char addr[64];
  struct rtp_endpoint *ep[MAX_TRACKS];
};
////////////////////////////////////////////////////////////////////////////////

#endif /* _RTSP_H_ */

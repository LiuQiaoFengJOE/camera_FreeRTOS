/**@file    stream.h
 * @author  何凱帆
 * @brief 流以及幀的處理
 * @details
 * 定義流和幀管理的數據結構
 */

#ifndef _STREAM_H_
#define _STREAM_H_

#include "basic_defs.h"
#include "osal/osal_list.h"
#include "osal/osal_irqflags.h"
#include "config/conf_parse.h"
#include "stream/mqueue.h"


typedef int (*frame_deliver_func)(void* frame, void* d);
struct frame;
struct stream;
struct stream_src;
struct stream_src_ops;
struct stream_self_ops;

typedef enum
{
	S_INPUT_PAUSE=0,
	S_INPUT_RUNNING,
	S_INPUT_DELETE,
}S_INPUT_STATUS;

typedef enum 
{
	MEDIA_VIDEO=0,
	MEDIA_AUDIO,
	MEDIA_MAX
}mediaType;


////////////////////////////////////////////////////////////////////////////////
// frame buffer
struct chunklist {
  struct list_head heap;        //< 可分配的內存塊池
  int chunksize;                //< 每個內存塊的大小
};



/**@brief 幀緩衝
 * @details
 * 在幀的管理結構中，幀(frame)、和幀緩衝(frame buffer)是兩個不同的概念。
 * - 幀(frame)是在流(stream)管理的基本單位，幀的流動形成了流的概念
 * - 幀緩衝(frame buffer)則是幀(frame)的內容表達，一幀可由多個幀緩衝構成
 * 幀緩衝由預分配好、相同大小的內存塊構成
 * 
 * 同時frame_buf可以作為一個基類，擴展出senditem等子類
 */
struct frame_buf {
  struct list_head  list; //< 同一個幀內的幀緩衝組織成表，或組織到空閒列表
  struct chunklist *heap; //< 所分配的空間來自哪個堆
  uint8_t          *dat;  //< payload數據
};

////////////////////////////////////////////////////////////////////////////////
// frame
/**@brief 幀
 * @details
 * 幀的流動形成了流的概念。
 * 
 * 每種幀都有一個源頭，這個源頭在流的概念中稱為源流(stream source)，
 * 在源流中保存了幀的重要屬性。
 * 而在此幀結構中，祇保存每一幀自己的屬性。
 */
struct frame
{
  struct list_head list; //< frame處於不同狀態時，與相同狀態的其他frame用list結構串在一起

  uint16_t id;       //< 讀鎖的引用計數
  int16_t ref_count; //< 引用計數
  uint32_t frame_staus;
  struct stream_src *stream; //< 無論frame如何流動，最終總要流回源端stream進行重新分配

  uint32_t timestamp; //< 每幀都有時間戳，以便在各個處理環節中使用
  struct
  {
    int32_t len;
    uint8_t *dat;
  } buf;                    //< buf數據的組織，適用於音頻幀
  struct list_head buflist; //< buf列表的組織，適用於視頻幀、rtp幀
  void *private;            //< 保存stream_src相關的私有數據，例如dri
  uint32_t dataLen;
  uint32_t headLen;
  uint32_t presentationTime;
  uint32_t audPresentTime;
  union
  {
    uint32_t mediaType;
    uint32_t streamNum;
  };
  union
  {
  	struct 
  	{
  	  int16_t width;
  	  int16_t hight;
  	  int16_t fps;     
  	} imageInfor;
  	struct 
  	{
  	  int32_t sampleRate;
  	} audInfor;
  };
  struct frame* next;
};

////////////////////////////////////////////////////////////////////////////////
// stream
/**@brief stream私有結構的基類
 * @details
 * 註冊到stream_dest的數據指針，統一採用stream_self及其繼承類。
 * 
 * stream_self是一個基類，目前就祇有一個元素，指向輸出的stream。
 * 絕大部分stream都有輸出，除非是末端stream。
 * 若是末端stream，則此元素output為NULL。
 * 
 * 定義stream_self基類最主要的目的是規範整個stream鏈的下行方式。
 * 之前的下行方式祇有process_frame一種，用來傳遞數據。而用來傳遞控制則無從下手。
 * 規範了stream_self之後，上級stream通過stream_dest就可以直接獲取到下級stream，
 * 這用來傳遞media等貫穿整條stream的屬性就方便多了。
 * 
 * 大多數情況下，流祇接入到一個上級，因此input_num通常為1。這時input是struct stream_dest*。
 * 有時一個流會接入到多個上級流中，例如sender和avi，input>1。這時input是struct stream_dest**。
 */
struct stream_self {
  struct stream_self_ops *ops;  //< 流的標準操作，例如set_running/plugto/plugby/ioctl
  struct stream *output;        //< 對應的流，對下級而言，這是stream_self所輸出的流
  int            input_num;     //< 此流接入到上一級流的數量
  void          *input;         //< 記錄所接入的上一級流或流數組
};

struct stream_src_self {
  struct stream_self_ops *ops;  //< 流的標準操作，例如set_running/plugto/plugby/ioctl
  struct stream_src *output;    //< 對應的流，對下級而言，這是stream_self所輸出的流
  int                input_num; //< 此流接入到上一級流的數量
  void              *input;     //< 記錄所接入的上一級流或流數組
  struct chunklist  *heap;      //< 用於分配frame buffer的池
  int32_t heap1Num;
  int32_t heap1Size;
  struct chunklist *heap1;
};

#define stream_self_get_input_num(s) ((struct stream_self*)(s))->input_num
#define stream_get_input_num(s) ({(s)?stream_self_get_input_num(s->self):0;})
#define stream_get_input(s, i) ({(s)?stream_self_get_input(s->self, i):NULL;})
struct stream_dest *stream_self_get_input (void *stream_self, int idx);
#define stream_getinfo(s,t) stream_self_getinfo(s->self, t)
void *stream_self_getinfo (void *stream_self, uint32_t infotype);

/**@brief 幀流動的一個環節
 * @details
 * 幀的流動形成了流的概念。
 * struct stream僅僅描述了幀在流動過程中的一個處理環節。
 * 
 * 但struct stream又不提供幀的任何處理細節。
 * struct stream和struct stream_dest代表了處理方法的抽象，
 * 這是幀流入和分發的處理抽象：
 * 幀流入stream，然後分發給stream_dest進行處理；
 * 每個stream_dest怎樣處理幀，是每個stream_dest自己process_frame的事。
 * 
 * 因此stream關注的細節，就祇剩下如何分發的問題了。
 * 
 * stream關注幀的分發，就不得不考慮一個問題，哪些stream_dest可以接受被分發的幀？
 * 因此stream還要提供由stream、steam_dest構成的整條流動鏈路的啟停機制。
 * 
 * 另外，源流的最原始端代表了媒介屬性。stream還需要提供媒介屬性的傳遞機制。
 * 媒介的概念與幀的概念在這裡表述一下。
 * 
 * 幀是流動數據。例如，一個JPEG幀，在編碼階段它可能以frame buffer形式存在；
 * 在網絡傳輸前，需要轉換成send list形式。
 * 因此在不同階段、即不同的stream中JPEG幀(frame)的結構是不一樣的；
 * 每個不同結構的JPEG幀，都有一個對應的源流來生成這樣的幀。
 * 
 * 但所有這些JPEG幀，都有一個共同的名字：JPEG。這個JPEG，代表的就是媒介(media)的概念。
 * 在最源端的源流中，一定有表示JPEG媒介的屬性，這個屬性必須能被傳遞到最末端的stream中去。
 */
struct stream {
  struct list_head list;          //< 流列表，用於流數據庫管理
  char name[256];                 //< 流名
  struct stream_self* self;       //< 每個具體流的私有結構
  uint32_t buf_size;              //< payload size or frame size，在配置好之後，0與否可以作為區分stream/stream_src的標誌

  uint32_t dest_socks;            //< 分發目標槽的數量
  struct stream_dest **dest_array;//< 分發目標槽的數組，dest_socks非0時有效
  struct list_head dest_list;     //< 分發目標的列表

  void (*get_framerate)(struct stream* s, int* fincr, int* fbase);
	uint8_t		running;							//是否已经启动
	uint8_t     takeOver;
  /* todo: set_running做成回調函數，以便在啟停時做些處理，例如關閉硬件 */
//  void (*set_running)(struct stream* s, int running);

  struct media_ops *media;
};

typedef void (*ioctl_func) (uint32_t func, void *obj, void *param);
typedef void (*set_status_func) (void *stream, S_INPUT_STATUS  status);
typedef void (*set_running_func)(void *stream, int waiting);//lyc del
typedef void *(*getinfo_func) (void *stream_self, uint32_t infotype);
typedef void (*config_func) (void *stream_self);
typedef uint32_t (*collect_func) (void *stream_self, void **array, uint32_t idx);
typedef uint32_t (*alloc_func) (void *stream_self, uint32_t base, uint32_t stage);
typedef void (*dest_plugto_func) (void *dest, void *stream);
typedef void (*stream_plugby_func) (void *stream, void *dest);
typedef uint32_t (*frame_lock_func)(void);
typedef void (*frame_unlock_func)(uint32_t flags);
/**@brief 流操作
 * @details
 * 除了process_frame之外，流還避免不了對set_running/plugto/plugby等事件的處理，
 * 以及獲取流信息等一些互動操作。
 */
struct stream_self_ops {
  ioctl_func          ioctl;        // 保留給上下游的流自行定義操作
  set_running_func  set_running;  //lyc del
  set_status_func    set_status;  // 下游流聲稱waiting狀態變化時，上游流進行處理的機會
  getinfo_func        getinfo;      // 取流私有信息的接口，具體的信息類型，由上下游流協商
  alloc_func          alloc;        // 流自行分配或釋放空間
  collect_func        collect;      // 流收集流
  config_func         config;       // 流自行根據上下游情況配置自己的參數
  dest_plugto_func    plugto;       // 下游流插入/拔出到上游時獲得的處理機會
  stream_plugby_func  plugby;       // 上游流被下游流插入/拔出時獲得的處理機會
  frame_lock_func   lock;
  frame_unlock_func unlock;
};
uint32_t stream_self_collect_default (struct stream_self *self, struct stream_self **array, uint32_t idx);

typedef void *(*stream_info_func)(struct stream *);
typedef int (*frame_size_func)(void *f);
typedef int (*frame_new_func)(void* f);
typedef int (*frame_free_func)(struct frame* f);

/**@brief 源流屬性
 * @details
 * 流的屬性是一個重要概念。
 * - 首先是幀的管理，幀緩衝是如何分配、釋放的，這得由源流決定
 * - 源流的運行環境比較複雜，線程、中斷、快速中斷都有可能，
 *   因此幀管理的互斥機制在不同源流中也是不一樣的
 */
struct stream_src_ops {
  stream_info_func stream_info;

  frame_size_func bufsize;
  set_status_func set_status;
  frame_new_func new;
  frame_free_func free;

  /* 源流的鎖機制，由各源流實例自行定義 */
  frame_lock_func   lock;
  frame_unlock_func unlock;
};

/**@brief 源流
 * @details
 * 源流是一種流，它與流是繼承關係。
 * 因此stream_src的首個成員必須是struct stream，這樣指針操作時可以簡單地互相轉換。
 * 這是C實現面向對象的典型實現方式。

 因此 stream 与 stream_src是同样的
 * 
 * 源流有兩個方面的內容：
 * - 幀的空滿表
 * - 流的屬性
 */
struct stream_src {
  struct stream s;            //< 源流繼承自流

  struct list_head free_list;
  struct list_head full_list;
  struct stream_src_ops* ops; //< 源流的屬性及操作

  uint16_t frame_num; // frame numbers
  uint16_t lst_size;  // frame_buf/send_item size (or zero when use buf.dat)
  uint32_t buf_num;   // payload numbers
};


/**@brief 流處理的分發目標
 * @details
 */
struct stream_dest {
  struct list_head list;

  struct stream* stream;            //< 分發目標所對應的流
  S_INPUT_STATUS  status;                      //< 表明這個分發目標是否處於接收數據的活動狀態
  int32_t socknum;                 //< 當對應流具有目標槽時，這裡記錄槽號

  frame_deliver_func process_frame; //< 幀處理方式
  struct stream_self* self;         //< 記錄每個具體分發目標的私有結構
  queue  mq;
  uint32_t dropFrame;
};

enum {
  STREAM_IOCTL_PLUG
};

////////////////////////////////////////////////////////////////////////////////
struct workmode;
typedef int (*wmm_push_func) (struct workmode *, void *);
typedef int (*wmm_pop_func) (struct workmode *, void *);
typedef uint8_t path_type;
struct mode_trans {
  path_type          *path;         //< 狀態轉換表

  wmm_push_func       push;         //< 壓入此狀態
  wmm_pop_func        pop;          //< 彈出當前狀態的回調函數
  void               *priv;         //< 此狀態對應的私有數據

  uint32_t            prev_mode;
  struct stream_src **stream;       //< 記錄進入此狀態時，流棧的位置
  uint32_t            base;         //< 記錄進入此狀態時，空間棧的位置
};

struct workmode {
  uint32_t            mode_num;     //< 狀態數量
  struct mode_trans  *trans;        //< 狀態轉換表及轉換函數

  struct stream_src **stream;       //< 流棧，要根據buf_size來判斷是否為struct stream
  uint32_t            base;         //< 空間棧
  uint32_t            end;          //< 空間棧結束地址

  uint32_t            curr_mode;    //< 記錄當前狀態
  struct stream_src **curr_stream;  //< 當前模式流棧頂
  uint32_t            curr_base;    //< 當前模式空間棧頂
  struct stream_src *adjusStreamMemStart; 
  uint32_t   remainMem;
};

////////////////////////////////////////////////////////////////////////////////
extern struct chunklist videoheap;
extern struct chunklist audioheap;
extern struct chunklist sendheap;


void frame_ref(struct frame* f);


int frame_bufsize(struct frame* f);

void stream_check(void);
struct stream* stream_find(char* name);
struct stream *stream_find_alloc(char *name, uint32_t size, void *d);
void stream_init(void *stream, char* name, void* d);
struct stream* stream_new(char* name, void* d);
struct stream_src* stream_src_new(char* name, void* d);
void stream_alloc_dest_socks(void *stream, uint32_t socks);
void stream_dest_del(struct stream_dest *dest);
int stream_dest_plugin (struct stream_dest *dest, void *stream);
struct stream_dest* stream_dest_new(frame_deliver_func process_frame, void* d,u32 queueSize);
struct stream_dest* stream_connect(char* name, frame_deliver_func process_frame, void* d,u32 queueSize);
void stream_self_init (void *stream_self, struct stream_self_ops *ops);
void stream_src_self_init (void *stream_self, struct stream_self_ops *ops, void *heap);
void stream_diffuse_media (void *stream, struct media_ops *media);
void stream_set_media (void *stream, struct media_ops *media);
void stream_src_init(struct stream_src* s, struct list_head* frame_list, struct stream_src_ops* ops);
void *stream_src_info(void *stream);
//void stream_del_frame(struct frame* f);
//struct frame *stream_lock_frame (void *stream, uint32_t timestamp);
void stream_dest_inturn(void* stream, struct frame* f);
void stream_dest_signal (void* stream, uint32_t pending);
struct frame *stream_next_frame (struct stream_src *s);


int streamDestInturnDealImmediately(void* stream, struct frame* f);
void freeFrame(struct frame* f);
void queueFreeFrames(queue *mq);
struct list_head *frameHeadBuffer(struct frame* f);
struct list_head *frameNewBuffer(struct frame* f);
struct list_head *streamNewBuffer(struct stream_src *s,struct list_head *head);
void streamFreeBuffer(struct stream_src *s,struct list_head *list);
struct frame* streamNewFrame(struct stream_src* s);
int frameAddFrame(struct frame *head,struct frame *f);
void streamPrintFreeFrame(struct stream_src* s);
void streamPrintFullFrame(struct stream_src* s);
struct list_head *frameTailBuffer(struct frame* f);
u32 streamFreeBuffSize(struct stream_src* s);
int streamDestAddFrame(struct stream_dest* streamDest, struct frame* f);
int streamDestGiveBackFrame(struct stream_dest* streamDest, struct frame* f);
struct frame* streamDestGetFrame(struct stream_dest* streamDest);
struct frame* streamDestPeekFrame(struct stream_dest* streamDest);
void streamPrintDestQueue(struct stream* s);
void streamSetSatus( struct stream_dest* dest, S_INPUT_STATUS status);
void upstreamSetSatus(struct stream_dest *dest, S_INPUT_STATUS status);
void streamDestFreeFrames(struct stream_dest* streamDest,int delQueue);
void streamSetRuning( struct stream_dest* dest);
void streamSetPause( struct stream_dest* dest);
void streamFreeFrames(struct stream_src* s);
u32 streamDestQueueValidSize(struct stream_dest* streamDest);
struct frame* streamDestGetRearFrame(struct stream_dest* streamDest);






#endif  /* _STREAM_H_ */


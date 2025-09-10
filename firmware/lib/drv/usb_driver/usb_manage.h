#ifndef _USB_DRIVER_H_
#define _USB_DRIVER_H_
#include "osal/osal_list.h"
#include "typedef.h"
#include "stream/stream.h"
#include "osal/osal_wait.h"
#include "osal/osal_timer.h"
// #include "usb_host/usb_host.h"
/*USB EP0 transfer type*/
#define TRANSF_SETUP 0xD0
#define TRANSF_IN 0x90
#define TRANSF_OUT 0x10

#define TT_ILLEGAL (0 << 4)
#define TT_ISOCHRONOUS (1 << 4)
#define TT_BULK (2 << 4)
#define TT_INTERRUPT (3 << 4)

#define _HUB_ADDR_ 5
#define _HUB_EP_ 3
// #define TT_INTERRUPT    (3 << 4)
#define HUB_CLASS 0X09
#define HUB_CLASS_DEV 0X09

#define _UVC_ADDR_ 4
#define _UVC_EP_ 4
// #define TT_ISOCHRONOUS   0x10
#define UVC_CLASS 0x0e
#define UVC_CLASS_DEV 0xef

typedef enum
{
    _OK_,
    _STALL_,
    _PIDER_,
    _PLDER_,
    _TOUT_,
    _UNKWN_,

} TSTA;

typedef struct
{
    u8 request_type;
    u8 request;
    u16 value;
    u16 index;
    u16 length;
} request_pack;

typedef struct
{
    u8 payload;
    u8 addr; // 设备地址
    u8 *data_fifo;
    u8 *ep0_fifo;
    u8 *ep1_fifo;
    u8 *ep2_fifo;
    u8 *buffer;       // usb transfer buffer must be
    u8 client_ep;     // 设备的端点
    u8 host_ep;       // 与这个设备通信的主机端点
    u8 trans_type;    // 端点0通信时临时状态记录
    u32 max_pack_len; // 一包数据最大长度
    u32 usb_type;     // iso int
    u32 len;
    request_pack class_req; // 端点0通信时用
} dev_mesg;

typedef struct
{
    u32 match_id;                                                   // 用于匹配device 匹配设备描述符
    u8 (*prob)(struct usb_dev *ud, u8 resolution, u32 dev_dsc_len); // 插入设备后匹配上后将被调用
    // void (*usb_driver_irq)();
    void *private_p; // 私有数据用于保存流节点
    u8 *fifo;        // 数据fifo
    u8 irq_num;      // 中断端点
    u8 index;        // 当前分辨率
    u8 init_index;   // 初始化分辨率     		1：VGA  2：720P
    struct list_head list;
    void (*irq_handle)(); // 数据中断处理函数
    void (*change_action)(struct usb_dev *udev, u8 resolution);

} usb_driver;

struct usb_dev;
struct USB_Host;
typedef struct
{
    /*ep0 ops*/
    u8 (*ep0_set_configuration)(struct usb_dev *udev, u8 cfg);
    u8 (*ep0_get_configuration)(struct usb_dev *udev, u8 *desc, u32 rlen);
    u8 (*ep0_set_addr)(struct usb_dev *udev, u8 adr);
    u8 (*ep0_get_desc)(struct usb_dev *udev, u8 *desc, u32 rlen);
    u8 (*ep0_set_data)(u8 *buf, u16 len);
    void (*ep0_get_req)(void);
    u8 (*usb_ep0_request)(void);
    /*MCU OPS*/
    u32 (*is_usb_data_error)(dev_mesg *usb_dev);
    void (*enable_ep_irq)(dev_mesg *usb_dev);
    void (*usb_ep_init)(dev_mesg *usb_dev, u8 in_ep);
    void (*kick_usb)(dev_mesg *usb_dev);
    void (*usb_host_speed)(u8 speed);
    void (*usb_init)(void);
    u32 (*usb_data_len)(dev_mesg *usb_dev);
    u32 (*usb_IntrUSB)();
    u32 (*usb_IntrOutl)();
    u32 (*usb_IntrInh)();
    u32 (*usb_IntrInl)();
    TSTA (*usb_setup_request)
    (dev_mesg *stack, u8 *ppkg);
    TSTA (*usb_txrx_pkg)
    (u8 epx, dev_mesg *ustack);
} host_ops;

typedef struct USB_Host
{

    host_ops ops; // 操作函数
    u8 *ep0_fifo; // 端点0fifo
    u8 *ep1_fifo;
    u8 *ep2_fifo;
    u8 (*usb_host_connect)(struct usb_dev *ud);
    void (*usb_host_disconnect)(struct USB_Host *host);
    void *private_p;
    usb_driver *(*usb_match)(struct usb_dev *udev);
    u8 is_enum_finish; // 枚举完成标志位，用于判断是否开始检测拔插
    u8 connect;        // 插入检测标志位
    u8 disconnect;
    u32 connect_count; // 热拔插计数
    struct list_head host_list;
    struct list_head *class_list; // driver链表
    struct timer_list timer;
    wait_queue_head_t wq; // 用于唤醒usb热拔插线程的信号量
} usb_host;

struct usb_dev
{
    u32 match_id;       // 用于匹配驱动的ID 在host获取设备描述符时赋值
    usb_driver *driver; // 支持此设备的驱动
    dev_mesg dev;       // 端点，地址等信息
    usb_host *host;     // 此设备挂载的USB host
    struct list_head list;
};
struct jpg_src
{
    struct stream_src_self self; // with a chunklist heap
    //struct list_head cur_buffer;
    int framerate;
	struct frame *curFrame;
};

//unsigned long arch_local_irq_save(void);
//void arch_local_irq_restore(unsigned long     flags);
extern struct list_head usb_device_head;
extern struct list_head usb_driver_head;
extern struct list_head usb_host_head;
extern u8 uvc_fifo[1024 * 3];
extern u8 ep0_fifo[64];
extern u8 ep1_fifo[64];
//extern u8 desc[2048];
extern u8 hub_fifo[16];

#endif /* _USB_DRIVER_H_ */

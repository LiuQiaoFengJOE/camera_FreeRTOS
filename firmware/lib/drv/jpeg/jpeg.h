#ifndef __JPEG_DRV_H__
#define __JPEG_DRV_H__

#include "stream/stream.h"
#include "sysCommon/commonInclude.h"
#include "rtp-media/jpeg.h"
#include "eloop/irqTimer.h"

#define MAX_COLUMN	10

#define TOTAL_JFRAME	24//12
#define NUM_OF_ROW	4// 4

////////////////////////////////////////////////////////////////////////////////
typedef struct children_frame
{
  u8 *frame_buf;               // 相框buf
  UT_array *yuv_instead_table; // table表

} children_frame;
typedef struct jpgenc_drv
{
  void *controller;
  int running; // 編碼器正在工作中
  struct frame *yuv_frame;
  struct frame *jpg_frame;

  uint32_t yuv_w, yuv_h, y_size;
  uint32_t jpg_w, jpg_h;
  uint32_t avi_pixel, wifi_pixel;
  uint8_t whichController;
  children_frame children_frame_struct; // 儿童相机相框的结构体
} jpgenc_drv;

////////////////////////////////////////////////////////////////////////////////
typedef  void (*imageDecDone)(void*);
typedef struct jpgdec_drv
{
  void *controller;
  int running;
  int err;
  struct frame *jpg_frame;
  struct list_head *cur_buffer;
  struct media_jpeg media;
  uint8_t *yuvBuf;
  uint8_t *uvBuf;
  uint32_t yuv_w, yuv_h;
  uint32_t stride;
  imageDecDone done;
} jpgdec_drv;

#if 0
struct jpgenc_manu
{
  struct stream_src_self self; // with a chunklist heap
  jpgenc_drv driver;
  struct list_head cur_buffer;
  uint32_t out_pack_size;
  void *watermark;       // 日期标签水印
  uint8_t watermarkMode; //	1:代表在kick时不重新enable,2:代表每次进kick时都需要重新enable(主要为了解决缩略图时,水印关闭)
  uint8_t whichController;
  int32_t socknum; // 比较特殊,记录帧的槽号,如果为0,则直接del,否则只是将frame对应位清空

  uint32_t buf_num_net;   // 网络设置的buf节点
  uint32_t buf_num_local; // 本地设置的buf节点
};
#endif
struct jpgencAuto
{
  struct stream_src_self self; // with a chunklist heap
  void *controller;
  void *waterMark;
  u32 srcW,srcH;
  u32 scaleW,scaleH;
  u32 buffW,buffH;
  u32 destW,destH;
  u32 tjpgW,tjpgH;
  u32 lastFrameTime;
  struct frame*  curFrame;
  struct frame*  tJframe[MAX_COLUMN/*NUM_OF_ROW*/];
  struct list_head tempListBuf;
  u32  out_pack_size;
  u8  useTempBuff;
  u8  useManu;
  u8  working;
  u8  curFIdx;
  IRQ_TIMER  *codecTimer;
  u8  largeImgFlag;
};
#define JPGAUTO_CONTROL   0
#define WATERMARK_AUTO_FLAG1    1

int jpgDecFrame(struct frame *f,uint8_t *yuvBUf,uint32_t w,uint32_t h,imageDecDone done);
int jpgDecSta(void);
u8 not_isr_jpgdec2(uint8_t *yuv_buf, uint8_t *jpgbuf, uint32_t jpegbuf_size, uint32_t video_w, uint32_t video_h);

#endif /* __JPEG_DRV_H__ */

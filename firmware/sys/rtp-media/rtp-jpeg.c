/**@file    rtp-jpeg.c
 * @author  何凱帆
 * @brief   rtp jpeg介質管理
 * @details 提供rtp jpeg介質
 */

#include "config/conf_parse.h"

#include "rtp-media/jpeg.h"
#include "stream/stream.h"
#include "osal/osal_sema.h"
#include "osal/osal_task.h"

#include "debug.h"
#include "rtsp/rtsp.h"
#include "task/app.h"

// todo: 找個地方定義
#define AVICHUNK_HDR 8
#define JPGDRITBL 512

#define RTP_HDRSZ_RTPHDR 12
#define RTP_HDRSZ_JHDR 8
#define RTP_HDRSZ_DRI 4
#define RTP_HDRSZ_QHDR 4
#define RTP_HDRSZ_QTBL 128

////////////////////////////////////////////////////////////////////////////////
static int get_sdp(char *dest, int len, int payload, int port, void *d)
{
  return snprintf(dest, len, "m=video %d RTP/AVP 26\r\n", port);
}

static int get_payload(int payload, void *d)
{
  return 26;
}

////////////////////////////////////////////////////////////////////////////////
/**@brief 掃描量化表
 * @param media [out] 掃描得到的量化表信息，存到這裡
 * @param buf   [in]  緩衝的起始地址
 * @param off   [in]  量化表相對於緩衝的起始位置
 * @param len   [in]  此量化表頭部所描述的長度
 * @retval -1 掃描出錯
 * @retval  0 掃描成功
 */
static int parse_DQT(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  int i;
  uint8_t *d = buf + off;

  for (i = 0; i < len; i += 65)
  {
    if ((d[i] & 0xF0) != 0)
    {
      printf("Unsupported quant table precision 0x%X!\n", d[i] & 0xF0);
      return -1;
    }
    media->quant[d[i] & 0xF] = i + off + 1;
  }
  return 0;
}

static int parse_DHT(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  uint32_t counter, i;
  uint8_t c, d;
  /* We should verify that this coder uses the standard Huffman tables */
  /* Kaifan:
   * Some USB-Sensor do not use standard Huffman tables.
   * So I add Huffman following quant table.
   * It's compatible to the old version of App,
   * It just looks like some dummy quant tables exist.
   * But the dummy size can not be too long, because the old rtpdec-jpeg.c uses hdr[1024]
   */
  if (0 == media->hufflen)
    media->huffoff = off - 4;
  media->hufflen += len + 4;
  PUT_16(media->exthdr + EXTHDROFF_HUFF, media->hufflen);

  while (len > 0)
  {
    d = buf[off++];
    d = ((d & 0x10) >> 3) | (d & 0x01);
    media->huff[d] = off;

    counter = 0;
    for (i = 0; i < 16; ++i)
      counter += buf[off++];
    len -= counter + 17;
    off += counter;
  }

  return 0;
}

static int parse_DRI(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  media->reset_interval = GET_16(buf + off);
  if (media->reset_interval)
  {
    media->type |= 0x40;
    /* Kaifan:
     * Some USB-Sensor's reset-interval is not width/16.
     * But the origin room for reset-interval is used by res_len.
     * So Save the reset-interval before the Huffman table
     */
    PUT_16(media->exthdr + EXTHDROFF_DRI, media->reset_interval);
  }
  return 0;
}

static int parse_SOF(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  int c;
  uint8_t *d = buf + off;
  uint16_t width, height;

  media->chroma_table = -1;

  if (d[0] != 8)
  {
    printf("Invalid precision %d in SOF0\n", d[0]);
    return -1;
  }

  height = GET_16(d + 1);
  width = GET_16(d + 3);
  if ((height & 0x7) || (width & 0x7))
  {
    printf("Width/height not divisible by 8!\n");
    return -1;
  }
  media->height = height >> 3;
  media->width = width >> 3;
  // printf("w:%d,h:%d\n", width,height);
  if (d[5] != 3)
  {
    printf("Number of components is %d, not 3!\n", d[5]);
    return -1;
  }
  /* Loop over the parameters for each component */
  for (c = 6; c < 6 + 3 * 3; c += 3)
  {
    switch (d[c])
    {
    case 1: /* Y */
      if (d[c + 1] == 0x21)
        media->type &= ~1; // YUV422
      else if (d[c + 1] == 0x22)
        media->type |= 1; // YUV420
      else
      {
        printf("Invalid sampling factor 0x%02X in Y component!\n", d[c + 1]);
        return -1;
      }
      media->luma_table = d[c + 2];
      break;
    case 2: /* Cb */
    case 3: /* Cr */
      if (d[c + 1] != 0x11)
      {
        printf("Invalid sampling factor 0x%02X in %s component!\n", d[c + 1], d[c] == 2 ? "Cb" : "Cr");
        return -1;
      }
      if (media->chroma_table < 0)
        media->chroma_table = d[c + 2];
      else if (media->chroma_table != d[c + 2])
      {
        printf("Cb and Cr components do not share a quantization table!\n");
        return -1;
      }
      break;
    default:
      printf("Invalid component %d in SOF!\n", d[c]);
      return -1;
    }
    media->component[d[c] - 1].blocks_per_mcu_h = d[c + 1] >> 4;
    media->component[d[c] - 1].blocks_per_mcu_v = d[c + 1] & 0xF;
    media->component[d[c] - 1].quant_table_id = d[c + 2];
  }
  return 0;
}

static int parse_SOF2(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  int c;
  uint8_t *d = buf + off;
  uint16_t width, height;

  media->chroma_table = -1;

  if (d[0] != 8)
  {
    printf("Invalid precision %d in SOF0\n", d[0]);
    return -1;
  }

  height = GET_16(d + 1);
  width = GET_16(d + 3);
  media->height = height;
  media->width = width;

  if (d[5] != 3)
  {
    printf("Number of components is %d, not 3!\n", d[5]);
    return -1;
  }
  /* Loop over the parameters for each component */
  for (c = 6; c < 6 + 3 * 3; c += 3)
  {
    switch (d[c])
    {
    case 1: /* Y */
      if (d[c + 1] == 0x21)
        media->type &= ~1; // YUV422
      else if (d[c + 1] == 0x22)
        media->type |= 1; // YUV420
      else
      {
        printf("Invalid sampling factor 0x%02X in Y component!\n", d[c + 1]);
        return -1;
      }
      media->luma_table = d[c + 2];
      break;
    case 2: /* Cb */
    case 3: /* Cr */
      if (d[c + 1] != 0x11)
      {
        printf("Invalid sampling factor 0x%02X in %s component!\n", d[c + 1], d[c] == 2 ? "Cb" : "Cr");
        return -1;
      }
      if (media->chroma_table < 0)
        media->chroma_table = d[c + 2];
      else if (media->chroma_table != d[c + 2])
      {
        printf("Cb and Cr components do not share a quantization table!\n");
        return -1;
      }
      break;
    default:
      printf("Invalid component %d in SOF!\n", d[c]);
      return -1;
    }
    media->component[d[c] - 1].blocks_per_mcu_h = d[c + 1] >> 4;
    media->component[d[c] - 1].blocks_per_mcu_v = d[c + 1] & 0xF;
    media->component[d[c] - 1].quant_table_id = d[c + 2];
  }
  return 0;
}

static int parse_SOS(struct media_jpeg *media, uint8_t *buf, int off, int len)
{
  int c;
  uint8_t *d = buf + off + 1;
  int i;
  for (i = 0; i < 3; ++i)
  {
    int id = d[i * 2 + 0] - 1;
    media->component[id].huffman_dc_table_id = d[i * 2 + 1] >> 4;
    media->component[id].huffman_ac_table_id = d[i * 2 + 1] & 0xF;
  }
}

/**@brief 掃描jpeg關鍵屬性
 * 要求傳入的buf/len完整地保存了從FFD8到FFDA之間的內容
 * @param media [out] 掃描結果
 * @param buf   [in]  緩衝，保存有jpeg頭部信息
 * @param len   [in]  緩衝大小
 * @return -1:err 0:noscan 1:newscan
 */
int jpg_scan(struct media_jpeg *media, uint8_t *buf, int len)
{
  int i;
  int blen; // jpeg標記的長度
  int jpegOrvideo;
 // buf = buf + 0x80000000;

  // 由于avi有缩略图，因此需要判断相邻两帧图片的对应段是否一样
  /*
    if(media->scan_data >0 && media->scan_data<len)
     {
     if(buf[media->scan_data-14] != 0xff || buf[media->scan_data-13] != 0xda)
     {
       media->need_scan =1 ;
     }
     }
  */
  // 不是0xff开头，代表数据是从回放视频传输过来
  // 0xff开头,不出错的话,是浏览图片的时候传过来的,则需要重新扫描,因为由于不同图片分辨率不一样，需要重新扫描
  // 而视频则不同,所有帧的分辨率都是一样的
  if (*buf != 0xff)
  {
    jpegOrvideo = AVICHUNK_HDR;
	//printf("video frame\n");
  }
  else
  {
    jpegOrvideo = 0;
    media->need_scan = 1; // 重新扫描
  }

  // if (!media->need_scan) return 0;
  {
    media->type = 0;
    media->hufflen = 0;
    media->exthdr[EXTHDROFF_MAGIC] = 0;
    media->exthdr[EXTHDROFF_VER] = 0;
    for (i = 0; i < 16; ++i)
      media->quant[i] = 0;
  }

  for (i = jpegOrvideo; i < len; i += blen + 2)
  {
    if (buf[i] != 0xFF)
    {
      printf("Found %02X at %d, expecting FF\n", buf[i], i);
      return -1;
    }
    while (buf[i + 1] == 0xFF)
      ++i; /* 協議規定可以有多個FF */

    /* SOI (FF D8) is a bare marker with no length field */
    if (buf[i + 1] == 0xD8)
      blen = 0;
    else
      blen = GET_16(buf + i + 2);

    switch (buf[i + 1])
    {
    case 0xDB: /* Quantization Table */
      if (parse_DQT(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Quantization Table err!\n");
        return -1;
      }
      break;
    case 0xC4: /* Huffman Table */
      if (parse_DHT(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Huffman Table err!\n");
        return -1;
      }
      break;
    case 0xDD: /* DRI */
      if (parse_DRI(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame DRI err!\n");
        return -1;
      }
      break;
    case 0xC0: /* Start of Frame */
      if (parse_SOF(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Start of Frame err!\n");
        return -1;
      }
      break;
    case 0xDA: /* Start of Scan */
      parse_SOS(media, buf, i + 4, blen - 2);
      media->scan_data = i + 14;
      media->need_scan = 0;
      return 1;
    }
  }

  printf("No scan data found!\n");
  return -1;
}

// 输入jpeg的width与height不需要整除8
int jpg_scan2(struct media_jpeg *media, uint8_t *buf, int len)
{
  int i;
  int blen; // jpeg標記的長度
  int jpegOrvideo;
  // buf = buf+0x80000000;

  // 由于avi有缩略图，因此需要判断相邻两帧图片的对应段是否一样
  /*
    if(media->scan_data >0 && media->scan_data<len)
     {
     if(buf[media->scan_data-14] != 0xff || buf[media->scan_data-13] != 0xda)
     {
       media->need_scan =1 ;
     }
     }
  */
  // 不是0xff开头，代表数据是从回放视频传输过来
  // 0xff开头,不出错的话,是浏览图片的时候传过来的,则需要重新扫描,因为由于不同图片分辨率不一样，需要重新扫描
  // 而视频则不同,所有帧的分辨率都是一样的
  if (*buf != 0xff)
  {
    jpegOrvideo = AVICHUNK_HDR;
  }
  else
  {
    jpegOrvideo = 0;
    media->need_scan = 1; // 重新扫描
  }

  // if (!media->need_scan) return 0;
  {
    media->type = 0;
    media->hufflen = 0;
    media->exthdr[EXTHDROFF_MAGIC] = 0;
    media->exthdr[EXTHDROFF_VER] = 0;
    for (i = 0; i < 16; ++i)
      media->quant[i] = 0;
  }

  for (i = jpegOrvideo; i < len; i += blen + 2)
  {
    if (buf[i] != 0xFF)
    {
      printf("Found %02X at %d, expecting FF\n", buf[i], i);
      uartPrintfBuf(buf, 16);
      return -1;
    }
    while (buf[i + 1] == 0xFF)
      ++i; /* 協議規定可以有多個FF */

    /* SOI (FF D8) is a bare marker with no length field */
    if (buf[i + 1] == 0xD8)
      blen = 0;
    else
      blen = GET_16(buf + i + 2);

    switch (buf[i + 1])
    {
    case 0xDB: /* Quantization Table */
      if (parse_DQT(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Quantization Table err!\n");
        return -1;
      }
      break;
    case 0xC4: /* Huffman Table */
      if (parse_DHT(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Huffman Table err!\n");
        return -1;
      }
      break;
    case 0xDD: /* DRI */
      if (parse_DRI(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame DRI err!\n");
        return -1;
      }
      break;
    case 0xC0: /* Start of Frame */
      if (parse_SOF2(media, buf, i + 4, blen - 2) < 0)
      {
        printf("jpeg_process_frame Start of Frame err!\n");
        return -1;
      }
      break;
    case 0xDA: /* Start of Scan */
      parse_SOS(media, buf, i + 4, blen - 2);
      media->scan_data = i + 14;
      media->need_scan = 0;
      return 1;
    }
  }

  printf("No scan data found!\n");
  return -1;
}

////////////////////////////////////////////////////////////////////////////////
static int cal_data_len(unsigned short *dri_len, int dri_num, int *dri_index, int *res_len, int max_size)
{
  int len = 0;
  int idx, res;

  idx = *dri_index;
  res = *res_len;
  do
  {
    if (res)
      len = res;
    else
      len += dri_len[idx];

    res = 0;

    if (len > max_size)
    {
      res = len - max_size;
      len = max_size;
      break;
    }

    ++idx;
  } while (idx <= dri_num && len < max_size);

  *dri_index = idx;
  *res_len = res;
  return len;
}

/**
 * @brief 計算下一個dri_index和剩餘
 *
 * @param dri_index [in] 當前dri_index
 * @param len       [in] 當前使用的大小
 * @param dri_len   [in] 記錄dri長度的數組
 * @param res_len   [inout] 當前dri_index剩餘長度、輸出下一個dri_index剩餘長度
 * @return 下一個dri_index
 */
static int next_dri_index(int dri_index, int len, unsigned short *dri_len, int *res_len)
{
  int res = *res_len;

  while (len > res)
  {
    len -= res;
    ++dri_index;
    res = dri_len[dri_index];
  }

  if (len == res)
  {
    ++dri_index;
    res = 0;
  }
  else
    res -= len;

  *res_len = res;
  return dri_index;
}

/* Restart Marker header present
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       Restart Interval      |F|L|       Restart Count         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static UINT32 cal_drihdr(int *dri_index, int max_data_size, unsigned short *dri_len, int *res_len)
{
  int idx = *dri_index;
  int dri_index_tmp = idx;
  UINT32 dri_val = idx + (*res_len ? 1 : 0x8000);

  idx = next_dri_index(idx, max_data_size, dri_len, res_len);

  if (*res_len)
  {
    dri_val += (max_data_size - dri_len[idx] - *res_len) << 16;
  }
  else
  {
    dri_val |= 0x4000;
  }
  dri_val |= (idx - dri_index_tmp) << 6;

  *dri_index = idx;
  return dri_val;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief pack frame f to send_list, allocated send_item from heap
 * @param send_list 目標List
 * @param heap      所需send_item的空間從heap中分配
 * @param f         源frame，也是以buflist構成
 * @retval -1 failure
 * @retval  0 OK
 */
 extern int stopStreamTrans;
static int jpg_pack(struct frame *r, struct stream_src_self *self, struct frame *f)
{
  struct chunklist *heap = self->heap;
  struct send_item *si;
  struct frame_buf *buf;
  struct media_jpeg *media;
  UINT32 frame_len;
  UINT32 timestamp;
  UINT8 *inter, *rtphdr, *jhdr, *qhdr, *drihdr;
  UINT32 *fourcc;
  UINT8 *dat;
  int off, src, dst, hdr_len;
  int plen;
  UINT16 *dri_len;
  int dri_hdr_len = 0;
  int dri_index_tmp, dri_index = 0;
  int res_len = 0;
  int ret = 0;
  int rtp_bufsize, jpg_bufsize;
  UINT32 srcSize = 0, packerSize = 0;
  // char test[4]= {0x00,0x28,0xff,0xff};
  UINT8 Restart_Count[2] = {0xff, 0xff};

  // p_dbg("jpg_packer");
  if (NULL == f || list_empty(&f->buflist))
  {
    p_dbg("list_empty %X", (unsigned)f);
    return -1;
  }
  media = (struct media_jpeg *)f->stream->s.media;

  rtp_bufsize = r->stream->ops->bufsize(r);
  jpg_bufsize = f->stream->ops->bufsize(f);
  timestamp = f->timestamp;

  // 首個buf
  buf = list_entry(f->buflist.next, struct frame_buf, list);
  dat = buf->dat;
  #if JR_APP_PROTOCOL
  ret = jpg_scan(media, dat, jpg_bufsize);
  if (ret == -1)
  {
    return -1;
  }
  #endif
  dri_len = (UINT16 *)(f->private); // 注意要跳過第一個FF D8
  fourcc = (UINT32 *)(dat);
  frame_len = fourcc[1]; // todo: FF D9之後，最多可能有3個0需要刪掉
                         // p_verb ("jpg_pack timestamp %X", timestamp);
  if (media->reset_interval)
    dri_hdr_len = RTP_HDRSZ_DRI;
  // p_verb ("dir_hdr_len %d", dri_hdr_len);
  srcSize += jpg_bufsize;
  off = src = dst = 0;
#if (JR_APP_PROTOCOL==0)
  src = f->headLen;
  dri_hdr_len =0;
#endif
  while (1)
  {
  	if(stopStreamTrans)
		return 1;
    // 先處理目標緩衝
    if (0 == dst)
    {
      // todo : 改用frame_buffer_new是否規範些？
      struct list_head *newbuf = frameNewBuffer(r);
      if (NULL == newbuf)
      {
        // 這裏不用管釋放send_list，由packer統一處理
        // p_dbg("send_item_new NULL. heap=%X\n", heap);
        //printf("D3 ");
        return 1;
      }
	  
      si = list_entry(newbuf, struct send_item, list);
      packerSize += rtp_bufsize;
	  inter = si->buf.dat;
      inter[0] = 0;
      rtphdr = inter + RTP_HDRSZ_INTERLEAVE;
      jhdr = rtphdr + RTP_HDRSZ_RTPHDR;
      drihdr = jhdr + RTP_HDRSZ_JHDR;

      // 判斷是否首個
      // 注意一定需要加判0 == src，因為一個源可能複製到多個目標緩衝
      // 循環外面已經處理了首個源緩衝，所以不用擔心首個目標緩衝拿不到量化表
      //     if(buf->list.prev == &f->buflist && 0 == src) {
      #if JR_APP_PROTOCOL
      if(0 == off)
      {
        // 首個rtp包需要傳輸量化表
        hdr_len = dst = RTP_HDRSZ_INTERLEAVE + RTP_HDRSZ_RTPHDR + RTP_HDRSZ_JHDR + dri_hdr_len + RTP_HDRSZ_QHDR + RTP_HDRSZ_QTBL;

        /* Quantization table header */
        qhdr = jhdr + RTP_HDRSZ_JHDR + dri_hdr_len;
        qhdr[0] = 0; /* MBZ */
        qhdr[1] = 0; /* precision */
        /* 沒有私有數據，意味著沒有dri信息，當前把這個情況處理成需要擴展頭部信息
         * 該信息保存在兩個量化表後面，以兩個0前導，後面是dri信息，再接著是huffman表
         */
        if (f->private)
        {
          PUT_16(qhdr + 2, RTP_HDRSZ_QTBL); /* length */
        }
        else
        {
          PUT_16(qhdr + 2, RTP_HDRSZ_QTBL); /* length */
          // dst += media->hufflen + EXTHDR_LEN;
          // hdr_len = dst;
          // PUT_16(qhdr+2, RTP_HDRSZ_QTBL + media->hufflen + EXTHDR_LEN); /* length */
          // printf("len:%d\r\n",RTP_HDRSZ_QTBL + media->hufflen + EXTHDR_LEN);
          // memcpy(qhdr+4+RTP_HDRSZ_QTBL, media->exthdr, EXTHDR_LEN);
          // memcpy(qhdr+4+RTP_HDRSZ_QTBL+EXTHDR_LEN, dat+media->huffoff, media->hufflen);
        }
        memcpy(qhdr + 4, dat + media->quant[media->luma_table], 64);
        memcpy(qhdr + 68, dat + media->quant[media->chroma_table], 64);

        src = media->scan_data;

        if (0)
        {
          UINT32 *ptr = dat + 0x260;
          p_dbg("%08X %08X %08X %08X", ptr[0], ptr[1], ptr[2], ptr[3]);
        }
      }
      else
	  #endif
      {
        hdr_len = dst = RTP_HDRSZ_INTERLEAVE + RTP_HDRSZ_RTPHDR + RTP_HDRSZ_JHDR + dri_hdr_len;
      }

      // 填寫rtp包頭信息
      PUT_32(rtphdr + 4, timestamp); // todo: timestamp如何傳遞

      /* There's at least 8-byte video-specific header
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      | Type-specific |              Fragment Offset                     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |      Type     |       Q       |     Width     |     Height    |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      */
      /* Main JPEG header */
      jhdr[0] = 0; /* type-specific, value 0 -> non-interlaced frame */
      /* vhdr[1..3] is the fragment offset */
      jhdr[1] = (off >> 16) & 0xFF;
      jhdr[2] = (off >> 8) & 0xFF;
      jhdr[3] = (off)&0xFF;
	  #if (JR_APP_PROTOCOL==0)
      jhdr[4] = 1;//media->type; // 1:420 0:422, 0x40:dri
      jhdr[5] = 1;//255;         /* Q, value 255 -> dynmic quant tables */
      jhdr[6] = f->imageInfor.width>>3;
      jhdr[7] = f->imageInfor.hight>>3;
      #else
	  jhdr[4] = media->type; // 1:420 0:422, 0x40:dri
      jhdr[5] = 255;
	  jhdr[6] = media->width;
      jhdr[7] = media->height;
	  #endif
      
      // memcpy(&jhdr[8],test,4);

    
    }

    // 計算長度
    plen = rtp_bufsize - dst;
    if (plen > jpg_bufsize - src)
      plen = jpg_bufsize - src;
    if (plen > frame_len - off)
      plen = frame_len - off;
    // printf("inter+dst:%X\tjhdr[8]:%X\r\n",inter+dst,&jhdr[8]);
    // uartPrintfBuf(jhdr, 12);
    //  複製數據
    memcpy(inter + dst, dat + src, plen);
    // uartPrintfBuf(jhdr, 12);
    if (0) // if(dst == RTP_HDRSZ_INTERLEAVE + RTP_HDRSZ_RTPHDR + RTP_HDRSZ_JHDR + dri_hdr_len + RTP_HDRSZ_QHDR + RTP_HDRSZ_QTBL)
    {
      UINT32 *ptr = inter + dst;
      p_dbg("%08X %08X %08X %08X plen=%d off=%d", ptr[0], ptr[1], ptr[2], ptr[3], plen, dst);
    }

    // 更新目標偏移量
    dst += plen;
    si->buf.len = dst;
    if (dst >= rtp_bufsize)
    {
      if (dri_hdr_len)
      {
        // UINT32 drival = 0;
        // if (dri_len)
        // drival = cal_drihdr(&dri_index, dst - hdr_len, &dri_len[1], &res_len);
        // PUT_32(drihdr, drival);
        memcpy(&jhdr[8], media->exthdr + EXTHDROFF_DRI, 2);
        memcpy(&jhdr[10], Restart_Count, 2);
      }
      dst = 0;
    }

    off += plen;
    src += plen;
    if (frame_len <= off)
      break;

    // uartPrintfBuf(jhdr, 12);

    // 更新源偏移量及處理源緩衝
    if (src >= jpg_bufsize)
    {
      if (buf->list.next == &f->buflist)
      {
        break;
      }
      buf = list_entry(buf->list.next, struct frame_buf, list);
      srcSize += jpg_bufsize;
      dat = buf->dat;
      src = 0;
    }
  }

  // Restart Marker，前2字节在jpeg文件中，后面是2字节为0xff,0xff(未知具体作用)
  if (dst && dri_hdr_len)
  {
    // UINT32 drival = 0;
    // if (dri_len)
    // drival = cal_drihdr(&dri_index, dst - hdr_len, &dri_len[1], &res_len);
    // PUT_32(drihdr, drival);	media->reset_interval
    memcpy(&jhdr[8], media->exthdr + EXTHDROFF_DRI, 2);
    memcpy(&jhdr[10], Restart_Count, 2);
  }
  //  printf("srcSize:%dKB,rtpSize:%dKB\n", srcSize>>10,packerSize>>10);
  // p_verb("jpg_pack jpeg %X", (unsigned)f);
  return 0;
}

#if DEMO_USER_PACK == 1

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief pack frame f to send_list, allocated send_item from heap
 * @param f         源frame，原图的帧
 * @param r         在这里不需要用到,保持兼容
 * @param user      客户的结构体,尽量保持与原来pack的兼容,因此使用void指针
 * @retval -1 failure	不删除frame r,由外部删除
 * @retval  0 OK			不删除frame r		,外部删除
 */

static int user_pack(struct frame *r, void *d, struct frame *f)
{
  // printf("user_pack!\r\n");
  struct user *userdata = (struct user *)d;
  userdata->len = jpg_write_to_buf(userdata->picture, f);
  userdata->flag = 1;
  // printf("user_pack len:%d\r\n",userdata->len);
  return 0;
}

#endif

////////////////////////////////////////////////////////////////////////////////
static struct media_jpeg media_ops = {
    .m = {
        .get_sdp = get_sdp,
        .get_payload = get_payload,
        .pack = jpg_pack,
#if DEMO_USER_PACK == 1
        .userpack = user_pack
#else
        .userpack = NULL // 如果需要测试客户模式的话,赋值user_pack,这里只是提供测试,需要填回NULL
#endif
    },
    .need_scan = 1};

void rtp_jpeg_register(void)
{
  media_register("jpeg", &media_ops);
}

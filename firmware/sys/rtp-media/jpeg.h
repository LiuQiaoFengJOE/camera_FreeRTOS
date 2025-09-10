#ifndef __MEDIA_JPEG_H__
#define __MEDIA_JPEG_H__

#include "rtsp/rtsp.h"

#define EXTHDR_LEN 6
#define EXTHDROFF_MAGIC 0
#define EXTHDROFF_VER 1
#define EXTHDROFF_DRI 2
#define EXTHDROFF_HUFF 4

struct jpeg_component
{
  uint8_t blocks_per_mcu_h;
  uint8_t blocks_per_mcu_v;
  uint8_t quant_table_id;
  uint8_t huffman_dc_table_id;
  uint8_t huffman_ac_table_id;
};

struct media_jpeg
{
  struct media_ops m;

  uint8_t need_scan; //< 下列數據需要重新掃描才能使用

  // 下面成員記錄掃描結果
  uint8_t type;               //< bit0:{422,420} bit6:DRI
  int8_t luma_table;          //< Y分量所用的量化表序號
  int8_t chroma_table;        //< CbCr分量所用的量化表序號
  uint16_t width;             //< 圖片寬
  uint16_t height;            //< 圖片高
  uint16_t scan_data;         //< 有效數據的起始位置
  uint16_t huffoff;           //< Huffman表距離frame頭的偏移
  uint16_t hufflen;           //< 整個Huffman表的長度
  uint16_t quant[16];         //< 每個量化表距離frame頭的偏移量
  uint16_t huff[4];           //< 每個Huffman表距離frame頭的偏移量
  uint8_t exthdr[EXTHDR_LEN]; //< 擴展頭部的信息
  uint16_t reset_interval;    //< dri間隔，即多少個MCU會產生一個dri
  struct jpeg_component component[3];
};

#endif /* __MEDIA_JPEG_H__ */

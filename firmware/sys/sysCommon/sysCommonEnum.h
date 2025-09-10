#ifndef __SYSCOMMONENUM_H
#define __SYSCOMMONENUM_H
enum
{
  WM_NONE,
  WM_RECORD,
  WM_WIFI,
  WM_LIVE,
  WM_WEBFILE,
  WM_PLAYBACK,
  WM_USB,
  // WM_INTERCOM,
  // WM_INTERCOM_TCP,
  WM_MAX
};

enum
{
  AVI_NONE,
  AVI_REC,
  AVI_JPG,
};

// 与avisrc.c中一样，以后需要放在同一个头文件才行
enum
{
  CMD_NONE,
  CMD_PAUSE,
  CMD_RESUME,
  CMD_STOP,
  CMD_BROWSERPHOTO,
  CMD_PLAY_PCM
};

enum
{
  LOW,
  HIGH
};

// 照片水印
enum
{
  WATERMARK_DATE,
  WATERMARK_DATE_THUMB,
  WATERMARK_RETAIN2
};
#endif

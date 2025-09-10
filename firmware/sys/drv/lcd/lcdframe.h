#include "typedef.h"
#include "board_config.h"


#ifndef HAL_LCDFRAME_H
#define HAL_LCDFRAME_H

enum frame_type_u {
    FTYPE_VIDEO_ROTATE, // 0
                        //video rotate
                        //malloc from CSI中断/回放等非中断
                        //free from rotate中断
    FTYPE_VIDEO_DISPLAY,// 1
                        //video display
                        //malloc from CSI中断/回放等
                        //free from rotate中断，当de有新buffer时，free旧的的buffer
    FTYPE_OSD_ROTATE,   // 2
                        //osd rotate
                        //malloc from UI画图
                        //free from 1.rotate中断，2.当de有新buffer时，free旧的的buffer
    FTYPE_OSD_COMPRESS, // 3
                        //osd compress

    FTYPE_OSD_DISPLAY,  // 4
                        //osd dispaly
                        //malloc from rotate中断
                        //free from 1.osdcmp中断，2.当de有新buffer时，free旧的的buffer
};

typedef struct frame_s {
    struct frame_s * next;
    u8 * y_addr,* uv_addr;
    u8 * _y_addr,* _uv_addr;
    void (*priv_func)(struct frame_s *);
    u32 buf_size; //y & uv total size
    u32 data_size;//for osd
    u16 w,h;
	u16 posX,posY;
    u16 stride;
    u8  scan_mode;
    u8  id;
    u8  pip_sta;
    u8  buf_sta;
    u8  frame_type;
} disp_frame_t;

typedef struct disp_frames_s 
{
    disp_frame_t _frames[3];
    disp_frame_t * freelist;
    u8 frame_nums;
    u8 frame_type;
} disp_frames_t;

struct disp_frames_mgr_s
{
    disp_frames_t types[5];
};
extern struct disp_frames_mgr_s dfm;
void lcdframeInit(void);
void lcdframeVideoCfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height);
disp_frame_t * lcdframeMalloc(u8 type);
disp_frame_t *lcdVideoFrameMalloc(void);
disp_frame_t *lcdOsdFrameMalloc(void);
void lcdframeFree(disp_frame_t * p_frame);

#define OSD_SOFTWARE_ROTATE  1


#endif

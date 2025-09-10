#ifndef _USER_MEDIA_H_
#define _USER_MEDIA_H_

#define YUV_ALPHA_Y	0x8A
#define YUV_ALPHA_UV	0x81

enum
{
	CAM_EFFECT_0=0,		//  buf flip
	CAM_EFFECT_1,			//  Prismatic Mirror
	CAM_EFFECT_2,			//  ConcavelensMirror
	CAM_EFFECT_3,			//  ConvexlensMirror
	CAM_EFFECT_4,			//  trilateral_16_window
	CAM_EFFECT_5,			//  stream_10_window
	CAM_EFFECT_6,			//  rismatic_multiwindow
	CAM_EFFECT_7,			//  hexagon_multiwindow

	CAM_EFFECT_MAX
};

typedef struct res_img_s{
	u8  img_type; //0,bmp; 1,jpg
	u16 width;
	u16 height;
	u8* yuvbuf_ptr;	
	u32 yuvbuf_len;
	u32 mem_base;
}res_img_t;

//note: mem_base 如果有效，则不会自动申请及释放
int img_load(res_img_t* img, u16 res_id, u8 img_type, u16 w, u16 h,  u32 mem_base);
int img_release(res_img_t* img);
int img_direct_show(res_img_t* img, u16 x, u16 y, u16 w, u16 h);


int dec_jpg_res_to_yuvbuf(u32 res_id,u8 *yuv_buf, u16 w, u16 h);
int dec_jpg_buf_to_yuvbuf(u8* jpg_buf, u32 jpg_size, u8 *yuv_buf, u16 w, u16 h);

u8   bmp24_to_yuv420_buf(u32 id,u8 *rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h);
void yuv420_draw_buf(u8 *dst_ybuf,u16 dst_w,u16 dst_h,s16 draw_x,s16 draw_y,u16 draw_w,u16 draw_h,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y,u8 alpha_uv);
void yuv420_effect_handle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h);
u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h);
#endif

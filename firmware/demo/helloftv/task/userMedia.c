#include "task/app.h"
#include "drv/lcd/lcdframe.h"
#include "drv/jpeg/jpeg.h"
//#include "log.h"
#include "userMedia.h"
#include "application.h"

//==========
#define BYTE_TUNE(r) (r<0?0:(r>255?255:r)) 

//==rgb2yuv table==
static  u8 Y_R[256]; 
static  u8 Y_G[256]; 
static  u8 Y_B[256]; 

static  u8 U_R[256]; 
static  u8 U_G[256]; 
static  u8 U_B[256]; 

static  u8 V_R[256]; 
static  u8 V_G[256]; 
static  u8 V_B[256]; 

void yuv_rgb_table()
{ 
	u32 i; 
	static u8 init_one_time=0;
	if(0==init_one_time)
	{
		init_one_time=1;
		for(i = 0;i<256;i++) 
		{ 
		   //==rgb2yuv table==
		   Y_R[i]=(i*1224)>>12;
		   Y_G[i]=(i*2404)>>12;
		   Y_B[i]=(i*467)>>12;

		   U_R[i]=(i*602)>>12;
		   U_G[i]=(i*1183)>>12;
		   U_B[i]=(i*1785)>>12;

		   V_R[i]=(i*2519)>>12;
		   V_G[i]=(i*2109)>>12;
		   V_B[i]=(i*409)>>12;
		} 
	}
} 

static void rgb24_to_yuv420(u8* rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u8 r,g,b,r1,g1,b1; 
	s16 y,u,v,y1,u1,v1;
	u8 *rgb_line_buf,*temp_buf;
	u32 i,j;
	u32 line_align;
	line_align=(w*3+3)&~0x3;	// 4bytes align

	rgb_line_buf=rgb_buf+(h-1)*line_align; // last line
	for(j=0;j<h;j+=2)
	{
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 

			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //u = (U_B[b]-U_R[r]-U_G[g])+128; 
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 u = (U_B[b]-U_R[r]-U_G[g])+128; 

			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128;
			 //v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;
			 v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;

			 *ybuf++ = BYTE_TUNE(y);
			 *uvbuf++ = BYTE_TUNE(u);
			 *ybuf++ = BYTE_TUNE(y1);
			 *uvbuf++ = BYTE_TUNE(v1);
		}

		rgb_line_buf=rgb_line_buf-line_align;
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 
		
			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128; 
			 *ybuf++ = BYTE_TUNE(y);
			 *ybuf++ = BYTE_TUNE(y1);
		}
		rgb_line_buf=rgb_line_buf-line_align;

	}
}

u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
#if 0
	Image_ARG_T arg;
	u16 dec_w,dec_h;
	arg.target.width=w;
	arg.target.height=h;
	arg.yout  = ybuf;
	arg.uvout = uvbuf;
	arg.media.type = MEDIA_SRC_NVFS;
	arg.media.src.fd = (FHANDLE)jpg_id;
    arg.wait = 1;  // wait decode end
#if 0
	if(imageDecodeStart(&arg)<0)
	{
		deg_Printf("image decode fail.\n");
		return 1;
	}
#else
		//if(imageDecodeStart(&arg)<0)
		
		u8 * jpegBuffer = NULL;
		INT32U size,addr;
		addr = nv_open((int)arg.media.src.fd);
		if(addr<0)
		{
			deg_Printf("decode fail:nvopen addr<0\n");
			return STATUS_FAIL;
		}
			

		jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(arg.media.src.fd),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = nv_size((int)arg.media.src.fd);
			nv_read(addr,jpegBuffer,size);					
		}

		not_isr_jpgdec2();
		if(decodePic(jpegBuffer,(u8*)arg.yout,(u8*)arg.uvout,arg.target.width,arg.target.height)<0)
		{
			deg_Printf("layout logo : image decode fail.\n");
			if(jpegBuffer)
				hal_sysMemFree(jpegBuffer);
//			HAL_CRITICAL_EXIT();
			return -1;
		}
		if(jpegBuffer)
				hal_sysMemFree(jpegBuffer);

#endif


	imageDecodeGetResolution(&dec_w,&dec_h);
	deg_Printf("dec_w=%d,dec_h=%d\n",dec_w,dec_h);

#else
		u8 * jpegBuffer = NULL;
		INT32U size,addr;
		addr = nv_open((int)jpg_id);
		if(addr<0)
		{
			deg_Printf("decode fail:nvopen addr<0\n");
			return -1;
		}
			

		jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(jpg_id)+64,64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
		if(jpegBuffer)
		{
			size = nv_size((int)jpg_id);
			nv_read(addr,jpegBuffer,size);					
		}
		else
		{
			deg_Printf("decode fail:nvopen addr<0\n");
			return -1;
		}
		
		printf("jk %s, %d ,size:%d, w:%d, h:%d\n", __func__, __LINE__, size, w, h);
		not_isr_jpgdec2(ybuf,jpegBuffer,size,w,h); //图片的原尺寸要大于等于目标尺寸
		if(jpegBuffer)
				hal_sysMemFree(jpegBuffer);
		printf("jk decode over\n");

#endif
	

	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}

//u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
int dec_jpg_res_to_yuvbuf(u32 res_id,u8 *yuv_buf, u16 w,u16 h)
{
	u32 ures_addr = res_open(res_id);
	u32 ures_size = res_size(res_id);

//	printf("res_addr=%d, res_size:%d\r\n", ures_addr, ures_size);
	void *jpgbuf = osal_malloc(ures_size+64);
	if(jpgbuf==NULL)
	{
		printf("malloc jpgbuf:%d error\r\n", ures_size);
		return -1;
	}
	res_read(ures_addr, jpgbuf, ures_size);
	
	dcache_writeback_region((u32)jpgbuf, ures_size);

	not_isr_jpgdec2(yuv_buf, jpgbuf, ures_size, w, h);
	
	osal_free(jpgbuf);

	return 0;
}


//u8 jpg_buf_decode_buf(u8* jpg_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
int dec_jpg_buf_to_yuvbuf(u8* jpg_buf, u32 jpg_size, u8 *yuv_buf, u16 w, u16 h)
{
	return not_isr_jpgdec2(yuv_buf, jpg_buf, jpg_size, w, h);
}

int img_direct_show(res_img_t* img, u16 x, u16 y, u16 w, u16 h)
{

	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		printf("malloc lcdframe error\r\n");
		return -1;
	}
	lcdframeVideoCfg(lcdFrame, x, y, w, h);

	memcpy(lcdFrame->y_addr,img->yuvbuf_ptr, img->yuvbuf_len);
	
	int cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	lcdDealVideBuff(lcdFrame);
	
	return 0;
}


//img_type: 0,bmp;   1,jpg;
int img_load(res_img_t* img, u16 res_id, u8 img_type, u16 w, u16 h, u32 mem_base)
{
	img->img_type = img_type;
	img->width = w;
	img->height = h;
	img->mem_base = mem_base;
	if(img_type==1)
	{
		img->yuvbuf_len=w*h*3/2;

		if(!mem_base)
		{
			img->yuvbuf_ptr=osal_malloc(img->yuvbuf_len);
		}
		else
		{
			img->yuvbuf_ptr=mem_base;
		}
		if(NULL!=img->yuvbuf_ptr)
		{
			dec_jpg_res_to_yuvbuf(res_id,img->yuvbuf_ptr, w, h);
		}
		else
		{
			printf("mem err!\n");
			return -1;
		}
	}
	else
	{


		img->yuvbuf_len=w*h*3/2;
		if(!mem_base)
		{
			img->yuvbuf_ptr=osal_malloc(img->yuvbuf_len);
		}
		else
		{
			img->yuvbuf_ptr=mem_base;
		}
		if(NULL!=img->yuvbuf_ptr)
		{

			u32 line_align;
			line_align=(w*3+3)&~0x3;	// 4bytes align
			printf("res_id:%d, w=%d, line_align=%d\n", res_id, w, line_align);

			u32 ures_addr = res_open(res_id);
			u32 ures_size = res_size(res_id);
			if(ures_size < 54)
			{
				printf("res size to small!!!");
				return -2;
			}
			ures_size-=54;//skip bmp header

			if(ures_size> line_align*h)
			{
				ures_size=line_align*h;
			}
			else if(ures_size < w*h*3)
			{
				printf("please sure res fmt is bmp24!!!!, %d, %d, %d,%d \n", ures_size, w*h*3, w, h);
				return -3;
			}
			
			u8* bmp24 = osal_malloc(ures_size);
			if(bmp24)
			{
				printf("bmp res %d, addr=%x, size=%d\n", res_id, ures_addr, ures_size);
				res_read(ures_addr+54, bmp24, ures_size);//skip bmp header
				dcache_writeback_region((u32)bmp24, ures_size);

				printf("conv bmp to yuv, w=%d, h=%d...\n", w,h);
				rgb24_to_yuv420(bmp24, img->yuvbuf_ptr, img->yuvbuf_ptr+(w*h), w, h);

				osal_free(bmp24);
			}
			else
			{
				printf("mem err, malloc bmp24 buf failed!\n");
				return -1;
			}
		}
		else
		{
			printf("mem err!\n");
			return -1;
		}
	}
	
	return 0;
}

int img_release(res_img_t* img)
{
	if(NULL!=img)
	{
		if(!img->mem_base)
		{
			if(img->yuvbuf_ptr)
			{
				printf("release img buf...08%x", img->yuvbuf_ptr);
				osal_free(img->yuvbuf_ptr);
				img->yuvbuf_ptr=NULL;
			}
		}
		memset(img, 0, sizeof(res_img_t));
		return 0;
	}
	return -1;
}



u8 bmp24_to_yuv420_buf(u32 id,u8 *rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u32	addr,size;
	u32 line_align;
	line_align=(w*3+3)&~0x3;	// 4bytes align

	addr= res_open(id);
	if(addr<0)
	{
		printf("read id err\n");
		return 1;
	}
	size = res_size(id);
	if(size<54)
	{
		printf("size err\n");
		return 1;
	}
	size=size-54;	// offset bmp header
	
	if(size>line_align*h)	// not over rgb_buf size
	{
		size=line_align*h;
	}
	else if(size<w*h*3)
	{
		printf("bmp format err?\n");
	}
	
	res_read(addr+54,rgb_buf,size);	// offset bmp header
	//printf("data size=%d,w*h*3=%d\n",size-54,w*h*3);
	rgb24_to_yuv420(rgb_buf,ybuf,uvbuf,w,h);
	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}



//==tips: draw_x,drawy,src_w,src_h is 2X size,X is pixel num==
void yuv420_draw_buf(u8 *dst_ybuf,u16 dst_w,u16 dst_h,s16 draw_x,s16 draw_y,u16 draw_w,u16 draw_h,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y,u8 alpha_uv)
{
	u16 i,j;
	u16 draw_offset_x,draw_offset_y;
	u8 *dy,*duv,*sy,*suv;

	draw_x&=~0x1;		// align
	draw_y&=~0x1;		// align
	draw_w&=~0x1;		// align
	draw_h&=~0x1;		// align

	if((draw_x+draw_w<=0)||(draw_y+draw_h<=0)||(draw_x>=dst_w)||(draw_y>=dst_h))	// draw nothing
	{
		return;
	}

	if(draw_x<0)
	{
		draw_offset_x=-draw_x;
		draw_x=0;
		//printf("draw_offset_x=%d\n",draw_offset_x);
	}
	else
	{
		draw_offset_x=0;
	}
	
	if(draw_y<0)
	{
		draw_offset_y=-draw_y;
		draw_y=0;
		//printf("draw_offset_y=%d\n",draw_offset_y);
		// not finish!!
	}
	else
	{
		draw_offset_y=0;
	}

	if(draw_x+src_w>dst_w)
	{
		draw_w=dst_w-draw_x;
	}
	else
	{
		draw_w=src_w-draw_offset_x;
	}

	if(draw_y+src_h>dst_h)
	{
		draw_h=dst_h-draw_y;
	}
	else
	{
		draw_h=src_h-draw_offset_y;
	}
	
	//printf("draw_x=%d,draw_y=%d,draw_w=%d,draw_h=%d\n",draw_x,draw_y,draw_w,draw_h);
	dy=dst_ybuf+dst_w*draw_y+draw_x;
	duv=dst_ybuf+dst_w*dst_h+dst_w*draw_y/2+draw_x;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==
	for(j=0;j<draw_h;j+=2)
	{
		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}

		dy+=dst_w;
		sy+=src_w;

		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}
		
		dy+=dst_w;
		sy+=src_w;
		duv+=dst_w;
		suv+=src_w;

	}

}


void yuv420_effect_handle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h)
{
	u16 i,j;

	if(CAM_EFFECT_0==SysCtrl.photo_effect_idx)
	{
		u8 *y,*uv;
		u16 half_w=buf_w>>1;
		y=ybuf;
		uv=uvbuf;
		for(j=0;j<buf_h;j+=2)
		{
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}
			y+=buf_w;
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}

			for(i=0;i<half_w;i+=2)
			{
				*(uv+half_w-i-2)=*(uv+half_w+i);
				*(uv+half_w-i-1)=*(uv+half_w+i+1);
			}

			y+=buf_w;
			uv+=buf_w;
		}
	}
	else if(CAM_EFFECT_1==SysCtrl.photo_effect_idx){
		warpHandle(ybuf, uvbuf, buf_w, buf_h, NULL);
	}
	else if(CAM_EFFECT_2==SysCtrl.photo_effect_idx){
		if(180 == buf_h){
			u16 r[] = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 62, 63, 64, 65, 66, 66, 67, 68, 68, 69, 70, 70, 71, 72, 72, 73, 73, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 79, 80, 80, 81, 81, 82, 82, 82, 83, 83, 83, 84, 84, 84, 85, 85, 85, 85, 86, 86, 86, 86, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
		else if(720 == buf_h){
			u16 r[] = {197, 198, 199, 200, 201, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 253, 253, 254, 255, 256, 257, 257, 258, 259, 260, 260, 261, 262, 263, 264, 264, 265, 266, 266, 267, 268, 269, 269, 270, 271, 272, 272, 273, 274, 274, 275, 276, 276, 277, 278, 278, 279, 280, 280, 281, 282, 282, 283, 284, 284, 285, 286, 286, 287, 287, 288, 289, 289, 290, 290, 291, 292, 292, 293, 293, 294, 295, 295, 296, 296, 297, 297, 298, 299, 299, 300, 300, 301, 301, 302, 302, 303, 303, 304, 305, 305, 306, 306, 307, 307, 308, 308, 309, 309, 310, 310, 311, 311, 312, 312, 313, 313, 314, 314, 314, 315, 315, 316, 316, 317, 317, 318, 318, 319, 319, 319, 320, 320, 321, 321, 322, 322, 322, 323, 323, 324, 324, 324, 325, 325, 326, 326, 326, 327, 327, 328, 328, 328, 329, 329, 330, 330, 330, 331, 331, 331, 332, 332, 332, 333, 333, 333, 334, 334, 334, 335, 335, 335, 336, 336, 336, 337, 337, 337, 338, 338, 338, 339, 339, 339, 340, 340, 340, 340, 341, 341, 341, 342, 342, 342, 342, 343, 343, 343, 343, 344, 344, 344, 345, 345, 345, 345, 346, 346, 346, 346, 346, 347, 347, 347, 347, 348, 348, 348, 348, 349, 349, 349, 349, 349, 350, 350, 350, 350, 350, 351, 351, 351, 351, 351, 352, 352, 352, 352, 352, 352, 353, 353, 353, 353, 353, 353, 354, 354, 354, 354, 354, 354, 355, 355, 355, 355, 355, 355, 355, 355, 356, 356, 356, 356, 356, 356, 356, 356, 357, 357, 357, 357, 357, 357, 357, 357, 357, 357, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 358, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
	}
	else if(CAM_EFFECT_3==SysCtrl.photo_effect_idx){
		if(180 == buf_h){
			u16 r[] = {40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 23, 22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 16, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
		else if(720 == buf_h){
			u16 r[] = {162, 161, 160, 159, 158, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 124, 123, 122, 121, 120, 119, 118, 117, 116, 116, 115, 114, 113, 112, 111, 111, 110, 109, 108, 107, 106, 106, 105, 104, 103, 102, 102, 101, 100, 99, 99, 98, 97, 96, 95, 95, 94, 93, 93, 92, 91, 90, 90, 89, 88, 87, 87, 86, 85, 85, 84, 83, 83, 82, 81, 81, 80, 79, 79, 78, 77, 77, 76, 75, 75, 74, 73, 73, 72, 72, 71, 70, 70, 69, 69, 68, 67, 67, 66, 66, 65, 64, 64, 63, 63, 62, 62, 61, 60, 60, 59, 59, 58, 58, 57, 57, 56, 56, 55, 54, 54, 53, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 40, 39, 39, 38, 38, 37, 37, 37, 36, 36, 35, 35, 35, 34, 34, 33, 33, 33, 32, 32, 31, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 25, 24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			warpHandle(ybuf, uvbuf, buf_w, buf_h, r);
		}
	}
	else if(CAM_EFFECT_4==SysCtrl.photo_effect_idx){
		trilateral_16_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_5==SysCtrl.photo_effect_idx){
		stream_10_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_6==SysCtrl.photo_effect_idx){
		rismatic_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_7==SysCtrl.photo_effect_idx){
		hexagon_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	dcache_writeback_region((u32)ybuf,buf_w*buf_h*3/2);
}
/*******************************************************************************
* Function Name  : dispLayerGetFreeFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
void *dispLayerGetFreeFrame(INT8U layer)
{
	void * res = NULL;
	#if 0
	disp_frame_t * osd_frame = NULL;

	osd_frame = getOsdDrawFrame();
	//osd_frame->w = 480;
	//osd_frame->h = 640;
	res = osd_frame->y_addr;
	#else
	res = getOsdBuff();
	#endif
	return res;
}
/*******************************************************************************
* Function Name  : dispLayerSetFrame
* Description    : display show for osd0,osd1
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT32U addr: buffer to show
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerSetFrame(INT8U layer,u32 addr)
{
#if 0//PROJ_DISPLAY_OLD

	if(layer>=DISP_LAYER_MAX)
		return -1;

	if((layer == DISP_LAYER_OSD0) || (layer == DISP_LAYER_OSD1))
	{
		hal_osdSetBuff(OSD0,addr);
	}

#else
	lcdOsdEn(1);
	lcdOsdShow();
	
	//lcdOsdDeal();

#endif
	return 0;
}


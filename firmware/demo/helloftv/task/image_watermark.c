
//#include "../media.h"
//#include "../multimedia.h"
//#include "../../hal/inc/hal_mjpeg.h"
//#include "../../ax32_platform_demo/application.h"
#include "application.h"
#include "userMedia.h"
#include "image_watermark.h"

#if 1

//------for watermark
//=========for watermark==========

 //=========for watermark==========

static u16 id_w_st,id_h_st;

static u16 wm_pixel_w,wm_pixel_h;
static u8  wm_pic_num;


extern u8* small_pic_id_buf[20];
extern u32  small_pic_id[20];


static int watermark_bmp2yuv_set(u8* ydst_buf,u16 dst_w,u16 dst_h,u16 pos_x,u16 pos_y,char time_str)
{
	u8 timestemp_idx;  //time stemp str idx

	if(time_str == '0')
		timestemp_idx =0;
	else if(time_str == '1')
		timestemp_idx =1;
	else if(time_str == '2')
		timestemp_idx =2;
	else if(time_str == '3')
		timestemp_idx =3;
	else if(time_str == '4')
		timestemp_idx =4;
	else if(time_str == '5')
		timestemp_idx =5;
	else if(time_str == '6')
		timestemp_idx =6;
	else if(time_str == '7')
		timestemp_idx =7;
	else if(time_str == '8')
		timestemp_idx =8;
	else if(time_str == '9')
		timestemp_idx =9;
	else if(time_str == '/')
		timestemp_idx =10;
	else if(time_str == ':')
		timestemp_idx =11;
	else if(time_str == ' ')
		timestemp_idx =12;

	{
		yuv420_draw_buf(ydst_buf,dst_w,dst_h,pos_x,pos_y,id_w_st,id_h_st,small_pic_id_buf[timestemp_idx],id_w_st,id_h_st,0x7f,YUV_ALPHA_UV);
		ax32xx_sysDcacheWback((u32)ydst_buf,dst_w*dst_h*3/2);
	}

	return 0;
}


/*******************************************************************************
* Function Name  : watermark_bmp2yuv_init
* Description    : draw watermark init 
* Input          : 

* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_init(u16 pixel_w,u16 pixel_h,u8 pic_num)
{
	u8 i;
	u8* rgb24_buf_ts;

	yuv_rgb_table();
	
	wm_pixel_w=pixel_w;
	wm_pixel_h=pixel_h;
	wm_pic_num=pic_num;
	printf("in lib!!\n");
	printf("wm-bmp2yuv-init w:%d h:%d num:%d\n",wm_pixel_w,wm_pixel_h,wm_pic_num);
	id_w_st=(wm_pixel_w+0x1)&(~0x1);	// bmp must 2pixel align
	id_h_st=(wm_pixel_h+0x1)&(~0x1);
	
	rgb24_buf_ts=hal_sysMemMalloc(id_w_st*id_h_st*3,32);
	if(NULL==rgb24_buf_ts)
	{
		printf("mem err!\n");
		return;
	}

	for(i=0;i<wm_pic_num;i++)
	{
		small_pic_id_buf[i]=hal_sysMemMalloc(id_w_st*id_h_st*3/2,32);
		if(NULL!=small_pic_id_buf[i])
		{
			bmp24_to_yuv420_buf(small_pic_id[i],rgb24_buf_ts,small_pic_id_buf[i],small_pic_id_buf[i]+id_w_st*id_h_st,id_w_st,id_h_st);
			printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*small_pic_id_buf[i],*(small_pic_id_buf[i]+id_w_st*id_h_st),*(small_pic_id_buf[i]+1),*(small_pic_id_buf[i]+id_w_st*id_h_st+1));
		}
		else
		{
			printf("mem err!\n");
			return;
		}
	}

	if(NULL!=rgb24_buf_ts)
	{
		printf("free rgb24_buf_ts\n");
		hal_sysMemFree(rgb24_buf_ts);
		rgb24_buf_ts=NULL;
	}

}


/*******************************************************************************
* Function Name  : watermark_bmp2yuv_draw
* Description    : draw watermark on photo 
* Input          : u8* ybuf_src: src
				   u16 pos_x: X position
				   u16 pos_y: Y position
 					u16 gap  :	character gap

* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_draw(u8* ybuf_src,u16 pic_w, u16 pic_h, u16 pos_x, u16 pos_y,u16 gap)
{
 	 int i,w_offset=0;
	char *rtctime_pic;
 	rtctime_pic=hal_rtcTime2String(hal_rtcTimeGet()); //get rtc time 
//	printf("pos_x:%d pos_y:%d\n",pos_x,pos_y);
//	printf("in wm_dram:");
	for(i=0;i<19;i++)
	{
	//	printf("%c\t",*(rtctime_pic+i));
		watermark_bmp2yuv_set(ybuf_src,pic_w,pic_h,pos_x+w_offset,pos_y,*(rtctime_pic+i));
		w_offset=w_offset+wm_pixel_w+gap;
	}

}

/*******************************************************************************
* Function Name  : watermark_buf_bmp2yuv_free
* Description    : watermark_buf_free
* Input          : None
                     
* Output         : None
* Return         : None
*******************************************************************************/
void watermark_buf_bmp2yuv_free(void)
{
	int i;
	
	//--------free	 timestemp	buf----------------------------
		for(i=0;i<wm_pic_num;i++)
		{
			if(NULL!=small_pic_id_buf[i])
			{
				hal_sysMemFree(small_pic_id_buf[i]);
				small_pic_id_buf[i]=NULL;
			}
		}
	//-------------------------------------------

}


#endif








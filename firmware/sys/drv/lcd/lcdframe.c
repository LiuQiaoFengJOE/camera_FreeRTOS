#include "lcdframe.h"
#include "drv/lcd/lcdstruct.h"

struct disp_frames_mgr_s dfm;
#define  USE_HW_ROTATE_MODULE    1
#if USE_HW_ROTATE_MODULE
static void frames_vr_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_VIDEO_ROTATE;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h;

    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        stride  = (w + 0x1f) & (~0x1f);
        y_size  = stride * h;
        size    = y_size * 3 / 2;
        
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = sysMemMalloc(size,64);
			dcache_flush_region((u32)p_frame->y_addr,size);
            p_frame->uv_addr    = &p_frame->y_addr[y_size];
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
			p_frame->posX       =0;
			p_frame->posY       =0;
			//p_frame->configLcdEn=1;
            p_frame->stride     = stride;
            p_frame->scan_mode  = LCD_ROTATE_90;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}
static void frames_vd_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_VIDEO_DISPLAY;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h, _y_ofs, _uv_ofs;

    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
        {
            w   = disp_h;
            h   = disp_w;
            if(scan_mode == LCD_ROTATE_90)
                scan_mode = LCD_ROTATE_0;
            else
                scan_mode = LCD_ROTATE_180;
        }
        stride  = (w + 0x1f) & (~0x1f);
        y_size  = stride * h;
        size    = y_size * 3 / 2;
        //hal_lcdAddrCalculate(w,h,scan_mode,stride,&_y_ofs,&_uv_ofs);
        lcdc_caloff2(scan_mode, w, h, &_y_ofs, &_uv_ofs);
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = sysMemMalloc(size,64);
			dcache_flush_region((u32)p_frame->y_addr,size);
            p_frame->uv_addr    = &p_frame->y_addr[y_size];
            p_frame->_y_addr    = &p_frame->y_addr[_y_ofs];
            p_frame->_uv_addr   = &p_frame->uv_addr[_uv_ofs];
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
			p_frame->posX       =0;
			p_frame->posY       =0;
			//p_frame->configLcdEn=1;
            p_frame->stride     = stride;
            p_frame->scan_mode  = scan_mode;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

#else
static void frames_vd_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_VIDEO_DISPLAY;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h, _y_ofs, _uv_ofs;

    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        stride  = (w + 0x1f) & (~0x1f);
        y_size  = stride * h;
        size    = y_size * 3 / 2;
        //hal_lcdAddrCalculate(w,h,scan_mode,stride,&_y_ofs,&_uv_ofs);
		lcdc_caloff2(scan_mode, w, h, &_y_ofs, &_uv_ofs);
        
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        printf("lcd frame w:%d,h:%d\n", w,h);
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = sysMemMalloc(size,64);
			dcache_flush_region((u32)p_frame->y_addr,size);
            p_frame->uv_addr    = &p_frame->y_addr[y_size];
            p_frame->_y_addr    = &p_frame->y_addr[_y_ofs];
            p_frame->_uv_addr   = &p_frame->uv_addr[_uv_ofs];
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
			p_frame->posX       =0;
			p_frame->posY       =0;
			//p_frame->configLcdEn=1;
            p_frame->stride     = stride;
            p_frame->scan_mode  = scan_mode;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}
#endif
static void frames_od_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_OSD_DISPLAY;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size;
	u16 x,y,lcdW,lcdH;
    lcdGetOsdPos(&x,&y);
	lcdGetRes(&lcdW,&lcdH);
    if(buf_nums)
    {
    	if(scan_mode==LCD_ROTATE_90)
		{
			u16 temp = 	x;	
			x		=y;
			y		=lcdW-temp-disp_w;
		}
		else if(scan_mode==LCD_ROTATE_270)
		{
			u16 temp = 	x;	
			x		=lcdH-y-disp_h;
			y		=temp;
		}
		else if(scan_mode==LCD_ROTATE_180)
		{
			x       =lcdW-x-disp_w;
			y       =lcdH-y-disp_h;
		}
        stride  = disp_w;
        y_size  = stride * disp_h ;
        size    = y_size;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;       
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = sysMemMalloc(size,64);
            p_frame->uv_addr    = NULL;
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
			if(scan_mode==LCD_ROTATE_90||scan_mode==LCD_ROTATE_270)
			{
	            p_frame->w          = disp_h;
	            p_frame->h          = disp_w;
			}
			else
			{
	            p_frame->w          = disp_w;
	            p_frame->h          = disp_h;
			}
			p_frame->posX       =x;
			p_frame->posY       =y;
            p_frame->stride     = stride;
            p_frame->scan_mode  = LCD_ROTATE_0;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

void lcdframeInit(void)
{
	u16 lcdW,lcdH,osdW,osdH;
	u8 scan_mode=lcdGetScanMode();
	lcdGetRes(&lcdW,&lcdH);
	lcdGetOsdRes(&osdW,&osdH);
    memset(&dfm,0,sizeof(dfm));
#if LCD_EN
#if USE_HW_ROTATE_MODULE
	if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
	{
		dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 1;
    	dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 2;
	}
	else
	{
		dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 0;
    	dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 3;
	}
#else
    dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 0;
    dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 3;
#endif

    dfm.types[FTYPE_OSD_ROTATE].frame_nums    = 0;
    dfm.types[FTYPE_OSD_COMPRESS].frame_nums  = 0;
    dfm.types[FTYPE_OSD_DISPLAY].frame_nums   = 2;
	frames_od_init(osdW,osdH,scan_mode);

    frames_vr_init(lcdW,lcdH,scan_mode);
    frames_vd_init(lcdW,lcdH,scan_mode);
#endif
    //frames_or_init(osd_w,osd_h,scan_mode);
    //frames_oc_init(osd_w,osd_h,scan_mode);   
}
void lcdVidFrameFree(void)
{
#if LCD_EN
	int i;
	for(i=0;i<dfm.types[FTYPE_VIDEO_ROTATE].frame_nums;i++)
		sysMemFree(dfm.types[FTYPE_VIDEO_ROTATE]._frames[i].y_addr);
	for(i=0;i<dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums;i++)
		sysMemFree(dfm.types[FTYPE_VIDEO_DISPLAY]._frames[i].y_addr);
#endif
}
void lcdVidFrameRealloc(void)
{
#if LCD_EN
	u16 lcdW,lcdH,osdW,osdH;
	u8 scan_mode=lcdGetScanMode();
	lcdGetRes(&lcdW,&lcdH);
	memset(&dfm.types[FTYPE_VIDEO_ROTATE],0,sizeof(dfm.types[FTYPE_VIDEO_ROTATE]));
	memset(&dfm.types[FTYPE_VIDEO_DISPLAY],0,sizeof(dfm.types[FTYPE_VIDEO_DISPLAY]));
#if USE_HW_ROTATE_MODULE
	if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
	{
		dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 1;
		dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 2;
	}
	else
	{
		dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 0;
		dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 3;
	}
#else
	dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 0;
	dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 3;
#endif	
	frames_vr_init(lcdW,lcdH,scan_mode);
    frames_vd_init(lcdW,lcdH,scan_mode);
#endif
}

static void frames_vr_cfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
    u32 stride, y_size, size;
	if(p_frame->posX==posX&&p_frame->posY==posY&&p_frame->w==width&&p_frame->h==height)
		return;
    stride  = (width + 0x1f) & (~0x1f);
    y_size  = stride * height;
    size    = y_size * 3 >>1;
    p_frame->uv_addr    = &p_frame->y_addr[y_size];
    p_frame->buf_size   = size;
    p_frame->w          = width;
    p_frame->h          = height;
	p_frame->posX       =posX;
	p_frame->posY       =posY;
    p_frame->stride     = stride;
	//p_frame->configLcdEn=1;
}
static void frames_vd_cfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
    u32 stride, y_size, size, w, h, _y_ofs, _uv_ofs;
	INT16U lcdW=0,lcdH=0,x,y;
	u8 scan_mode=lcdGetScanMode();
	if(p_frame==NULL)
		return;
    lcdGetRes(&lcdW,&lcdH);
	if(scan_mode==LCD_ROTATE_0)
	{
		x       =posX;
		y       =posY;
	}
	else if(scan_mode==LCD_ROTATE_90)
	{
		x       =posY;
		y       =lcdW-posX-width;
	}
	else if(scan_mode==LCD_ROTATE_270)
	{
		x       =lcdH-posY-height;
		y       =posX;
	}
	else
	{
		x       =lcdW-posX-width;
		y       =lcdH-posY-height;
	}
	w       = width;
    h       = height;
#if USE_HW_ROTATE_MODULE
	if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
    {
        w   = height;
        h   = width;
        if(scan_mode == LCD_ROTATE_90)
            scan_mode = LCD_ROTATE_0;
        else
            scan_mode = LCD_ROTATE_180;
    }
#endif
	if(p_frame->posX==x&&p_frame->posY==y&&p_frame->w==w&&p_frame->h==h)
		return;
    stride  = (w + 0x1f) & (~0x1f);
    y_size  = stride * h;
    size    = y_size * 3 >>1;
    lcdc_caloff2(scan_mode, w, h, &_y_ofs, &_uv_ofs);
	printf("yofs:%d,uvofs:%d\n", _y_ofs,_uv_ofs);
    p_frame->uv_addr    = &p_frame->y_addr[y_size];
    p_frame->_y_addr    = &p_frame->y_addr[_y_ofs];
    p_frame->_uv_addr   = &p_frame->uv_addr[_uv_ofs];
    p_frame->buf_size   = size;
    p_frame->w          = w;
    p_frame->h          = h;
	p_frame->posX       =x;
	p_frame->posY       =y;
    p_frame->stride     = stride;
    p_frame->scan_mode  = scan_mode;
	//p_frame->configLcdEn=1;
}
void lcdframeVideoCfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
	if(p_frame->frame_type==FTYPE_VIDEO_ROTATE)
		frames_vr_cfg(p_frame,posX,posY,width,height);
	else if(p_frame->frame_type==FTYPE_VIDEO_DISPLAY)
		frames_vd_cfg(p_frame,posX,posY,width,height);
}

disp_frame_t * lcdframeMalloc(u8 type)
{
    u32 flag;
	flag = arch_local_irq_save();
    
    disp_frame_t ** pp_free = &dfm.types[type].freelist;
    disp_frame_t * p = NULL;

    if(*pp_free)
    {
        p = *pp_free;
        *pp_free = p->next;
        
        p->next = NULL;
        p->priv_func = NULL;
        p->data_size = 0;
        p->pip_sta = 0;
        p->buf_sta = 1;
    }
    
    arch_local_irq_restore(flag);
    
    if(!p)
    {
       // printf("lcd no frame\n");
    }
    
    return p;
}
void lcdframeFree(disp_frame_t * p_frame)
{
    u32 flag;
	if(p_frame==NULL)
		return;
	flag = arch_local_irq_save();
    disp_frame_t ** pp_free = &dfm.types[p_frame->frame_type].freelist;
    
    if(p_frame->buf_sta && (*pp_free != p_frame))
    {
        if(p_frame->next)
        {
            printf("[f_free] err frame\n");
        }
        
        p_frame->buf_sta = 0;
        p_frame->next = *pp_free;
        *pp_free = p_frame;
    }
    arch_local_irq_restore(flag);
}

disp_frame_t *lcdVideoFrameMalloc(void)
{
#if USE_HW_ROTATE_MODULE
	u8 scan_mode=lcdGetScanMode();
	if(scan_mode==LCD_ROTATE_90||scan_mode==LCD_ROTATE_270)
		return lcdframeMalloc(FTYPE_VIDEO_ROTATE);
	else
		return lcdframeMalloc(FTYPE_VIDEO_DISPLAY);
#else
	return lcdframeMalloc(FTYPE_VIDEO_DISPLAY);
#endif
}
disp_frame_t *lcdOsdFrameMalloc(void)
{
	return lcdframeMalloc(FTYPE_OSD_DISPLAY);
}



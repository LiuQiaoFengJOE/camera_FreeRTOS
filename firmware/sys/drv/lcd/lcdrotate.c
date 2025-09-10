#include "lcdrotate.h"
#include "osal/osal_irqflags.h"
#include "AX329x.h"

static struct rotate_opt_s 
{
    struct rotate_ch_s
    {
        u8 ch;
        disp_frame_t * src,* dst;
    } preset[ROTATE_CH_MAX],rotating;
    
    u8 prior;
} rotate_opt;
static void hal_rotateKick(u8 ch,disp_frame_t * src,disp_frame_t * dst)
{
    int timeout = 10;
    //实际运行过程中，这里不允许出现busy
    while(ax32xx_rotateCheckBusy() && --timeout);
    if(!timeout)
    {
        printf("[rotate reset]%X,%X,%X,%X\n",ROTATECON,rotate_opt.rotating.ch,rotate_opt.rotating.src,rotate_opt.rotating.dst);
        //ax32xx_wdtEnable(0);
        while(1);
    }  
	lcdframeFree(rotate_opt.rotating.src);
	lcdframeFree(rotate_opt.rotating.dst);
    rotate_opt.rotating.ch  = ch;
    rotate_opt.rotating.src = src;
    rotate_opt.rotating.dst = dst;
    
    ax32xx_rotateStart(ch,src->w,src->h,
                       dst->y_addr,dst->uv_addr,dst->stride,
                       src->y_addr,src->uv_addr,src->stride,
                       1);
}

static void hal_rotateFrameDoneIsr(void)
{
	void (*priv_func)(struct frame_s *);
	if(getLcdDebugEn())
	{
		printf("++++++%s,---%d\n\n",__FUNCTION__,__LINE__);
	}
	priv_func=rotate_opt.rotating.src->priv_func;
    lcdframeFree(rotate_opt.rotating.src);
    if(rotate_opt.rotating.ch == ROTATE_CH0)
    {
    	if(priv_func)
			priv_func(NULL);
        lcdSetVideoBuffer(rotate_opt.rotating.dst);
    }
    else
    {
        //hal_osdDealBuffer(rotate_opt.rotating.dst);
        printf("not surport osd ratate\n");
    }
    rotate_opt.rotating.ch  = -1;
    rotate_opt.rotating.src = rotate_opt.rotating.dst = NULL;
    //如果preset有值,处理preset
    int ch = rotate_opt.prior;
    if(!rotate_opt.preset[ch].src)
    {
        ch = rotate_opt.prior ^ 1;
    }
    disp_frame_t * src = rotate_opt.preset[ch].src;
    disp_frame_t * dst = rotate_opt.preset[ch].dst;
    if(src)
    {
        rotate_opt.preset[ch].src = 
        rotate_opt.preset[ch].dst = NULL;
        hal_rotateKick(ch,src,dst);
    }
}
void lcdRotateInit(void)
{
    memset(&rotate_opt,0,sizeof(rotate_opt));
    rotate_opt.prior = ROTATE_CH1;
    rotate_opt.preset[ROTATE_CH0].ch = ROTATE_CH0;
    rotate_opt.preset[ROTATE_CH1].ch = ROTATE_CH1;
    ax32xx_rotateISRRegiser(hal_rotateFrameDoneIsr);
}
void lcdRotateStop(void)
{
	u32 flag;
	flag = arch_local_irq_save();
	ax32xx_rotatestop();
	lcdframeFree(rotate_opt.rotating.src);
	rotate_opt.rotating.src=NULL;
	lcdframeFree(rotate_opt.rotating.dst);
	rotate_opt.rotating.dst=NULL;
	lcdframeFree(rotate_opt.preset[ROTATE_CH0].src);
	rotate_opt.preset[ROTATE_CH0].src=NULL;
    lcdframeFree(rotate_opt.preset[ROTATE_CH0].dst);
	rotate_opt.preset[ROTATE_CH0].dst=NULL;
	arch_local_irq_restore(flag);
}
void lcdRotateAdd(u8 ch,disp_frame_t * src,disp_frame_t * dst)
{
    u32 flag;
	flag = arch_local_irq_save();
	if(dst&&src)
		lcdframeVideoCfg(dst,src->posX,src->posY,src->w,src->h);
    if(rotate_opt.rotating.src)
    {
        //先释放preset
        if(rotate_opt.preset[ch].src)
        {
            lcdframeFree(rotate_opt.preset[ch].src);
            lcdframeFree(rotate_opt.preset[ch].dst);
        }
        
        //更新preset
        rotate_opt.preset[ch].src = src;
        rotate_opt.preset[ch].dst = dst;
    }
    else
    {
        hal_rotateKick(ch,src,dst);
    }
    arch_local_irq_restore(flag);
}

void lcdRotatePrint(void)
{
	printf("rotating src:0x%x,dest:0x%x\n",rotate_opt.rotating.src,rotate_opt.rotating.dst);
	printf("preset rotating src:0x%x,dest:0x%x\n",rotate_opt.preset[ROTATE_CH0].src,rotate_opt.preset[ROTATE_CH0].dst);
}


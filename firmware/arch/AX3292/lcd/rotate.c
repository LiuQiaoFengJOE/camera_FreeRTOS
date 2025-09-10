#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"
#include "sys/sys.h"
#include "AX329x_IRQ.h"
#include "board_config.h"
static void (*framedone_isr)(void);
void ax32xx_rotateISRRegiser(void (*isr)(void))
{
    framedone_isr = isr;
}
u32 ax32xx_rotateCheckBusy(void)
{
    return ROTATECON >> 31;
}
void ax32xx_rotateReset(void)
{
    RSTCON &=~(1<<3);
    asm("l.nop\nl.nop\nl.nop\nl.nop\n");
    RSTCON |= (1<<3);
}
void ax32xx_rotateIRQHandler(void)
{
    if(ROTATESTA & 1)
    {
        ROTATESTA = 1;
        if(framedone_isr)
            framedone_isr();
    }
}
void ax32xx_rotateStart(
    u8 src_mode,
    u32 src_width,u32 src_height,
    u8 *dst_y,u8 *dst_uv,u32 dst_stride,
    u8 *src_y,u8 *src_uv,u32 src_stride,
    u8 int_en)
{
    ax32xx_sysClkSet(SYS_CLK_ROTATE,1);
    
    ROTATECON   = (0<<31)|//kick start
                  (int_en<<2)| //int en
                  (src_mode<<1)| //mode : 0:video,1:osd
                  (1<<0); //enable
                  
    ROTATESTA   = (1<<0); //rotate pending/clear

    ROTIMGSIZE  = (((src_height&~1)-1)<<16)|//src height
                  (((src_width &~1)-1)<<0); //src width
                  
    ROTSRCYADR  = (((u32)src_y &~0x3F)<<0);//src y addr(aligned 32)
    ROTSRCUVADR = (((u32)src_uv&~0x3F)<<0);//src uv addr(aligned 32)

    ROTTGYADR   = (((u32)dst_y &~0x1F)<<0);//dst y addr(aligned 32)
    ROTTGUVADR  = (((u32)dst_uv&~0x1F)<<0);//dst uv addr(aligned 32)

    ROTADDRSTRI = ((dst_stride/4)<<16)|//dst stride(unit:word,aligned 4byte)
                  ((src_stride/4)<<0); //src stride(unit:word,aligned 4byte)
                  
    if(int_en)
		irq_register(IRQ_ROTATE, 0, ax32xx_rotateIRQHandler, NULL);
	else
    	irq_unregister(IRQ_ROTATE);
    ROTATECON  |= (1<<31);//kick
}
void ax32xx_rotatestop(void)
{
	ax32xx_rotateReset();
	ROTATECON=0;
}
/*******************************************************************************
* Function Name  : ax32xx_rotateWaitFrameDone
* Description    : rotate wating frame done
* Input          : none
* Output         : none
* Return         : 0 : success,-1 : timeout
*******************************************************************************/
int ax32xx_rotateWaitFrameDone(void)
{
    int timeout = APB_CLK_CONFIG / 1000 / 8 * 50;
    
    while(!(ROTATESTA & (1<<0)) && timeout--);
    ROTATESTA   = (1<<0);
    
    if(timeout)
        return 0;
    else
    {
        debg("rotate time out\n");
        return -1;
    }
}
u32 ax32xx_rotateGetSrcYAddr(void)
{
	return (ROTSRCYADR&0x1ffffff);
}



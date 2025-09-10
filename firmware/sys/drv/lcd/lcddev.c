#include "spr_defs.h"
#include "basic_defs.h"
#include "drv/lcd/lcdstruct.h"
#include "osal/osal_task.h"
#include "task/app.h"
#include "drv/lcd/lcdframe.h"

extern UINT32 APB_CLK;
extern void ax32xx_lcdPanelMode(u8 panel);
extern void ax32xx_lcdSpiBaudRateSet(u32 baudrate);
extern void ax32xx_lcdSpiSetClkIdleState(bool idle_sta);
extern void ax32xx_lcdSpiSampleEdge(bool edge);
extern void ax32xx_lcdSpiShiftDir(u32 shift_dir);
extern void ax32xx_lcdSpiSendCmdBits(u8 bits);
extern void ax32xx_lcdSpiSendDatBits(u8 bits);

ALIGNED(32)
static u8 lcdGamma[3][512];
ALIGNED(32)
static u8 lcdContra[256];

/*******************************************************************************
 * Function Name  : lcd_isp_res_load
 * Description    : lcd isp_resource load
 * Input          : lcddev_t *p_lcddev : lcd op node
 * Output         : none
 * Return         : none
 *******************************************************************************/
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clip(x, a, b) min(max((x), (a)), (b))

void lcd_isp_res_load(lcddev_t *p_devLcd, u32 res_isp_lcd/*RES_LCD_MSG*/)
{
  u32 i = 0, j = 0, len, tab_nums;
  u32 lcd_msg_addr;
  // uint32_t *pal = (uint32_t*) osal_malloc (12*128*4+13*256);//lcdc_get_palette();
  //_Lcd_Header_ * lcd_header_get(void);
  //_Lcd_Header_ * p_devLcdHeader = lcd_header_get();
  lcd_msg_addr = res_open(res_isp_lcd); // 获取LCDMSG的信息，不需要像行车那样获取地址

  if (p_devLcd)
  { // load gamma table
    len = 256;
    tab_nums = 13;
    if (p_devLcd->contra_index < tab_nums)
    {
      // hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_contra_tab_adr+p_devLcd->contra_index*len),(u32)lcdContra,len);
      res_read(lcd_msg_addr + 12 * 128 * 4 + p_devLcd->contra_index * len, (u32)lcdContra, len);
      printf("contra_index:%d\n", p_devLcd->contra_index);
      //Uart_PrintfBuf("lcdContra:\n", lcdContra, len);
    }
    len = 512;
    tab_nums = 12;
    if (p_devLcd->gamma_red < tab_nums)
    {
      // hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_red*len),(u32)lcdGamma[0],len);
      res_read(lcd_msg_addr + p_devLcd->gamma_red * len, (u32)lcdGamma[0], len);
      printf("gamma_red:%d\n", p_devLcd->gamma_red);
      //Uart_PrintfBuf("lcdGamma:\n", lcdGamma[0], len);
    }
    if (p_devLcd->gamma_green < tab_nums)
    {
      // hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_green*len),(u32)lcdGamma[1],len);
      res_read(lcd_msg_addr + p_devLcd->gamma_green * len, (u32)lcdGamma[1], len);
      printf("gamma_green:%d\n", p_devLcd->gamma_green);
      //Uart_PrintfBuf("lcdGamma:\n", lcdGamma[1], len);
    }
    if (p_devLcd->gamma_blue < tab_nums)
    {
      // hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_blue*len),(u32)lcdGamma[2],len);
      res_read(lcd_msg_addr + p_devLcd->gamma_blue * len, (u32)lcdGamma[2], len);
      printf("gamma_blue:%d\n", p_devLcd->gamma_blue);
      //Uart_PrintfBuf("lcdGamma:\n", lcdGamma[2], len);
    }
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 512; j++)
      {
        s32 temp;
        temp = lcdGamma[i][j];
        temp = temp + (-p_devLcd->brightness) * (temp + 1) / 256;
        temp = clip(temp, 0, 255);
        lcdGamma[i][j] = lcdContra[temp];
      }
    }
  }
}

static void hal_lcd_isp_init(lcddev_t *p_lcddev)
{
  if (p_lcddev == NULL)
    return;
  ax32xx_deSetCSC(p_lcddev->brightness, p_lcddev->saturation, p_lcddev->contrast);
  ax32xx_deSetGAMA((u32)lcdGamma[0], (u32)lcdGamma[1], (u32)lcdGamma[2]);
}

/*******************************************************************************
 * Function Name  : lcd_isp_config
 * Description    : lcd isp config
 * Input          : lcddev_t *p_lcddev : lcd op node
 * Output         : none
 * Return         : none
 *******************************************************************************/
void lcd_isp_config(lcddev_t *p_lcddev, u32 res_isp_lcd)
{
  lcd_isp_res_load(p_lcddev, res_isp_lcd);
  hal_lcd_isp_init(p_lcddev);
}

static void hal_lcdParaMCU(lcddev_t *p_lcddev, void *para_tab)
{
  u32(*tab)[2] = para_tab;

  if (tab == NULL)
    return;
  if (p_lcddev->bus_cmd == 0)
  {
    for (; tab[0][0] != LCD_TAB_END; tab++)
    {
      if (tab[0][0] == MCU_CMD)
      	{
        ax32xx_lcdMcuSendCmd(tab[0][1]);
		//printf("0x%x,\n",tab[0][1]);
      	}
      else if (tab[0][0] == MCU_DAT)
      	{
        ax32xx_lcdMcuSendData(tab[0][1]);
		//printf("0x%x,\n",tab[0][1]);
      	}
      else if (tab[0][0] == DELAY_MS)
        ax32xx_sysCpuMsDelay(tab[0][1]);
    }
  }
  else  if(p_lcddev->bus_cmd == 1)
  {
    for (; tab[0][0] != LCD_TAB_END; tab++)
    {
      if (tab[0][0] == MCU_CMD)
        ax32xx_lcdMcuSendCmd16(tab[0][1]);
      else if (tab[0][0] == MCU_DAT)
        ax32xx_lcdMcuSendData16(tab[0][1]);
      else if (tab[0][0] == DELAY_MS)
        ax32xx_sysCpuMsDelay(tab[0][1]);
    }
  }
  else
    {
		ax32xx_lcdSPIMode(p_lcddev->spi_cpol,p_lcddev->spi_cpha,p_lcddev->spi_order,p_lcddev->spi_bits);
		ax32xx_spiLcdnit();
		printf("ax32xx_spiLcdnit ------------>\n");
		for(;tab[0][0] != LCD_TAB_END;tab++)
		{
			if(tab[0][0] == SPI_DAT){
				if(0== (tab[0][1]>>8)){ // cmd 
					ax32xx_lcdMcuSendCmd(tab[0][1]&0xff);
                  //deg_Printf("cmd 0x%x\n",tab[0][1]&0xff);
				}else{	// data	
				    ax32xx_lcdMcuSendData(tab[0][1]&0xff);
					//deg_Printf("dat 0x%x\n",tab[0][1]&0xff);
				}
			}else if(tab[0][0] == DELAY_MS)
				ax32xx_sysCpuMsDelay(tab[0][1]);
		}        
    }
}

////////////////////////////////////////////////////////////////////////////////
void lcdpanel_rgb_init(lcddev_t *p_lcddev)
{
  uint32_t(*tab)[2] = p_lcddev->init_table;

  if (NULL == tab)
    return;
  ax32xx_lcdSPIMode(p_lcddev->spi_cpol,p_lcddev->spi_cpha,p_lcddev->spi_order,p_lcddev->spi_bits);
  ax32xx_lcdSPIInit();
  for (; tab[0][0] != LCD_TAB_END; ++tab)
  {
    if (tab[0][0] == SPI_DAT)
      ax32xx_lcdSPISendData(tab[0][1]);
    else if (tab[0][0] == DELAY_MS)
      osal_task_sleep(tab[0][1]);
  }
}

void lcdpanel_mcu_init(lcddev_t *p_lcddev)
{
  uint32_t(*tab)[2] = p_lcddev->init_table;

  if (NULL == tab)
    return;

  if (p_lcddev->bus_cmd == LCD_CMDBUS_WIDTH8)
  {
    for (; tab[0][0] != LCD_TAB_END; ++tab)
    {
      // printf ("%08X %08X\n", tab[0][0], tab[0][1]);
      if (tab[0][0] == MCU_CMD)
        ax32xx_lcdMcuSendCmd(tab[0][1]);
      else if (tab[0][0] == MCU_DAT)
        ax32xx_lcdMcuSendData(tab[0][1]);
      else if (tab[0][0] == DELAY_MS)
        osal_task_sleep(tab[0][1]);
    }
  }
  else
  {
    for (; tab[0][0] != LCD_TAB_END; ++tab)
    {
      if (tab[0][0] == MCU_CMD)
        ax32xx_lcdMcuSendCmd16(tab[0][1]);
      else if (tab[0][0] == MCU_DAT)
        ax32xx_lcdMcuSendData16(tab[0][1]);
      else if (tab[0][0] == DELAY_MS)
        osal_task_sleep(tab[0][1]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void hal_lcdTeMode1ISR(unsigned int irqno, void *d, void *regs)
{
  if (ax32xx_deGetUpdateStatus())
  //if(!(DE_STATUS & BIT(9)))
  {
    ax32xx_lcdKick();
	static u32 lastTime;
	if(os_time_get()-lastTime>20030)
	{
		lastTime=os_time_get();
		printf("TE\n");
	}
  }
}

void timer_hal_lcdTeMode(void *d)
{
  while (1)
  {
    if (!(lcdc_de_status() & BIT(9)))
    {
      ax32xx_lcdKick();
    }
    osal_task_sleep(50);
  }
}

void lcdc_caloff(lcddev_t *p_lcddev, uint32_t *y_off, uint32_t *uv_off)
{
  uint32_t pic_w = p_lcddev->video_w;
  uint32_t pic_h = p_lcddev->video_h;
  uint32_t stride = (p_lcddev->video_w + 31) & ~31;
  switch (p_lcddev->scan_mode)
  {
  default:
  case 0: // Normal left-top
    *y_off = *uv_off = 0;
    break;
  case 5: // rotate 270
  case 1: // Vertical mirror left_down
    *y_off = (pic_h - 1) * stride;
    *uv_off = (pic_h / 2 - 1) * stride;
    break;
  case 4: // rotate 90
  case 2: // Horizontal mirror right-top
    *y_off = *uv_off = pic_w - 1;
    break;
  case 3: // rotate 180 right-down
    *y_off = (pic_h - 1) * stride + pic_w - 1;
    *uv_off = (pic_h / 2 - 1) * stride + pic_w - 1;
    break;
  }
}

void lcdc_caloff2(uint8_t scan_mode, int16_t w, int16_t h, uint32_t *y_off, uint32_t *uv_off)
{
  uint32_t pic_w = w;
  uint32_t pic_h = h;
  uint32_t stride = (w + 31) & ~31;
  switch (scan_mode)
  {
  default:
  case 0: // Normal left-top
    *y_off = *uv_off = 0;
    break;
  case 5: // rotate 270
  case 1: // Vertical mirror left_down
    *y_off = (pic_h - 1) * stride;
    *uv_off = (pic_h / 2 - 1) * stride;
    break;
  case 4: // rotate 90
  case 2: // Horizontal mirror right-top
    *y_off = *uv_off = pic_w - 1;
    break;
  case 3: // rotate 180 right-down
    *y_off = (pic_h - 1) * stride + pic_w - 1;
    *uv_off = (pic_h / 2 - 1) * stride + pic_w - 1;
    break;
  }
}

void lcdc_reconfig(lcddev_t *p_lcddev)
{
  uint32_t stride = ((p_lcddev->video_w + 31) & ~31);
  ax32xx_Dereinit(p_lcddev->screen_w, p_lcddev->screen_h);
  // ax32xx_videoSetDither(p_lcddev->red_width,p_lcddev->green_width,p_lcddev->blue_width);
  ax32xx_videoSetStride(stride, stride);
  ax32xx_osdSetUpscaler(p_lcddev->screen_w, p_lcddev->screen_h, p_lcddev->screen_w, p_lcddev->screen_h, NULL);
  printf("lcd_bus_type- = %d	 h*w:%d*%d\n", p_lcddev->lcd_bus_type, p_lcddev->video_h, p_lcddev->video_w);
  if (p_lcddev->lcd_bus_type == LCD_BUS_I80)
  {
    ax32xx_lcdResolutionSet(p_lcddev->screen_w, p_lcddev->screen_h);
    ax32xx_lcdWindowSizeSet(p_lcddev->screen_w - 1, p_lcddev->screen_h - 1);
    ax32xx_lcdClkNumberSet(p_lcddev->screen_w * p_lcddev->screen_h * p_lcddev->clk_per_pixel - 1);

    hal_lcdParaMCU(p_lcddev, p_lcddev->init_table);

    if (p_lcddev->te_mode == 0) // just interrupt,no auto kick TX
    {
      ax32xx_lcdTeMode(0, 1);
      // 	  ax32xx_lcdIRQEnable(LCD_IRQ_TE,1);
    }
    else if (p_lcddev->te_mode == 1) // auto kick TX
    {
      //		  ax32xx_lcdISRRegister(LCD_IRQ_TE,hal_lcdTeMode1ISR);
      lcdcirq_register_te(hal_lcdTeMode1ISR, NULL);
      ax32xx_lcdTeMode(0, 1);
    }
    else if (p_lcddev->te_mode == 0xff)
    {
      // TODO:
      // 使用定时器中断，调用hal_lcdTeMode1ISR，每秒50~60次即可
      ax32xx_lcdKick();
    }
  }
  else
  {
  }
  // reconfig_osd();
}

void lcdc_init(lcddev_t *p_lcddev,disp_frame_t * display, u32 res_isp_lcd)
{
  ax32xx_deWait();
  ax32xx_deInit(p_lcddev->screen_w, p_lcddev->screen_h);
  ax32xx_deSetBackground(0, 0, 0); //(0x20,0x20,0x20);//(0x55,0xAA,0x55);//
  ax32xx_videoSetDither(p_lcddev->red_width, p_lcddev->green_width, p_lcddev->blue_width);
  //ax32xx_videoSetPosition(0, 0);
  ax32xx_videoSetPosition(display->posX, display->posY);
  ax32xx_videoSetAddr(display->_y_addr,display->_uv_addr, 0);
  ax32xx_videoSetStride(display->stride, display->stride);
  ax32xx_videoSetSize(display->w, display->h);
  ax32xx_videoSetScanMode(display->scan_mode);
  ax32xx_videoSetScale(0x00, 0x00, 0x40, 0x00);
  ax32xx_osdSetUpscaler(display->w, display->h,display->w, display->h, NULL);
  ax32xx_videoEnable(1);
  lcd_isp_config(p_lcddev, res_isp_lcd);
  ax32xx_deSet_DE_CCM(p_lcddev->de_ccm);
  ax32xx_deSet_DE_SAJ(p_lcddev->de_saj);
  ax32xx_deUpdate();

  printf("lcd_bus_type = %d   h*w:%d*%d\n", p_lcddev->lcd_bus_type, p_lcddev->video_h, p_lcddev->video_w);
  if (p_lcddev->lcd_bus_type == LCD_BUS_I80)
  {

    printf("[LCD mcu init]\n");
    ax32xx_lcdInit();
    ax32xx_lcdMCUIOConfig(p_lcddev->bus_width, p_lcddev->te_mode != 0xff);
    // ax32xx_lcdIRQEnable(LCD_IRQ_DEND,1);
    if(p_lcddev->bus_cmd==2)
    {
		ax32xx_lcdSpiBaudRateSet(2);// div=0
		ax32xx_lcdPanelMode(2);// spi mode
		ax32xx_lcdSpiSetClkIdleState(0);//clk idle state set low
		ax32xx_lcdSpiSampleEdge(0);// sample at rising edge
		//wrong expression in datasheet,it should be pre(0)/next(1) sample edge
		ax32xx_lcdSpiShiftDir(0);// send data from high bit
		ax32xx_lcdSpiSendCmdBits(7);
		ax32xx_lcdSpiSendDatBits(7);           
    }else
        ax32xx_lcdPanelMode(p_lcddev->lcd_bus_type);
	
    //ax32xx_lcdPanelMode(1);
    ax32xx_lcdPreLineSet(7);
    ax32xx_lcdSignalSet(0, 0, 1, 0, 0, 1, 0, 0);
    ax32xx_lcdBusWidth(p_lcddev->bus_width); // must after ax32xx_lcdSignalSet
    ax32xx_lcdClkSet(p_lcddev->pclk_div);
    ax32xx_lcdSyncSet(0, 0, 0, 0);
    ax32xx_lcdDESignalSet(0, 0, 0, 0);
    ax32xx_lcdPositionSet(0, 0);
    ax32xx_lcdResolutionSet(p_lcddev->screen_w, p_lcddev->screen_h);
    ax32xx_lcdWindowSizeSet(p_lcddev->screen_w - 1, p_lcddev->screen_h - 1);
    ax32xx_lcdDataModeSet(p_lcddev->lcd_data_mode, p_lcddev->lcd_data_mode1, p_lcddev->even_order, p_lcddev->odd_order);
    ax32xx_lcdClkNumberSet(p_lcddev->screen_w * p_lcddev->screen_h * p_lcddev->clk_per_pixel - 1);
    ax32xx_lcdEndLineSet(0, 0);
    ax32xx_lcdEnable(1);
    printf("%s hal_lcdParaMCU\n", __FUNCTION__);
    hal_lcdParaMCU(p_lcddev, p_lcddev->init_table);
	
	if(p_lcddev->bus_cmd==2){// spi serise interface,reset spi port output 16bits every trans(default RGB565).
			ax32xx_lcdSpiSendCmdBits(p_lcddev->bus_width-1);
			ax32xx_lcdSpiSendDatBits(p_lcddev->bus_width-1);
			ax32xx_lcdBusWidth(p_lcddev->bus_width);  // must after ax32xx_lcdSignalSet
			ax32xx_lcdClkNumberSet(p_lcddev->screen_w*p_lcddev->screen_h-1);
		}
    if (p_lcddev->te_mode == 0) // just interrupt,no auto kick TX
    {
      ax32xx_lcdTeMode(0, 1);
      //       ax32xx_lcdIRQEnable(LCD_IRQ_TE,1);
    }
    else if (p_lcddev->te_mode == 1) // auto kick TX
    {
      //        ax32xx_lcdISRRegister(LCD_IRQ_TE,hal_lcdTeMode1ISR);
      lcdcirq_register_te(hal_lcdTeMode1ISR, NULL);
      ax32xx_lcdTeMode(0, 1);
    }
    else if (p_lcddev->te_mode == 0xff)
    {
      // TODO:
      // 使用定时器中断，调用hal_lcdTeMode1ISR，每秒50~60次即可
      ax32xx_lcdKick();
      // osal_thread_create( timer_hal_lcdTeMode, NULL, 21, 0, 512, "TE" );
    }
  }
  else
  { // RGB
    

    lcdpanel_rgb_init(p_lcddev);
    ax32xx_lcdInit();
    ax32xx_lcdRGBIOConfig(p_lcddev->bus_width, p_lcddev->de_inv != 0xff);
    //    ax32xx_lcdIRQEnable(LCD_IRQ_FEND,1);
    ax32xx_lcdPanelMode(0);
    ax32xx_lcdLineSet(7);
    ax32xx_lcdSignalSet(p_lcddev->vlw, p_lcddev->pclk_inv & 1, p_lcddev->de_inv & 1, p_lcddev->hs_inv & 1, p_lcddev->vs_inv, p_lcddev->de_inv != 0xff, p_lcddev->hs_inv != 0xff, p_lcddev->vs_inv != 0xff);
    ax32xx_lcdBusWidth(p_lcddev->bus_width); // must after ax32xx_lcdSignalSet
    if (p_lcddev->init_table)
      ax32xx_lcdBusEnable(0x01, 0); // disable d0
    ax32xx_lcdClkSet(p_lcddev->pclk_div);            // APB_CLK/p_lcddev->pclk_div);
    ax32xx_lcdSyncSet(0, p_lcddev->hlw, 0, p_lcddev->vlw);
    u32 temp1, temp2, temp3, temp4;
    temp1 = p_lcddev->hlw + p_lcddev->hbp * p_lcddev->clk_per_pixel;
    temp2 = p_lcddev->vlw + p_lcddev->vbp;
    temp3 = p_lcddev->screen_w * p_lcddev->clk_per_pixel;
    temp4 = p_lcddev->screen_h - 1;
    ax32xx_lcdDataSet(temp1, temp1 + temp3, temp2, temp2 + temp4);
    ax32xx_lcdPositionSet(temp1, temp2);
    ax32xx_lcdResolutionSet(p_lcddev->screen_w, p_lcddev->screen_h);
    ax32xx_lcdWindowSizeSet(temp1 + temp3 + p_lcddev->hfp * p_lcddev->clk_per_pixel, temp2 + temp4 + p_lcddev->vlw + p_lcddev->vfp);
    //ax32xx_lcdModeSet(p_lcddev->lcd_data_mode);
    ax32xx_lcdDataModeSet(p_lcddev->lcd_data_mode,p_lcddev->lcd_data_mode1,p_lcddev->even_order,p_lcddev->odd_order);
    ax32xx_lcdClkNumberSet(0);
    ax32xx_lcdEndLineSet(temp2 + p_lcddev->screen_h, temp2 + p_lcddev->screen_h + 2);
    ax32xx_lcdEnable(1);
  }
}

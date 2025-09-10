#include "basic_defs.h"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
#include "debug.h"

//=====sensor 720P  check frame center =======
#define HAL_CFG_MD_SX 320
#define HAL_CFG_MD_EX (320 + 640)
#define HAL_CFG_MD_SY 180
#define HAL_CFG_MD_EY (180 + 360)

#define HAL_CFG_MD_THRESHOLD 1 // THRESHOLD LEVEL   0: easy happen, 1: normal  happen, 2 :not easy happen

#if (0 == HAL_CFG_MD_THRESHOLD)
#define HAL_CFG_MD_PIXELNUM 5 // value more min ,more easy to happen
#define HAL_CFG_MD_BLOCKNUM 5 // value more min ,more easy to happen
#define HAL_CFG_MD_FRAMENUM 1 // value more min ,more easy to happen
#elif (1 == HAL_CFG_MD_THRESHOLD)
#define HAL_CFG_MD_PIXELNUM 20 // value more min ,more easy to happen
#define HAL_CFG_MD_BLOCKNUM 20 // value more min ,more easy to happen
#define HAL_CFG_MD_FRAMENUM 1  // value more min ,more easy to happen
#else
#define HAL_CFG_MD_PIXELNUM 40 // value more min ,more easy to happen
#define HAL_CFG_MD_BLOCKNUM 40 // value more min ,more easy to happen
#define HAL_CFG_MD_FRAMENUM 1  // value more min ,more easy to happen
#endif

// todo
void csi_setup()
{
  if (csi_enabled())
    return;

  _Sensor_Adpt_ *sensor = sensor_adapt_get();

  ax32xx_csiInit();
  ax32xx_csiModeSet(0xFFFFFFFF, 0);
  ax32xx_csiSyncSet(sensor->hsyn, sensor->vsyn);
  printf("csi setup :pixelw----->%x    pixelh----->%x\n", sensor->pixelw, sensor->pixelh);
  ax32xx_csiSizeSet(sensor->pixelw, sensor->pixelh); // todo

  ax32xx_mdXPos(HAL_CFG_MD_SX, HAL_CFG_MD_EX);
  ax32xx_mdYPos(HAL_CFG_MD_SY, HAL_CFG_MD_EY);
  ax32xx_mdInit(HAL_CFG_MD_PIXELNUM, HAL_CFG_MD_BLOCKNUM, HAL_CFG_MD_FRAMENUM);

  if (sensor->typ == CSI_TYPE_YUV422)
  {
    printf("****YUV422******\n");
    ax32xx_csiPrioritySet(sensor->colrarray);
    ax32xx_csiTypeSet(CSI_TYPE_YUV422);
  }
  else if (sensor->typ == CSI_TYPE_RAW8 || sensor->typ == CSI_TYPE_RAW10 || (sensor->typ == CSI_TYPE_RAW12))
  {
    ax32xx_csiPrioritySet(sensor->colrarray);
    ax32xx_csiTypeSet(sensor->typ);
    ax32xx_ispModeSet(ISP_MODE_CFAEN, 1);
    ax32xx_csiModeSet(CSI_MODE_SAMPLE, 1);
    ax32xx_pclk_fir_Set(sensor->pclk_fir_en);
    ax32xx_pclk_analog_Set(sensor->pclk_fir_class);
    ax32xx_pclk_inv_Set(sensor->pclk_inv_en);
    printf("sensor->csi_tun:%x\n", sensor->csi_tun);
    ax32xx_csi_clk_tun_Set(sensor->csi_tun);
    isp_printf();
    // if (sensor->rduline)
    //   ax32xx_csiModeSet(CSI_MODE_CFASIZE, 1);
  }
  else if (sensor->typ == CSI_TYPE_COLOR_BAR)
  {
    // ax32xx_csiPrioritySet(CSI_PRIORITY_CBY0CRY1);
    // ax32xx_csiTypeSet(CSI_TYPE_YUVR422);
    // ax32xx_csiModeSet(CSI_MODE_TEST_EN, 1);
    // ax32xx_csiModeSet(8<<20, 1);
    ax32xx_csiPrioritySet(0);
    ax32xx_csiModeSet(CSI_MODE_SAMPLE, 1);
    ax32xx_csiTypeSet(CSI_TYPE_RAW8);
    ax32xx_ispModeSet(ISP_MODE_CFAEN, 1);
    ax32xx_csiModeSet(CSI_MODE_TEST_EN, 1);
    ax32xx_csiTestModeSet(sensor->pixelw, sensor->pixelh, 2046, 1024);
  }
  // csi_enable ();
  ax32xx_csi_only_FrameSet();
  // csimdt_enable(1);
  // ax32xx_csiEnable(1);
}

#include "typedef.h"

#include "spr_defs.h"
#include "AX329x.h"
#include "sdc.h"
#include "sys/sys.h"
#include "board_config.h"

struct sd_controller
{
  pSFR sdreg;

  // Dat1的信息
  int pint;
  int port;
  int pin;
};

#define get_sdreg(c) ((struct sd_controller *)(c))->sdreg

extern UINT32 APB_CLK; // defined by board

#define SD0_PORT PORTA
#if WIFI_SUPPORT_MODE == DVRUNNING
#define SD0_CLK_PIN 0
#define SD0_CMD_PIN 1
#define SD0_DAT0_PIN 6
#define SD0_DAT1_PIN 5
#define SD0_DAT2_PIN 0
#define SD0_DAT3_PIN 1
#else if (WIFI_SUPPORT_MODE == FLIGHT_MODE) || (WIFI_SUPPORT_MODE == USENSOR_MODE)
#define SD0_CLK_PIN 8
#define SD0_CMD_PIN 7
#define SD0_DAT0_PIN 9
#define SD0_DAT1_PIN 5
#define SD0_DAT2_PIN 0
#define SD0_DAT3_PIN 1
#endif

#define SD1_PORT PORTD
#define SD1_CLK_PIN 2
#define SD1_CMD_PIN 0
#define SD1_DAT0_PIN 1
#define SD1_DAT1_PIN 3
#define SD1_DAT2_PIN 10
#define SD1_DAT3_PIN 11

extern UINT32 APB_CLK; // defined by board

struct sd_controller sdc0 =
    {
        .sdreg = (u32)&SD1CON0,
        .port = 5,  /* PORTF */
        .pin = 8,   /* PF8 */
        .pint = 19, /* pint19 */
};

struct sd_controller sdc1 =
    {
        .sdreg = (u32)&SDCON0,
        .port = 0, /*PORTA */
        .pin = 3,  /*PF10 */
        .pint = 1, /*pint1 */
};
struct sd_controller *sdcard_control = &sdc0;
int sdio_int_check(struct sd_controller *controller)
{
  return (PWKPND & BIT(controller->pint));
}

void sdio_int_clrpnd(struct sd_controller *controller)
{
  PWKPND = BIT(controller->pint);
  // mtspr( SPR_PICSR, mfspr(SPR_PICSR) & ~(1<<7) );
}
void port_int_printf(struct sd_controller *controller)
{
  printf("PWKPND:%x  PWKEN:%x  %x\n", PWKPND, PWKEN, BIT(controller->pint));
}

int port_int_ck(int pint)
{
  return (PWKPND & BIT(pint));
}
void port_int_clrpnd(int pint)
{
  PWKPND = BIT(pint);
}

void *sdc0_io_init(void)
{
  //printf("%s  \n", __func__);
  ax32xx_sysClkSet(SYS_CLK_SD1, 1);
  ax32xx_dmaChannelEnable(DMA_SD1, 1);

  LDOCON |= BIT(21);
  PMAPCON1 &= ~(7 << 17);

#if WIFI_SUPPORT_MODE == DVRUNNING
  PMAPCON1 |= (5 << 17); // map 4
  soft_delay(1000);
  PAFMAP |= BIT(SD0_DAT0_PIN);
  PEFMAP |= BIT(SD0_CMD_PIN) | BIT(SD0_CLK_PIN);
  PEPU |= BIT(SD0_CLK_PIN) | BIT(SD0_CMD_PIN);
  PAPU |= BIT(SD0_DAT0_PIN);
  PAPD &= ~BIT(SD0_DAT0_PIN);
  PEDIR &= ~BIT(SD0_CLK_PIN); //|BIT(SD0_DAT1_PIN)|BIT(SD0_DAT2_PIN)|BIT(SD0_DAT3_PIN));
  PADIR |= BIT(SD0_DAT0_PIN);
#else if (WIFI_SUPPORT_MODE == FLIGHT_MODE) || (WIFI_SUPPORT_MODE == USENSOR_MODE)
  PMAPCON1 |= (2 << 17); // map 2
  soft_delay(1000);
  PDFMAP |= BIT(SD0_DAT0_PIN);
  PDFMAP |= BIT(SD0_CMD_PIN) | BIT(SD0_CLK_PIN);
  PDPU |= BIT(SD0_CLK_PIN) | BIT(SD0_CMD_PIN);
  PDPU |= BIT(SD0_DAT0_PIN);
  PDPD &= ~BIT(SD0_DAT0_PIN);
  PDDIR &= ~BIT(SD0_CLK_PIN); //|BIT(SD0_DAT1_PIN)|BIT(SD0_DAT2_PIN)|BIT(SD0_DAT3_PIN));
  PDDIR |= BIT(SD0_DAT0_PIN);
#endif

  soft_delay(1000);

  SD1CON0 = 0;
  soft_delay(1000);
  SD1CON0 |= /*BIT(BIT_8CKE) |*/ BIT(BIT_SDCKE) | BIT(6); /*BIT(5)*/
  SD1BAUD = APB_CLK / 400000 - 1; // APB_CLK/(250000) - 1;
  SD1CON0 |= BIT(BIT_SDE);
  SD1CON0 |= BIT(BIT_DIE);

  return &sdc0;
}

void *sdc1_io_init(void)
{
  // PCON0 &= ~BIT(16);          //CLK ENABLE
  // PMAPCON2 &= ~(3<<4);
  // PMAPCON1 &= ~(3<<14);
  PMAPCON1 &= ~(1 << 16);
  PMAPCON1 |= (1 << 16); // group 0
  // NOC_REG0 |= 1<<13;

  PAFMAP |= BIT(SD1_CLK_PIN) | BIT(SD1_CMD_PIN) | BIT(SD1_DAT0_PIN) | BIT(SD1_DAT1_PIN);
  PFFMAP |= BIT(SD1_DAT2_PIN) | BIT(SD1_DAT3_PIN);

  PAPU |= BIT(SD1_CLK_PIN) | BIT(SD1_CMD_PIN) | BIT(SD1_DAT0_PIN) | BIT(SD1_DAT1_PIN);
  PAPD &= ~BIT(SD1_DAT0_PIN);
  PFPU |= BIT(SD1_DAT2_PIN) | BIT(SD1_DAT3_PIN);
  // PDPD  |= BIT(SD1S_DAT0_PIN);
  PADIR &= ~BIT(SD1_CLK_PIN); //|BIT(SD1S_DAT1_PIN)|BIT(SD1S_DAT2_PIN)|BIT(SD1S_DAT3_PIN));
  PADIR |= BIT(SD1_DAT0_PIN) | BIT(SD1_DAT1_PIN);
  PFDIR |= BIT(SD1_DAT2_PIN) | BIT(SD1_DAT3_PIN);
  SDCON0 = 0;
  soft_delay(1000);

  SDCON0 = BIT(BIT_SDE) | BIT(BIT_8CKE);
  soft_delay(40000);

  SDBAUD = APB_CLK / 250000 - 1;
  SDCON0 |= BIT(BIT_SDCKE);
  SDCON0 |= BIT(BIT_SDCSEN);
  soft_delay(40000);
  ax32xx_sysClkSet(SYS_CLK_SD0, 1);
  ax32xx_dmaChannelEnable(DMA_SD0, 1);

  return &sdc1;
}

void sdc_ext_int_init(void *controller)
{
  int port = ((struct sd_controller *)controller)->port;
  int pin = ((struct sd_controller *)controller)->pin;
  int pint = ((struct sd_controller *)controller)->pint;
  pSFR PxPU = (u32)&PAPU;
  pSFR PxDIR = (u32)&PADIR;
  PMAPCON0 |= BIT(31);
  sdreg(PxPU, port) |= BIT(pin);
  sdreg(PxDIR, port) |= BIT(pin);
  PWKEDG |= BIT(pint); // falling edge
  PWKLEV |= BIT(pint);
  PWKPND = BIT(pint); // clr pnd
  PWKEN |= BIT(pint); // enable
}

void isr_port_edg(u32 pint)
{
  PWKEDG |= BIT(pint); // falling edge
  PWKLEV |= BIT(pint);
  PWKPND = BIT(pint); // clr pnd
  PWKEN |= BIT(pint); // enable
}

void sdc_ext_int_enable(void *host, void *controller, int enable)
{
  int pint = ((struct sd_controller *)controller)->pint;
  int dat1_high = 1;

  if (enable)
  {
    PWKPND = BIT(pint); // clr pnd
    PWKEN |= BIT(pint); // enable
  }
  else
  {
    PWKEN &= ~BIT(pint);
  }
}

int sdc_check_dat1_high(void *controller)
{
  uint32_t val;
  int port = ((struct sd_controller *)controller)->port;
  int pin = ((struct sd_controller *)controller)->pin;
  pSFR PORTx = (u32)&PORTA;

  val = sdreg(PORTx, port) & BIT(pin);

  if (!val)
    sdc_testio_low(7);

  return val;
}

void sdc_reset(void *controller)
{
  pSFR base = get_sdreg(controller);

  sdreg(base, SDxCON0) &= ~BIT(BIT_SDE);
  soft_delay(100);
  sdreg(base, SDxCON0) |= BIT(BIT_SDE);
}

int sdc_set_cont_clock(void *controller, int cont)
{
  pSFR base = get_sdreg(controller);
  if (cont)
    sdreg(base, SDxCON0) |= BIT(BIT_SDCKE);
  else
    sdreg(base, SDxCON0) &= ~BIT(BIT_SDCKE);

  return SD_OK;
}

extern u8 sdcard_buf[64];

int sdc_send_cmd(void *controller, uint8_t byCMDIndex, uint32_t dwArgument, uint32_t byCMDType, uint32_t *resp)
{

  pSFR base = get_sdreg(controller);

  int i;
  uint32_t res;
  // static int ccc=200;
  // if (ccc) {
  // printf ("CMD%d %X %X\r\n", byCMDIndex, dwArgument, byCMDType);
  //--ccc;
  // }
  //   sd_pd14(0);

  sdreg(base, SDxIDX) = byCMDIndex | 0x40;

  sdreg(base, SDxARG) = dwArgument;

  sdreg(base, SDxCON1) = BIT(BIT_CPCLR);

  if (byCMDType & MMC_CMD_8CYC)            // BIT 5
    sdreg(base, SDxCON0) |= BIT(BIT_8CKE); // BIT 4
  //  sdc_set_cont_clock (controller, byCMDType & MMC_CMD_CONTCLK);

  // SDxCADR should be init first
  sdreg(base, SDxCADR) = (u32)sdcard_buf;
  // kick
  // 此處做了簡單處理，將位定義與芯片一致
  // 如果不一致，在此需要做一個轉換表
  sdreg(base, SDxCON1) = byCMDType & MMC_RSP_MSK;

  i = 0xffffff;
  while (!(sdreg(base, SDxCON0) & BIT(BIT_CPND)))
  {
    if (--i == 0)
    {
      // 超時出錯
      printf("CMD%d pending time out!\r\n", byCMDIndex);
      sdreg(base, SDxCON1) = BIT(BIT_CPCLR);
      sdreg(base, SDxCON0) &= ~BIT(BIT_8CKE);
      return SD_CMD_RSP_TIMEOUT;
    }
  }
  // 命令結束
  sdreg(base, SDxCON1) = BIT(BIT_CPCLR);
  sdreg(base, SDxCON0) &= ~BIT(BIT_8CKE);

  res = sdreg(base, SDxCON0);

  // if ((byCMDType & MMC_RSP_CRC) && (res & BIT(BIT_CCRCE))) {
  //   printf ("****RESP crc err:%x\r\n",sdreg(base,SDxCON0));
  //   return SD_CMD_CRC_FAIL;
  // }
  if ((byCMDType & MMC_RSP_BUSY) && (res & BIT(BIT_NRPS)))
  {
    //printf("**Wait rsp timeout:%d\r\n", byCMDIndex);
    return SD_CMD_RSP_TIMEOUT;
  }

  if (resp)
  {
    *resp = sdreg(base, SDxARG);
  }

  return SD_OK;
}

void sdc_set_bus_width(void *controller, int width)
{
  pSFR base = get_sdreg(controller);

  if (width == 4)
    sdreg(base, SDxCON0) |= BIT(BIT_DW4);
  else
    sdreg(base, SDxCON0) &= ~BIT(BIT_DW4);
}

void sdc_set_baud(void *controller, uint32_t baud)
{
  pSFR base = get_sdreg(controller);
  sdreg(base, SDxBAUD) = APB_CLK / baud - 1;
}

int sdc_wait_data0_high(void *controller)
{
  pSFR base = get_sdreg(controller);
  int i = 0xFFFFFF;

  sdreg(base, SDxCON0) |= BIT(BIT_SDCKE);
  while (--i)
  {
    if (sdreg(base, SDxCON0) & BIT(BIT_BUSY))
    { // wait not busy
      sdreg(base, SDxCON0) &= ~BIT(BIT_SDCKE);
      return SD_OK;
    }
  }
  sdreg(base, SDxCON0) &= ~BIT(BIT_SDCKE);
  printf("WaitDAT0High fail !!!\r\n");
  return SD_DATA_TIMEOUT;
}

void sdc_kick_dma_read(void *controller, uint32_t addr, uint32_t len)
{
  pSFR base = get_sdreg(controller);

  dcache_flush_region(addr, len);

  sdreg(base, SDxCON0) &= ~BIT(BIT_SDCKE);
  // kick DMA 會自動出clock
  sdreg(base, SDxDADR) = addr;
  sdreg(base, SDxCON1) = BIT(BIT_DPCLR);
  sdreg(base, SDxDCNT) = len;
  sdreg(base, SDxCON1) = BIT(BIT_DKST);
}

void sdc_sddisr_enable(void *controller, uint8_t enable)
{
  pSFR base = get_sdreg(controller);

  if (enable)
  {
    sdreg(base, SDxCON0) |= BIT(BIT_DIE);
  }
  else
  {
    sdreg(base, SDxCON0) &= ~BIT(BIT_DIE);
  }
}

void sdc_set_early_clk(void *controller, uint8_t enable)
{
  pSFR base = get_sdreg(controller);

  if (enable)
  {
    sdreg(base, SDxCON0) |= BIT(BIT_ELYCLK);
  }
  else
  {
    sdreg(base, SDxCON0) &= ~BIT(BIT_ELYCLK);
  }
}

void sdc_set_edge(void *controller, uint8_t rise)
{
  pSFR base = get_sdreg(controller);

  if (rise)
  {
    sdreg(base, SDxCON0) |= BIT(BIT_ORISE);
  }
  else
  {
    sdreg(base, SDxCON0) &= ~BIT(BIT_ORISE);
  }
}

void sdc_kick_dma_write(void *controller, uint32_t addr, uint32_t len)
{
  pSFR base = get_sdreg(controller);

  dcache_writeback_region(addr, len);

  // kick DMA 會自動出clock
  sdreg(base, SDxDADR) = addr;
  sdreg(base, SDxCON1) = BIT(BIT_DPCLR);
  sdreg(base, SDxDCNT) = len;
  sdreg(base, SDxCON1) = BIT(BIT_DKST) | BIT(BIT_DKST + 1);
}

int sdc_wait_data_pending(void *controller)
{
  pSFR base = get_sdreg(controller);
  uint32_t i = 0xFFFFFF;

  while (!(sdreg(base, SDxCON0) & BIT(BIT_DPND)))
  {
    if (--i == 0)
    {
      sdreg(base, SDxCON1) |= BIT(BIT_DPCLR);
      return SD_DATA_TIMEOUT;
    }
  }
  return SD_OK;
}

int sdc_check_read_crc(void *controller)
{
  pSFR base = get_sdreg(controller);

  if (sdreg(base, SDxCON0) & BIT(BIT_DCRCE))
  {
    printf("err crc\n");
    return SD_DATA_CRC_FAIL;
  }
  return SD_OK;
}

int sdc_check_write_crc(void *controller)
{
  pSFR base = get_sdreg(controller);

  if ((sdreg(base, SDxCON0) & 0x600000) != 0x400000)
    return SD_DATA_CRC_FAIL;
  return SD_OK;
}

u32 sdc_con_dat(void *controller)
{
  pSFR base = get_sdreg(controller);
  return sdreg(base, SDxCON0);
}

char sdc_close(void *controller)
{
  pSFR base = get_sdreg(controller);
  sdreg(base, SDxCON0) &= ~BIT(BIT_SDE);
  sdreg(base, SDxCON1) = BIT(BIT_DPCLR) | BIT(BIT_CPCLR);
  sdreg(base, SDxCON0) &= ~(BIT(BIT_DIE) | BIT(BIT_CIE));
  return TRUE;
}

unsigned int check_data_pend(void *controller)
{
  u32 ret;
  pSFR base = get_sdreg(controller);
  ret = sdreg(base, SDxCON0) & BIT(BIT_DPND);
  return ret;
}

void clean_data_pend(void *controller)
{
  pSFR base = get_sdreg(controller);
  sdreg(base, SDxCON1) |= BIT(BIT_DPCLR);
}

u32 get_sdc_rsp(void *controller)
{
  pSFR base = get_sdreg(controller);
  return sdreg(base, SDxARG);
}
/**********************************************/
#if 0


static char sdcard_Close(void)
{
     return sdc_close(sdcard_control);
}

static unsigned int is_sdcard_isr_pend()
{
    return check_data_pend(sdcard_control);
}

static void sdcard_clean_panding()
{
   clean_data_pend(sdcard_control);
}

static void sdcard_sclk_cfg(u32 baud)
{
    sdc_set_baud(sdcard_control,baud);
}
static void sdcard_cfg(void)
{
    sdc0_io_init();
}

 uint32_t sdcard_GetRspVar(void)
{
	return get_sdc_rsp(sdcard_control);
}

static char sdccard_send_cmd(u8 byCMDIndex, uint32_t dwArgument,uint32_t byCMDType)
{
    if(sdc_send_cmd (sdcard_control, byCMDIndex,dwArgument,byCMDType, NULL) == 0)
        return true;
    else return false;
}


static char sdcard_WaitDAT0High(void)
{
    return sdc_wait_data0_high(sdcard_control);
}

static char sdcard_WaitDataPending(void)
{
   return sdc_wait_data_pending(sdcard_control);
}
static void sdcard_kick_dma_write (u32 addr, u32 len)
{
    sdc_kick_dma_write(sdcard_control,addr,len);
}
static void sdcard_kick_dma_read (u32 addr, u32 len)
{
    sdc_kick_dma_read(sdcard_control,addr,len);
}

static void sdcard_set_bus(u8 bus)
{
    u32 buss;
    if(bus)buss = 4;
    sdc_set_bus_width(sdcard_control,buss);
}

static unsigned int is_sdard_read_RCR_err()
{
    return sdc_check_read_crc(sdcard_control);
}
static unsigned char is_sdard_write_RCR_err()
{
    return sdc_check_write_crc(sdcard_control);
}




sdiodisk_dev sdc0_dev={
    .sclk_cfg = sdcard_sclk_cfg,
    .sd_config = sdcard_cfg,
    .get_rspvar = sdcard_GetRspVar,
    .send_cmd = sdccard_send_cmd,
    .Close = sdcard_Close,
    .WaitDAT0High = sdcard_WaitDAT0High,
    .WaitDataPending = sdcard_WaitDataPending,
    .kick_dma_read = sdcard_kick_dma_read,
    .kick_dma_write = sdcard_kick_dma_write,
    .set_bus = sdcard_set_bus,
    .is_isr_pend = is_sdcard_isr_pend,
    .is_read_RCR_err = is_sdard_read_RCR_err,
    .is_write_RCR_err =is_sdard_write_RCR_err,
    .clean_panding=sdcard_clean_panding,
    .buff = sdcard_buf,
};
#endif
/**********************************************/
////////////////////
void sdc_testio_init(void)
{
  /*
PORTD |= BIT(4)|BIT(7);
PDDIR &= ~(BIT(4)|BIT(7));
 */
}

void sdc_testio_high(int pin)
{
  /*
PORTD |= BIT(pin);
//  printf ("PICMR = %X\r\n", mfspr(SPR_PICMR));
* */
}

void sdc_testio_low(int pin)
{
  /*
PORTD &= ~BIT(pin);
 * */
}

void sdc_testio_toggle(int pin)
{
  /*
PORTD ^= BIT(pin);
 * */
}

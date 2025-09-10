#include "typedef.h"
#include "wifi_io.h"
#include "error.h"
#include "debug.h"

extern struct mmc_host bw_mmc_host;
struct sdio_func *wifi_sdio_func;

struct sdio_func rtw_func = {
    .num = 1,
    .enable_timeout = 100,
};

struct mmc_card rtw_card = {
    .host = &bw_mmc_host,
    .func = &rtw_func,
};

struct mmc_host bw_mmc_host = {
    .card = &rtw_card,
};

u32 return_addr()
{
  return rtw_card.host;
}

int sdio_bus_probe(void)
{
  int ret;
  // u32 temp =0;
  struct sdio_func *func = &rtw_func;

  func->card = &rtw_card;
  // printf("%x \n",func->card->host->lock);
  // temp = *((int *)0x215c444);
  //   printf("addr data:%x \n",temp);

  printf("sizeof struct mmc_card  :%d\r\n", sizeof(struct mmc_card));
  sema_init(&func->card->host->lock, 1);

  sdio_claim_host(func);

  ret = sdio_enable_func(func);
  if (ret)
  {
    sdio_disable_func(func);
    sdio_release_host(func);
    return -EIO;
  }

  sdio_release_host(func);

  sdio_set_block_size(func, 512);

  wifi_sdio_func = func;

  return 0;
}

int sdio_bus_remove(void)
{
  return 0;
}

SDIO_BUS_OPS rtw_sdio_bus_ops = {
    sdio_bus_probe,
    sdio_bus_remove,
    sdio_enable_func,
    sdio_disable_func,
    NULL,
    NULL,
    sdio_claim_irq,
    sdio_release_irq,
    sdio_claim_host,
    sdio_release_host,
    sdio_readb,
    sdio_readw,
    sdio_readl,
    sdio_writeb,
    sdio_writew,
    sdio_writel,
    sdio_memcpy_fromio,
    sdio_memcpy_toio};

void init_busops()
{
  rtw_sdio_bus_ops.bus_probe();
}

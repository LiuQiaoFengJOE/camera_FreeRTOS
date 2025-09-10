#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include <stdint.h>

enum SPI_BUS_MODE
{
  SPI_BUS_2S, //< SPI雙線單向 --> <--
  SPI_BUS_1,  //< SPI單線雙向 <->
  SPI_BUS_2P, //< SPI雙線並行 <-> <->
  SPI_BUS_4P  //< SPI4線並行  <-> <-> <-> <->
};

struct spi_device
{
  void (*init)(uint32_t u32SPIBaud, int bus_mode);
  void (*send_byte)(uint8_t u8Data);
  uint8_t (*recv_byte)(void);
  void (*send_chunk)(uint8_t *buf, uint32_t len);
  void (*recv_chunk)(uint8_t *buf, uint32_t len);
  void (*chip_sel)(int level);
  int status;
};

#endif /* __HAL_SPI_H__ */

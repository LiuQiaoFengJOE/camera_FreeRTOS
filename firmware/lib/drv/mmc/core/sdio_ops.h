#ifndef __SDIO_OSP_H__
#define __SDIO_OSP_H__
#define mmc_send_io_op_cond sdio_card_check_ocr
// int mmc_send_io_op_cond(u32 ocr, u32 *rocr);
int mmc_io_rw_direct(struct mmc_card *card, int write, unsigned fn, unsigned addr, u8 in, u8 *out);
int sdio_reset(struct mmc_host *host);
int mmc_io_rw_extended(struct mmc_card *card, int write, unsigned fn,
					   unsigned addr, int incr_addr, u8 *buf, unsigned blocks, u32 blksz);

#endif

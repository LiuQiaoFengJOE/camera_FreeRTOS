#ifndef __SDIO_IO_H__
#define __SDIO_IO_H__

int sdio_enable_func(struct sdio_func *func);
int sdio_disable_func(struct sdio_func *func);
int sdio_claim_irq(struct sdio_func *func, void (*handler)(struct sdio_func *));
int sdio_release_irq(struct sdio_func *func);
void sdio_claim_host(struct sdio_func *func);
void sdio_release_host(struct sdio_func *func);
u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
void sdio_writeb(struct sdio_func *func, u8 b, unsigned int addr, int *err_ret);
int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,
					 void *src, int count);
int sdio_memcpy_fromio(struct sdio_func *func, void *dst,
					   unsigned int addr, int count);
u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret);
void sdio_writew(struct sdio_func *func, u16 b, unsigned int addr, int *err_ret);
u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret);
void sdio_writel(struct sdio_func *func, u32 b, unsigned int addr, int *err_ret);
int sdio_set_block_size(struct sdio_func *func, unsigned blksz);

#endif

#define DEBUG

#include "type.h"
#include "sdio.h"
#include "system.h"

int sdio_reset(struct mmc_host *host)
{
	int ret;
	u8 abort;

	/* SDIO Simplified Specification V2.0, 4.4 Reset for SDIO */

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_ABORT, 0, &abort);
	if (ret)
		abort = 0x08;
	else
		abort |= 0x08;
	printf("p4\n");

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_ABORT, abort, NULL);
	printf("p5\n");

	return ret;
}

#include "typedef.h"

#include "spr_defs.h"
#include "AX329x.h"

/**
 * line大小
 */
#define __LINE_SIZE__ 16
/**
 * cache大小
 */
#define __CAHCHE_SIZE__ 8192 // 4096

#define __DCTAG_ADR__(x) *((u32 *)0x88400 + (x))

////////////////////////////////////////////////////////////////////////////////
void icache_init(void)
{
	int cnt = 0;

	__sprclr(SPR_ICCR, 0);
	mtspr(SPR_ICAIR, cnt << 4);
	__sprset(SPR_ICCR, 0);
}

void icache_disable(void)
{
	__sprclr(SPR_SR, 4);
}

void icache_enable(void)
{
	__sprset(SPR_SR, 4);
}

void icache_invalid_all(void)
{
	int cnt = 0;

	// invalidate 256 lines
	for (cnt = 0; cnt < __CAHCHE_SIZE__ / __LINE_SIZE__; cnt++)
	{
		mtspr(SPR_ICBIR, cnt << 4);
	}
}

////////////////////////////////////////////////////////////////////////////////
void dcache_init(void)
{
	int cnt = 0;
	__sprclr(SPR_DCCR, 0);
	mtspr(SPR_DCAIR, cnt << 4);
	// enable D-Cache
	// debg("D-cache open\n");
	__sprset(SPR_DCCR, 0);
}

void dcache_disable(void)
{
	int cnt = 0;

	__sprclr(SPR_SR, 3);
	// flush 256 lines
	//  for (cnt = 0; cnt <__CAHCHE_SIZE__/__LINE_SIZE__; cnt ++)
	//  {
	//    mtspr(SPR_DCBFR, cnt << 4);
	//  }
}

void dcache_enable(void)
{
	__sprset(SPR_SR, 3);
}

void dcache_writeback_all(void)
{
	int cnt = 0;

	// writeback 256 lines
	for (cnt = 0; cnt < __CAHCHE_SIZE__ / __LINE_SIZE__; cnt++)
	{
		mtspr(SPR_DCBWR, cnt << 4);
	}
}

void dcache_flush_all(void)
{
	int cnt = 0;

	// flush 256 lines
	for (cnt = 0; cnt < __CAHCHE_SIZE__ / __LINE_SIZE__; cnt++)
	{
		mtspr(SPR_DCBFR, cnt << 4);
	}
}

void dcache_invalid_all(void)
{
	int cnt = 0;

	// invalidate 256 lines
	for (cnt = 0; cnt < __CAHCHE_SIZE__ / __LINE_SIZE__; cnt++)
	{
		mtspr(SPR_DCBIR, cnt << 4);
	}
}

void dcache_writeback_region (uint32_t addr, uint32_t size)
{
	int len;
	int dccnt ;	
	if(addr & BIT(31))
		return;
	if(addr < 0x2000000)
		return;	

	while(size){
		if(size >= 4096){
			len = 4096;
			dccnt = ((addr + len -1) - (addr & ~0x0f)) / 16;
			SPR_DCCNT = dccnt; 
			mtspr(SPR_DCBWR, addr);
			addr += 4096;
			size -= 4096;
		}else{
			len = size;
			dccnt = ((addr + len -1) - (addr & ~0x0f)) / 16;
			SPR_DCCNT = dccnt + 1; 
			mtspr(SPR_DCBWR, addr);
			size = 0;
		}	
	}
}


void dcache_flush_region (uint32_t addr, uint32_t size)
{
	u32 len;
	u32 dccnt ;
	if(addr & BIT(31))
		return;
	if(addr < 0x2000000)
		return;
		
	while(size){
		if(size >= 4096){
			len = 4096;
			dccnt = ((addr + len -1) - (addr & ~0x0f)) / 16 ;
			SPR_DCCNT = dccnt;
			mtspr(SPR_DCBFR, addr);
			addr += 4096;
			size -= 4096;
		}else{
			len = size;
			dccnt = ((addr + len -1) - (addr & ~0x0f)) / 16 ;
			SPR_DCCNT = dccnt+1;
			mtspr(SPR_DCBFR, addr);
			size = 0;
		}
		//printf("dcnt:%d",dccnt);
	}

}
#if 0
void dcache_invalid_region(uint32_t addr, uint32_t size)
{
	u32 len;
	u32 dccnt;
	if (addr & BIT(31))
		return;
	if (addr < 0x44000)
		return;
	addr=(addr & ~0x0f);
	size+=(addr & 0x0f);
	size=((size+15)&~0x0f);
	while (size)
	{
		if (size >= 4096)
		{
			len = 4096;
			dccnt = ((addr + len) - (addr & ~0x0f)) / 16;
			SPR_DCCNT = dccnt;
			mtspr(SPR_DCBIR, addr);
			addr += 4096;
			size -= 4096;
		}
		else
		{
			len = size;
			dccnt = ((addr + len) - (addr & ~0x0f)) / 16;
			SPR_DCCNT = dccnt;
			mtspr(SPR_DCBIR, addr);
			size = 0;
		}
	}
}
#endif


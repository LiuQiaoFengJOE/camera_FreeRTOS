#include "memPool.h"

extern unsigned long arch_local_irq_save(void);
extern void arch_local_irq_restore(unsigned long flags);

uint32 mempCreate(memPool* memp,void* ptr,uint32 size,uint32 blkSize)
{
	uint8* startAddr;
	uint8* endAddr;
	uint8* nextAddr;
	uint32 blkCnt=0;
	if(blkSize<sizeof(uint8*))
		blkSize=sizeof(uint8*);
	if(blkSize>size)
		return 0;
	endAddr=(uint8*)ptr+size;
	startAddr=(uint8*)ptr;
	memp->mempPtr=(void*)startAddr;
	nextAddr=startAddr+blkSize;
	while(nextAddr<=endAddr)
	{
		*(uint8**)startAddr=nextAddr;
		startAddr=nextAddr;
		nextAddr=startAddr+blkSize;
		blkCnt++;
	}
	memp->blkSize=blkSize;
	memp->freeBlks=blkCnt;
	memp->maxBlks=blkCnt;
	memp->minFreeBlks=blkCnt;
	
	return memp->maxBlks;
}

void* mempGet(memPool* memp)
{
	void* ptr;	
	uint32 flags;
	flags=arch_local_irq_save();
	if(memp->freeBlks==0)
	{
		arch_local_irq_restore(flags);
		return (void*)0;
	}
	ptr=memp->mempPtr;
	memp->freeBlks--;
	if(memp->freeBlks)
		memp->mempPtr=*(void**)ptr;
	else
		memp->mempPtr=(void*)0;
	if(memp->minFreeBlks>memp->freeBlks)
		memp->minFreeBlks=memp->freeBlks;
	arch_local_irq_restore(flags);
	return ptr;
}

uint32 mempPut(memPool* memp,void* ptr)
{	
	uint32 flags;
	flags=arch_local_irq_save();
	if(memp->freeBlks>=memp->maxBlks)
	{	
		arch_local_irq_restore(flags);
		return 0;
	}
	*(void**)ptr=memp->mempPtr;
	memp->mempPtr=ptr;
	memp->freeBlks++;
	arch_local_irq_restore(flags);
	return memp->freeBlks;
}

void mempInfor(memPool* memp)
{
	uint32 flags;
	flags=arch_local_irq_save();
	debug_msg("memPool blkSize:%d\n",memp->blkSize);
	debug_msg("memPool freeBlks:%d\n",memp->freeBlks);
	debug_msg("memPool maxBlks:%d\n",memp->maxBlks);
	debug_msg("memPool minFreeBlks:%d\n",memp->minFreeBlks);
	arch_local_irq_restore(flags);
}


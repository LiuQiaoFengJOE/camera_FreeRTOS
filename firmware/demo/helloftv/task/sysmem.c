#include "typedef.h"

#define MEM_NODE_MAX 32
typedef struct MEM_NODE_S
{
	u32 flag;
	u32 addr;
	u32 size;
	struct MEM_NODE_S *next;
} MEM_NODE_T;
static MEM_NODE_T memNode[MEM_NODE_MAX], *idleMem, *busyMem;
static u32 halMemStart, halMemEnd, halMemIdx;
extern u32 _sdram_heap_start;
extern u32 _sdram_heap_end;

static MEM_NODE_T *memNodeMalloc(void)
{
	MEM_NODE_T *node;
	node = idleMem;
	if (node)
	{
		idleMem = node->next;
		node->flag = 0;
		node->addr = 0;
		node->size = 0;
	}

	return node;
}
static void memNodeFree(MEM_NODE_T *node)
{
	if (node)
	{
		node->next = idleMem;
		idleMem = node;
		node->flag = 0;
	}
}
void sysMemPrint(void)
{
	MEM_NODE_T *node;

	node = busyMem;
	while (node)
	{
		printf("mem:[%d] %x,%x,\n", node->flag, node->addr, node->size);

		node = node->next;
	}
}
void sysMemInit(void)
{
	MEM_NODE_T *node;
	int i;

	// ax32xx_sysInit((u32 *)&halMemStart,(u32 *)&halMemEnd);
	halMemStart = (u32)&_sdram_heap_start;
	halMemEnd = (u32)&_sdram_heap_end;

	halMemStart = (halMemStart + 0x3f) & (~0x3f);
	halMemIdx = halMemStart;

	idleMem = &memNode[0];
	for (i = 0; i < MEM_NODE_MAX; i++)
	{
		if (i == (MEM_NODE_MAX - 1))
			memNode[i].next = NULL;
		else
			memNode[i].next = &memNode[i + 1];
		memNode[i].flag = 0;
		memNode[i].addr = 0;
		memNode[i].size = 0;
	}
	busyMem = NULL;
	node = memNodeMalloc(); // memory end node
	node->size = 0;
	node->next = NULL;
	node->flag = 1;
	busyMem = node;
	node = memNodeMalloc(); // memory start node
	node->addr = halMemStart;
	node->size = halMemEnd - halMemStart;
	node->flag = 0;
	node->next = busyMem;
	busyMem = node;
	printf("total mem:%dKB\n", node->size >> 10);
	return 0;
}
void *sysMemMalloc(u32 size, u8 algin)
{
	u32 addr;
	MEM_NODE_T *cur, *node;
	u32 flag;
	flag = arch_local_irq_save();
	node = memNodeMalloc();
	if (node == NULL) // no more node
	{
		arch_local_irq_restore(flag);
		return NULL;
	}
	size = ((size + 0x3f) & (~0x3f));
	cur = busyMem;
	while (cur)
	{
		if ((cur->flag == 0) && (cur->size >= size))
			break;
		cur = cur->next;
	}
	if (cur == NULL) // no more memory
	{
		memNodeFree(node);
		sysMemPrint();
		arch_local_irq_restore(flag);
		return NULL;
	}
	addr = cur->addr;
	node->size = cur->size - size; // algin -64
	node->addr = cur->addr + size;
	node->flag = 0;
	node->next = cur->next;
	cur->next = node;
	cur->flag = 1;
	cur->size = size;
	arch_local_irq_restore(flag);
	return ((void *)addr);
}
void sysMemFree(void *mem)
{
	if (mem == NULL) // address  is null
		return;
	MEM_NODE_T *prev, *cur, *node;
	u32 addr;
	u32 flag;
	flag = arch_local_irq_save();
	addr = (u32)mem;

	cur = busyMem;
	prev = NULL;
	while (cur)
	{
		if ((cur->flag) && (cur->addr == addr))
			break;
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL) // can not find this address
	{
		arch_local_irq_restore(flag);
		return;
	}

	node = cur->next;
	cur->flag = 0;
	if (node && (node->flag == 0))
	{
		cur->size += node->size;
		cur->next = node->next;
		node->next = NULL;
		memNodeFree(node);
	}
	if (prev && (prev->flag == 0))
	{
		prev->size += cur->size;
		prev->next = cur->next;
		cur->next = NULL;
		memNodeFree(cur);
	}
	arch_local_irq_restore(flag);
}
u32 sysMemRemain(void)
{
	MEM_NODE_T *node, *mnode;
	u32 freesize = 0, busysize = 0;
	u32 flag;
	flag = arch_local_irq_save();
	node = busyMem;
	mnode = NULL;
	while (node)
	{
		if (node->flag == 0)
		{
			if (mnode)
			{
				if (mnode->size < node->size)
					mnode = node;
			}
			else
				mnode = node;
			freesize += node->size;
		}
		else
		{
			busysize += node->size;
		}
		node = node->next;
	}
	arch_local_irq_restore(flag);
	if (mnode)
	{
		printf("remain mem:%dKB\n", mnode->size >> 10);
	}
	if (mnode)
		return mnode->size;
	else
		return 0;
}

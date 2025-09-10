#ifndef __MEMORY_H__
#define __MEMORY_H__

// #include <sys_config.h>

#define USE_MEM_DEBUG 0
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 0
#define MEM_ALIGNMENT 4
#define USE_MEM_ASSERT 0

/* MEM_SIZE would have to be aligned, but using 64000 here instead of
 * 65535 leaves some room for alignment...
 */

#ifndef MEM_SIZE_T
#define MEM_SIZE_T
typedef unsigned int mem_size_t;
#endif

typedef unsigned int _mem_ptr_t;

// void  mem_init(void);
// void *mem_calloc_ex(const char *name, mem_size_t count, mem_size_t size);
// void *mem_malloc_ex(const char *name, mem_size_t size);
// #define mem_malloc(SIZE)	mem_malloc_ex(__FUNCTION__, (SIZE))
#ifndef WIFI_XR819_SUPPORT
#define mem_init()
#endif

void sys_meminit(void *pool, unsigned int size);

#if USE_MEM_DEBUG

void *mem_malloc_ex(const char *name, mem_size_t size);
void *mem_calloc_ex(const char *name, mem_size_t count, mem_size_t size);

void *pvPortMalloc(size_t xWantedSize, char *name);
#define mem_malloc(SIZE) pvPortMalloc(SIZE, __func__) // mem_malloc_ex(__FUNCTION__, SIZE)
#define mem_malloc_ex(FUNC, SIZE) pvPortMalloc(SIZE, __func__)
#define mem_calloc(SIZE, COUNT) mem_calloc_ex(__FUNCTION__, (SIZE), (COUNT))
// #define dma_memcpy(SIZE, COUNT) mem_calloc_ex(__FUNCTION__, (SIZE), (COUNT))
#define mem_memcpy(DST, SRC, CNT) dma_memcpy(DST, SRC, CNT) // dma_memcpy(DST, SRC, CNT)
#else

void *mem_malloc(mem_size_t size);
void *mem_calloc(mem_size_t count, mem_size_t size);
#endif

// #define malloc 		pvPortMalloc
#define malloc mem_malloc
#define free vPortFree     // mem_free
#define mem_free vPortFree // mem_free//
#define memcpy dma_memcpy
void mem_slide_check(int show_list);

// uint32_t mem_free(void *rmem);
uint32_t get_mem_size(void *rmem);
#endif /*__LWIP_MEM_H__*/

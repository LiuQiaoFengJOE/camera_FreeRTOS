#ifndef _STREAM_MALLOC_H
#define _STREAM_MALLOC_H
typedef void (*Streamfree)(void *buf);

void stream_Malloc_space_init(void *buf, unsigned int total_space_size);
void stream_all_space_free(Streamfree streamFree);
void *stream_malloc(unsigned int size);
void stream_free(void *buf);
void stream_Malloc_space_init_again();

#endif

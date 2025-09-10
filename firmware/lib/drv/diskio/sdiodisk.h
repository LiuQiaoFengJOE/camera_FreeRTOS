#ifndef __SDIODISK_H
#define __SDIODISK_H
#define WARNING_CAP 1024 * 50 // sd空间不足的警告值

typedef struct
{
	void *controller;
	void (*io_init)(void);
	u32 status;
} sdcard_dev;

#endif

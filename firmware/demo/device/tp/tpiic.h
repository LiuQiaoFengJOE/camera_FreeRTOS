#ifndef _TPIIC_H
#define  _TPIIC_H
//#include "../../hal/inc/hal.h"
#include "typedef.h"

void tp_iic_init();
int iicWrite1(unsigned char slaveid,unsigned char addr, unsigned char *buf, unsigned int len);
int iicRead1(unsigned char slaveid,unsigned char addr, unsigned char *buf, unsigned int len);
int iicWrite2(unsigned char slaveid,unsigned short addr, unsigned char *buf, unsigned int len);
int iicRead2(unsigned char slaveid,unsigned short addr, unsigned char *buf, unsigned int len);
int iicWrite3(unsigned char slaveid,unsigned int addr, unsigned char *buf, unsigned int len);
int iicRead3(unsigned char slaveid,unsigned int addr, unsigned char *buf, unsigned int len);

int _touch_panel_read(unsigned char w_chip_id, unsigned char r_chip_id, unsigned int register_address, unsigned char *buf, unsigned int len);
int _touch_panel_write(unsigned char w_chip_id, unsigned short register_address, unsigned char *buf, unsigned int len);
int _touch_prog_read(unsigned char w_chip_id, unsigned char r_chip_id, unsigned int register_address, unsigned char *buf, unsigned int len);
int _touch_prog_write(unsigned char w_chip_id, unsigned short register_address, unsigned char *buf, unsigned int len);





#endif


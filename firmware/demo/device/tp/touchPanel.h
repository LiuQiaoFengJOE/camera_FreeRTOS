#ifndef _TOUCH_PANEL_H
#define _TOUCH_PANEL_H
//#include "../../hal/inc/hal.h"
#include "typedef.h"

typedef struct
{
	int x;
	int y;
}point;

typedef struct _touchPanel
{
	char* name;
	unsigned int osdX0;
	unsigned int osdY0;
	unsigned int osdX1;
	unsigned int osdY1;
	unsigned int lcdW;
	unsigned int lcdH;
	unsigned int tpW;
	unsigned int tpH;
	void (*init)(void);
	int (*match)(void);
	int (*getPoint)(point*);
	struct _touchPanel* next;
}touchPanel;

#define GESTRUE_TYPE_NONE            0
#define GESTRUE_TYPE_PRESS           1
#define GESTRUE_TYPE_MOVE            2
#define GESTRUE_TYPE_ERROR           3



#define SLID_DIRECT_NONE       0
#define SLID_DIRECT_UP         1
#define SLID_DIRECT_DOWN       2
#define SLID_DIRECT_RIGHT      3
#define SLID_DIRECT_LEFT       4

typedef struct
{
	unsigned int type;
	unsigned int x;
	unsigned int y;
	unsigned int slidDiretion;
	unsigned int speed;
}gestrueInfor;

gestrueInfor* touchPanelGetInfor(int moveThreshold);

#define TOUCH_PANEL(name,tpW,tpH,init,match,getPoint) touchPanel name={#name,0,0,0,0,0,0,tpW,tpH,init,match,getPoint,NULL};

extern touchPanel icnt81TP;


void touchPanelHardWareInit(void);
void touchPanelInit(void);





#endif

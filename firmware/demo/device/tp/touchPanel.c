#include "touchPanel.h"
#include "tpiic.h"
//#include "../../ax32_platform_demo/inc/display.h"
//#include "../../xos/xos.h"

//#define TP_RES_CH    GPIO_PA
//#define TP_RES_PIN   GPIO_PIN7
touchPanel* TP=NULL;
static gestrueInfor gsInfor;

static void registerTouchPanel(touchPanel* p)
{
	if(p->init)
		p->init();
	p->next=TP;
	TP=p;
}
static touchPanel* getTouchPanel(void)
{
	while(TP!=NULL)
	{
		if(TP->match&&TP->getPoint)
			if(TP->match())
				return TP;
		TP=TP->next;
	}
	return NULL;
}
void touchPanelHardWareInit(void)
{
#if 0//用的硬件复位
	hal_gpioInit(TP_RES_CH,TP_RES_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
	hal_gpioWrite(TP_RES_CH,TP_RES_PIN,GPIO_LOW);
	XOSTimeDly(20);
	hal_gpioWrite(TP_RES_CH,TP_RES_PIN,GPIO_HIGH);
#endif
	/*XOSTimeDly*/osal_task_sleep(60);
	tp_iic_init();
// registerTouchPanel begin
	registerTouchPanel(&icnt81TP);
// registerTouchPanel end
	TP=getTouchPanel();
	if(TP==NULL)
	{
		printf("do not find touch panel!!!\n");
	}else{

		printf("tpPanel init end******************************************\n");
	}
}

void touchPanelInit(void)
{
	unsigned short w=0,h=0;

	if(TP==NULL)
		return;
#if 0//用的硬件复位
	hal_gpioInit(TP_RES_CH,TP_RES_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
	hal_gpioWrite(TP_RES_CH,TP_RES_PIN,GPIO_LOW);
	XOSTimeDly(20);
	hal_gpioWrite(TP_RES_CH,TP_RES_PIN,GPIO_HIGH);
#endif
	/*XOSTimeDly*/osal_task_sleep(60);
	tp_iic_init();
	if(TP->init)
		TP->init();
	/*hal_lcdGetResolution*/lcdGetRes(&w,&h);
	TP->lcdW=w;
	TP->lcdH=360;//h;
	/*hal_lcdGetOSDShowingPosition*/lcdGetOsdPos(&w,&h);
	TP->osdX0=w;
	TP->osdY0=0;//h;
	//dispLayerGetResolution(DISP_LAYER_OSD0,&w,&h);
	lcdGetOsdRes(&w,&h);
	TP->osdX1=TP->osdX0+w-1;
	TP->osdY1=360;//TP->osdY0+h-1;
	printf("touch panel[%s]:width[%d],high[%d]\n",TP->name,TP->tpW,TP->tpH);
	printf("lcd resolution:width[%d],high[%d]\n",TP->lcdW,TP->lcdH);
	printf("osd region:[%d %d %d %d]\n",TP->osdX0,TP->osdY0,TP->osdX1,TP->osdY1);
	gsInfor.type=GESTRUE_TYPE_NONE;
}

#define distanceOfPoint(p1,p2)     (abs(p1.x-p2.x)+abs(p1.y-p2.y))

gestrueInfor* touchPanelGetInfor(int moveThreshold)
{
	static point lastPoint;
	static int restart=1;
	unsigned int distanceX,distanceY;
	int ret=0,retry=3;
	point curPoint={0,0};
	if(TP==NULL)
		goto END;
	while(retry)
	{
		ret=TP->getPoint(&curPoint);
		if(ret>=0)
		{
			break;
		}
		retry--;
	}
	if (ret>0)
		ret = 1;
	
	if(ret==1)
	{
#if 0
		printf("TP->lcdW=%d \n",TP->lcdW);
		printf("TP->lcdH=%d \n",TP->lcdH);
		
		printf("TP->tpW=%d \n",TP->tpW);
		printf("TP->tpH=%d \n",TP->tpH);

		printf("TP->osdX0=%d \n",TP->osdX0);
		printf("TP->osdY0=%d \n",TP->osdY0);
#endif	

#if 1
		curPoint.x=curPoint.x*TP->lcdW/TP->tpW;
		curPoint.y=curPoint.y*TP->lcdH/TP->tpH;
		if(curPoint.x>=TP->osdX0&&curPoint.x<=TP->osdX1&&
			curPoint.y>=TP->osdY0&&curPoint.y<=TP->osdY1)
		{
#if 0		
			gsInfor.x=TP->tpW-(curPoint.x-TP->osdX0);
			gsInfor.y=TP->tpH-(curPoint.y-TP->osdY0);
#else
			gsInfor.x=curPoint.x-TP->osdX0;
			gsInfor.y=curPoint.y-TP->osdY0;	
#endif
		}
		else
		{
			ret=0;
		}
#endif

	}
	
	if(ret==1)
	{
		//printf("&&&&&&&gsInfor   x:%d,y:%d\n",gsInfor.x,gsInfor.y);
		if(restart)
		{
			restart=0;
			//printf("=$NONE1=\n");
			gsInfor.type=GESTRUE_TYPE_NONE;
			goto END;
		}
		
		distanceX=abs(curPoint.x-lastPoint.x);
		distanceY=abs(curPoint.y-lastPoint.y);
		if(gsInfor.type==GESTRUE_TYPE_MOVE||distanceX>=moveThreshold||distanceY>=moveThreshold)
		{
			if(distanceX<=3&&distanceY<=3)
			{
				gsInfor.speed=0;
				goto END;
			}
			if(distanceX>distanceY)
			{
				//printf("=#MOVE1=\n");
				gsInfor.type=GESTRUE_TYPE_MOVE;
				if(curPoint.x>lastPoint.x)
					gsInfor.slidDiretion=SLID_DIRECT_RIGHT;
				else
					gsInfor.slidDiretion=SLID_DIRECT_LEFT;
				gsInfor.speed=distanceX*1000/TP->lcdW;
			}
			else
			{
				//printf("=#MOVE2=\n");
				gsInfor.type=GESTRUE_TYPE_MOVE;
				if(curPoint.y>lastPoint.y)
					gsInfor.slidDiretion=SLID_DIRECT_DOWN;
				else
					gsInfor.slidDiretion=SLID_DIRECT_UP;
				gsInfor.speed=distanceY*1000/TP->lcdH;
			}
		}
		else
		{
			//printf("=@PRESS1=ret:%d\n", ret);
			//osal_task_sleep(5);
			gsInfor.type=GESTRUE_TYPE_PRESS;
		}

	}
	else if(ret<0)
	{
		restart=1;
		//printf("=+error=ret:%d\n", ret);
		gsInfor.type=GESTRUE_TYPE_ERROR;
	}
	else
	{
		if(restart==0&&gsInfor.type==GESTRUE_TYPE_NONE)
		{
			//printf("=@PRESS2=ret:%d\n", ret);
			//osal_task_sleep(5);
			restart=1;
			gsInfor.type=GESTRUE_TYPE_PRESS;
			goto END;
		}
		restart=1;
		//printf("=$NONE2=\n");
		gsInfor.type=GESTRUE_TYPE_NONE;
	}
END:
	lastPoint.x=curPoint.x;
	lastPoint.y=curPoint.y;
	//printf("----gsInfor:%d,%d, %d-, ret:%d\n", gsInfor.x, gsInfor.y, gsInfor.type, ret);
	return &gsInfor;
}






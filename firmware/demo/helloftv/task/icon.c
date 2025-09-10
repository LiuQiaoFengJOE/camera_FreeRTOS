#include "task/app.h"

#define  ICON_DATA_ADDR             RES_OSD_SOURCE
#define  ICON_PALETTE_ADDR        RES_PALETTE


typedef struct Icon_Info_S
{
	u32 width;
	u32 height;
	u32 offset;
}Icon_Info_T;
typedef struct Icon_Ctrl_S
{
	u32 Icon;
	u32 iconAddr;

	Icon_Info_T iconInfo;
}Icon_Ctrl_T;
static Icon_Ctrl_T iconCtrl;
extern R_ICON_T User_Icon_Table[];

int iconInit(void)
{
	int ret;
	ret = res_open(ICON_DATA_ADDR);
	if(ret<0)
	{
		printf("icon : find data fail\n");
		return -1;
	}
    iconCtrl.iconAddr = ret;
	iconCtrl.Icon = 0xffffffff;
    printf("icon : initial ok.0x%x\n",iconCtrl.iconAddr);
	return 0;
}

int iconGetSize(int num,INT16U *width,INT16U *height)
{
    if(iconCtrl.Icon != num)
    {
		res_read(iconCtrl.iconAddr+sizeof(Icon_Info_T)*num,&iconCtrl.iconInfo,sizeof(Icon_Info_T));
		iconCtrl.Icon = num;
    }

	if(width)
		*width = iconCtrl.iconInfo.width;
	if(height)
		*height = iconCtrl.iconInfo.height;
	return 0;
}
int iconGetAddr(int num)
{
	if(iconCtrl.Icon != num)
	{
		res_read(iconCtrl.iconAddr+sizeof(Icon_Info_T)*num,&iconCtrl.iconInfo,sizeof(Icon_Info_T));
		iconCtrl.Icon = num;
	}
	return (iconCtrl.iconAddr+iconCtrl.iconInfo.offset);
}

static int R_iconGetIndex(unsigned long int r_id)
{
    unsigned int index,i;
    R_ICON_T *table;
    
    if(r_id>=R_ICON_MAX)
       return -1;
       
    if((r_id & R_ID_TYPE_ICON)==0)
       return -1;
    
    index = r_id &R_ID_MASK;
    table = User_Icon_Table+index;
    if(table->r_id == r_id)
       return index;
    
    for(i=0;i<(R_ICON_MAX&R_ID_MASK);i++)
    {
        if(table->r_id == r_id)
           return i;
    }
    return -1;
}

static void *R_iconGetName(unsigned int id)
{
	unsigned int idx,i;

	idx = id &R_ID_MASK;
	if(idx>=(R_ICON_MAX&R_ID_MASK))
		return NULL;
	
	if(User_Icon_Table[idx].r_id == id)
		return ((void *)User_Icon_Table[idx].r_name);
	
	for(i=0;i<(R_ICON_MAX&R_ID_MASK);i++)
	{
		if(id == User_Icon_Table[idx].r_id)
			break;
	}

	if(i>=(R_ICON_MAX&R_ID_MASK))
		return NULL;
	else
		return ((void *)User_Icon_Table[i].r_name);
}
static void *R_iconLoadResData(R_ICON_T *icon)
{
    void *iconname;
	int ret;
	iconname = R_iconGetName(icon->r_id);
    
    if(icon->r_height == 0|| icon->r_width==0)
    {
        ret = iconGetSize((u32)iconname,&(icon->r_width),&(icon->r_height));
        if(ret<0)
            return NULL;
    }
    icon->r_data = (void *)iconGetAddr((u32)iconname);
	return icon->r_data;
}

void *R_iconGetData(unsigned long int r_id)
{
    int index;
    R_ICON_T *table;
	//printf("R : get data : 0x%x\r\n",r_id);
    index = R_iconGetIndex(r_id);
    if(index<0)
    {
//		printf("R icon : get index error : 0x%x\r\n",r_id);
       return (void *)0;
    }
    table = User_Icon_Table+index;
	if(table->r_data == (void *)0)
	{
		R_iconLoadResData(table);
	}     
    return (void *)table->r_data;     
}
int R_iconGetSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
    int index;
    R_ICON_T *table;
    index = R_iconGetIndex(r_id);
    if(index<0)
       return -1;
    table = User_Icon_Table+index;
	if(table->r_width ==0 || table->r_height == 0)
	     iconGetSize((u32)table->r_name,&(table->r_width),&(table->r_height));   
    if(width)
       *width = table->r_width;
    if(height)
       *height = table->r_height;
       
    return 0;   
}
void *R_iconGetDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
    int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return (void *)0;

	if(R_iconGetData(r_id)==NULL)
		return NULL;
       
    table = User_Icon_Table+index;
    if(width)
       *width = table->r_width;
    if(height)
       *height = table->r_height;
       
    return table->r_data;      
}

unsigned int R_getTColor(unsigned long int r_id)
{
	int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return 0xffffffff;

	table = User_Icon_Table+index;

	return table->r_tcolor;
}	


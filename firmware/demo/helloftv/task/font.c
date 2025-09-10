#include "task/app.h"


#define  FONT_DATA                RES_RESFONT
#define  FONT_INDEX              RES_RESFONTIDX


typedef struct Font_Data_S
{
	u16 width;
	u16 height;
	u32 offset;
}Font_Data_T;

typedef struct Font_Idx_S
{
	u32 index;
	u32 offset;
}Font_Idx_T;
typedef struct Font_Str_S
{
	u16 width;
	u16 height;
	u16 number;
	u16 offset;
}Font_Str_T;

typedef struct Font_Ctrl_S
{
	u8 lanMax;
	u8 strMax;
	u8 curLan;
	u8 curStr;

    u32 curChar;
	u32 charMax;

	u32 curOffset;

	Font_Str_T strInfo;
	Font_Data_T charInfo;

	u32 addrData;
	u32 addrIndex;

	u16 strCache[128];
    u8  charCache[1024];
}Font_Ctrl_T;

static u8 CH_INV_W = 0;

static Font_Ctrl_T fontCtrl;

int fontInit(void)
{
	int addr;

	fontCtrl.curStr = 0xff;
	fontCtrl.curLan = 0xff;
	fontCtrl.strMax = 0;
	fontCtrl.curChar= 0xffffffff;

	addr = res_open(FONT_DATA);
	if(addr <0)
	{
		printf("font : initial fail.can not find data\n");
		return -1;
	}
	fontCtrl.addrData = addr;
	res_read(addr,&fontCtrl.charMax,4);  // get counter of char
	
	addr = res_open(FONT_INDEX);
	if(addr<0)
	{
		printf("font : initial fail.can not find index\n");
		return -1;
	}
	fontCtrl.addrIndex = addr;
    res_read(fontCtrl.addrIndex,&addr,4); 
	if((addr&0x0000ffff) != 0x0000584d)
	{
		printf("font : initial fail.index error\n");
		return -2;
	}
    fontCtrl.lanMax = (addr>>24)&0xff;
	CH_INV_W = (addr>>16)&0xff;
	printf("font : initial ok.max lan =%d,max char %d,CH_INV_W=%d\n",fontCtrl.lanMax,fontCtrl.charMax,CH_INV_W);
    return fontCtrl.lanMax;	
}
int fontSetLanguage(u8 num)
{
	Font_Idx_T index;
	
	if(num>=fontCtrl.lanMax)
		return -1;

	if(num == fontCtrl.curLan)
		return 0;

	res_read(fontCtrl.addrIndex+(num+1)*8,&index,8); 
	if(num!=(index.index&0xff))
		return -1;
	fontCtrl.curOffset = index.offset;
	fontCtrl.curLan = num;
	fontCtrl.strMax = (index.index>>8)&0xff;
// all cache is invliad	
	fontCtrl.curStr = 0xff;
	fontCtrl.curChar= 0xffffffff;
    printf("font : language = %d,str max = %d\n",num,fontCtrl.strMax);
	return fontCtrl.curLan;
}
static int fontGetCharData(u32 num,u8 *buffer)
{
	int temp;
	if(num>=fontCtrl.charMax)
		return -1;

	if(num != fontCtrl.curChar)
	{
		res_read(fontCtrl.addrData+(num+1)*8,&fontCtrl.charInfo,8);
	}
    temp = fontCtrl.charInfo.height*((fontCtrl.charInfo.width+7)>>3);
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
    res_read(fontCtrl.addrData+fontCtrl.charInfo.offset,buffer,temp); // read data

	return 0;
}

int fontGetStringInfo(u8 num,u16 *width,u16 *height)
{
	if(num>=fontCtrl.strMax)
		return -1;
	if(num!=fontCtrl.curStr)
	{
		res_read(fontCtrl.addrIndex+fontCtrl.curOffset+(num+1)*8,&fontCtrl.strInfo,8);
	}

	if(width)
		*width = fontCtrl.strInfo.width;
	if(height)
		*height = fontCtrl.strInfo.height;

	return 0;
}

static int fontGetString(u8 num,u16 *buffer)
{
	int temp;
	if(num>=fontCtrl.strMax)
		return -1;
	if(num!=fontCtrl.curStr)
	{
		res_read(fontCtrl.addrIndex+fontCtrl.curOffset+(num+1)*8,&fontCtrl.strInfo,8);
	}
    temp = fontCtrl.strInfo.number<<1;
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
	res_read(fontCtrl.addrIndex+fontCtrl.curOffset+fontCtrl.strInfo.offset,buffer,temp);

	return fontCtrl.strInfo.number;
}

u32 resGetString(u8 id,u16 *width,u16 *height)
{
	int temp;
	if(id>=fontCtrl.strMax)
		return 0;
	if(id!=fontCtrl.curStr)
	{
		res_read(fontCtrl.addrIndex+fontCtrl.curOffset+(id+1)*8,&fontCtrl.strInfo,8);
		fontCtrl.curStr=id;
	}
	if(fontCtrl.strInfo.number>128)
		fontCtrl.strInfo.number=128;
    	temp = fontCtrl.strInfo.number<<1;
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
	res_read(fontCtrl.addrIndex+fontCtrl.curOffset+fontCtrl.strInfo.offset,fontCtrl.strCache,temp);
	if(width)
		*width = fontCtrl.strInfo.width;
	if(height)
		*height = fontCtrl.strInfo.height;
	return fontCtrl.strInfo.number;
}
u8* resGetChar(u8 id,u8 num,u16 *width,u16 *height,uint8* special)
{
    u8 *charCache = fontCtrl.charCache;
	if(id>=fontCtrl.strMax)
		return NULL;
	if(id!=fontCtrl.curStr)
	{
		resGetString(id,NULL,NULL);
	}
	if(num>=fontCtrl.strInfo.number)
		return NULL;
	if(fontGetCharData(fontCtrl.strCache[num]&0x3fff,charCache)<0)
		return NULL;
	if(fontCtrl.strCache[num]&0x8000)
	{
		if(width)
			*width = fontCtrl.charInfo.width;
		if(height)
			*height = (fontCtrl.charInfo.height>>2)+1;
		if(fontCtrl.strCache[num]&0x4000)
		{
			if(special)
				*special=2;
			if(height)
				*height = fontCtrl.charInfo.height;
		}
		else
		{
			if(special)
				*special=1;
			if(height)
				*height = (fontCtrl.charInfo.height>>2)+1;
		}
	}
	else
	{
		if(width)
			*width = fontCtrl.charInfo.width;
		if(height)
			*height = fontCtrl.charInfo.height;
		if(special)
			*special=0;
	}
	return charCache;
}






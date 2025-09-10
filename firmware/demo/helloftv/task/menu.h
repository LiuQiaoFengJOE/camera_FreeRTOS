#ifndef __MENU_H
#define __MENU_H
#include "config.h"

#define INDENT_STR   "    "

#define ARRAY_LEN(a)  (sizeof(a)/sizeof(a[0]))
typedef void (*menuItemProc)(winHandle handle,uint32 parameNum,uint32* parame);
typedef void (*netItemProc)(uint32 d);

typedef struct
{
	uint32 image;
	uint32 str;
	u32  netOptionId;
	char *name;
}menuOption;
typedef struct
{
	uint32 configId;
	uint32 image;
	uint32 str;
	uint32 optionSum;
	menuOption* pOption;
	u32  netItemId;
}menuItem;
typedef struct
{
	uint32 configId;
	uint32  netItemId;
	uint32 optionSum;
	menuOption* pOption;
	netItemProc proc;
	char *name;
	uint32 type;
}netItem;
typedef struct
{
	uint32 unselectImage;
	uint32 selectImage;
	uint32 str;
	uint32 itemSum;
	menuItem* pItem;
}menuPage;
typedef struct
{
	uint32 curPage;
	uint32 pageSum;
	menuPage* pPage;
}menu;

#define MENU_OPTION_START(name)  menuOption  menuOption##name[]={
#define MENU_OPTION_IMAGE_STR(image,str) {image,str,0xff,"NULL"},
#define MENU_OPTION_IMAGE(image)         {image,(uint32)0,0xff,"NULL"},
#define MENU_OPTION_STR(str)             {(uint32)0,str,0xff,"NULL"},
#define MENU_OPTION_STR_NET(str,netId)             {(uint32)0,str,netId,"NULL"},

#define MENU_OPTION_IMAGE_STRX(image,str,name) {image,str,0xff,name},
#define MENU_OPTION_IMAGEX(image,name)         {image,(uint32)0,0xff,name},
#define MENU_OPTION_STRX(netId,name)             {(uint32)0,0,netId,name},
#define MENU_OPTION_STR_NETX(str,netId,name)             {(uint32)0,str,netId,name},

#define MENU_OPTION_END()           };

#define MENU_ITME_START(name)    menuItem  menuItem##name[]={
#define MENU_ITEM_OPTIONS(name,id,image,str)  {id,image,str,ARRAY_LEN(menuOption##name),menuOption##name,0xff},
#define MENU_ITEM_NO_ID(name, image,str)      {(uint32)0,image,str,ARRAY_LEN(menuOption##name),menuOption##name,0xff},
#define MENU_ITEM_PROC(itemProc,image,str)    {(uint32)0,image,str,(uint32)0,(menuOption*)itemProc,0xff},
#define MENU_ITEM_OPTIONS_NET(name,id,image,str,netId)  {id,image,str,ARRAY_LEN(menuOption##name),menuOption##name,netId},
#define MENU_ITME_END()             };

#define MENU_PAGE_START(name)       menuPage  menuPage##name[]={
#define MENU_PAGE_ITEMS(name,unselectImage,selectImage,str)  {unselectImage,selectImage,str,ARRAY_LEN(menuItem##name),menuItem##name},
#define MENU_PAGE_END()             };

#define MENU_DEFINE(name)   menu  menu##name={(uint32)0,ARRAY_LEN(menuPage##name),menuPage##name};
#define MENU(name)          (menu##name)

#define NET_ITEM(name,id,netId)  {id,netId,ARRAY_LEN(menuOption##name),menuOption##name,NULL,"NULL",0},
#define NET_PROC(name,id,netId,proc)  {id,netId,ARRAY_LEN(menuOption##name),menuOption##name,proc,"NULL",0},
#define NET_ITEMX(name,id,netId,namex)  {id,netId,ARRAY_LEN(menuOption##name),menuOption##name,NULL,namex,0},
#define NET_PROCX(name,id,netId,proc,namex)  {id,netId,ARRAY_LEN(menuOption##name),menuOption##name,proc,namex,0},
#define XML_ITEM(netId,namex,type)  {CONFIG_ID_MAX,netId,0,NULL,NULL,namex,type},
#define XML_ITEMX(name,netId,namex,type)  {CONFIG_ID_MAX,netId,ARRAY_LEN(menuOption##name),menuOption##name,NULL,namex,type},


#endif

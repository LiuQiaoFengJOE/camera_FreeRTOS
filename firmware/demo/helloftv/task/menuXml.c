#include "application.h"
#include "uiWin.h"
#include "menu.h"
#include "tcpControl/tcpMessage.h"
#include "appcmd/socket_cmd.h"

int xmlOption(char* buff,char *name,u32 id,u32 indent)
{
	int len,i;
	len=0;
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Value>\n");
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Name>%s</Name>\n",name);
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<ID>0x%02X</ID>\n",id);
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"</Value>\n");
	return len;
}
int xmlItem(char* buff,netItem *item,u32 indent)
{
	int len,i;
	u32 curOption=configGet(item->configId);
	u32 curNetOptionId=0;
	len=0;
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Setting>\n");
//////////////////////////////////////////	
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Name>%s</Name>\n",item->name);
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<ID>0x%07X</ID>\n",item->netItemId);
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Type>0x%02X</Type>\n",item->type);
	if(item->netItemId==System_DefaultSetting)
	{
		for(i=0;i<indent+1;i++)
			len+=sprintf(&buff[len],INDENT_STR);
		len+=sprintf(&buff[len],"<Reflash>0x01</Reflash>\n");
	}
	
	if(item->configId!=CONFIG_ID_MAX)
	{
		for(i=0;i<item->optionSum;i++)
		{
			if(item->pOption[i].str==curOption)
			{
				curNetOptionId=item->pOption[i].netOptionId;
				break;
			}
		}
	}
	if(item->netItemId!=WiFi_Passwd)
	{
		for(i=0;i<indent+1;i++)
			len+=sprintf(&buff[len],INDENT_STR);
		if(item->netItemId==WiFi_Name)
			len+=sprintf(&buff[len],"<Default>jrxCamera</Default>\n");
		else
			len+=sprintf(&buff[len],"<Default>0x%02X</Default>\n",curNetOptionId);
	}
	if(item->optionSum)
	{
		for(i=0;i<indent+1;i++)
			len+=sprintf(&buff[len],INDENT_STR);
		len+=sprintf(&buff[len],"<Values>\n");
		//////////////////////////////////////
		for(i=0;i<item->optionSum;i++)
		{
			len+=xmlOption(&buff[len],item->pOption[i].name,item->pOption[i].netOptionId,indent+2);
		}
		/////////////////////////////////////
		for(i=0;i<indent+1;i++)
			len+=sprintf(&buff[len],INDENT_STR);
		len+=sprintf(&buff[len],"</Values>\n");
	}
////////////////////////////////////////	
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"</Setting>\n");
	return len;
}
int xmlItems(char* buff,netItem *items,u32 sum,char *name,u32 indent)
{
	int len,i;
	len=0;
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Category>\n");
///////////////////////////////////////
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Name>%s</Name>\n",name);
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"<Settings>\n");
	///////////////////////////////////
	for(i=0;i<sum;i++)
	{
		len+=xmlItem(&buff[len],&items[i],indent+2);
	}
	///////////////////////////////////
	for(i=0;i<indent+1;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"</Settings>\n");
///////////////////////////////////////
	for(i=0;i<indent;i++)
		len+=sprintf(&buff[len],INDENT_STR);
	len+=sprintf(&buff[len],"</Category>\n");
	return len;
}

void xmlTest(void)
{
#if 0
#define XML_BUFF   1024*25
	static char xmlStr[XML_BUFF];
	int len;
	//len=xmlOption(xmlStr,"option",12,3);
	//len=xmlItem(xmlStr,&netItems[0],1);
	len=xmlCreate(xmlStr,XML_BUFF);
	xmlStr[len]=0;
	printf("xml len:%d\n",len);
	Uart_PutStr(xmlStr);
	//printf("%s",xmlStr);
#endif
}


#include "application.h" 


int uintToString(char* str,uint8 len,uint32 data)
{
	char tmp[11];
	int n=0,i=0;
	if(data)
	{
		while(data)
		{
			tmp[n]=(data%10)+'0';
			data=data/10;
			n++;
		}
		len--;
		for(i=0;i<n;i++)
		{
			str[i]=tmp[n-i-1];
			if(i>=len)
			{
				str[i]=0;
				return i-1;
			}
		}
		str[i]=0;
		return i;
	}
	else
	{
		*str++='0';
		*str=0;
		return 1;
	}
	return n;
}
int IntToStr(char* str,u32 data)
{
	char tmp[11];
	int n=0,i=0;
	if(data)
	{
		while(data)
		{
			tmp[n]=(data%10)+'0';
			data=data/10;
			n++;
		}
		for(i=0;i<n;i++)
			str[i]=tmp[n-i-1];
	}
	else
	{
		*str='0';
		n=1;
	}
	return n;
}
int intToStringFill0(char* str,uint8 len,uint32 data,u32 sum)
{
	char tmp[11];
	int n=0,i=0;
	u32 fill0Sum=0;
	if(data)
	{
		while(data)
		{
			tmp[n]=(data%10)+'0';
			data=data/10;
			n++;
		}
		len--;
		if(n<sum&&n<len)
		{
			if(len>=sum)
				fill0Sum=sum-n;
			else
				fill0Sum=len-n;
			for(i=0;i<fill0Sum;i++)
				str[i]='0';
			len=len-fill0Sum;
		}
		for(i=0;i<n;i++)
		{
			str[i+fill0Sum]=tmp[n-i-1];
			if(i>=len)
			{
				str[i+fill0Sum]=0;
				return i-1;
			}
		}
		str[i+fill0Sum]=0;
		return i+fill0Sum;
	}
	else
	{
		if(len>sum)
			fill0Sum=sum;
		else
			fill0Sum=len-1;
		for(i=0;i<fill0Sum;i++)
			str[i]='0';
		str[i]=0;
		return i;
	}
}

char* debugDataString(char* str,u32 (*getData)(u32,u32*))
{
	//static char str[DEBUG_PARAMA_MAX*11];
	static int MaxLen=0;
	char* tmp=str;
	int i,len;
	u32 sum=1;
	for(i=0;i<sum;i++)
	{
		len=IntToStr(tmp,getData(i,&sum));
		*(tmp+len)=' ';
		tmp=tmp+len+1;
	}
	len=tmp-str;
	if(len>=MaxLen)
	{
		MaxLen=len;
	}
	else
	{
		len=MaxLen-len;
		while(len--)
			*tmp++=' ';
	}
	*tmp=0;
	return str;
}

int calWeek(int y,int m,int d)
{  
	int w;
	if((m==1)||(m==2))
	{
		m+=12;
		y--;
	}
	w=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7; 
	if(w>6)
		w=7;
	return w;
}
   




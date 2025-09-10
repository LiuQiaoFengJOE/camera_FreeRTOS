#include <stdio.h>
#include "utarray.h"



#define	yuvErr	0x20//yuv的一个容错
#define yuvUnit	0x02//连续多少个才算是需要替换的地方,最好是2
UT_array *nums;
UT_array *yuv_instead_table;

#define osal_malloc malloc
#define osal_free	free




void pre_deal_yuv(unsigned char *yuv_buf,unsigned int width,unsigned int height)
	{
		unsigned int i;
		unsigned int size = width*height;
		unsigned int tmp =0;
		for(i=0;i<size;i++)
		{
			if(*(yuv_buf+i)<=yuvErr)
			{
				if(i%2==1)
				{
					i++;
				}
				for(;i<size;i++)
				{
	
					if(*(yuv_buf+i)>yuvErr)
					{
	
						if(i%2)
						{
							*(yuv_buf+i) = 0;
							if((i/width)%2==0)
							{
								*(yuv_buf+i+width) = 0;
								
							}
							else
							{
								*(yuv_buf+i-width) = 0;
							}
						}
						break;
					}
					else
					{
						tmp =*(yuv_buf+i);
						if((i/width)%2==0)
						{
							*(yuv_buf+i+width) = 0;
							
						}
						else
						{
							*(yuv_buf+i-width) = 0;
						}
						*(yuv_buf+i) = 0;
					}
				}
			}
			tmp = *(yuv_buf+i);
			
		}
		
	}




UT_array *yuv_scan(unsigned char *yuv_buf,unsigned int width,unsigned int height)
{
	unsigned int start,end;
	unsigned int i,j,k,n;
	unsigned int block = 0;
	unsigned int num = 0;
	unsigned int test;
	unsigned int size = width*height;
	UT_array *table;
	int *p;
	
	for(i=0;i<height;i+=2)
	{
		for(j=0;j<width;j+=2)
		{
			n = i*width+j;
			if(*(yuv_buf+n)==0)
			{
				test = ((width>>1) *(i>>1))+(j>>1);
				k = size+test*2;
				*(yuv_buf+k) = 0;
				*(yuv_buf+k+1) = 0;
			}
		}
	}

	utarray_new(table,&ut_int_icd);
	utarray_push_back(table,&num);
	//size = size*3/2;
	for(i=0;i<size;i++)
	{
		if(*(yuv_buf+i)==0)
		{
			start = i;
			for(;i<size;i++)
			{
				if(*(yuv_buf+i))
				{
					break;
				}
			}
			end = i;
			block = end-start+1;
			if(block>=yuvUnit)
			{
				utarray_push_back(table,&start);
				utarray_push_back(table,&end);
				num++;
			}
		}
		block = 0;
		
	}
	utarray_push_back(table,&size);
	//printf("11111len:%d\r\n",utarray_len(table));
	



	size = size*3/2;
	utarray_push_back(table,&i);

	for(;i<size;i++)
	{
		if(*(yuv_buf+i)==0)
		{
			start = i;
			for(;i<size;i++)
			{
				if(*(yuv_buf+i))
				{
					break;
				}
			}
			end = i;
			block = end-start+1;
			if(block>=yuvUnit)
			{
				utarray_push_back(table,&start);
				utarray_push_back(table,&end);
				num++;
			}
		}
		block = 0;
		
	}
	utarray_push_back(table,&size);
	//printf("num:%d\r\n",num);
	//printf("22222len:%d\r\n",utarray_len(table));
	return table;
} 


unsigned int* get_list(UT_array *table,unsigned int *size1,int w,int h)
{
	int *p;
	unsigned int start,end,count;
	int flag = 0;
	unsigned int *frontList = (unsigned int *)osal_malloc(utarray_len(table)*sizeof(unsigned int));
	unsigned int *backList = (unsigned int *)osal_malloc(utarray_len(table)*sizeof(unsigned int));
	unsigned int *List = (unsigned int *)osal_malloc(utarray_len(table)*sizeof(unsigned int));
	int F=0,B=0,T=0;
	memset(frontList,0,utarray_len(table)*sizeof(unsigned int));
	memset(backList,0,utarray_len(table)*sizeof(unsigned int));
	memset(List,0,utarray_len(table)*sizeof(unsigned int));
	
	for(p=(int*)utarray_front(table);p!=NULL;) 
	{
		start = (unsigned int)*p;
		p=(int*)utarray_next(table,p);
		//printf("start:%d\n",start);
		//List[T] = start;
		//T++;
		#if 1
		if(start<=w*h && flag==0)
		{
			frontList[F] = start;
			F++;
			if(start==w*h)
			{
				if(F%2==0)
				{
					flag = 1;
					}
			}
			
		}
		else if (start>=w*h)
		{
			backList[B] = start;
			B++;
		}
		#endif
	}
	
	
	unsigned int k1,j1,i1;
	k1 = 0;
	j1 = 0;
	i1 = 0;
	unsigned int value = 0;
	while(1)
	{
#if 0
		//需要比较插值
		if(k1<F && j1<B)
		{
			if(frontList[k1]/2+w*h>=backList[j1])
			{
				value = backList[j1];
				j1++;
				List[i1++] = value;
				
				value = backList[j1];
				j1++;
				List[i1++] = value;
				continue;
			}
			else if(frontList[k1]/2+w*h<backList[j1])
			{
				value = frontList[k1];
				k1++;
				List[i1++] = value;
				value = frontList[k1];
				k1++;
				List[i1++] = value;
				continue;
			}
		}
		else if(k1<F)
#else
		if(k1<F)
#endif
		{
			value = frontList[k1];
			k1++;
			List[i1++] = value;
			continue;
		}
		else if(j1<B)
		{
			value = backList[j1];
			j1++;
			List[i1++] = value;
			continue;
		}
		else
		{
			//printf("i1:%d\n",i1);
			//printf("k1:%d\n",k1);
			//printf("j1:%d\n",j1);
			break;
		}
	}
	//printf("size addr:%X\n",size1);
	//printf("size1:%X\n",*size1);
	
	*size1 = i1;
	//printf("F:%d\tB:%d\n",F,B);
	//printf("frontList:%d\t%d\tbackList:%d\t%d\t%d\n",frontList[F-1],frontList[0],backList[B-1],backList[0],backList[0]);
	//printf("flag:%d\n",flag);
	//printf("size addr:%X\n",size1);
	//printf("fr1:%X\tba1:%X\n",frontList,backList);
	osal_free(backList);
	osal_free(frontList);
	return List;
}


//#define TEST
int main(int argc,char *argv[])
{
	FILE *fp;
	unsigned char *yuvbuf;
	unsigned char *yuvbuf2;
	unsigned int filesize;
	unsigned int ListSize = 0;
	unsigned int *List = NULL;
	unsigned int w,h,default_jpg_w,default_jpg_h;
	
	printf("argc:%d\n",argc);
	
	if(argc<5)
	{
		printf("input format err\n");
		printf("example:yuv.exe 1280 720 frame2.jpg frame2.bin\n");
		return 1;
	}
	w = atoi(argv[1]);
	h = atoi(argv[2]);
	printf("w:%d\th:%d\n",w,h);
	
	fp = fopen(argv[3],"rb");
	if(!fp)
	{
		printf("no found file:%s\n",argv[3]);
		return 1;
	}
 	fseek(fp,0,SEEK_END);//将文件位置指针置于文件结尾
	filesize=ftell(fp);//得到当前位置与文件开始位置的字节偏移量。
	fseek(fp,0,0);//将文件位置指针置于文件结尾
	yuvbuf = (unsigned char *)malloc(filesize);
	fread(yuvbuf,1,filesize,fp);
	fclose(fp);
	

	if(filesize!=w*h*3/2)
	{
		printf("filesize is not correct:%s\t%d\n",argv[3],filesize);
		free(yuvbuf);
		return 1;
	}
	

	
	
	
	
	default_jpg_w = w;
	default_jpg_h = h;
	//memset(yuvbuf2,128,filesize);
	
	//yuv的buf读取完毕
	
	
	//处理yuv的buf
	pre_deal_yuv(yuvbuf,default_jpg_w,default_jpg_h);
	
	yuv_instead_table = yuv_scan(yuvbuf,default_jpg_w,default_jpg_h);
	List = get_list(yuv_instead_table,&ListSize,default_jpg_w,default_jpg_h);
	printf("ListSize:%d\t%d\n",ListSize,sizeof(unsigned int));
	

	
	fp = fopen(argv[4],"wb");
	fwrite(List,sizeof(unsigned int),ListSize,fp);
	fclose(fp);
	
#ifdef TEST

	yuvbuf2 = (unsigned char *)malloc(filesize);
	fp = fopen("test.yuv","rb");
	printf("test.yuv:%d\n",fread(yuvbuf2,1,filesize,fp));
	fclose(fp);
	
	unsigned int i;
	unsigned int start,end;
	unsigned char *src_buf = yuvbuf;
	unsigned char *dest_buf = yuvbuf2;//需要分发的帧,因此这里要把相框复制到分发的yuv数据中
	
	for(i=0;i<ListSize;) 
	{
			
		start = List[i++];
		end = List[i++];

		if(end-start>=1)
		{
			memcpy(dest_buf+start,src_buf+start,end-start);
		}
	
	}

	fp = fopen("1.yuv","wb");
	fwrite( yuvbuf2,1,filesize,fp);
	fclose(fp);
	free(yuvbuf2);
#endif
	
	
	free(yuvbuf);
	return 0;
	
	

}
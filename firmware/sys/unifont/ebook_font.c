#include "stdio.h"
#include "string.h"

#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "task/app.h"
#include "ebook_font.h"

#define deg_Printf printf

#define FONT_UNKNOWN_WIDTH 16


/**
 * �����ļ����岿��
 *1. �ַ���ʾռ�õĸ߶�
 *    ��(2byte)��ʾ
 *2. ���������ַ�unicode��Χ����
 *    ��(2byte)��ʾ
 *3. �ַ���Χ��Ϣ
 *    ��ʼ�ַ�(2byte) + �ַ�����(2byte) + ����ƫ��ֵ���(2byte)
 *4. �ַ�����ƫ��ֵ
 *    �ַ����(1byte) + �ַ��ĵ���ƫ��ֵ(3byte)
 *5. �ַ�������Ϣ
 *
 */

typedef struct {
	ECHAR first;
	short num;
	short order;
}EBOOK_FONT_SCOPE_T;

typedef struct {
	int   fd;
	short font_h;
	short scope_num;
	EBOOK_FONT_SCOPE_T *scope_tab;
	EADDR *off_tab;
	EADDR pixel_start;
}EBOOK_FONT_INFO_T;

//static EBOOK_FONT_INFO_T gEbookFontInfo = {0};

EFONT_HANDLE EbookFont_Init(int font_id)
{
	EBOOK_FONT_INFO_T *font_ptr = NULL;
	int addr, offset;
	int size, i;
	EBOOL ret = EFALSE;

	// uninit old
	deg_Printf("[%s]\n",__func__);

	do {
		deg_Printf("FONT: struct size=%d\n",sizeof(EBOOK_FONT_INFO_T));
		font_ptr = osal_malloc(sizeof(EBOOK_FONT_INFO_T));//hal_sysMemMalloc(sizeof(EBOOK_FONT_INFO_T), 64);
		if (font_ptr == NULL) {
			deg_Printf("FONT: malloc struct fail\n");
			break;
		}
		// open file
		addr =  res_open(font_id);
		if(addr < 0) {
			deg_Printf("FONT: open %d fail\n",font_id);
			break;
		}
		font_ptr->fd = addr;
		offset = 0;

		// read font height
		size = sizeof(short);
		res_read(addr + offset, &(font_ptr->font_h), size);
		offset += size;

		// read scope num
		size = sizeof(short);
		res_read(addr + offset, &(font_ptr->scope_num), size);
		offset += size;

		// read scopes
		size = font_ptr->scope_num * sizeof(EBOOK_FONT_SCOPE_T);
		deg_Printf("FONT: scope_tab size=%d, scope_num:%d, %d\n",size, font_ptr->scope_num, sizeof(EBOOK_FONT_SCOPE_T));
		font_ptr->scope_tab = (EBOOK_FONT_SCOPE_T *)osal_malloc(size);//(EBOOK_FONT_SCOPE_T *)hal_sysMemMalloc(size,64);
		if(font_ptr->scope_tab == NULL) {
			deg_Printf("FONT: malloc scope_tab fail\n");
			break;
		}
		res_read(addr + offset, font_ptr->scope_tab, size);
		offset += size;

		// read offset table
		size = 0;
		for (i = 0; i < font_ptr->scope_num; i++) {
			size += sizeof(EADDR) * font_ptr->scope_tab[i].num;
			//printf("scope[%d] num:%d\n", i, font_ptr->scope_tab[i].num);
		}
		deg_Printf("FONT: off_tab size=%d\n",size);
		font_ptr->off_tab = (EADDR *)osal_malloc(size);//hal_sysMemMalloc(size,64);
		if(font_ptr->off_tab == NULL) {
			deg_Printf("FONT: malloc off_tab fail\n");
			break;
		}
		//deg_Printf("===font_ptr:%p, scope_tab:%p, off_tab:%p, font_ptr->scope_num:%d\n", font_ptr, font_ptr->scope_tab, font_ptr->off_tab, font_ptr->scope_num);
		res_read(addr + offset, font_ptr->off_tab, size);
		offset += size;

		// pixel start
		font_ptr->pixel_start = offset;

		ret = ETRUE;
	} while (0);
	if (ret == EFALSE) {
		EbookFont_Uninit(font_ptr);
		font_ptr = NULL;
	}

	return (EFONT_HANDLE)font_ptr;
}

EBOOL EbookFont_Uninit(EFONT_HANDLE hFont)
{
	EBOOK_FONT_INFO_T *font_ptr = (EBOOK_FONT_INFO_T *)hFont;

	if (font_ptr == NULL) {
		return EFALSE;
	}

	if (font_ptr->scope_tab) {
		osal_free(font_ptr->scope_tab);
		font_ptr->scope_tab = NULL;
	}
	if (font_ptr->off_tab) {
		osal_free(font_ptr->off_tab);
		font_ptr->off_tab = NULL;
	}
	osal_free(font_ptr);

	return ETRUE;
}

int EbookFont_GetLineHeight(EFONT_HANDLE hFont)
{
	EBOOK_FONT_INFO_T *font_ptr = (EBOOK_FONT_INFO_T *)hFont;

	if (font_ptr == NULL) {
		return 0;
	}

	return font_ptr->font_h;
}

int EbookFont_GetCharWidth(EFONT_HANDLE hFont, ECHAR ch)
{
	EBOOK_FONT_INFO_T *font_ptr = (EBOOK_FONT_INFO_T *)hFont;
	EADDR offset;
	int i;

	if (font_ptr == NULL) {
		return 0;
	}
	
	if(ch<0x20)
	{
		return 0;
	}

	for (i = 0; i < font_ptr->scope_num; i++) {
		if (ch >= font_ptr->scope_tab[i].first && ch < font_ptr->scope_tab[i].first + font_ptr->scope_tab[i].num) {
			break;
		}
	}
	if (i >= font_ptr->scope_num) {
		printf("==unknow char:0x%x\n", ch);
		return FONT_UNKNOWN_WIDTH;
	}
	offset = font_ptr->scope_tab[i].order + (ch - font_ptr->scope_tab[i].first);
	offset = font_ptr->off_tab[offset];

	return (offset>>24)&0xFF;
}

int EbookFont_GetStrSize(EFONT_HANDLE hFont, const ECHAR *str, EU16 *width, EU16 *heigth, int hgap)
{
	EBOOK_FONT_INFO_T *font_ptr = (EBOOK_FONT_INFO_T *)hFont;
	int num = 0;
	int total_w = 0;

	if (font_ptr == NULL || str == NULL) {
		return 0;
	}

	while (str[num]) {
		total_w += EbookFont_GetCharWidth(hFont,str[num]) + hgap;
		num++;
	}
	if (total_w > 0) {
		total_w -= hgap;
	}
	if (width != NULL) {
		*width = total_w;
	}
	if (heigth != NULL) {
		*heigth = EbookFont_GetLineHeight(hFont);
	}

	return num;
}

EBOOL EbookFont_GetCharPixels(EFONT_HANDLE hFont, ECHAR ch, EBOOK_FONT_PIXEL_T *pixel_info)
{
	EBOOK_FONT_INFO_T *font_ptr = (EBOOK_FONT_INFO_T *)hFont;
	unsigned char buf[4];
	EADDR offset;
	int i;

	// check validity
	if (pixel_info == NULL) {
		return EFALSE;
	}
	if (font_ptr == NULL) {
		return EFALSE;
	}
	
	if(ch<0x20)
	{
		return EFALSE;
	}

	// find char
	for (i = 0; i < font_ptr->scope_num; i++) {
		if (ch >= font_ptr->scope_tab[i].first && ch < font_ptr->scope_tab[i].first + font_ptr->scope_tab[i].num) {
			break;
		}
	}
	if (i >= font_ptr->scope_num) {
		unsigned char *p;
		unsigned char end;
		// unknown char, display as a rectangle
		if (font_ptr->font_h > FONT_UNKNOWN_WIDTH) {
			pixel_info->startline = (font_ptr->font_h-FONT_UNKNOWN_WIDTH)/2;
			pixel_info->lines = FONT_UNKNOWN_WIDTH;
		} else {
			pixel_info->startline = 0;
			pixel_info->lines = font_ptr->font_h;
		}
		pixel_info->width = FONT_UNKNOWN_WIDTH;
		pixel_info->stride = (pixel_info->width + 7) / 8;
		// first line
		p = pixel_info->pixels;
		memset(p, 0xFF, pixel_info->stride);
		// mid line
		p += pixel_info->stride;
		end = (pixel_info->width % 8);
		end = 0x01 << ((8-end)%8);
		for (i = 1; i < pixel_info->lines - 1; i++) {
			memset(p, 0, pixel_info->stride);
			p[0] |= 0x80;
			p[pixel_info->stride - 1] |= end; 
			p += pixel_info->stride;
		}
		// last line
//		p = pixel_info->pixels + (pixel_info->lines - 1)*pixel_info->stride;
		memset(p, 0xFF, pixel_info->stride);

		return ETRUE;
	}

	// offset
	offset = font_ptr->scope_tab[i].order + (ch - font_ptr->scope_tab[i].first);
	offset = font_ptr->off_tab[offset];
	pixel_info->width = (offset>>24)&0xFF;
	offset &= 0x00FFFFFF;

	// pixel
	offset += font_ptr->pixel_start;
	res_read(font_ptr->fd + offset, buf, 3);
	pixel_info->startline = buf[0];
	pixel_info->lines = buf[1];
	pixel_info->stride = buf[2];
	if (pixel_info->lines == 0) {
		memset(pixel_info->pixels, 0 ,sizeof(pixel_info->pixels));
	} else {
		offset += 3;
		res_read(font_ptr->fd + offset, pixel_info->pixels, pixel_info->lines * pixel_info->stride);
	}

	return ETRUE;
}


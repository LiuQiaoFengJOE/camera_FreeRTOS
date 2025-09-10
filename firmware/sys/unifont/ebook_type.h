#ifndef __EBOOK_TYPE__
#define __EBOOK_TYPE__

#define ETRUE  1
#define EFALSE 0

typedef unsigned char  EBOOL;
typedef unsigned char  EBYTE;
typedef unsigned short EU16;
typedef unsigned short ECHAR;
typedef unsigned int   EU32;
typedef unsigned long  EADDR;

typedef unsigned char  ECOLOR;

typedef enum {
	EFONT_0 = 0,
//	EFONT_1,

	EFONT_MAX
}EFONT_CLASS;

typedef enum {
	LCD_DIR_0,
	LCD_DIR_90,
	LCD_DIR_180,
	LCD_DIR_270,
}LCD_DIR_E;

typedef struct {
	ECOLOR *buffer;
	EU16  width;
	EU16  height;
	LCD_DIR_E dir;
}EBOOK_LCD_T;

typedef struct {
	EU16 x0;
	EU16 y0;
	EU16 x1;
	EU16 y1;
}EBOOK_RECT_T;

typedef struct {
	ECHAR *str;
	EU16 len;
}EBOOK_STR_T;

typedef struct {
	EFONT_CLASS font;
	EBOOK_RECT_T rect; // display area
	ECOLOR strColor;   // stroke color
	ECOLOR bgColor; // stroke color
	EBYTE hgap; // word gap
	EBYTE vgap; // line gap
}EBOOK_DISP_STYLE_T;

// �ٶ�����ַ���40x40����
#define MAX_PIXEL_BYTE (40*40/8)
typedef struct {
	unsigned char startline;   // �ӵڼ�����ʾ
	unsigned char lines;       // ����
	unsigned char stride;      // ÿ���ֽ���
	unsigned char width;       // ÿ�е�����
	unsigned char pixels[MAX_PIXEL_BYTE]; // ������Ϣ,��stride * lines �ֽ���Ч����
}EBOOK_FONT_PIXEL_T;


#endif // __EBOOK_TYPE__


#include "../app.h"
#if GAME_FUNC_EN
#include "task/application.h"
#include "funcGame.h"
#include "task/uiWin.h"

#define U 1
#define D 2
#define L 3 
#define R 4



// 7ÖÖ·½¿éµÄ4Ðý×ª×´Ì¬£¨4Î»ÎªÒ»ÐÐ£©
static INT16U gs_uTetrisTable[7][4] = { 
		{ 0x00F0U, 0x2222U, 0x00F0U, 0x2222U },  // IÐÍ
		{ 0x0072U, 0x0262U, 0x0270U, 0x0232U },  // TÐÍ
		{ 0x0223U, 0x0074U, 0x0622U, 0x0170U },  // LÐÍ
		{ 0x0226U, 0x0470U, 0x0322U, 0x0071U },  // JÐÍ
		{ 0x0063U, 0x0264U, 0x0063U, 0x0264U },  // ZÐÍ
		{ 0x006CU, 0x0462U, 0x006CU, 0x0462U },  // SÐÍ
		{ 0x0660U, 0x0660U, 0x0660U, 0x0660U }   // OÐÍ
};
//static INT16U gs_uTetrisTable_default[4] = {0};
//static INT16U save_flag = 0;
// =============================================================================
// ³õÊ¼×´Ì¬µÄÓÎÏ·³Ø
// Ã¿¸öÔªËØ±íÊ¾ÓÎÏ·³ØµÄÒ»ÐÐ£¬ÏÂ±ê´óµÄÊÇÓÎÏ·³Øµ×²¿
// Á½¶Ë¸÷ÖÃ2¸ö1£¬µ×²¿2È«ÖÃÎª1£¬±ãÓÚ½øÐÐÅö×²¼ì²â
// ÕâÑùÒ»À´ÓÎÏ·³ØµÄ¿í¶ÈÎª12ÁÐ
// Èç¹ûÏëÒª´«Í³µÄ10ÁÐ£¬Ö»Ðè¶àÌîÁ½¸ö1¼´¿É£¨0xE007£©£¬µ±È»ÏÔÊ¾Ïà¹Ø²¿·ÖÒ²ÒªËæÖ®¸Ä¶¯
// µ±Ä³¸öÔªËØÎª0xFFFFUÊ±£¬ËµÃ÷¸ÃÐÐÒÑ±»ÌîÂú
// ¶¥²¿4ÐÐÓÃÓÚ¸ø·½¿é£¬²»ÏÔÊ¾³öÀ´
// ÔÙ³ýÈ¥µ×²¿2ÐÐ£¬ÏÔÊ¾³öÀ´µÄÓÎÏ·³Ø¸ß¶ÈÎª22ÐÐ
static const INT16U gs_uInitialTetrisPool[22] = {0xC003,0xC003, 0xC003,
		0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003,
		0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003,
		0xC003, 0xFFFF, 0xFFFF };
		
#define COL_BEGIN 2
#define COL_END 14
#define ROW_BEGIN 4
#define ROW_END 20

#define RECT_W 14
#define RECT_H 14

// =============================================================================
typedef struct TetrisManager  // Õâ¸ö½á¹¹Ìå´æ´¢ÓÎÏ·Ïà¹ØÊý¾Ý
{
	uint16_t pool[22];  // ÓÎÏ·³Ø
	int8_t x;  // µ±Ç°·½¿éx×ø±ê£¬´Ë´¦×ø±êÎª·½¿é×óÉÏ½Ç×ø±ê
	int8_t y;  // µ±Ç°·½¿éy×ø±ê
	int8_t type[3];  // µ±Ç°¡¢ÏÂÒ»¸öºÍÏÂÏÂÒ»¸ö·½¿éÀàÐÍ
	int8_t orientation[3];  // µ±Ç°¡¢ÏÂÒ»¸öºÍÏÂÏÂÒ»¸ö·½¿éÐý×ª×´Ì¬
	unsigned score;  // µÃ·Ö
	unsigned erasedCount[4];  // ÏûÐÐÊý
	unsigned erasedTotal;  // ÏûÐÐ×ÜÊý
	unsigned tetrisCount[7];  // ¸÷·½¿éÊý
	unsigned tetrisTotal;  // ·½¿é×ÜÊý
	bool dead;  // ¹Ò
} TetrisManager;

// =============================================================================
typedef struct TetrisControl  // Õâ¸ö½á¹¹Ìå´æ´¢¿ØÖÆÏà¹ØÊý¾Ý
{
	bool pause;  // ÔÝÍ£
	bool clockwise;  // Ðý×ª·½Ïò£ºË³Ê±ÕëÎªtrue
	int8_t direction;  // ÒÆ¶¯·½Ïò£º0Ïò×óÒÆ¶¯ 1ÏòÓÒÒÆ¶¯
	// ÓÎÏ·³ØÄÚÃ¿¸ñµÄÑÕÉ«
	// ÓÉÓÚ´Ë°æ±¾ÊÇ²ÊÉ«µÄ£¬½öÓÃÓÎÏ·³ØÊý¾ÝÎÞ·¨´æ´¢ÑÕÉ«ÐÅÏ¢
	// µ±È»£¬Èç¹ûÖ»ÊµÏÖµ¥É«°æµÄ£¬¾ÍÃ»±ØÒªÓÃÕâ¸öÊý×éÁË
	uint16_t color[22][16];
} TetrisControl;


static TetrisManager tetrisManager;
static TetrisControl tetrisControl;

static u8 *tetris_icon = 0;


// =============================================================================
// ²åÈë·½¿é
void insertTetris(TetrisManager *manager) {
	// µ±Ç°·½¿é
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	// µ±Ç°·½¿éÃ¿4Î»È¡³ö£¬Î»»òµ½ÓÎÏ·³ØÏàÓ¦Î»ÖÃ£¬¼´Íê³É²åÈë·½¿é
	manager->pool[manager->y + 0] |= (((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] |= (((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] |= (((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] |= (((tetris >> 0xC) & 0x000F) << manager->x);
}
bool checkCollision_L(uint16_t tetris)
{
	uint16_t dest = 0;
	
	dest |= ((tetris >> 0x0) & 0x0008);
	dest |= ((tetris >> 0x4) & 0x0008);
	dest |= ((tetris >> 0x8) & 0x0008);
	dest |= ((tetris >> 0xC) & 0x0008);
	
	return dest;
}
bool checkCollision_R(uint16_t tetris)
{
	uint16_t dest = 0;
	
	dest |= ((tetris >> 0x0) & 0x0001);
	dest |= ((tetris >> 0x4) & 0x0001);
	dest |= ((tetris >> 0x8) & 0x0001);
	dest |= ((tetris >> 0xC) & 0x0001);
	
	return dest;
}

// =============================================================================
// Åö×²¼ì²â
bool checkCollision(const TetrisManager *manager) {
	// µ±Ç°·½¿é
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];
	uint16_t dest = 0;
	
	
	// »ñÈ¡µ±Ç°·½¿éÔÚÓÎÏ·³ØÖÐµÄÇøÓò£º
	// ÓÎÏ·³Ø×ø±êx y´¦Ð¡·½¸ñÐÅÏ¢£¬°´µÍµ½¸ß´æ·ÅÔÚ16Î»ÎÞ·ûºÅÊýÖÐ
	dest |= (((manager->pool[manager->y + 0] >> manager->x) << 0x0) & 0x000F);
	dest |= (((manager->pool[manager->y + 1] >> manager->x) << 0x4) & 0x00F0);
	dest |= (((manager->pool[manager->y + 2] >> manager->x) << 0x8) & 0x0F00);
	dest |= (((manager->pool[manager->y + 3] >> manager->x) << 0xC) & 0xF000);

	// Èôµ±Ç°·½¿éÓëÄ¿±êÇøÓò´æÔÚÖØµþ£¨Åö×²£©£¬ÔòÎ»ÓëµÄ½á¹û²»Îª0
	return ((dest & tetris) != 0);
}
void show_one_diamonds(int8_t x,int8_t y,const uint16_t color)
{
	u8 * p;
	DISPLAY_ICONSHOW icon;
	
	p = (u8 *)setting_frame_buff;
	
	icon.icon_w = RECT_W;
	icon.icon_h = RECT_H;
	icon.transparent = TRANSPARENT_COLOR;
	icon.pos_x = x*RECT_W + 8;
	icon.pos_y = y*RECT_H + 8;
	
	frame_icon_draw(p, tetris_icon, &icon, /*NULL*/0);
}

/*******************************************************************************
* Function Name  : R_drawAsciiChar
* Description    : draw ascii char   
* Input          :  unsigned short *obuff : outbuff
                       char c : string
                       int x : draw x
                       int y : draw y
                       unsigned short width : buffer width
                       unsigned short color : color
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
int R_drawAsciiChar(unsigned short *obuff,char c,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font)
{
	unsigned char w,h,temp,mask,panel;
	int i,j;
	color_t *line,*outbuff;
	const unsigned char *table;

	table = ascii_draw_get(c,&w,&h,font);
	if(table == ((void *)0))
		return -1;

    panel = font&R_FONT_PAN;
    outbuff = (color_t *)obuff;
	line = outbuff+(x)+((y*width));

	for(i=0;i<h;i++)
	{
	  for(j=0;j<(w+7)/8;j++)
	  {

	   temp = *table++;

			 for(mask=0;mask<8;mask++)
			 {
				 
				 if(temp&(0x80>>mask))
				 {
					 *line++ = (color_t)color;
					 
				 }
				 else if(panel)
				 {
					 *line++ = fcolor;
				 }
				 else
				 {
				 	line++;
				 }

			 }
		 }
		 y++;
		 line =outbuff+(x)+((y*width));
		 dcache_flush_region((u32)outbuff+(x)+(((y-1)*width)),(w+7)/8);
	}

	return ((h<<8)|w);		
}


extern INT16U score[3];
// =============================================================================
// ÏÔÊ¾µÃ·ÖÐÅÏ¢
void printScore(const TetrisManager *manager) 
{
	int8_t i;
	INT16U pos_x_shift = 236;
	INT32U buf_show = setting_frame_buff;
	
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/10000,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/1000%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/100%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/10%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
}

// =============================================================================
// ÏÔÊ¾ÏÂÒ»¸ö
void printNextTetris(const TetrisManager *manager) {
	int8_t i,x,y;
	uint16_t tetris;

	// ÏÂÒ»¸ö£¬ÓÃÏàÓ¦ÑÕÉ«ÏÔÊ¾
	tetris = gs_uTetrisTable[manager->type[1]][manager->orientation[1]];

	for (i = 0; i < 16; ++i) {
		y = (i >> 2) + 11;  // ´ýÉèÖÃµÄÐÐ
		if (y > ROW_END)  // ³¬¹ýµ×²¿ÏÞÖÆ
		{
			break;
		}
		x = (i & 3) + 16;  // ´ýÉèÖÃµÄÁÐ
		if ((tetris >> i) & 1)  // ¼ì²âµÄµ½Ð¡·½¸ñÊôÓÚµ±Ç°·½¿éÇøÓò
		{
			show_one_diamonds(x,y,0xF7A1);
		}
	}
}

// =============================================================================
// ¸øÒ»¸ö·½¿é
void giveTetris(TetrisManager *manager) {
	uint16_t tetris;

	manager->type[0] = manager->type[1];  // ÏÂÒ»¸ö·½¿éÖÃÎªµ±Ç°
	manager->orientation[0] = manager->orientation[1];

	manager->type[1] = rand_int() % 7;  // Ëæ»úÉú³ÉÏÂÒ»¸ö·½¿é
	manager->orientation[1] = rand_int() & 3;

	tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]]; // µ±Ç°·½¿é
	// ÉèÖÃµ±Ç°·½¿éy×ø±ê£¬±£Ö¤¸Õ¸ø³öÊ±Ö»ÏÔÊ¾·½¿é×îÏÂÃæÒ»ÐÐ
	// ÕâÖÖÊµÏÖÊ¹µÃÍæ¼Ò¿ÉÒÔÒÔºÜ¿ìµÄËÙ¶È½«·½¿éÂäÔÚ²»ÏÔÊ¾³öÀ´µÄ¶¥²¿4ÐÐÄÚ
	if (tetris & 0xF000) {
		manager->y = 0;
	} else {
		manager->y = (tetris & 0xFF00) ? 1 : 2;
	}
	manager->x = 6;  // ÉèÖÃµ±Ç°·½¿éx×ø±ê
	
	if (checkCollision(manager))  // ¼ì²âµ½Åö×²
	{
		manager->dead = 1;  // ±ê¼ÇÓÎÏ·½áÊø
	}
	else  // Î´¼ì²âµ½Åö×²
	{
		insertTetris(manager);  // ½«µ±Ç°·½¿é¼ÓÈëÓÎÏ·³Ø
	}
}


// =============================================================================
// ÒÆ³ý·½¿é
void removeTetris(TetrisManager *manager) {
	// µ±Ç°·½¿é
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	// µ±Ç°·½¿éÃ¿4Î»È¡³ö£¬°´Î»È¡·´ºóÎ»Óëµ½ÓÎÏ·³ØÏàÓ¦Î»ÖÃ£¬¼´Íê³ÉÒÆ³ý·½¿é
	manager->pool[manager->y + 0] &=
			~(((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] &=
			~(((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] &=
			~(((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] &=
			~(((tetris >> 0xC) & 0x000F) << manager->x);
}

// =============================================================================
// ÏÔÊ¾µ±Ç°·½¿é
void printCurrentTetris(const TetrisManager *manager,const TetrisControl *control) {
	int8_t x, y;
//	int8_t i = 0, j= 0;

	// ÏÔÊ¾µ±Ç°·½¿éÊÇÔÚÒÆ¶¯ºóµ÷ÓÃµÄ£¬Îª²ÁÈ¥ÒÆ¶¯Ç°µÄ·½¿é£¬ÐèÒªÀ©Õ¹ÏÔÊ¾ÇøÓò
	// ÓÉÓÚ²»¿ÉÄÜÏòÉÏÒÆ¶¯£¬¹Ê²»ÐèÒªÏòÏÂÀ©Õ¹
	y = (manager->y > ROW_BEGIN) ? (manager->y - 1) : ROW_BEGIN;  // ÏòÉÏÀ©Õ¹Ò»¸ñ
	for (; y < ROW_END && y < manager->y + 4; ++y) {
		x = (manager->x > COL_BEGIN) ? (manager->x - 1) : COL_BEGIN;  // Ïò×óÀ©Õ¹Ò»¸ñ
		for (; x < COL_END && x < manager->x + 5; ++x)  // ÏòÓÒÀ©Õ¹Ò»¸ñ
		{
			if ((manager->pool[y] >> x) & 1)  // ÓÎÏ·³Ø¸Ã·½¸ñÓÐ·½¿é
			{
				// ÓÃÏàÓ¦ÑÕÉ«£¬ÏÔÊ¾Ò»¸öÊµÐÄ·½¿é
				show_one_diamonds( x-2, y-ROW_BEGIN, control->color[y][x]);
			} 
			else
			{
				//show_one_diamonds( x, y-ROW_BEGIN, 0x8c71);
			}
		}
	}
}
// =============================================================================
// ÉèÖÃÑÕÉ«
void setPoolColor(const TetrisManager *manager, TetrisControl *control) {
	// ÓÉÓÚÏÔÊ¾ÓÎÏ·³ØÊ±£¬ÏÈÒªÔÚÓÎÏ·³ØÀïÅÐ¶ÏÄ³Ò»·½¸ñÓÐ·½¿é²ÅÏÔÊ¾ÏàÓ¦·½¸ñµÄÑÕÉ«
	// ÕâÀïÖ»×÷ÉèÖÃ¼´¿É£¬Ã»±ØÒªÇå³ý
	// µ±ÒÆ¶¯·½¿é»ò¸øÒ»¸ö·½¿éÊ±µ÷ÓÃ

	int8_t i, x, y;

	// µ±Ç°·½¿é
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	for (i = 0; i < 16; ++i) {
		y = (i >> 2) + manager->y;  // ´ýÉèÖÃµÄÐÐ
		if (y > ROW_END)  // ³¬¹ýµ×²¿ÏÞÖÆ
		{
			break;
		}
		x = (i & 3) + manager->x;  // ´ýÉèÖÃµÄÁÐ
		if ((tetris >> i) & 1)  // ¼ì²âµÄµ½Ð¡·½¸ñÊôÓÚµ±Ç°·½¿éÇøÓò
		{
			control->color[y][x] = 0xF7A1;  // ÉèÖÃÑÕÉ«
		}
	}
}

// =============================================================================
// Ðý×ª·½¿é
void rotateTetris(TetrisManager *manager, TetrisControl *control) {
	int8_t ori = manager->orientation[0];  // ¼ÇÂ¼Ô­Ðý×ª×´Ì¬

	removeTetris(manager);  // ÒÆ×ßµ±Ç°·½¿é

	// Ë³/ÄæÊ±ÕëÐý×ª
	manager->orientation[0] =
			(control->clockwise) ? ((ori + 1) & 3) : ((ori + 3) & 3);

	if (checkCollision(manager))  // ¼ì²âµ½Åö×²
	{
		manager->orientation[0] = ori;  // »Ö¸´ÎªÔ­Ðý×ª×´Ì¬
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é¡£ÓÉÓÚ×´Ì¬Ã»¸Ä±ä£¬²»ÐèÒªÉèÖÃÑÕÉ«
	} else {
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é
		setPoolColor(manager, control);  // ÉèÖÃÑÕÉ«
		printCurrentTetris(manager, control);  // ÏÔÊ¾µ±Ç°·½¿é
	}
}



// ÏûÐÐ¼ì²â
bool checkErasing(TetrisManager *manager, TetrisControl *control) {
	static const unsigned scores[5] = { 0, 50, 150, 300, 400};  // ÏûÐÐµÃ·Ö
	int8_t count = 0;
	int8_t k = 0, y = manager->y + 3;  // ´ÓÏÂÍùÉÏ¼ì²â

	do {
		if (y < ROW_END && manager->pool[y] == 0xFFFFU)  // ÓÐÐ§ÇøÓòÄÚÇÒÒ»ÐÐÒÑÌîÂú
				{
			++count;
			// Ïû³ýÒ»ÐÐ·½¿é
			memmove(manager->pool + 1, manager->pool, sizeof(uint16_t) * y);
			// ÑÕÉ«Êý×éµÄÔªËØËæÖ®ÒÆ¶¯
			memmove(control->color[1], control->color[0],
					sizeof(int8_t[16]) * y);
		} else {
			--y;
			++k;
		}
	} while (y >= manager->y && k < 4);

	manager->erasedTotal += count;  // ÏûÐÐ×ÜÊý
	manager->score += scores[count];  // µÃ·Ö
	//game_gradr += scores[count]; 

	if (count > 0) {
		++manager->erasedCount[count - 1];  // ÏûÐÐ
	}

	giveTetris(manager);  // ¸øÏÂÒ»¸ö·½¿é
	setPoolColor(manager, control);  // ÉèÖÃÑÕÉ«

	return (count > 0);
}


// =============================================================================
// ÏÔÊ¾ÓÎÏ·³Ø
void printTetrisPool(const TetrisManager *manager, const TetrisControl *control) {
	int8_t x, y;

	for (y = ROW_BEGIN; y < ROW_END; ++y)  // ²»ÏÔÊ¾¶¥²¿4ÐÐºÍµ×²¿2ÐÐ
	{
		// ¶¨µãµ½ÓÎÏ·³ØÖÐµÄ·½¸ñ
		for (x = COL_BEGIN; x < COL_END; ++x)  // ²»ÏÔÊ¾×óÓÒ±ß½ç
		{
			if ((manager->pool[y] >> x) & 1)  // ÓÎÏ·³Ø¸Ã·½¸ñÓÐ·½¿é
			{
				// ÓÃÏàÓ¦ÑÕÉ«£¬ÏÔÊ¾Ò»¸öÊµÐÄ·½¿é
				show_one_diamonds( x-2, y-ROW_BEGIN, control->color[y][x]);
			} else  // Ã»ÓÐ·½¿é£¬ÏÔÊ¾¿Õ°×
			{
				//show_one_diamonds( x, y-ROW_BEGIN, 0x8c71);
			}
		}
	}
	printCurrentTetris(manager, control);  // ÏÔÊ¾µ±Ç°·½¿é
	printNextTetris(manager);  // ÏÔÊ¾ÏÂÒ»¸ö·½¿é
	printScore(manager);  // ÏÔÊ¾µÃ·ÖÐÅÏ¢
}


// =============================================================================
// ÏòÏÂÒÆ¶¯·½¿é
void moveDownTetris(TetrisManager *manager, TetrisControl *control) 
{
	int8_t y = manager->y;  // ¼ÇÂ¼Ô­ÐÐÎ»ÖÃ

	removeTetris(manager);  // ÒÆ×ßµ±Ç°·½¿é
	++manager->y;  // ÏòÏÂÒÆ¶¯

	if (checkCollision(manager))  // ¼ì²âµ½Åö×²
	{
		manager->y = y;  // »Ö¸´ÎªÔ­ÐÐÎ»ÖÃ
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é¡£ÓÉÓÚÎ»ÖÃÃ»¸Ä±ä£¬²»ÐèÒªÉèÖÃÑÕÉ«
		if (checkErasing(manager, control))  // ¼ì²âµ½ÏûÐÐ
		{
			printTetrisPool(manager, control);  // ÏÔÊ¾ÓÎÏ·³Ø
		}
		//ap_setting_memcpy(buf_show,buf_base,getDispDevBufSize());
	}
	else 
	{
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é
		setPoolColor(manager, control);  // ÉèÖÃÑÕÉ«
		//printTetrisPool(manager, control);  // ÏÔÊ¾ÓÎÏ·³Ø
		printCurrentTetris(manager, control);  // ÏÔÊ¾µ±Ç°·½¿é
	}
}
// =============================================================================
// Ë®Æ½ÒÆ¶¯·½¿é
void horzMoveTetris(TetrisManager *manager, TetrisControl *control) {
	int x = manager->x;  // ¼ÇÂ¼Ô­ÁÐÎ»ÖÃ
	
	removeTetris(manager);  // ÒÆ×ßµ±Ç°·½¿é
	control->direction == 0 ? (--manager->x) : (++manager->x);  // ×ó/ÓÒÒÆ¶¯

	if (checkCollision(manager))  // ¼ì²âµ½Åö×²
	{
		manager->x = x;  // »Ö¸´ÎªÔ­ÁÐÎ»ÖÃ
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é¡£ÓÉÓÚÎ»ÖÃÃ»¸Ä±ä£¬²»ÐèÒªÉèÖÃÑÕÉ«
	} else {
		insertTetris(manager);  // ·ÅÈëµ±Ç°·½¿é
		setPoolColor(manager, control);  // ÉèÖÃÑÕÉ«
		printCurrentTetris(manager, control);  // ÏÔÊ¾µ±Ç°·½¿é
	}
}

// =============================================================================
// ¼ü°´ÏÂ
void keydownControl(TetrisManager *manager, TetrisControl *control, int key) 
{
//	INT16U tetris;
//	tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]]; // µ±Ç°·½¿é
	setting_frame_buff = get_game_show_buf(TRUE);

	switch (key) {
		case U:  // ÉÏ
			control->clockwise = 1;  // Ë³Ê±ÕëÐý×ª
			rotateTetris(manager, control);  // Ðý×ª·½¿é
			break;
		
		case L:  // ×ó
			control->direction = 1;  // Ïò×óÒÆ¶¯
			horzMoveTetris(manager, control);  // Ë®Æ½ÒÆ¶¯·½¿é
			break;
		
		case R:  // ÓÒ
			control->direction = 0;  // ÏòÓÒÒÆ¶¯
			horzMoveTetris(manager, control);  // Ë®Æ½ÒÆ¶¯·½¿é
			break;
		
		case D:  // ÏÂ
			moveDownTetris(manager, control);  // ÏòÏÂÒÆ¶¯·½¿é
			break;
		
		default:
			break;
	}
	
	printTetrisPool(manager, control);//ÏÔÊ¾ÓÎÏ·³Ø
	frame_buff_display();
}

//½áÊøÓÎÏ·ÅÐ¶Ï
INT8U endgame(TetrisManager *manager)
{
	if(manager->dead == 1)
		return 0;
	return 1;
}
// ÖØÐÂ¿ªÊ¼ÓÎÏ·
void restartGame(TetrisManager *manager, TetrisControl *control) 
{
	//INT8U i;
	memset(manager, 0, sizeof(TetrisManager));  // È«²¿ÖÃ0
	//gp_memset((INT8S *) manager, 0, sizeof(TetrisManager));
	// ³õÊ¼»¯ÓÎÏ·³Ø
	memcpy(manager->pool, gs_uInitialTetrisPool, sizeof(uint16_t[22]));
	
	manager->type[1] = rand_int() % 7;  // ÏÂÒ»¸ö
	manager->orientation[1] = rand_int() & 3;

	memset(control, 0, sizeof(TetrisControl));  // È«²¿ÖÃ0

	giveTetris(manager);  // ¸øÏÂÒ»¸ö·½¿é
	setPoolColor(manager, control);  // ÉèÖÃÑÕÉ«
}


// ³õÊ¼»¯ÓÎÏ·
void game_tetris_init(TetrisManager *manager, TetrisControl *control)
{ 
	tetris_icon = get_bin_data(RES_GAME_TETRIS_ICON);
	game_bk_draw((u8 *)get_game_base_buf(), RES_GAME_TETRIS_BK, /*NULL*/0);
	get_game_show_buf(TRUE);
	frame_buff_display();
	restartGame(manager, control); 
	sleeptime = 500;
	endGamestatus = 0;
}



void game_tetris_exit()
{
	endGamestatus = 1;
	free_buf(tetris_icon);
}


void game_tetris_entry(void)
{
	u8 msg, err;
	game_tetris_init(&tetrisManager, &tetrisControl);
	while(1){
		
		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_UP:
				keydownControl(&tetrisManager, &tetrisControl,U);
				break;
			case KEY_EVENT_DOWN:
				keydownControl(&tetrisManager, &tetrisControl,D);
				break;
			case KEY_EVENT_RIGHT:
				keydownControl(&tetrisManager, &tetrisControl,L);
				break;
			case KEY_EVENT_LEFT:
				keydownControl(&tetrisManager, &tetrisControl,R);
				break;
			case KEY_EVENT_POWER:
			case KEY_EVENT_TOUCH_BACK:		
				game_tetris_exit();
				return;
			case SYS_EVENT_GAME_TICK:
				if(endgame(&tetrisManager))
					keydownControl(&tetrisManager, &tetrisControl,D);
				else
					XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_POWER,0));
				break;
		}
	}
}

#endif
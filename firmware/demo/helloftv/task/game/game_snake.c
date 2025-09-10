#include "../app.h"
#if GAME_FUNC_EN
#include "task/application.h"
#include "task/uiWin.h"

#include "funcGame.h"



#define u8 INT8U  
#define u16 INT16U
#if 1
#define icon_body		    0 	
#define icon_food		    196
#define icon_heart_D		392
#define icon_heart_L	    588
#define icon_heart_R		784
#define icon_heart_U	    980
#else
#define icon_body		0
#define icon_food		294
#define icon_heart_D		588
#define icon_heart_L		882
#define icon_heart_R		1176
#define icon_heart_U		1470
#endif
static u8 *tc_icon = 0; //,*game_icon = 0;


/*********************************************************/
typedef struct SNAKE //ÉßÉíµÄÒ»¸ö½Úµã
{
	int x;
	int y;
	struct SNAKE *next;
}snake;
#define SNAKE_NOP_COLOR   0
#define SNAKE_HEART_COLOR 1
#define SNAKE_BODY_COLOR  2
#define SNAKE_FOOD_COLOR  3
#define SNAKE_WALL_COLOR  4

#define U 1
#define D 2
#define L 3 
#define R 4       //ÉßµÄ×´Ì¬£¬U£ºÉÏ £»D£ºÏÂ£»L:×ó R£ºÓÒ

u8 color_palette[] = {
	/*NULL*/0,
	R_COLOR_GREEN,
	R_COLOR_YELLOW,
	R_COLOR_BLACK,
	R_COLOR_WHITE
};



u8 biteSelf();
void free_one_snake(snake *p);
void snakeMove();
void creatMap();
void * get_malloc(u16 size);
u16 rand_int();
snake *head, *food;//ÉßÍ·Ö¸Õë£¬Ê³ÎïÖ¸Õë
snake *snake_p;//±éÀúÉßµÄÊ±ºòÓÃµ½µÄÖ¸Õë

u16 status = R;


void show_all();


#define MAP_W 20//28 //20
#define MAP_H 14//22//14
#define RECT_W 14//(TFT_HEIGHT/(MAP_W+2))
#define RECT_H 14//(TFT_HEIGHT/(MAP_H+2))


u8 map_sz[MAP_W][MAP_H] = {{0}};
static snake *snakeBuf = 0;
static bool snakeBufDlag[MAP_W*MAP_H] = {0};

snake *snake_get_buf(void)
{
	u16 i;
	for(i = 0; i < MAP_W*MAP_H; i++){
		if(!snakeBufDlag[i]){
			snakeBufDlag[i] = 1;
			return snakeBuf + i;
		}
	}
	return NULL;
}

void snake_put_buf(snake *buf)
{
	s32 i = buf - snakeBuf;
	if(i >= 0 && i < MAP_W*MAP_H)
		snakeBufDlag[i] = 0;
}

void snake_init()
{
	snake *tail;
	int i;
	tail = snake_get_buf();//´ÓÉßÎ²¿ªÊ¼£¬Í·²å·¨£¬ÒÔx,yÉè¶¨¿ªÊ¼µÄÎ»ÖÃ//
	tail->x = 0;
	tail->y = 0;
	tail->next = NULL;
	for (i = 0; i < 3; i++)//³õÊ¼³¤¶ÈÎª4
	{
		head = snake_get_buf();
		head->next = tail;
		head->x = tail->x+1;
		head->y = tail->y;
		map_sz[head->x][head->y] = SNAKE_BODY_COLOR;
		tail = head;
	}
	
}


void rand_next(snake *next)
{
	u16 x, y;
	x = next->x;
	y = next->y;
	do{
		if (map_sz[next->x][next->y] == 0)
			return;
		else
		{
			next->x = next->x + 1;
			if (next->x >= MAP_W)
			{
				next->x = 0;
				next->y = next->y + 1;
			}
			if (next->y >= MAP_H)
			{
				next->y = 0;
			}
			if (next->x == x && next->y == y)
			{
				endGamestatus = 1;
				return;
			}
			
		}
	} while (1);
}


void createFood()//Ëæ»ú³öÏÖÊ³Îï
{
	snake_put_buf(food);
	
	food = snake_get_buf();

	food->x = rand_int() % MAP_W;
	food->y = rand_int() % MAP_H;
	rand_next(food);
	printf("jk food x:%d, y:%d\n", food->x, food->y);
	if (endGamestatus ==0)
		map_sz[food->x][food->y] = SNAKE_FOOD_COLOR;
}



void creatMap()//´´½¨µØÍ¼
{
	u16 /*i,*/x,y;
	for (x = 0; x<MAP_W ; x++)
		for (y = 0; y<MAP_H; y++)
			map_sz[x][y] = SNAKE_NOP_COLOR;

}

u8 chet_endGamestatus(u16 x,u16 y)
{
	if (x<0 || x >= MAP_W || y<0 || y >= MAP_H || biteSelf() == 1){
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_POWER,0));
		return 1;
	}
	return 0;
}

u8 biteSelf()//ÅÐ¶ÏÊÇ·ñÒ§µ½ÁË×Ô¼º
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;
		}
		self = self->next;
	}
	return 0;
}

void snakeMove()//ÉßÇ°½ø,ÉÏU,ÏÂD,×óL,ÓÒR
{
	printf("%s, %d  \n", __func__, __LINE__);
	snake * nexthead = 0;
	while(!nexthead)
		nexthead = snake_get_buf();
	if (status == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
	}
	else if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
	}
	else if (status == L)
	{
		nexthead->x = head->x - 1;
		nexthead->y = head->y;
	}
	else if (status == R)
	{
		nexthead->x = head->x + 1;
		nexthead->y = head->y;
	}
	if (chet_endGamestatus(nexthead->x, nexthead->y))
	{
		//show_win(GAME_FAIL);//ÏÔÊ¾ÓÎÏ·Ê§°Ü´°¿Ú
		snake_put_buf(nexthead);
		
		return;
	}
	//Èç¹ûÏÂÒ»¸öÓÐÊ³Îï//
	if (nexthead->x == food->x && nexthead->y == food->y)
	{
		nexthead->next = head;
		head = nexthead;
		snake_p = head;
		map_sz[snake_p->x][snake_p->y] = SNAKE_BODY_COLOR;
		createFood();
	}
	else //Èç¹ûÃ»ÓÐÊ³Îï//
	{
		nexthead->next = head;
		head = nexthead;
		snake_p = head;
		while (snake_p->next->next != NULL)
		{
			map_sz[snake_p->x][snake_p->y] = SNAKE_BODY_COLOR;
			snake_p = snake_p->next;
		}
		map_sz[snake_p->x][snake_p->y] = SNAKE_NOP_COLOR;
		snake_put_buf(snake_p->next);
		snake_p->next = NULL;
	}
	show_all();
}


void detore_snake()
{
	free_buf((void *)snakeBuf);
	endGamestatus = 1;
	hal_sysMemFree_sdr(tc_icon);
}
/***********************ÐèÒªÖØÔØµÄº¯Êý**********************************/


void snakeOSDDisplayBufGet()
{
	u32 i = 0, j = TFT_HEIGHT*TFT_WIDTH;
	u8 *buf = (u8 *)setting_frame_buff;
	for(; i < j; i++)
		*(buf + i) = R_COLOR_TRANSFER;
#if 0
	for(i = 0;i < 100; i++)
		for(j = 0; j <200; j++)//i:行，j:列
			*(buf+i*360+ j) = R_COLOR_RED;
#endif
}
static int jkcount = 0;
void snake_I80(u8 *buf)
{
	u8 *src = (u8 *)setting_frame_buff;
	s32 i = 0, j = TFT_HEIGHT*TFT_WIDTH;
	printf("jk %s %d,scanmode :%d\n", __func__, __LINE__, lcdGetScanMode());
	#if 0
	#if 0
	for(i = TFT_HEIGHT - 1; i >= 0; i--){
				for(j = 0; j < TFT_WIDTH; j++){
					*(buf + j*TFT_HEIGHT + i) = *(src++);
				}
		}
	#endif
	for(; i < j; i++)
		*(buf + i) = *(src+i);
	#else
	switch(lcdGetScanMode()){
		case LCD_ROTATE_90:
			for(i = TFT_HEIGHT - 1; i >= 0; i--){
				for(j = 0; j < TFT_WIDTH; j++){
					*(buf + j*TFT_HEIGHT + i) = *(src++);
				}
			}
			break;
		case LCD_ROTATE_270:
			for(i = 0; i < TFT_HEIGHT; i++){
				for(j = TFT_WIDTH - 1; j >= 0; j--){
					*(buf + j*TFT_HEIGHT + i) = *(src++);
				}
			}
			break;
		default:
			break;
	}
	#endif
}


void snakeOSDDisplay()
{
	#if 1
	u32 frame_buff = (u32)dispLayerGetFreeFrame(0);
	if(frame_buff)
	{
		snake_I80((u8 *)frame_buff);
		dcache_flush_region(frame_buff,TFT_WIDTH*TFT_HEIGHT);
		dispLayerSetFrame(0,frame_buff);
		//setting_frame_buff = 0;
		printf("jk %s %d\n", __func__, __LINE__);
		//lcd_bk_ctrl(1);
	}
	#else
	disp_frame_t *p_lcd_buffer;
	//==draw bk==
	memcpy(p_lcd_buffer->y_addr,setting_frame_buff,640*480);

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);//(disp_frame_t *)gamemenu_bk_buf;//
	dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE_DEFAULT);
	#endif
	//ax32xx_deUpdate();
}
//逆序保留，（x=0,y=0）保存在buf[640*480-1]
void snake_frame_icon_draw(u8 *buf, u8 *icon_buf, DISPLAY_ICONSHOW *icon, u8 color) 
{
	u32 i, j, offset_pixel, offset_data, tft_w = TFT_WIDTH, tft_h = TFT_HEIGHT, tft_w_h = TFT_WIDTH*TFT_HEIGHT - 1;
	
	for(j = 0; j < icon->icon_h; j++){
		
		if(icon->pos_y + j >= tft_h)
			break;
		offset_data = j * icon->icon_w;
		offset_pixel = (icon->pos_y + j) * tft_w + icon->pos_x;
		
		for(i = 0; i < icon->icon_w; i++){
			if(icon->pos_x + i >= tft_w)
				break;
			if(!icon_buf && color){
				*(buf + tft_w_h - offset_pixel - i) = color;
				//printf("jk1 %d(%d,%d) = %x\n", tft_w_h - offset_pixel - i, i,offset_pixel,color);
				continue;
			}
			else if(icon_buf){
				if(icon->transparent == *(icon_buf + offset_data + i))
					continue;
				if(color){
					*(buf + tft_w_h - offset_pixel - i) = color;
					//printf("jk2 %d(%d,%d) = %x\n", tft_w_h - offset_pixel - i, i,offset_pixel,color);
				}
				else{
					*(buf + tft_w_h - offset_pixel - i) = color_palette[*(icon_buf + offset_data + i)];
					//printf("jk3 %d(%d,%d) = %x\n", tft_w_h - offset_pixel - i, i,offset_pixel,color);
				}
			}
		}
	}
}
void show_all()
{
	u8 x, y,i;
	snakeOSDDisplayBufGet();
	#if 1
	for (x = 0; x < MAP_W; x++)
	{
		for (y = 0; y < MAP_H; y++)
		{
			if (x == head->x&&y == head->y)
				show_icon_rect(x + 1, y + 1, SNAKE_HEART_COLOR);
			else
				show_icon_rect(x + 1, y + 1, map_sz[x][y]);

		}
	}
	
	for (i = 0; i<MAP_W + 1; i++)//上下边框
	{

		show_icon_rect(i, 0, SNAKE_WALL_COLOR);
		show_icon_rect(i, MAP_H + 1,SNAKE_WALL_COLOR);
	}
	for (i = 0; i<MAP_H + 1; i++)//左右边框
	{
		show_icon_rect(0, i, SNAKE_WALL_COLOR);
		show_icon_rect(MAP_W + 1, i, SNAKE_WALL_COLOR);
	}
	#endif
	show_icon_rect(MAP_W+1, MAP_H+1, SNAKE_WALL_COLOR);

	snakeOSDDisplay();
}


void * get_malloc(u16 size)
{
	return hal_sysMemMalloc(size, 32);
}

void key_read(u8 key_read)
{
	if (key_read == U && status != D)
	{
		status = U;	
		fps_count_reset();
	}
	else if (key_read == D && status != U)
	{
		status = D;
		fps_count_reset();
	}
	else if (key_read == L && status != R)
	{
		status = L;
		fps_count_reset();
	}
	else if (key_read == R && status != L)
	{
		status = R;
		fps_count_reset();
	}
	snakeMove();
	show_all();
}
void show_icon_rect(u16 x, u16 y,u8 type)
{
	u8 *p;
	const u8 *p_icon;
	DISPLAY_ICONSHOW icon;
	if(type == SNAKE_HEART_COLOR)
	{
		if(status == R)
			p_icon = tc_icon+icon_heart_R;
		else if(status == L)
			p_icon = tc_icon+icon_heart_L;
		else if(status == U)
			p_icon = tc_icon+icon_heart_U;
		else
			p_icon = tc_icon+icon_heart_D;
	}
	else if(type == SNAKE_FOOD_COLOR)
		p_icon = tc_icon+icon_food;
	else
		p_icon = tc_icon+icon_body;
	
	p = (u8 *)setting_frame_buff;

	icon.icon_w = 14;
	icon.icon_h = 14;
	icon.transparent = 0;
	icon.pos_x = x*RECT_W + 5;//5;
	icon.pos_y = y*RECT_H + 4;//4;
	
	if (type == SNAKE_WALL_COLOR)
		snake_frame_icon_draw(p, (u8*)p_icon, &icon, R_COLOR_RED);
	else if (type == SNAKE_NOP_COLOR)
		;
	else 
		snake_frame_icon_draw(p, (u8*)p_icon, &icon, /*NULL*/0);
	
}

//disp_frame_t * snake_show_buf; 




void snake_main_init()
{
	u16 lcd_w,  lcd_h,cnt = 0;
	food = NULL;
	head = NULL;
	//printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	status = R;
	sleeptime = 650;// 33;
	tc_icon = get_bin_data(RES_GAME_SNAKE_ICON);
	//printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());

	//snake_show_buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	//printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	//game_bk_draw((u8 *)snake_show_buf->y_addr, 0, /*NULL*/0x8080);
	game_bk_draw((u8 *)get_game_base_buf(), 0, /*NULL*/0x8080);
	get_game_show_buf(true);
	frame_buff_display();
	//printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	//lcdGetRes(&lcd_w,&lcd_h);
	//hal_dispframeVideoCfg(snake_show_buf,0,0,lcd_w,lcd_h);
	//lcdframeVideoCfg(snake_show_buf,(lcd_w-TFT_WIDTH)/2,(lcd_h-TFT_HEIGHT)/2,TFT_WIDTH,TFT_HEIGHT);
	
	//ax32xx_sysDcacheFlush((u32)snake_show_buf->y_addr,snake_show_buf->w*snake_show_buf->h*3/2);
	//dispLayerShow(DISP_LAYER_VIDEO,(INT32U)snake_show_buf,0,0,VIDEO_ROTATE_DEFAULT);
	
	printf("jk %d,PortGetFreeHeapSize:%d, w:%d, h:%d\r\n", __LINE__, xPortGetFreeHeapSize(), lcd_w, lcd_h);
#if 1
	snakeBuf = (snake*)hal_sysMemMalloc(MAP_W*MAP_H*sizeof(snake), 32);
	if(NULL!=snakeBuf)
	{
		printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	}
	else
	{
		deg_Printf("mem err!\n");
		printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
		return 0;
	}
	snake_init();
	creatMap();
	endGamestatus = 0;
	createFood();
	show_all();
#endif
	printf("jk %s, %d, exit\n", __func__, __LINE__);
}

void game_snake_entry(void)
{
	u8 msg, err;
//	u32 mmsg;
	//dispLayerInit(DISP_LAYER_OSD0);
	printf("jk %s, %d, entry\n", __func__, __LINE__);

	setting_frame_buff = (INT32U)hal_sysMemMalloc(TFT_WIDTH*TFT_HEIGHT,32);//(INT32U)dispLayerGetFrame(DISP_LAYER_OSD0);//
	if(!setting_frame_buff)
	{
		printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
		return;
	}
	snake_main_init();
	while(1){
		deg_Printf("jk wait recmsg\n");

		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		// mmsg = (u32)XMsgQPend(SysCtrl.sysQ,&err);
		// msg  =  mmsg |0xff;
		// if ((mmsg>>16) == KEY_RELEASE)
		deg_Printf(" recmsg %d\n", msg);
		if(err == X_ERR_NONE)
		{
			switch (msg){
				//case KEY_EVENT_OK://
				case KEY_EVENT_UP:
					key_read(U);
					break;
				case KEY_EVENT_PHOTO://printer
				case KEY_EVENT_DOWN:
					key_read(D);
					break;
				case KEY_EVENT_RIGHT:
					key_read(R);
					break;
				case KEY_EVENT_LEFT:
					key_read(L);
					break;
				case KEY_EVENT_POWER:
				case KEY_EVENT_TOUCH_BACK:		
					detore_snake();
					hal_sysMemFree_sdr((void*)setting_frame_buff);
					//dispLayerFreeFrame(DISP_LAYER_VIDEO,snake_show_buf);
					return;
				case SYS_EVENT_GAME_TICK:
					snakeMove();
					break;
			}
		}
	}
	printf("jk %s, %d, exit\n", __func__, __LINE__);
}

#endif
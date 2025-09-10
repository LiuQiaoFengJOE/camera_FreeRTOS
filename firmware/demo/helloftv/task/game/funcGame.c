#include "../app.h"

#if GAME_FUNC_EN

#include "task/uiWin.h"
#include "funcGame.h"
#include "asm/osal_rand.h"


#define  PWR_KEY_CH      GPIO_PD
#define  PWR_KEY_PIN     GPIO_PIN1
#define  ADC_KEY_CH      ADC_CH_PB1

#define  LF_KEY_CH      GPIO_PE
#define  LF_KEY_PIN     GPIO_PIN0

#define  SET_KEY_CH      GPIO_PE
#define  SET_KEY_PIN     GPIO_PIN1

msgGameFunc gameEntry;

static u8* game_base_buf;
static u8 * game_show_buf = 0;
static u16 bk_w,bk_h;


#define SYSTEM_PCM_BUFFER_SIZE_MAX 	(300*1024)
#define SYSTEM_PCM_BUFFER_SIZE		512

void gameInit()
{
	//==init==
	//if(game_snake_entry != gameEntry){		
		//hal_lcdGetResolution(&bk_w,&bk_h);
		bk_w = TFT_WIDTH;
		bk_h = TFT_HEIGHT;
		game_base_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
		if(!game_base_buf)
		{
			deg_Printf("mem err!\n");
			return ;
		}
	//}
}


void gameExit()
{
	//if(game_snake_entry != gameEntry){
		//game_over(true);
		if(game_base_buf)
		{
			hal_sysMemFree_sdr(game_base_buf);
			game_base_buf=NULL;
		}
		if(game_show_buf)
		{
			hal_sysMemFree_sdr(game_show_buf);
			game_show_buf=NULL;
		}
	//}
	//else
		//game_over(false);
		uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_GAME_TIPS_GAME_OVER,2);
		functionStart(FUNCTION_GAME_MENU,0);
}

void taskGameService(uint32 arg)
{
	gameInit();

	gameEntry();
	gameExit();
	return;
}

	
void taskGameOpen(uint32 arg)
{
	deg_Printf("game open\n");
	//game_audio_init();
	uiOpenWindow(&gameWindow,0);
}


void taskGameClose(uint32 arg)
{

	deg_Printf("game exit.\n");
}

static u32	game_service_cnt = 0;
void taskGame(void* arg)
{
	u32 msgType,msgData;

	yuv_rgb_table();

	//taskGameOpen(0);

	while(1)
	{
		printf("task game start service :%d\n", game_service_cnt);
		if(recMsg(OS_TASK_GAME,&msgType,&msgData)>0)
		{
			printf("jk game task msgid:%d, data:%d\n",msgType,msgData);
			//msgDeal(msgType,msgData);
			
			switch (msgType){
			case KEY_EVENT_OK://
				taskGameService(0);
				break;
			default:
				printf("game task msgid err :%d, data:%d\n",msgType,msgData);
				break;
			}
		}
		//functionStart(FUNCTION_GAMEMENU,0);
		game_service_cnt++;
	}
	//taskGameClose(0);
}

sysFunction functionGame=
{
	"Game",
	NULL,
	taskGameOpen,
	taskGameClose,
};

INT32U setting_frame_buff = 0;


bool endGamestatus = 1;
u16 sleeptime = 60;
static u16 fps_count = 0;
static u8 gameTickCount = 0;

static u8 gameKeyType = GAME_KEY_NONE;

void fps_count_reset()
{
	fps_count = 0;
}
#if 0

void game_key_event()
{//photo up    ok down 
	u8 msgEvent[] = {NULL, KEY_EVENT_LEFT, KEY_EVENT_OK, KEY_EVENT_PHOTO, KEY_EVENT_RIGHT, KEY_EVENT_POWER};
	u8 get_key = game_key_judge();
	if(gameKeyType != get_key){
		gameKeyCount = 0;
		gameKeyType = get_key;
	}
	else{
		gameKeyCount++;
		if(gameKeyCount == 3 && gameKeyType != GAME_KEY_NONE){
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(msgEvent[gameKeyType],0));
			game_audio_play(RES_MUSIC_KEY_SOUND,0,0);
			fps_count_reset();
		}
	}
}

static void game_tick()
{
	if(endGamestatus==0)
	{
		fps_count += sleeptime;
		if (fps_count >= 1000)
		{
			fps_count = 0;
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_GAME_TICK,0));
		}
	}
}

void gameTickGenerator()
{
	if(taskCurrent() != TASK_GAME)
		return;
	game_key_event();
	
	gameTickCount++;
	if(3 == gameTickCount){ // 32Hz
		gameTickCount = 0;
		game_tick();
	}
}

#endif

static u8 randList[] = {
28,    126,   77,    182,   207,   143,   188,   20,    74,    249,   99,    216,   137,   57,    200,   206,   114,   98,    220,   0,     230,   88,    136,   150,   176,   171,   15,    56,    69,    159,   23,    149,   
180,   31,    123,   64,    116,   8,     164,   102,   66,    210,   186,   18,    39,    94,    3,     250,   58,    202,   118,   255,   22,    115,   104,   131,   236,   168,   128,   38,    30,    4,     196,   194,   
111,   101,   163,   27,    129,   24,    73,    238,   119,   2,     29,    25,    40,    192,   160,   223,   130,   205,   162,   93,    117,   55,    110,   145,   52,    53,    201,   217,   173,   42,    140,   81,    
133,   19,    254,   152,   181,   193,   67,    5,     11,    82,    71,    134,   153,   121,   245,   41,    35,    178,   124,   165,   225,   33,    233,   63,    141,   47,    157,   144,   96,    177,   21,    219,   
14,    61,    122,   12,    72,    6,     79,    231,   10,    215,   70,    46,    59,    43,    146,   174,   89,    243,   107,   45,    155,   32,    148,   87,    212,   203,   75,    169,   100,   51,    158,   248,   
242,   7,     50,    109,   1,     113,   237,   34,    190,   232,   97,    161,   185,   65,    103,   84,    239,   156,   112,   76,    246,   252,   105,   108,   240,   189,   167,   222,   80,    251,   179,   26,    
139,   83,    135,   226,   90,    247,   221,   127,   170,   209,   234,   218,   154,   37,    85,    197,   183,   68,    60,    235,   9,     166,   172,   120,   138,   44,    187,   208,   214,   184,   49,    16,    
213,   13,    48,    54,    191,   229,   62,    92,    151,   195,   228,   125,   17,    36,    106,   142,   199,   204,   91,    86,    198,   253,   224,   211,   147,   175,   244,   241,   227,   95,    132,   78
};
static u8 prevRand = 0;
u16 rand_int()
{	//return (u16)randList[XOSRandom()&0xff];
	prevRand+=randList[(prevRand + osal_random())&0xff];
	return (u16)randList[prevRand];
}
u32 get_game_base_buf()
{
	if(game_base_buf)
		return (u32)game_base_buf;
}

u32 get_game_show_buf(bool show_bk)
{
	if(!game_show_buf)
	{
		game_show_buf = hal_sysMemMalloc(bk_w*bk_h*3/2,32);
		if(NULL==game_show_buf)
		{
			deg_Printf("mem err!\n");
			printf("jk %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
			return 0;
		}
	}
	if(show_bk)
	{
		memcpy(game_show_buf,game_base_buf,bk_w*bk_h*3/2);
	}

	return (u32)game_show_buf;
}

u8 *get_bin_data(u32 id)
{
	u32	addr,size;

	addr= nv_open(id);
	if(addr<0)
	{
		deg_Printf("read id err\n");
		return 0;
	}
	size = nv_size(id);
	if(size<0)
	{
		deg_Printf("get size err\n");
		return 0;
	}
	u8 *data_buf = hal_sysMemMalloc(size,32);
	if(data_buf){
		nv_read(addr,data_buf,size);
		ax32xx_sysDcacheFlush((u32)data_buf, size);
		return data_buf;
	}
	return 0;
}

void frame_icon_draw(u8 *y_buf, u8 *icon_y_buf, DISPLAY_ICONSHOW *icon, u32 color)
{
	u32 i, j, y, uv, offset_pixel, offset_pixel_uv, offset_data, offset_data_uv;
	u16 tft_w = TFT_WIDTH, tft_h = TFT_HEIGHT, tft_w_uv = ((tft_w + 0x1)&(~0x1)) >> 1, tft_h_uv = ((tft_h + 0x1)&(~0x1)) >> 1, icon_w_uv = ((icon->icon_w + 0x1)&(~0x1)) >> 1, icon_h_uv = ((icon->icon_h  + 0x1)&(~0x1)) >> 1;
	u16 *icon_uv_buf = (u16 *)(icon_y_buf + icon->icon_w*icon->icon_h);
	u16 *uv_buf = (u16 *)(y_buf + tft_w*tft_h);
	
	for(j = 0; j < icon->icon_h; j++){
		
		if(icon->pos_y + j >= tft_h)
			break;
		offset_data = j * icon->icon_w;
		offset_pixel = (icon->pos_y + j) * tft_w + icon->pos_x;
		offset_data_uv = (j >> 1) * icon_w_uv;
		offset_pixel_uv = (((icon->pos_y + j) >> 1) * tft_w_uv);
		
		for(i = 0; i < icon->icon_w; i++){
			if(icon->pos_x + i >= tft_w)
				break;
			if(!icon_y_buf && color){
				*(y_buf + offset_pixel + i) = (color >> 16);
				*(uv_buf + offset_pixel_uv + ((icon->pos_x + i) >> 1)) = (color & 0xffff);
				continue;
			}
			else if(icon_y_buf){
				if(icon->transparent == (*(icon_y_buf + offset_data + i) << 16) + *(icon_uv_buf + offset_data_uv + (i >> 1)))
					continue;
				if(color){
					*(y_buf + offset_pixel + i) = (color >> 16);
					*(uv_buf + offset_pixel_uv + ((icon->pos_x + i) >> 1)) = (color & 0xffff);
				}
				else{
					*(y_buf + offset_pixel + i) = *(icon_y_buf + offset_data + i);
					*(uv_buf + offset_pixel_uv + ((icon->pos_x + i) >> 1)) = *(icon_uv_buf + offset_data_uv + (i >> 1));
				}
			}
		}
	}
}

void game_OSD_str_draw(uint8* drawBuff, uiRect* winRect,uiRect* invalidRect,resID id,charFont font,uint8 alignment,uiColor fontColor,uiColor bgColor,uiColor rimColor)
{
	uint16 i,resWidth,resHeight,width,strH;
	int16 resX,resY;
	
	uint8* destAddr;
	uint8* lastAddr;
	uint8* charBuff;
	uint32 strNum;
	uiRect resRect,charRect,drawRect;
	uint8 special;
	int32 lcdRotate=hal_lcdGetShowingRotate();
	
	strNum=resGetStringInfor(id,&resWidth,&resHeight,font);
	if(strNum==0)
	{
		debug_err("find string error!!!,0x%x\n",id);
		return;
	}
	strH=resHeight;
	drawRect.x0=invalidRect->x0;
	drawRect.y0=invalidRect->y0;
	drawRect.x1=invalidRect->x1;
	drawRect.y1=invalidRect->y1;
	if(alignment&ALIGNMENT_LEFT)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	else if(alignment&ALIGNMENT_RIGHT)
	{
		resRect.x1=winRect->x1;
		resX=resRect.x0=winRect->x1-resWidth+1;
	}
	else
	{
		resX=resRect.x0=(winRect->x0+winRect->x1-resWidth)>>1;
		resRect.x1=resRect.x0+resWidth-1;
	}
	if(resRect.x0<winRect->x0)
	{
		resX=resRect.x0=winRect->x0;
		resRect.x1=resRect.x0+resWidth-1;
	}
	resY=resRect.y0=(winRect->y0+winRect->y1-resHeight)>>1;
	if(!resIdIsStr(id))
		resY=resRect.y0=resRect.y0+3;
	resRect.y1=resRect.y0+resHeight-1;
	if(winOverlapping(&resRect,&drawRect)==false)
	{
		if(bgColor!=INVALID_COLOR)
			drawRectangle(&drawRect,bgColor);
		return;
	}
	if(bgColor!=INVALID_COLOR)
		drawNotIntersect(&drawRect,&resRect,bgColor);
	winIntersection(&drawRect,&drawRect,&resRect);
	resRect.x0=drawRect.x0-resX;
	resRect.y0=drawRect.y0-resY;
	resRect.x1=drawRect.x1-resX;
	resRect.y1=drawRect.y1-resY;
	//drawBuff=getDrawBuff();
	//destAddr=drawBuff+drawRect.y0*LCD_WIDTH+drawRect.x0;
	
	if(lcdRotate==LCD_ROTATE_0)
		destAddr=drawBuff+drawRect.y0*LCD_WIDTH+drawRect.x0;
	else if(lcdRotate==LCD_ROTATE_270)
		destAddr=drawBuff+drawRect.x0*LCD_HEIGHT+(LCD_HEIGHT-drawRect.y0-1);
	else if(lcdRotate==LCD_ROTATE_180)
		destAddr=drawBuff+(LCD_HEIGHT-drawRect.y0-1)*LCD_WIDTH+(LCD_WIDTH-drawRect.x0-1);
	else if(lcdRotate==LCD_ROTATE_90)
		destAddr=drawBuff+(LCD_WIDTH-drawRect.x0-1)*LCD_HEIGHT+drawRect.y0;
	else
		return;

		 
	width=0;
	lastAddr=destAddr;
	for(i=0;i<strNum;i++)
	{
		charBuff=resGetCharInfor(id, i, &resWidth, &resHeight,font,&special);
		if(charBuff==NULL)
			break;
		if(special==0)
			width+=resWidth;
		if(width>resRect.x0)
		{
			if(special)
			{
				if(resRect.y0>=resHeight)
					continue;
				charRect.y0=resRect.y0;
				charRect.y1=resHeight;
			}
			else
			{
				if(width-resRect.x0<resWidth)
					charRect.x0=resWidth-(width-resRect.x0);
				else
					charRect.x0=0;
				if(width>resRect.x1+1)
					charRect.x1=(resWidth-(width-(resRect.x1+1))-charRect.x0)-1;
				else
					charRect.x1=resWidth-1;
					//charRect.x1=resWidth-charRect.x0-1;
				if(strH>resHeight)
				{
					charRect.y0=resRect.y0-(int16)((strH-resHeight)>>1);
					charRect.y1=resRect.y1-(int16)((strH-resHeight)>>1);
					if(charRect.y0>=resHeight||charRect.y1<0)
					{
						charRect.y0=-1;
						charRect.y1=-1;
					}
					else
					{
						if(charRect.y0<0)
							charRect.y0=0;
						if(charRect.y1>=resHeight)
							charRect.y1=resHeight-1;
					}
				}
				else
				{
					charRect.y0=resRect.y0;
					charRect.y1=resRect.y1;
				}
			}
			if(special)
				drawChar(lastAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
			else
			{
				if(strH>resHeight)
				{
					if(charRect.y0>=0)
					{
						if(lcdRotate==LCD_ROTATE_0)
							drawChar(destAddr+((strH-resHeight)>>1)*LCD_WIDTH,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_270)
							drawChar(destAddr-((strH-resHeight)>>1),charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_180)
							drawChar(destAddr-((strH-resHeight)>>1)*LCD_WIDTH,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
						else if(lcdRotate==LCD_ROTATE_90)
							drawChar(destAddr+((strH-resHeight)>>1),charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
					}
				}
				else
					drawChar(destAddr,charBuff,&charRect,resWidth,resHeight,fontColor,bgColor,rimColor);
			}
			if(special==0)
			{
				//destAddr+=(charRect.x1-charRect.x0+1);
				lastAddr=destAddr;
				if(lcdRotate==LCD_ROTATE_0)
					destAddr+=(charRect.x1-charRect.x0+1);
				else if(lcdRotate==LCD_ROTATE_270)
					destAddr+=(charRect.x1-charRect.x0+1)*LCD_HEIGHT;
				else if(lcdRotate==LCD_ROTATE_180)
					destAddr-=(charRect.x1-charRect.x0+1);
				else if(lcdRotate==LCD_ROTATE_90)
					destAddr-=(charRect.x1-charRect.x0+1)*LCD_HEIGHT;
					 
			}
			if(width>resRect.x1)
			{
				if(i<strNum-1)
				{
					resGetCharInfor(id, i+1, NULL, NULL,font,&special);
					if(special)
						continue;
				}
				break;
			}
		}
	}
	dcacheFlush(&drawRect);
}
#if 1
bool game_tips(resID tips_msg)
{
	u8 msg = 1, err, selLeft, selRight;
	bool idx = true;
	u32 i, j;
	uiRect sRect = {113, 207, 92, 116};
	u8 *frame_buff;

	endGamestatus = 1;
	//dispLayerInit(DISP_LAYER_OSD0);
	while(1){
		if(msg == NULL)
			msg = (u8)XMsgQPend(SysCtrl.sysQ,&err); 
		if(msg){
			switch (msg){
				case KEY_EVENT_RIGHT:
				case KEY_EVENT_LEFT:
					idx = !idx;
					break;
				case KEY_EVENT_POWER:
				case KEY_EVENT_OK:
					//dispLayerUninit(DISP_LAYER_OSD0);
					lcdOsdEn(0);
					endGamestatus = 0;
					return idx;
			}
			frame_buff = (u8 *)dispLayerGetFreeFrame(0);
			if(frame_buff)
			{
				
				if(lcdGetScanMode() == LCD_ROTATE_90)
				{

					for(j = 90; j < 230; j++){
						for(i = 85; i < 155; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = R_COLOR_WHITE;
						}
					}

					for(j = 93; j < 227; j++){
						for(i = 88; i < 120; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = R_COLOR_BLACK;
						}
					}					
					if(idx){
						selLeft = R_COLOR_BLUE;
						selRight = R_COLOR_BLACK;
					}
					else{
						selLeft = R_COLOR_BLACK;
						selRight = R_COLOR_BLUE;
					}

					deg_Printf("is rotato show 90\n");
					for(j = 93; j < 160; j++){
						for(i = 121; i < 152; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = selRight;
						}
					}
					for(j = 161; j < 227; j++){
						for(i = 121; i < 152; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = selLeft;
						}
					}
					sRect.x0 = 93, sRect.x1 = 227, sRect.y0 = 87, sRect.y1 = 112;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,tips_msg,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,R_COLOR_BLACK,R_COLOR_BLACK);

					sRect.x0 = 93, sRect.x1 = 160, sRect.y0 = 122, sRect.y1 = 147;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,R_ID_STR_COM_YES,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,selLeft,selLeft);
					sRect.x0 = 161, sRect.x1 = 227, sRect.y0 = 122, sRect.y1 = 147;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,R_ID_STR_COM_NO,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,selRight,selRight);

				}else if(lcdGetScanMode() == LCD_ROTATE_270)
				{

					for(j = 88; j < 232; j++){
						for(i = 83; i < 157; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = R_COLOR_WHITE;
						}
					}

					for(j = 91; j < 229; j++){
						for(i = 122; i < 154; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = R_COLOR_BLACK;
						}
					}

					if(idx)
					{
						selLeft = R_COLOR_BLUE;
						selRight = R_COLOR_BLACK;
					}else
					{
						selLeft = R_COLOR_BLACK;
						selRight = R_COLOR_BLUE;
					}

					deg_Printf("is rotato show 270\n");
					for(j = 159; j < 229; j++){
						for(i = 86; i < 119; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = selRight;
						}
					}
					for(j = 91; j < 158; j++){
						for(i = 86; i < 119; i++){
							*(frame_buff + j*OSD_HEIGHT + i) = selLeft;
						}
					}
					sRect.x0 = 93, sRect.x1 = 227, sRect.y0 = 87, sRect.y1 = 112;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,tips_msg,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,R_COLOR_BLACK,R_COLOR_BLACK);

					sRect.x0 = 162, sRect.x1 = 227, sRect.y0 = 121, sRect.y1 = 147;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,R_ID_STR_COM_NO,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,selRight,selRight);
					sRect.x0 = 90, sRect.x1 = 160, sRect.y0 = 121, sRect.y1 = 147;
					game_OSD_str_draw(frame_buff, &sRect,&sRect,R_ID_STR_COM_YES,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,selLeft,selLeft);

				}



				dcache_flush_region(frame_buff,OSD_WIDTH*OSD_HEIGHT);
				dispLayerSetFrame(0,frame_buff);
				//ax32xx_deUpdate();
			}
			msg = NULL;
		}
	}
}
#endif
static void _delay_us(u32 dtime)
{
	volatile int cnt=dtime<<1;
	while(cnt--)
	{
		asm("l.nop");
	}
}
extern void lcdForceOsdShow(void);
void game_over(bool osd_init)
{
	//if(osd_init)
		//dispLayerInit(DISP_LAYER_OSD0);
	u8 *frame_buff = (u8 *)dispLayerGetFreeFrame(0);
	u32 i = 0, j = TFT_HEIGHT*TFT_WIDTH;
	uiRect sRect = {0, 320, 108, 138};
	if(frame_buff)
	{
		for(; i < j; i++)
			*(frame_buff + i) = R_COLOR_BLACK;

		game_OSD_str_draw(frame_buff, &sRect,&sRect,R_ID_STR_GAME_TIPS_GAME_OVER,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_WHITE,R_COLOR_BLACK,R_COLOR_BLACK);

		
		dcache_flush_region(frame_buff,TFT_WIDTH*TFT_HEIGHT);
		printf("==gamedd==%s, %d\n", __FUNCTION__, __LINE__);
		lcdForceOsdShow();
		//dispLayerSetFrame(0,frame_buff);
		//ax32xx_deUpdate();
		printf("==gamedd==%s, %d\n", __FUNCTION__, __LINE__);
		//XOSTimeDly(1000);
		_delay_us(1000);
		printf("==gamedd==%s, %d\n", __FUNCTION__, __LINE__);
	}
	//dispLayerUninit(DISP_LAYER_OSD0);
}

void game_bk_draw(u8 *y_buf, u32 jpg, u32 color)
{
	s32 offset = TFT_WIDTH*TFT_HEIGHT;
	u16 *uv_buf = (u16 *)(y_buf + offset);
	if(color){
		while((--offset) >= 0)
			*(y_buf + offset) = (color >> 16);
		offset = (TFT_WIDTH >> 1)*(TFT_HEIGHT >> 1);
		while((--offset) >= 0)
			*(uv_buf + offset) = (color & 0xffff);
	}
	else if(jpg){
		dec_jpg_res_to_yuvbuf(jpg, y_buf,TFT_WIDTH, TFT_HEIGHT);
		//jpg_decode_buf(jpg, y_buf, uv_buf, TFT_WIDTH, TFT_HEIGHT);
	}
}
	
void frame_buff_display()
{
#if 0
	if(game_show_buf)
	{
		ax32xx_sysDcacheFlush((u32)game_show_buf->y_addr,game_show_buf->w*game_show_buf->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)game_show_buf,0,0,VIDEO_ROTATE);
		game_show_buf = 0;
		//XOSTimeDly(5);
	}
#else
	disp_frame_t *p_lcd_buffer;
	INT16U screenW=0,screenH=0;
	
	p_lcd_buffer = lcdVideoFrameMalloc();//(disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);//(disp_frame_t *)gamemenu_bk_buf;//
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		//lcdGetRes(&lcd_w,&lcd_h);
		lcd_w = bk_w;
		lcd_h = bk_h;
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		//hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
		lcdGetRes(&screenW, &screenH);
		lcdframeVideoCfg(p_lcd_buffer,(screenW-TFT_WIDTH)/2,(screenH-TFT_HEIGHT)/2,lcd_w,lcd_h);
#if 1
		
		memcpy(p_lcd_buffer->y_addr,game_show_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
#endif
		//dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE_DEFAULT);

		INT16U cnt=0;
		while(lcdDeBusy())
		{
			osal_task_sleep(10);
			if(cnt++>=5)
				break;
		}
		lcdDealVideBuff((disp_frame_t *)p_lcd_buffer);
		osal_task_sleep(60);
		lcd_bk_ctrl(1);

	}
#endif
}

void free_buf(void *p)
{
	if (p != NULL)
		hal_sysMemFree_sdr((void *)p);
	p = NULL;
}
//==id : res id==
//==wait: 0 not wait ,1 wait pcm free==
//==finish: 0: not finish 
u8 game_audio_play(u32 id,u8 wait,u8 finish)
{
	dacPlaySound(id,100);
	return 0;
}

/*******************************************************************************
* Function Name  : XMsgQPend
* Description    : X message pend
* Input          : XMsgQ *mque :  message queue point
                      INT8U *err : err state
* Output         : none                                            
* Return         : MSG_T * message
*******************************************************************************/
u32 XMsgQPend(void *mque,INT8U *err)
{

	u32 msgType,msgData;
	recMsg(OS_TASK_GAME,&msgType,&msgData);

    return (msgType);
}

/*******************************************************************************
* Function Name  : XMsgQPost
* Description    : X message post
* Input          : XMsgQ *mque :  message queue point
                      MSG_T *msg  : message
* Output         : none                                            
* Return         : INT32S : 0 : success
*******************************************************************************/
INT32S XMsgQPost(void *mque,void *msg)
{

	u32 msgType,msgData;

	
	msgType=getType(msg);
	msgData=getValue(msg);
	//recMsg(OS_TASK_GAME,&msgType,&msgData)
	sendMsgNonblock(OS_TASK_GAME, msgType, msgData);
    return 0;
}

#endif

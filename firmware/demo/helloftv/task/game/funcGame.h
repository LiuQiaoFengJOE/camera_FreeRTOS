#ifndef __TASKGAME_H_
#define __TASKGAME_H_
#include "lcd/lcdframe.h"
#include "task/app.h"

#define XOSTimeGet os_time_get
#define  deg_Printf printf
#define XOSTimeDly osal_task_sleep
#define hal_lcdGetShowingRotate lcdGetScanMode

#define hal_sysMemMalloc(a, b) osal_malloc(a)
#define hal_sysMemFree_sdr(a) osal_free(a)

#define nv_open res_open
#define nv_size res_size
#define nv_read res_read

#define  X_ERR_NONE        0
#define  X_ERR_QFULL      255
#define  X_ERR_QEMPTY    254
#define  X_ERR_TIMEOUT   253

#define TFT_WIDTH		320
#define TFT_HEIGHT		240

#define TRANSPARENT_COLOR 0x8c8080

#define RED_COLOR		0x4c5bff


#define	YUV_BLACK	0x8080
#define	YUV_RED 	0x6C5BFF
#define	YUV_RED1 	0x4C5BFF
#define YUV_WHITE	0xFF8080

enum{
GAME_KEY_NONE,
GAME_KEY_L,
GAME_KEY_U,
GAME_KEY_D,
GAME_KEY_R,
GAME_KEY_OK,
GAME_KEY_POWER,
};

typedef struct
{
	INT16U	icon_w;
	INT16U	icon_h;
	INT32U	transparent;
	INT16S	pos_x;
	INT16S	pos_y;
} DISPLAY_ICONSHOW;

typedef void (*msgGameFunc)(void);

extern msgGameFunc gameEntry;
extern bool endGamestatus;
extern u32 setting_frame_buff;
extern u16 sleeptime;

extern void game_snake_entry(void);
extern void game_sokoban_entry(void);
extern void game_tetris_entry(void);
extern void game_block_entry(void);
extern void game_maze_entry(void);

u32 XMsgQPend(void *mque,INT8U *err);
INT32S XMsgQPost(void *mque,void *msg);

#endif
  
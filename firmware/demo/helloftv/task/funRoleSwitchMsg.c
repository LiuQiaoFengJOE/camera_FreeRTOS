#include "task/app.h"
#include "drv/lcd/lcdframe.h"
#include "funRoleSwitchRes.c"
#include "mediaFile/picIdentify.h"

#define ROLESWITCH_TASK_NUM 10
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define BUFFER_SIZE         (SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2)
#define KEY_DEBOUNCE_MS     50

extern menu MENU(movie);
static void RoleSwitchIdxBuf(void);
static void RoleSwitch_showbuf(void);
static void jpgdecById(u32 id, u8* buf);

static u32 RoleSwitch_bmp_id[ROLESWITCH_TASK_NUM] = {
    RES_AIROLEDOCTOR, RES_AIROLETEACHER, RES_AIROLESTORYTELLER,
    RES_AIROLEFLATTERER, RES_AIROLESOCRATES, RES_AIROLENAYSAYER,
    RES_AIROLENEWTON, RES_AIROLEDARWIN, RES_AIROLECOOK,
    RES_AIROLEPOET
};

static u8* RoleSwitch_id_buf = NULL;
static u16 id_w = SCREEN_WIDTH, id_h = SCREEN_HEIGHT;
static u8  RoleSwitch_taskidx = 0;
static u8  jpgdecFlag = 0;
static u32 last_key_time = 0;

static void jpgdecById(u32 id, u8* buf){
    u32 id_addr = res_open(id);
    if (id_addr == 0) {
        printf("jpgdecById: resource %d not found\n", id);
        return;
    }
    uint32_t jpgbufsize = res_size(id);
    INT16U lcdW = SCREEN_WIDTH, lcdH = SCREEN_HEIGHT;

    if (buf == NULL) {
        printf("jpgdecById: buffer is NULL\n");
        return;
    }
    printf("jpgbufsize:%d, lcdw:%d, lcdh:%d\r\n", jpgbufsize, lcdW, lcdH);

    void *jpgbuf = osal_malloc(jpgbufsize + 64);
    if (jpgbuf == NULL) {
        printf("malloc jpgbuf:%d error\r\n", jpgbufsize);
        return;
    }
    res_read(id_addr, jpgbuf, jpgbufsize);
    dcache_writeback_region((u32)jpgbuf, jpgbufsize);
    not_isr_jpgdec2(buf, jpgbuf, jpgbufsize, lcdW, lcdH);
    osal_free(jpgbuf);
}


static void RoleSwitch_showbuf(void) {
    disp_frame_t *p_lcd_buffer;
    INT16U screenW = SCREEN_WIDTH, screenH = SCREEN_HEIGHT;
    INT16U lcdW = SCREEN_WIDTH, lcdH = SCREEN_HEIGHT;

    p_lcd_buffer = lcdVideoFrameMalloc();
    if (p_lcd_buffer == NULL) {
        printf("malloc lcdframe error\r\n");
        return;
    }
    printf("RoleSwitch_showbuf: screen=%dx%d, video=%dx%d, expected=%dx%d\n", screenW, screenH, lcdW, lcdH, id_w, id_h);
    if (lcdW != id_w || lcdH != id_h) {
        printf("Error: LCD resolution mismatch, expected %dx%d, got %dx%d\n", id_w, id_h, lcdW, lcdH);
        lcdframeFree(p_lcd_buffer);
        return;
    }
    lcdframeVideoCfg(p_lcd_buffer, 0, 0, lcdW, lcdH);

    if (!(jpgdecFlag & 0x02) || RoleSwitch_id_buf == NULL) {
        printf("Error: Buffer for taskidx %d not found or invalid! Retrying decode...\n", RoleSwitch_taskidx);
        RoleSwitchIdxBuf();
        if (!(jpgdecFlag & 0x02) || RoleSwitch_id_buf == NULL) {
            printf("Error: Retry failed for taskidx %d! Clearing screen...\n", RoleSwitch_taskidx);
            memset(p_lcd_buffer->y_addr, 0, p_lcd_buffer->w * p_lcd_buffer->h);
            memset(p_lcd_buffer->y_addr + p_lcd_buffer->w * p_lcd_buffer->h, 128, p_lcd_buffer->w * p_lcd_buffer->h / 2);
            dcache_writeback_region((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
            lcdDealVideBuff(p_lcd_buffer);
            lcdframeFree(p_lcd_buffer);
            return;
        }
    }

    if (p_lcd_buffer->w != lcdW || p_lcd_buffer->h != lcdH) {
        printf("Error: Frame buffer size mismatch, expected %dx%d, got %dx%d\n", lcdW, lcdH, p_lcd_buffer->w, p_lcd_buffer->h);
        lcdframeFree(p_lcd_buffer);
        return;
    }

    memcpy(p_lcd_buffer->y_addr, RoleSwitch_id_buf, BUFFER_SIZE);
    dcache_writeback_region((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
    lcdDealVideBuff(p_lcd_buffer);
    lcdframeFree(p_lcd_buffer);
}

static int RoleSwitchKeyMsgOk(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        printf("Role Switch KeyMsgOk!!!\n");
        RoleSwitchStrShow(handle, false);
        if (0 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_DOCTOR;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (1 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_TEACHER;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (2 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_STORYTLLER;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (3 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_FLATTERER;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (4 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_SOCRATES;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (5 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_NAYSAYER;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (6 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_NEWTON;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (7 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_DARWIN;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (8 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_COOK;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        } else if (9 == RoleSwitch_taskidx) {
            SysCtrl.aiuserRole = PICID_ROLE_POET;
            functionStart(FUNCTION_TAKE_PHOTO, 0);
        }
        printf("id_idx:%d\n", RoleSwitch_taskidx);
    }
    return 0;
}

static void RoleSwitchIdxBuf(void) {
    printf("Entering RoleSwitchIdxBuf, taskidx=%d\n", RoleSwitch_taskidx);
    if (jpgdecFlag & 0x02) {
        printf("Buffer already decoded for taskidx %d, skipping decode\n", RoleSwitch_taskidx);
        return;
    }
    if (RoleSwitch_id_buf == NULL) {
        printf("Error: Buffer not allocated\n");
        return;
    }
    if (id_w == 0 || id_h == 0) {
        printf("Error: Invalid resolution %dx%d, resetting to default\n", id_w, id_h);
        id_w = SCREEN_WIDTH;
        id_h = SCREEN_HEIGHT;
    }
    printf("Decoding image for taskidx %d\n", RoleSwitch_taskidx);
    jpgdecById(RoleSwitch_bmp_id[RoleSwitch_taskidx], RoleSwitch_id_buf);
    u32 i, y_zero_count = 0, uv_128_count = 0;
    u32 y_size = id_w * id_h;
    u32 uv_size = id_w * id_h / 2;
    for (i = 0; i < y_size; i++) {
        if (RoleSwitch_id_buf[i] == 0) y_zero_count++;
    }
    for (i = y_size; i < y_size + uv_size; i++) {
        if (RoleSwitch_id_buf[i] == 128) uv_128_count++;
    }
    if (y_zero_count == y_size || uv_128_count == uv_size) {
        printf("Warning: Decoded data invalid (Y zeros=%u/%u, UV 128s=%u/%u) for taskidx %d\n",
               y_zero_count, y_size, uv_128_count, uv_size, RoleSwitch_taskidx);
        return;
    }
    dcache_writeback_region((u32)RoleSwitch_id_buf, BUFFER_SIZE);
    jpgdecFlag |= 0x02;
    printf("Updated icon %d\n", RoleSwitch_taskidx);
}

static int RoleSwitchKeyMsgUp(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        u32 current_time = os_time_get();
        if (current_time - last_key_time < KEY_DEBOUNCE_MS) {
            printf("Key up ignored due to debounce\n");
            return 0;
        }
        last_key_time = current_time;
        RoleSwitch_taskidx = (RoleSwitch_taskidx - 1 + ROLESWITCH_TASK_NUM) % ROLESWITCH_TASK_NUM;
        jpgdecFlag &= ~0x02;
        RoleSwitchNameShow(handle, RoleSwitch_taskidx);
        printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, RoleSwitch_taskidx);
        RoleSwitchIdxBuf();
        RoleSwitch_showbuf();
    }
    return 0;
}

static int RoleSwitchKeyMsgDown(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        u32 current_time = os_time_get();
        if (current_time - last_key_time < KEY_DEBOUNCE_MS) {
            printf("Key down ignored due to debounce\n");
            return 0;
        }
        last_key_time = current_time;
        RoleSwitch_taskidx = (RoleSwitch_taskidx + 1) % ROLESWITCH_TASK_NUM;
        jpgdecFlag &= ~0x02;
        RoleSwitchNameShow(handle, RoleSwitch_taskidx);
        printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, RoleSwitch_taskidx);
        RoleSwitchIdxBuf();
        RoleSwitch_showbuf();
    }
    return 0;
}

static int RoleSwitchKeyMsgMenu(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        u32 current_time = os_time_get();
        if (current_time - last_key_time < KEY_DEBOUNCE_MS) {
            printf("Key menu ignored due to debounce\n");
            return 0;
        }
        last_key_time = current_time;
        RoleSwitch_taskidx = (RoleSwitch_taskidx - 1 + ROLESWITCH_TASK_NUM) % ROLESWITCH_TASK_NUM;
        jpgdecFlag &= ~0x02;
        RoleSwitchNameShow(handle, RoleSwitch_taskidx);
        printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, RoleSwitch_taskidx);
        RoleSwitchIdxBuf();
        RoleSwitch_showbuf();
    }
    return 0;
}

static int RoleSwitchKeyMsgMode(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        u32 current_time = os_time_get();
        if (current_time - last_key_time < KEY_DEBOUNCE_MS) {
            printf("Key mode ignored due to debounce\n");
            return 0;
        }
        last_key_time = current_time;
        RoleSwitch_taskidx = (RoleSwitch_taskidx + 1) % ROLESWITCH_TASK_NUM;
        jpgdecFlag &= ~0x02;
        RoleSwitchNameShow(handle, RoleSwitch_taskidx);
        printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, RoleSwitch_taskidx);
        RoleSwitchIdxBuf();
        RoleSwitch_showbuf();
    }
    return 0;
}

static int RoleSwitchOpenWin(winHandle handle, uint32 parameNum, uint32* parame) {
    printf("Role Switch Open Win:%d, modeMem:0x%x, memBase:0x%x\n", os_time_get(), modeFreeSpace(), getCurModeFreeBuffBase());
    printf("Free memory before allocation: %u bytes\n", modeFreeSpace());
    jpgdecFlag = 0;
    RoleSwitch_taskidx = 0;

    id_w = SCREEN_WIDTH;
    id_h = SCREEN_HEIGHT;

    printf("RoleSwitch_taskidx=%d\n", RoleSwitch_taskidx);
    printf("LCD Resolution: %d x %d\n", id_w, id_h);

    u8* base_addr = (getCurModeFreeBuffBase() + 0x3f) & (~0x3f);
    RoleSwitch_id_buf = base_addr;
    if (RoleSwitch_id_buf == NULL) {
        printf("mem err for id_buf!\n");
        return -1;
    }

    RoleSwitchNameShow(handle, RoleSwitch_taskidx);
    printf("First decode for taskidx %d\n", RoleSwitch_taskidx);
    RoleSwitchIdxBuf();
    RoleSwitch_showbuf();

    RoleSwitchStrShow(handle, false);
    RoleSwitchBaterryShow(handle);
    return 0;
}

// static int RoleSwitchCloseWin(winHandle handle, uint32 parameNum, uint32* parame) {
//     printf("Role Switch Close Win!!!\n");
//     if (RoleSwitch_id_buf != NULL) {
//         RoleSwitch_id_buf = NULL;
//     }
//     jpgdecFlag = 0;
//     RoleSwitch_taskidx = 0;
//     id_w = 0;
//     id_h = 0;
//     return 0;
// }

static int RoleSwitchCloseWin(winHandle handle, uint32 parameNum, uint32* parame) {
    printf("Role Switch Close Win!!!\n");
    
    if (RoleSwitch_id_buf != NULL) {
        // 新增：清零缓冲区，防止残留
        memset(RoleSwitch_id_buf, 0, BUFFER_SIZE);
        dcache_writeback_region((u32)RoleSwitch_id_buf, BUFFER_SIZE);  // 刷新缓存
        
        // 可选：清屏，确保LCD重置
        disp_frame_t *p_lcd_buffer = lcdVideoFrameMalloc();
        if (p_lcd_buffer != NULL) {
            memset(p_lcd_buffer->y_addr, 0, p_lcd_buffer->w * p_lcd_buffer->h);
            memset(p_lcd_buffer->y_addr + p_lcd_buffer->w * p_lcd_buffer->h, 128, p_lcd_buffer->w * p_lcd_buffer->h / 2);
            dcache_writeback_region((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
            lcdDealVideBuff(p_lcd_buffer);
            lcdframeFree(p_lcd_buffer);
        }
        
        RoleSwitch_id_buf = NULL;
    }
    
    jpgdecFlag = 0;
    id_w = 0;
    id_h = 0;
    return 0;
}

static int RoleSwitchSysMsgUSB(winHandle handle, uint32 parameNum, uint32* parame) {
    RoleSwitchBaterryShow(handle);
    return 0;
}

static int RoleSwitchSysMsgBattery(winHandle handle, uint32 parameNum, uint32* parame) {
    if (SysCtrl.usb == USB_STAT_NULL)
        RoleSwitchBaterryShow(handle);
    return 0;
}

static int RoleSwitchsysMsgPower(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        printf("Role Switch Close Win!!!\n");
        if (RoleSwitch_id_buf != NULL) {
            RoleSwitch_id_buf = NULL;
        }
        RoleSwitch_taskidx = 0;
        functionStart(FUNCTION_MAINMENU, 0);
    }
    return 0;
}

static int uiRoleSwitchSlidRelease(winHandle handle, uint32 parameNum, uint32* parame) {
    if (parameNum != 1) {
        printf("uiRoleSwitchSlidRelease, parame num error %d\n", parameNum);
        return 0;
    }
    return 0;
}

msgDealInfor RoleSwitchMsgDeal[] = {
    {SYS_OPEN_WINDOW, RoleSwitchOpenWin},
    {SYS_CLOSE_WINDOW, RoleSwitchCloseWin},
    {SYS_SLID_RELEASE, uiRoleSwitchSlidRelease},
    {KEY_EVENT_UP, RoleSwitchKeyMsgUp},
    {KEY_EVENT_DOWN, RoleSwitchKeyMsgDown},
    // {KEY_EVENT_MENU, RoleSwitchKeyMsgMenu},
    // {KEY_EVENT_MODE, RoleSwitchKeyMsgMode},
    {KEY_EVENT_OK, RoleSwitchKeyMsgOk},
    {SYS_EVENT_USB, RoleSwitchSysMsgUSB},
    {SYS_EVENT_BAT, RoleSwitchSysMsgBattery},
    {KEY_EVENT_MODE, RoleSwitchsysMsgPower},
    {EVENT_MAX, NULL},
};

WINDOW(RoleSwitchWindow, RoleSwitchMsgDeal, RoleSwitchWin)
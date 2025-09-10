#include "task/uiWin.h"
#include "mediaFile/picIdentify.h"
#include "task/userMedia.h"
#include "drv/lcd/lcdframe.h"

#define hal_sysMemMalloc(a, b) osal_malloc(a)
#define hal_sysMemFree_sdr(a) osal_free(a)
static u8 player_mp3_Volume[6]= {'V','O','L',':','0','0'};
enum
{
    ID_CONTENT_0_ID = 0,
    ID_CONTENT_1_ID,
    ID_CONTENT_2_ID,
    ID_CONTENT_3_ID,
    ID_CONTENT_MAX_ID,
    VOLUME_NUM_ID,
    SHOWUI_ID
};

#define MAX_CNT_PRE_LINE 30
static unsigned short idContent[(MAX_CNT_PRE_LINE / 2 + 2) * ID_CONTENT_MAX_ID];

// Widget creation information for content window
static widgetCreateInfor idContentWin[] =
{
    createFrameWin(Rx(0), Ry(140), Rw(320), Rh(140), R_COLOR_TBLACK, WIN_ABS_POS),
    createStringIcon(ID_CONTENT_0_ID, Rx(10), Ry(10), Rw(320), Rh(24), strToResId(" "), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createStringIcon(ID_CONTENT_1_ID, Rx(10), Ry(10 + 24 * 1), Rw(320), Rh(24), strToResId(" "), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createStringIcon(ID_CONTENT_2_ID, Rx(10), Ry(10 + 24 * 2), Rw(320), Rh(24), strToResId(" "), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createStringIcon(ID_CONTENT_3_ID, Rx(10), Ry(10 + 24 * 3), Rw(320), Rh(24), strToResId(" "), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    // createStringIcon(VOLUME_NUM_ID,Rx(228),Ry(38),Rw(64),Rh(32),strToResId(player_mp3_Volume),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
    widgetEnd(),
};

static unsigned short tmpStr[2] = {0x0020, 0x0000};

// Widget creation information for Unicode content window
static widgetCreateInfor idContentUniWin[] = {
    createFrameWin(Rx(0), Ry(0), Rw(320), Rh(240), R_COLOR_TRANSFER, WIN_ABS_POS),
    createRect(SHOWUI_ID,Rx(0), Ry(140), Rw(320), Rh(140),R_COLOR_TBLACK),
    createUniStrIcon(ID_CONTENT_0_ID, Rx(10), Ry(145), Rw(310), Rh(34), strToResId((char *)tmpStr), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createUniStrIcon(ID_CONTENT_1_ID, Rx(10), Ry(145 + 34 * 1), Rw(310), Rh(34), strToResId((char *)tmpStr), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createUniStrIcon(ID_CONTENT_2_ID, Rx(10), Ry(145 + 34 * 2), Rw(310), Rh(34), strToResId((char *)tmpStr), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    // createStringIcon(VOLUME_NUM_ID,Rx(5),Ry(0),Rw(64),Rh(32),strToResId(player_mp3_Volume),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
    widgetEnd(),
};

#define ICON_PIXEL_W        60
#define ICON_PIXEL_H        60
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define MAX_RETRIES         5
#define RETRY_DELAY_MS      100

// 绘制120x120图标的函数，适配新库，带重试机制
static void display_icon(u32 icon_res_id, int16_t x_pos, int16_t y_pos) {
    res_img_t icon_img = {0};
    u8 *disp_buf = NULL;
    u16 icon_w, icon_h;
    disp_frame_t *p_lcd_buffer = NULL;
    int retry;

    icon_w = (ICON_PIXEL_W + 0x1) & (~0x1);
    icon_h = (ICON_PIXEL_H + 0x1) & (~0x1);
  //  printf("初始化尺寸: icon_w=%d, icon_h=%d\n", icon_w, icon_h);

    // 重试分配中间缓冲区
    for (retry = 0; retry < MAX_RETRIES; retry++) {
        disp_buf = hal_sysMemMalloc(SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2, 32);
        if (disp_buf) {
          //  printf("中间缓冲区分配成功: disp_buf=%p (尝试 %d)\n", disp_buf, retry + 1);
            break;
        }
      //  printf("中间缓冲区分配失败！ disp_buf=%p (尝试 %d)\n", disp_buf, retry + 1);
        if (retry < MAX_RETRIES - 1) osal_task_sleep(RETRY_DELAY_MS);
    }
    if (!disp_buf) {
        printf("中间缓冲区分配失败，退出！\n");
        goto cleanup;
    }

    // 重试加载图标
    for (retry = 0; retry < MAX_RETRIES; retry++) {
        if (img_load(&icon_img, icon_res_id, 0, ICON_PIXEL_W, ICON_PIXEL_H, 0) == 0) {
         //  printf("图标加载成功: ID=%d, yuvbuf_ptr=%p, width=%d, height=%d (尝试 %d)\n", 
              //     icon_res_id, icon_img.yuvbuf_ptr, icon_img.width, icon_img.height, retry + 1);
            break;
        }
        printf("图标加载失败！ ID=%d (尝试 %d)\n", icon_res_id, retry + 1);
        if (retry < MAX_RETRIES - 1) osal_task_sleep(RETRY_DELAY_MS);
    }
    if (!icon_img.yuvbuf_ptr) {
        printf("图标加载失败，退出！\n");
        goto cleanup;
    }

    // 重试获取LCD帧缓冲区
    for (retry = 0; retry < MAX_RETRIES; retry++) {
        p_lcd_buffer = lcdVideoFrameMalloc();
        if (p_lcd_buffer) {
           // printf("LCD帧缓冲区: y_addr=%p, w=%d, h=%d (尝试 %d)\n", 
             //      p_lcd_buffer->y_addr, p_lcd_buffer->w, p_lcd_buffer->h, retry + 1);
            break;
        }
       // printf("获取LCD帧缓冲区失败！ (尝试 %d)\n", retry + 1);
        if (retry < MAX_RETRIES - 1) osal_task_sleep(RETRY_DELAY_MS);
    }
    if (!p_lcd_buffer) {
        printf("获取LCD帧缓冲区失败，退出！\n");
        goto cleanup;
    }

    lcdframeVideoCfg(p_lcd_buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   // printf("显示区域配置: x=0, y=0, w=%d, h=%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (x_pos == -1 || y_pos == -1) {
        x_pos = (SCREEN_WIDTH - icon_w) / 2;
        y_pos = (SCREEN_HEIGHT - icon_h) / 2;
    }
    if (x_pos < 0 || x_pos > SCREEN_WIDTH - icon_w || y_pos < 0 || y_pos > SCREEN_HEIGHT - icon_h) {
        printf("坐标越界: x=%d, y=%d\n", x_pos, y_pos);
        goto cleanup;
    }
  //  printf("图标显示位置: x=%d, y=%d\n", x_pos, y_pos);

    memcpy(disp_buf, p_lcd_buffer->y_addr, SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2);
   // printf("复制帧缓冲区到中间缓冲区: src=%p, dst=%p, size=%d\n", 
    //       p_lcd_buffer->y_addr, disp_buf, SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2);

    yuv420_draw_buf(disp_buf, SCREEN_WIDTH, SCREEN_HEIGHT, x_pos, y_pos,
                    icon_w, icon_h, icon_img.yuvbuf_ptr, icon_w, icon_h, YUV_ALPHA_Y, YUV_ALPHA_UV);
   // printf("绘制完成: 目标地址=%p, 图标尺寸=%dx%d\n", disp_buf, icon_w, icon_h);

    memcpy(p_lcd_buffer->y_addr, disp_buf, SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2);
    //printf("复制中间缓冲区到帧缓冲区: src=%p, dst=%p, size=%d\n", 
        //   disp_buf, p_lcd_buffer->y_addr, SCREEN_WIDTH * SCREEN_HEIGHT * 3 / 2);

    ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
   // printf("缓存刷新: 地址=%p, 大小=%d\n", p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);

    // 重试LCD就绪检查
    for (retry = 0; retry < MAX_RETRIES; retry++) {
        int cnt = 0;
        while (lcdDeBusy()) {
            osal_task_sleep(10);
            if (cnt++ >= 5) break;
        }
        if (cnt < 5) {
            printf("LCD就绪 (尝试 %d)\n", retry + 1);
            break;
        }
        printf("LCD忙碌超时 (尝试 %d)\n", retry + 1);
        if (retry < MAX_RETRIES - 1) osal_task_sleep(RETRY_DELAY_MS);
    }
    if (retry >= MAX_RETRIES) {
        printf("LCD就绪检查失败，退出！\n");
        goto cleanup;
    }

    lcdDealVideBuff(p_lcd_buffer);
    osal_task_sleep(60);
   // printf("图标显示完成，位置: (%d, %d)\n", x_pos, y_pos);

cleanup:
    if (icon_img.yuvbuf_ptr) img_release(&icon_img);
    if (disp_buf) hal_sysMemFree_sdr(disp_buf);
  //  printf("资源释放: icon_img.yuvbuf_ptr=%p, disp_buf=%p\n", icon_img.yuvbuf_ptr, disp_buf);
}

static void DialogBoxShow(winHandle handle, int show) {
    if (show == PICID_ROLE_DOCTOR) {
        display_icon(RES_ICON_MTDOCTOR, 130, 0);
    } else if (show == PICID_ROLE_TEACHER) {
        display_icon(RES_ICON_MTTEACHER, 130, 0);
    } else if (show == PICID_ROLE_STORYTLLER) {
        display_icon(RES_ICON_MTSTORYTLLER, 130, 0);
    } else if (show == PICID_ROLE_POET) {
        display_icon(RES_ICON_MTPOET, 130, 0);
    } else if (show == PICID_ROLE_NAYSAYER) {
        display_icon(RES_ICON_MTNAYSAYER, 130, 0);
    } else if (show == PICID_ROLE_FLATTERER) {
        display_icon(RES_ICON_MTFLATTERER, 130, 0);
    } else if (show == PICID_ROLE_SOCRATES) {
        display_icon(RES_ICON_MTSOCRATES, 130, 0);
    } else if (show == PICID_ROLE_NEWTON) {
        display_icon(RES_ICON_MTNEWTON, 130, 0);
    } else if (show == PICID_ROLE_DARWIN) {
        display_icon(RES_ICON_MTDARWIN, 130, 0);
    } else if (show == PICID_ROLE_COOK) {
        display_icon(RES_ICON_MTCOOK, 130, 0);
    }
}

static void player_update_volume(winHandle handle)
{
    s8 audio_Volume = MP3_get_volume();
    printf("audio_Volume=%d\n",audio_Volume);
    player_mp3_Volume[4] = (audio_Volume / 10) + '0';
    player_mp3_Volume[5] = (audio_Volume % 10) + '0';
    player_mp3_Volume[6] = 0;
    winSetResid(winItem(handle,VOLUME_NUM_ID),strToResId(player_mp3_Volume));
    winSetVisible(winItem(handle,VOLUME_NUM_ID),true);
}

static char *idContentSkipInvisibleChar(u8*cStr)
{
	u8 *tmp = cStr;
	int i;
	
	if (cStr == NULL)
		return cStr;

	for (i=0; i<strlen(cStr); i++)
	{
		if (*tmp >= 0x20)
			break;

		tmp++;
	}

	return tmp;
}

// UTF-8 解码：返回 Unicode 码点，并更新指针到下一个字符
static uint32_t utf8_to_ucs4(const char **utf8) {
    const uint8_t *p = (const uint8_t *)*utf8;
    uint32_t codepoint = 0;

    if (p[0] <= 0x7F) {  // 1字节 (ASCII)
        codepoint = p[0];
        *utf8 += 1;
    } else if ((p[0] & 0xE0) == 0xC0) {  // 2字节
        codepoint = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        *utf8 += 2;
    } else if ((p[0] & 0xF0) == 0xE0) {  // 3字节（中文字符在此）
        codepoint = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
        *utf8 += 3;
    } else if ((p[0] & 0xF8) == 0xF0) {  // 4字节（生僻字，需要代理对）
        codepoint = ((p[0] & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        *utf8 += 4;
    } else {  // 非法 UTF-8
        *utf8 += 1;
        return 0xFFFD;  // Unicode 替换字符
    }

    return codepoint;
}

static int idContentGetCopyCnt(char *cStr, int maxCnt, int *lastSpacePtr)
{
    if (cStr == NULL)
        return 0;
	int i;
    const char *p = cStr;
    int charCount = 0;
    int lastSpace = -1;
    int byteCount = 0;
    int effectiveMaxCnt = maxCnt < 28 ? maxCnt : 28; // 字符限制为28

    // 遍历字符串，按 UTF-8 解析字符
    while (*p) {
        uint32_t codepoint = utf8_to_ucs4(&p);
        int currentByteCount = p - cStr;

        // 检查不可见字符（除空格外）
        if (codepoint < 0x20 && codepoint != 0x20) {
           // printf("Invisible char U+%04X at byte %d, stopping\n", codepoint, byteCount);
            break;
        }

        // 记录空格位置（仅在有效字符数内）
        if (codepoint == 0x20 && charCount < effectiveMaxCnt) {
            lastSpace = byteCount;
           // printf("Found space at byte %d (char %d)\n", lastSpace, charCount);
        }

        charCount++;
        byteCount = currentByteCount;

        // 超过字符限制且有空格，换行
        if (charCount > effectiveMaxCnt && lastSpace >= 0) {
           // printf("Exceeding effectiveMaxCnt=%d, breaking at space byte=%d\n", effectiveMaxCnt, lastSpace);
            if (lastSpacePtr) *lastSpacePtr = lastSpace;
            return lastSpace + 1; // 包含空格
        }
    }

    // 替换特殊字符（双引号 -> 单引号）
    for (i = 0; i < byteCount && cStr[i] != '\0'; i++) {
        if (cStr[i] == 0x22) {
            cStr[i] = 0x27;
            printf("Replaced quote at byte %d\n", i);
        }
    }

    // 无空格或超限，返回适当字节数
    if (charCount > effectiveMaxCnt) {
       // printf("No space found within effectiveMaxCnt=%d, breaking at byte %d\n", effectiveMaxCnt, byteCount);
        if (lastSpacePtr) *lastSpacePtr = -1;
        return byteCount > effectiveMaxCnt ? effectiveMaxCnt : byteCount;
    }

  //  printf("Returning byte count: %d (char count: %d)\n", byteCount, charCount);
    if (lastSpacePtr) *lastSpacePtr = lastSpace;
    return byteCount;
}

// UTF-16BE 编码：返回写入的字节数（2或4）
static int ucs4_to_utf16be(uint32_t codepoint, uint8_t *utf16be)
{
    if (codepoint <= 0xFFFF) {  // 基本多文种平面（BMP）
        utf16be[1] = (codepoint >> 8) & 0xFF;  // 高字节在前（BE）
        utf16be[0] = codepoint & 0xFF;         // 低字节
        return 2;
    } else {  // 代理对（Surrogate Pair）
        codepoint -= 0x10000;
        uint16_t high = 0xD800 | ((codepoint >> 10) & 0x3FF);  // 高代理
        uint16_t low = 0xDC00 | (codepoint & 0x3FF);           // 低代理
        utf16be[3] = (high >> 8) & 0xFF;
        utf16be[2] = high & 0xFF;
        utf16be[1] = (low >> 8) & 0xFF;
        utf16be[0] = low & 0xFF;
        return 4;
    }
}

// 主转换函数：返回 UTF-16BE 字符串（需调用者 free）
static uint8_t *utf8_to_utf16be(const char *utf8, u32 *out_len)
{
    u32 len = 0;
    const char *p = utf8;
    u32 j, i=0;
	
    // 第一次遍历：计算输出缓冲区大小
    while (*p) {
		i++;
        uint32_t codepoint = utf8_to_ucs4(&p);
        len += (codepoint <= 0xFFFF) ? 2 : 4;
    }
	printf("==u8to16, len:%d, i:%d\n", len, i);
	for (j=0; j<i; j++)
		printf("%x ", *(u8*)(p+j));
	printf("\n==end\n");
    // 分配内存（包含可能的 BOM，此处省略）
    uint8_t *utf16be = (uint8_t *)osal_malloc(len);
    if (!utf16be) return NULL;

    // 第二次遍历：实际转换
    p = utf8;
    uint8_t *dst = utf16be;
    while (*p) {
        uint32_t codepoint = utf8_to_ucs4(&p);
        dst += ucs4_to_utf16be(codepoint, dst);
    }

    *out_len = len;
    return utf16be;
}

static int utf16be_strlen(const uint8_t *str, size_t max_bytes) {
    if (str == NULL || max_bytes % 2 != 0) {
        return (size_t)-1; // 错误：输入无效
    }

    int len = 0;
	int i;
    for (i = 0; i + 1 < max_bytes; i += 2) {
        uint16_t code_unit = (str[i] << 8) | str[i + 1]; // 读取 UTF-16BE 字符
        if (code_unit == 0) {
            break; // 遇到 \0\0 结束
        }
        len+=2;
    }
    return len;
}

static uint8_t *split_utf16be_str(uint8_t *utf16be_str, uint32_t utf16be_len,
                                  uint32_t charsPerLine,
                                  uint8_t *output[ID_CONTENT_MAX_ID],
                                  uint32_t output_len[ID_CONTENT_MAX_ID],
                                  int totalChars, int maxTotalChars) {
    if (utf16be_str == NULL || utf16be_len == 0 || charsPerLine == 0) {
        return NULL;
    }

    uint32_t processed = 0;
    int i = 0;
    const int maxLines = 3;

    for (; i < maxLines && processed < utf16be_len; i++) {
        uint32_t remain = utf16be_len - processed;
        uint32_t chunk_size = 0;
        uint32_t char_count = 0;
        uint32_t byte_index = 0;

        int max_chars = charsPerLine;
        if (i == maxLines - 1) {
            int chars_left = maxTotalChars - totalChars;
            max_chars = chars_left > charsPerLine - 3 ? charsPerLine - 3 : chars_left;
        }

        while (byte_index < remain && char_count < max_chars) {
            uint16_t code_unit = (utf16be_str[processed + byte_index] << 8) |
                                (utf16be_str[processed + byte_index + 1]);
            bool is_surrogate = (code_unit >= 0xD800 && code_unit <= 0xDBFF);
            uint32_t bytes_to_add = is_surrogate ? 4 : 2;

            // 检查是否为书名号 '》' (U+3009) 且角色为诗人
            bool break_at_bookend = false;
            if ((SysCtrl.aiuserRole == PICID_ROLE_POET || SysCtrl.aiuserRole == PICID_ROLE_STORYTLLER) && code_unit == 0x0B30 && !is_surrogate) {
                break_at_bookend = true;
               // printf("Found bookend '》' at byte %d, char %d\n", processed + byte_index, char_count);
            }
            // 添加当前字符
            if (bytes_to_add == 4 && byte_index + 3 >= remain) {
                break; // 防止代理对越界
            }
            chunk_size += bytes_to_add;
            byte_index += bytes_to_add;
            char_count++;

            // 如果遇到书名号，提前结束当前行
            if (break_at_bookend) {
               // printf("Breaking line after bookend, chunk_size: %d\n", chunk_size);
                break;
            }
        }

        if (chunk_size < 2) {
            break;
        }

        output[i] = utf16be_str + processed;
        output_len[i] = chunk_size;
        processed += chunk_size;
        totalChars += char_count;

        // 打印分段内容以调试
       /* printf("Segment %d: ", i);
        int j;
        for (j = 0; j < output_len[i]; j++) {
            printf("%02x ", output[i][j]);
        }
        printf("\n");
		*/
        // 仅当有剩余内容且是最后一行时添加省略号
        if (i == maxLines - 1 && processed < utf16be_len) {
            uint8_t *last_line = output[i];
            static unsigned short ellipsis[] = {0x2026, 0x0000};
            int ellipsis_len = 6;
            if (output_len[i] + ellipsis_len <= MAX_CNT_PRE_LINE) {
                memcpy(last_line + output_len[i], ellipsis, ellipsis_len);
                output_len[i] += ellipsis_len;
                printf("Added ellipsis to last line, new len: %d\n", output_len[i]);
            } else {
                printf("No space for ellipsis\n");
            }
            break;
        }
    }

    return NULL;
}

static char *idContentShow(winHandle handle, char *cStr, int isUtf16be) {
    if (cStr == NULL || strlen(cStr) <= 0)
        return NULL;

    int numPreLine = MAX_CNT_PRE_LINE;
    char *idStr;
    int i;
    int tmpLen;
    int cpyCnt;
    int lastSpace;
    const int ellipsisLen = 3; // 省略号 "..." 占用3字符
    const int maxTotalChars = numPreLine * ID_CONTENT_MAX_ID;

    memset(idContent, 0, sizeof(idContent));
  //  printf("Input cStr: %s\n", cStr);

    if (SysCtrl.ailangid == PICID_LANGID_SCH) {
        uint8_t *output[ID_CONTENT_MAX_ID] = {0};
        uint32_t output_len[ID_CONTENT_MAX_ID] = {0};
        int totalUniChars = 0;
        const int maxTotalUniChars = 13 * 3;

        if (isUtf16be == 0) {
            u32 utf16be_len;
            uint8_t *utf16be_str = utf8_to_utf16be(cStr, &utf16be_len);
            if (!utf16be_str) {
                printf("==utf16be_str is NULL\n");
                return NULL;
            }
            u32 orignalLen = strlen(cStr);
          //  printf("===%s,%d, orignalLen:%d, utf16be_len:%d\n", __FUNCTION__, __LINE__, orignalLen, utf16be_len);

            // 使用临时缓冲区
            char *temp_buf = (char *)osal_malloc(utf16be_len + 2);
            if (!temp_buf) {
                printf("Memory allocation failed for temp_buf\n");
                osal_free(utf16be_str);
                return NULL;
            }
            memset(temp_buf, 0, utf16be_len + 2);
            memcpy(temp_buf, utf16be_str, utf16be_len);
            temp_buf[utf16be_len] = '\0';
            temp_buf[utf16be_len + 1] = '\0';
            osal_free(utf16be_str);

           /* printf("UTF-16BE bytes: ");
            for (i = 0; i < utf16be_len; i++) {
                printf("%02x ", (uint8_t)temp_buf[i]);
            }
            printf("\n");

            printf("====%s,%d, %d, %p\n", __FUNCTION__, __LINE__, utf16be_strlen(temp_buf, 200), temp_buf);
			*/
            uint8_t *remaining = split_utf16be_str(temp_buf, utf16be_strlen(temp_buf, 200), 13, output, output_len, totalUniChars, maxTotalUniChars);
            for (i = 0; i < 3 && output[i] != NULL && output_len[i] > 0; i++) {
                idStr = (char *)idContent + MAX_CNT_PRE_LINE * i;
                tmpLen = output_len[i] > MAX_CNT_PRE_LINE ? MAX_CNT_PRE_LINE : output_len[i];
                memcpy(idStr, output[i], tmpLen);
                idStr[tmpLen] = '\0';
                idStr[tmpLen + 1] = '\0';
               // printf("Setting ID_CONTENT_%d: %p, len: %d\n", i, idStr, tmpLen);
                winSetResid(winItem(handle, ID_CONTENT_0_ID + i), strToResId(idStr));
            }
            for (; i < 3; i++) {
                winSetResid(winItem(handle, ID_CONTENT_0_ID + i), strToResId((char *)tmpStr));
            }
            osal_free(temp_buf);
          //  printf("====%s,%d, i:%d\n", __FUNCTION__, __LINE__, i);
            return NULL;
        }
        // isUtf16be == 1 的逻辑保持不变
    } else {
        char *cur = cStr;
        for (i = 0; i < ID_CONTENT_MAX_ID; i++) {
            cur = idContentSkipInvisibleChar(cur);
            idStr = (char *)idContent + MAX_CNT_PRE_LINE * i;
           // printf("Line %d, cur: %s\n", i, cur);
            if (strlen(cur) > 0) {
                lastSpace = -1;
                cpyCnt = idContentGetCopyCnt(cur, numPreLine, &lastSpace);
              //  printf("Copy count: %d, last space: %d\n", cpyCnt, lastSpace);
                if (cpyCnt > 0) {
                    // 最后一行的省略号处理
                    if (i == ID_CONTENT_MAX_ID - 1 && strlen(cur + cpyCnt) > 0) {
                        // 有剩余文本，添加省略号
                        int maxCopy;
                        if (lastSpace >= 0) {
                            // 在最后一个空格后添加省略号
                            maxCopy = lastSpace + 1; // 包含空格
                            if (maxCopy + ellipsisLen > numPreLine) {
                                maxCopy = numPreLine - ellipsisLen; // 确保不超过28
                            }
                        } else {
                            // 无空格，截断到25字符
                            maxCopy = cpyCnt < (numPreLine - ellipsisLen) ? cpyCnt : (numPreLine - ellipsisLen);
                        }
                        strncpy(idStr, cur, maxCopy);
                        idStr[maxCopy] = '.';
                        idStr[maxCopy + 1] = '.';
                        idStr[maxCopy + 2] = '.';
                        idStr[maxCopy + 3] = '\0';
                      //  printf("Added ellipsis after space=%d, Set ID_CONTENT_%d: %s, len: %d\n", lastSpace, i, idStr, strlen(idStr));
                    } else {
                        // 正常复制
                        strncpy(idStr, cur, cpyCnt);
                        idStr[cpyCnt] = '\0';
                      //  printf("Set ID_CONTENT_%d: %s, len: %d\n", i, idStr, strlen(idStr));
                    }
                    cur += cpyCnt;
                    winSetResid(winItem(handle, ID_CONTENT_0_ID + i), strToResId(idStr));
                } else {
                   // printf("No chars to copy, setting space\n");
                    winSetResid(winItem(handle, ID_CONTENT_0_ID + i), strToResId(" "));
                }
            } else {
               // printf("Empty line, setting space\n");
                winSetResid(winItem(handle, ID_CONTENT_0_ID + i), strToResId(" "));
            }
        }
        
        // 超过4行的内容不显示
        printf("Reached max lines, discarding remaining: %s\n", cur);
        return NULL;	
    }
}


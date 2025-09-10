#include "task/uiWin.h"
#include "mediaFile/picIdentify.h"
#include "mp3_dec_wrap.h"
#include "mp3_filelist.h"

enum
{
    ID_CONTENT_0_ID = 0, // 用于显示 languageword
    ID_CONTENT_1_ID,     // 用于显示 name
    ID_CONTENT_MAX_ID
};

#define MAX_CNT_PRE_LINE 80 // 增加缓冲区，支持长文本
static unsigned short idContent[(MAX_CNT_PRE_LINE / 2 + 2) * ID_CONTENT_MAX_ID];

static unsigned short tmpStr[2] = {0x0020, 0x0000};

// 窗口布局：两行控件，放大字体
static widgetCreateInfor smallModelWin[] =
{
    createFrameWin(Rx(30), Ry(40), Rw(260), Rh(140), R_COLOR_TBLACK, WIN_ABS_POS),
    createUniStrIcon(ID_CONTENT_0_ID, Rx(10), Ry(10), Rw(220), Rh(56), strToResId((char *)tmpStr), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    createUniStrIcon(ID_CONTENT_1_ID, Rx(10), Ry(10 + 56 + 10), Rw(220), Rh(56), strToResId((char *)tmpStr), ALIGNMENT_CENTER, R_COLOR_WHITE, 0),
    widgetEnd(),
};

// 将 UTF-8 字符转换为 UCS-4
static uint32_t utf8_to_ucs4(const char **utf8)
{
    const uint8_t *p = (const uint8_t *)*utf8;
    uint32_t codepoint = 0;

    if (p[0] <= 0x7F) {
        codepoint = p[0];
        *utf8 += 1;
    }
    else if ((p[0] & 0xE0) == 0xC0 && p[1]) {
        codepoint = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        *utf8 += 2;
    }
    else if ((p[0] & 0xF0) == 0xE0 && p[1] && p[2]) {
        codepoint = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
        *utf8 += 3;
    }
    else if ((p[0] & 0xF8) == 0xF0 && p[1] && p[2] && p[3]) {
        codepoint = ((p[0] & 0x07) << 18) | ((p[1] & 0x3F) << 12) | ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        *utf8 += 4;
    }
    else {
        printf("==%s,%d, invalid UTF-8 sequence at %p\n", __FUNCTION__, __LINE__, *utf8);
        *utf8 += 1;
        return 0xFFFD;
    }

    return codepoint;
}

// 将 UCS-4 转换为 UTF-16BE
static int ucs4_to_utf16be(uint32_t codepoint, uint8_t *utf16be)
{
    if (codepoint <= 0xFFFF) {
        utf16be[1] = (codepoint >> 8) & 0xFF;
        utf16be[0] = codepoint & 0xFF;
        return 2;
    }
    else {
        codepoint -= 0x10000;
        uint16_t high = 0xD800 | ((codepoint >> 10) & 0x3FF);
        uint16_t low = 0xDC00 | (codepoint & 0x3FF);
        utf16be[3] = (high >> 8) & 0xFF;
        utf16be[2] = high & 0xFF;
        utf16be[1] = (low >> 8) & 0xFF;
        utf16be[0] = low & 0xFF;
        return 4;
    }
}

// 将 UTF-8 转换为 UTF-16BE
static uint8_t *utf8_to_utf16be(const char *utf8, u32 *out_len)
{
    u32 len = 0;
    const char *p = utf8;

    if (!utf8 || !*utf8) {
        printf("==%s,%d, empty or null UTF-8 input\n", __FUNCTION__, __LINE__);
        *out_len = 0;
        return NULL;
    }

    while (*p)
    {
        uint32_t codepoint = utf8_to_ucs4(&p);
        len += (codepoint <= 0xFFFF) ? 2 : 4;
    }

    uint8_t *utf16be = (uint8_t *)osal_malloc(len);
    if (!utf16be) {
        printf("==%s,%d, memory allocation failed for %u bytes\n", __FUNCTION__, __LINE__, len);
        *out_len = 0;
        return NULL;
    }

    p = utf8;
    uint8_t *dst = utf16be;
    while (*p)
    {
        uint32_t codepoint = utf8_to_ucs4(&p);
        dst += ucs4_to_utf16be(codepoint, dst);
    }

    *out_len = len;
    printf("==%s,%d, UTF-8 to UTF-16BE, input:%s, len:%u\n", __FUNCTION__, __LINE__, utf8, len);
    return utf16be;
}

// 跳过不可见字符（UTF-8 兼容）
static u8 *idContentSkipInvisibleChar(u8 *cStr)
{
    if (cStr == NULL || *cStr == '\0') {
        printf("==%s,%d, null or empty input\n", __FUNCTION__, __LINE__);
        return cStr;
    }

    const u8 *p = cStr;
    while (*p) {
        const u8 *start = p;
        uint32_t codepoint = utf8_to_ucs4(&p);
        if (codepoint >= 0x20) {
            printf("==%s,%d, first visible char at %p, codepoint:0x%04X\n", __FUNCTION__, __LINE__, start, codepoint);
            return (char *)start;
        }
    }

    printf("==%s,%d, all chars invisible, input:%s\n", __FUNCTION__, __LINE__, cStr);
    return cStr + strlen(cStr); // 返回空字符串
}

// 显示 languageword 和 name（统一使用 UTF-16BE）
static void idContentShow(winHandle handle, char *languageword, char *name)
{
    if ((languageword == NULL || strlen(languageword) <= 0) && (name == NULL || strlen(name) <= 0)) {
        printf("==%s,%d, no content to show\n", __FUNCTION__, __LINE__);
        return;
    }
    // 处理 languageword
    if (languageword && strlen(languageword) > 0) {
        languageword = idContentSkipInvisibleChar(languageword);
        u32 utf16be_len;
        uint8_t *utf16be_str = utf8_to_utf16be(languageword, &utf16be_len);
        if (utf16be_str) {
            char *idStr = (char *)idContent;
            int tmpLen = utf16be_len > MAX_CNT_PRE_LINE ? MAX_CNT_PRE_LINE : utf16be_len;
            memcpy(idStr, utf16be_str, tmpLen);
            idStr[tmpLen] = '\0';
            idStr[tmpLen + 1] = '\0';
            winSetResid(winItem(handle, ID_CONTENT_0_ID), strToResId(idStr));
            osal_free(utf16be_str);
        } else {
            winSetResid(winItem(handle, ID_CONTENT_0_ID), strToResId((char *)tmpStr));
        }
    } else {
        winSetResid(winItem(handle, ID_CONTENT_0_ID), strToResId((char *)tmpStr));
    }

    // 处理 name
    if (name && strlen(name) > 0) {
        name = idContentSkipInvisibleChar(name);
        u32 utf16be_len;
        uint8_t *utf16be_str = utf8_to_utf16be(name, &utf16be_len);
        if (utf16be_str) {
            char *idStr = (char *)idContent + MAX_CNT_PRE_LINE;
            int tmpLen = utf16be_len > MAX_CNT_PRE_LINE ? MAX_CNT_PRE_LINE : utf16be_len;
            memcpy(idStr, utf16be_str, tmpLen);
            idStr[tmpLen] = '\0';
            idStr[tmpLen + 1] = '\0';
            winSetResid(winItem(handle, ID_CONTENT_1_ID), strToResId(idStr));
            osal_free(utf16be_str);
        } else {
            winSetResid(winItem(handle, ID_CONTENT_1_ID), strToResId((char *)tmpStr));
        }
    } else {
        winSetResid(winItem(handle, ID_CONTENT_1_ID), strToResId((char *)tmpStr));
    }
}
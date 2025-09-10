#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
//#include <string.h>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

/* 蓝绿色 */
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD_RED     "\033[1m\033[31m"

#define LOG_COLOR_TRACE GREEN
#define LOG_COLOR_DEBUG CYAN
#define LOG_COLOR_INFO WHITE
#define LOG_COLOR_WARN YELLOW
#define LOG_COLOR_ERROR RED
#define LOG_COLOR_FATAL BOLD_RED

#define LOG_LEVEL_TRACE     5
#define LOG_LEVEL_DEBUG     4
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_WARN      2
#define LOG_LEVEL_ERROR     1
#define LOG_LEVEL_FATAL     0

#ifndef LOG_LEVEL
	#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#if 0
//去除__FILE__中的完整路径(因为调用stttchr,所以会增加运行时的cpu)，另外的解决办法是编译选项中c文件不要用绝对路径
extern char *strrchr(const char *s, int c);
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#else
//当前编译环境无法修改__FILE__，暂时改为空吧
#ifndef __FILENAME__ 
#define __FILENAME__ " "
#endif
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#if __cplusplus <= 199711L
#define trace(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_TRACE"[TRACE]""[%s][%s][%d]:"fmt RESET"\n", __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
//C++11,当字符串跟变量连接的时候，必须fmt前后增加一个空格才行
#define trace(fmt, ...)   \
	do{ \
		printf(LOG_COLOR_TRACE"[TRACE][%s][%s][%d]:" fmt RESET "\n", __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	}while(0)
#endif
#else
#define trace(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#if __cplusplus <= 199711L
#define debug(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_DEBUG"[DEBUG][%s][%s][%d]:"fmt RESET"\n",  __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
#define debug(fmt, ...)   \
		do{ \
			printf(LOG_COLOR_DEBUG"[DEBUG][%s][%s][%d]:" fmt RESET "\n",  __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
		}while(0)
#endif
#else
#define debug(fmt, ...)

#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#if __cplusplus <= 199711L
#define info(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_INFO"[INFO ][%s][%s][%d]:"fmt RESET"\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
#define info(fmt, ...)   \
	do{ \
		printf(LOG_COLOR_INFO"[INFO ][%s][%s][%d]:" fmt RESET "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
	}while(0)
#endif
#else
#define info(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#if __cplusplus <= 199711L
#define warning(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_WARN"[WARN ][%s][%s][%d]:"fmt RESET"\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
#define warning(fmt, ...)   \
	do{ \
		printf(LOG_COLOR_WARN"[WARN ][%s][%s][%d]:" fmt RESET "\n",  __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
	}while(0)
#endif
#else
#define warning(fmt, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#if __cplusplus <= 199711L
#define error(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_ERROR"[ERROR][%s][%s][%d]:"fmt RESET"\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
#define error(fmt, ...)   \
    do{ \
        printf(LOG_COLOR_ERROR"[ERROR][%s][%s][%d]:%s" fmt RESET "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)
#endif
#else
#define error(fmt, ...)
#endif


//dump一段内存区域的内容
//tag : 每行的前缀字符串，如无特殊需要，为空字符串""就可以.
//bpl : 每行的长度
//data: 打印的数据缓冲区
//len : 打印的数据缓冲区长度
//示例: debug_hexdump("", 0x10, buf, 64);
#define debug_hexdump(tag, bpl, data, len)                                                 \
    ({                                                                                     \
        char _hex[(bpl)*3 + 1];                                                            \
        char _str[(bpl) + 1];                                                              \
        int _i;                                                                            \
        for (_i = 0; _i < (len); _i++)                                                     \
        {                                                                                  \
            unsigned char byte = ((char *)(data))[_i];                                              \
            snprintf(_hex + _i % (bpl)*3, 4, "%02X ", byte);                               \
            _str[_i % (bpl)] = (byte >= '!' && byte <= '~') ? byte : '.';                  \
            if ((_i + 1) % (bpl) == 0 || _i == (len)-1)                                    \
            {                                                                              \
                _str[_i % (bpl) + 1] = '\0';                                               \
                debug("%04X %s %-*s  %s", _i / (bpl) * (bpl), (tag), (bpl)*3, _hex, _str); \
            }                                                                              \
        }                                                                                  \
    })

#ifdef __cplusplus
}
#endif

#endif //_LOG_H_

#ifndef __LWIP_CC_H__
#define __LWIP_CC_H__

typedef unsigned char u8_t; // 基本数据类型设置
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned int u32_t;
typedef signed int s32_t;
typedef unsigned int mem_ptr_t;

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

//
// Setup PACKing macros for GCC Tools
//
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#ifdef DEBUG
extern void __error__(char *pcFilename, unsigned long ulLine);
#define LWIP_PLATFORM_ASSERT(expr)   \
                                     \
  {                                  \
    if (!(expr))                     \
    {                                \
      __error__(__FILE__, __LINE__); \
    }                                \
  }
#else
#define LWIP_PLATFORM_ASSERT(expr)
#endif

#endif /* __LWIP_CC_H__ */
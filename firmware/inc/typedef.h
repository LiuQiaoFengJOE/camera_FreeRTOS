#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __MINGW32__
#include <windows.h>
#else

#ifndef BYTE
#define BYTE uint8_t
#endif

#ifndef WORD
#define WORD uint16_t
#endif

#ifndef WCHAR
#define WCHAR uint16_t
#endif

#ifndef DWORD
#define DWORD uint32_t
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef UCHAR
#define UCHAR uint8_t
#endif

#ifndef CHAR
#define CHAR int8_t
#endif

#ifndef PUCHAR
#define PUCHAR uint8_t *
#endif

#ifndef PCHAR
#define PCHAR int8_t *
#endif

#ifndef INT
#define INT int
#endif

#ifndef UINT
#define UINT unsigned int
#endif


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#endif /* __MINGW32__ */

/* <linux/stddef.h>> */
#ifndef NULL
#define NULL ((void *)0)
#endif

/* <asm-generic/int-ll64.h> */
typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;
typedef __signed__ long long __s64;
typedef unsigned long long __u64;

/* <asm/types.h> */
typedef unsigned int dma_addr_t;
typedef unsigned long long dma64_addr_t;

typedef unsigned char	u8, U8, uint8, UINT8,INT8U,u8_t;
typedef signed   char	s8, S8, int8, INT8,INT8S,s8_t;

typedef unsigned short	u16, U16, uint16, UINT16,INT16U,u16_t;
typedef signed   short	s16, S16, int16, INT16,INT16S,s16_t;

typedef unsigned int	u32, U32, uint32, UINT32,INT32U,u32_t;
typedef signed   int	s32, S32, int32, INT32,INT32S,s32_t;

typedef unsigned long long	u64, U64, uint64, UINT64,INT64U,u64_t;
typedef signed long long	s64, S64, int64, INT64,INT64S,s64_t;
typedef  unsigned char    color_t;

/* <linux/posix_types.h> */
// typedef unsigned long __kernel_ino_t;
// typedef unsigned int __kernel_mode_t;
// typedef unsigned long __kernel_nlink_t;
// typedef long __kernel_off_t;
// typedef int __kernel_pid_t;
// typedef int __kernel_ipc_pid_t;
// typedef unsigned int __kernel_uid_t;
// typedef unsigned int __kernel_gid_t;
// typedef unsigned int __kernel_size_t;
// typedef int __kernel_ssize_t;
// typedef int __kernel_ptrdiff_t;
// typedef long __kernel_time_t;
// typedef long __kernel_suseconds_t;
// typedef long __kernel_clock_t;
// typedef int __kernel_timer_t;
// typedef int __kernel_clockid_t;
// typedef int __kernel_daddr_t;
// typedef char* __kernel_caddr_t;
// typedef unsigned short __kernel_uid16_t;
// typedef unsigned short __kernel_gid16_t;
// typedef unsigned int __kernel_uid32_t;
// typedef unsigned int __kernel_gid32_t;
// typedef long long __kernel_loff_t;

/* <linux/types.h> */
// typedef __u32 __kernel_dev_t;

// #ifdef dev_t
// #undef dev_t
// typedef __kernel_dev_t dev_t;
// #endif
//
// #ifdef ino_t
// #undef ino_t
// typedef __kernel_ino_t ino_t;
// #endif
//
// typedef __kernel_mode_t mode_t;
//
// #ifdef nlink_t
// #undef nlink_t
// typedef __kernel_nlink_t nlink_t;
// #endif
//
// typedef __kernel_off_t off_t;
// typedef __kernel_pid_t pid_t;
// typedef __kernel_daddr_t daddr_t;
// typedef __kernel_suseconds_t suseconds_t;
//
// #ifdef timer_t
// #undef timer_t
// typedef __kernel_timer_t timer_t;
// #endif
//
// #ifdef clockid_t
// #undef clockid_t
// typedef __kernel_clockid_t clockid_t;
// #endif

// #ifdef bool
// #undef bool
// typedef BOOL bool;
// #endif
//
// #ifdef uid_t
// #undef uid_t
// typedef __kernel_uid32_t uid_t;
// #endif
//
// #ifdef gid_t
// #undef gid_t
// typedef __kernel_gid32_t gid_t;
// #endif
//
// typedef __kernel_uid16_t uid16_t;
// typedef __kernel_gid16_t gid16_t;
//
// typedef unsigned long uintptr_t;
// typedef __kernel_loff_t loff_t;

/*
 * The following typedefs are also protected by individual ifdefs for
 * historical reasons:
 */
// typedef __kernel_size_t            size_t;
// typedef __kernel_ssize_t        ssize_t;
// typedef __kernel_ptrdiff_t        ptrdiff_t;
// typedef __kernel_size_t            size_t;
// conflict with lwip in wifi

// #ifndef size_t
////#define size_t unsigned long
// #endif
//
// #ifndef SSIZE_T
// #define SSIZE_T
//
// #ifndef ssize_t
////#define ssize_t int
// #endif
//
// #endif
//// typedef __kernel_ptrdiff_t        ptrdiff_t;
// #define prtdiff_t int
//
// #if 0 // ndef __time_t_defined
// typedef __kernel_time_t            time_t;
// #endif
//
// #ifdef clock_t
// #undef clock_t
// typedef __kernel_clock_t clock_t;
// #endif
//
// typedef __kernel_caddr_t caddr_t;

/* bsd */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

/* sysv */
typedef unsigned char unchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef signed int sint;

typedef __u8 u_int8_t;
typedef __u16 u_int16_t;
typedef __u32 u_int32_t;

typedef __u64 u_int64_t;

/*
 * Below are truly Linux-specific types that should never collide with
 * any application/library that wants linux/types.h.
 */
typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;
typedef __u16 __sum16;
typedef __u32 __wsum;

/* this is a special 64bit data type that is 8-byte aligned */
#define __aligned_u64 __u64 __attribute__((aligned(8)))
#define __aligned_be64 __be64 __attribute__((aligned(8)))
#define __aligned_le64 __le64 __attribute__((aligned(8)))

typedef unsigned gfp_t;
typedef unsigned fmode_t;

typedef u32 phys_addr_t;
typedef phys_addr_t resource_size_t;

// typedef struct {
//   int counter;
// } atomic_t;

// typedef atomic_t atomic_long_t;
// #define ATOMIC_LONG_INIT(i) ATOMIC_INIT(i)

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

/**/
#define SETB(REG, BIT) ((REG) |= (1UL << (BIT)))
#define CLRB(REG, BIT) ((REG) &= (~(1UL << (BIT))))
#define XRLB(REG, BIT) ((REG) ^= (1UL << (BIT)))
#define CHKB(REG, BIT) (REG & (1UL << BIT))
#define BIT_CK_1(REG, BIT) (REG & (1UL << BIT))
#define BIT_CK_0(REG, BIT) (!(REG & (1UL << BIT)))

#define MALIG(d,n)   (((d)+(n-1))&(~(n-1)))

#endif
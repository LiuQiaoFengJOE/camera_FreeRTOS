#ifndef _DEBUG_H
#define _DEBUG_H

// #include <stdio.h>
#include "typedef.h"
#include "osal/osal_time.h"

__attribute__((format(printf, 1, 2))) int printf(const char *fmt, ...);

extern int dbg_level;

#define SYS_MONITOR 0
#define RELEASE_VERSION 0

#if RELEASE_VERSION
// #undef DEBUG
#endif
unsigned int os_time_get(void);
#ifdef DEBUG
#define ERR_PRINT_TIME                             \
  do                                               \
  {                                                \
    uint32_t t = os_time_get();                    \
    printf("[E: %lu.%03lu] ", t / 1000, t % 1000); \
  } while (0)
#define WARN_PRINT_TIME                          \
  do                                             \
  {                                              \
    uint32_t t = os_time_get();                  \
    printf("[W: %u.%03u] ", t / 1000, t % 1000); \
  } while (0)
#define DBG_PRINT_TIME                           \
  do                                             \
  {                                              \
    uint32_t t = os_time_get();                  \
    printf("[D: %u.%03u] ", t / 1000, t % 1000); \
  } while (0)
#define INFO_PRINT_TIME                          \
  do                                             \
  {                                              \
    uint32_t t = os_time_get();                  \
    printf("[I: %u.%03u] ", t / 1000, t % 1000); \
  } while (0)
#define VERB_PRINT_TIME                          \
  do                                             \
  {                                              \
    uint32_t t = os_time_get();                  \
    printf("[V: %u.%03u] ", t / 1000, t % 1000); \
  } while (0)
#define p_info(...)      \
  do                     \
  {                      \
    if (!dbg_level)      \
      break;             \
    INFO_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_verb(...)      \
  do                     \
  {                      \
    if (!dbg_level)      \
      break;             \
    VERB_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_warn(...)      \
  do                     \
  {                      \
    WARN_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_err(...)       \
  do                     \
  {                      \
    ERR_PRINT_TIME;      \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_dbg_track                            \
  do                                           \
  {                                            \
    if (!dbg_level)                            \
      break;                                   \
    DBG_PRINT_TIME;                            \
    printf("%s,%d", __FUNCTION__, __LINE__, ); \
    printf(" \n");                             \
  } while (0)
#define p_dbg(...)       \
  do                     \
  {                      \
    if (!dbg_level)      \
      break;             \
    DBG_PRINT_TIME;      \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_dbg_enter                     \
  do                                    \
  {                                     \
    if (!dbg_level)                     \
      break;                            \
    DBG_PRINT_TIME;                     \
    printf("enter %s\n", __FUNCTION__); \
    printf(" \n");                      \
  } while (0)
#define p_dbg_exit                     \
  do                                   \
  {                                    \
    if (!dbg_level)                    \
      break;                           \
    DBG_PRINT_TIME;                    \
    printf("exit %s\n", __FUNCTION__); \
    printf(" \n");                     \
  } while (0)
#define p_dbg_status               \
  do                               \
  {                                \
    if (!dbg_level)                \
      break;                       \
    DBG_PRINT_TIME;                \
    printf("status %d\n", status); \
    printf(" \n");                 \
  } while (0)
#define p_err_miss                     \
  do                                   \
  {                                    \
    ERR_PRINT_TIME;                    \
    printf("%s miss\n", __FUNCTION__); \
    printf(" \n");                     \
  } while (0)
#define p_err_mem                         \
  do                                      \
  {                                       \
    ERR_PRINT_TIME;                       \
    printf("%s mem err\n", __FUNCTION__); \
    printf(" \n");                        \
  } while (0)
#define p_err_fun                                     \
  do                                                  \
  {                                                   \
    ERR_PRINT_TIME;                                   \
    printf("%s err in %d\n", __FUNCTION__, __LINE__); \
    printf(" \n");                                    \
  } while (0)
#define dump_hex(tag, buff, size)                           \
  do                                                        \
  {                                                         \
    int dump_hex_i;                                         \
    if (!dbg_level)                                         \
      break;                                                \
    printf("%s[", tag);                                     \
    for (dump_hex_i = 0; dump_hex_i < size; dump_hex_i++)   \
    {                                                       \
      printf("%02x ", ((unsigned char *)buff)[dump_hex_i]); \
    }                                                       \
    printf("] \n");                                         \
                                                            \
  } while (0)
#else
#define ERR_PRINT_TIME
#define WARN_PRINT_TIME
#define DBG_PRINT_TIME
#define INFO_PRINT_TIME
#define VERB_PRINT_TIME
#define p_info(...)      \
  do                     \
  {                      \
    INFO_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_verb(...)      \
  do                     \
  {                      \
    VERB_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_warn(...)      \
  do                     \
  {                      \
    WARN_PRINT_TIME;     \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_err(...)       \
  do                     \
  {                      \
    ERR_PRINT_TIME;      \
    printf(__VA_ARGS__); \
    printf(" \n");       \
  } while (0)
#define p_err_miss                     \
  do                                   \
  {                                    \
    ERR_PRINT_TIME;                    \
    printf("%s miss\n", __FUNCTION__); \
    printf(" \n");                     \
  } while (0)
#define p_err_mem                         \
  do                                      \
  {                                       \
    ERR_PRINT_TIME;                       \
    printf("%s mem err\n", __FUNCTION__); \
    printf(" \n");                        \
  } while (0)
#define p_err_fun                                     \
  do                                                  \
  {                                                   \
    ERR_PRINT_TIME;                                   \
    printf("%s err in %d\n", __FUNCTION__, __LINE__); \
    printf(" \n");                                    \
  } while (0)

#define p_dbg_track
#define p_dbg(...)
#define p_dbg_exit
#define p_dbg_enter
#define p_dbg_status

#define dump_hex(tag, buff, size)

#define debg(...) printf(__VA_ARGS__)

#endif

#define p_hex(X, Y) dump_hex("", (unsigned char *)X, Y)

#define assert(x)                                                \
                                                                 \
  do                                                             \
  {                                                              \
                                                                 \
    if (!(x))                                                    \
                                                                 \
    {                                                            \
                                                                 \
      p_err("%s:%d assert " #x "failed \n", __FILE__, __LINE__); \
                                                                 \
      while (1)                                                  \
        ;                                                        \
    }                                                            \
                                                                 \
  } while (0)

#if SYS_MONITOR

typedef enum
{
  PRINT_TYPE_D,
  PRINT_TYPE_H,
  PRINT_TYPE_P_D,
  PRINT_TYPE_P_H,
} STAT_PRINT_TYPE;

struct dev_monitor_item
{
  const char *name;
  uint32_t value;
  //  STAT_PRINT_TYPE type;
};

#define INC_MONITER_ERR_VALUE(x) (x->err_cnt)++
#define ADD_MONITER_READ_VALUE(x, v) (x->read_total_size) += (v)
#define ADD_MONITER_WRITE_VALUE(x, v) (x->write_total_size) += (v)

#define INC_MONITOR_VALUE(X) X.value++
#define ADD_MONITOR_VALUE(X, V) X.value += V
#define SET_MONITER_VALUE(X, V) X.value = V

#define DECLARE_MONITOR_ITEM(TAG, NAME) \
  struct dev_monitor_item NAME          \
      __attribute__((section("monitor_item_table"))) = {TAG, 0}

#define EXTERN_MONITOR_ITEM(NAME) extern struct dev_monitor_item NAME;

#define INC_MONITOR_ITEM_VALUE(X) X.value++
#define DEC_MONITOR_ITEM_VALUE(X) X.value--
#define SET_MONITOR_ITEM_VALUE(X, V) X.value = V
#define ADD_MONITOR_ITEM_VALUE(X, V) X.value += V
#define SUB_MONITOR_ITEM_VALUE(X, V) X.value -= V
#define GET_MONITOR_ITEM_VALUE(X) X.value

#else
#define DECLARE_MONITOR_ITEM(TAG, NAME)
#define INC_MONITOR_ITEM_VALUE(X)
#define DEC_MONITOR_ITEM_VALUE(X)
#define SET_MONITOR_ITEM_VALUE(X, V)
#define ADD_MONITOR_ITEM_VALUE(X, V)
#define SUB_MONITOR_ITEM_VALUE(X, V)

#define INC_MONITER_ERR_VALUE(x)
#define ADD_MONITER_READ_VALUE(x, v)
#define ADD_MONITER_WRITE_VALUE(x, v)

#define INC_MONITOR_VALUE(X)
#define ADD_MONITOR_VALUE(X, V)
#define SET_MONITER_VALUE(X, V)
#define GET_MONITOR_ITEM_VALUE(X)
#define EXTERN_MONITOR_ITEM(NAME)

#endif

void send_test_pkg(int cmd, const void *data, int size);
void dev_monitor_task(void *arg);
void switch_dbg(void);
void init_monitor(void);

#endif

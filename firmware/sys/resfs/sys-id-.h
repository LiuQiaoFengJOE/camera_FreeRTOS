/**@file sys-id.h
 * @brief System pre-defined ID
 * @note
 * System-Engineers        can not #include sysid.h directly, uses sys-id.h instead.
 * Application-Engineers   can not #include sysid.h directly, uses sys-id.h instead.
 * Application-Engineers                                 can not modify sysid.h!
 * Configuration-Engineers can     #include sysid.h, but can not modify sysid.h!
 * Configuration-Engineers can reference sys-id.h to define the following macros.
 */

#ifndef __SYSID_IN_SDK__
#define __SYSID_IN_SDK__

////////////////////////////////////////////////////////////////////////////////
#define SYS_RESID() enum \
{
#define DEF_RESID(n) n,
#define SET_RESID(n, v) n = v,
#define END_RESID(n, reserved)     \
  __##n, /* Get the last number */ \
      n = __##n + reserved - 1     \
  }                                \
  ;
#define USR_RESID(m) enum \
{                         \
  __##m##__ = m,

////////////////////////////////////////////////////////////////////////////////
#define SYS_FONTID() enum \
{
#define DEF_FONTID(n) n,
#define SET_FONTID(n, v) n = v,
#define END_FONTID(n, reserved)    \
  __##n, /* Get the last number */ \
      n = __##n + reserved - 1     \
  }                                \
  ;
#define USR_FONTID(m) enum \
{                          \
  __##m##__ = m,

////////////////////////////////////////////////////////////////////////////////
#define SYS_COLORID() enum \
{
#define DEF_COLORID(n) n,
#define SET_COLORID(n, v) n = v,
#define END_COLORID(n, reserved)   \
  __##n, /* Get the last number */ \
      n = __##n + reserved - 1     \
  }                                \
  ;

////////////////////////////////////////////////////////////////////////////////
#define SYS_MSGID() enum \
{
#define DEF_MSGID(n) n,
#define SET_MSGID(n, v) n = v,
#define END_MSGID(n, reserved)     \
  __##n, /* Get the last number */ \
      n = __##n + reserved - 1     \
  }                                \
  ;
#define USR_MSGID(m) enum \
{                         \
  __##m##__ = m,

////////////////////////////////////////////////////////////////////////////////
#define SYS_CTRLID() enum \
{
#define DEF_CTRLID(n) n,
#define SET_CTRLID(n, v) n = v,
#define END_CTRLID(n, reserved)    \
  __##n, /* Get the last number */ \
      n = __##n + reserved - 1     \
  }                                \
  ;

/* Finally #include sysid.h here. */
#include "sysid.h"
#endif /* __SYSID_IN_SDK__ */

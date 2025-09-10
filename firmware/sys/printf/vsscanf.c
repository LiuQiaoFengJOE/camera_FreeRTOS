/**@file vsscanf.c
 * @brief A not too simple vsscanf implementation downloaded from network
 */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "typedef.h"

#include "board_config.h"
#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152)||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158) // 支持sv6030
//#include "rtos.h"
#endif

#define TOLOWER(X) tolower(X)
#define USHRT_MAX ((u16)(~0U))
#define SHRT_MAX ((s16)(USHRT_MAX >> 1))

static char *skip_spaces(char *s)
{
     while (isspace(*s))
          ++s;
     return s;
}

static int skip_atoi(const char **s)
{
     int i = 0;

     while (isdigit(**s))
          i = i * 10 + *((*s)++) - '0';
     return i;
}

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152)||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)||(WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101) // 支持sv6030

static unsigned int simple_guess_base(const char *cp)
{
     if (cp[0] == '0')
     {
          if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
               return 16;
          else
               return 8;
     }
     else
     {
          return 10;
     } // tolower
}
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
#define TOLOWER(X) tolower(X)
static unsigned int simple_guess_base(const char *cp)
{
     if (cp[0] == '0')
     {
          if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
               return 16;
          else
               return 8;
     }
     else
     {
          return 10;
     } // tolower
}

#endif

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101)

unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base)
{
     unsigned long long result = 0;

     if (!base)
          base = simple_guess_base(cp);

     if (base == 16 && cp[0] == '0' && TOLOWER(cp[1]) == 'x')
          cp += 2;

     while (isxdigit(*cp))
     { // 检查当前cp是否是个十六进制数值，不是直接返回0
          unsigned int value;

          value = isdigit(*cp) ? *cp - '0' : TOLOWER(*cp) - 'a' + 10;
          if (value >= base)
               break;
          result = result * base + value;
          cp++;
     }
     if (endp)
          *endp = (char *)cp;

     return result;
}

unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base)
{
     unsigned long result = 0, value;
     // 对于base，首先如果*cp以0x开头且下来是数字，则一定为16进制。然后如果用户指定了一个非0的进制，则
     // 遵循用户输入的进制。如果用户输入了0进制，则根据是否0开头来确定是8进制还是10进制
     if (*cp == '0')
     {
          cp++;
          if ((*cp == 'x') && isxdigit(cp[1]))
          {
               base = 16; // 如果cp是0x开头的，下面一个又是数字，那么base一定为16。
               cp++;      // 此时即使用户输入了一个base，也忽略这个base而强制其为16。
          }
          if (!base)
          {
               base = 8; // 如果用户输入base为0，cp是0开头，接下来不是x，那么强制为8进制
          }
     }
     if (!base)
     {
          base = 10; // 如果用户输入base为0，cp是非0开头，则使用10进制。
     }
     // while循环里value
     // 第一个不是数字的字母。这个字母的范围取决于base，譬如如果是16进制那么f也算是数字。
     // 而如果是8进制那么9都不算是数字了。因此解析结束的范围只能和base比较来限定。
     while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp) ? toupper(*cp) : *cp) - 'A' + 10) < base)
     {
          result = result * base + value;
          cp++;
     }
     if (endp)                // 此处if判断的作用是：用户在使用这个函数时，如果不关心endp则可以直接
          *endp = (char *)cp; // 使用null,而不必担心程序运行会出错。这种处理技巧很实用，用户可以
     return result;           // 自行决定是否使用这个函数提供的参数式返回值。
}
#endif

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long long simple_strtoll(const char *cp, char **endp, unsigned int base)
{
     if (*cp == '-')
          return -simple_strtoull(cp + 1, endp, base);

     return simple_strtoull(cp, endp, base);
}

long simple_strtol(const char *cp, char **endp, unsigned int base)
{
     if (*cp == '-')
          return -simple_strtoul(cp + 1, endp, base);

     return simple_strtoul(cp, endp, base);
}

/* Convert a character to lower case */
/* Convert a character to lower case */
__inline__ static char
_tolower(char c)
{
     if ((c >= 'A') && (c <= 'Z'))
     {
          c = (c - 'A') + 'a';
     }
     return c;
}

/**
 * vsscanf - Unformat a buffer into a list of arguments
 * @buf:     input buffer
 * @fmt:     format of buffer
 * @args:     arguments
 */
int vsscanf(const char *buf, const char *fmt, va_list args)
{
     const char *str = buf;
     char *next;
     char digit;
     int num = 0;
     u8 qualifier;
     u8 base;
     s16 field_width;
     char is_sign;

     while (*fmt && *str)
     {
          /* skip any white space in format */
          /* white space in format matchs any amount of
           * white space, including none, in the input.
           */
          if (isspace(*fmt))
          {
               fmt = skip_spaces(++fmt);
               str = skip_spaces(str);
          }

          /* anything that is not a conversion must match exactly */
          if (*fmt != '%' && *fmt)
          {
               if (*fmt++ != *str++)
                    break;
               continue;
          }

          if (!*fmt)
               break;
          ++fmt;

          /* skip this conversion.
           * advance both strings to next white space
           */
          if (*fmt == '*')
          {
               while (!isspace(*fmt) && *fmt != '%' && *fmt)
                    fmt++;
               while (!isspace(*str) && *str)
                    str++;
               continue;
          }

          /* get field width */
          field_width = -1;
          if (isdigit(*fmt))
               field_width = skip_atoi(&fmt);

          /* get conversion qualifier */
          qualifier = -1;
          if (*fmt == 'h' || _tolower(*fmt) == 'l' ||
              _tolower(*fmt) == 'z')
          {
               qualifier = *fmt++;
               if (qualifier == *fmt)
               {
                    if (qualifier == 'h')
                    {
                         qualifier = 'H';
                         fmt++;
                    }
                    else if (qualifier == 'l')
                    {
                         qualifier = 'L';
                         fmt++;
                    }
               }
          }

          if (!*fmt || !*str)
               break;

          base = 10;
          is_sign = 0;

          switch (*fmt++)
          {
          case 'c':
          {
               char *s = (char *)va_arg(args, char *);
               if (field_width == -1)
                    field_width = 1;
               do
               {
                    *s++ = *str++;
               } while (--field_width > 0 && *str);
               num++;
          }
               continue;
          case 's':
          {
               char *s = (char *)va_arg(args, char *);
               if (field_width == -1)
                    field_width = SHRT_MAX;
               /* first, skip leading white space in buffer */
               str = skip_spaces(str);

               /* now copy until next white space */
               while (*str && !isspace(*str) && field_width--)
                    *s++ = *str++;
               *s = '\0';
               num++;
          }
               continue;
          case 'n':
               /* return number of characters read so far */
               {
                    int *i = (int *)va_arg(args, int *);
                    *i = str - buf;
               }
               continue;
          case 'o':
               base = 8;
               break;
          case 'x':
          case 'X':
               base = 16;
               break;
          case 'i':
               base = 0;
          case 'd':
               is_sign = 1;
          case 'u':
               break;
          case '%':
               /* looking for '%' in str */
               if (*str++ != '%')
                    return num;
               continue;
          default:
               /* invalid format; stop here */
               return num;
          }

          /* have some sort of integer conversion.
           * first, skip white space in buffer.
           */
          str = skip_spaces(str);

          digit = *str;
          if (is_sign && digit == '-')
               digit = *(str + 1);

          if (!digit || (base == 16 && !isxdigit(digit)) || (base == 10 && !isdigit(digit)) || (base == 8 && (!isdigit(digit) || digit > '7')) || (base == 0 && !isdigit(digit)))
               break;

          switch (qualifier)
          {
          case 'H': /* that's 'hh' in format */
               if (is_sign)
               {
                    signed char *s = (signed char *)va_arg(args, signed char *);
                    *s = (signed char)simple_strtol(str, &next, base);
               }
               else
               {
                    unsigned char *s = (unsigned char *)va_arg(args, unsigned char *);
                    *s = (unsigned char)simple_strtoul(str, &next, base);
               }
               break;
          case 'h':
               if (is_sign)
               {
                    short *s = (short *)va_arg(args, short *);
                    *s = (short)simple_strtol(str, &next, base);
               }
               else
               {
                    unsigned short *s = (unsigned short *)va_arg(args, unsigned short *);
                    *s = (unsigned short)simple_strtoul(str, &next, base);
               }
               break;
          case 'l':
               if (is_sign)
               {
                    long *l = (long *)va_arg(args, long *);
                    *l = simple_strtol(str, &next, base);
               }
               else
               {
                    unsigned long *l = (unsigned long *)va_arg(args, unsigned long *);
                    *l = simple_strtoul(str, &next, base);
               }
               break;
          case 'L':
               if (is_sign)
               {
                    long long *l = (long long *)va_arg(args, long long *);
                    *l = simple_strtoll(str, &next, base);
               }
               else
               {
                    unsigned long long *l = (unsigned long long *)va_arg(args, unsigned long long *);
                    *l = simple_strtoull(str, &next, base);
               }
               break;
          case 'Z':
          case 'z':
          {
               size_t *s = (size_t *)va_arg(args, size_t *);
               *s = (size_t)simple_strtoul(str, &next, base);
          }
          break;
          default:
               if (is_sign)
               {
                    int *i = (int *)va_arg(args, int *);
                    *i = (int)simple_strtol(str, &next, base);
               }
               else
               {
                    unsigned int *i = (unsigned int *)va_arg(args, unsigned int *);
                    *i = (unsigned int)simple_strtoul(str, &next, base);
               }
               break;
          }
          num++;

          if (!next)
               break;
          str = next;
     }

     /*
      * Now we've come all the way through so either the input string or the
      * format ended. In the former case, there can be a %n at the current
      * position in the format that needs to be filled.
      */
     if (*fmt == '%' && *(fmt + 1) == 'n')
     {
          int *p = (int *)va_arg(args, int *);
          *p = str - buf;
     }

     return num;
}
/**
 * sscanf - Unformat a buffer into a list of arguments
 * @buf:     input buffer
 * @fmt:     formatting of buffer
 * @...:     resulting arguments
 */
int sscanf(const char *buf, const char *fmt, ...)
{
     va_list args;
     int i;

     va_start(args, fmt);
     i = vsscanf(buf, fmt, args);
     va_end(args);

     return i;
}

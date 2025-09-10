#ifndef _LIBICONV_CFG_H
#define _LIBICONV_CFG_H



/* Define to 1 to enable a few rarely used encodings. */
#undef ENABLE_EXTRA

/* Define to 1 if the package shall run at any location in the filesystem. */
#undef ENABLE_RELOCATABLE

/* Define to a type if <wchar.h> does not define. */
#undef mbstate_t

/* Define if you have <iconv.h>, the iconv_t type, and the   iconv_open, iconv, iconv_close functions. */
#undef HAVE_ICONV

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST const

/* Define to 1 if you have the getc_unlocked() function. */
#undef HAVE_GETC_UNLOCKED

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#undef HAVE_LANGINFO_CODESET

/* Define if you have the mbrtowc() function. */
#undef HAVE_MBRTOWC

/* Define to 1 if you have the setlocale() function. */
#undef HAVE_SETLOCALE

/* Define to 1 if you have the <stddef.h> header file. */
#undef HAVE_STDDEF_H

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define if you have the wcrtomb() function. */
#undef HAVE_WCRTOMB

/* Define if the machine's byte ordering is little endian. */
#undef WORDS_LITTLEENDIAN
#define WORDS_LITTLEENDIAN

/* Define to the value of ${prefix}, as a string. */
#undef INSTALLPREFIX



//======for platfarm=======
#include "typedef.h"
#include "../../inc/platform_if.h"


#endif


%{
/*
 * Copyright (C) 2004 Nathan Lutchansky <lutchann@litech.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string.h>

#include "conf_parse.h"
#include "osal/osal_alloc.h"
#include "debug.h"

/* reentrant兼容設計：
 * - 如果有-R選項，則這段代碼前面定義了YY_TYPEDEF_YY_SCANNER_T和yyscan_t
 * - 如果無-R選項，則這段代碼前沒有定義YY_TYPEDEF_YY_SCANNER_T
 */
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
/* 不是reentrant環境 */
#define CONF_SCAN_REENTRANT 0

void yylex_init(void **scanner) {}
#define yyalloc(...) osal_malloc(__VA_ARGS__)
#define yyfree(...)  osal_free(__VA_ARGS__)
#define yyrealloc(...) osal_realloc(__VA_ARGS__)
#define YY_FATAL_ERROR(msg) scanner_error(msg, NULL)

static struct token *cur_token;
static int lineno;
static void (*scanner_error)(yyconst char msg[] ,yyscan_t yyscanner);
#else /* YY_TYPEDEF_YY_SCANNER_T */
/* 是reentrant環境
 * 在下面flex生成的定義中，需要一個YY_EXTRA_TYPE類型
 */
#define CONF_SCAN_REENTRANT 1

#define yyalloc(size, scan) osal_malloc(size)
#define yyfree(ptr, scan)  osal_free(ptr)
#define yyrealloc(ptr, size, scan) osal_realloc(ptr, size)
#define YY_FATAL_ERROR(msg) scanner_error(msg, yyscanner)

struct conf_scan_t {
  struct token *token;
  int           lno;
  void        (*scan_err)(yyconst char msg[] ,yyscan_t yyscanner);
};
#define YY_EXTRA_TYPE struct conf_scan_t

#define cur_token     ((struct yyguts_t*)yyscanner)->yyextra_r.token
#define lineno        ((struct yyguts_t*)yyscanner)->yyextra_r.lno
#define scanner_error ((struct yyguts_t*)yyscanner)->yyextra_r.scan_err

#endif  /* YY_TYPEDEF_YY_SCANNER_T */
%}

%option noyywrap
%option noyyalloc noyyrealloc noyyfree
%option never-interactive

%%

0[0-9]+			{
				cur_token->type = TOKEN_ONUM;
				cur_token->v.num = strtol( yytext + 1, NULL, 8 );
				return cur_token->type;
			}

0x[0-9a-fA-F]+		{
				cur_token->type = TOKEN_HNUM;
				cur_token->v.num = strtol( yytext + 2, NULL, 16 );
				return cur_token->type;
			}

0b[0-1]+		{
				cur_token->type = TOKEN_BNUM;
				cur_token->v.num = strtol( yytext + 2, NULL, 2 );
				return cur_token->type;
			}
[0-9]+			{
				cur_token->type = TOKEN_DNUM;
				cur_token->v.num = strtol( yytext, NULL, 10 );
				return cur_token->type;
			}

[a-zA-Z/][a-zA-Z/0-9_\-\+@:\.%]* {
				cur_token->type = TOKEN_BSTR;
				strncpy( cur_token->v.str, yytext, yyleng );
				cur_token->v.str[yyleng] = 0;
        printf ("found string %s\r\n", cur_token->v.str);
				return cur_token->type;
			}

"\""[^"\n]*"\""		{
				cur_token->type = TOKEN_QSTR;
				strncpy( cur_token->v.str,
						yytext + 1, yyleng - 2 );
				cur_token->v.str[yyleng - 2] = 0;
				return cur_token->type;
			}

[ \t\r]+			/* whitespace */

"\n"			++lineno;

"#"[^\n]*		/* comment */

";"			|
"{"			|
"}"			{
				cur_token->type = *yytext;
				return cur_token->type;
			}

<<EOF>>			return TOKEN_EOF;

.			return TOKEN_ERR;

%%

int get_next_token( struct token *tok, int *line, yyscan_t yyscanner )
{
	int ret;

	cur_token = tok;

#if CONF_SCAN_REENTRANT
	ret = yylex(yyscanner);
#else
	ret = yylex();
#endif

	*line = lineno;

	return ret;
}

int start_conf_read( FILE *fp, void (*scan_err)(const char msg[], void *scanner), yyscan_t yyscanner ) {
#if CONF_SCAN_REENTRANT
  yyset_in (fp, yyscanner);
#else
  yyset_in (fp);
#endif
  lineno        = 1;
  scanner_error = scan_err;

  return 0;
}

int start_conf_data (char *config_data, int config_data_len, void (*scan_err)(const char msg[], void *scanner), yyscan_t yyscanner )
{
#if CONF_SCAN_REENTRANT
  yyset_in (4, yyscanner);
#else
  yyset_in (4);
#endif
  lineno        = 1;
  scanner_error = scan_err;

  if(yy_scan_buffer(config_data, (int)config_data_len) < 0)
    return -1;
  return 0;
}


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
#ifndef _CONF_PARSE_H_
#define _CONF_PARSE_H_

#include <stdio.h>
#define YY_INPUT(...)
#define ECHO \
  do         \
  {          \
  } while (0)
#undef stdin
#undef stdout
#define stdin 4
#define stdout 8
#define isatty(n) my_isatty(n)
#define fileno(f) my_fileno(f)
static inline int my_isatty(int n) { return 0; }
static inline int my_fileno(int n) { return 0; }
__attribute__((weak)) int errno;
// #define	EINTR		 4	/* Interrupted system call */

/* 這個在conf_parse.c中使用 */
#define TOKEN_NUM 4
#define TOKEN_STR 8

/* 定義不同類型的NUM，以便實現回寫時保持原先的進制格式
 * 這個在conf_scan.l中使用
 */
#define TOKEN_ERR -1
#define TOKEN_EOF 0
#define TOKEN_BNUM 4
#define TOKEN_DNUM 5
#define TOKEN_ONUM 6
#define TOKEN_HNUM 7
#define TOKEN_BSTR 8
#define TOKEN_QSTR 9

/* 由於兩套TOKEN TYPE系統不同，因此需要添加一個類型匹配支持 */
#define is_TOKEN_TYPE_MATCH(t1, t2) (((t1) >> 2) == ((t2) >> 2))

#define case_TOKEN_NUM \
  case TOKEN_BNUM:     \
  case TOKEN_DNUM:     \
  case TOKEN_ONUM:     \
  case TOKEN_HNUM
#define case_TOKEN_STR \
  case TOKEN_BSTR:     \
  case TOKEN_QSTR
#define is_TOKEN_NUM(type) (((type) > TOKEN_EOF) && ((type) < TOKEN_BSTR))
#define is_TOKEN_STR(type) ((type) == TOKEN_BSTR || ((type) == TOKEN_QSTR))

struct token
{
  int type;
  union
  {
    int num;
    char str[256];
  } v;
};

struct statement
{
  char *directive;
  int (*process)(int num_tokens, struct token *tokens, void *d);
  int min_args;
  int max_args;
  int types[10];
};

/* conf_parse.c */
void register_config_context(char *type,
                             char *name,
                             void *(*start_block)(void),
                             int (*end_block)(void *d),
                             struct statement *s);

/* conf_scan.l */
int start_conf_read(FILE *fp, void (*scan_err)(const char msg[], void *scanner), void *);
int get_next_token(struct token *tok, int *line, void *);

#endif /* _CONF_PARSE_H_ */

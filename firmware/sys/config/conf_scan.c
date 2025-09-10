#include "typedef.h"
#include "conf_parse.h"

static uint8_t *yybuf;
static uint8_t *yybuf_end;
static uint8_t *yytext;
static uint32_t yyleng;
static uint32_t lineno;

int get_next_char(void)
{
  if (yytext + yyleng >= yybuf_end)
    return 0;

  return yytext[yyleng++];
}

int look_ahead_char(void)
{
  if (yytext + yyleng >= yybuf_end)
    return 0;

  return yytext[yyleng];
}

void put_back_char(void)
{
  if (yyleng)
    --yyleng;
}

#define is_space(ch) (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
#define is_bin(ch) (ch == '0' || ch == '1')
#define is_dec(ch) (ch >= '0' && ch <= '9')
#define is_hexl(ch) (ch >= 'a' && ch <= 'f')
#define is_hexh(ch) (ch >= 'A' && ch <= 'F')
#define is_hex(ch) (is_dec(ch) || is_hexl(ch) || is_hexh(ch))
#define is_alpha(ch) ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))

#define scan_line()       \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (ch && ch != '\n')
#define scan_oct()        \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (is_dec(ch))
#define scan_hex()        \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (is_hex(ch))
#define scan_bin()        \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (is_bin(ch))
#define scan_dec()        \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (is_dec(ch))
#define scan_qstr()       \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (ch && ch != '\n' && ch != '"')
#define scan_str()        \
  do                      \
  {                       \
    ch = get_next_char(); \
  } while (               \
      is_alpha(ch) ||     \
      is_dec(ch) ||       \
      ch == '/' ||        \
      ch == '_' ||        \
      ch == '-' ||        \
      ch == '+' ||        \
      ch == '@' ||        \
      ch == ':' ||        \
      ch == '.' ||        \
      ch == '%')

#define skip_space()      \
  do                      \
  {                       \
    yytext += yyleng - 1; \
    yyleng = 1;           \
  } while (0)
#define skip_token()      \
  if (yyleng)             \
  {                       \
    yytext += yyleng - 1; \
    yyleng = 0;           \
  }

int get_next_token(struct token *cur_token, int *line, void *yyscanner)
{
  int ch;

  skip_token();

  // 先跳過空白和注釋
  do
  {
    ch = get_next_char();
    if (ch == '#')
      scan_line(); // 注釋，跳過整行，留下換行符
    if (ch == '\n')
      ++lineno;
  } while (is_space(ch));
  skip_space();

  *line = lineno;

  if (ch == 0)
    return TOKEN_EOF;

  if (ch == '0')
  {
    // 識別到數字，進一步看格式
    int ch = look_ahead_char();

    if (is_dec(ch))
    {
      scan_oct();
      cur_token->v.num = simple_strtol(yytext, NULL, 8);
      cur_token->type = TOKEN_ONUM;
      return cur_token->type;
    }
    if (ch == 'x' || ch == 'X')
    {
      ch = get_next_char();
      scan_hex();
      cur_token->v.num = simple_strtol(yytext + 2, NULL, 16);
      cur_token->type = TOKEN_ONUM;
      return cur_token->type;
    }
    if (ch == 'b' || ch == 'B')
    {
      ch = get_next_char();
      scan_bin();
      cur_token->v.num = simple_strtol(yytext + 2, NULL, 2);
      cur_token->type = TOKEN_ONUM;
      return cur_token->type;
    }
  }
  if (is_dec(ch))
  {
    scan_dec();
    cur_token->v.num = simple_strtol(yytext, NULL, 10);
    cur_token->type = TOKEN_ONUM;
    return cur_token->type;
  }

  if (ch == ';' || ch == '{' || ch == '}')
  {
    ++yyleng; // 要跨一個字節，skip_token會還回來
    cur_token->type = ch;
    return cur_token->type;
  }

  if (ch == '"')
  {
    scan_qstr();
    strncpy(cur_token->v.str, yytext + 1, yyleng - 2);
    cur_token->v.str[yyleng - 2] = 0;
    cur_token->type = TOKEN_QSTR;
    ++yyleng; // 要再吃掉尾巴上的引號
    return cur_token->type;
  }

  if (is_alpha(ch) || (ch == '/'))
  {
    scan_str();
    strncpy(cur_token->v.str, yytext, yyleng - 1);
    cur_token->v.str[yyleng - 1] = 0;
    cur_token->type = TOKEN_BSTR;
    return cur_token->type;
  }

  return TOKEN_ERR;
}

int start_conf_data(char *config_data, int config_data_len, void (*scan_err)(const char msg[], void *scanner), void *yyscanner)
{
  yybuf = config_data;
  yybuf_end = config_data + config_data_len;
  yytext = config_data;
  yyleng = 0;
  lineno = 1;

  return 0;
}

void yylex_init(void *yyscanner) {}
void yylex_destroy(void *yyscanner) {}

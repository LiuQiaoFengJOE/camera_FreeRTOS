#include "typedef.h"
#include "osal/osal_alloc.h"

#include "conf_parse.h"
#include "debug.h"

int config_test(int num_tokens, struct token *tokens, void *d)
{
  int port;

  port = tokens[1].v.num;

  if (port <= 0 || port > 65535)
  {
    p_err("invalid listen port %d", port);
    return -1;
  }
  p_info("config_test %d", port);
  return 1;
}

int config_frameheap(int num_tokens, struct token *tokens, void *d);
static struct statement global_statements[] = {
/* directive name, process function, min args, max args, arg types */

#if 0
	{ "port", config_port, 1, 1, { TOKEN_NUM } },
	{ "rtprange", config_rtprange, 2, 2, { TOKEN_NUM, TOKEN_NUM } },
	{ "frameheap", config_frameheap, 1, 2, { TOKEN_NUM, TOKEN_NUM } },
	{ "sipproxy", config_sip_proxy, 1, 1, { TOKEN_STR } },
	{ "sipname", config_sip_name, 1, 1, { TOKEN_STR } },
	{ "sipdomain", config_sip_domain, 1, 1, { TOKEN_STR } },
	{ "sipusername", config_sip_username, 1, 1, { TOKEN_STR } },
	{ "sippassword", config_sip_password, 1, 1, { TOKEN_STR } },
	{ "sipregister", config_sip_register, 1, 1, { TOKEN_STR } },
#endif

    {"videoheap", config_frameheap, 2, 2, {TOKEN_NUM, TOKEN_NUM}},
    {"audioheap", config_frameheap, 2, 2, {TOKEN_NUM, TOKEN_NUM}},
    {"sendheap", config_frameheap, 2, 2, {TOKEN_NUM, TOKEN_NUM}},
    //  { "test", config_test, 1, 1, { TOKEN_NUM } },
    /* empty terminator -- do not remove */
    {NULL, NULL, 0, 0, {}}};

struct config_context
{
  struct config_context *next;
  char type[256];
  char name[256];
  void *(*start_block)(void);
  int (*end_block)(void *d);
  struct statement *statements;
};

/* Create a global config context pointing to the global_statements array
 * defined in global_config.h */
static struct config_context global_context = {NULL, {}, {}, NULL, NULL, global_statements};

void register_config_context(char *type,
                             char *name,
                             void *(*start_block)(void),
                             int (*end_block)(void *d),
                             struct statement *s)
{
  struct config_context *cc;

  for (cc = &global_context; cc->next; cc = cc->next)
    ;
  cc->next = (struct config_context *)osal_malloc(sizeof(struct config_context));
  cc = cc->next;
  cc->next = NULL;
  strcpy(cc->type, type);
  strcpy(cc->name, name);
  cc->start_block = start_block;
  cc->end_block = end_block;
  cc->statements = s;
}

static int process_statement(struct config_context *cc, void *context_data, struct token *t, int num_tokens, int line)
{
  struct statement *s;
  char *directive = t[0].v.str;
  int num_args = num_tokens - 1, i;
  int seen = 0;
  struct token *args = t + 1;

  if (!is_TOKEN_STR(t[0].type))
  {
    p_err("line %d: malformed configuration directive (unexpected numeral?)");
    return -1;
  }

  for (s = cc->statements; s->directive; ++s)
    if (!strcasecmp(s->directive, directive))
    {
      seen = 1;
      if (num_args < s->min_args || num_args > s->max_args)
        continue;
      for (i = 0; i < num_args; ++i)
        if (!is_TOKEN_TYPE_MATCH(s->types[i], args[i].type))
          break;
      if (i < num_args)
        continue;
      s->process(num_tokens, t, context_data);
      return 0;
    }

  if (seen)
    p_err("line %d: wrong number/type of arguments for directive \"%s\"", line, directive);
  else
    p_err("line %d: unknown configuration directive \"%s\"", line, directive);
  return -1;
}

static int scan_err(const char msg[], void *scanner)
{
  p_err("%s\r\n", msg);
}

int conf_parse(char *config_data, int config_data_len)
{
  struct token t[10];
  int cur_token = 0, line;
  struct config_context *cc = &global_context;
  void *context_data = NULL;
  void *scanner;

  yylex_init(&scanner);
  if (start_conf_data(config_data, (int)config_data_len, scan_err, scanner) < 0)
    return -1;

  while (get_next_token(&t[cur_token], &line, scanner) > 0)
  {
    switch (t[cur_token].type)
    {
    case_TOKEN_NUM:
    case_TOKEN_STR:
      if (++cur_token == 10)
      {
        p_err("line %d: max number of arguments exceeded", line);
        return -1;
      }
      break;
    case ';':
	  if(cc==NULL)
	  {
	  	cur_token = 0;
	  	break;
	  }
      if (process_statement(cc, context_data, t, cur_token, line) < 0)
        return -1;
      cur_token = 0;
      break;
    case '{':
      if (cur_token != 2 || !is_TOKEN_STR(t[0].type) || !is_TOKEN_STR(t[1].type))
      {
        p_err("line %d: blocks must begin with a type and module name", line);
        return -1;
      }
      if (cc != &global_context)
      {
        p_err("line %d: unexpected '{' (missing '}'?)", line);
        return -1;
      }
      while ((cc = cc->next) && (strcasecmp(cc->type, t[0].v.str) != 0 || strcasecmp(cc->name, t[1].v.str) != 0))
        ;
      if (!cc)
      {
        p_err("line %d: unknown module %s %s", line, t[0].v.str, t[1].v.str);
		cur_token = 0;
		break;
        //return -1;
      }
      if (cc->start_block && (context_data = cc->start_block()) == NULL)
        return -1;
      cur_token = 0;
      break;
    case '}':
	  if(cc==NULL)
	  {
	    cur_token = 0;
        cc = &global_context;
        context_data = NULL;
	  	break;
	  }
      if (cur_token != 0)
      {
        p_err("line %d: last statement in block is missing a semicolon", line);
        return -1;
      }
      if (cc == &global_context)
      {
        p_err("line %d: unbalanced }", line);
        return -1;
      }
      if (cc->end_block && cc->end_block(context_data) < 0)
        return -1;
      cur_token = 0;
      cc = &global_context;
      context_data = NULL;
      break;
    default:
      p_err("internal parser error: unrecognized type %d", t[cur_token].type);
      return -1;
    }
  }
  yylex_destroy(scanner);
  return 0;
}

int read_config_file(char *config_file)
{
  char *config_data;
  uint32_t config_data_len;
  FILE *fp;
  int ret;

  fp = osal_fopen(config_file, "rb");
  if (fp == NULL)
  {
    p_err("Can't open file %s\r\n", config_file);
    return 1;
  }
  config_data_len = osal_fsize(fp);

  config_data = osal_malloc(config_data_len + 4);
  if (osal_fread(config_data, 1, config_data_len, fp) < config_data_len)
  {
    p_err("Read file %s error\r\n", config_file);
    osal_free(config_data);
    osal_fclose(fp);
    return 1;
  }
  osal_fclose(fp);

  config_data[config_data_len] = 0;
  config_data[config_data_len + 1] = 0;
  ret = conf_parse(config_data, config_data_len);

  osal_free(config_data);

  return ret;
}

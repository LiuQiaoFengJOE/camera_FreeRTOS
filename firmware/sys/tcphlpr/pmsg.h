/**@file    pmsg.h
 * @author  何凱帆
 * @brief
 * @details
 */

#ifndef _PMSG_H_
#define _PMSG_H_

struct hdrf
{
  char *name;
  char *value;
};

#define MAX_FIELDS 32

struct pmsg
{
  unsigned char *msg;
  int max_len;
  int msg_len;
  struct hdrf fields[MAX_FIELDS];
  int header_count;
  enum
  {
    PMSG_REQ,
    PMSG_RESP
  } type;
  char *proto_id;
  union
  {
    struct
    {
      char *method;
      char *uri;
    } req;
    struct
    {
      int code;
      char *reason;
    } stat;
  } sl;
};

char *pmsg_add_string(struct pmsg *msg, char *s);
int pmsg_parse(struct pmsg *msg);
char *pmsg_get_header(struct pmsg *msg, char *name);
int pmsg_add_header(struct pmsg *msg, char *name, char *value);
int pmsg_add_header_printf(struct pmsg *msg, char *name, char *fmt, ...);
int pmsg_replace_header(struct pmsg *msg, char *name, char *value);
int pmsg_copy_headers(struct pmsg *dest, struct pmsg *src, char *name);
int pmsg_get_param(char *value, char *tag, char *dest, int size);
struct pmsg *pmsg_new(int size);
void pmsg_free(struct pmsg *msg);

void *pmsg_alloc_msgbuf(int size);

#endif /* _PMSG_H_ */

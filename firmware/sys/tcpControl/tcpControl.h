#ifndef _TCP_CONTROL_H
#define _TCP_CONTROL_H
#include "cjson/cjson.h"
#include "setJSON.h"
#include "osal/osal_list.h"
#include "uthash/uthash.h"

// struct setJson;

// typedef void (*callback)(struct setJson *json,cJSON *cJSON);

struct tcpControl
{
	int fd;
	unsigned int time;
};

void *get_sender_fd_list();
struct tcpControl_fd
{
	int fd;		/*添加唯一的fd       key*/
	int type;	/*类型  0:默认   1、sender	  2、rtsp  */
	void *self; /*不同类型,私有结构体不一样,默认为NULL*/
	char mac[6];
	UT_hash_handle hh; /* makes this structure hashable */
};

#if 0

struct setJson
{
	char *key;
	char type;		//使用cjson的类型
	callback callback;
	union
	{
		char *valuestring;			/* The item's string, if type==cJSON_String */
		int valueint;				/* The item's number, if type==cJSON_Number */
		double valuedouble;			/* The item's number, if type==cJSON_Number */
	}value;
	struct setJson *next;
};



static void settimer_json(struct setJson *json,cJSON *cJSON);
static void takephoto_json(struct setJson *json,cJSON *cJSON);
static void record_json(struct setJson *json,cJSON *cJSON);
void global_executeSetJson(cJSON *cJSON);
void global_init_setting();
uint8_t setJson_valueInt(struct setJson *json,const char *name,int value);
uint8_t setJson_valueDouble(struct setJson *json,const char *name,double value);
uint8_t setJson_valueString(struct setJson *json,const char *name,char *value);
void setJson_make_cJSON(struct cJSON *json,struct setJson *setJson);
struct cJSON *make_cJSON(struct setJson *json);
*/
#endif

#endif

#ifndef _SETJSON_H
#define _SETJSON_H
#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "cjson/cjson.h"
#include "systemSetting/systemSetting.h"
#include "spi/spi.h"

struct setJson;

typedef void (*callback)(struct setJson *json, cJSON *cJSON);

struct setJson
{
	char *key;
	char type; // 使用cjson的类型
	callback callback;
	union
	{
		char *valuestring;	/* The item's string, if type==cJSON_String */
		int valueint;		/* The item's number, if type==cJSON_Number */
		double valuedouble; /* The item's number, if type==cJSON_Number */
	} value;
	struct setJson *next;
};

void exceute_setting(struct cJSON *srcJson, const struct cJSON *destJson);
void global_execute_sensor_setting();
void default_sensor_setjson(struct setJson *json);
void default_exceuteSetjson(struct setJson *json);
void global_init_sensorJson();
void set_sensor_SetJson(cJSON *json);
void cJSON_to_setJson(struct setJson *json, cJSON *cJSON);
static struct setJson *search_SetJson(struct setJson *json, const char *name);
uint8_t setJson_valueString(struct setJson *json, const char *name, char *value);
uint8_t setJson_valueDouble(struct setJson *json, const char *name, double value);
uint8_t setJson_valueInt(struct setJson *json, const char *name, int value);
void addSetJson(struct setJson *json, char *key, char type, callback callback);
void initSetJson(struct setJson *json);
static void ISO_json(struct setJson *json, cJSON *cJSON);
static void EV_json(struct setJson *json, cJSON *cJSON);

struct setJson *get_exceute_setJson();
struct setJson *get_sensor_setJson();
static void executeSetJson(struct setJson *json, cJSON *cJSON);

struct cJSON *make_cJSON(struct setJson *json);
void setJson_make_cJSON(struct cJSON *json, struct setJson *setJson);
void save_sensor_setting(struct setJson *json, cJSON *cJSON);
void save_sensor_setJson();
uint8_t setWriteFlag();
uint8_t clearWriteFlag();
uint8_t search_sensor_Int(const char *name);
uint8_t *get_wifiSSID();
void global_init();

#endif

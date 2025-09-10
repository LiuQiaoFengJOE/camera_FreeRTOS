#include "setJSON.h"

static struct setJson exceute_setJson; // 所有执行的方法在这个结构体
static struct setJson SensorSetting;   // sensor配置结构体,最后执行也是在exceute_setJson中执行,同时sd卡也要把这个结构体全部保存
static uint8_t writeFlag = 0;

extern void vTaskList(char *pcWriteBuffer);

struct setJson *get_sensor_setJson()
{
	return &SensorSetting;
}

struct setJson *get_exceute_setJson()
{
	return &exceute_setJson;
}

static uint8_t getWriteFlagValue()
{
	return writeFlag;
}

static uint8_t *getWriteFlag()
{
	return &writeFlag;
}

uint8_t setWriteFlag()
{
	uint8_t *flag = getWriteFlag();
	*flag = 1;
}

uint8_t clearWriteFlag()
{
	uint8_t *flag = getWriteFlag();
	*flag = 0;
}

static void excute_json(struct setJson *json, cJSON *cJSON)
{
	struct setJson *sensorSetJson = get_sensor_setJson();
	if (cJSON->type == json->type)
	{
		set_Setting(cJSON->string, cJSON->valueint);
	}
	else
	{
		printf("cJSON type is err:%d\r\n", json->type);
	}
}

// 关于wifi的参数,没有作用,只是打印相关信息
static void set_wifi_json(struct setJson *json, cJSON *cJSON)
{
	if (cJSON->type == json->type)
	{
		printf("%s:%s\r\n", json->key, cJSON->valuestring);
	}
	else
	{
		printf("cJSON type is err:%d\r\n", json->type);
	}
}

void set_wifiMAC_json(u8 *mac)
{
	struct setJson *sensorSetJson = get_sensor_setJson();
	setJson_valueString(sensorSetJson, WIFIMAC, mac);
	save_sensor_setJson();
}

// 获得wifiSSID
uint8_t *get_wifiSSID()
{
	struct setJson *sensorSetJson = get_sensor_setJson();
	struct setJson *setJson;
	setJson = search_SetJson(sensorSetJson, WIFISSID);
	if (setJson->type == cJSON_String)
	{
		return setJson->value.valuestring;
	}
	else
	{
		return NULL;
	}
}

// 获得wifiPW
uint8_t *get_wifiPW()
{
	struct setJson *sensorSetJson = get_sensor_setJson();
	struct setJson *setJson;
	setJson = search_SetJson(sensorSetJson, WIFIPW);
	if (setJson->type == cJSON_String)
	{
		return setJson->value.valuestring;
	}
	else
	{
		return NULL;
	}
}

// 获得wifiSSID
uint8_t *get_wifiMAC()
{
	struct setJson *sensorSetJson = get_sensor_setJson();
	struct setJson *setJson;
	setJson = search_SetJson(sensorSetJson, WIFIMAC);
	if (setJson->type == cJSON_String)
	{
		return setJson->value.valuestring;
	}
	else
	{
		return NULL;
	}
}

// 初始化setJson
void initSetJson(struct setJson *json)
{
	json->key = NULL;
	json->type = cJSON_NULL;
	json->callback = NULL;
	json->value.valuestring = NULL;
	json->next = NULL;
}

// 添加setJson方法
void addSetJson(struct setJson *json, char *key, char type, callback callback)
{
	struct setJson *mjson = json;
	struct setJson *add = osal_malloc(sizeof(struct setJson));
	while (mjson->next)
	{
		mjson = mjson->next;
	}

	add->next = NULL;
	add->type = type;
	add->value.valuestring = NULL;

	add->key = osal_malloc(strlen(key) + 1);
	memcpy(add->key, key, strlen(key) + 1);
	add->callback = callback;

	mjson->next = add;
}

// 设置setJson中的值			name:key   value:value
//  return  1:成功   0:失败
uint8_t setJson_valueInt(struct setJson *json, const char *name, int value)
{
	struct setJson *setJson;

	setJson = search_SetJson(json, name);
	if (!setJson)
	{
		printf("no found json's name:%s\r\n", name);
		return 0;
	}
	if (setJson->type != cJSON_Number)
	{
		printf("!!!error!!!setJson_valueInt input type err,not cJSON_Number:%d\r\n", setJson->type);
		return 0;
	}
	setJson->value.valueint = value;
	setWriteFlag();
	// printf("setJson_valueInt finish:%s\r\n",name);
	return 1;
}

// 设置setJson中的值
//  return  1:成功   0:失败
uint8_t setJson_valueDouble(struct setJson *json, const char *name, double value)
{
	struct setJson *setJson;

	setJson = search_SetJson(json, name);
	if (!setJson)
	{
		printf("no found json's name:%s\r\n", name);
		return 0;
	}

	if (setJson->type != cJSON_Number)
	{
		printf("!!!error!!!setJson_valueInt input type err,not cJSON_Number:%d\r\n", setJson->type);
		return 0;
	}
	setJson->value.valuedouble = value;
	setWriteFlag();
	// printf("setJson_valueDouble finish:%s\r\n",name);
	return 1;
}

// 设置setJson中的值
//  return  1:成功   0:失败
uint8_t setJson_valueString(struct setJson *json, const char *name, char *value)
{
	struct setJson *setJson;

	setJson = search_SetJson(json, name);
	if (!setJson)
	{
		printf("no found json's name:%s\r\n", name);
		return 0;
	}

	if (setJson->type != cJSON_String)
	{
		printf("!!!error!!!setJson_valueInt input type err,not cJSON_String:%d\r\n", setJson->type);
		return 0;
	}

	// 如果原来配置了值,则需要释放
	if (setJson->value.valuestring)
	{
		osal_free(setJson->value.valuestring);
	}
	setJson->value.valuestring = osal_malloc(strlen(value) + 1);
	memcpy(setJson->value.valuestring, value, strlen(value) + 1);
	setWriteFlag();
	// printf("setJson_valueString finish:%s\r\n",name);
	return 1;
}

// 从setJson中寻找key为name的结构体，并且返回该接头体
static struct setJson *search_SetJson(struct setJson *json, const char *name)
{
	struct setJson *Njson = json;
	while (1)
	{
		if (!Njson)
		{
			printf("search_Json fail:%s\r\n", name);

			return NULL;
		}
		if (Njson->key && !strcmp(Njson->key, name))
		{
			return Njson;
		}

		Njson = Njson->next;
	}
	// 不会到这里
	return NULL;
}

// 搜索保存到json中的数据
uint8_t search_sensor_Int(const char *name)
{
	struct setJson *Njson = get_sensor_setJson();
	while (1)
	{
		if (!Njson)
		{
			printf("search_Json fail:%s\r\n", name);

			return 0;
		}
		if (Njson->key && !strcmp(Njson->key, name))
		{
			break;
		}

		Njson = Njson->next;
	}
	if (Njson->type == cJSON_Number)
	{
		return Njson->value.valueint;
	}
}

// cJSON  to  setJson
// 将cJSON对应key中的值设置到setJson对应key位置
void cJSON_to_setJson(struct setJson *json, cJSON *cJSON)
{
	if (cJSON->type == json->type)
	{
		if (cJSON->type == cJSON_Number)
		{
			setJson_valueInt(json, cJSON->string, cJSON->valueint);
		}
		else if (cJSON->type == cJSON_String)
		{
			setJson_valueString(json, cJSON->string, cJSON->valuestring);
		}
		else
		{
			printf("boot_json no this type is err:%d\r\n", cJSON->type);
		}
	}
	else
	{
		printf("cJSON type is err:%d\r\n", json->type);
	}
}

// 执行json的配置,通过找到exceute_setJson的结构体,然后调用配置
// 只执行json中的方法,不是所有
void set_sensor_SetJson(cJSON *json)
{
	printf("global_executeSetJson!\r\n");
	if (json->type != cJSON_Object)
	{
		printf("%s json isn't correct json type!\r\n", __FUNCTION__);
		return;
	}
	int i;
	// 找到exceute_setJson结构体,执行回调函数
	struct setJson *sensorSetJson = get_sensor_setJson();
	for (i = 0; i < cJSON_GetArraySize_or1k(json); i++)
	{
		cJSON *item = cJSON_GetArrayItem_or1k(json, i);
		executeSetJson(sensorSetJson, item);
	}
}

// sensorJson初始化,配置默认值,如果sd卡或者flash中没有对应的配置,则使用默认配置
void global_init_sensorJson()
{
	printf("global_init_setting!\r\n");
	struct setJson *sensorSetJson = get_sensor_setJson();
	initSetJson(sensorSetJson);
	default_sensor_setjson(sensorSetJson);
}

// 配置exceuteSetjson的方法,最好在初始化的时候配置
// 所有执行的方法都在这里
// 这里所有执行的方法都会对应到systemSetting.c中
// 并且这里会在开机的时候会进行回调一次,因此有些不是别要的,就不要设置回调函数,例如:删除文件、锁文件等
void default_exceuteSetjson(struct setJson *json)
{
	addSetJson(json, QUALITYNAME, cJSON_Number, excute_json);		 // 拍照或者录像的质量
	addSetJson(json, LANGUAGENAME, cJSON_Number, excute_json);		 // 设置语言
	addSetJson(json, LOOPNAME, cJSON_Number, excute_json);			 // 循环录像
	addSetJson(json, DPINAME, cJSON_Number, excute_json);			 // 本地储存图片或者录像的分辨率
	addSetJson(json, RECSOUNDNAME, cJSON_Number, excute_json);		 // 录像声音
	addSetJson(json, AWBNAME, cJSON_Number, excute_json);			 // 白平衡
	addSetJson(json, EVNAME, cJSON_Number, excute_json);			 // 曝光
	addSetJson(json, KEYSOUNDNAME, cJSON_Number, excute_json);		 // 按键声音
	addSetJson(json, SAVENAME, cJSON_Number, excute_json);			 // 保存到sd卡或者flash
	addSetJson(json, TAKEPHOTONAME, cJSON_Number, excute_json);		 // wifi拍照
	addSetJson(json, WIFIDPINAME, cJSON_Number, excute_json);		 // wifi传输分辨率
	addSetJson(json, RECORDNAME, cJSON_Number, excute_json);		 // wifi录像
	addSetJson(json, WIFIQUALITYNAME, cJSON_Number, excute_json);	 // wifi录像
	addSetJson(json, MDTNAME, cJSON_Number, excute_json);			 // 移动侦测
	addSetJson(json, FORMATNAME, cJSON_Number, NULL);				 // 格式化,无需回调函数
	addSetJson(json, PHOTOBURSTNAME, cJSON_Number, excute_json);	 // 连拍
	addSetJson(json, PHOTOTIMELAPSENAME, cJSON_Number, excute_json); // 定时拍照
	addSetJson(json, WIFISSID, cJSON_String, set_wifi_json);		 // wifiSSID
	addSetJson(json, WIFIPW, cJSON_String, set_wifi_json);			 // wifiPW
	addSetJson(json, WIFIMAC, cJSON_String, set_wifi_json);			 // wifiPW
	addSetJson(json, WATERMARKNAME, cJSON_Number, excute_json);		 //
	addSetJson(json, SCREENSAVENAME, cJSON_Number, excute_json);	 //
	addSetJson(json, AUTOPOWEROFFNAME, cJSON_Number, excute_json);	 //
	addSetJson(json, DELFILENAME, cJSON_Number, NULL);				 // 删除文件,无需会标函数
	addSetJson(json, FILELOCKNAME, cJSON_Number, NULL);				 // 文件锁,无需回调函数
	addSetJson(json, PDPINAME, cJSON_Number, excute_json);			 //
	addSetJson(json, ACUTANCENAME, cJSON_Number, excute_json);		 //
	addSetJson(json, CHILDRENDPINAME, cJSON_Number, excute_json);	 // 儿童相机分辨率选择
}

// 全局初始化，一开始需要启动
void global_init()
{
	struct setJson *exceuteSetJson = get_exceute_setJson();
	initSetJson(exceuteSetJson);
	default_exceuteSetjson(exceuteSetJson);
	global_init_sensorJson();
}

// 全局执行,包含的所有方法都执行
// 执行完后删除生成的json数据
// 将sensor结构体转换成JSON数据,然后去exceuteSetJson中找到对应执行的方法
void global_execute_sensor_setting()
{
	int i;
	cJSON *json;
	// sensor的结构体
	struct setJson *sensorSetJson = get_sensor_setJson();
	// 执行方法结构体
	struct setJson *exceuteSetJson = get_exceute_setJson();
	json = make_cJSON(sensorSetJson);
	exceute_setting(json, exceuteSetJson);
	cJSON_Delete_or1k(json);
}

void save_sensor_setting(struct setJson *json, cJSON *JSON)
{

	if (JSON->type == json->type)
	{
		save_sensor_setJson();
	}
	else
	{
		printf("cJSON type is err:%d\r\n", json->type);
	}
}

void save_sensor_setJson()
{

	if (!getWriteFlagValue())
	{
		printf("setJson no change!\r\n");
		return;
	}
	cJSON *getjson;
	char *formatJson;
	// void *fp = NULL;
	struct spidata data;
	struct setJson *sensorSetJson = get_sensor_setJson();
	// 执行方法结构体
	struct setJson *exceuteSetJson = get_exceute_setJson();
	getjson = make_cJSON(sensorSetJson);
	formatJson = cJSON_PrintUnformatted_or1k(getjson);

	if (formatJson)
	{
		data.type = JSON_TYPE;
		data.version = VERSION;
		data.datalen = strlen(formatJson) + 1;

		spi_write_data(FLASH_START, formatJson, &data);
		osal_free(formatJson);
	}

	cJSON_Delete_or1k(getjson);
	clearWriteFlag();
}

// srcJson需要执行的json数据
// destJson保存执行方法的json数据
void exceute_setting(struct cJSON *srcJson, const struct cJSON *destJson)
{
	int i;
	for (i = 0; i < cJSON_GetArraySize_or1k(srcJson); i++)
	{
		cJSON *item = cJSON_GetArrayItem_or1k(srcJson, i);
		executeSetJson(destJson, item);
	}
}

// 执行json中的回调函数
static void executeSetJson(struct setJson *json, cJSON *cJSON)
{
	struct setJson *Njson = json;
	while (1)
	{
		if (!Njson)
		{
			printf("no this key:%stype:%d\r\n", cJSON->string, cJSON->type);

			break;
		}
		if (Njson->type == cJSON->type && !strcmp(Njson->key, cJSON->string))
		{
			// printf("executeSetJson found json:%s->type:%d\r\n",Njson->key,Njson->type);
			if (Njson->callback)
			{
				Njson->callback(Njson, cJSON);
			}
			else
			{
				printf("%s json->callback is NULL!\r\n", Njson->key);
			}
			break;
		}

		Njson = Njson->next;
	}
}

// 生成cJSON，遍历setJson生成完整的cJSON
struct cJSON *make_cJSON(struct setJson *json)
{

	struct setJson *Njson = json;
	cJSON *makeJson = NULL;
	makeJson = cJSON_CreateObject_or1k();

	while (1)
	{
		if (!Njson)
		{
			break;
		}
		if (Njson->key)
		{
			setJson_make_cJSON(makeJson, Njson);
		}

		Njson = Njson->next;
	}
	return makeJson;
}

// 将一个setJson变成cJSON格式
void setJson_make_cJSON(struct cJSON *json, struct setJson *setJson)
{
	if (setJson->type == cJSON_String)
	{
		cJSON_AddItemToObject_or1k(json, setJson->key, cJSON_CreateString_or1k(setJson->value.valuestring));
	}
	else if (setJson->type == cJSON_Number)
	{
		cJSON_AddItemToObject_or1k(json, setJson->key, cJSON_CreateNumber_or1k(setJson->value.valueint));
	}
	else
	{
		printf("make_cJSON no this type:%d\r\n", setJson->type);
	}
}

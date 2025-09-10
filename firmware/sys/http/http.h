#ifndef _HTTP_H
#define _HTTP_H
#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "cjson/cjson.h"
#include "eloop/eloop.h"

#include <stdarg.h>

struct url;
struct httpEntry;
struct httpserver;
struct httpClient;
struct settingJson;

int config_http(int port);
void http_reponse_Test(struct httpClient *httpClient);
void http_reponse_File(struct httpClient *httpClient);
void http_reponse_OJpg(struct httpClient *httpClient);
void http_reponse_TJpg(struct httpClient *httpClient);
void http_send_error(struct httpClient *httpClient);
uint8_t httpqueue(struct httpClient *httpClient);
void http_reponse_AVI(struct httpClient *httpClient);
void new_http_reponse_OJpg(struct httpClient *httpClient);
void new_http_reponse_TJpg(struct httpClient *httpClient);
void http_reponse_NES(struct httpClient *httpClient);

void http_post_Test(struct httpClient *httpClient);
typedef void (*http_reponse)(struct httpClient *httpClient);
typedef void (*jsonReponse)(struct settingJson *settingJson, cJSON *cJSON);
typedef uint8_t (*http_queue)(struct httpClient *httpClient);

struct settingJson
{
	char *key;
	char type;
	jsonReponse reponse;
	struct settingJson *next;
};

// type:	0  	默认ip地址，不带参数
//		1	完整匹配(整个地址都要匹配)
//		2	首字符串匹配(匹配前部分,后面动态)
struct url
{
	char *url;
	char type;
	http_reponse reponse;
	http_queue queue;
	struct url *next;
};

// http头部的信息
struct httpEntry
{
	char *key;
	char *value;
	char *pattern;
	struct httpEntry *next;
};

struct httpserver
{
	int fdServer;
	// xQueueHandle httpQueue;
	xQueueHandle thread_pool_queue; // 线程池队列
};

struct httpClient
{
	int fdClient;  // 客户端的socket
	char *pattern; // 需要释放
	unsigned int time;
	struct httpserver *httpserver; // 不释放
	struct url *web;			   // 不能释放
	struct httpEntry *http_request;
};

struct httpClientTest
{
	int fdClient; // 客户端的socket
};

struct httpIndex
{
	const char *Type;
	const char *Length;
	const char *Disposition;
	const char *Connection;
};

struct httpresp
{
	char *headbuf;
	uint32_t msgLen;
	uint32_t msgMaxLen;
	uint8_t stat;
};

struct httpKeep
{
	struct event *read_event; // tcp读取事件
	int fdClient;
};

static void http_thread_pool(struct httpClient *httpClient);

#endif

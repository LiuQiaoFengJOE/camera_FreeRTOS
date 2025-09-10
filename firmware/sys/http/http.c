/**@file    ftp.c
 * @author  何凱帆
 * @brief   ftp支持
 * @details 提供支持函數
 */

#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "cjson/cjson.h"

#include "debug.h"
#include "http.h"
#include "mac_fd/mac_fd.h"
#include "tcpControl/tcpMessage.h"

#define WEBURLNUM 5

const char *http404 = "<html><body><p>URL is err</p></body></html>";

struct url getweb;
struct url postweb;
struct settingJson postSetting;

// http回应的一些字段
struct httpIndex httpIndex = {
	.Type = "Content-Type",
	.Length = "Content-Length",
	.Disposition = "Content-Disposition",
	.Connection = "Connection"};

struct avimsg
{
	void *str;
	void *fp;
	void *aviinfo_point; // 申请的空间，需要释放
	uint32_t size;
};

uint32_t http_avi_thumb(char *filename, struct avimsg *avimsg)
{
	void *avi;
	void *str;
	struct aviinfo *aviinfo_point = osal_malloc(get_aviinfo_size());
	if (!aviinfo_point)
	{
		printf("aviinfo_point no space malloc!\r\n");
		return 2;
	}
	avimsg->aviinfo_point = aviinfo_point;
	printf("get_aviinfo_size:%d\r\n", get_aviinfo_size());
	uint32_t base, size;
	avi = avidemux2_open(filename, aviinfo_point);
	if (!avi)
	{
		printf("read avi err!\r\n");
		return 1;
	}
	str = avidemux_get_aviinfo(avi);
	avidemux_set_cur(str, 0);
	avidemux_read_begin(str, &base, &size);
	avimsg->str = str;
	avimsg->fp = avidemux_get_fp(avi);
	// 偏移8字节，去掉avi的00dc头部信息,后面就是原图数据开头
	osal_fseek(avimsg->fp, osal_ftell(avimsg->fp) + 8);
	int thumbLocate = http_avi_thumb_Locate(avimsg->fp);
	if (thumbLocate < 0)
	{
		printf("thumbLocate:%d\r\n", thumbLocate);
		avimsg->size = 0;
		return 1;
	}
	// 本帧图片大小-缩略图
	avimsg->size = size - thumbLocate;
	printf("size:%d\r\n", avimsg->size);
	return 0;
}

// 初始化
void initJson(struct settingJson *json)
{
	json->key = NULL;
	json->type = 0;
	json->reponse = NULL;
	json->next = NULL;
}

// 添加json方法
void addJson(struct settingJson *json, char *key, char type, jsonReponse reponse)
{
	struct settingJson *mjson = json;
	struct settingJson *add = osal_malloc(sizeof(struct settingJson));
	while (mjson->next)
	{
		mjson = mjson->next;
	}

	add->next = NULL;
	add->type = type;

	add->key = osal_malloc(strlen(key) + 1);
	memcpy(add->key, key, strlen(key) + 1);

	add->reponse = reponse;

	mjson->next = add;
}

// 配置json拥有的方法
void default_json(struct settingJson *json)
{
	addJson(json, "timer", cJSON_Number, NULL);
}

// 执行json中的函数
void executeJson(struct settingJson *json, cJSON *cJSON)
{
	struct settingJson *Njson = json;
	while (1)
	{
		if (!Njson)
		{
			break;
		}
		if (Njson->type == cJSON->type && !strcmp(Njson->key, cJSON->string))
		{
			printf("found json:%s->type:%d\r\n", Njson->key, Njson->type);
			if (Njson->reponse)
			{
				Njson->reponse(Njson, cJSON);
			}
			else
			{
				printf("json->reponse is NULL!\r\n");
			}
			break;
		}

		Njson = Njson->next;
	}
}

// type:	0  	默认ip地址，不带参数
//		1	完整匹配(整个地址都要匹配)
//		2	首字符串匹配(匹配前部分,后面动态)

void initWeb(struct url *web)
{
	web->url = NULL;
	web->type = 1;
	web->reponse = http_send_error;
	web->queue = NULL;
	web->next = NULL;
}

// web方法添加
void addWeb(struct url *web, char *url, char type, http_reponse reponse, http_queue queue)
{
	printf("url:%s\r\n", url);
	struct url *addweb = web;
	struct url *add = osal_malloc(sizeof(struct url));
	while (addweb->next)
	{
		addweb = addweb->next;
		// printf("test:%d\turl:%s\r\n",addweb->type,addweb->url);
	}

	add->next = NULL;
	add->type = type;
	add->queue = queue;
	add->reponse = reponse;
	add->url = osal_malloc(strlen(url) + 1);
	memcpy(add->url, url, strlen(url) + 1);
	addweb->next = add;
}

// get方法的一些配置
void default_getweb(struct url *web)
{
	addWeb(web, "test", 1, http_reponse_Test, NULL);	// 测试http
	addWeb(web, "file/", 2, http_reponse_File, NULL);	// 测试文件下载,格式为:192.168.1.1/file/xxx/xxx(xxx/xxx是文件名,包含位置)
	addWeb(web, "NES/", 2, http_reponse_NES, NULL);		// 测试文件下载
	addWeb(web, "DCIM/O/", 2, http_reponse_OJpg, NULL); // photo的原图查看
	addWeb(web, "DCIM/T/", 2, http_reponse_TJpg, NULL); // photo的缩略图查看
	addWeb(web, "DCIM/", 2, http_reponse_AVI, NULL);	// AVI缩略图查看

	addWeb(web, "IMG/O/", 2, new_http_reponse_OJpg, NULL);	 // photo的原图查看
	addWeb(web, "IMG/T/", 2, new_http_reponse_TJpg, NULL);	 // photo的原图查看
	addWeb(web, "VIDEO/T/", 2, new_http_reponse_OJpg, NULL); // photo的原图查看
}

// post方法的一些配置
void default_postweb(struct url *web)
{
	addWeb(web, "test", 1, http_post_Test, NULL);
}

// 初始化并申请空间给httpClient
struct httpClient *initHttpClient()
{
	struct httpClient *httpClient = osal_malloc(sizeof(struct httpClient));
	if (!httpClient)
	{
		return NULL;
	}
	httpClient->fdClient = 0;
	httpClient->httpserver = NULL;
	// httpClient->http_request = NULL;
	httpClient->web = NULL;
	httpClient->pattern = NULL;
	return httpClient;
}

// http缩略图，需要的的缩略图有特定格式，因此需要自己拍的格式才会读到正确
int http_thumb(void *fp)
{
	u32 thumbLocate;
	int thumbsize;
	u8 buf[8];
	int start = osal_ftell(fp);
	osal_fread(buf, 1, 2, fp);
	if (buf[0] != 0xff || buf[1] != 0xd8)
	{
		printf("not photo!!!\r\n");
		osal_fseek(fp, start);
		return -1;
	}
	osal_fseek(fp, start + 20);
	osal_fread(buf, 1, 8, fp);
	// printf("%x\t%x\t%x\t%x\r\n",buf[0],buf[1],buf[2],buf[3]);
	if (buf[0] == 0xff && buf[1] == 0xe1 && buf[2] == 0x00 && buf[3] == 0x06)
	{
		memcpy(&thumbLocate, &buf[4], 4);
		osal_fseek(fp, start + thumbLocate);
		thumbsize = osal_fsize(fp) - thumbLocate;
		printf("thumbLocate:%d\r\n", thumbLocate);
		return thumbsize;
	}
	else
	{
		printf("%x\t%x\t%x\t%x\r\n", buf[0], buf[1], buf[2], buf[3]);
		printf("not our photo!!!\r\n");
		osal_fseek(fp, start);
		return -2;
	}
}

// 寻找AVI缩略图的位置,返回相对本帧图片的缩略图位置,并且偏移到对应位置,最后直接读取缩略图大小即可
int http_avi_thumb_Locate(void *fp)
{
	int thumbLocate;
	u8 buf[8];
	// 该帧的起始位置
	int start = osal_ftell(fp);
	osal_fread(buf, 1, 2, fp);
	if (buf[0] != 0xff || buf[1] != 0xd8)
	{
		printf("not photo!!!\r\n");
		uartPrintfBuf(buf, 2);
		// osal_fseek(fp,start);
		osal_fclose(fp);
		return -1;
	}
	osal_fseek(fp, start + 20);
	osal_fread(buf, 1, 8, fp);
	// printf("%x\t%x\t%x\t%x\r\n",buf[0],buf[1],buf[2],buf[3]);
	if (buf[0] == 0xff && buf[1] == 0xe1 && buf[2] == 0x00 && buf[3] == 0x06)
	{
		memcpy(&thumbLocate, &buf[4], 4);
		osal_fseek(fp, start + thumbLocate);
		printf("thumbLocate:%d\r\n", thumbLocate);
		return thumbLocate;
	}
	else
	{
		printf("%x\t%x\t%x\t%x\r\n", buf[0], buf[1], buf[2], buf[3]);
		printf("not our photo!!!\r\n");
		osal_fseek(fp, start);
		return -2;
	}
}

// 读取一行，以\r\n代表回车，主要用于http请求字段
static uint8_t readline(uint8_t *buf, u32 *len)
{
	u32 lineLen = 0;
	// uartPrintfBuf(buf,4);
	if (*buf == 0 || *buf == '\0')
	{
		return 2; // 结束,代表该字符串没有任何数据
	}

	if (*buf == '\r' && *(buf + 1) == '\n')
	{
		return 3; // http头部分结束,空一行
	}
	while (1)
	{
		if (*buf == 0)
		{
			return 1; // 无检测到换行符
		}
		else
		{
			if (*buf == '\r' && *(buf + 1) == '\n')
			{
				*len = lineLen + 2;
				return 0; // 检测到换行符号
			}
			buf++;
			lineLen++;
		}
	}
}

// 这是对GET请求字段的判断，因为与其他字段不同，所以单独判断
void parseGet(uint8_t *recBuf, struct httpEntry *http)
{

	uint8_t len = 0;
	uint8_t *src = recBuf;
	while (*recBuf != ' ' && *recBuf)
	{
		len++;
		recBuf++;
	}

	uint8_t *method = osal_malloc(len + 1);
	memcpy(method, src, len);
	*(method + len) = '\0';
	recBuf++;
	src = recBuf;
	len = 0;

	while (*recBuf != ' ' && *recBuf)
	{
		len++;
		recBuf++;
	}
	uint8_t *url = osal_malloc(len + 1);
	memcpy(url, src, len);
	*(url + len) = '\0';

	http->key = method;
	http->value = url;
	http->next = NULL;
	printf("url:%s\r\n", url);
}

// 解析某一段字符串是否为http请求的字段
// 格式:		key: value
// 这段解析根据chrome浏览器的格式，所以是针对性的，可能不适合其他地方，例如冒号后面有一个空格，程序中直接跳过而不是对空格判断
void parseRequest(uint8_t *recBuf, uint8_t buflen, struct httpEntry *http)
{

	uint8_t len = 0;
	uint8_t *src = recBuf;
	struct httpEntry *httpNext = osal_malloc(sizeof(struct httpEntry));
	while (http->next)
	{
		http = http->next;
	}
	http->next = httpNext;
	while (*recBuf != ':' && *recBuf)
	{
		if (len >= buflen)
		{
			break;
		}
		len++;
		recBuf++;
	}
	uint8_t *index = osal_malloc(len + 1);
	memcpy(index, src, len);
	*(index + len) = '\0';
	recBuf += 2;
	len += 3;
	src = recBuf;

	uint8_t *value;

	// 如果非正常情况下，会导致buflen(一行数据的长度)也没有找到":",应该是错误的
	// 未经过验证
	if (buflen >= len)
	{
		value = osal_malloc(buflen - len + 1);
		memcpy(value, src, buflen - len);
		*(value + buflen - len) = '\0';
	}
	else
	{
		value = osal_malloc(1);
		*value = '\0';
	}

	httpNext->key = index;
	httpNext->value = value;
	httpNext->next = NULL;
}

// post内容
void postContent(uint8_t *recBuf, uint8_t buflen, struct httpEntry *http)
{
	struct httpEntry *httpNext = osal_malloc(sizeof(struct httpEntry));
	while (http->next)
	{
		http = http->next;
	}
	http->next = httpNext;

	uint8_t *index = osal_malloc(8);
	uint8_t *value = osal_malloc(buflen + 1);
	memcpy(index, "Content", 7);
	*(index + 7) = '\0';
	memcpy(value, recBuf, buflen);
	*(value + buflen) = '\0';
	httpNext->key = index;
	httpNext->value = value;
	httpNext->next = NULL;
}

// 保存http请求头的信息
void http_request_msg(uint8_t *recBuf, struct httpEntry *http_request)
{
	uint8_t times = 0;
	uint8_t result = 0;
	uint32_t len;
	uint8_t *startBuf = recBuf;
	while (1)
	{
		result = readline(recBuf, &len);
		if (result != 0)
		{
			// printf("result:%d\r\n",result);
			if (result == 3) // 检测到有空的一行,\r\n	2字节
			{
				recBuf += 2; // 偏移2字节，去掉空行
				uint16_t contentLen = strlen(startBuf) - (recBuf - startBuf);
				printf("contentLen:%d\r\n", contentLen);
				if (contentLen) // 代表空行后面还有内容,很有可能是post过来的内容
				{
					postContent(recBuf, contentLen, http_request);
				}
			}
			break;
		}
		// 添加一个保存key与value的链表
		times++;
		if (times == 1)
		{
			// 第一次时解析http的get的头部,格式与后面数据不一样
			parseGet(recBuf, http_request);
		}
		else
		{
			parseRequest(recBuf, len, http_request);
		}
		recBuf += len;
	}
}

// 释放http请求头的空间
void free_request(struct httpEntry *http)
{
	struct httpEntry *next = http;
	struct httpEntry *current;
	while (1)
	{
		if (next->next)
		{
			current = next;
			next = next->next;
			osal_free(current->key);
			osal_free(current->value);
			osal_free(current);
		}
		else
		{
			if (next->value)
			{
				osal_free(next->value);
			}

			if (next->key)
			{
				osal_free(next->key);
			}
			osal_free(next);

			break;
		}
	}
}

// 打印接收到的请求字段，现在按使用的是GET字段，其他都没有进行判断
void print_request(struct httpEntry *http)
{
	struct httpEntry *next = http;
	while (1)
	{
		printf("%s:%s\r\n", next->key, next->value);
		if (next->next)
		{
			next = next->next;
		}
		else
		{
			break;
		}
	}
}

// 返回  NULL代表没有匹配成功			      url:返回匹配成功的地址
// 地址的匹配
// 可能要修改web的遍历方式，这样增加的时候可以改动小一点
// 暂时主要识别GET /test HTTP/1.1这部分字段
struct url *parsePath(struct httpEntry *http)
{
	uint8_t i = 0;
	struct url *parseWeb;
	if (!strcmp(http->key, "GET"))
	{
		printf("enter GET!\r\n");
		if (http->value[0] != '/')
		{
			printf("not GET request!!!\r\n");
			return NULL;
		}
		// 第一个保留为404页面
		// 循环寻找web的get方法

		parseWeb = &getweb;
		while (parseWeb)
		{

			// printf("web%d url:%s\tvalue:%s\r\n",i,web[i].url,&http->value[1]);
			// 完全匹配
			if (parseWeb->type == 1)
			{
				if (!strcmp(&http->value[1], parseWeb->url))
				{
					return parseWeb;
				}
			}
			// 前部分匹配
			else if (parseWeb->type == 2)
			{
				uint8_t len;
				len = strlen(parseWeb->url);
				uint8_t *pattern = osal_malloc(len + 1);
				memcpy(pattern, &http->value[1], len);
				*(pattern + len) = '\0';
				// printf("pattern:%s\r\n",pattern);
				if (!strcmp(pattern, parseWeb->url))
				{
					http->pattern = &http->value[1 + len];
					osal_free(pattern);
					return parseWeb;
				}
				osal_free(pattern);
			}
			else
			{
				printf("no parsePath!\r\n");
				return NULL;
			}
			if (!parseWeb->next)
			{
				break;
			}
			parseWeb = parseWeb->next;
		}
	}

	else if (!strcmp(http->key, "POST"))
	{
		printf("enter POST!\r\n");
		if (http->value[0] != '/')
		{
			printf("not POST request!!!\r\n");
			return NULL;
		}
		// 第一个保留为404页面
		// 循环寻找web的get方法

		parseWeb = &postweb;
		while (parseWeb)
		{

			// printf("web%d url:%s\tvalue:%s\r\n",i,web[i].url,&http->value[1]);
			// 完全匹配
			if (parseWeb->type == 1)
			{
				if (!strcmp(&http->value[1], parseWeb->url))
				{
					printf("parsePath1 correct:%s!!!\r\n", &http->value[1]);
					return parseWeb;
				}
			}
			// 前部分匹配
			else if (parseWeb->type == 2)
			{
				uint8_t len;
				len = strlen(parseWeb->url);
				uint8_t *pattern = osal_malloc(len + 1);
				memcpy(pattern, &http->value[1], len);
				*(pattern + len) = '\0';
				// printf("pattern:%s\r\n",pattern);
				if (!strcmp(pattern, parseWeb->url))
				{
					http->pattern = &http->value[1 + len];
					printf("parsePath2 correct:%s!!!\r\n", http->pattern);
					osal_free(pattern);
					return parseWeb;
				}
				osal_free(pattern);
			}
			else
			{
				printf("no this parseWeb type:%d!\r\n", parseWeb->type);
				printf("parseWeb:%X\r\n", parseWeb);
				return NULL;
			}
			if (!parseWeb->next)
			{
				break;
			}
			parseWeb = parseWeb->next;
		}
	}

	// printf("not correct path!!!\r\n");

	// 没有匹配的，返回第一个，都是null,没有太多实际作用
	return &getweb;
}

const uint8_t reponse2[] = "Hello!!!";

// 释放httpClient的资源，包括socket和结构体其他需要释放的空间
void closeRes(struct httpClient *httpClient)
{
	if (httpClient->pattern)
	{
		printf("pattern:%s\r\n", httpClient->pattern);
		osal_free(httpClient->pattern);
	}
	if (httpClient->http_request)
	{
		free_request(httpClient->http_request);
	}
	osal_socket_close(httpClient->fdClient);
}
// 创建http头部和响应码
struct httpresp *http_create_reply(int code, char *reply)
{

	uint32_t len;
	struct httpresp *resp;
	resp = osal_malloc(sizeof(struct httpresp));
	if (!resp)
	{
		return NULL;
	}
	resp->headbuf = osal_malloc(1024);
	resp->msgMaxLen = 1024;
	resp->msgLen = 0;
	resp->stat = 0;
	len = sprintf(resp->headbuf, "HTTP/1.0 %d %s\r\n", code, reply);
	resp->msgLen += len;
	return resp;
}

// 头部添加
void http_add_header(struct httpresp *resp, char *name, char *value)
{
	if (resp->stat || !resp || !resp->headbuf)
	{
		printf("http_add_header err!\r\n");
		return;
	}
	uint32_t len;
	uint32_t headLen;
	headLen = strlen(name) + strlen(value) + 4; //  包括4字节的（: \r\n)
	if (headLen + resp->msgLen > resp->msgMaxLen)
	{
		resp->stat = 1;
		return;
	}

	sprintf(resp->headbuf + resp->msgLen, "%s: %s\r\n", name, value);
	resp->msgLen += headLen;
}

// 头部添加
void http_download_header(struct httpresp *resp, char *name, char *value, char *filename)
{
	if (resp->stat || !resp || !resp->headbuf)
	{
		printf("http_add_header err!\r\n");
		return;
	}
	uint32_t len;
	uint32_t headLen;
	headLen = strlen(name) + strlen(value) + strlen(filename) + 4; //  包括4字节的（: \r\n)
	if (headLen + resp->msgLen > resp->msgMaxLen)
	{
		resp->stat = 1;
		return;
	}

	sprintf(resp->headbuf + resp->msgLen, "%s: %s%s\r\n", name, value, filename);
	resp->msgLen += headLen;
}

// http头部发送,name应该是contentLen字段
// 头部的最后信息应该是内容的长度
int http_header_send(struct httpresp *resp, char *name, uint32_t len, int fd)
{
	int res;
	char contentLen[10];
	sprintf(contentLen, "%d\r\n", len);
	http_add_header(resp, name, contentLen);
	if (resp->stat || !resp || !resp->headbuf)
	{
		free_http_head(resp);
		printf("http header err!\r\n");
		return -2;
	}
	res = osal_socket_send(fd, resp->headbuf, strlen(resp->headbuf), 0);
	free_http_head(resp);
	return res;
}

// 404返回
void http_send_error(struct httpClient *httpClient)
{
	struct httpresp *head;
	int fd = httpClient->fdClient;
	head = http_create_reply(404, "Not Found");
	http_add_header(head, "Connection", "close");
	http_add_header(head, "Content-Type", "text/html");
	http_header_send(head, httpIndex.Length, strlen(http404), fd);
	osal_socket_send(fd, http404, strlen(http404), 0);
	closeRes(httpClient);
}

// 文件一次读取最大值
#define HTTPREADLEN 8 * 1024 + 0x10

// 文件内容发送，传递的fp已经偏移好位置
int http_file_send(void *fp, int fd)
{
	uint32_t len;
	uint32_t total_len = 0;
	int res = 0;
	uint8_t *fileContent = osal_malloc(HTTPREADLEN);
	uint8_t *buf = ((uint32_t)fileContent + 0x10) & (~0x0f);
	printf("content:%X\r\n", fileContent);
	printf("buf:%X\r\n", buf);
	while (1)
	{
		len = osal_fread(buf, 1, HTTPREADLEN - 0x10, fp);
		total_len += len;
		// printf("http file send:%d\r\n",len);
		if (len == 0)
		{

			osal_fclose(fp);
			// int iRecvLen = osal_socket_recv( fd, fileContent, 1023, 0);
			osal_free(fileContent);
			printf("total_len:%d\r\n", total_len);
			// printf("http_file_send iRecvLen:%d\r\n",iRecvLen);
			return res;
		}
		res = osal_socket_send(fd, buf, len, 0);
		if (res == -1)
		{
			printf("socket err!\r\n");
			osal_fclose(fp);
			osal_free(fileContent);
			return res;
		}
	}
}

int http_send1(void *fp, int fd, uint32_t size)
{
	uint32_t readlen = HTTPREADLEN - 0x10;
	int len;
	int res;
	// 为了16byte对齐,所以要增加0x10
	uint8_t *fileContent = osal_malloc(HTTPREADLEN);

	if (!fileContent)
	{
		// Todo:申请空间不足
		return 1;
	}
	// 进行16byte对齐
	uint8_t *buf = ((uint32_t)fileContent + 0x10) & (~0x0f);
	while (1)
	{
		if (size < readlen)
		{
			readlen = size;
		}

		len = osal_fread(buf, 1, readlen, fp);
		if (len == 0)
		{
			res = 3;
			break;
		}
		res = osal_socket_send(fd, buf, len, 0);
		if (res < 0)
		{
			printf("http_send err:%d!\r\n", res);
			res = 2;
			break;
		}
		size -= len;
		if (size == 0)
		{
			res = 0;
			break;
		}
	}
	//osal_fclose(fp);
	osal_free(fileContent);
	return res;
}

// 释放http头部回应的空间
void free_http_head(struct httpresp *resp)
{
	if (resp)
	{
		if (resp->headbuf)
		{
			osal_free(resp->headbuf);
		}
		osal_free(resp);
	}
}

// static void http_reponse()

// 测试下载文件
void http_reponse_File(struct httpClient *httpClient)
{
	int fd = httpClient->fdClient;
	void *fp;
	int res;
	uint8_t path[40];
	// 地址长度不能超过30
	if (strlen(httpClient->pattern) > 30)
	{
		goto FileNameErr;
	}
	// 地址匹配
	printf("fileAddr:%s\r\n", httpClient->pattern);
	sprintf(path, "%s%s", "1:/DCIM/", httpClient->pattern);
	printf("Line:%d\tfunc:%s\n", __LINE__, __FUNCTION__); // portMAX_DELAY
	printf("path:%s\n", path);

	fp = osal_fopen(path, "rb");
	if (fp == NULL)
	{
		goto FileFail;
	}

	struct httpresp *head;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "application/force-download");
	http_add_header(head, httpIndex.Connection, "close");
	// 文件名
	http_download_header(head, httpIndex.Disposition, "attachment; filename=", httpClient->pattern);
	res = http_header_send(head, httpIndex.Length, osal_fsize(fp), fd);
	if (res < 0)
	{
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}

	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	printf("HeadSenderr:%d!\r\n", res);
	osal_fclose(fp);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err!!!\r\n");
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}

// photo文件夹的原图查看
void new_http_reponse_OJpg(struct httpClient *httpClient)
{
	uint8_t path[30];
	void *fp;
	int fd = httpClient->fdClient;
	int res;
	printf("%s\r\n", __FUNCTION__);
	printf("fileName:%s\r\n", httpClient->pattern);
#if 1
	sprintf(path, "%s%s", "1:/", httpClient->pattern);
	printf("path:%s\r\n", path);
	fp = osal_fopen(path, "rb");

	if (fp == NULL)
	{
		goto FileFail;
	}

	printf("Line:%d\tfunc:%s\n", __LINE__, __FUNCTION__); // portMAX_DELAY

	// 头发送
	struct httpresp *head;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, osal_fsize(fp), fd);
	if (res < 0)
	{
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}
#endif
	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	printf("HeadSenderr:%d!\r\n", res);
	osal_fclose(fp);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err!!!\r\n");
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}

// photo文件夹的原图查看
void http_reponse_OJpg(struct httpClient *httpClient)
{
	uint8_t path[30];
	void *fp;
	int fd = httpClient->fdClient;
	printf("%s\r\n", __FUNCTION__);

	// 文件名不能大于12，原图文件判断
	if (strlen(httpClient->pattern) > 12)
	{
		goto FileNameErr;
	}
	printf("fileName:%s\r\n", httpClient->pattern);
	sprintf(path, "%s%s", "1:/DCIM/", httpClient->pattern);
	fp = osal_fopen(path, "rb");

	if (fp == NULL)
	{
		goto FileFail;
	}

	int res;
	printf("Line:%d\tfunc:%s\n", __LINE__, __FUNCTION__); // portMAX_DELAY

	// 头发送
	struct httpresp *head;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, osal_fsize(fp), fd);
	if (res < 0)
	{
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}
	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	printf("HeadSenderr:%d!\r\n", res);
	osal_fclose(fp);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err!!!\r\n");
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}

// NES游戏下载
void http_reponse_NES(struct httpClient *httpClient)
{
	uint8_t path[50];
	void *fp;
	int fd = httpClient->fdClient;
	printf("%s\r\n", __FUNCTION__);

	// 文件名不能大于12，原图文件判断
	if (strlen(httpClient->pattern) > 50)
	{
		goto FileNameErr;
	}
	printf("fileName:%s\r\n", httpClient->pattern);
	sprintf(path, "%s%s", "1:/NES/", httpClient->pattern);
	printf("path:%s\n", path);
	fp = osal_fopen(path, "rb");

	if (fp == NULL)
	{
		goto FileFail;
	}

	int res;
	printf("Line:%d\tfunc:%s\n", __LINE__, __FUNCTION__); // portMAX_DELAY

	// 头发送
	struct httpresp *head;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, osal_fsize(fp), fd);
	if (res < 0)
	{
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}
	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	printf("HeadSenderr:%d!\r\n", res);
	osal_fclose(fp);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err:%s!!!\r\n", path);
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}
extern uint32 getAviThumbnail(void *fp,uint32 *pos);

// AVI缩略图
void http_reponse_AVI(struct httpClient *httpClient)
{
	uint8_t path[30];
	int res;
	uint32 pos,size;
	int fd = httpClient->fdClient;
	void *fp;
	if (strlen(httpClient->pattern) > 12)
	{
		goto ReleaseRes;
	}
	printf("fileName:%s\r\n", httpClient->pattern);
	sprintf(path, "%s%s", "1:/DCIM/", httpClient->pattern);
	//res = http_avi_thumb(path, &avimsg);
	fp = osal_fopen(path, "rb");
	if(fp==NULL)
		goto ReleaseRes;
	size=getAviThumbnail(fp,&pos);
	printf("thumbnail pos:0x%x, size:%d\n",pos,size);
	if (size == 0)
		goto FREE_FILE;

	// http头配置与发送
	struct httpresp *head;
	head = http_create_reply(200, "OK");
	if(head==NULL)
	{
		printf("http create reply err\n");
		goto FREE_FILE;
	}
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, size, fd);
	if (res < 0)
	{
		printf("head send err,socket err:%d!\r\n", res);
		goto FREE_FILE;
	}
	osal_fseek(fp,pos);
	res = http_send1(fp, fd, size);
FREE_FILE:
	osal_fclose(fp);
ReleaseRes:
	closeRes(httpClient);
	return;
}

// photo中的缩略图查看
void new_http_reponse_TJpg(struct httpClient *httpClient)
{
	uint8_t path[30];
	void *fp;
	int thumbsize;
	int fd = httpClient->fdClient;
	sprintf(path, "%s%s", "1:/", httpClient->pattern);
	printf("path:%s\r\n", path);
	fp = osal_fopen(path, "rb");
	if (fp == NULL)
	{
		goto FileFail;
	}

	thumbsize = http_thumb(fp);
	if (thumbsize < 0)
	{
		goto ThumbFail;
	}

	// http头配置与发送
	struct httpresp *head;
	int res;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, thumbsize, fd);
	if (res < 0)
	{
		printf("socket err!\r\n");
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}

	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	osal_fclose(fp);
	printf("HeadSenderr:%d!\r\n", res);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err!!!\r\n");
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ThumbFail:
	printf("http_thumb err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}

// photo中的缩略图查看
void http_reponse_TJpg(struct httpClient *httpClient)
{
	uint8_t path[30];
	void *fp;
	int thumbsize;
	int fd = httpClient->fdClient;
	// 图片文件缩略图的一些判断
	if (strlen(httpClient->pattern) > 12)
	{
		goto FileNameErr;
	}
	printf("fileName:%s\r\n", httpClient->pattern);
	sprintf(path, "%s%s", "1:/DCIM/", httpClient->pattern);
	fp = osal_fopen(path, "rb");
	if (fp == NULL)
	{
		goto FileFail;
	}

	thumbsize = http_thumb(fp);
	if (thumbsize < 0)
	{
		goto ThumbFail;
	}

	// http头配置与发送
	struct httpresp *head;
	int res;
	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "image/jpeg");
	http_add_header(head, httpIndex.Connection, "close");
	res = http_header_send(head, httpIndex.Length, thumbsize, fd);
	if (res < 0)
	{
		printf("socket err!\r\n");
		goto HeadSendErr;
	}

	// 文件发送
	res = http_file_send(fp, fd);
	if (res == -1)
	{
		goto SocketErr;
	}

	// 正常结束
	goto ReleaseRes;

HeadSendErr:
	osal_fclose(fp);
	printf("HeadSenderr:%d!\r\n", res);
	goto ReleaseRes;
FileFail:
	printf("open file fail!\r\n");
	goto ReleaseRes;

FileNameErr:
	printf("file path err!!!\r\n");
	goto ReleaseRes;
SocketErr:
	printf("socket err!!!\r\n");
	goto ReleaseRes;

ThumbFail:
	printf("http_thumb err!!!\r\n");
	goto ReleaseRes;

ReleaseRes:
	closeRes(httpClient);
	return;
}

// 测试http的post方法
void http_post_Test(struct httpClient *httpClient)
{

	struct httpresp *head;
	// uint8_t *http_reponse = osal_malloc(1024);
	int fd = httpClient->fdClient;

	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "application/json");
	http_add_header(head, httpIndex.Connection, "close");
	cJSON *json;
	char *out = NULL;
	struct httpEntry *postContent = httpClient->http_request;
	struct httpEntry *postContentLen = NULL;

	// 读取post的内容和长度
	while (1)
	{
		if (!postContent)
		{
			break;
		}
		if (!strcmp(postContent->key, "Content"))
		{
			break;
		}
		if (!strcmp(postContent->key, "Content-Length"))
		{

			postContentLen = postContent;
		}

		postContent = postContent->next;
	}

	if (postContent)
	{
		json = cJSON_Parse(postContent->value); // 获取整个大的句柄
		int i = 0;

		for (i = 0; i < cJSON_GetArraySize(json); i++)
		{
			cJSON *item = cJSON_GetArrayItem(json, i);

			executeJson(&postSetting, item);
			/*
			if(cJSON_String == item->type)
			{
				printf("%s->%s\r\n",item->string,item->valuestring);
			}
			*/
		}

		// out=cJSON_Print(json);  //这个是可以输出的。为获取的整个json的值
		out = cJSON_PrintUnformatted(json);
		cJSON_Delete(json);

		if (postContentLen)
		{
			printf("%s->%s\r\n", postContentLen->key, postContentLen->value);
		}
	}

	// 如果接收不完整，则会在这里重新去尝试接收,前提已经接收到所有的头部,否则可能还是会出错,尚未做处理
	else
	{
		printf("rec continue\r\n");
		if (postContentLen)
		{
			printf("%s->%s\r\n", postContentLen->key, postContentLen->value);
			uint8_t *recBuf = osal_malloc(1024);
			int iRecvLen = osal_socket_recv(fd, recBuf, 1024, 0);
			recBuf[iRecvLen] = 0;
			printf("rec again len:%d\r\n", strlen(recBuf));
			json = cJSON_Parse(recBuf); // 获取整个大的句柄
			int i = 0;

			for (i = 0; i < cJSON_GetArraySize(json); i++)
			{
				cJSON *item = cJSON_GetArrayItem(json, i);

				executeJson(&postSetting, item);
			}

			out = cJSON_PrintUnformatted(json);
			cJSON_Delete(json);
			osal_free(recBuf);
			printf("rec again finish!\r\n");
		}
	}

	if (out)
	{
		http_header_send(head, httpIndex.Length, strlen(out), fd);
		osal_socket_send(fd, out, strlen(out), 0);
	}

	osal_free(out);
	// osal_free(http_reponse);
	closeRes(httpClient);
}

// 测试http，显示hello
void http_reponse_Test(struct httpClient *httpClient)
{

	struct httpresp *head;
	uint8_t *http_reponse = osal_malloc(1024);
	int fd = httpClient->fdClient;

	head = http_create_reply(200, "OK");
	http_add_header(head, httpIndex.Type, "text/html");
	http_add_header(head, httpIndex.Connection, "close");
	http_header_send(head, httpIndex.Length, strlen(reponse2), fd);

	sprintf(http_reponse, "%s", "Hello!!!");
	osal_socket_send(fd, http_reponse, strlen(http_reponse), 0);

	osal_free(http_reponse);
	closeRes(httpClient);
// user_action(MSG_ID_TAKE_PHOTO);	//模拟测试按键拍照的效果
#if 0
	if(workmode_is(WM_TAKEPHOTO2))
	{
		printf("push wifi!\r\n");
		enter_wmm_mode (WM_WIFI);
	}
	else
	{
		printf("push record2\r\n");
		enter_wmm_mode(WM_TAKEPHOTO2);
	}
#endif
}

void thread_rec_pool_queue(struct httpserver *httpserver)
{
	struct httpClient *httpClient = osal_malloc(sizeof(struct httpClient));
	while (1)
	{
		xQueueReceive(httpserver->thread_pool_queue, httpClient, portMAX_DELAY);
		printf("%s\r\n", __FUNCTION__);
		http_thread_pool(httpClient);
	}
}

// 发送有个队列消息
uint8_t httpqueue_thread_send_pool(struct httpClient *httpClient)
{
	long res;
	res = xQueueSend(httpClient->httpserver->thread_pool_queue, httpClient, 0);
	printf("QueueSend finish:%d!!!\r\n", res);
	if (!res)
	{
		osal_free(httpClient);
		printf("QueueSend fail!\r\n");
	}
}

#if 0
//监听到socket就进入http_thread进行处理
static void http_thread (struct httpClient *httpClient)
{
	//static uint32_t requestTimes = 0;
	printf("xPortGetFreeHeapSize:%d\r\n",xPortGetFreeHeapSize());
	httpqueue_thread_send_pool(httpClient);
	osal_free(httpClient);
  osal_thread_delete(osal_thread_myself());
	return ;

	uint8_t *recBuf;
	struct url *web;
	int  fd = httpClient->fdClient;
	struct httpEntry *http_request = osal_malloc(sizeof(struct httpEntry));
	http_request ->next = NULL;
	http_request->key = NULL;
	http_request->value = NULL;
	http_request->pattern = NULL;
	
	recBuf = osal_malloc(1024);

  	int iRecvLen = osal_socket_recv( fd, recBuf, 1023, 0);
	recBuf[iRecvLen] = 0;
	//printf("buf:%s\r\n",recBuf);
	//printf("iRecvLen:%d\r\n",iRecvLen);

	if(iRecvLen)
	{
		http_request_msg(recBuf,http_request);
		//这里只是判断了GET方法，其他字段都没有去判断
		web = parsePath(http_request);
		if(web && (web->reponse || web->queue))
		{
			//如果部分匹配，则将后面的内容复制出来
			if(http_request->pattern)
			{
				httpClient->pattern = osal_malloc(strlen(http_request->pattern)+1);
				memcpy(httpClient->pattern,http_request->pattern,strlen(http_request->pattern)+1);
			}

			httpClient->web = web;
			httpClient->http_request = http_request;
			if(web->queue)
			{
				web->queue(httpClient);
			}
			else
			{
				web->reponse(httpClient);
			}
	
		}

		//增加404错误，以及一些提示
		//不应该出现在这里
		else
		{
			http_send_error(httpClient);
			printf("gettype:%d\r\n",getweb.type);
			printf("posttype:%d\r\n",postweb.type);
			printf("shouldn't appear here!\r\n");
		}

	}
	else
	{
		osal_socket_close(fd);
		printf("RecErr:iRecvLen:%d\r\n",iRecvLen);
		osal_free(http_request);
	}

	//free_request(http_request);
	osal_free (recBuf);
	osal_free(httpClient);
  osal_thread_delete(osal_thread_myself());
	return ;
}
#endif

// 监听到socket就进入http_thread进行处理
static void http_thread_pool(struct httpClient *httpClient)
{
	// static uint32_t requestTimes = 0;
	uint8_t *recBuf;
	struct url *web;
	int fd = httpClient->fdClient;
	struct httpEntry *http_request = osal_malloc(sizeof(struct httpEntry));
	http_request->next = NULL;
	http_request->key = NULL;
	http_request->value = NULL;
	http_request->pattern = NULL;

	recBuf = osal_malloc(1024);

	while (1)
	{
		int iRecvLen = osal_socket_recv(fd, recBuf, 1023, 0);
		printf("iRecvLen:%d\r\n", iRecvLen);
		if (iRecvLen > 0)
		{
			recBuf[iRecvLen] = 0;
			http_request_msg(recBuf, http_request);
			// 这里只是判断了GET方法，其他字段都没有去判断
			web = parsePath(http_request);
			httpClient->http_request = http_request;
			if (web && (web->reponse || web->queue))
			{
				if (web->url)
				{
					printf("web->url:%s\r\n", web->url);
				}
				else
				{
					printf("no found web\r\n");
				}
				// 如果部分匹配，则将后面的内容复制出来
				if (http_request->pattern)
				{
					printf("request->pattern:%s\r\n", http_request->pattern);
					httpClient->pattern = osal_malloc(strlen(http_request->pattern) + 1);
					memcpy(httpClient->pattern, http_request->pattern, strlen(http_request->pattern) + 1);
				}

				httpClient->web = web;
				// httpClient->http_request = http_request;
				if (web->queue)
				{
					web->queue(httpClient);
				}
				else
				{
					web->reponse(httpClient);
				}
			}

			// 增加404错误，以及一些提示
			// 不应该出现在这里
			else
			{
				http_send_error(httpClient);
				printf("shouldn't appear here!\r\n");
			}
			break;
		}
		else if (iRecvLen == 0)
		{
			osal_socket_close(fd);
			printf("RecErr:iRecvLen:%d\t%d\r\n", iRecvLen, fd);
			osal_free(http_request);
			break;
		}
		else
		{
			printf("RecErr:iRecvLen:%d\t%d\r\n", iRecvLen, fd);
			osal_socket_close(fd);
			osal_free(http_request);
			break;
		}
	}
	osal_free(recBuf);
	return;
}

// 客户端

// 创建http  的GET请求
struct httpresp *http_create_GET(char *url)
{

	uint32_t len;
	struct httpresp *resp;
	resp = osal_malloc(sizeof(struct httpresp));
	if (!resp)
	{
		return NULL;
	}
	resp->headbuf = osal_malloc(1024);
	resp->msgMaxLen = 1024;
	resp->msgLen = 0;
	resp->stat = 0;
	len = sprintf(resp->headbuf, "GET /%s HTTP/1.1\r\n", url);
	resp->msgLen += len;
	return resp;
}

int http_GET_header_send(struct httpresp *resp, int fd)
{
	int res;
	if (resp->stat || !resp || !resp->headbuf)
	{
		free_http_head(resp);
		printf("http header err!\r\n");
		return -2;
	}
	res = osal_socket_send(fd, resp->headbuf, strlen(resp->headbuf), 0);
	free_http_head(resp);
	return res;
}

void *http_client_NES(char *url, char *recbuf, u32 recMaxLen, u32 *NesLen)
{
	if (!recbuf)
	{
		return NULL;
	}
	struct sockaddr_in server_addr;
	int iRecvLen;
	u32 Nes_Rec_Len = 0;
	u8 result = 0;
	u32 len;
	char *srcbuf = recbuf;
	char *recEnd = NULL;
	char *recBuf1 = osal_malloc(1460);
	int sock_fd;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.1");
	server_addr.sin_port = htons(80); // æå¡å¨çæ¥æ¶ç«¯å£,å®¢æ·ç«¯ææåéçå½ä»¤é½å°è¾¾è¿ä¸ªç«¯å£
	connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	osal_set_tcp_nodelay(sock_fd, 1);

	printf("sock_fd:%d\n", sock_fd);
	// GET的头部
	struct httpresp *head;
	head = http_create_GET(url);
	http_GET_header_send(head, sock_fd);

	// 接收数据
	while (1)
	{
		iRecvLen = osal_socket_recv(sock_fd, recbuf, 1460, 0);
		if (iRecvLen > 0)
		{
			recbuf += iRecvLen;
			Nes_Rec_Len += iRecvLen;
			// printf("iRecLen:%d\n",iRecvLen);
		}
		else
		{
			break;
		}
	}
	osal_free(recBuf1);
	osal_socket_close(sock_fd);

	printf("Nes http content:%d\n", recbuf - srcbuf);
	recEnd = recbuf;
	recbuf = srcbuf;
	// 解析http获得内容
	while (1)
	{
		result = readline(recbuf, &len);
		printf("result:%d\t%d\r\n", result, len);
		if (result != 0)
		{

			if (result == 3) // 检测到有空的一行,\r\n	2字节
			{
				recbuf += 2; // 偏移2字节，去掉空行
				u32 contentLen = recEnd - recbuf;
				printf("contenLen:%d\n", contentLen);
				*NesLen = contentLen;
				return recbuf;
			}
			break;
		}
		recbuf += len;
	}
	printf("result:%d\n", result);
	*NesLen = 0;
	return NULL;
}

/********************* Initial ********************/
static void do_accept(struct event_info *ei, void *d)
{
	SOCK_HDL fd;
	unsigned int i;
	struct sockaddr_in addr;
	ERR_STR strerr;
	struct httpClient *httpClient = initHttpClient();
	if (!httpClient)
	{
		return;
	}
	struct httpserver *httpserver = (struct httpserver *)d;
	printf("httpserver fd:%d\r\n", httpserver->fdServer);

	i = sizeof(addr);
	if ((fd = osal_socket_accept(httpserver->fdServer, (struct sockaddr *)&addr, &i)) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_warn("error accepting TCP connection: %s", strerr);
		return;
	}
	p_verb("accepted connection from %s:%d\r\n",
		   inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	if (1)
	{
		unsigned char mac[8];
		osal_socket_getmac(fd, mac);
		p_info("MAC %02x-%02x-%02x-%02x-%02x-%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}

	if (osal_set_tcp_nodelay(fd, 1) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_info("error setting TCP_NODELAY on socket: %s", strerr);
	}
	printf("create new thread fd:%d\r\n", fd);
	httpClient->fdClient = fd;
	httpClient->httpserver = httpserver;
	httpClient->time = os_time_get();
	httpClient->pattern = NULL;
	httpClient->http_request = NULL;
	printf("11xPortGetFreeHeapSize:%d\r\n", xPortGetFreeHeapSize());
#if 1
	int nNetTimeout = 1000; // 1秒
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int)) < 0)
	{
		printf("socket_errno:%d\r\n", socket_errno);
		osal_socket_strerr(strerr, socket_errno);
		p_warn("111111error setsockopt TCP connection: %s", strerr);
	}
#endif
	httpqueue_thread_send_pool(httpClient);
	osal_free(httpClient);
	// osal_thread_create(http_thread, httpClient, 15, 0, 4096, "http_thread");
}

static int http_listen(int port)
{
	struct sockaddr_in addr;
	SOCK_HDL fd;
	ERR_STR strerr;
	struct httpserver *httpserver = osal_malloc(sizeof(struct httpserver));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(port);

	if ((fd = osal_socket_open(SOCK_STREAM)) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_err("error creating listen socket: %s\r\n", strerr);
		return -1;
	}

	if (osal_set_reuseaddr(fd, 1) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_warn("ignoring error on setsockopt: %s\r\n", strerr);
	}
	if (osal_socket_bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_err("unable to bind to tcp socket: %s\r\n", strerr);
		osal_socket_close(fd);
		return -1;
	}
	if (osal_socket_listen(fd, 32) < 0)
	{
		//    osal_socket_strerr(strerr, socket_errno);
		//    p_err("error when attempting to listen on tcp socket: %s\r\n", strerr);
		osal_socket_close(fd);
		return -1;
	}
	httpserver->fdServer = fd;
	// httpserver->httpQueue = xQueueCreate(32, sizeof(struct httpClient));
	httpserver->thread_pool_queue = xQueueCreate(32, sizeof(struct httpClient));
	//	osal_thread_create(httpQueue, httpserver, 20, 0, 4096, "httpqueque");

	osal_thread_create(thread_rec_pool_queue, httpserver, 19, 0, 512, "queue1");
	osal_thread_create(thread_rec_pool_queue, httpserver, 19, 0, 512, "queue2");
	osal_thread_create(thread_rec_pool_queue, httpserver, 19, 0, 512, "queue3");
	osal_thread_create(thread_rec_pool_queue, httpserver, 19, 0, 512, "queue4");
	osal_thread_create(thread_rec_pool_queue, httpserver, 19, 0, 512, "queue5");

	initWeb(&getweb);
	default_getweb(&getweb);

	eloop_add_fd(fd, 0, 0, do_accept, (void *)httpserver);

	p_info("listening on http port %s:%d\r\n", inet_ntoa(addr.sin_addr), port);

	return 0;
}

/********************* GLOBAL CONFIGURATION DIRECTIVES ********************/
int config_http(int port)
{
	if (port <= 0 || port > 65535)
	{
		p_err("invalid listen port %d", port);
		return -1;
	}
	return http_listen(port);
}

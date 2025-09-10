#include "mac_fd.h"

struct record_mac
{
	char mac[6];
	UT_hash_handle hh; /* makes this structure hashable */
};
struct record_mac *head_mac = NULL;

struct record_mac *find_mac(const char *mac)
{
	struct record_mac *s;
	portINIT_CRITICAL();
	portENTER_CRITICAL();
	HASH_FIND(hh, head_mac, mac, 6, s);
	portEXIT_CRITICAL();
	return s;
}

// 添加mac地址
void mac_add(const char *mac)
{
	struct record_mac *s;
	s = find_mac(mac);
	// 存在则退出
	if (s)
	{
		return;
	}
	s = osal_malloc(sizeof(struct record_mac));
	memcpy(s->mac, mac, 6);
	portINIT_CRITICAL();
	portENTER_CRITICAL();
	HASH_ADD_KEYPTR(hh, head_mac, s->mac, 6, s);
	portEXIT_CRITICAL();
}

// 删除某个mac地址
void delete_mac(const char *mac)
{
	struct record_mac *s;
	s = find_mac(mac);
	if (s)
	{
		portINIT_CRITICAL();
		portENTER_CRITICAL();
		HASH_DEL(head_mac, s);
		portEXIT_CRITICAL();
		osal_free(s);
	}
}

// 删除record_mac结构体
void delete_record_mac(struct record_mac *s)
{
	if (s)
	{
		portINIT_CRITICAL();
		portENTER_CRITICAL();
		HASH_DEL(head_mac, s);
		portEXIT_CRITICAL();
		osal_free(s);
	}
}

uint32_t mac_num()
{
	return HASH_COUNT(head_mac);
}

// 获取其中一个mac地址,直到没有检测到有mac地址
char *get_one_mac()
{
	struct record_mac *s, *tmp;
	portINIT_CRITICAL();
	portENTER_CRITICAL();

	HASH_ITER(hh, head_mac, s, tmp)
	{
		if (s)
		{
			portEXIT_CRITICAL();
			return s->mac;
		}
	}
	portEXIT_CRITICAL();
	return NULL;
}

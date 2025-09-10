#include "typedef.h"
#include "debug.h"

void *osal_realloc(void *mem_address, uint32_t newsize)
{
  void *ptr = pvPortMalloc(newsize);
  if (ptr)
  {
    if (mem_address)
      memcpy(ptr, mem_address, newsize);
    vPortFree(mem_address);
  }
  else
    p_err("realloc error\r\n");
  return ptr;
}

void *osal_calloc(uint32_t num, uint32_t size)
{
  void *ptr = pvPortMalloc(num * size);
  if (ptr)
  {
    memset(ptr, 0, num * size);
  }
  else
    p_err("calloc error\r\n");
  return ptr;
}

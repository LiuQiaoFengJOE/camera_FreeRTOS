/**@file resfs.c
 * @brief 資源文件系統
 * @details
 * SDK的所有資源，是以獨立的資源文件形式存在的。
 * 為了節省存儲空間，所有資源文件會打包到一個文件之中，成為單一的資源文件。
 *
 * 為了從概念上區分各個獨立的資源文件和打包後的單一文件，後者改稱為資源文件系統。
 * 為了管理各個獨立的資源文件，資源文件系統必須維護一個目錄結構。
 *
 * 當然，這裡不需要複雜文件系統結構，因此，所謂的目錄，也祇需要記錄位置和大小兩個信息。
 *
 * 因此可以描繪出資源文件系統的景象：
 * - 資源文件系統，可以是fatfs中的某一文件，也可以是SPI Flash某處開始的一段數據
 * - 每個資源文件都以resid命名，而非具體的文件名。resid是正整數
 * - 資源文件系統的頭部，列出每個資源的位置和大小，即偏移和長度。resid則正好可以作為列表的下標
 * - 每個資源的偏移，毫無疑問是對於資源文件系統的這個整體而言的。即資源文件系統起始於一個虛擬的0地址：
 *   - 如果資源文件系統保存在fatfs中，此0地址不虛擬了，而是實在的0地址
 *   - 如果資源文件系統是SPI Flash某處開始，資源文件系統本身有一個起始地址，
 *     在使用其中的資源時，資源的偏移就要加上資源文件系統的起始地址
 *
 * 在本文件中，實現的是存在於fatfs中的資源文件系統。
 */

/*************************************************************************
似乎这里需要加入互斥锁,比如读取资源时,seek完后发生任务调度后,再次操作资源
文件,那么原来的fp指向也会被改变,那么返回任务后,读取的数据也会出错,现在
没有出错可能是由于读取资源文件比较少的原因，出现的现象为:有时读取ui或者说
读取文件会概率出错,在现阶段,出现这种概率会很小《Y因为基本只有一个线程去
读取资源文件，其他地方偶尔会有读取资源文件
*************************************************************************/
#include <stdint.h>
#include "asm/osal_semaphore.h"

/**@brief 資源文件信息
 * @details
 * 這是資源文件系統頭部記錄的資源文件信息
 */
typedef struct res_info
{
  int32_t addr;
  int32_t leng;
} res_info_t;

/**@brief 資源管理
 * @details
 * 記錄資源文件系統的管理狀態。
 *
 * 實現為fatfs的一個文件時，利用fp來記錄資源文件系統的保存位置；
 * 如果改為SPI Flash中，則將fp改為addr來記錄資源文件系統在SPI Flash上的位置
 */
typedef struct resfs_ctrl
{
  void *fp;                    // 資源文件系統在fatfs的文件句柄
  uint32_t resid;              // 當前打開的資源id
  struct res_info res;         // 當前打開的資源文件信息
  SemaphoreHandle_t res_mutex; // 资源文件锁
} resfs_ctrl_t;

// 系統祇需要一個資源文件，因此在此靜態管理
static struct resfs_ctrl resfs;
// 實現的是fatfs上的資源文件系統，定義為0
// 如果是SPI Flash，則應定義為resfs.addr；並在初始化時設其初始值
#define RESFS_ADDRESS 0

/**@brief 初始化資源文件系統
 * @details
 * 這裡實現的是，資源文件是某文件系統中的一個文件。
 * 因此position需要傳入的是資源文件名。
 * @param position [in] 資源文件系統的位置
 * @return 資源控制句柄
 */
void *resfs_init(char *position)
{
  resfs.fp = osal_fopen(position, "rb");
  resfs.resid = 0;
  if(resfs.res_mutex==NULL)
  	resfs.res_mutex = xSemaphoreCreateMutex();
  return &resfs;
}

int resfs_fini(void)
{
  xSemaphoreTake(resfs.res_mutex, portMAX_DELAY);
  osal_fclose(resfs.fp);
  xSemaphoreGive(resfs.res_mutex);
  return 0;
}

/**@brief 打開一個資源文件已備使用
 * @details
 * 指定資源ID，讀入該資源文件的信息，以備後續使用。
 * 調用者記錄下返回的資源的起始位置，後續在讀入資源內容時用。
 * @param resid [in] 資源ID
 * @return 資源文件的起始位置
 */
uint32_t res_open(uint32_t resid)
{
  xSemaphoreTake(resfs.res_mutex, portMAX_DELAY);
  uint32_t addr;
  if (resfs.resid != resid)
  {
    resfs.resid = resid;
    osal_fseek(resfs.fp, resid * sizeof(struct res_info));
    osal_fread(&resfs.res, 1, sizeof(struct res_info), resfs.fp);
  }
  addr = RESFS_ADDRESS + resfs.res.addr;
  xSemaphoreGive(resfs.res_mutex);
  return addr;
}

uint32_t res_size(uint32_t resid)
{
  xSemaphoreTake(resfs.res_mutex, portMAX_DELAY);
  uint32_t leng;
  if (resfs.resid != resid)
  {
    resfs.resid = resid;
    osal_fseek(resfs.fp, resid * sizeof(struct res_info));
    osal_fread(&resfs.res, 1, sizeof(struct res_info), resfs.fp);
  }
  leng = resfs.res.leng;
  xSemaphoreGive(resfs.res_mutex);
  return leng;
}

uint32_t res_read(uint32_t addr, void *buff, uint32_t size)
{
  xSemaphoreTake(resfs.res_mutex, portMAX_DELAY);
  osal_fseek(resfs.fp, addr);
  osal_fread(buff, 1, size, resfs.fp);
  xSemaphoreGive(resfs.res_mutex);
  return size;
}

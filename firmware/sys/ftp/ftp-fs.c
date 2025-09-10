/**@file    ftp-fs.c
 * @author  何凱帆
 * @brief   ftp支持
 * @details 提供支持函數
 */

#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "ftp-fs.h"
#include "osal/osal_alloc.h"

#include "debug.h"

static char month_shortNames[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
// 获取当前路径,包括虚拟磁盘的路径部分
void ftp_getcwd(char *buf, unsigned int len)
{
  if (NULL == osal_getcwd(buf + 1, len - 3))
    p_dbg("f_getcwd err\r\n");
  buf[0] = '\"';
  buf[2] = buf[1];
  buf[1] = '/';
  len = strlen(buf);
  buf[len] = '\"';
  buf[len + 1] = '\0';
  p_dbg("ftp_getcwd %s\r\n", buf);
}

int ftp_setcwd(char *path)
{
  uint32_t len;
  if (path[2] == ':')
    path++;
  len = strlen(path);
  if (len > 2 && path[len - 2] != ':' &&
      (path[len - 1] == '/' || path[len - 1] == '\\'))
    path[len - 1] = 0;
  //p_dbg("ftp_setcwd:%s\tpathLen:%d\r\n", path, strlen(path));
  return osal_chdir(path);
}

void *virdisk(char *path)
{
  // 有驱动器号
  if (strlen(path) > 1)
  {
    if (path[0] == '/')
    {
      if (path[1] == '0')
      {
        if (f_chdrive("0:") != 0)
        {
          printf("chdrive fail:%c\n", path[1]);
          return NULL;
        }
      }
      else if (path[1] == '1')
      {
        if (f_chdrive("1:") != 0)
        {
          printf("chdrive fail:%c\n", path[1]);
          return NULL;
        }
      }
      else
      {
        printf("chdrive fail:%c\n", path[1]);
        return NULL;
      }

      // 跳到根目录的path
      if (strlen(path) > 2)
      {
        return path + 2;
      }
      // 有驱动器号的情况下,跳到对应的驱动器根目录
      else
      {
        path[2] = '/';
        path[3] = '\0';
        return path + 2;
      }
    }
    else
    {
      return path;
    }
  }
  // 没有驱动器号,直接返回当前地址
  else
  {
    return path;
  }
}

// return 1:虚拟的根目录    	     2:没有该目录 		     -1:跳转驱动器有误     		 	0:返回除去驱动器后的目录

int ftp_setcwd2(char *path)
{
  char *rootpath = path;
  rootpath = virdisk(path);
  // 虚拟的根目录，在这里有2个虚拟磁盘 flash与sdcard
  if (strlen(path) == 1 && path[0] == '/')
  {
    return 1;
  }
  if (rootpath)
  {
    if (ftp_setcwd(rootpath) == 0)
    {
      return 0;
    }
    char buf[128];
    ftp_getcwd(buf, 128);
    printf("ftp_setcwd err:2\r\n");
    return 2;
  }
  return -1;
}

unsigned int ftp_formatdir(void *HFIL, char *buf, unsigned int len)
{
  unsigned char month;
  unsigned char day;
  unsigned short year;
  unsigned short fdate;
  char *name;

  if (!HFIL)
    return 0;
  //  if( len <= 76 ) return 0;

  name = osal_dirent_name(HFIL);
  if (!name)
    return 0;
  if (name[0] == '.')
  {
    if (!name[1])
      return 0;
    if (name[1] == '.' && !name[2])
      return 0;
  }

  strcpy(buf, "-rw-------    1 1000     1000 ");
  if (osal_dirent_isdir(HFIL))
  {
    buf[0] = 'd';
    buf[14] = '2';
  }

  fdate = osal_dirent_date(HFIL);
  if (fdate != 0)
  {
    month = ((fdate >> 5) & 0x0F); // From 0, or 1?
    day = ((fdate >> 0) & 0x1F);   // From 0, or 1?
    year = ((fdate >> 9) & 0x7F) + 1980;

    if (month == 0 || month > 12)
      month = 1;
    if (day == 0)
      day = 1;
  }
  else
  {
    month = 1;
    day = 1;
    year = 1980;
  }
  sprintf(buf + 30, "%8u %s %02u %04u ",
          osal_dirent_size(HFIL),
          month_shortNames[month - 1],
          day,
          year);
  //  strcat( buf, "May 02 01:00 " );
  //  strcat( buf, "2017-05-02 01:00:00 " );

  len = strlen(name) + strlen(buf);
  strcpy(buf + strlen(buf), name);

  buf[len + 0] = '\r';
  buf[len + 1] = '\n';
  buf[len + 2] = 0;

  return len + 2;
}

FRESULT ftp_mount(FATFS *fs, const TCHAR *path, BYTE opt)
{
  return f_mount(fs, path, opt);
}
FRESULT ftp_chdir(const TCHAR *path)
{
  return f_chdir(path);
}
DIR *ftp_opendir(const TCHAR *path)
{
  DIR *dp;
  FRESULT res;
  dp = osal_malloc(sizeof(DIR));
  res = f_opendir(dp, path);
  if (res == FR_OK)
  {
    return dp;
  }
  printf("opendir fail:%d\t%s!\n", res, path);
  return NULL;
}

FRESULT ftp_closedir(DIR *dp)
{
  FRESULT res = 0;
  res = f_closedir(dp);
  return res;
}
FILINFO *ftp_readdir(DIR *dir)
{

  FILINFO *fno;
  FRESULT res;
  fno = osal_malloc(sizeof(FILINFO));
  res = f_readdir(dir, fno);
  if (res == FR_OK)
  {
    return fno;
  }
  printf("res:%d\n", res);
  printf("readdir fail!\r\n");
  return NULL;
}

FRESULT ftp_stat(const TCHAR *path, FILINFO *fno)
{
  return f_stat(path, fno);
}

/*
-1: 不是jpeg头
-2:	不是自己拍出来的照片
*/
int ftp_thumb(void *fp)
{
  u32 thumbLocate;
  u8 buf[8];
  osal_fread(buf, 1, 2, fp);
  if (buf[0] != 0xff || buf[1] != 0xd8)
  {
    printf("not photo!!!\r\n");
    osal_fseek(fp, 0);
    return -1;
  }
  osal_fseek(fp, 20);
  osal_fread(buf, 1, 8, fp);
  printf("%x\t%x\t%x\t%x\r\n", buf[0], buf[1], buf[2], buf[3]);
  if (buf[0] == 0xff && buf[1] == 0xe1 && buf[2] == 0x00 && buf[3] == 0x06)
  {
    memcpy(&thumbLocate, &buf[4], 4);
    osal_fseek(fp, thumbLocate);
    printf("thumbLocate:%d\r\n", thumbLocate);
    return thumbLocate;
  }
  else
  {
    printf("not our photo!!!\r\n");
    osal_fseek(fp, 0);
    return -2;
  }
}

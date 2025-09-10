#include "fatfs/ff.h"
#include "osal/osal_alloc.h"

// todo: more mode
static uint32_t file_mode(const char *mode)
{
	uint32_t mod;

	if (!mode)
		return 0;

	if (mode[0] == 'r')
		return FA_READ | FA_OPEN_EXISTING;
	if (mode[0] == 'w')
		return FA_READ | FA_WRITE | FA_CREATE_NEW;
	if (mode[0] == 'a')
		return FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
	if (mode[0] == 'e')
		return FA_READ | FA_WRITE;

	return 0;
}

inline void *osal_fopen(const char *name, const char *mode)
{
	FIL *fp;
	FRESULT res;
	uint32_t mod = file_mode(mode);

	fp = osal_malloc(sizeof(FIL));
	if (fp == NULL)
	{
		// todo errno
		printf("osal_fopen malloc fail!\r\n");
		return fp;
	}
	res = f_open(fp, name, mod);
	if (FR_OK != res)
	{
		// todo errno
		printf("osal_fopen [%s] fail:%d\r\n",name, res);
		osal_free(fp);
		return NULL;
	}

	return fp;
}

inline int osal_fclose(void *fp)
{
	if(fp==NULL)
		return EOF;
	FRESULT res = f_close(fp);
	if (FR_OK != res)
	{
		// todo errno
		printf("osal_fclose res:%d\r\n", res);
		osal_free(fp);
		return EOF;
	}
	osal_free(fp);
	return 0;
}

inline uint32_t osal_fread(char *buf, uint32_t is, uint32_t s, void *fp)
{
	uint32_t bw;
	FRESULT res;
	res = f_read(fp, buf, is * s, &bw);
	if (FR_OK != res)
	{
		// todo: errno
		printf("osal_fread err:%d\r\n!!!\r\n", res);
		return 0;
	}

	return bw;
}

inline uint32_t osal_fread2(char *buf, uint32_t is, uint32_t s, void *fp)
{
	uint32_t bw;
	FRESULT res;
	char *temp_buf = NULL;
	char *read_buf_locate = NULL;
	uint32_t buf_addr = (uint32_t)buf;

	uint32_t fp_tell = f_tell((FIL *)fp);
	uint8_t remain = 0;

	// 计算输入buf与未读扇区是否对齐
	if ((buf_addr - fp_tell) % 4)
	{
		remain = fp_tell % 4;
	}

	// printf("fp_tell:%X\t%d\t%d\taddr:%X\terr:%d\r\n",fp_tell,remain,f_tell((FIL*)fp),buf_addr,(buf_addr-fp_tell)%4);
	if (remain || (buf_addr - fp_tell) % 4)
	{
		temp_buf = osal_malloc(is * s + 4); // 申请一个缓冲buf,需要足够大小
		if (!temp_buf)
		{
			printf("osal_fread2 malloc err!\r\n");
			return 0;
		}
		read_buf_locate = temp_buf + remain; // remain是否4byte对齐,不对齐,代表需要填充4-remain才能4byte对齐,所以write_buf_locate长度应该为is*s+4-remain
											 // 对应位置则为temp_buf+remain
	}
	else // 不需要缓冲buf
	{
		read_buf_locate = buf;
	}
	res = f_read(fp, read_buf_locate, is * s, &bw);
	if (FR_OK != res)
	{
		// todo: errno
		// 读取不成功也需要将缓冲buf释放
		if (read_buf_locate != buf)
		{
			osal_free(temp_buf);
		}

		printf("osal_fread err:%d\r\n!!!\r\n", res);
		return 0;
	}
	else // 读取成功的话,判断是否为同一个buf,相同,代表没有申请新的缓冲buf
	{
		if (read_buf_locate != buf)
		{
			memcpy(buf, read_buf_locate, bw);
			osal_free(temp_buf);
		}
	}

	return bw;
}

inline uint32_t osal_fwrite(const char *buf, uint32_t is, uint32_t s, void *fp)
{
	uint32_t bw;
	FRESULT res;
	res = f_write(fp, buf, is * s, &bw);

	if (FR_OK != res)
	{
		// todo: errno
		printf("f_write error:%d,fp:0x%x\r\n", res,fp);
		return 0;
	}

	if (bw != is * s)
	{
		printf("f_write:%d,actual size:%d\n", bw, is * s);
	}

	return bw;
}

inline uint32_t osal_fwrite2(const char *buf, uint32_t is, uint32_t s, void *fp)
{
	uint32_t bw;
	FRESULT res;

	char *temp_buf = NULL;
	char *write_buf_locate = NULL;
	uint8_t shift = 0; // 提前写入字节数
	uint8_t remain = 0;
	uint32_t buf_addr = (uint32_t)buf;

	uint32_t fp_tell = f_tell((FIL *)fp);

	// 计算输入buf与未读扇区是否对齐
	if ((buf_addr - fp_tell) % 4)
	{
		remain = fp_tell % 4; // 需要填充多少byte
	}

	if (remain || (buf_addr - fp_tell) % 4) // 如果需要填充
	{
		if (remain)
		{
			shift = 4 - remain;
		}
		// 将多余的byte先写入
		res = f_write(fp, buf, shift, &bw);
		if (FR_OK != res)
		{
			// todo: errno
			printf("1osal_fwrite2 result %d\r\n", res);
			printf("1fp addr =  %d\r\n", fp);
			return 0;
		}

		temp_buf = osal_malloc(is * s); // 写入不超过is*s
		// 空间不足
		if (!temp_buf)
		{
			printf("osal_fwrite2 malloc err!\r\n");
			return remain; // 已经写入的remain字节
		}
		write_buf_locate = temp_buf;
		memcpy(write_buf_locate, buf + shift, is * s - shift);
	}
	else
	{
		write_buf_locate = buf;
	}

	res = f_write(fp, write_buf_locate, is * s - shift, &bw);
	bw += shift; // 写入的总字节数

	// 无论写不写入,如果申请量的buf,都要释放
	if (temp_buf)
	{
		osal_free(temp_buf);
	}

	if (FR_OK != res)
	{
		// todo: errno
		printf("osal_fwrite2 result %d\r\n", res);
		printf("fp addr =  %d\r\n", fp);
		return 0;
	}

	if (bw == 0)
	{
		printf("bw = %d\rwantsize = %d\r\n", bw, is * s);
	}

	return bw;
}

inline uint32_t osal_ftell(void *fp)
{
	return f_tell(((FIL *)fp));
}

inline uint32_t osal_fsize(void *fp)
{
	return f_size(((FIL *)fp));
}

inline uint32_t osal_fseek(void *fp, u32 ops)
{
	return f_lseek((FIL *)fp, ops);
}

inline char *osal_getcwd(char *buf, uint32_t len)
{
	if (FR_OK != f_getcwd(buf, len))
		return NULL;
	return buf;
}

inline int osal_chdir(char *buf)
{
	int res;
	if (buf[1] == ':')
	{
		f_chdrive(buf);
		if (FR_OK != f_chdir(buf + 2))
			return -1;
	}
	else
	{
		res = f_chdir(buf);
		if (FR_OK != res)
		{
			printf("buf:%s\tres:%d\r\n", buf, res);
			return -1;
		}
	}
	return 0;
}

struct diriter
{
	DIR dir;
	FILINFO fil;
};

inline void *osal_opendir(char *path)
{
	struct diriter *dir;
	dir = osal_malloc(sizeof(struct diriter));
	if (!dir)
		return NULL;

	if (f_opendir(&dir->dir, path) == FR_OK)
		return dir;

	osal_free(dir);
	return NULL;
}

inline void osal_closedir(void *HDIR)
{
	struct diriter *dir = (struct diriter *)HDIR;
	if (!dir)
		return;

	osal_free(dir);
}

inline void *osal_readdir(void *HDIR)
{
	struct diriter *dir = (struct diriter *)HDIR;
	if (!dir)
		return NULL;

	if (f_readdir(&dir->dir, &dir->fil) != FR_OK)
		return NULL;

	if (dir->dir.sect == 0)
		return NULL;
	return &dir->fil;
}

inline char *osal_dirent_name(void *HFIL)
{
	FILINFO *fil = (FILINFO *)HFIL;
	if (!fil)
		return NULL;
	return fil->fname;
}

inline int osal_dirent_isdir(void *HFIL)
{
	FILINFO *fil = (FILINFO *)HFIL;
	if (!fil)
		return 0;
	return fil->fattrib & AM_DIR;
}

inline uint32_t osal_dirent_date(void *HFIL)
{
	FILINFO *fil = (FILINFO *)HFIL;
	return fil->fdate;
}

inline uint32_t osal_dirent_size(void *HFIL)
{
	FILINFO *fil = (FILINFO *)HFIL;
	return fil->fsize;
}

inline uint32_t osal_fmkdir(const char *dir)
{
	return f_mkdir(dir);
}

inline FRESULT osal_stat(const TCHAR *path, FILINFO *fno)
{
	return f_stat(path, fno);
}

inline FRESULT osal_unlink(const TCHAR *path)
{
	return f_unlink(path);
}

inline FRESULT osal_rename(const TCHAR *oldpath, const TCHAR *newpath)
{
	return f_rename(oldpath, newpath);
}

inline FRESULT osal_fchmod(const TCHAR *path, u8 attr, u8 mask)
{
	return f_chmod(path, attr, mask);
}

inline FRESULT osal_fstat(const TCHAR *path, FILINFO *fno)
{
	return f_stat(path, fno);
}

// 判断是否存在某个文件
inline FRESULT osal_fexist(const char *name)
{
	FIL *fp;
	fp = osal_fopen(name, "rb");
	if (fp)
	{
		osal_fclose(fp);
		return 0;
	}

	return 1;
}

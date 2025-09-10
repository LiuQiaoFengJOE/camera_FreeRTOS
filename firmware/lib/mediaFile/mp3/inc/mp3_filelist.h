#ifndef _MP3_FILELIST_H_
#define _MP3_FILELIST_H_


//重新载入文件列表
int MP3_filelist_reload(char *path);

typedef enum
{
	OP_PREV,
	OP_CUR,
	OP_NEXT,
}mp3_op_flag_e;

int MP3_set_cur_file(char* file);

//获取总文件数量
int MP3_get_total_files();

//获取当前文件序号
int MP3_get_item_idx();


//获取指定文件序号的文件名
char* MP3_get_item_name(int idx);


//获取指定序号的完整文件路径
int MP3_get_item_fullpath(int idx, char* buf_ptr, int buf_max_size);


//移动当前文件列表选择项目
int MP3_move_item(mp3_op_flag_e flag);
#endif


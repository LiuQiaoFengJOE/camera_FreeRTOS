#ifndef __FAT_H
#define __FAT_H
void *R_W_fat_mem(int sector, int count);
void *R_fat_mem(int sector, int count);
void *R_W_fat_mem2(int sector, int count);
// #define R_W_fat_mem(a,b) R_W_fat_mem2(__FUNCTION__,__LINE__,a,b)
void print_all_fat_id();
int fat_id_num();
void sort_by_fat_sector();

void release_all_fat_element();
void *get_fat_mem(void *s);
int get_fat_count(void *s);
int get_fat_sector(void *s);
void free_fat_mem(void *s);
void *get_fat_element();
#endif
#ifndef BPB_H_
#define BPB_H_

#include <stdint.h>

#if FATVER == 28
typedef struct DIR_t
{
    uint8_t entry_type;
    uint8_t name[10];
    uint8_t dummy11[9];
    uint16_t clust_high;
    uint8_t dummy22[4];
    uint16_t clust_low;
    uint32_t file_len;
} DIR_t, *pDIR_t;
#else
typedef struct DIR_t
{
    uint8_t entry_type;
    uint8_t secondary_flag;
    uint8_t dummy2[6];
    uint32_t valid_data_length;
    uint8_t dummy12[8];
    uint32_t clust;
    uint32_t data_length;
    uint32_t dummy28;
} DIR_t, *pDIR_t;
#endif

#define None_DirectoryEntry 0x00
#define File_DirectoryEntry 0x85
#define StreamExt_DirectoryEntry 0xC0
#define FileName_DirectoryEntry 0xC1

#if FATVER == 28
// FT_FAT32
// FAT的BPB從偏移8開始定義
typedef struct BPB_t
{
    uint8_t byte_per_sec_shift;  // 08
    uint8_t sec_per_clust_shift; // 09
    uint8_t dummy_0A;            // 0A
    // 真正的BPB開始
    uint8_t byte_per_sec[2];  // 0B
    uint8_t sec_per_clust;    // 0D
    uint16_t sec_num_of_FAT_; // 0E

    uint8_t num_of_FAT;            // 10
    uint8_t max_root_entries[2];   // 11
    uint8_t obsolete_total_sec[2]; // 13
    uint8_t media_type;            // 15
    uint16_t sec_per_FAT;          // 16
    // DOS 3.31 BPB
    uint32_t sec_num_of_FAT;       // 18 這裏存放計算過的絕對地址，原本這裏是sec_per_track和num_of_head
    uint32_t sec_num_of_partition; // 1C
    uint32_t total_sec;            // 20

    uint32_t FAT32_sec_per_FAT;   // 24
    uint16_t active_FAT;          // 28
    uint16_t FAT_version;         // 2A
    uint32_t root_clust;          // 2C
    uint16_t sec_num_of_fs_info;  // 30
    uint16_t sec_num_of_the_copy; // 32
    uint8_t alphapp_sys[12];      // 34
    uint8_t drive_num;            // 40
    uint8_t unknown;              // 41
    uint8_t ext_boot_sign;        // 42
    uint8_t volume_ID[4];         // 43
    uint8_t volume_label[11];     // 47
    uint8_t file_sys_type[8];     // 52
} BPB_t, *pBPB_t;
typedef uint32_t FAT_ITEM_t;
#define ERR_FAT_NUM 0xFFFFFF8
#endif

#define ITEMSIZE_LOG2 (FATVER + 15) / 16

#endif /* BPB_H_ */

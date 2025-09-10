#include "BLDR.h"

#define NO_CHKSUM 1
#define INVALID_KEY 0
#define CFG_FUNC 1
#define FLAG_BYTE (CFG_FUNC << 0) | (INVALID_KEY << 1) | (NO_CHKSUM << 2)

#define SECTION(s) __attribute__((section(s)))

extern void dbr_entry(void);
extern void mbr_entry(void);

/* defined at ld-script */
extern uint8_t bpb_ram_base[];

SECTION(".text_dbr")
BLDR_t __true_start = {
    .ver = 0x00020000,
    .magic = "BLDR",
    .chksum = 0,
    .off = 1,
    .flags = FLAG_BYTE,

    .reserved = "0123456789ABCDEF",

    .addr = (uint32_t)bpb_ram_base, // 代码存放地址           0x1c00
    .secno = 2,
    /* fat32 at sector 2 */ // FLASH的位置
    .secs = 1,              // 代码长度
    .entry = dbr_entry,     // 运行位置             0x1cd8
                        // 这部份东西是拷到sram上运行的，目的，再看看

    .chksum_app = 0x01234567,
    .magickey = 0x01234567,
    .spi_param = 0,
    .spinand_cmd = 0x0313C00F,

    .baud = 4, /* todo: never mind */
};

SECTION(".text_mbr")
BLDR_t __mbr_start = {
    .ver = 0x00020000,
    .magic = "BLDR",
    .chksum = 0,
    .off = 1,
    .flags = FLAG_BYTE,

    .reserved = "0123456789ABCDEF",

    .addr = (uint32_t)bpb_ram_base,
    .secno = 1, /* mbr at sector 2 */
    .secs = 1,
    .entry = mbr_entry,

    .chksum_app = 0x67452301,
    .magickey = 0x01234567,
    .spi_param = 0,
    .spinand_cmd = 0x0FC01303,

    .baud = 4, /* todo: never mind */
};

char dbr_hdr[8] SECTION(".data.dbr_hdr");     // boottool回填的      8字节
BPB_FAT32_t dbr_bpb SECTION(".data.dbr_bpb"); // 同上            0x58字节    加起来就0x60字节，刚好跟生成的BIN一样

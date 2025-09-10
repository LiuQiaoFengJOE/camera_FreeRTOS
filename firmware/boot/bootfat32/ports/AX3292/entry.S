/**@file entry.S
 * @brief 從SD卡啟動的支持庫
 * @details
 * 提供從SD卡啟動的支持例程。
 * 此模塊：
 * - 定義與bootbsp的接口
 * - 定義與boottool的接口
 * - 為減少代碼量、同時為了實現與bootbsp的彙編對接，實現DBR/MBR處理流程
 */

#include <or1k-asm.h>

        .macro      PUB_FUNC name
        .global     \name
\name:
        .endm

        .macro      PUB_LABEL name
        .global     \name
\name:
        .endm

        .macro      PUB_VAL name, val
        .global     \name
        .equ        \name, \val
        .endm



////////////////////////////////////////////////////////////////////////////////
// MBR/DBR的處理流程
#define BOOTBSP_STACK_FRAME 8

        .text
handle_mbr:
        // 先將分區表複製到0x80處
        l.movhi     r3, hi(bpb_ram_base+0x80)
        l.ori       r3, r3, lo(bpb_ram_base+0x80)
        l.movhi     r4, hi(bsec_ram_base + 0x1BE)
        l.ori       r4, r4, lo(bsec_ram_base + 0x1BE)
OR1K_DELAYED (
OR1K_INST(
        l.ori       r5, r4, 0x40
),
OR1K_INST(
        l.jal       bootfat32_memcpy2
)
)

        l.movhi     r2, hi(bpb_ram_base+0x80)
        l.ori       r2, r2, lo(bpb_ram_base+0x80)

bootbsp_loop:
        // 先讀入DBR
        l.lwz       r4, 8(r2)
        l.movhi     r3, hi(bsec_ram_base)
        l.ori       r3, r3, lo(bsec_ram_base)
OR1K_DELAYED (
OR1K_INST(
        l.ori       r5, r0, 1
),
OR1K_INST(
        l.jal       bootbsp_readsecs
)
)

        l.addi      r2, r2, 16

handle_one_dbr:
        // 再將BPB複製到BSP中
        l.movhi     r4, hi(bsec_ram_base)             //bsec_ram_base在整个工程找不到，那估计是在ld文件中
        l.ori       r4, r4, lo(bsec_ram_base)         //将r4记录成bsec_ram_base的地址
        l.movhi     r3, hi(bpb_ram_base)
        l.ori       r3, r3, lo(bpb_ram_base)          //将r3记录成bpb_ram_base的地址，0x1c00
OR1K_DELAYED (
OR1K_INST(
        l.ori       r5, r4, 0x80                      //ld文件中的bsec_ram_base为0x1a00,所以r5为0x1a80   
),
OR1K_INST(
        l.jal       bootfat32_memcpy2                 //这里没看到1a00到1a80的过数据，通过打印发现这里是第0号sector的内容
        											  //所以这里要查看一下是哪里将这个sector拷到0x1a00
        											  //已查明，这里是mask里面LOAD bootsec的sram分配地址。。。第三个坑
)
)




OR1K_DELAYED_NOP (
        l.jal       bootfat32_init
)


        l.movhi     r4, hi(load_param+4)                   //bootfat32_start    //.ld中记录为0x00001400
        l.ori       r4, r4, lo(load_param+4)               //后面被PC工具改为0x2080f60,SDRAM的地址，但还没看到有初始化SDRAM的地方，所以要确定

do_find_load_file:
OR1K_DELAYED_NOP (
OR1K_INST(
        l.jal       find_load_file
)
)



        l.sfeqi     r11, ~0      //相等置1，否则清0，现在是不相等，所以清零了
OR1K_DELAYED (
OR1K_INST(
        l.ori       r4, r11, 0   //r11有值的话，会给到r4，R11的值为0x2100000，这个值其实是解压代码的位置
),
OR1K_INST(
        l.bnf       bootbsp_end   //f为0时跳转 ，因为清零了，所以跳转
)
)

        l.movhi     r3, hi(bpb_ram_base+0xC0)           //0x1c00 + 0xc0 = 0x1cc0
        l.ori       r3, r3, lo(bpb_ram_base+0xC0)
        l.sfgeu     r2, r3							    //if(r2 >= r3)	{ [f] set 1}   ,现在应该是置1了


OR1K_DELAYED (
OR1K_INST(
        l.lwz       r4, 4(r1)
),
OR1K_INST(
        l.bnf       bootbsp_loop        //f为0时跳转 ，因为置1了，所以不用跳转
)
)

/*   - 這裡的關鍵是：
 *   - 在加載失敗時構建一個返回流程
 *   - 在加載成功時構建一個跳轉流程
 */
        l.ori       r9, r4, 0
        l.addi      r1, r1, BOOTBSP_STACK_FRAME
        l.lwz       r2, -8(r1)

        l.ori       r11, r0, 2



bootbsp_end:
        // 保存read_func函數指針

//		l.ori      r3,r4,0
//		l.jal	   test_print
//		l.nop

		
		


        l.movhi     r3, hi(bootbsp_readsecs)
        l.ori       r3, r3, lo(bootbsp_readsecs)     
        l.lwz       r5, 0(r3)  
        l.movhi     r6, hi(bootbsp_putchar)         

OR1K_DELAYED (
OR1K_INST(
        l.ori       r6, r6, lo(bootbsp_putchar)     
),
OR1K_INST(
        l.jr        r4                    //这里是跳转到解码unzip去，但有个问题，就是，谁帮他将代码load到SDRAM
        								  //答：在find_load_file里面，会去读去文件系统，对比文件发现是所需文件后，即将这个
        								  //文件load到对应的内存位置
)
)


////////////////////////////////////////////////////////////////////////////////
// void boot_memcpy2(uint16_t *dst, uint16_t *src, uint16_t *src_end)
PUB_FUNC            bootfat32_memcpy2
0:
        l.addi      r4, r4, 2
        l.lhz       r6, -2(r4)                //r4地址的值给到r6,

        l.sfgeu     r4, r5                    //判断两个是否相等

        l.addi      r3, r3, 2           

OR1K_DELAYED (
OR1K_INST(
        l.sh        -2(r3), r6                //r6的值给到r3的地址
),
OR1K_INST(
        l.bnf       0b
)
)

OR1K_DELAYED_NOP (
OR1K_INST(
        l.jr        r9
)
)


////////////////////////////////////////////////////////////////////////////////
// 定義輸出接口
        .section    .bootfat32.param, "ax"
        // struct LOAD_PARAM
PUB_FUNC            load_param
        .long       do_find_load_file // 進一步加載時，調用此函數                          0x18e4
        .long       bootfat32_start   // 加載到這個地址                               0x2080f60
        .long       dbr_entry         // 加載後，運行這個地址     				       0x2100000
        .asciz      "TIGA    BIN"     // 8.3短文件名
        .long       0, 0              // 留空

        .section    .bootfat32.entry, "ax"
dbr_entry:
OR1K_DELAYED (
OR1K_INST(
        l.nop
),
OR1K_INST(
        l.j         handle_one_dbr
)
)

mbr_entry:
OR1K_DELAYED (
OR1K_INST(
        l.nop
),
OR1K_INST(
        l.j         handle_mbr
)
)



////////////////////////////////////////////////////////////////////////////////
// 定義輸入接口
//PUB_VAL             bootbsp_putchar,  bpb_ram_base + 0xC0
//PUB_VAL             bootbsp_readsecs, bpb_ram_base + 0xC8

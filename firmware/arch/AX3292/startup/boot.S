/**@file boot.S
 * @brief 從SD卡啟動的支持庫
 * @details 提供從SD卡啟動的支持例程
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
// struct bootbsp_t bootbsp_param SECTION(".text.bootbsp.param");
        .section    .text.bootbsp.param, "ax"
PUB_FUNC            bootbsp_param
        .long       bpb_ram_base
        .long       3
        .long       3
        .long       bootfat32_dbr

////////////////////////////////////////////////////////////////////////////////
#define BOOTBSP_STACK_FRAME 8

        .section    .text.bootbsp.entry, "ax"
partition_table_end:

PUB_FUNC            bootbsp_putchar_entry
OR1K_DELAYED (
OR1K_INST(
        l.nop
),
OR1K_INST(
        l.j         putchar
)
)

PUB_FUNC            bootbsp_readsecs_entry
OR1K_DELAYED (
OR1K_INST(
        l.nop
),
OR1K_INST(
        l.j         bootbsp_readsecs
)
)

PUB_FUNC            mbr_entry
OR1K_DELAYED (
OR1K_INST(
        l.nop
),
OR1K_INST(
        l.j         mbr_main
)
)


PUB_FUNC            dbr_entry         
PUB_FUNC            dbr_main
        l.sw        -4(r1), r9                            //先将R9 ,R2放在堆栈里
        l.sw        -8(r1), r2       
        l.addi      r1, r1, -BOOTBSP_STACK_FRAME

        l.movhi     r2, hi(partition_table_end)           //这里要记录下来，后面的代码要用到这个的r2
														  //00001cc0 <bootbsp_putchar_entry>
														  //就一打印函数的地址，打印r3的东西
OR1K_DELAYED (
OR1K_INST(
        l.ori       r2, r2, lo(partition_table_end)          //r2指向partition_table_end这个地址
),
OR1K_INST(
        l.j         bootbsp_init
)
)

PUB_FUNC            mbr_main
        l.sw        -4(r1), r9
        l.sw        -8(r1), r2
        l.addi      r1, r1, -BOOTBSP_STACK_FRAME

        l.movhi     r2, hi(partition_table_end - 0x40)						//这里要记录下来，后面的代码要用到这个的r2

OR1K_DELAYED (
OR1K_INST(
        l.ori       r2, r2, lo(partition_table_end - 0x40)
),
OR1K_INST(
        l.j         bootbsp_init
)
)

////////////////////////////////////////////////////////////////////////////////
// void boot_init(uint32_t r3, uint32_t bootfat32_secno, READ_SEC readfunc)
        .section    .text.bootbsp.entry, "ax"
PUB_FUNC            bootbsp_init
        l.addi      r1, r1, -4

        // 保存read_func函數指針
        l.movhi     r3, hi(read_func)                //保存read_func指针
        l.ori       r3, r3, lo(read_func)

        // 初始化串口
OR1K_DELAYED (
OR1K_INST(
        l.sw        0(r3), r5						 //r5是什么？   操蛋的说，这里是MASK代码里面的read_func函数
),
OR1K_INST(
        l.jal       boot_uart_init					 //配置uart，里面会有调用mask的代码
)
)
		//初始化完UART后再向下跑
        // 讀入bootfat32
        l.movhi     r6, hi(bootbsp_param)
        l.ori       r6, r6, lo(bootbsp_param)

        l.lwz       r3, 0(r6)   // addr      0x4400         //这里是从bin中记录下来的数据，有可能是我们生成的，也有可能是后期改动过的
        l.lwz       r4, 4(r6)   // secno     3
        l.lwz       r5, 8(r6)   // secs      3
        l.lwz       r6, 12(r6)  // entry     0x49f0

OR1K_DELAYED (
OR1K_INST(
        l.sw        0(r1), r6   // entry save
),
OR1K_INST(
        l.jal       bootbsp_readsecs                //这个其实是想要运行MASK里面的read_func函数
        											//参数为前面的r3,r4,r5,r6,
        						//这参数的意思为，从第3个sec里面拷3个sec的数据，放到0x1400中，然后执行0x19f0的代码
        						//代码匹配度为，dbr.bin的0x1c00  ===>第2个sec，
        						//根据boottool提示，ax3281-bootfat32.bin为第三个sector，跳去观察一下
        						//观察这个bin得知，这里是将bootfat32.bin整个拷到去0x1400上运行，所以这里是跳到去bootfat32
        						//的dbr_entry中，没错，bootfat32也是有一个dbr_entry的。。。。。
)
)

        l.lwz       r11, 0(r1) // entry restore and return
OR1K_DELAYED (
OR1K_INST(
        l.addi      r1, r1, 4
),
OR1K_INST(
        l.jr        r11
)
)

////////////////////////////////////////////////////////////////////////////////
// void read_sectors(char *ptr, uint32_t secnum, uint32_t secs)
        .section    .text.bootbsp.entry, "ax"
read_func:
        .ascii      "READ"
PUB_FUNC            bootbsp_readsecs
        l.movhi     r6, hi(read_func)
        l.ori       r6, r6, lo(read_func)      //读取read_func的函数，准备去执行
        l.lwz       r7, 0(r6)				   //
OR1K_DELAYED (
OR1K_INST(
        l.addi      r6, r0, 0                  //清掉R6
),
OR1K_INST(
        l.jr        r7                         //运行read_func的函数
)
)


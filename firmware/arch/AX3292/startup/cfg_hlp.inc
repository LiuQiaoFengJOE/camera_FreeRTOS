/**@file cfg_hlp.inc
 * @brief 定義用於的宏
 * @details 祇被bootsect.S包含，專屬於MCU，與MaskROM相關模塊對應
 */


/* 下面是or1k cpu兼容配置，在bootsect.S包含前，需要先包含or1k-asm.h */

#define IDX_INIT_SD4            4*0
#define IDX_SDRAM_INIT          4*1
#define IDX_INIT_PLL            4*2
#define IDX_TABLE_INIT          4*3
#define IDX_SOFT_DELAY          4*4

#define IDX_SHIFT               6
#define IDX_ADD                 0x20DD309C
#define IDX_SUB                 0x20DD309E
#define IDX_EQU                 0x26DD

        .macro      CONFIG_BEGIN
        .word       0xD7E157F8
        .word       0xD7E14FFC
        .word       0x9C21FFF8
        .word       0x9D430000
        .endm

        .macro      CONFIG_END
        .word       0x85210004
        .word       0x85410000
OR1K_DELAYED(
OR1K_INST(.word       0x9C210008),
OR1K_INST(.word       0x44004800)
)
        .endm

        .macro      table_init_begin
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_TABLE_INIT),
OR1K_INST(.word       0x48005000)
)
        .endm

        .macro      mem_set addr, val
        .long       \addr, \val
        .endm

        .macro      bic_orr addr, bval, oval
        .long       1+\addr, \bval, \oval
        .endm

        .macro      table_init_end
        .long       0
        .endm

        .macro      param_end
        .long       0
        .endm

        .macro      sdram_init con, tm
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_SDRAM_INIT),
OR1K_INST(.word       0x48005000)
)
        .word       \con, \tm
        .endm

        .macro      sdc_dw4
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_INIT_SD4),
OR1K_INST(.word       0x48005000)
)
        .endm

        .macro      init_pll cfg, i, f, div
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_INIT_PLL),
OR1K_INST(.word       0x48005000)
)
        .short      \i, \f, \cfg
        .byte       \div
        .byte      0
        .endm

        .macro      soft_delay t
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_SOFT_DELAY),
OR1K_INST(.word       0x48005000)
)
        .word       \t
        .endm

        .macro      return0
        .word       0xA9600000
        .endm

        .macro      return2
        .word       0xA9600002
        .endm

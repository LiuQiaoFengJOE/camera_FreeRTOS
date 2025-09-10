SECTIONS {
    bsec_ram_base   = 0x00004a00;
    bpb_ram_base    = 0x00004c00;
    bootfat32_start = 0x00004400;      
    bootfat32_secs  = (SIZEOF(.text) + 511) /  512;
    RAMBASE         = 0x2000000;/*0x00040000;*/
    __sdram         = 0x2000000;
    SDRAM_SIZE      = 0x02000000 / 4;
    bootbsp_putchar = bpb_ram_base + 0xC0;
    bootbsp_readsecs = bpb_ram_base + 0xC8;

    .text bootfat32_start : {
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata.*)
        *(.bss)
        . = (( . + 0x1FF ) & ~ 0x1FF) - 0x30;
        *(.bootfat32.param)
        *(.bootfat32.entry)
    }
    .bss.buf ALIGN(4) : {
      *(COMMON) /* FAT_SEC_BUFF is here. */
    }

    ASSERT (. <= bpb_ram_base, "bootfat32 too large")

    /* 這裡定義bpb參數與bootbsp的初始化程序複用 */
    .bss.bpb bpb_ram_base : AT(0x1000){
        *(.bss.hdr)
        *(.bss.bpb)
        *(.bss.var)
    }
}


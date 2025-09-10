ENTRY(__true_start)


SECTIONS {
    bsec_ram_base   = 0x00004a00;
    bpb_ram_base    = 0x00004c00;

    bootfat32_start = 0x00004400;
    bootfat32_secs  = (SIZEOF(.text) + 511) /  512;
    RAMBASE         = 0x2000000;/*0x00040000;*/
    __sdram         = 0x2000000;
    SDRAM_SIZE      = 0x02000000 / 4;

    boot_table_init = 0x00104690;
    bootfat32_dbr   = bsec_ram_base - 0x10;
    bootfat32_mbr   = bsec_ram_base - 0x08;

    .text.dbr bsec_ram_base : AT(0) {
        KEEP(*(.data.dbr_hdr))
        KEEP(*(.data.dbr_bpb))
        KEEP(*(.text_dbr))

        BYTE(0)/* Dummy */
        . = (( . + 0x1FF ) & ~ 0x1FF) - 2;
        BYTE(0x55)
        BYTE(0xAA)

    }

    .text.mbr bsec_ram_base : AT(0x200) {
        KEEP(*(.text_mbr))

        BYTE(0)/* Dummy */
        . = (( . + 0x1FF ) & ~ 0x1FF) - 2;
        BYTE(0x55)
        BYTE(0xAA)

    }

    .text bpb_ram_base : AT(0x400) {
      *(.text.bootbsp.param)
      *(.text.bootbsp.init*)
      . = 0xC0;
      *(.text.bootbsp.entry)
      *(.text.bootbsp)
    }
}


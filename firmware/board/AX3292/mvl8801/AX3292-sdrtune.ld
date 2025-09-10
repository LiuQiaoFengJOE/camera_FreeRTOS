SECTIONS {
    bsec_ram_base   = 0x00004a00;
    bpb_ram_base    = 0x00004c00;
    
    boost_start     = 0x00000000;
    boost_max       = 0x2000; /* 似乎要按簇對齊 */
    RAMBASE         = 0x2000000;/*0x00040000;*/
    __sdram         = 0x2000000;
    SDRAM_SIZE      = 0x02000000 / 4;

    boot_table_init = 0x00104690;
    bootfat32_dbr   = bsec_ram_base - 0x10;
    bootfat32_mbr   = bsec_ram_base - 0x08;
    find_load_file  = bsec_ram_base - 0x30;
    putchar         = bpb_ram_base + 0xC0;
    readsecs        = bpb_ram_base + 0xC8;

    .text.boost0 boost_start : AT(0) {
      KEEP(*step0*.o(
        .text.boosthdr
      ))
      *(.text.boostentry.0)
      *step0*.o(
        .text*
        .rodata*
        .data*
      )
      ASSERT (. <= boost_max, "boost0 too large");
    }

    .text.boost1 boost_start : AT(boost_max) {
      KEEP(*step1*.o(
        .text.boosthdr
      ))
      *(.text.boostentry.1)
      *step1*.o(
        .text*
        .rodata*
        .data*
      )
      ASSERT (. <= boost_max, "boost1 too large");
    }

    .bss boost_start + boost_max : {
        .bss*
        /*COMMON*/
    }
}


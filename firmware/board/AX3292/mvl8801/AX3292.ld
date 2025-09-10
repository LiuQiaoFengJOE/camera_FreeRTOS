MEMORY
{
	boot     : ORIGIN = 0x2000000-0x200, LENGTH = 0x0000200
	ram_boot : ORIGIN = 0x0000000, LENGTH = 0x0004000
	ram_user : ORIGIN = 0x0000000, LENGTH = 0x0005000
	usbfifo	 : ORIGIN = 0x0008000, LENGTH = 0x0001000
	sdram	 : ORIGIN = 0x2000000, LENGTH = 0x00800000
	exsdram	 : ORIGIN = 0x2800000, LENGTH = 0x00800000
}


SECTIONS
{
      
      		/* bootsec */
     /* bootsec */
      .bootsec 0x2000000-0x200 : AT(0x2000000-0x200) 
      {
         KEEP(*(.bootsec))
      } > boot
		
        .exception : AT((LOADADDR(.bootsec) + SIZEOF(.bootsec) + 0x1FF) & 0xfffffe00)
        {
_exception_vma = .;
            *(.vector)
            *(.vector.data)
        } > ram_user
         .fifo (NOLOAD): 
        {
__ufifo_start = .;
            *(.usb_fifo)
        } > usbfifo
__ufifo_end = .;
      /* RAM AREA2: mask variable use + code use + buf use*/
      .ram 0x0000200 (NOLOAD): 
      {
__sram_start = .;
         /* mask rom use var area, must be careful len = __MASK_VARLEN__*/
         *(.msk_var_use) 
         *(.sram_comm)
         *(._sram_use)
         *(._urx_buf_)
         /* code if need run in sram */
         //*(.text.fast_isr)
         //*(.text.jpeg_enc_Isr)
         
         /* buff if need run in sram */
         *(.sdc_buf)
         *(._ep0_fif0_)
         *(._msc_txbuf)
         *(._msc_rxbuf)
         *(._tmfont_)
         *(._qdt_mjp_)
         *(._Tbdri_buff)
         *(._que_j_)
                     
      } > ram_user         
    __sram_end = .;
      /* user code in sdram */
      .text 0x2000000 : AT(0x2000000)
      {
_text_start = .;
         KEEP(*(.text.reset))
         KEEP(*(.text.exception))
         KEEP(*(.reset))
         *(.vector.entry)
         *(.text*)
_text_end1 = .;
         *(.jpeg_dripool)
         *(.eh_frame) 
         *(.rodata*) 
         . = ALIGN(4);
_sensor_head_start = .;
	 	  KEEP(*(.sensor.header.items))
_sensor_head_end = .;
		 . = ALIGN(4);
_lcd_head_start = .;
	 	 KEEP(*(.lcd.header.items))
_lcd_head_end = .;

/* section information for finsh shell */
		. = ALIGN(4);
__fsymtab_start = .;
		KEEP(*(FSymTab))
__fsymtab_end = .;

         *(.data*)
         
      } > sdram
_text_end = .;
      
      _text_sec = 0x1;   /*start lba address for boot_loader upload in SPI-FLASH */   
      _text_len = ((_text_end - _text_start)+0x1ff) / 512;      /*code lba length */   
      
      /* bss in sdram */
      .bss ALIGN(4) (NOLOAD):  
      {
_bss_start = .;
         *(.bss*)
         *(COMMON)
         *(.big_buffer*)
      } > sdram
_bss_end = .;
end = ALIGN(4); 
      __bss_len = (_bss_end - _bss_start);


/*      
   .resource : AT((_text_end + 0x1FF) & 0xfffffe00)
   {
      _res_start = .;
      *(.res_osd_lut)
      
      _res_gamma_start = .;
      *(.res_lcd_gamma)
      
      _res_osd_start = .;
      *(.res_osd)
      
      _res_jpeg_table_start = .;
      *(.res_jpeg_table)
      
      _res_end = .;
   } > sdram
*/   
   _mem_end = 0x2800000;
   . = ALIGN(4);
   bss_end = .;
   _sdram_heap_start = . + 0x10;
   . = _sdram_heap_start;
   . = _mem_end - 0x10000;
   _sdram_heap_end = .;
   /*. = ALIGN(4);
   ucHeap = .;
   _heap_start = .;*/    
   /*4M~0x7F0000*/
   /*. = _mem_end - 0x10000;
   _heap_end = .;   
   configTOTAL_HEAP_SIZE = . - ucHeap;*/
   
   /*addr of resource in spiflash,uint:byte
   _resource_start_addr = LOADADDR(.resource) - LOADADDR(.bootsec); 
   _res_osd_lut_addr    = _resource_start_addr;
   _res_lcd_gamma_addr  = _resource_start_addr + (_res_gamma_start - _res_start);
   _res_osd_addr        = _resource_start_addr + (_res_osd_start - _res_start);
   _res_jpeg_tab_addr   = _resource_start_addr + (_res_jpeg_table_start - _res_start);
   
   _res_jpeg_tab_size   = _res_end - _res_jpeg_table_start;
   */   
}
    PROVIDE(_stack_base = _mem_end - 4);
    /* PROVIDE(__stack = 0x2000 - 4); */

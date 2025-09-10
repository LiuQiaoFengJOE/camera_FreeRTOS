SECTIONS
{/*
  .dummy 0x10000  (NOLOAD) : AT (0x10000)
  {
  }*/

  /* sensor資源由四個部分組成 */
  /* 第一部分：頭部 _Sensor_Header_
   * 祇含有目錄偏移和目錄長度
   */
  .header 0 : AT (0)
  {
    LONG (ADDR(.directory));
    LONG (SIZEOF(.directory));
  }
  /* 第二部分：默認表格 _Sensor_Common_
   * gamma/contract表格，16字節，緊隨在頭部之後
   */
  .common 64 :
  {
    * (.sensor_res.header.common)
  }
  /* 第三部分：目錄 _Sensor_Ident_
   * 每個sensor有一個目錄，保存各表格的位置大小和iic ID信息
   * 頭部信息指示了此部分的位置和大小
   */
  .directory :
  {
    * (.sensor_res.header.items)
  }
  /* 第四部分：內容
   * 第二、三部分祇是表格的指針，表格內容全部集中在這裡
   * 指針是一個偏移量，相對頭部而言
   */
  .text :
  {
    * (
        .text*
        .data* .data.rel.*
        .bss*
        /* 每個sensor資源的前面是函數區，到_Sensor_Adpt_為止 */
        .sensor_res.struct .rodata*
      )
  }
}

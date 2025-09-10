SECTIONS
{
  .header 0 : AT (0)
  {
    * (.lcd_res.struct)
  }
  .directory :
  {
    * (.lcd_res.init_tab)
  }
}

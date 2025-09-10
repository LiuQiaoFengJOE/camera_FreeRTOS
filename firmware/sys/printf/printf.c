#include <stdarg.h>

#include "typedef.h"
#include "board_config.h"

#include "osal/osal_irqflags.h"

__attribute__((weak))

#if LED_MODE == STARTUART
int
putchar(int ch)
{
  return ch;
}

/*******************************************************************************
 * Function Name  : uart_PutChar_n
 * Description    : '\n' denote newline
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PutChar_n(u8 c)
{
  if ('\n' == c)
  {
    putchar(0x0D);
    putchar(0x0A);
  }
  else
  {
    putchar(c);
  }
}

/*******************************************************************************
 * Function Name  : uart_PutStr
 * Description    : uart output strings
 * Input          : p_str:strings pointer
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PutStr(const char *p_str)
{
  while (*p_str)
  {
    Uart_PutChar_n(*p_str++);
  }
}

/*******************************************************************************
 * Function Name  : Uart_Put_hex
 * Description    : uart output  use hex number
 * Input          : dwHex: unsigned  long number
 *                  bMode: 0: small letter, 1:capital letter
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_Hex(DWORD dwHex, BOOL bMode)
{
  BYTE HexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', bMode ? 'A' : 'a'};
  BYTE aHex[8] = {0};
  int i;

  for (i = 11; i < 16; i++)
  {
    HexTable[i] = HexTable[i - 1] + 1;
  }

  i = 8;
  do
  {
    aHex[--i] = dwHex & 0xf;
    dwHex >>= 4;
  } while (dwHex);

  while (i < 8)
  {
    putchar(HexTable[aHex[i++]]);
  }
}

/*******************************************************************************
 * Function Name  : uart_Put_udec
 * Description    : uart output unsigned decimal number
 * Input          : dwDec:  unsigned  long number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_udec(u32 dwDec)
{
  BYTE aDec[10] = {0};
  int i = 10;

  do
  {
    aDec[--i] = '0' + dwDec % 10;
    dwDec /= 10;
  } while (dwDec);

  while (i < 10)
  {
    putchar(aDec[i++]);
  }
}

/*******************************************************************************
 * Function Name  : uart_Put_dec
 * Description    : uart output signed decimal number
 * Input          : dwDec:  signed  long number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_dec(s32 lDec)
{
  BYTE aDec[10] = {0};
  int i = 10;

  if (lDec < 0)
  {
    putchar('-');
    lDec = ~(lDec - 1);
  }

  do
  {
    aDec[--i] = '0' + lDec % 10;
    lDec /= 10;
  } while (lDec);

  while (i < 10)
  {
    putchar(aDec[i++]);
  }
}

/*******************************************************************************
 * Function Name  : Uart_Printf
 * Description    : uart output character,type:
 *                   %d,%i:signed long decimal number
 *                   %u  :unsigned long decimal number
 *                   %x  :unsigned long hex number(small letter)
 *                   %X  :unsigned long hex number(capital letter)
 *                   %c  :character
 *                   %s  :character string
 * Input          :  *pszStr: char type pointer
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Printf(const char *pszStr, ...)
{
  uint32_t irq_flag;

  // Uart_Init();

  local_irq_save(irq_flag);

  va_list arglist;
  va_start(arglist, pszStr);

  while ('\0' != *pszStr)
  {
    if ('%' != *pszStr)
    {
      Uart_PutChar_n(*pszStr);
    }
    else
    {
      switch (*(++pszStr))
      {
      case '\0':
        Uart_PutChar_n('%');
        pszStr--;
        break;

      case 'd':
      case 'i':
        Uart_Put_dec(va_arg(arglist, long));
        break;

      case 'u':
        Uart_Put_udec(va_arg(arglist, DWORD));
        break;

      case 'x':
        Uart_Put_Hex(va_arg(arglist, DWORD), FALSE);
        break;

      case 'X':
        Uart_Put_Hex(va_arg(arglist, DWORD), TRUE);
        break;

      case 'c':
        Uart_PutChar_n(va_arg(arglist, int));
        break;

      case 's':
        Uart_PutStr(va_arg(arglist, char *));
        break;

      default:
        Uart_PutChar_n('@');
        break;
      }
    }
    pszStr++;
  }

  local_irq_restore(irq_flag);
}

/*******************************************************************************
 * Function Name  : uart_PrintfBuf
 * Description    : uart output character use hex number(capital letter)
 * Input          : *pBuf   :output character pointer
 *                  iDataLen:character length
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PrintfBuf(u8 *str, void *pBuf, int iDataLen)
{
  int i;
  u8 *pTempBuf = (BYTE *)pBuf;
  u8 p = 0, q = 0;
  uint32_t irq_flag;

  local_irq_save(irq_flag);
  Uart_PutStr((char *)str);

  for (i = 0; i < iDataLen; i++)
  {
    if ((0 == i % 32) && (i != 0))
    {
      Uart_Printf(" \n");
    }

    p = pTempBuf[i] & 0x0f;
    q = (pTempBuf[i] >> 4) & 0x0f;
    Uart_Printf("%x", q);
    Uart_Printf("%x ", p);

    // Uart_Printf("0x%X, ", pTempBuf[i]);
  }
  Uart_Printf(" \n");
  local_irq_restore(irq_flag);
}

void uartPrintfBuf(void *pBuf, int iDataLen)
{
  int i;
  u8 *pTempBuf = (BYTE *)pBuf;
  // uart_Printf("pBuffAddr = 0x%X", (int)pBuf);
  for (i = 0; i < iDataLen; i++)
  {
    if (0 == i % 32)
    {
      Uart_Printf("\n");
    }
    Uart_Printf("%X ", pTempBuf[i]);
  }
  Uart_Printf("\n");
}

static char string[2048];
int staInactiveCnt;

int printf(const char *fmt, ...)
{
  va_list ap;
  uint32_t irq_flag;
  //tx r
  
  if(fmt[0]=='S'&&fmt[1]=='T'&&fmt[2]=='A')
  {
    staInactiveCnt++;
  	return;
  }
  local_irq_save(irq_flag);

  va_start(ap, fmt);
  vsprintf(string, fmt, ap);
  va_end(ap);

  Uart_PutStr(string);

  local_irq_restore(irq_flag);
  return 0;
}

int vprintk(const char *fmt, va_list args)
{
  uint32_t irq_flag;

  local_irq_save(irq_flag);

  vsprintf(string, fmt, args);

  Uart_PutStr(string);

  local_irq_restore(irq_flag);
  return 0;
}

int puts(const char *string)
{
  uint32_t irq_flag;
  if(string[0]=='t'&&string[1]=='x'&&string[2]==' ')
  	return;
  local_irq_save(irq_flag);

  Uart_PutStr(string);
  while (*string)
    string++;
  if (*(--string) != '\n')
    Uart_PutStr(" \n");

  local_irq_restore(irq_flag);
  return 0;
}

#else

int
putchar(int ch)
{
  return ch;
}

/*******************************************************************************
 * Function Name  : uart_PutChar_n
 * Description    : '\n' denote newline
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PutChar_n(u8 c)
{
}

/*******************************************************************************
 * Function Name  : uart_PutStr
 * Description    : uart output strings
 * Input          : p_str:strings pointer
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PutStr(const char *p_str)
{
}

/*******************************************************************************
 * Function Name  : Uart_Put_hex
 * Description    : uart output  use hex number
 * Input          : dwHex: unsigned  long number
 *                  bMode: 0: small letter, 1:capital letter
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_Hex(DWORD dwHex, BOOL bMode)
{
}

/*******************************************************************************
 * Function Name  : uart_Put_udec
 * Description    : uart output unsigned decimal number
 * Input          : dwDec:  unsigned  long number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_udec(u32 dwDec)
{
}

/*******************************************************************************
 * Function Name  : uart_Put_dec
 * Description    : uart output signed decimal number
 * Input          : dwDec:  signed  long number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Put_dec(s32 lDec)
{
}

/*******************************************************************************
 * Function Name  : Uart_Printf
 * Description    : uart output character,type:
 *                   %d,%i:signed long decimal number
 *                   %u  :unsigned long decimal number
 *                   %x  :unsigned long hex number(small letter)
 *                   %X  :unsigned long hex number(capital letter)
 *                   %c  :character
 *                   %s  :character string
 * Input          :  *pszStr: char type pointer
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_Printf(const char *pszStr, ...)
{
}

/*******************************************************************************
 * Function Name  : uart_PrintfBuf
 * Description    : uart output character use hex number(capital letter)
 * Input          : *pBuf   :output character pointer
 *                  iDataLen:character length
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Uart_PrintfBuf(u8 *str, void *pBuf, int iDataLen)
{
}

void uartPrintfBuf(void *pBuf, int iDataLen)
{
}

static char string[2048];
int printf(const char *fmt, ...)
{
}

int vprintk(const char *fmt, va_list args)
{
}

int puts(const char *string)
{
  return 0;
}
#endif

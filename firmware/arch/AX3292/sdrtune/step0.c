#include "sdrtune.h"
#include "sys/sys.h"

#define DLL_TUN 0
#define CLK_TUN 1
#define SDRAM_TUNE_SOURCE CLK_TUN // only METALFIX B1 can use DLL_TUN
#define SDRTUN_DEBG 0

#define CLKTUN_CONST (BIT(6) | BIT(13) | BIT(5) | BIT(12))
#define CLKTUN_OUTPUT_POS 7
#define CLKTUN_INPUT_POS 0

#define DLLTUN_CH1_POS 25
#define DLLTUN_CH2_POS 21
#define DLLTUN_CONST ((0xf << DLLTUN_CH1_POS) | (0xf << DLLTUN_CH2_POS))
#define exact_delay(r1, r2, t) asm(\
  "l.movhi	 %0,  hi(%2)\n"\
  "l.ori	 %0, %0, lo(%2)\n"\
  "l.addi	 %1, r0, 0\n"\
  "0:l.addi  %1,%1,1\n"\
  "l.sfeq	 %0,%1\n"\
  "l.bnf	 0b\n"\
  :"=r"(r1),"=r"(r2):"i"(t)\
  );

static void exception_delay(u32 cnt)
{
	int i, j;
	exact_delay(i,j,cnt);
   // while(cnt--) asm("l.nop");
}

#define SDRAM_CMD_READ_WRITE 0x84
#define SDRAM_CMD_PRE_ALL 6
#define SDRAM_CMD_CBR 3
#define SDRAM_CMD_MRS 2
#define SDRAM_CMD_NOP 1
#define issue_cmd(cmd) SDRHCMD = cmd | BIT(4)
#define wait_SDRC_OK() while ((SDRHCMD & (1 << 6)) == 0)

// #define CLKTUN_CONST 0x00000040
#define CLKTUN_OUTPUT_POS 7

//------------CHIP VISION
#define THUNDERSE_A1 0x00000000
#define THUNDERSE_B1 0x00010000
#define THUNDERSE_B2 0x00030000

static void my_memset(void *dst, u8 val, int cnt)
{
	u8 *pDstTemp = (u8 *)dst;
	while (cnt--)
	{
		*pDstTemp++ = val;
	}
}

static void my_memcpy4(uint32_t *dst, uint32_t *src, int words)
{
	while (words--)
		*dst++ = *src++;
}

static bool my_memcmp4(uint32_t *dst, uint32_t *src, int words)
{
	while (words--)
		if (*dst++ != *src++)
			return false;
	return true;
}

static void delay(uint32_t cnt)
{
	while (cnt--)
		asm("l.nop");
}

static void _Dcache_flush_all(void)
{
	SPR_DCAIR = 0x2000000;
}

static void dcache_enable(bool enable)
{
	if (enable)
	{
		// debgstr("dcache enable\n");
		__sprset(SPR_DCCR, 0);
		_Dcache_flush_all();
	}
	else
	{
		// debgstr("dcache disable\n");
		__sprclr(SPR_DCCR, 0);
	}
}

void my_putchar(char c)
{
#if SDRTUN_DEBG
	if ('\n' == c)
	{
		UART0DAT = 0x0D;
		while ((UART0PND & 0x2) == 0)
			;
		UART0PND |= BIT(1);
		UART0DAT = 0x0A;
		while ((UART0PND & 0x2) == 0)
			;
		UART0PND |= BIT(1);
	}
	else
	{
		UART0DAT = c & 0xff;
		while ((UART0PND & 0x2) == 0)
			;
		UART0PND |= BIT(1);
	}
#endif
}

static void my_puts(char *str)
{
	while (*str)
		my_putchar(*str++);
}

static void putword(uint32_t val)
{
	static const char hex[16] = "0123456789ABCDEF";
	int i;
	char out=0;
	my_putchar('0');
	my_putchar('x');
	for (i = 0; i < 8; ++i)
	{
		if(out||(val>>28))
		{
			out=1;
			my_putchar(hex[val >> 28]);
		}
		val <<= 4;
	}
}

////////////////////////////////////////////////////////////////////////////////

// address align(4), random range(0,8*1024*1024 - 10000)
static u32 tuning_test_addr[] = {
	// 0x026ACC74, 0x023B26C4, 0x027C5B84, 0x023E2C1C, 0x023B5654, 0x027F537C, 0x02508F9C, 0x02232F44,
	// 0x0275D2D0, 0x027D97FC, 0x02656A68, 0x02591464, 0x02684250, 0x0268B960, 0x027A7E2C, 0x020023E8,
	// 0x0275B4FC, 0x026245A8, 0x0232F294, 0x026648EC, 0x0235C43C, 0x0205224C, 0x02229B4C, 0x02565FE8,
	// 0x022D5F84, 0x02106AD8, 0x024045D8, 0x02010780, 0x02519074, 0x0205B72C, 0x0266D718, 0x025683E0,
	0x0264BC04,
	0x0277B338,
	0x02784EE8,
	0x02273F3C,
	0x0210483C,
	0x027741AC,
	0x020A2AA4,
	0x026C2228,
	0x027261C0,
	0x023CC00C,
	0x02233EBC,
	0x021F6194,
	0x02673A34,
	0x02232C68,
	0x025D1644,
	0x023FC228,
	0x02287868,
	0x02601B70,
	0x0244062C,
	0x021978AC,
	0x02468EB0,
	0x025BF570,
	0x02694D20,
	0x02260C74,
	0x026A0190,
	0x02779854,
	0x02585E78,
	0x02140590,
	0x027C8088,
	0x023CE83C,
	0x020E64C0,
	0x021EC9DC,
};

// data,random range(0,0xFFFFFFFF)
static const u32 tuning_test_data[] = {
	// 0x392B944D, 0x4B16C719, 0xD9F72F51, 0xB35C8D7C, 0x0FDF8A0B, 0x5004BA88, 0x60CF8AEE, 0x50FE04B9,
	// 0x36AC5174, 0xFE72698C, 0x6191F295, 0x4FCFAB39, 0xE962B343, 0x355F4C50, 0x26015E00, 0x105B8A35,
	// 0xD3D9209F, 0xBAB9DDC5, 0x9B19F3E9, 0x2A1B0AA1, 0xDDF26143, 0x12D2E044, 0x54B4A12E, 0x08B629A0,
	// 0x91D472A0, 0x79ED30BA, 0x2D73FF91, 0x65DAA782, 0x1F79DE34, 0x412243A8, 0x6115F79F, 0x2709CB25,
	0xFDCFB6F6,
	0x82DC7E0C,
	0x64711906,
	0x94466C03,
	0x02FF4741,
	0xA28D8DF9,
	0xB8311948,
	0x5D73D025,
	0x9BE168E0,
	0xD85EA796,
	0xFD90593D,
	0xF12C429F,
	0x3EF4FC5F,
	0xF0C616BB,
	0x2AE37380,
	0xA8886B3A,
	0xAD4736F6,
	0xC517CF51,
	0x618E6B88,
	0x40D5DA4C,
	0x3012C3F1,
	0x0CF1FB8D,
	0x465AC24D,
	0x55EC889F,
	0x611D1DBB,
	0x876BEF10,
	0xF54E57AE,
	0x6F5E319F,
	0x9A415B44,
	0x847B25A1,
	0xD21EB67C,
	0xB9AF496C,
};

static void sdram_tuning_check_init(void)
{
	if (!(SDRACON1 & (1 << 29)))
	{

		u32 i;
		u32 temp;
		for (i = 0; i < ARRAY_NUM(tuning_test_addr); i++)
		{
			temp = tuning_test_addr[i] & 0xFF1FFFFF; // 确保在2M范围内
			if ((temp & 0xFFFFF) > (0x100000 - 10 * 1024))
				temp -= 10 * 1024; // 确保不能太靠近边界
			tuning_test_addr[i] = temp;

			// if(temp > (0x2000000 + 2 * 1024 * 1024 - 10 * 1024))
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// static bool sdram_tuning_check_byte(u8 * dst)
//{
//	   u32 i = 0;
//
//	   dcache_enable(false);
//	   for(i = 0;i < 0x100;i++)
//	   {
//		   dst[i] = i;
//	   }
//
//	   delay(1);
//	   dcache_enable(true);
//	   for(i = 0;i < 0x100;i++)
//	   {
//		   if(dst[i] != i)
//		   {
//			   return false;
//		   }
//	   }
//	   return true;
// }

static bool sdram_tuning_check_word(u32 *dst)
{
	u32 i = 0;

	//	_dcache_enable(false);
	for (i = 0; i < ARRAY_NUM(tuning_test_data); i++)
	{
		dst[i] = tuning_test_data[i];
	}

	// delay(1);
	//	_dcache_enable(true);
	for (i = 0; i < ARRAY_NUM(tuning_test_data); i++)
	{
		if (dst[i] != tuning_test_data[i])
		{
			return false;
		}
	}
	for (i = 0; i < ARRAY_NUM(tuning_test_data); i++)
	{
		dst[i] = tuning_test_data[i];
	}

	// delay(1);
	// _dcache_enable(true);
	for (i = 0; i < ARRAY_NUM(tuning_test_data); i++)
	{
		if (dst[i] != tuning_test_data[i])
		{
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/* 嘗試非常簡易的檢驗方法 */
#define ADR(a) (((a)&adrmsk) + 0x02000000)
#define ADDR0 0x55555554
#define DATA0 0x33CCCC33
#define ADDR1 0xAAAAAAA8
#define DATA1 0xAA5555AA
#define ADDR2 0xCCCCCCCC
#define DATA2 0x99666699
#define WORD_WRITE(p, i) *(volatile uint32_t *)ADR(ADDR##p) = DATA##p
#define HALF_WRITE(p, i) *(volatile uint16_t *)(ADR(ADDR##p) + i * 2) = DATA##p >> (i * 16)
#define BYTE_WRITE(p, i) *(volatile uint8_t *)(ADR(ADDR##p) + i) = DATA##p >> (i * 8)
#define WORD_READ(p, i) (*(volatile uint32_t *)ADR(ADDR##p))
#define HALF_READ(p, i) ((*(volatile uint16_t *)(ADR(ADDR##p) + i * 2)) << (i * 16))
#define BYTE_READ(p, i) ((*(volatile uint8_t *)(ADR(ADDR##p) + i)) << (i * 8))

static bool sdram_tuning_check(uint8_t sdrsize_shf)
{
	uint32_t adrmsk = (1 << sdrsize_shf) - 1;
	uint32_t data0, data2;

	BYTE_WRITE(0, 0);
	HALF_WRITE(1, 0);
	BYTE_WRITE(0, 1);
	WORD_WRITE(2, 0);
	BYTE_WRITE(0, 2);
	HALF_WRITE(1, 1);
	BYTE_WRITE(0, 3);

	data2 = BYTE_READ(2, 0);
	data0 = HALF_READ(0, 0);
	data2 += BYTE_READ(2, 1);
	if (WORD_READ(1, 0) != DATA1)
		return false;
	data2 += BYTE_READ(2, 2);
	if (HALF_READ(0, 1) + data0 != DATA0)
		return false;
	if (BYTE_READ(2, 3) + data2 != DATA2)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
#define DUMP_MATRIX 1

const uint8_t tune_val_x[] =
	{
		// BIT(12) is set
		// backwards
		//	0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		//	0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		// forwards
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		//	0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		//	0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
};

uint8_t tune_val_y[] =
	{
		// BIT(5) is set
		// forwards
		//	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		//	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		// backwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

const uint8_t tune_val[] =
	{
		// forwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		// backwards
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		//	0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		//	0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
};

const u8 tune_tab_1_clk[] = // out x  b11_7_tab

	{
		// BIT(12) is set
		// backwards
		//	0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		//	0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		// backwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		//	0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
};
const u8 tune_tab_2_clk[] = // IN Y  b4_0_tab

	{
		// BIT(5) is set
		// backwards
		//	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		//	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		// backwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

// #else
const u8 tune_tab_1_dll[] = // 16 phase ：22.5/（0~360）

	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
const u8 tune_tab_2_dll[] = // 16 phase 22.5/（0~360）
	{

		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

#if DUMP_MATRIX
static uint32_t tuning_matrix[32]; // 36保證按4字節對齊
/**@brief 做一次矩陣嘗試
 * @details
 * 以(x0,y0)為起點，以step為步長，向右上方向作矩形擴展，
 * 以此取值進行嘗試
 */
#if 0
static void tuning_round (uint8_t x0, uint8_t y0, uint8_t step, uint32_t init_val)
{
  uint8_t x, y;
  // 首先將調節結果清0
  my_memset (tuning_matrix, 0, 32*4);//sizeof(tuning_round));

  // 矩陣嘗試
  for (x = 0; x < 32; x += step) {
	uint32_t clktune = init_val | (tune_val_x[x] << CLKTUN_OUTPUT_POS);
	for (y = 0; y < 32; y += step) {
	  CLKTUN = clktune | tune_val[y];
	  delay (1000);
	  if (sdram_tuning_check())
		tuning_matrix[x] |= 1<<y;
	}
	WTD_CLR();
  }
uint32_t div = CLKCON1;
CLKCON1 = 1;
putword (div);
for (x=5; x; --x)
  puts ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\r\n\r\n");

  for (x=0; x<32; ++x) {
	for (y=0; y<32; ++y) {
//		if (tuning_matrix[x] & (1<<(index_y[y]-1)))
	  if (tuning_matrix[x] & (1<<y))
		putchar ('O');
	  else
		putchar (' ');
	}
	putchar ('\r');
	putchar ('\n');
  }
puts ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\r\n\r\n");
}
#endif
#endif /* DUMP_MATRIX */

#if SDRAM_TUNE_SOURCE == CLK_TUN
u8 tune_tab_1[] = // out x	b11_7_tab

	{
		// BIT(12) is set
		// backwards
		//	0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		//	0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		// backwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		//	0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
};
u8 tune_tab_2[] = // IN Y  b4_0_tab

	{
		// BIT(5) is set
		// backwards
		//	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		//	0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		// backwards
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		//	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

#else
u8 tune_tab_1[] = // 16 phase ：22.5/（0~360）

	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
u8 tune_tab_2[] = // 16 phase 22.5/（0~360）
	{

		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

#endif

uint32_t max_x0;  //< 記錄最新掃到的最大範圍起始
uint32_t max_y0;  //< 記錄最新掃到的最大範圍起始
uint32_t max_cnt; //< 記錄最新掃到的最大範圍數量

tune_t tune_by; 						   // 仅包含b11_7的tune信息 tune_b11_7
tune_t tune_values[ARRAY_NUM(tune_tab_1)]; // 包含所有的信息
u32 SDRTUN2_CON;

u32 vbg_param;
u32 sdr_tune_source;
u32 chip_vision;

static void dump_max(void)
{
	putword(max_x0);
	putchar(' ');
	putword(max_y0);
	putchar(' ');
	putword(max_cnt);
	my_puts(" found");
}

uint32_t sdram_init(void)
{

	int i;
	SDRCON = 0x181 | (1 << 5); // param[0];
	SDRTM = 0x2297fbf5; 	   // param[1];

	asm("l.nop");
	asm("l.nop");
	asm("l.nop");

	SDRCON |= 0x80;

	exception_delay(200); // minimum 200us, should be 400us

	issue_cmd(SDRAM_CMD_NOP);
	wait_SDRC_OK();

	issue_cmd(SDRAM_CMD_PRE_ALL);
	wait_SDRC_OK();

	for (i = 8; i; --i)
	{
		issue_cmd(SDRAM_CMD_CBR);
		wait_SDRC_OK();
	}

	SDRMRS = 0x32; // param[2];   //CAS latency should be parameter
	issue_cmd(SDRAM_CMD_MRS);
	wait_SDRC_OK();

	return 16;
}

bool sdram_tuning_check_2(uint8_t sdrsize_shf)
{
  uint32_t adrmsk = (1<<sdrsize_shf) - 1;
  uint32_t data0,data2;

  BYTE_WRITE (0,0);
  HALF_WRITE (1,0);
  BYTE_WRITE (0,1);
  WORD_WRITE (2,0);
  BYTE_WRITE (0,2);
  HALF_WRITE (1,1);
  BYTE_WRITE (0,3);

  data2  = BYTE_READ (2,0);
  data0  = HALF_READ (0,0);
  data2 += BYTE_READ (2,1);
  if	  (WORD_READ (1,0) != DATA1) return false;
  data2 += BYTE_READ (2,2);
  if (	   HALF_READ (0,1) + data0 != DATA0) return false;
  if (	   BYTE_READ (2,3) + data2 != DATA2) return false;
  return true;
}


static bool sdram_tuning_check_1(void)
{
	u32 i = 0;
	for(i = 0;i < ARRAY_NUM(tuning_test_addr);i++)
	{
		if(!sdram_tuning_check_word((u32 *)(tuning_test_addr[i])))
			return false;
	}
	return true;
}

void dllcon_init(void)
{

	int i, j;
	if (chip_vision == THUNDERSE_A1)
	{
		LDOCON |= BIT(28);
		// DLLCON |= (BIT(10) | BIT(9)) ;		// LDOS sel ldo vol 11
		DLLCON |= BIT(11);		 // enable ldoen
		exact_delay(i, j, 2400); //(6*2400+5)*6.94ns(144M)=100us
		DLLCON |= BIT(2);		 // SEL VREF=VBG
		DLLCON |= BIT(1);		 // vcdlref=vtv

		DLLCON |= BIT(19);								  // enable DLLEN
		DLLCON |= (BIT(15) | BIT(14) | BIT(13) | BIT(5)); // dll_freqs_12v 200M,111,  lowspeed
		DLLCON |= BIT(12);								  // ICPSEL 1:dcbias 0: selfbias,sel  dcbias

		// DLLCON |= BIT(17);	//DLL_FORCEALL
		DLLCON |= BIT(16);		 // DLL_Forceout
		DLLCON |= BIT(4);		 // DLL START
		exact_delay(i, j, 2400); //(6*2400+5)*6.94ns(144M)=100us

		// exact_delay(i,j,240000); 				//(6*2400+5)*6.94ns(144M)=10ms
		// while((DLLCON & BIT(0))==0);
		//	DLLCON = 0;
	}
	else if (chip_vision == THUNDERSE_B1)
	{
		LDOCON |= BIT(28); // enable VREF_EN
		DLLCON |= BIT(2);  // REFSEL 0: iref 1:vref
		// delay(3) ;  //delay 100ns (500ns)
		// exception_delay(1);

		// DLLCON |= BIT(1); //VTBIAS 0:V TO IV BIAS 1: V TO V BIAS

		// DLLCON |= BIT(10); // LDOS
		// DLLCON |= BIT(9);	// ldos sel ldo vol
		DLLCON |= BIT(11); // enable ldoen

		exact_delay(i, j, 47); // delay 100us (161us)(107US)
		//	exception_delay(100);

		DLLCON |= BIT(19); // enable DLLEN
		exact_delay(i, j, 10);

		DLLCON |= BIT(15); // dll_freqs_12v 200M,110
		DLLCON |= BIT(14); // dll_freqs_12v 200M,110
		// DLLCON |= BIT(13);	//dll_freqs_12v 200M,110
		DLLCON |= BIT(5); // LOWSPEED

		DLLCON |= BIT(12); // ICPSEL 1:dcbias 0: selfbias

		DLLCON &= ~(BIT(7) | BIT(6)); // loops00:16phase;01:12phase;10:10phase;11:8phase
		// DLLCON |= BIT(6);
		// DLLCON |= BIT(7);

		DLLCON |= BIT(17); // DLL_FORCEALL
		DLLCON |= BIT(16); // DLL_Forceout

		// DLLCON |= SDRITUN | SCOTUN;

		// DLLCON |= BIT(3);  //vpullsel 0:pull to 0.8 1:pull to 1.2

		// DLLCON |= BIT(1); //VTBIAS 0:V TO IV BIAS 1: V TO V BIAS

		exact_delay(i, j, 47);
		DLLCON |= BIT(4); // DLL START

		exact_delay(i, j, 10);	 // delay 100ns
		DLLCON |= BIT(18);		 // flocken
		exact_delay(i, j, 2400); // delay 100us->200us 107us
		DLLCON &= ~BIT(18); 	 // flocken DISABLE

		// delay(100000);
		DLLCON |= BIT(8); // dll_lockdet
		exact_delay(i, j, 2400);
		while ((DLLCON & BIT(0)) == 0)
			;
		putchar('2');
	}
	else
		return;
}
#define USE_32DATA_LEN    (sdr_tune_source == CLK_TUN)
u32 tuneData[32];
void tuneDataInit(void)
{
	u8 i;
	for(i=0;i<32;i++)
		tuneData[i]=0;
}
void setTuneDataBit32(u8 x, u8 y)
{
	tuneData[x] = tuneData[x] | (1 << y);
}
void setTuneDataBit16(u8 x, u8 y)
{
	u16 *tuneDataP = tuneData;
	tuneDataP[x] = tuneDataP[x] | (1 << y);
}
void setTuneDataBit(u8 x, u8 y)
{
	if (USE_32DATA_LEN)
		setTuneDataBit32(x,y);
	else
		setTuneDataBit16(x,y);
}
u8 getTuneDataBit32(u8 x, u8 y)
{
	if (tuneData[x] & (1 << y))
		return 1;
	return 0;
}
u8 getTuneDataBit16(u8 x, u8 y)
{
	u16 *tuneDataP = tuneData;
	if (tuneDataP[x] & (1 << y))
		return 1;
	return 0;
}
#define EX_REGION  0
bool tuneDataReginMatch32(u8 x, u8 y, u8 regin)
{
	u8 xEnd, yEnd, i, j;
	xEnd = x + regin+EX_REGION;
	yEnd = y + regin;
	if (getTuneDataBit32(xEnd-1, yEnd-1) == 0)
		return false;
	if (getTuneDataBit32(x, y) == 0)
		return false;
	if (getTuneDataBit32(x, yEnd-1) == 0)
		return false;
	if (getTuneDataBit32(xEnd-1, y) == 0)
		return false;
	
	for (i = x; i<xEnd; i++)
	{
		for (j = y; j<yEnd; j++)
		{
			if (getTuneDataBit32(i, j) == 0)
				return false;
		}
	}
	return true;
}
bool tuneDataReginMatch16(u8 x, u8 y, u8 regin)
{
	u8 xEnd, yEnd, i, j;
	xEnd = x + regin+(EX_REGION/2);
	yEnd = y + regin;
	if (getTuneDataBit16(xEnd-1, yEnd-1) == 0)
		return false;
	if (getTuneDataBit16(x, y) == 0)
		return false;
	if (getTuneDataBit16(x, yEnd-1) == 0)
		return false;
	if (getTuneDataBit16(xEnd-1, y) == 0)
		return false;
	for (i = x; i<xEnd; i++)
	{
		for (j = y; j<yEnd; j++)
		{
			if (getTuneDataBit16(i, j) == 0)
				return false;
		}
	}
	return true;
}
bool getTuneDataInfor(u8 *x, u8 *y, u8 regin)
{
	s8 x1, y1, xEnd, yEnd;
	if (USE_32DATA_LEN)
	{
		xEnd = 32 - (regin+EX_REGION);
		yEnd = 32 - regin;
	}
	else
	{
		xEnd = 16 - (regin+(EX_REGION/2));
		yEnd = 16 - regin;
	}
	if (USE_32DATA_LEN)
	{
		//for (x1 = 0; x1 <= xEnd; x1 += 2)
		for (x1 = xEnd; x1 >=0; x1 --)
		{

			for (y1 = 0; y1 <= yEnd; y1 += 2)
			{
				if (tuneDataReginMatch32(x1, y1, regin))
				{
					*x = x1 + ((regin+EX_REGION) >> 1)+1;
					*y = y1 + (regin >> 1);
					return true;
				}
			}
		}
	}
	else
	{
		//for (x1 = 0; x1 <= xEnd; x1 += 2)
		for (x1 = xEnd; x1 >=0; x1 --)
		{

			for (y1 = 0; y1 <= yEnd; y1 += 2)
			{
				if (tuneDataReginMatch16(x1, y1, regin))
				{
					*x = x1 + ((regin+(EX_REGION/2)) >> 1)+1;
					*y = y1 + (regin >> 1);
					return true;
				}
			}
		}
	}
	return false;
}

#if 0
#define R_ADC_CTRL ADCCON
#define R_ADC_BAUDRATE ADCBAUD
#define R_ADC_DATA ADCDAT
#define R_SYS_CLK PCON0

#define R_SYS_CLK1 PCON1

#define HAL_CFG_ADC_BAUDRATE 1000000UL
u16 adcRead(void)
{
	volatile int i=0x8ffff;
	u16 value;
//	if(R_ADC_CTRL&(ADC_EN))
	 while(R_ADC_CTRL&0x40) //ADC_EOC
	 {
		i--;
		if(i==0)
		{

			break;
		}
	 }
	value = R_ADC_DATA&0x3ff;

	return value;
}
void adcConverStart(void)
{
	R_ADC_CTRL |= 0x80;  // start one change ADC_GO
}
void adcSetBaudrate(u32 baudrate)
{
	R_ADC_BAUDRATE = ( 144000000/(2*baudrate)- 1) & 0x7F;	 //APB_CLK
}
void sysClkSet(u8 clk_type,u8 en)
{
	u32 CLKSFR = (u32)&R_SYS_CLK;
	u8 dev_id = clk_type;
	
	if(clk_type > 31)
	{
		CLKSFR = (u32)&R_SYS_CLK1;
		dev_id = clk_type - 32;
	}
	if(en)
		SFR_AT CLKSFR &=~(1<<dev_id);
	else
		SFR_AT CLKSFR |= (1<<dev_id);
}
void adcEnable(u8 ch,u8 en)
{
	en&=1;
	if(ch>11) //ADC_CH_BGOP
		return ;
	if(en)
	{
		sysClkSet(SYS_CLK_ADC,1);
		R_ADC_CTRL	= (0x10)|(ch);	//ADC_EN

	}
	else
		R_ADC_CTRL	&= ~(0x10);//ADC_EN
	
}

s32 adcGetChannel(u8 ch)
{
	adcEnable(ch,1);
	adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);
	adcConverStart();
	return adcRead();
}

#endif
#if 1
typedef  void (* func_init_pll )(void *);
#define clr_wdt()    WDTCON = 0x5003d
#define soft_delay(t) do {volatile int i=(t)*2; while(i--); clr_wdt(); } while(0)    //10us RC 2M

#define COMBO_CLOCK    144000000

bool check_if_xosc_exist(void)
{	
	u32 i;
	CLKCON0 |= BIT(24); //Timer 32kHz signal source sel : 0:from RC2M; 1:xosc32k 
	TMR0CON = (7<<8) ;
	TMR0PR = 32768/2 -1; 
	TMR0CNT = 0; 
	TMR0CON |= BIT(0); //enable and work on the Counter Mode

	for(i = 0; i < 0xffff; i++){
		if(TMR0CNT > 16){
			//test_dbg_putstr("xosc!\n");
			for(i = 0; i < 20; i++){
				TMR0CNT = 0;
				soft_delay(14000);		   //50ms RC 2.4M
				if(TMR0CNT > 600){
					//test_dbg_putstr("check if xosc ok!\n");
					return true;	
				}	
			}
			return true;	
		}	
	}
	//test_dbg_putstr("no xosc!\n");
	return false;
  /**/	
}

bool init_sys_clk(void){
	bool if_xosc_exist = check_if_xosc_exist();
	u32 rc10M_turn = ((*(u32 *)0x4fa8)& 0xfffc00)>>10;	
	u8 use_rc = 0;

	if(if_xosc_exist)
	{
		func_init_pll init_pll1 = 0x00104424; 
		u16 param[4]; //= {132000000/32768, (132000000%32768)*2, 0,  264000000/132000000-1};
		//prevent complier from using memcpy
		param[0] = COMBO_CLOCK/32768;
		param[1] = (COMBO_CLOCK%32768)*2;
		param[2] = 0;
		param[3] = (COMBO_CLOCK*2)/COMBO_CLOCK-1;
	   
		init_pll1(param);  
		//test_dbg_putstr ("32k\r\n"); 
		
		PLL1DIV = ((200000000/32768)<<16)|((200000000%32768)*2);
		PLL1CON = 0x1323;
		SDRACON0 = 0x3ff;
		CLKCON1 = (CLKCON1 &~ (7<<5))|(1<<5);	//ddr pll divide
		CLKCON0 = (CLKCON0 &~ (1<<22))|(1<<23);  //0:ddrpll 1:syspll

		{//print only for test 
			UART0BAUD = COMBO_CLOCK/115200 -1;
			//while((UART0PND & 0x2)==0);
			UART0PND |= BIT(5)|BIT(4);			  //清除标志位
			UART0CON |= BIT(4);
			//UART0PND |= BIT(1);  
			//UART0DAT = 'b'; 
		}

		//======write flag to rtc ram====
		{
			volatile int timeout = 0x8000;
			use_rc = 0xAA;

			RTCCON |= (1<<8);		 //CE ENABLE 
			timeout = 0x8000;
			RTCDATA = IRTRAM_WR_CMD;	// W CMD
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = 8;				//W addr
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = use_rc;		//W data
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			RTCCON &= ~(1<<8);		 //CE DISABLE	
		}

	}
	else
	{
		if(rc10M_turn == 0)
		{
			//rc10M_turn = 10000;
			//===NO 32K XOS,  IC not RC10M check ,can't run!!!
			while(1);
		}
 
		CLKCON2 |= (1<<16);  //RC10M enable, delay 100us
		CLKCON0 |= BIT(3);	 //RC select RC10M;rc sel:00:rc2M ,10:RC10M
		soft_delay(1);
		CLKCON0 &= ~(BIT(1)|BIT(0)); //system clk choose RC 	

		PLL0DIV  = (COMBO_CLOCK/1000/rc10M_turn)<<16;
		PLL0DIV  |= (COMBO_CLOCK/1000%rc10M_turn)*65536/rc10M_turn;
		PLL0CON = 0x10e7;
		CLKCON1 = (COMBO_CLOCK*2)/COMBO_CLOCK-1;
		// µÈ´ýÖÁÉÙ1mS 
		soft_delay (100);
		// Ïµ½y•rçŠÄRCÇÐ“Qµ½PLL, CLKCON0[1:0]=·Öîl£º0b11£»²»·Öîl£º0x10
		CLKCON0 |= ((COMBO_CLOCK*2)/COMBO_CLOCK-1)?3:2;
		//test_dbg_putstr ("rc10M_turn\r\n");  
		
		PLL1DIV  = (200000/rc10M_turn)<<16;
		PLL1DIV  |= (200000%rc10M_turn)*65536/rc10M_turn;
		PLL1CON = 0x13e7;
		SDRACON0 = 0x3ff;
		CLKCON1 = (CLKCON1 &~ (7<<5))|(1<<5);	//ddr pll divide
		CLKCON0 = (CLKCON0 &~ (1<<22))|(1<<23);  //0:ddrpll 1:syspll

		{//print only for test 
			UART0BAUD = COMBO_CLOCK/115200 -1;
			//while((UART0PND & 0x2)==0);
			UART0PND |= BIT(5)|BIT(4);			  //清除标志位
			UART0CON |= BIT(4);
			//UART0PND |= BIT(1);  
			//UART0DAT = 'c'; 
		}
		//======write flag to rtc ram====
		{
			volatile int timeout = 0x8000;

			RTCCON |= (1<<8);		 //CE ENABLE 
			timeout = 0x8000;
			RTCDATA = IRTRAM_WR_CMD;	// W CMD
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = 8;				//W addr
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = use_rc;		//W data
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			RTCCON &= ~(1<<8);		 //CE DISABLE	
		}

	}
	
}
#endif
#define USE_PRESAVE_TUNE				0
void sdram_clock_tune(void)
{
	// my_puts("sdram_clock_tune");
	init_sys_clk();
	LDOCON = (LDOCON & ~(0x03 << 13)) | ((SYS_VOL_V1_30 & 0x03) << 13); // LDO 1.2V; 0:1.0V, 1: 1.2V, 2: 1.25V, 3: 1.3V

	exception_delay(20);
	//	uint32_t mid;
	WTD_DIS();
	LDOCON |=(1<<15);//adkey bandgap enable11
	//	u16 vbg_set = 0xf;
	//efuse
	u8 vbg_trim=(((*(u32 *)(0x5000-140+0x30 + 8))>>24)& 0x1f);
	if(vbg_trim != 0){
		LDOCON = (LDOCON & ~(0x1f<<16))|((vbg_trim)<<16);
		//vbg_set = vbg_trim;
	}	
	u8 vbg_ofs = (*(u32 *)(0x5000-140+0x30 + 4) & 0x7f);
	if(vbg_ofs != 0){
		vbg_param = vbg_ofs +737;
	}
	else{
		vbg_param =800;
	}
	//LDOCON |= (0xF<<16);//bandgap voltage select (3<<12)
#if 0 //DLL打开前打开后测量1次	
	u16 vbg_adc0,vbg_adc1;
	vbg_adc0 = adcGetChannel(11);//adc0
#endif	

	//-------------read chip id
	chip_vision = CHIP_ID &0xffff0000;

	if(chip_vision == THUNDERSE_A1 )  //B2 do not use dll
	{
		sdr_tune_source = CLK_TUN;
		dllcon_init();
		my_puts("\nthunderse_A1\n");
	}
	if(chip_vision == THUNDERSE_B1 )  //B2 do not use dll
	{
		sdr_tune_source = DLL_TUN;
		dllcon_init();
		my_puts("\nthunderse_B1\n");
	}	
	else if(chip_vision == THUNDERSE_B2 ) 
	{	
		sdr_tune_source = CLK_TUN;
		DLLCON = 0;
		my_puts("\nthunderse_B2\n");
	}
	//delay(200);
	exception_delay(200);
	SPR_DCCR &= ~BIT(0);

	/*************read presave tune value*******/
#if USE_PRESAVE_TUNE
	u32 pre_save_tunevalue = 0;
	u32 crc_check = 0;

	_rtcRamRead(HAL_TUNEVALUE_ADDR,(u8 *)&pre_save_tunevalue,4);
	_rtcRamRead(HAL_CRCCHECK_ADDR,(u8 *)&crc_check,4);


	//pre_save_tunevalue = 0;
	if(pre_save_tunevalue)
	{
		if(crc_check == crc_cal(pre_save_tunevalue))
		{
			if(sdr_tune_source == CLK_TUN){
				CLKTUN = pre_save_tunevalue;  //11_7: OUTPUT, 4_0: INPUT 
			}else{
				DLLCON = pre_save_tunevalue;
			}
			my_putchar('b');
			goto TUNE_END;
		}
		//my_putchar('d');
	}
#endif
	// u32 cnty	= 0,tuny = 0xffffffff;
	// u32 cntx	 = 0,tunx  = 0xffffffff;
	// u32 cnty_temp = 0;
	u32 i = 0,
		j = 0;

		
	SDRTUN2_CON = 0;	
	//_sdr_tuning2_conifg_set(0);


	WTD_DIS();


	u8 regin=9;
	u8 by_id = 0,bx_id = 0, save_tunevale = 0;
	sdram_tuning_check_init();
#if 1//SDRAM_SIZE == SDRAM_SIZE_8M
	uint32_t sdrsize_shf = 23;	//8M  
#else
	uint32_t sdrsize_shf = 21;	//2M
#endif
	uint32_t i_max,j_max;
	if(sdr_tune_source == CLK_TUN)
	{
		i_max = ARRAY_NUM(tune_tab_1_clk);
		j_max = ARRAY_NUM(tune_tab_2_clk); 
	}
	else{
		i_max = ARRAY_NUM(tune_tab_1_dll);
		j_max = ARRAY_NUM(tune_tab_2_dll);
	}
	again:
	if(sdr_tune_source == CLK_TUN)
	CLKTUN |= CLKTUN_CONST;
	else
	CLKTUN &= ~CLKTUN_CONST;


	sdram_init();

	tuneDataInit();
	//  WDTCON = 8 & 7;

	my_putchar('\n');


	for( i = 0; i < i_max; i++)
	{
		for(j = 0;j < j_max;j++)
		{
			if(sdr_tune_source == CLK_TUN)
			{
				//clktun_const = (1<<12) | (1<<13) | (1<<6) | (1<<5);
				CLKTUN = CLKTUN_CONST | (tune_tab_1_clk[i] << CLKTUN_OUTPUT_POS) | (tune_tab_2_clk[j]<<CLKTUN_INPUT_POS);
			}else{
				DLLCON &= ~DLLTUN_CONST;
				DLLCON |= (tune_tab_1_dll[i]<<DLLTUN_CH1_POS)|(tune_tab_2_dll[j]<<DLLTUN_CH2_POS);
			}
			WTD_DIS();
			exception_delay(5);
			sdram_init();
			 if(sdram_tuning_check_2 (sdrsize_shf)&&sdram_tuning_check_1())
			 {	
				my_putchar('1');
				setTuneDataBit(i,j);
			 }
			 else
			 {
				my_putchar('0');
			 }
				WTD_DIS();
		}
		my_putchar('\n');  
	} 
	//select best tuning value
	if (USE_32DATA_LEN)
		regin=11;
	else
		regin=9;
		while(1)
		{
		if(getTuneDataInfor(&bx_id, &by_id, regin))
		{
			//my_putchar('\n'); 
			//my_putchar('O');
			//my_putchar('K');	
			//my_putchar('\n'); 
			break;
		}
		else
		{
			//my_putchar('f');	 
			//my_putchar('a');	 
			//my_putchar('i');	 
			//my_putchar('l');	 
			//my_putchar('\n'); 
			if (USE_32DATA_LEN)
			{
				if(regin>7)
					regin-=2;
				else
					goto again;
				my_puts("test regin:");
				putword(regin);
				my_putchar('\n');
			}
			else
			{
				if(regin>5)
					regin-=2;
				else
					goto again;
			}
		}
		}
	save_tunevale = 1;	//0: not save tune value, 1: save tune value
	my_puts("tun src:");
	putword(sdr_tune_source);
	my_putchar('\n');
	my_puts("regin:");
	putword(regin);
	my_putchar('\n');
	my_puts("w:");
	putword(by_id);
	my_puts(",h:");
	putword(bx_id);
	my_putchar('\n');
	if(sdr_tune_source == CLK_TUN){
		 CLKTUN = CLKTUN_CONST | (tune_tab_1_clk[bx_id]<<7) | (tune_tab_2_clk[by_id]<<0);  //11_7: OUTPUT, 4_0: INPUT 
		 my_puts("clk\n");
	}else{
		DLLCON &= ~DLLTUN_CONST;
		DLLCON |= (tune_tab_1_dll[bx_id]<<DLLTUN_CH1_POS)|(tune_tab_2_dll[by_id]<<DLLTUN_CH2_POS);
		my_puts("dll\n");
	}
	//u32 tunevalue = 0, tune_crc = 0;
#if USE_PRESAVE_TUNE
	pre_save_tunevalue = 0;
	crc_check = 0;
	if(save_tunevale)
	{
		if(sdr_tune_source == CLK_TUN){
			pre_save_tunevalue = CLKTUN;  //11_7: OUTPUT, 4_0: INPUT 
		}else{
			pre_save_tunevalue = DLLCON;
		}
		crc_check = crc_cal(pre_save_tunevalue);

	}	
	_rtcRamWrite(HAL_TUNEVALUE_ADDR,(u8 *)&pre_save_tunevalue,4); //save tune value
	_rtcRamWrite(HAL_CRCCHECK_ADDR,(u8 *)&crc_check,4); //save tune value
	TUNE_END:	
#endif
	my_putchar('E');
	exception_delay(100);
	WTD_DIS();
	//delay(100);
	sdram_init();

	exception_delay(100);
	 WTD_DIS();
	//delay(100);
#if 0 //DLL打开前打开后测量1次		
	vbg_adc1 = adcGetChannel(11);//adc0
	vbg_param = vbg_adc1 * vbg_param / vbg_adc0;
#endif	
}

////////////////////////////////////////////////////////////////////////////////
void boostentry0(void); // define in boost.S file

__attribute__((section(".text.boosthdr"))) struct boost_t boosthdr_0 =
	{
		.rambase = (uint32_t)boost_start,
		.entry = (uint32_t)boostentry0,
};

struct boost_t *boost0(void)
{
	sdram_clock_tune();
	return &boosthdr_0;
}

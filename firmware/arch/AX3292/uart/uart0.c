#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"
#include "board_config.h"

extern UINT32 APB_CLK; // defined by board

struct uart0_spec_t
{
  volatile __sfr unsigned long *pufmap;
  volatile __sfr unsigned long *pudir;
  UINT32 pin;
};

static const struct uart0_spec_t uart0_spec[] = {
    {NULL, NULL, 0},
    {&PAFMAP, &PADIR, 0}, // group 0
    {&PAFMAP, &PADIR, 1}, // group 1
    {&PAFMAP, &PADIR, 4}, // group 2
    {&PAFMAP, &PADIR, 5}, // group 3
    {&PBFMAP, &PADIR, 6}, // group 4
    {&PAFMAP, &PADIR, 7}, // group 5
    {&PAFMAP, &PADIR, 9}, // group 6
    {&PAFMAP, &PADIR, 12},// group 7
    {&PBFMAP, &PBDIR, 0}, // group 8
    {&PBFMAP, &PBDIR, 1}, // group 9
    {&PBFMAP, &PBDIR, 5}, // group 10
    {&PBFMAP, &PBDIR, 6},  // group 11
    {&PDFMAP, &PDDIR, 1},  // group 12
    {&PDFMAP, &PDDIR, 2},  // group 13
    {&PDFMAP, &PDDIR, 3},  // group 14
    {&PDFMAP, &PDDIR, 7},  // group 15
    {&PDFMAP, &PDDIR, 10},  // group 16
    {&PFFMAP, &PFDIR, 0},  // group 17
    {&PFFMAP, &PFDIR, 2},  // group 18
    {&PFFMAP, &PFDIR, 8},  // group 19
    {&PFFMAP, &PFDIR, 10},  // group 20
    {&PFFMAP, &PFDIR, 11},  // group 21
    {&PEFMAP, &PEDIR, 0},  // group 22
    {&PEFMAP, &PEDIR, 1},  // group 23
    {&PEFMAP, &PEDIR, 2},  // group 24
    {&PEFMAP, &PEDIR, 3}  // group 25
};

/**
 * @brief 初始化uart0的IO
 * 指定UART0的Tx/Rx分組，進行IO初始化
 * @param tx_grp [in] UART Tx的組號+1，0表示disable
 * @param rx_grp [in] UART Tx的組號+1，0表示disable
 */
void uart0_io_init(UINT32 tx_grp, UINT32 rx_grp)
{
  printf("\n APB_CLK \n");
  PMAPCON0 = (PMAPCON0 & ~0x3FF) | ((tx_grp&0x1F) << 5) | (rx_grp&0x1F);

  if (tx_grp)
  {
    *uart0_spec[tx_grp].pufmap |= BIT(uart0_spec[tx_grp].pin);
    *uart0_spec[tx_grp].pudir &= ~BIT(uart0_spec[tx_grp].pin);
  }

  if (rx_grp)
  {
    *uart0_spec[rx_grp].pufmap |= BIT(uart0_spec[rx_grp].pin);
    *uart0_spec[rx_grp].pudir |= BIT(uart0_spec[rx_grp].pin);
  }
}
/**
 * @brief 初始化uart0，設定波特率
 * @param baud [in] 指定波特率
 */
void uart0_putchar(UINT32 c)
{
  UART0DAT = c;
  while ((UART0PND & BIT(1)) == 0)
    ;
  SETB(UART0PND, 1);
}

void clear_rx_pending() // rx
{
  UART0PND |= BIT(4); // clear rx sta
}

void uart0_receive(u8 *ubuf)
{
  *ubuf = UART0DAT;
}
#include "FreeRTOS.h"
#include "semphr.h"

volatile u32 rx_len = 0;
#define RX_BUF_LEN 128
u8 rx_buf[RX_BUF_LEN];
extern u8 rxsema_init;

//DEBUG_WIFI_MODULE 调试使用
extern SemaphoreHandle_t uart_rx_interrupt_sema;
void uart0_isr(void *param)
{
  if (UART0PND & BIT(0))
  {
    clear_rx_pending();
	if(0 == rx_len)
		memset(rx_buf, 0, RX_BUF_LEN);
	//printf("uart0 recv!!!!!\n\n\n\n");
	if(rx_len<RX_BUF_LEN)
    	uart0_receive(&rx_buf[rx_len]);
	#if DEBUG_WIFI_MODULE
	#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV) ||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)
	if('\n' == rx_buf[rx_len])
	{
		//rx_buf[rx_len] = '\0';
		rx_len = 0;
		if(rxsema_init){
			xSemaphoreGiveFromISR(uart_rx_interrupt_sema,NULL);
			return;
		}
	}
	#endif
	#endif
    rx_len++;
  }
}
//FINSH_RX_ENABLE 调试使用,finsh使用轮询
volatile u32  read_idx = 0;
volatile u32  write_idx = 0;
int console_data_ready()
{
	return (read_idx >= write_idx)?0:1;
} 
int8_t console_getc()
{
	if(0 == write_idx){
		return 0;
	}
	int pos =  read_idx;
	if(write_idx-read_idx>=RX_BUF_LEN){//缓冲区满
		int pos =  write_idx-RX_BUF_LEN;
		
		//找到第一条定位的指令开头继续
		while(rx_buf[pos%RX_BUF_LEN]!='\n'&& pos < write_idx){
			pos++;//退出时包括了'\n'
		};

		//有'\n'就确认为下一条指令开头
		if(pos < write_idx-1){
			read_idx = pos +1;
		}else{ //没有就从缓冲开头算
			read_idx = write_idx-RX_BUF_LEN;
		}
	}
	
	int8_t c = rx_buf[read_idx%RX_BUF_LEN];
	if(read_idx == write_idx){	
		unsigned long flag;
		flag=arch_local_irq_save();
		write_idx = 0;
		read_idx = 0;
		memset(rx_buf, 0, RX_BUF_LEN);
		arch_local_irq_restore(flag);	
	}else{
		read_idx++;
	}
	return c;
}

void uart0_isr2(void *param)
{
	if (UART0PND & BIT(0)){
		clear_rx_pending();

		uart0_receive(&rx_buf[write_idx%RX_BUF_LEN]);

		if(!rxsema_init){//还没有准备，先丢弃
			rx_buf[write_idx%RX_BUF_LEN] = '\0';
			return;
		}
		if('\r' ==rx_buf[write_idx%RX_BUF_LEN]){
			rx_buf[write_idx%RX_BUF_LEN] = '\n';
			printf("\r");
		}
		write_idx ++;
		rx_len = (write_idx-read_idx)<RX_BUF_LEN?(write_idx-read_idx):RX_BUF_LEN;
	}
}


void uart0_init(UINT32 baud)
{
	//==PB6 handle clean USB setting==
#if FINSH_RX_ENABLE || DEBUG_WIFI_MODULE
	//USB11CON0=0;
	//USB11CON1=0;
	USB11CON0 |= (1<<6);//control by soft,for next USB11CON1 set
	USB11CON1 &= ~(BIT(4)|BIT(6)); //disable dp,dm 120K pullup
	USB11CON1 &= ~(BIT(7)|BIT(5)); //disable dp,dm 15k pulldown
	//==end PB6 handle clean USB setting==
#endif

  //UART0CON = 0;
  UART0BAUD = APB_CLK / baud - 1;
#if DEBUG_WIFI_MODULE
  irq_register(8, 0, uart0_isr0, NULL);
  UART0CON |= BIT(0);
#elif FINSH_RX_ENABLE
  irq_register(8, 0, uart0_isr2, NULL);
  UART0CON |= BIT(0);
#endif

  UART0PND |= BIT(5) | BIT(4);
  UART0CON |= BIT(4);
  //    if(APB_CLK != 136000000) uart0_putchar('n');
  // if(baud != 115200) uart0_putchar('v');
}

////////////////////////////////////////////////////////////////////////////////
#define mem_set(adr, val) \
  (uint32_t)(&adr), (uint32_t)(val)
#define bic_orr(adr, bic, orr) \
  (uint32_t)(&adr) + 1, (uint32_t)(bic), (uint32_t)(orr)

__attribute__((section(".text.bootbsp.init0"))) void uart0_baud_init(uint32_t baud)
{
  UART0BAUD = baud;
}


//  {&PAFMAP, &PADIR,  0},  // group 0
__attribute__((section(".text.bootbsp.init1")))
const uint32_t uart0_bootinit_1[] = {
    bic_orr(PADIR, BIT(0), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 1 << 5),
    bic_orr(PAFMAP, 0, BIT(0)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  1},  // group 1
__attribute__((section(".text.bootbsp.init2")))
const uint32_t uart0_bootinit_2[] = {
    bic_orr(PADIR, BIT(1), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 2 << 5),
    bic_orr(PAFMAP, 0, BIT(1)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  4},  // group 2
__attribute__((section(".text.bootbsp.init3")))
const uint32_t uart0_bootinit_3[] = {
    bic_orr(PADIR, BIT(4), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 3 << 5),
    bic_orr(PAFMAP, 0, BIT(4)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  5},  // group 3
__attribute__((section(".text.bootbsp.init4")))
const uint32_t uart0_bootinit_4[] = {
    bic_orr(PADIR, BIT(5), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 4 << 5),
    bic_orr(PAFMAP, 0, BIT(5)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  6},  // group 4
__attribute__((section(".text.bootbsp.init5")))
const uint32_t uart0_bootinit_5[] = {
    bic_orr(PADIR, BIT(6), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 5 << 5),
    bic_orr(PAFMAP, 0, BIT(6)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  7},  // group 5
__attribute__((section(".text.bootbsp.init6")))
const uint32_t uart0_bootinit_6[] = {
    bic_orr(PADIR, BIT(7), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 6 << 5),
    bic_orr(PAFMAP, 0, BIT(7)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  9},  // group 6
__attribute__((section(".text.bootbsp.init7")))
const uint32_t uart0_bootinit_7[] = {
    bic_orr(PADIR, BIT(9), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 7 << 5),
    bic_orr(PAFMAP, 0, BIT(9)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PAFMAP, &PADIR,  12},  // group 7
__attribute__((section(".text.bootbsp.init8")))
const uint32_t uart0_bootinit_8[] = {
    bic_orr(PADIR, BIT(12), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 8 << 5),
    bic_orr(PAFMAP, 0, BIT(12)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PBFMAP, &PBDIR, 0},  // group 8
__attribute__((section(".text.bootbsp.init9")))
const uint32_t uart0_bootinit_9[] = {
    bic_orr(PBDIR, BIT(0), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 9 << 5),
    bic_orr(PBFMAP, 0, BIT(0)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//  {&PBFMAP, &PBDIR, 1}   // group 9
__attribute__((section(".text.bootbsp.init10")))
const uint32_t uart0_bootinit_10[] = {
    bic_orr(PBDIR, BIT(1), 0),
    bic_orr(PMAPCON0, (0x1F<< 5), 10 << 5),
    bic_orr(PBFMAP, 0, BIT(1)),
    //  mem_set (UART0BAUD, 120000000L/115200 - 1),
    mem_set(UART0CON, BIT(4)),
    0};

//	{&PBFMAP, &PBDIR, 5}   // group 10
__attribute__((section(".text.bootbsp.init11")))
const uint32_t uart0_bootinit_11[] = {
	bic_orr(PBDIR, BIT(5), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 11 << 5),
	bic_orr(PBFMAP, 0, BIT(5)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PBFMAP, &PBDIR, 6}   // group 11
__attribute__((section(".text.bootbsp.init12")))
const uint32_t uart0_bootinit_12[] = {
	bic_orr(PBDIR, BIT(6), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 12 << 5),
	bic_orr(PBFMAP, 0, BIT(6)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PDFMAP, &PDDIR, 1}   // group 12
__attribute__((section(".text.bootbsp.init13")))
const uint32_t uart0_bootinit_13[] = {
	bic_orr(PDDIR, BIT(1), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 13 << 5),
	bic_orr(PDFMAP, 0, BIT(1)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PDFMAP, &PDDIR, 2}   // group 13
__attribute__((section(".text.bootbsp.init14")))
const uint32_t uart0_bootinit_14[] = {
	bic_orr(PDDIR, BIT(2), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 14 << 5),
	bic_orr(PDFMAP, 0, BIT(2)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};
	
//	{&PDFMAP, &PDDIR, 3}   // group 14
__attribute__((section(".text.bootbsp.init15")))
const uint32_t uart0_bootinit_15[] = {
	bic_orr(PDDIR, BIT(3), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 15 << 5),
	bic_orr(PDFMAP, 0, BIT(3)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PDFMAP, &PDDIR, 7}   // group 15
__attribute__((section(".text.bootbsp.init16")))
const uint32_t uart0_bootinit_16[] = {
	bic_orr(PDDIR, BIT(7), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 16 << 5),
	bic_orr(PDFMAP, 0, BIT(7)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PDFMAP, &PDDIR, 10}   // group 16
__attribute__((section(".text.bootbsp.init17")))
const uint32_t uart0_bootinit_17[] = {
	bic_orr(PDDIR, BIT(10), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 17 << 5),
	bic_orr(PDFMAP, 0, BIT(10)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PFFMAP, &PFDIR, 0}	// group 17
__attribute__((section(".text.bootbsp.init18")))
const uint32_t uart0_bootinit_18[] = {
	bic_orr(PFDIR, BIT(0), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 18 << 5),
	bic_orr(PFFMAP, 0, BIT(0)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PFFMAP, &PFDIR, 2}	// group 18
__attribute__((section(".text.bootbsp.init19")))
const uint32_t uart0_bootinit_19[] = {
	bic_orr(PFDIR, BIT(2), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 19 << 5),
	bic_orr(PFFMAP, 0, BIT(2)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PFFMAP, &PFDIR, 8}	// group 19
__attribute__((section(".text.bootbsp.init20")))
const uint32_t uart0_bootinit_20[] = {
	bic_orr(PFDIR, BIT(8), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 20 << 5),
	bic_orr(PFFMAP, 0, BIT(8)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PFFMAP, &PFDIR, 10}	// group 20
__attribute__((section(".text.bootbsp.init21")))
const uint32_t uart0_bootinit_21[] = {
	bic_orr(PFDIR, BIT(10), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 21 << 5),
	bic_orr(PFFMAP, 0, BIT(10)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PFFMAP, &PFDIR, 11}	// group 21
__attribute__((section(".text.bootbsp.init22")))
const uint32_t uart0_bootinit_22[] = {
	bic_orr(PFDIR, BIT(11), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 22 << 5),
	bic_orr(PFFMAP, 0, BIT(11)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PEFMAP, &PEDIR, 0}	// group 22
__attribute__((section(".text.bootbsp.init23")))
const uint32_t uart0_bootinit_23[] = {
	bic_orr(PEDIR, BIT(0), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 23 << 5),
	bic_orr(PEFMAP, 0, BIT(0)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PEFMAP, &PEDIR, 1}	// group 23
__attribute__((section(".text.bootbsp.init24")))
const uint32_t uart0_bootinit_24[] = {
	bic_orr(PEDIR, BIT(1), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 24 << 5),
	bic_orr(PEFMAP, 0, BIT(1)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PEFMAP, &PEDIR, 2}	// group 24
__attribute__((section(".text.bootbsp.init25")))
const uint32_t uart0_bootinit_25[] = {
	bic_orr(PEDIR, BIT(2), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 25 << 5),
	bic_orr(PEFMAP, 0, BIT(2)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

//	{&PEFMAP, &PEDIR, 3}	// group 25
__attribute__((section(".text.bootbsp.init26")))
const uint32_t uart0_bootinit_26[] = {
	bic_orr(PEDIR, BIT(3), 0),
	bic_orr(PMAPCON0, (0x1F<< 5), 26 << 5),
	bic_orr(PEFMAP, 0, BIT(3)),
	//	mem_set (UART0BAUD, 120000000L/115200 - 1),
	mem_set(UART0CON, BIT(4)),
	0};

	

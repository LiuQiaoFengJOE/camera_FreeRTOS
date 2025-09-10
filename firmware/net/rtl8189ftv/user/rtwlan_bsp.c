/**
 ******************************************************************************
 * @file    realtek_wlan_bsp.c
 * @author  Realtek software team
 * @version V0.1
 * @date    05-March-2013
 * @brief   Realtek WLAN hardware configuration.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h" // for include CONFIG_ISR_THREAD_MODE_INTERRUPT of autoconf.h

// FreeRTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "debug.h"

/* Public viriables ---------------------------------------------------------*/
#if (SPI_OP_MODE & \
	 (SPI_OP_MODE_DMA_POLLING | SPI_OP_MODE_DMA_INTERRUPT))
DMA_InitTypeDef DMA_InitStructure;

#if (SPI_OP_MODE & SPI_OP_MODE_DMA_INTERRUPT)
extern xSemaphoreHandle spi_dma_interrupt_sema;
#endif // SPI_OP & DMA_INTERRUPT

#endif // SPI_OP& (DMA_POLLING || DMA_Interrupt)

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#define WIFI_USE_SDIO

#if defined(WIFI_USE_GSPI)
/**
 * @brief  Configures the SPI Peripheral.
 * @param  None
 * @retval None
 */
static void SPI_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

#if (SPI_OP_MODE & SPI_OP_MODE_DMA_INTERRUPT)
	NVIC_InitTypeDef NVIC_InitStructure;
#endif

	/* Peripheral Clock Enable -------------------------------------------------*/
	/* Enable the SPI clock */
	SPIx_CLK_INIT(SPIx_CLK, ENABLE);

	/* Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

	/* SPI GPIO Configuration --------------------------------------------------*/
	/* Connect SPI pins to AF5 */
	GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
	GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
	GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_MOSI_PIN;
	GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	/* SPI MISO pin configuration  */
	GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
	GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPIx_);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
#if (SPI_OP_MODE & SPI_OP_MODE_IO)
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
#elif (SPI_OP_MODE & \
	   (SPI_OP_MODE_DMA_POLLING | SPI_OP_MODE_DMA_INTERRUPT))
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
#endif
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// Prescaler = 2 : 30MHz for SPI1, 15Mhz for SPI2
	// Prescaler = 4 : 15MHz for SPI1, 7.5Mhz for SPI2 ...
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRate_Prescaler;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	/* SPI can be initialized in init state, but it will be enabled at the begin of the transaction*/
	SPI_Init(SPIx_, &SPI_InitStructure);
	SPI_Cmd(SPIx_, ENABLE);

#if (SPI_OP_MODE & \
	 (SPI_OP_MODE_DMA_POLLING | SPI_OP_MODE_DMA_INTERRUPT))
	/* DMA configuration -------------------------------------------------------*/

	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);

	/* Clear any pending flag on Tx Stream  */
	DMA_ClearFlag(SPIx_DMA_STREAM_TX, SPIx_TX_DMA_TCFLAG | SPIx_TX_DMA_FEIFLAG | SPIx_TX_DMA_DMEIFLAG |
										  SPIx_TX_DMA_TEIFLAG | SPIx_TX_DMA_HTIFLAG);

	/* Clear any pending flag on Rx Stream  */
	DMA_ClearFlag(SPIx_DMA_STREAM_RX, SPIx_RX_DMA_TCFLAG | SPIx_RX_DMA_FEIFLAG | SPIx_RX_DMA_DMEIFLAG |
										  SPIx_RX_DMA_TEIFLAG | SPIx_RX_DMA_HTIFLAG);

	/* Disable the SPI Tx DMA stream */
	DMA_Cmd(SPIx_DMA_STREAM_TX, DISABLE);
	/* Configure the DMA stream for the SPI peripheral TX direction */
	DMA_DeInit(SPIx_DMA_STREAM_TX);
	/* Disable the SPI Rx DMA stream */
	DMA_Cmd(SPIx_DMA_STREAM_RX, DISABLE);
	/* Configure the DMA stream for the SPI peripheral RX direction */
	DMA_DeInit(SPIx_DMA_STREAM_RX);

	DMA_InitStructure.DMA_Channel = SPIx_DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPIx_DMA_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0; /* This field will be configured in spi tx function */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE; /* This field will be configured in spi tx function */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	/* Init DMA for Reception */
	/* DMA is intitialized here, but will be re-initialized after Memory0BaseAddr
		 is decided during reception/ transaction */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /*RX priority should be higher than TX */
	DMA_Init(SPIx_DMA_STREAM_RX, &DMA_InitStructure);

	/* Init DMA for Transmission */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_Init(SPIx_DMA_STREAM_TX, &DMA_InitStructure);

#if (SPI_OP_MODE & SPI_OP_MODE_DMA_INTERRUPT)
	/* Enable the selected DMA interrupts for Transmission */
	DMA_ITConfig(SPIx_DMA_STREAM_TX, DMA_IT_TC, ENABLE);

	/* Enable the selected DMA interrupts for Transmission */
	DMA_ITConfig(SPIx_DMA_STREAM_RX, DMA_IT_TC, ENABLE);
#endif

	/* Enable RX DMA stream ahead of TX DMA stream*/
	/* Enable DMA RX Channel */
	SPI_I2S_DMACmd(SPIx_, SPI_I2S_DMAReq_Rx, ENABLE);
	/* Enable DMA TX Channel */
	SPI_I2S_DMACmd(SPIx_, SPI_I2S_DMAReq_Tx, ENABLE);

#if (SPI_OP_MODE & SPI_OP_MODE_DMA_INTERRUPT)
	/* Configure the Priority Group to 1 bit */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Configure the SPI DMA Tx interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = SPIx_TX_DMA_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = SPIx_RX_DMA_IRQn;
	NVIC_Init(&NVIC_InitStructure);

#endif // SPI_OP_MODE &SPI_OP_MODE_DMA_INTERRUPT

#endif //(SPI_OP_MODE_DMA_POLLING | SPI_OP_MODE_DMA_INTERRUPT)
}

void SPI_Chip_Select_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOG Periph clock enable */
	RCC_AHB1PeriphClockCmd(SPIx_CS_GPIO_CLK, ENABLE);

	/* Set Output High as defulat */
	GPIO_WriteBit(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, Bit_SET);

	/* use GPIO G6 */
	GPIO_InitStructure.GPIO_Pin = SPIx_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);
}

#ifdef USE_WLAN_PW_PIN
void WLAN_Power_Swtich_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOG Periph clock enable */
	RCC_AHB1PeriphClockCmd(WLAN_PW_GPIO_CLK, ENABLE);

	/* Set Output Low (3.3 Power on) as defulat */
	GPIO_WriteBit(WLAN_PW_GPIO_PORT, WLAN_PW_PIN, Bit_RESET);

	/* use GPIO G6 */
	GPIO_InitStructure.GPIO_Pin = WLAN_PW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WLAN_PW_GPIO_PORT, &GPIO_InitStructure);
}
#endif

#ifdef USE_WLAN_PD_PIN
void WLAN_Power_Down_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOG Periph clock enable */
	RCC_AHB1PeriphClockCmd(WLAN_PD_GPIO_CLK, ENABLE);

	/* Set Output High (Chip select) as defulat */
	GPIO_WriteBit(WLAN_PD_GPIO_PORT, WLAN_PD_PIN, Bit_SET);

	/* use GPIO */
	GPIO_InitStructure.GPIO_Pin = WLAN_PD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WLAN_PD_GPIO_PORT, &GPIO_InitStructure);
}
#endif

void WLAN_BSP_UsLoop(int us);
/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_On(void)
{
#ifdef USE_WLAN_PD_PIN
	GPIO_WriteBit(WLAN_PD_GPIO_PORT, WLAN_PD_PIN, Bit_SET);
	WLAN_BSP_UsLoop(10 * 1000); // 10ms
	GPIO_WriteBit(WLAN_PD_GPIO_PORT, WLAN_PD_PIN, Bit_RESET);
	WLAN_BSP_UsLoop(10 * 1000); // 10ms
	GPIO_WriteBit(WLAN_PD_GPIO_PORT, WLAN_PD_PIN, Bit_SET);
	WLAN_BSP_UsLoop(10 * 1000); // 10ms
#endif
}

/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_Off(void)
{
#ifdef USE_WLAN_PD_PIN
	GPIO_WriteBit(WLAN_PD_GPIO_PORT, WLAN_PD_PIN, Bit_RESET);
#endif
}

/**
 * @brief  Enable or Disable interrupt IRQ
 * @param  set 1 to enable interrupt. set 0 to disable interrupt
 * @retval None
 */
void Wlan_SoC_Interrupt_Set(unsigned long set)
{

	EXTI_InitTypeDef EXTI_InitStructure;

	/* Configure EXTI Line7 */
	EXTI_InitStructure.EXTI_Line = SPIx_INT_EXT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	if (set)
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
 * @brief  Configures EXTI Line 9 (connected to PA9 pin) in interrupt mode
 * @param  None
 * @retval None
 */
void Wlan_Interrpt_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIOG clock */
	RCC_AHB1PeriphClockCmd(SPIx_INT_GPIO_CLK, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA9 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = SPIx_INT_PIN;
	GPIO_Init(SPIx_INT_GPIO_PORT, &GPIO_InitStructure);

	/* Connect EXTI Line9 to PA9 pin */
	SYSCFG_EXTILineConfig(SPIx_INT_EXT_PORT_SRC, SPIx_INT_EXT_PIN_SRC);

	/* Configure EXTI Line9 */
	EXTI_InitStructure.EXTI_Line = SPIx_INT_EXT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = SPIx_INT_EXT_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}
#elif defined(WIFI_USE_SDIO)
void WLAN_BSP_UsLoop(int us);
/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_On(void)
{
}
/* Customer function to control HW specific wlan gpios */
void Set_WLAN_Power_Off(void)
{
}
#endif

/**
 * @brief  Configures the SPI Peripheral.
 * @param  None
 * @retval status of Realtek wlan interface initialization. Return 0 if FALSE.
 */
int WLAN_BSP_Config()
{

	int ret = 1;

#if defined(WIFI_USE_GSPI)
	/* Configure SPI */
	SPI_Config();
	/* Configure GPIO for SPI chip select*/
	SPI_Chip_Select_GPIO_Config();

	/* Configure GPIO for WLAN PDn pin*/
#ifdef USE_WLAN_PD_PIN
	WLAN_Power_Down_GPIO_Config();
#endif

	/* Configure GPIO for WLAN 3.3V Power switch*/
#ifdef USE_WLAN_PW_PIN
	WLAN_Power_Swtich_GPIO_Config();
#endif

#if (SPI_OP_MODE & SPI_OP_MODE_DMA_INTERRUPT)
	/*Initialize Semaphre for SPI interrupt */
	// Semaphare is created here (before any thread is created) and is never freed
	vSemaphoreCreateBinary(spi_dma_interrupt_sema);
#endif

	/* Configure SPI interrupt pin */
#ifdef CONFIG_ISR_THREAD_MODE_INTERRUPT
	Wlan_Interrpt_Init();
#endif
#endif

	return ret;
}

/**
 * @brief  Deinit the SPI Peripheral.
 * @param  None
 * @retval status of Realtek wlan interface de-initialization. Return 0 if FALSE.
 */
int WLAN_BSP_DeConfig()
{
	int ret = 1;
	// Free semaphore
	// spi_dma_interrupt_sema

	// Disable DMA
	return ret;
}

/**
 * @brief  Implement finit loop for micro-second sleep or delay
 * @param  micro seconds to be delayed
 * @retval None
 */

#if defined(__ICCARM__)
static inline void sys_ctl_delay(unsigned long ulCount)
{
	/*
		us *= STM32_DELAY_US_MULT;
		us--;
		asm(" mov r0, us \n"
		"loop: subs r0, #1 \n"
		" bhi loop \n");
	*/
	while (ulCount--)
		;
}

#elif defined(__CC_ARM)

__asm void sys_ctl_delay(unsigned long ulCount)
{
	subs r0, #1;
	bne sys_ctl_delay;
	bx lr;
}

#elif defined(__GNUC__)
// #error "please implement inline assembler for __GNU__"
static inline void sys_ctl_delay(unsigned long ulCount)
{
	/*
		us *= STM32_DELAY_US_MULT;
		us--;
		asm(" mov r0, us \n"
		"loop: subs r0, #1 \n"
		" bhi loop \n");
	*/
	while (ulCount--)
		;
}
#endif

/**
 * FreeRTOS does not provide us level delay. Use busy wait
 * It is CPU platform dependent
 */

void WLAN_BSP_UsLoop(int us)
{

	unsigned long nloops = us * (configCPU_CLOCK_HZ / 3000000);
	sys_ctl_delay(nloops);
}

unsigned long WLAN_BSP_Transfer(unsigned char *buf, unsigned int buf_len)
{
#ifdef JORNNY
	return spi_transaction_transfer(buf, buf_len);
#else
	printf("miss WLAN_BSP_Transfer\n");
	return 0;
#endif
}

#ifdef CONFIG_SUSPEND
void system_suspend_resume()
{
#ifdef JORNNY
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/*Íâ²¿ÖÐ¶ÏÊ¹ÓÃPC7*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
	/* Configure the CEC Line as EXTI Line on Falling Edge */
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	printf("\n\rSystem enter suspend!");
	/* Request to enter Stop mode */
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

	/* Disable the CEC EXTI Line */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	SystemInit();
	printf("\n\rsystem resume!");
#else
	printf("miss system resume!\n");
#endif
}
#endif

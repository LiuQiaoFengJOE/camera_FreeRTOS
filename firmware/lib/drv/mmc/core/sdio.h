#ifndef __SDIO_H__
#define __SDIO_H__

// #include "api.h"
#include "osal/osal_semaphore.h"
#include "../core.h"
#include "typedef.h"

typedef struct semaphore _sema;

typedef int (*io_rw_extend_func)(struct mmc_host *host, u32 arg, u8 *buf, unsigned blocks, unsigned blksz);

struct mmc_host
{
  struct mmc_card *card; /* device attached to this host */
  _sema sdio_irq_sema;
  int sdio_irq_pending;
  _sema lock; /* lock for claim and bus ops */

  io_rw_extend_func mmc_io_rw_extend;
  void *controller;
};

struct sdio_cccr
{
  unsigned int sdio_vsn;
  unsigned int sd_vsn;
  unsigned int multi_block : 1,
      low_speed : 1,
      wide_bus : 1,
      high_power : 1,
      high_speed : 1,
      disable_cd : 1;

} __packed;

struct mmc_card
{
  struct mmc_host *host; /* the host this device belongs to */
  struct sdio_func *func;
  struct sdio_cccr cccr; /* common card info */
  uint32_t rca;
};

struct sdio_func
{
  struct mmc_card *card;                   /* the card this device belongs to */
  void (*irq_handler)(struct sdio_func *); /* IRQ callback */

  unsigned int max_blksize;               /* maximum block size */
  unsigned int cur_blksize;               /* current block size */
  unsigned int enable_timeout;            /* max enable timeout in msec */
  unsigned int num; /* function number */ // add
  unsigned short vendor; /* vendor id */  // add
  unsigned short device; /* device id */  // add
  unsigned num_info;                      /* number of info strings */
  const char **info; /* info strings */   // add
  unsigned char class;                    /* standard interface class */

  unsigned int tmpbuf_reserved; // for tmpbuf 4 byte alignment
  unsigned char tmpbuf[4];      /* DMA:able scratch buffer */

#ifdef CONFIG_READ_CIS
  struct sdio_func_tuple *tuples;
#endif
  void *drv_priv;
};

#define SD_SEND_DATA_EDGE 0 // send data at clk edge :  0:fall; 1:rise

// #define RSP_NONE				0X00
// #define RSP_6B_NO_BUSY			0X01
// #define RSP_17B_NO_BUSY			0X03
// #define RSP_6B_CHECK_BUSY		0X05
// #define RSP_17B_CHECK_BUSY		0X07
// #define NO_RSP_NO_BUSY			0X02
// #define NO_RSP_CHECK_BUSY		0X04

#define DATA_NONE 0X00
#define DATA_RECEIVE 0X10
#define DATA_SEND_NO_BUSY 0X20
#define DATA_SEND_CHECK_BUSY 0X30

#define SDIO_RESET_PORT GPIOB
#define SDIO_RESET_PIN GPIO_Pin_12
#define SDIO_PDN_PORT GPIOA
#define SDIO_PDN_PIN GPIO_Pin_2
#define SDIO_RESET_OUTPUT // REG32(PG_DIR) &= ~BIT(11)
#define SDIO_RESET_HIGH   // REG32(PG) |= BIT(11)//GPIO_SET(SDIO_RESET_PORT, SDIO_RESET_PIN)
#define SDIO_RESET_LOW    // REG32(PG) &= ~BIT(11)//GPIO_CLR(SDIO_RESET_PORT, SDIO_RESET_PIN)

#define SDIO_PDN_OUTPUT REG32(PA_DIR) &= ~BIT(6) // REG32(PF_DIR) &= ~BIT(6)//REG32(PB_DIR) &= ~BIT(0);
#define SDIO_PDN_HIGH REG32(PA) |= BIT(6)        // REG32(PF) |= BIT(6)//REG32(PB) |= BIT(0)//GPIO_SET(SDIO_PDN_PORT, SDIO_PDN_PIN)
#define SDIO_PDN_LOW REG32(PA) &= ~BIT(6)        // REG32(PF) &= ~BIT(6)//REG32(PB) &= ~BIT(0)//GPIO_CLR(SDIO_PDN_PORT, SDIO_PDN_PIN)

/* SDIO commands                         type  argument     response */
#define SD_IO_SEND_OP_COND 5 /* bcr  [23:0] OCR         R4  */
#define SD_IO_RW_DIRECT 52   /* ac   [31:0] See below   R5  */
#define SD_IO_RW_EXTENDED 53 /* adtc [31:0] See below   R5  */

#define SDIO_CCCR_ABORT 0x06 /* function abort/card reset */

#define SDIO_FBR_BASE(f) ((f)*0x100) /* base of function f's FBRs */

#define SDIO_FBR_STD_IF 0x00

#define SDIO_FBR_SUPPORTS_CSA 0x40 /* supports Code Storage Area */
#define SDIO_FBR_ENABLE_CSA 0x80   /* enable Code Storage Area */

#define SDIO_FBR_STD_IF_EXT 0x01

#define SDIO_FBR_POWER 0x02

#define SDIO_FBR_POWER_SPS 0x01 /* Supports Power Selection */
#define SDIO_FBR_POWER_EPS 0x02 /* Enable (low) Power Selection */

#define SDIO_FBR_CIS 0x09 /* CIS pointer (3 bytes) */

#define SDIO_FBR_CSA 0x0C /* CSA pointer (3 bytes) */
#define SDIO_FBR_CSA_DATA 0x0F
#define SDIO_FBR_BLKSIZE 0x10 /* block size (2 bytes) */

#define SDIO_CCCR_CAPS 0x08

#define SDIO_CCCR_CAP_SDC 0x01  /* can do CMD52 while data transfer */
#define SDIO_CCCR_CAP_SMB 0x02  /* can do multi-block xfers (CMD53) */
#define SDIO_CCCR_CAP_SRW 0x04  /* supports read-wait protocol */
#define SDIO_CCCR_CAP_SBS 0x08  /* supports suspend/resume */
#define SDIO_CCCR_CAP_S4MI 0x10 /* interrupt during 4-bit CMD53 */
#define SDIO_CCCR_CAP_E4MI 0x20 /* enable ints during 4-bit CMD53 */
#define SDIO_CCCR_CAP_LSC 0x40  /* low speed card */
#define SDIO_CCCR_CAP_4BLS 0x80 /* 4 bit low speed card */

#define SDIO_CCCR_CIS 0x09 /* common CIS pointer (3 bytes) */

#define SDIO_CCCR_CCCR 0x00

#define SDIO_CCCR_REV_1_00 0 /* CCCR/FBR Version 1.00 */
#define SDIO_CCCR_REV_1_10 1 /* CCCR/FBR Version 1.10 */
#define SDIO_CCCR_REV_1_20 2 /* CCCR/FBR Version 1.20 */

#define SDIO_SDIO_REV_1_00 0 /* SDIO Spec Version 1.00 */
#define SDIO_SDIO_REV_1_10 1 /* SDIO Spec Version 1.10 */
#define SDIO_SDIO_REV_1_20 2 /* SDIO Spec Version 1.20 */
#define SDIO_SDIO_REV_2_00 3 /* SDIO Spec Version 2.00 */

#define SDIO_CCCR_SD 0x01

#define SDIO_SD_REV_1_01 0 /* SD Physical Spec Version 1.01 */
#define SDIO_SD_REV_1_10 1 /* SD Physical Spec Version 1.10 */
#define SDIO_SD_REV_2_00 2 /* SD Physical Spec Version 2.00 */

#define SDIO_CCCR_POWER 0x12

#define SDIO_POWER_SMPC 0x01 /* Supports Master Power Control */
#define SDIO_POWER_EMPC 0x02 /* Enable Master Power Control */

#define SDIO_CCCR_SPEED 0x13

#define SDIO_SPEED_SHS 0x01 /* Supports High-Speed mode */
#define SDIO_SPEED_EHS 0x02 /* Enable High-Speed mode */

/* Mask for errors Card Status R1 (OCR Register) */
#define SD_OCR_ADDR_OUT_OF_RANGE ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS ((uint32_t)0xFDFFE008)

/* Masks for R6 Response */
#define SD_R6_GENERAL_UNKNOWN_ERROR ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED ((uint32_t)0x00008000)

#define SD_VOLTAGE_WINDOW_SD ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY ((uint32_t)0x40000000)
#define SD_STD_CAPACITY ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN ((uint32_t)0x000001AA)

#define SD_MAX_VOLT_TRIAL ((uint32_t)0x0000FFFF)
#define SD_ALLZERO ((uint32_t)0x00000000)

#define SD_WIDE_BUS_SUPPORT ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT ((uint32_t)0x00010000)
#define SD_CARD_LOCKED ((uint32_t)0x02000000)
#define SD_CARD_PROGRAMMING ((uint32_t)0x00000007)
#define SD_CARD_RECEIVING ((uint32_t)0x00000006)
#define SD_DATATIMEOUT ((uint32_t)0x000FFFFF)
#define SD_0TO7BITS ((uint32_t)0x000000FF)
#define SD_8TO15BITS ((uint32_t)0x0000FF00)
#define SD_16TO23BITS ((uint32_t)0x00FF0000)
#define SD_24TO31BITS ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES ((uint32_t)0x00000020)

/* Command Class Supported */
#define SD_CCCC_LOCK_UNLOCK ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT ((uint32_t)0x00000040)
#define SD_CCCC_ERASE ((uint32_t)0x00000020)

/* Following commands are SD Card Specific commands.
 * SDIO_APP_CMD should be sent before sending these commands.
 */
#define SDIO_SEND_IF_COND ((uint32_t)0x00000008)
#define SDIO_INIT_CLK_DIV ((uint8_t)0x80)

#define DCTRL_CLEAR_MASK ((uint32_t)0xFFFFFF08)
#define CCR_ENABLE_Reset ((uint32_t)0xFFFFFFFE)

/* Exported types ------------------------------------------------------------*/

#define SDIO_CCCR_IF 0x07 /* bus interface controls */

#define SDIO_BUS_WIDTH_1BIT 0x00
#define SDIO_BUS_WIDTH_4BIT 0x02
#define SDIO_BUS_ECSI 0x20 /* Enable continuous SPI interrupt */
#define SDIO_BUS_SCSI 0x40 /* Support continuous SPI interrupt */

#define R5_COM_CRC_ERROR (1 << 15)   /* er, b */
#define R5_ILLEGAL_COMMAND (1 << 14) /* er, b */
#define R5_ERROR (1 << 11)           /* erx, c */
#define R5_FUNCTION_NUMBER (1 << 9)  /* er, c */
#define R5_OUT_OF_RANGE (1 << 8)     /* er, c */
#define R5_STATUS(x) (x & 0xCB00)
#define R5_IO_CURRENT_STATE(x) ((x & 0x3000) >> 12) /* s, b */

#define SDIO_CCCR_IOEx 0x02
#define SDIO_CCCR_IORx 0x03

#define SDIO_CCCR_IENx 0x04 /* Function/Master Interrupt Enable */
#define SDIO_CCCR_INTx 0x05 /* Function Interrupt Pending */

#define MMC_CARD_BUSY 0x80000000 /* Card Power up status bit */

/* SDIO Commands  Index */
#define SDIO_GO_IDLE_STATE ((uint8_t)0)
#define SDIO_SEND_OP_COND ((uint8_t)1)
#define SDIO_ALL_SEND_CID ((uint8_t)2)
#define SDIO_SET_REL_ADDR ((uint8_t)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR ((uint8_t)4)
#define SDIO_SDIO_SEN_OP_COND ((uint8_t)5)
#define SDIO_HS_SWITCH ((uint8_t)6)
#define SDIO_SEL_DESEL_CARD ((uint8_t)7)
#define SDIO_HS_SEND_EXT_CSD ((uint8_t)8)
#define SDIO_SEND_CSD ((uint8_t)9)
#define SDIO_SEND_CID ((uint8_t)10)
#define SDIO_READ_DAT_UNTIL_STOP ((uint8_t)11) /* SD Card doesn't support it */
#define SDIO_STOP_TRANSMISSION ((uint8_t)12)
#define SDIO_SEND_STATUS ((uint8_t)13)
#define SDIO_HS_BUSTEST_READ ((uint8_t)14)
#define SDIO_GO_INACTIVE_STATE ((uint8_t)15)
#define SDIO_SET_BLOCKLEN ((uint8_t)16)
#define SDIO_READ_SINGLE_BLOCK ((uint8_t)17)
#define SDIO_READ_MULT_BLOCK ((uint8_t)18)
#define SDIO_HS_BUSTEST_WRITE ((uint8_t)19)
#define SDIO_WRITE_DAT_UNTIL_STOP ((uint8_t)20) /* SD Card doesn't support it */
#define SDIO_SET_BLOCK_COUNT ((uint8_t)23)      /* SD Card doesn't support it */
#define SDIO_WRITE_SINGLE_BLOCK ((uint8_t)24)
#define SDIO_WRITE_MULT_BLOCK ((uint8_t)25)
#define SDIO_PROG_CID ((uint8_t)26) /* reserved for manufacturers */
#define SDIO_PROG_CSD ((uint8_t)27)
#define SDIO_SET_WRITE_PROT ((uint8_t)28)
#define SDIO_CLR_WRITE_PROT ((uint8_t)29)
#define SDIO_SEND_WRITE_PROT ((uint8_t)30)
#define SDIO_SD_ERASE_GRP_START ((uint8_t)32) /* To set the address of the first write \
                                                 block to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_END ((uint8_t)33)   /* To set the address of the last write block of the \
                                                 continuous range to be erased. (For SD card only) */
#define SDIO_ERASE_GRP_START ((uint8_t)35)    /* To set the address of the first write block to be erased. \
                                                 (For MMC card only spec 3.31) */

#define SDIO_ERASE_GRP_END ((uint8_t)36) /* To set the address of the last write block of the \
                                            continuous range to be erased. (For MMC card only spec 3.31) */

#define SDIO_ERASE ((uint8_t)38)
#define SDIO_FAST_IO ((uint8_t)39)      /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE ((uint8_t)40) /* SD Card doesn't support it */
#define SDIO_LOCK_UNLOCK ((uint8_t)42)
#define SDIO_APP_CMD ((uint8_t)55)
#define SDIO_GEN_CMD ((uint8_t)56)
#define SDIO_NO_CMD ((uint8_t)64)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these
   commands. */
#define SDIO_APP_SD_SET_BUSWIDTH ((uint8_t)6)           /* For SD Card only */
#define SDIO_SD_APP_STAUS ((uint8_t)13)                 /* For SD Card only */
#define SDIO_SD_APP_SEND_NUM_WRITE_BLOCKS ((uint8_t)22) /* For SD Card only */
#define SDIO_SD_APP_OP_COND ((uint8_t)41)               /* For SD Card only */
#define SDIO_SD_APP_SET_CLR_CARD_DETECT ((uint8_t)42)   /* For SD Card only */
#define SDIO_SD_APP_SEND_SCR ((uint8_t)51)              /* For SD Card only */
#define SDIO_SDIO_RW_DIRECT ((uint8_t)52)               /* For SD I/O Card only */
#define SDIO_SDIO_RW_EXTENDED ((uint8_t)53)             /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SD_APP_GET_MKB ((uint8_t)43)                     /* For SD Card only */
#define SDIO_SD_APP_GET_MID ((uint8_t)44)                     /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RN1 ((uint8_t)45)                 /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RN2 ((uint8_t)46)                 /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RES2 ((uint8_t)47)                /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RES1 ((uint8_t)48)                /* For SD Card only */
#define SDIO_SD_APP_SECURE_READ_MULTIPLE_BLOCK ((uint8_t)18)  /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK ((uint8_t)25) /* For SD Card only */
#define SDIO_SD_APP_SECURE_ERASE ((uint8_t)38)                /* For SD Card only */
#define SDIO_SD_APP_CHANGE_SECURE_AREA ((uint8_t)49)          /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MKB ((uint8_t)48)            /* For SD Card only */

typedef enum
{
  SD_NO_TRANSFER = 0,
  SD_TRANSFER_IN_PROGRESS
} SDTransferState;

typedef struct
{
  uint16_t TransferredBytes;
  SD_Error TransferError;
  uint8_t padding;
} SDLastTransferInfo;

typedef struct /* Card Specific Data */
{
  uint8_t CSDStruct;           /* CSD structure */
  uint8_t SysSpecVersion;      /* System specification version */
  uint8_t Reserved1;           /* Reserved */
  uint8_t TAAC;                /* Data read access-time 1 */
  uint8_t NSAC;                /* Data read access-time 2 in CLK cycles */
  uint8_t MaxBusClkFrec;       /* Max. bus clock frequency */
  uint16_t CardComdClasses;    /* Card command classes */
  uint8_t RdBlockLen;          /* Max. read data block length */
  uint8_t PartBlockRead;       /* Partial blocks for read allowed */
  uint8_t WrBlockMisalign;     /* Write block misalignment */
  uint8_t RdBlockMisalign;     /* Read block misalignment */
  uint8_t DSRImpl;             /* DSR implemented */
  uint8_t Reserved2;           /* Reserved */
  uint32_t DeviceSize;         /* Device Size */
  uint8_t MaxRdCurrentVDDMin;  /* Max. read current @ VDD min */
  uint8_t MaxRdCurrentVDDMax;  /* Max. read current @ VDD max */
  uint8_t MaxWrCurrentVDDMin;  /* Max. write current @ VDD min */
  uint8_t MaxWrCurrentVDDMax;  /* Max. write current @ VDD max */
  uint8_t DeviceSizeMul;       /* Device size multiplier */
  uint8_t EraseGrSize;         /* Erase group size */
  uint8_t EraseGrMul;          /* Erase group size multiplier */
  uint8_t WrProtectGrSize;     /* Write protect group size */
  uint8_t WrProtectGrEnable;   /* Write protect group enable */
  uint8_t ManDeflECC;          /* Manufacturer default ECC */
  uint8_t WrSpeedFact;         /* Write speed factor */
  uint8_t MaxWrBlockLen;       /* Max. write data block length */
  uint8_t WriteBlockPaPartial; /* Partial blocks for write allowed */
  uint8_t Reserved3;           /* Reserded */
  uint8_t ContentProtectAppli; /* Content protection application */
  uint8_t FileFormatGrouop;    /* File format group */
  uint8_t CopyFlag;            /* Copy flag (OTP) */
  uint8_t PermWrProtect;       /* Permanent write protection */
  uint8_t TempWrProtect;       /* Temporary write protection */
  uint8_t FileFormat;          /* File Format */
  uint8_t ECC;                 /* ECC code */
  uint8_t CSD_CRC;             /* CSD CRC */
  uint8_t Reserved4;           /* always 1*/
} SD_CSD;

typedef struct /*Card Identification Data*/
{
  uint8_t ManufacturerID; /* ManufacturerID */
  uint16_t OEM_AppliID;   /* OEM/Application ID */
  uint32_t ProdName1;     /* Product Name part1 */
  uint8_t ProdName2;      /* Product Name part2*/
  uint8_t ProdRev;        /* Product Revision */
  uint32_t ProdSN;        /* Product Serial Number */
  uint8_t Reserved1;      /* Reserved1 */
  uint16_t ManufactDate;  /* Manufacturing Date */
  uint8_t CID_CRC;        /* CID CRC */
  uint8_t Reserved2;      /* always 1 */
} SD_CID;

typedef struct /*Card Identification Data*/
{
  uint32_t sizeofword;
  uint32_t word_pos;
  uint32_t err;
  uint32_t *data;
} SD_DATA;

#define DISABLE_SDIO_IRQ() disable_irq(PORT_WAKEUP_INT) // CLRB(REG32(IE), PORT_WAKEUP_INT);	//disable wakeup interrupt
#define ENABLE_SDIO_IRQ() enable_irq(PORT_WAKEUP_INT)   // SETB(REG32(IE), PORT_WAKEUP_INT);	//enable wakeup interrupt

typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint32_t CardCapacity;  /* Card Capacity */
  uint32_t CardBlockSize; /* Card Block Size */
  uint16_t RCA;           /* ÉñÂíÀ´µÄ£¿ */
  uint8_t CardType;
} SD_CardInfo;

/* SDIO CIS Tuple code */
#define CISTPL_NULL 0x00
#define CISTPL_CHECKSUM 0x10
#define CISTPL_VERS_1 0x15
#define CISTPL_ALTSTR 0x16
#define CISTPL_MANFID 0x20
#define CISTPL_FUNCID 0x21
#define CISTPL_FUNCE 0x22
#define CISTPL_SDIO_STD 0x91
#define CISTPL_SDIO_EXT 0x92
#define CISTPL_END 0xff

/* Exported constants --------------------------------------------------------*/
#define SD_DMA_MODE ((uint32_t)0x00000000)
#define SD_INTERRUPT_MODE ((uint32_t)0x00000001)
#define SD_POLLING_MODE ((uint32_t)0x00000002)

/* Supported Memory Cards */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1 ((uint32_t)0x0)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0 ((uint32_t)0x1)
#define SDIO_HIGH_CAPACITY_SD_CARD ((uint32_t)0x2)
#define SDIO_MULTIMEDIA_CARD ((uint32_t)0x3)
#define SDIO_SECURE_DIGITAL_IO_CARD ((uint32_t)0x4)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD ((uint32_t)0x5)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD ((uint32_t)0x6)
#define SDIO_HIGH_CAPACITY_MMC_CARD ((uint32_t)0x7)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int sdio_init(struct mmc_host *host, struct mmc_card *card);
void sdio_irq_handler(uint32_t irqno, struct mmc_host *host, uint32_t *regs);
void sdio_irq_thread(void *data);
void disable_sdio_int_ext(void);
void enable_sdio_int_ext(void);

#define ENABLE_4BIT_DATA_BUS 1
// #include "sdc.h"
#include "sdio_ops.h"
#include "sdio_io.h"

#endif /* __SDCARD_H */

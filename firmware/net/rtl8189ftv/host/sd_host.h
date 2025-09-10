#ifndef _SD_H_
#define _SD_H_

#define SDC_MODULE_SELECT 0

enum sd_operation
{
	SD_TX,
	SD_RX
};

///////////////////////////////////////////////////////////////////////////////////////
// Test Ò»Ïß/ËÄÏßÄ£Ê½
#define SDC_BUS_WIDTH 4 // 1: 1-bit data bus width; 4: 4-bit data bus width

///////////////////////////////////////////////////////////////////////////////////////
// SDC²¨ÌØÂÊ
#define SDC__BAUD 4
#define SD_CLK 20000000

///////////////////////////////////////////////////////////////////////////////////////
// block length
#define BLOCK_LENGTH 512

// SDC  IO  Define
#if SDC_MODULE_SELECT
#define SDC_CFG0 REG32(SDC1_CFG0)
#define SDC_CFG1 REG32(SDC1_CFG1)
#define SDC_CFG2 REG32(SDC1_CFG2)
#define SDC_INDEX REG32(SDC1_INDEX)
#define SDC_ARG REG32(SDC1_ARG)
#define SDC_DCNT REG32(SDC1_DCNT)
#define SDC_BAUD REG32(SDC1_BAUD)
#define SDC_RESP0 REG32(SDC1_RESP0)
#define SDC_RESP1 REG32(SDC1_RESP1)
#define SDC_RESP2 REG32(SDC1_RESP2)
#define SDC_RESP3 REG32(SDC1_RESP3)
#define SDC_RESP4 REG32(SDC1_RESP4)
#else
#define SDC_CFG0 (SDC0_CFG0)
#define SDC_CFG1 (SDC0_CFG1)
#define SDC_CFG2 (SDC0_CFG2)
#define SDC_INDEX (SDC0_INDEX)
#define SDC_ARG (SDC0_ARG)
#define SDC_DCNT (SDC0_DCNT)
#define SDC_BAUD (SDC0_BAUD)
#define SDC_RESP0 (SDC0_RESP0)
#define SDC_RESP1 (SDC0_RESP1)
#define SDC_RESP2 (SDC0_RESP2)
#define SDC_RESP3 (SDC0_RESP3)
#define SDC_RESP4 (SDC0_RESP4)
#endif

/*
 *	SDC Responses Type
 *
 *	6 byte responses type:
 *					byte0=command index=SDC_RESP1[7:0]
 *					byte1=SDC_RESP0[31:24]
 *					byte2=SDC_RESP0[23:16]
 *					byte3=SDC_RESP0[15:8]
 *					byte4=SDC_RESP0[7:0]
 *
 *
 *	17 byte responses type:
 *					byte0=command index=SDC_RESP4[7:0]
 *					byte1=SDC_RESP3[31:24]
 *					byte2=SDC_RESP3[23:16]
 *					byte3=SDC_RESP3[15:8]
 *					byte4=SDC_RESP3[7:0]
 *					byte5=SDC_RESP2[31:24]
 *					byte6=SDC_RESP2[23:16]
 *					byte7=SDC_RESP2[15:8]
 *					byte8=SDC_RESP2[7:0]
 *					byte9=SDC_RESP1[31:24]
 *					byte10=SDC_RESP1[23:16]
 *					byte11=SDC_RESP1[15:8]
 *					byte12=SDC_RESP1[7:0]
 *					byte13=SDC_RESP0[23:16]
 *					byte14=SDC_RESP0[15:8]
 *					byte15=SDC_RESP0[7:0]
 */

// 6 byte responses type:
#define RSP6_47_40 ((SDC_RESP1 >> 0) & 0xff)
#define RSP6_39_32 ((SDC_RESP0 >> 24) & 0xff)
#define RSP6_31_24 ((SDC_RESP0 >> 16) & 0xff)
#define RSP6_23_16 ((SDC_RESP0 >> 8) & 0xff)
#define RSP6_15_8 ((SDC_RESP0 >> 0) & 0xff)
// #define RSP6_7_0	((SDC_RESP0 >> 24) & 0xff)

// 17 byte responses type:
#define RSP17_135_128 ((SDC_RESP4 >> 0) & 0xff)
#define RSP17_127_120 ((SDC_RESP3 >> 24) & 0xff)
#define RSP17_119_112 ((SDC_RESP3 >> 16) & 0xff)
#define RSP17_111_104 ((SDC_RESP3 >> 8) & 0xff)
#define RSP17_103_96 ((SDC_RESP3 >> 0) & 0xff)
#define RSP17_95_88 ((SDC_RESP2 >> 24) & 0xff)
#define RSP17_87_80 ((SDC_RESP2 >> 16) & 0xff)
#define RSP17_79_72 ((SDC_RESP2 >> 8) & 0xff)
#define RSP17_71_64 ((SDC_RESP2 >> 0) & 0xff)
#define RSP17_63_56 ((SDC_RESP1 >> 24) & 0xff)
#define RSP17_55_48 ((SDC_RESP1 >> 16) & 0xff)
#define RSP17_47_40 ((SDC_RESP1 >> 8) & 0xff)
#define RSP17_39_32 ((SDC_RESP1 >> 0) & 0xff)
#define RSP17_31_24 ((SDC_RESP0 >> 16) & 0xff)
#define RSP17_23_16 ((SDC_RESP0 >> 8) & 0xff)
#define RSP17_15_8 ((SDC_RESP0 >> 0) & 0xff)
// #define RSP17_7_0		((SDC_RESP0 >> 0 ) & 0xff)

// Cmd Responses Type
#define NO_RESP ((1 << 2) | (0 << 1) | (0 << 0)) // send cmd. without receive response; without check busy
#define RESP_1 ((0 << 2) | (0 << 1) | (1 << 0))	 // send cmd. receive 6 byte response;  without check busy
#define RESP_1B ((1 << 2) | (0 << 1) | (1 << 0)) // send cmd. receive 6 byte response;  with check busy
#define RESP_2 ((0 << 2) | (1 << 1) | (1 << 0))	 // send cmd. receive 17 byte response; without check busy
#define RESP_3 ((0 << 2) | (0 << 1) | (1 << 0))	 // send cmd. receive 6 byte response; without check busy
#define RESP_6 ((0 << 2) | (0 << 1) | (1 << 0))	 // send cmd. receive 6 byte response; without check busy
#define RESP_7 ((0 << 2) | (0 << 1) | (1 << 0))	 // send cmd. receive 6 byte response; without check busy

/*
 *
 * */
enum sd_version
{
	NOT_SD,
	SD_10,
	SD_20_SC,
	SD_20_HC,
	MMC,
};

typedef enum
{
	SD_FREE,
	SD_READ,
	SD_WRITE,
	//   SD_ABEND,
	SD_OFFLINE
} CARD_STATE;

typedef struct
{
	unsigned short dwRCA;
	unsigned int dwCap;
	unsigned int dwNextLBA;
	enum sd_version eVer;
	CARD_STATE eCardState;
} SD_INFO;

enum card_status
{
	DISONLINE, // SD¿¨²»ÔÚÏß
	UNACTIVE,  // SD¿¨¸Õ²åÈë,Î´½øÐÐ³õÊ¼»¯
	ACTIVED,   // SD¿¨³õÊ¼»¯³É¹¦
	INVALID,   // SD¿¨ÎÞÐ§
};

/*
 *
 * */
enum card_operation
{
	OPT_NONE,
	OPT_READ,
	OPT_WRITE,
};

/*
 *
 * */
enum card_type
{
	UNUSABLE,
	SD_VER_1,
	SD_VER_2,
	MMC_TYPE,
};

/*
 * MMC device
 */
struct sd_card
{
	unsigned short wRca;
	unsigned int iPreLba; // ÓÃÓÚ¼ÇÂ¼ÉÏÒ»´Î²Ù×÷µÄÂß¼­¿éµØÖ·
	unsigned int dwCapacity;
	unsigned char bVersion;
	bool bSdhc;
	enum card_type bCardType;
	enum card_status volatile bCardStatus;
	enum card_operation volatile bLastOperation;
} __attribute__((packed));

extern void sd_insert_card(void); // SD¿¨²åÈëÊ±µ÷ÓÃ¸Ãº¯Êý,Íâ²¿²»ÐèÒªÆäËü±äÁ¿¼ÇÂ¼SD¿¨µÄÔÚÏßÇé¿ö
extern void sd_remove_card(void); // SD¿¨ÒÆ³ýÊ±µ÷ÓÃ¸Ãº¯Êý,Íâ²¿²»ÐèÒªÆäËü±äÁ¿¼ÇÂ¼SD¿¨µÄÔÚÏßÇé¿ö

extern bool sd_read(unsigned int pDataBuf, unsigned int dwLba, unsigned int dwLBANum);
extern bool sd_write(unsigned int pDataBuf, unsigned int dwLba, unsigned int dwLBANum);
extern unsigned int sd_n_sectors(void);
extern bool sd_stop_transmission(void);
extern struct sd_card g_tSDMMC_Var;
enum card_status sd_get_card_status(void);
bool sdc_init(void);
#endif /* _SD_H_ */

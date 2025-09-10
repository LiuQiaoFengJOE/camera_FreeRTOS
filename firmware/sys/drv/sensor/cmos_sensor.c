#include "basic_defs.h"
#include "osal/osal_alloc.h"
#include "drv/i2c/i2c.h"
#include "cmos_sensor.h"
#include "debug.h"
#include "sys/sys.h"
#include "task/app.h"
#include "fatfs/ff.h"

// static Sensor_Init_t *devSensorInit=NULL; // read from resource
// static Sensor_Op_t *devSensorOp=NULL;  // read from resource
static _Sensor_Ident_ *devSensorInit = NULL; // read from resource
static _Sensor_Adpt_ *devSensorOp = NULL;

static void *iic_controller;
static INT8U devWRId, devRDId, devAddrW, devDataW;
extern uint32_t _sensor_head_start[];
extern uint32_t _sensor_head_end[];
const Sensor_Op_t test_img =
	{
		.typ = CSI_TYPE_COLOR_BAR, // test color
		.pixelw = 1280,
		.pixelh = 720,
		.hsyn = 1,
		.vsyn = 1,
		.colrarray = 0, // 0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
		.AVDD = 0x16,
		.DVDD = 0x06,
		.VDDIO = 0x10,
		.rotate_adapt = {0},
		.hvb_adapt = {48000000, 2000, 0, 0, 0xff},
		.mclk = 16000000, // 19000000
		.pclk_fir_en = 0,
		.pclk_inv_en = 0,
		.csi_tun = 0,
		.isp_all_mod = 0, //_ISP_AUTO_<<_YGAMA_POS_,//_ISP_MODE_DEFAULT_
		//.hgrm_adapt = {256,0,8,240,480,800,1040,180,360,640,680,0x5a96a555,0x1a96a,0x1000000,0x0},//0x5a96a555,0x1a96a//0x2000000,0x0//0x0a815000,0x5
		.ae_adapt = {.exp_adapt = {4, {24, 40, 60, 80, 80, 80, 100, 136}, 3, 195 * 256, 12, 4, 2048, 0x4142},																				  // 28->36
					 .hgrm_adapt = {256, 160, 320, 960, 1120, 200, 400, 640, 680, 0x55555555, 0x55555555, 0x55555555, 0x01, {8, 9, 10, 11, 12, 13, 14, 15}, {8, 9, 10, 11, 12, 13, 14, 15}}}, // 0.5,2,2
		.blc_adapt = {-4, -6, -6, -4},
		.ddc_adapt = {2, 1, 0, 0, 16, {8, 8, 8, 8, 8, 8, 8, 8}, {8, 8, 8, 8, 8, 8, 8, 8}, 1, {2, 2, 2, 2, 2, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {5, 8, 12, 15, 20, 25, 30}, 0},
		.awb_adapt = {0x03, 188, 620, 3, 2, 0xa, 0xc0, 0, 3, 0, 3, 0, 0, {(348 << 20) | (256 << 10) | (370 << 0), (368 << 20) | (256 << 10) | (350 << 0), (465 << 20) | (256 << 10) | (225 << 0), (370 << 20) | (256 << 10) | (385 << 0), (370 << 20) | (256 << 10) | (385 << 0)}, 0, {0}, {0}, {0}, 0}, // manual:0xff
		//.ccm_adapt = {0x38,0x00,0x04,0x04,0x44,-0x4,0x04,-0x4,0x40,0x00,0x00,0x00},
		.ccm_adapt = {{0x100, 0x00, 0x00,
					   0x00, 0x100, 0x00,
					   0x00, 0x00, 0x100},
					  0x0c,
					  0xc,
					  0xc},
		//.ygama_adapt = {NULL,0,{11,14,17,17,17,17,17,17},0,0,0xff,144,208,96,144},
		.ygama_adapt = {0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0xff, 144, 208, 96, 144},
		.rgbgama_adapt = {0, {1, 1, 1, 1, 1, 1, 1, 1}, {20, 12, 12, 12, 12, 12, 12, 12}, 0, 0xff, 0, 0xff, 0, 0xff, 64, 224, 4000}, // 2
		.ch_adapt = {1, 1, {1, 1, 1, 1, 1, 1}, {320, 192, 320, 256, 384, 128}, {64, 64, 192, 128, 256, 0}, {12, 6, 6, 12, 6, 6}, {6, 12, 6, 6, 12, 6}, {6, 6, 12, 6, 6, 12}, {2, 4, 6, 8, 12, 16, 20, 24, 28, 28, 28, 28, 28, 28, 28, 28, 28}, {32, 32, 32, 32, 32, 32, 32, 32}},
		.vde_adapt = {0x80, 0x80, 0x80, 0x80, {72, 72, 72, 72, 72, 72, 72, 72, 72}},

		.cfd_adapt = {4, 0xb0, 0x20, 1, 1, 2, 0xff, 0},
		.md_adapt = {40, 40, 1, 1, 1280, 180, 720},
		.p_fun_adapt = {NULL, NULL, NULL},

};
const Sensor_Init_t null_init = {&test_img, sizeof(test_img), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void sensor_iic_info(INT8U wid, INT8U rid, INT8U awidth, INT8U dwidth)
{
	devWRId = wid;
	devRDId = rid;
	devAddrW = awidth;
	devDataW = dwidth;
}

_Sensor_Ident_ *hal_csi_SeninitGet(void)
{
	return devSensorInit;
}
uint32_t sensor_iic_read(uint8_t *data)
{
	uint32_t temp = 0;

	if (devAddrW == 2)
	{
		uint16_t addr;
		addr = (data[0] << 8) | data[1];
		i2c16bitAddrRead(iic_controller, devWRId, addr, (uint8_t *)&temp, devDataW);
	}
	else
		i2c8bitAddrRead(iic_controller, devWRId, data[0], (uint8_t *)&temp, devDataW);

	return temp;
}

void sensor_iic_write(uint8_t *data)
{
	if (devAddrW == 2)
	{
		uint16_t addr;
		addr = (data[0] << 8) | data[1];
		i2c16bitAddrWrite(iic_controller, devWRId, addr, &data[2], devDataW);
	}
	else
		i2c8bitAddrWrite(iic_controller, devWRId, data[0], &data[1], devDataW);
}

static int sensorCheckId(Sensor_Init_t *p_sensor_ident)
{
	uint8_t u8Buf[3];
	uint32_t id = 0;

	if (0)
	{
		printf(
			"w_cmd    = %02X\nr_cmd    = %02X\naddr_num = %02X\ndata_num = %02X\nid_reg  = %04X\nid      = %02X\n",
			p_sensor_ident->w_cmd,
			p_sensor_ident->r_cmd,
			p_sensor_ident->addr_num,
			p_sensor_ident->data_num,
			p_sensor_ident->id_reg,
			p_sensor_ident->id);
	}

	if (p_sensor_ident->addr_num == 2)
	{
		u8Buf[0] = p_sensor_ident->id_reg >> 8;
		u8Buf[1] = p_sensor_ident->id_reg;
	}
	else
		u8Buf[0] = p_sensor_ident->id_reg;

	sensor_iic_info(p_sensor_ident->w_cmd, p_sensor_ident->r_cmd, p_sensor_ident->addr_num, p_sensor_ident->data_num);
	osal_task_sleep(10);
	id = sensor_iic_read(u8Buf);
	// printf ("id = %X\n", id);
	if (id == p_sensor_ident->id)
		return 1;
	else
		return -1;
}

u8 sensor_id = 0;
Sensor_Init_t *sensorAutoCheck(void)
{
	//_Sensor_Header_ sensorHeader;
	// uint32_t addr, addrSensor;
	uint32_t i, num;
	Sensor_Init_t *sensor;
	Sensor_Init_t *sensor1084=NULL;
#if 0
  addr = res_open(RES_SENSOR);
  res_read(addr, &sensorHeader, sizeof(_Sensor_Header_));
  printf("header_items_addr:%x\nheader_items_total_size:%x\n", sensorHeader.header_items_addr, sensorHeader.header_items_total_size);
  num = sensorHeader.header_items_total_size / sizeof(Sensor_Init_t);
  printf("num:%x   size:%x\n", num, sizeof(Sensor_Init_t));
  addrSensor = addr + sensorHeader.header_items_addr;
  printf("addrsensor:%x\n", addrSensor);
#endif
	printf("sensor head start:0x%x,end:0x%x\n", _sensor_head_start, _sensor_head_end);
	num = ((uint32_t)_sensor_head_end - (uint32_t)_sensor_head_start) / sizeof(Sensor_Init_t);
	printf("sensor sum:%d\n", num);
	sensor = _sensor_head_start;
	for (i = 0; i < num; ++i)
	{
		// res_read(addrSensor, sensor, sizeof(Sensor_Init_t));
		if (sensorCheckId(sensor) >= 0)
		{
			sensor_id = sensor->id;
			printf("found sensor id = %X\n", sensor->id);

			return sensor;
		}
		printf("sensor check:0x%x,0x%x,0x%x\n",sensor->id_reg,sensor->id,sensor->w_cmd);
		if(sensor->id==0x84)
			sensor1084=sensor;
		sensor++;
		// addrSensor += sizeof(Sensor_Init_t);
	}
	//if(sensor1084)
	//	return sensor1084;
	printf("not found sensor id\n");
	return &null_init;
}

int sensorInitTab(Sensor_Init_t *sensor)
{
	uint32_t i, step;
	uint8_t *initbuf = sensor->sensor_init_tab_adr;
	if (initbuf == NULL)
		return -1;
#if 0
  uint8_t *initbuf = (uint8_t *)osal_malloc(sensor->sensor_init_tab_size);
  if (NULL == initbuf)
  {
    printf("sensorInitTab malloc error\n");
    return -1;
  }

  res_read(addr + sensor->sensor_init_tab_adr, initbuf, sensor->sensor_init_tab_size);
#endif
	step = sensor->addr_num + sensor->data_num;
	for (i = 0; initbuf[i] != 0xFF || initbuf[i + 1] != 0xFF; i += step)
	{
		sensor_iic_write(&initbuf[i]);
		if (i == 0)
			ax32xx_sysCpuMsDelay(10);
	}

	// osal_free(initbuf);

	return 0;
}

void sensorInitTable(Sensor_Init_t *sensor, uint32_t addr)
{
	_Sensor_Common_ common;

	p_verb("y:%X rgb:%X contrast:%X", sensor->ygamma_tab_adr, sensor->rgb_gamma_tab_adr, sensor->sensor_struct_addr);
	res_read(addr + sizeof(_Sensor_Header_), &common, sizeof(common));

	// sensor_struct 用完了，可以拿來保存contrast_tab
	sensor->sensor_struct_addr = common.contrast_tab_adr + addr;
	sensor->sensor_struct_size = common.contrast_tab_size;
	// 如果沒有定義gamma表，則使用默認表
	if (!sensor->ygamma_tab_adr)
	{
		sensor->ygamma_tab_adr = common.gamma_tab_adr + addr;
		sensor->ygamma_tab_size = common.gamma_tab_size;
	}
	else
		sensor->ygamma_tab_adr += addr;
	if (!sensor->rgb_gamma_tab_adr)
	{
		sensor->rgb_gamma_tab_adr = common.gamma_tab_adr + addr;
		sensor->rgb_gamma_tab_size = common.gamma_tab_size;
	}
	else
		sensor->rgb_gamma_tab_adr += addr;
	if (sensor->lsc_tab_adr)
	{
		sensor->lsc_tab_adr += addr;
	}
	p_verb("sen:%X gamma:%X contrast:%X", addr, common.gamma_tab_adr, common.contrast_tab_adr);
	p_verb("y:%X rgb:%X contrast:%X  lsc:%x", sensor->ygamma_tab_adr, sensor->rgb_gamma_tab_adr, sensor->sensor_struct_addr, sensor->lsc_tab_adr);
}

/* 獲取三個函數的最小地址，當成是三個函數的起始地址。
 * 有一點冒險，所以必須要求sensor的三個函數不要太複雜。通常這是可以滿足的。
 */
static uint32_t sensorFuncStart(P_Fun_Adapt *func)
{
	uint32_t addr = ~0;
	if (func->fp_exp_gain_wr && (uint32_t)func->fp_exp_gain_wr < addr)
		addr = func->fp_exp_gain_wr;
	if (func->fp_hvblank && (uint32_t)func->fp_hvblank < addr)
		addr = func->fp_hvblank;
	if (func->fp_rotate && (uint32_t)func->fp_rotate < addr)
		addr = func->fp_rotate;
	return addr;
}

void sensor_func_free(Sensor_Init_t *sensor)
{
	printf("%s:%X\r\n", __FUNCTION__, sensor->sensor_init_tab_adr);
	if (sensor->sensor_init_tab_adr)
	{
		osal_free(sensor->sensor_init_tab_adr);
	}
}

void sensorInitFunc(Sensor_Init_t *sensor, Sensor_Op_t *adapt, uint32_t addr)
{
	uint32_t func;
	uint32_t start, size;

	start = sensorFuncStart(&adapt->p_fun_adapt);
	if (start == ~0)
	{
		sensor->sensor_init_tab_adr = 0;
		return;
	}

	size = sensor->sensor_struct_addr - start;
	printf("size:%x\n", size);
	func = (uint32_t)osal_malloc(size);
	printf("func:%X\r\n", func);
	if (NULL == func)
	{
		p_err("%s malloc error", __func__);
		sensor->sensor_init_tab_adr = 0;
		return;
	}

	res_read(addr + start, (void *)func, size);
	dcache_writeback_region(func, size);

	if (adapt->p_fun_adapt.fp_exp_gain_wr)
		adapt->p_fun_adapt.fp_exp_gain_wr += func - start;
	if (adapt->p_fun_adapt.fp_hvblank)
		adapt->p_fun_adapt.fp_hvblank += func - start;
	if (adapt->p_fun_adapt.fp_rotate)
		adapt->p_fun_adapt.fp_rotate += func - start;

	sensor->sensor_init_tab_adr = func; // 借用這裡保存函數地址。如果需要釋放空間，可以從這裡拿
}

void sensor_printf(u32 pdata)
{
	printf("%d", pdata);
}
// the iic controller should be init already

void sensor_h62_reset()
{
	u8 buf[2];
	buf[0] = 0x12;
	buf[1] = 0x80;
	sensor_iic_write(buf);

	buf[0] = 0x1D;
	buf[1] = 0x00;
	sensor_iic_write(buf);

	buf[0] = 0x1E;
	buf[1] = 0x00;
	sensor_iic_write(buf);
	printf("sensor reset h62\n");
}

void sensorInit(void *controller)
{
	uint32_t addr;
	devSensorOp = NULL;
	devSensorInit = NULL;
	ax32xx_csiMclkSet(14000000); // default mclk for id check
	printf("sensor init start\n");
	// sensor_func_free(&devSensorInit);
	//  if (NULL == iic_controller)
	{
		iic_controller = controller;
		devSensorInit = sensorAutoCheck();
		if (devSensorInit == NULL)
		{
			return;
		}
	} // else return;  // 已經初始化過了？
	devSensorOp = devSensorInit->sensor_struct_addr;
	// res_read(addr + devSensorInit->sensor_struct_addr, devSensorOp, devSensorInit->sensor_struct_size); // todo check size
	devSensorOp->iic_write = sensor_iic_write;
	devSensorOp->iic_read = sensor_iic_read;
	devSensorOp->sen_debg = sensor_printf;
	printf("mclk = %d\n", devSensorOp->mclk);
	ax32xx_csiMclkSet(devSensorOp->mclk);
#if 0
	ax32xx_sysLDOSet(SYS_LDO_LSEN, devSensorOp->DVDD, 1);
	ax32xx_sysCpuMsDelay(1);
#endif
	ax32xx_sysLDOSet(SYS_LDO_HSEN, devSensorOp->AVDD, 1);
	ax32xx_sysCpuMsDelay(1);

	sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
	sensorInitTab(devSensorInit); // iic初始化表格

	// 下列兩個順序不要顛倒。要先Func再Table。因此Table中的一些地址值要在Func中用
	// sensorInitFunc(&devSensorInit, devSensorOp, addr); // 處理iic反饋的三個函數，也在資源中，必須支持 PIC
	// sensorInitTable(&devSensorInit, addr);             // 修正gamma/contrast表地址，處理默認表格
	// hal_csiRegister (devSensorOp, &devSensorInit);
	sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
}

void *hal_csiAdptGet(void)
{
	return devSensorOp;
}
void *sensor_adapt_get(void)
{
	return devSensorOp;
}

void *sensor_init_get(void)
{
	return devSensorInit;
}

void sensorGetRes(u32 *w, u32 *h)
{
	if (w)
		*w = devSensorOp->pixelw;
	if (h)
		*h = devSensorOp->pixelh;
}

char *SensorGetName(void)
{
	if (devSensorInit == NULL)
		return ((char *)("NULL"));

	return devSensorInit->sensor_name;
}
static u32 ispmode_tmp;
int cut_sdr_data_en;
void hal_save_frame_isr(unsigned int a, void *b, void *c)
{
	static u32 fnum = 0;
	if (fnum > 0)
	{
		// ax32xx_csiEnable(0);
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN, 0);
		// ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END,NULL);
		csiirq_unregister_jdma_frame_end();
		cut_sdr_data_en = 1;
		ISPMODE = ispmode_tmp;
	}
	fnum++;
}
void hal_Cut_SDR_SrcCSI(u8 *sdr_buf, bool csi_save)
{
	_Sensor_Adpt_ *senradpt = hal_csiAdptGet();
	ispmode_tmp = ISPMODE;
	if (csi_save)
	{
		ISPMODE = ISP_MODE_CSI_SAVEEN;
	}
	else
	{
		ISPMODE = ISP_MODE_CFAEN;
	}
	if (sdr_buf != NULL)
	{
		csiirq_register_jdma_frame_end(hal_save_frame_isr, NULL);
		ax32xx_csiMJPEGFrameSet(sdr_buf, sdr_buf + senradpt->pixelw * senradpt->pixelh, senradpt->pixelh, senradpt->pixelw * (csi_save + 1));
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN, 1);
	}
	ax32xx_csiEnable(1);
}
void sdram_data_capture_task(u8 *fname)
{
	// capture rawdata
	void *fp;
	_Sensor_Adpt_ *senradpt = hal_csiAdptGet();
	printf("capture rawdata start\n");
	char filename[18] = "RAWD0000.raw";
	if(modeFreeSpace()<senradpt->pixelw * senradpt->pixelh * 2)
	{
		printf("capture rawdata err,no memory,%dKB\n",modeFreeSpace()>>10);
		return ;
	}
	hal_Cut_SDR_SrcCSI(getCurModeFreeBuffBase(), 1);
	FRESULT ret = f_mkdir(_DATDSK_"/DEBG");
	if (ret != FR_OK)
	{
		if (ret != FR_EXIST)
		{
			printf("DEBG mkdir err\n");
			return;
		}
	}
	f_chdir(_DATDSK_"/DEBG");
	u32 filenum, loop = 100;
	while (loop--)
	{
		if (cut_sdr_data_en == 1)
		{
			printf("capture sdrdata pending\n");
			uint write;
			for (filenum = 0; filenum < 1000; filenum++)
			{
				filename[4] = '0' + ((filenum / 1000) % 10);
				filename[5] = '0' + ((filenum / 100) % 10);
				filename[6] = '0' + ((filenum / 10) % 10);
				filename[7] = '0' + (filenum % 10);
				fp=osal_fopen((char const *)filename, "r");
				if (fp==NULL)
				{
					fp=osal_fopen((char const *)filename, "w");
					if (fp)
					{
						osal_fwrite(getCurModeFreeBuffBase(), 1,senradpt->pixelw * senradpt->pixelh * 2,fp);
						printf("Raw len = %x\n", write);
						loop = 0;
						memcpy(fname, filename, sizeof(filename));
					}
					osal_fclose(fp);
					break;
				}
				osal_fclose(fp);
			}
		}
		osal_task_sleep(10);
	}
}

void Sensor_DebugRegister(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG

	// printf("Sensor_DebugRegister\n");
	// printf("%x%x",pbdata[0],pbdata[1]);
	u8 u8AddrLength = param_3B & 0xff;
	u8 u8DataLength = (param_3B & 0xff00) >> 8;
	// printf("%x,%x",u8AddrLength,u8DataLength);

	// sensor_iic_write(pbdata);

	if (u8AddrLength == 2)
	{
		INT16U addr;
		addr = pbdata[0];
		addr = (addr << 8) | pbdata[1];
		i2c16bitAddrWrite(iic_controller, devWRId, addr, &pbdata[2], u8DataLength);
	}
	else
	{
		i2c8bitAddrWrite(iic_controller, devWRId, pbdata[0], &pbdata[1], u8DataLength);
	}
#endif
	/**/
}

void usb_cut_raw(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	cut_sdr_data_en = 0;
	sdram_data_capture_task(pbdata);
	printf("usb cut raw finish\n");

#endif
}
void Sensor_common_Get(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	// printf(__func__);
	static u32 i = 0;
	hal_Sensor_common_Get(pbdata, trans_len, param_3B);
	/*if(!i){
		extern int dispLayerInit(INT8U layer);
		dispLayerInit(0);
		i++;
	}*/
#endif
}
void Sensor_common_Set(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	// printf(__func__);
	hal_Sensor_common_Set(pbdata, trans_len, param_3B);
#endif
}


void sensor_change(void)
{
#if 0
	static u8 senser_select = 0;

	printf("sensorChange start, %d\n", senser_select);
	hal_csiEnable(0);
	lcdPreviewStop();//dispLayerUninit(DISP_LAYER_VIDEO);
	
	if(senser_select)
	{
        senser_select=0;
		sensor_reset(0);
		sensor_pwdn(1);
	}
	else
	{
        senser_select=1;
		sensor_reset(1);
		sensor_pwdn(0);
	}
	void *i2c = i2c_get_controller(0);
	printf("i2c controller %08X\n", i2c);
	i2cInit(i2c, 40000);
	printf("i2cInit\n");
	sensorInit(i2c);
	
	osal_task_sleep(400);		// wait AE ok
	hal_csiEnable(1);
	lcdPreviewStart();//dispLayerInit(DISP_LAYER_VIDEO);
	printf("change end, %d\n", senser_select);
#endif	
}

void isp_debug_write(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	//  printf(__func__);
	hal_isp_debug_write(pbdata, trans_len, param_3B);
	/*	int isp_mode = param_3B & 0xff;
		int startPos = param_3B >> 8;
		int i;
		printf("\n");
		printf("isp_mode:%x len:%d 3B:%x\n",isp_mode,trans_len,param_3B);
		switch(isp_mode){
			case _AE_POS_		:	printf("write ae\n");
									AE *ae_p = (AE *)pbdata;
									printf("%d\n",ae_p->exp_adapt.gain_max);
									devSensor_Cmd.ae_adapt.exp_adapt.gain_max = ae_p->exp_adapt.gain_max;
									memcpy(&devSensor_Cmd.ae_adapt.exp_adapt.exp_tag,&(ae_p->exp_adapt.exp_tag),sizeof(ae_p->exp_adapt.exp_tag));
									break;
			case _BLC_POS_  	: 	printf("wirte blc\n");
									BLC *blc_p = (BLC *)pbdata;
									devSensor_Cmd.blc_adapt.blkl_r = blc_p->blkl_r;
									devSensor_Cmd.blc_adapt.blkl_gr = blc_p->blkl_gr;
									devSensor_Cmd.blc_adapt.blkl_gb = blc_p->blkl_gb;
									devSensor_Cmd.blc_adapt.blkl_b = blc_p->blkl_b;
									//printf("%d %d %d %d\n",devSensor_Cmd.blc_adapt.blkl_r,devSensor_Cmd.blc_adapt.blkl_gr,devSensor_Cmd.blc_adapt.blkl_gb,devSensor_Cmd.blc_adapt.blkl_b);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _LSC_POS_       : 	printf("wirte lsc:\n");
									CopyToMember(lsc_tab, 0, sizeof(lsc_tab), pbdata, startPos, trans_len);

									for(i = startPos/2;i<(((startPos+trans_len)/2)<572?((startPos+trans_len)/2):572);i++){
										printf("  ");
										uart_Put_dec(lsc_tab[i]);
									}
									if(_GET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _DDC_POS_       : 	printf("write ddc\n");
									memcpy(&(devSensor_Cmd.ddc_adapt),pbdata,sizeof(devSensor_Cmd.ddc_adapt));
									devSensor_Cmd.ddc_adapt.indx_table[0] = devSensor_Cmd.ddc_adapt.ddc_class;
									devSensor_Cmd.ddc_adapt.indx_table[1] = clip((devSensor_Cmd.ddc_adapt.ddc_class-2),0,5);
									printf("%d\n",devSensor_Cmd.ddc_adapt.hot_num);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;
			case _AWB_POS_       : 	printf("wirte awb:\n");
									awb_update(pbdata,trans_len,param_3B);
									printf("seg_mode:%d\n",devSensor_Cmd.awb_adapt.seg_mode);
									//printf("ymin:%d\n",devSensor_Cmd.awb_adapt.ymin);
									//printf("ymax:%d\n",devSensor_Cmd.awb_adapt.ymax);
									//printf("cb_th:%d\n",devSensor_Cmd.awb_adapt.cb_th[0]);

									for(i =0;i<128;i++){
										printf("  %d",devSensor_Cmd.awb_adapt.awb_tab[i]);
									}
									printf("\n");
									_SET_ISP_MODE_(isp_module_state,_AWB_POS_,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									break;

			case _CCM_POS_       : 	printf("write ccm\n");
									CCM *ccm_p = (CCM *)pbdata;
									memcpy(&(devSensor_Cmd.ccm_adapt.ccm),ccm_p->ccm,sizeof(ccm_p->ccm));
									printf("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[0],devSensor_Cmd.ccm_adapt.ccm[1],devSensor_Cmd.ccm_adapt.ccm[2]);
									printf("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[3],devSensor_Cmd.ccm_adapt.ccm[4],devSensor_Cmd.ccm_adapt.ccm[5]);
									printf("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[6],devSensor_Cmd.ccm_adapt.ccm[7],devSensor_Cmd.ccm_adapt.ccm[8]);
									_SET_ISP_MODE_(isp_module_state,_CCM_POS_,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									break;

			case _DGAIN_POS_ 	 :	printf("write dgain\n");
									//	memcpy(&devSensor_Cmd.rgbdgain_adapt,pbdata,sizeof(devSensor_Cmd.rgbdgain_adapt));
									if(_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _YGAMA_POS_     : 	printf("wirte ygama\n");
									//ygama_update(pbdata,trans_len,param_3B);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _RGB_GAMA_POS_  : 	printf("write rgb_gamma\n");
									//	memcpy(&(devSensor_Cmd.rgbgama_adapt),pbdata,sizeof(devSensor_Cmd.rgbgama_adapt));
									if(_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _CH_POS_		 :	printf("write ch\n");
									CH *ch_p = (CH *)pbdata;
									memcpy(&devSensor_Cmd.ch_adapt.enhence,&ch_p->enhence,sizeof(ch_p->enhence));
									memcpy(&devSensor_Cmd.ch_adapt.r_rate,&ch_p->r_rate,sizeof(ch_p->r_rate));
									memcpy(&devSensor_Cmd.ch_adapt.g_rate,&ch_p->g_rate,sizeof(ch_p->g_rate));
									memcpy(&devSensor_Cmd.ch_adapt.b_rate,&ch_p->b_rate,sizeof(ch_p->b_rate));*/
	/*
	printf("en:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.enhence[0],devSensor_Cmd.ch_adapt.enhence[1],devSensor_Cmd.ch_adapt.enhence[2]
	,devSensor_Cmd.ch_adapt.enhence[3],devSensor_Cmd.ch_adapt.enhence[4],devSensor_Cmd.ch_adapt.enhence[5]);
	printf("ra:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.r_rate[0],devSensor_Cmd.ch_adapt.r_rate[1],devSensor_Cmd.ch_adapt.r_rate[2]
	,devSensor_Cmd.ch_adapt.r_rate[3],devSensor_Cmd.ch_adapt.r_rate[4],devSensor_Cmd.ch_adapt.r_rate[5]);
	printf("ga:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.g_rate[0],devSensor_Cmd.ch_adapt.g_rate[1],devSensor_Cmd.ch_adapt.g_rate[2]
	,devSensor_Cmd.ch_adapt.g_rate[3],devSensor_Cmd.ch_adapt.g_rate[4],devSensor_Cmd.ch_adapt.g_rate[5]);
	printf("ba:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.b_rate[0],devSensor_Cmd.ch_adapt.b_rate[1],devSensor_Cmd.ch_adapt.b_rate[2]
	,devSensor_Cmd.ch_adapt.b_rate[3],devSensor_Cmd.ch_adapt.b_rate[4],devSensor_Cmd.ch_adapt.b_rate[5]);
	printf("%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.rate[0],devSensor_Cmd.ch_adapt.rate[1],devSensor_Cmd.ch_adapt.rate[2]
	,devSensor_Cmd.ch_adapt.rate[3],devSensor_Cmd.ch_adapt.rate[4],devSensor_Cmd.ch_adapt.rate[5],devSensor_Cmd.ch_adapt.rate[6]
	,devSensor_Cmd.ch_adapt.rate[7]);*/

	/*			if(_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _EE_POS_        : 	printf("write ee\n");
				EE *ee_p = (EE *)pbdata;
				devSensor_Cmd.ee_adapt.ee_class = ee_p->ee_class;
				printf("ee_class:%d\n",devSensor_Cmd.ee_adapt.ee_class);
				if(_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;
case _VDE_POS_      : 	printf("write vde\n");
				VDE *vde_p = (VDE *)pbdata;
				devSensor_Cmd.vde_adapt.contra = vde_p->contra;
				devSensor_Cmd.vde_adapt.bright_k = vde_p->bright_k;
				devSensor_Cmd.vde_adapt.bright_oft = vde_p->bright_oft;
				devSensor_Cmd.vde_adapt.hue = vde_p->hue;
				memcpy(&(devSensor_Cmd.vde_adapt.sat_rate),vde_p->sat_rate,sizeof(vde_p->sat_rate));
				if(_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _CFD_POS_       :// memcpy(&(devSensor_Cmd.cfd_adapt),pbdata,sizeof(devSensor_Cmd.cfd_adapt));

				if(_GET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _SAJ_POS_		:	printf("write saj\n");
				SAJ *saj_p = (SAJ *)pbdata;
				memcpy(&(devSensor_Cmd.saj_adapt.sat_rate),saj_p->sat_rate,sizeof(saj_p->sat_rate));
				printf("%d %d %d %d %d %d %d %d\n",devSensor_Cmd.saj_adapt.sat_rate[0],devSensor_Cmd.saj_adapt.sat_rate[1],devSensor_Cmd.saj_adapt.sat_rate[2]
				,devSensor_Cmd.saj_adapt.sat_rate[3],devSensor_Cmd.saj_adapt.sat_rate[4],devSensor_Cmd.saj_adapt.sat_rate[5],devSensor_Cmd.saj_adapt.sat_rate[6]
				,devSensor_Cmd.saj_adapt.sat_rate[7]);
				if(_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;


default :break;
}
//	tell the main that who was changed
//	printf("sta:%x\n",isp_module_state);
//	tell the main modules was changed
//	_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
printf("isp write out\n");*/
#endif
}

void isp_debug_read(u8 *pbdata, u32 trans_len, u32 param_3B)
{

#if CMOS_USB_ONLINE_DBG
	// printf(__func__);
	hal_isp_debug_read(pbdata, trans_len, param_3B);
#endif
}

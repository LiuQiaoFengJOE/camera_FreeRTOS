#include "dac/dac.h"
#include "dac/ax32xx_dac.h"
#include "stream/mqueue.h"
#include "AX329x.h"
const static u32 halSampleRateTable[] =
	{
		48000, 44100, 32000, 0, 24000, 22050, 16000, 0, 12000, 11025, 8000};
#define DAC_QUEUE_SIZE 5
#define DAC_MAX_SIZE (32 * 1024)
typedef struct _dac_manage
{
	queue playQueue;
	u32 queueSize;
	u32 dacBuffSize;
	u32 playBufAddr;
	u32 playBufPos;
	u32 playBufSize;
	void *curParame;
	_getPlayBuff getPlayBuff;
	_playDone playDone;
	u8 playing;
	u8 useHalfint;
} dac_anage;
static dac_anage dacManage;
static int dac_frame_int(uint32_t irq, void *d, void *reg)
{
	dac_anage *pDac = (dac_anage *)d;
	//printf("dac start\n");
	ax32xx_dacBufferFlush(pDac->dacBuffSize);
	if (queueValidData(&pDac->playQueue) > 2)
	{
		printf("dac queue err:%d\n", queueValidData(&pDac->playQueue));
	}
}
static int dac_frame_half(uint32_t irq, void *d, void *reg)
{
	dac_anage *pDac = (dac_anage *)d;
	void *buffArg;
	u32 curPlayBuffSize = 0;
	//printf("dac half\n");
	if (pDac->useHalfint)
	{
		pDac->playBufPos += pDac->dacBuffSize;
		if (pDac->playBufPos < pDac->playBufSize)
		{
			curPlayBuffSize = pDac->playBufSize - pDac->playBufPos;
			if (curPlayBuffSize > DAC_MAX_SIZE)
				curPlayBuffSize = DAC_MAX_SIZE;
			ax32xx_dacBufferSet(pDac->playBufAddr + pDac->playBufPos, curPlayBuffSize);
			printf("aud size:%d\n", curPlayBuffSize);
			pDac->dacBuffSize = curPlayBuffSize;
			pDac->playBufPos += curPlayBuffSize;
			return;
		}
		pDac->playBufAddr = NULL;
		pDac->playBufSize = 0;
		pDac->playBufPos = 0;
		pDac->dacBuffSize = 0;
		if (pDac->getPlayBuff)
		{
			buffArg = pDac->getPlayBuff(&pDac->playBufAddr, &pDac->playBufSize, pDac->curParame);
			if (buffArg)
			{
				if (queueValidData(&pDac->playQueue) < DAC_QUEUE_SIZE)
					queueInput(&pDac->playQueue, buffArg);
				else
				{
					if (pDac->playDone)
						pDac->playDone(buffArg);
					printf("aud queue1:%d\n", queueValidData(&pDac->playQueue));
					return;
				}
			}
			while (queueValidData(&pDac->playQueue) > 2)
			{
				if (queueOutput(&pDac->playQueue, &buffArg) >= 0)
					if (pDac->playDone)
						pDac->playDone(buffArg);
			}
			if (pDac->playBufAddr && pDac->playBufSize)
			{
				curPlayBuffSize = pDac->playBufPos = pDac->playBufSize;
				if (curPlayBuffSize > DAC_MAX_SIZE)
					curPlayBuffSize = DAC_MAX_SIZE;
				ax32xx_dacBufferSet(pDac->playBufAddr, curPlayBuffSize);
				pDac->playBufPos = 0;
				pDac->dacBuffSize = curPlayBuffSize;
			}
		}
	}
}
static int dac_frame_done(uint32_t irq, void *d, void *reg)
{
	dac_anage *pDac = (dac_anage *)d;
	void *buffArg;
	u32 curPlayBuffSize = 0;
	//printf("dac empty\n");
	if (pDac->useHalfint==0)
	{
		pDac->playBufPos += pDac->dacBuffSize;
		if (pDac->playBufPos < pDac->playBufSize)
		{
			curPlayBuffSize = pDac->playBufSize - pDac->playBufPos;
			if (curPlayBuffSize > DAC_MAX_SIZE)
				curPlayBuffSize = DAC_MAX_SIZE;
			ax32xx_dacStart(pDac->playBufAddr + pDac->playBufPos, curPlayBuffSize);
			printf("aud size1:%d\n", curPlayBuffSize);
			pDac->dacBuffSize = curPlayBuffSize;
			//pDac->playBufPos += curPlayBuffSize;
			return;
		}
		pDac->playBufAddr = NULL;
		pDac->playBufSize = 0;
		pDac->playBufPos = 0;
		pDac->dacBuffSize = 0;
		if (pDac->getPlayBuff)
		{
			buffArg = pDac->getPlayBuff(&pDac->playBufAddr, &pDac->playBufSize, pDac->curParame);
			if (buffArg)
			{
				if (queueValidData(&pDac->playQueue) < DAC_QUEUE_SIZE)
					queueInput(&pDac->playQueue, buffArg);
				else
				{
					if (pDac->playDone)
						pDac->playDone(buffArg);
					printf("aud queue1:%d\n", queueValidData(&pDac->playQueue));
					return;
				}
			}
			while (queueValidData(&pDac->playQueue) > 2)
			{
				if (queueOutput(&pDac->playQueue, &buffArg) >= 0)
					if (pDac->playDone)
						pDac->playDone(buffArg);
			}
			if (pDac->playBufAddr && pDac->playBufSize)
			{
				curPlayBuffSize = pDac->playBufPos = pDac->playBufSize;
				if (curPlayBuffSize > DAC_MAX_SIZE)
					curPlayBuffSize = DAC_MAX_SIZE;
				ax32xx_dacStart(pDac->playBufAddr, curPlayBuffSize);
				pDac->playBufPos = 0;
				pDac->dacBuffSize = curPlayBuffSize;
			}
		}
	}

	while (queueValidData(&pDac->playQueue) > 2)
	{
		if (queueOutput(&pDac->playQueue, &buffArg) >= 0)
			if (pDac->playDone)
				pDac->playDone(buffArg);
	}

	if (pDac->playBufAddr == NULL || pDac->playBufSize == 0)
	{
		while (queueValidData(&pDac->playQueue))
		{
			if (queueOutput(&pDac->playQueue, &buffArg) >= 0)
				if (pDac->playDone)
					pDac->playDone(buffArg);
		}
		//printf("dac stop\n");
		ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(0));
		ax32xx_dacStop();
		pDac->playing = 0;
	}
}
void dacInit(void)
{
	queueInit(&dacManage.playQueue, DAC_QUEUE_SIZE);
	dacManage.queueSize = DAC_QUEUE_SIZE;
	dacManage.playing = 0;
	ax32xx_dacInit();
	ax32xx_dacEnable(0);
	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(0));
	dac_register_empty(dac_frame_done, &dacManage);
	dac_register_half(dac_frame_half, &dacManage);
	dac_register_INT(dac_frame_int, &dacManage);
	ax32xx_dacStop();
	ax32xx_dacEnable(1);
	//ax32xx_dacSampleRateSet(DAC_SAMPLE_RATE_16000); // 16k
}

s32 dacPlay(u32 sampleRate, u8 volume, _getPlayBuff getPlayBuff, _playDone playDone, void *curParame)
{
	int i;
	void *curPlayBuffArg;
	uint32_t flags;
	u8 *curPlayBuff = NULL;
	u32 curPlayBuffSize = 0;
	
	if (sampleRate == 0)
	{
		printf("sampleRate err:%d\n",sampleRate);
		return -1;
	}

	flags = arch_local_irq_save();
	if (dacManage.playing)
	{
		arch_local_irq_restore(flags);
		printf("dac is using\n");
		return -1;
	}

	for (i = 0; i < 11; i++)
	{
		if (sampleRate == halSampleRateTable[i])
			break;
	}
	if (i >= 11)
	{
		printf("sampleRate err:%d\n",sampleRate);
		arch_local_irq_restore(flags);
		return -1;
	}
	
	dacManage.playing = 1;
	arch_local_irq_restore(flags);

	dacManage.playBufAddr = NULL;
	dacManage.playBufSize = 0;
	dacManage.playBufPos = 0;
	dacManage.getPlayBuff = getPlayBuff;
	dacManage.playDone = playDone;
	dacManage.curParame = curParame;
	dacManage.useHalfint = 1;

	curPlayBuffArg = dacManage.getPlayBuff(&curPlayBuff, &curPlayBuffSize, dacManage.curParame);
	if (queueValidData(&dacManage.playQueue))
	{
		while (1)
		{
			printf("dac play err\n");
		}
	}
	if (curPlayBuff == NULL || curPlayBuffSize == 0)
	{
		if (curPlayBuffArg && dacManage.playDone)
			dacManage.playDone(curPlayBuffArg);
		dacManage.playing = 0;
		printf("dac play err,len:%d\n",curPlayBuffSize);
		return -1;
	}
	if (curPlayBuffArg)
	{
		queueInput(&dacManage.playQueue, curPlayBuffArg);
	}
	dacManage.playBufAddr = curPlayBuff;
	dacManage.playBufSize = curPlayBuffSize;
	ax32xx_dacSampleRateSet(i);
	
	if (curPlayBuffSize > DAC_MAX_SIZE)
		curPlayBuffSize = DAC_MAX_SIZE;
	dacManage.dacBuffSize = curPlayBuffSize;
	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(volume));
	printf("dac play, addr:0x%x,len:%d,sampleRate:%d,%d\n",curPlayBuff,curPlayBuffSize,sampleRate,i);
	ax32xx_dacStart(curPlayBuff, curPlayBuffSize);
	//osal_task_sleep(5);
	//ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(volume));
	return 0;
}

s32 dacPlayBuff(u32 sampleRate, u8 volume, u8 *curPlayBuff, u32 curPlayBuffSize)
{
	int i;
	uint32_t flags;
	
	if (sampleRate == 0)
		return -1;
	
	flags = arch_local_irq_save();
	if (dacManage.playing)
	{
		arch_local_irq_restore(flags);
		return -1;
	}

	for (i = 0; i < 11; i++)
	{
		if (sampleRate == halSampleRateTable[i])
			break;
	}
	if (i >= 11)
	{
		arch_local_irq_restore(flags);
		return -1;
	}
	
	dacManage.playing = 1;
	arch_local_irq_restore(flags);

	dacManage.playBufAddr = curPlayBuff;
	dacManage.playBufSize = curPlayBuffSize;
	dacManage.playBufPos = 0;
	dacManage.getPlayBuff = NULL;
	dacManage.playDone = NULL;
	dacManage.curParame = NULL;
	dacManage.useHalfint = 0;

	ax32xx_dacSampleRateSet(i);
	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(volume));
	if (curPlayBuffSize > DAC_MAX_SIZE)
		curPlayBuffSize = DAC_MAX_SIZE;
	dacManage.dacBuffSize = curPlayBuffSize;
	ax32xx_dacStart(curPlayBuff, curPlayBuffSize);
	return 0;
}
int dacIsPlaying(void)
{
	if(dacManage.playing)
		return 1;
	return 0;
}
void dacPlaySound(u32 resId,u8 vol)
{
#define SOUND_LEN    (32*1024)
#define SOUND_SAMLE   16000
#define SOUND_HEAD_LEN   44
	static u8 *soundBuff=NULL;
	static u32 curResId=0xffffffff;
	static u32 curResLen;
	if(dacIsPlaying())
	{
		printf("dac is working\n");
		return;
	}
	if(soundBuff==NULL)
		soundBuff=osal_malloc(SOUND_LEN+4);
	if(soundBuff==NULL)
	{
		printf("err,no buff save sound\n");
		return;
	}
	soundBuff=((u32)soundBuff+3)&(~0x3);
	if(curResId!=resId)
	{
		u32 addr = res_open(resId);
		uint32_t size = res_size(resId);
		if(size==0||size<=SOUND_HEAD_LEN)
		{
			printf("res [%d] size err\n",resId);
			return;
		}
		size-=SOUND_HEAD_LEN;
		if(size>SOUND_LEN)
			size=SOUND_LEN;
		res_read(addr+SOUND_HEAD_LEN, soundBuff, size);
		curResId=resId;
		curResLen=size;
	}
	dacPlayBuff(SOUND_SAMLE, vol,soundBuff,curResLen);
}



#include "config/conf_parse.h"

#include "rtp-media/jpeg.h"
#include "stream/stream.h"
#include "osal/osal_sema.h"
#include "osal/osal_task.h"

#include "debug.h"
#include "rtsp/rtsp.h"

////////////////////////////////////////////////////////////////////////////////
static int get_sdp(char *dest, int len, int payload, int port, void *d)
{
	printf("audio get_sdp\r\n");
	return snprintf(dest, len, "m=audio %d RTP/AVP 97\r\na=rtpmap:97 L16/16000/1\r\n", port);
}

static int get_payload(int payload, void *d)
{
	return 97;
}

void memCpyReversion(u16 *dest, u8 *src, u32 len)
{
	u32 i;
	len = len >> 1;
	for (i = 0; i < len; i++)
	{
		dest[i] = ((*src) << 8) | (*(src + 1));
		src += 2;
	}
}
extern int stopStreamTrans;
// 这个与视频不一样的是，音频每次只发一个8184的包，视频一帧包含多个包，所以这里只用获取一次frame.buf
static int audio_pack(struct frame *r, struct stream_src_self *self, struct frame *f)
{
	// printf("Line:%d\tfunc:%s\n",__LINE__,__FUNCTION__);
	// return 0;
	struct chunklist *heap = self->heap;
	struct send_item *si;
	UINT8 *buf;
	struct frame_buf *fbuf;
	UINT8 *inter, *rtphdr, *realdata;
	UINT32 timestamp;
	int rtp_bufsize, audio_bufsize;
	int src;
	int cplen;
	UINT8 *dat;
	UINT8 t;
	u16 i;
	timestamp = f->timestamp;

	rtp_bufsize = r->stream->ops->bufsize(r); // 1440
					  // 在后面大小端修改后，除掉前面8字节01wb:size
	src = 0;
	// 
	if(f->buf.len&&f->buf.dat)
	{
		audio_bufsize = f->buf.len;
		buf = f->buf.dat;
	}
	else
	{
		if(list_empty(&f->buflist))
			return -1;
		fbuf = list_entry(f->buflist.next, struct frame_buf, list);
		audio_bufsize = f->dataLen;
		buf = fbuf->dat;
	}
	// 跳过前8字节01wb+size
	// 大小端的转换，PCM网络传输是大端，储存是小端
	/*	for(i=0;i<audio_bufsize;i+=2)
		{
			t = buf[i];
			buf[i] = buf[i+1];
			buf[i+1] = t;
		}*/
	//printf("au:%d\n",f->headLen);
	dat = buf + f->headLen;
	audio_bufsize -= f->headLen; ////除掉前面8字节01wb:size，audio_bufsize=8192

	while (1)
	{
		if(stopStreamTrans)
			return 1;
		// todo : 改用frame_buffer_new是否規範些？
		struct list_head *newbuf = frameNewBuffer(r);
		// p_dbg("heap=%X\n",heap);

		// 获取不到buf，导致无法传输
		if (NULL == newbuf)
		{
			// 這裏不用管釋放send_list，由packer統一處理
			// p_dbg("send_item_new NULL. heap=%X\n", heap);
			return 1;
		}
		si = list_entry(newbuf, struct send_item, list);
		inter = si->buf.dat;
		inter[0] = 0;
		rtphdr = inter + RTP_HDRSZ_INTERLEAVE;
		realdata = rtphdr + 12;
		cplen = audio_bufsize - src;
		if (cplen > rtp_bufsize - (12+RTP_HDRSZ_INTERLEAVE))
		{
			cplen = rtp_bufsize - (12+RTP_HDRSZ_INTERLEAVE);
		}
		memCpyReversion(realdata, dat + src, cplen);
		// memcpy(realdata,dat+src,cplen);
		src += cplen;
		PUT_32(rtphdr + 4, timestamp);
		si->buf.len = cplen + (12+RTP_HDRSZ_INTERLEAVE);
		if (src == audio_bufsize)
		{

			// printf("")
			return 0;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
static struct media_jpeg media_ops = {
	.m = {
		.get_sdp = get_sdp,
		.get_payload = get_payload,
		.pack = audio_pack,
		.userpack = NULL},
	.need_scan = 1};

void rtp_audio_register(void)
{
	media_register("audio", &media_ops);
}

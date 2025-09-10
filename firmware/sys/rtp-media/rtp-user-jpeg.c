#include "config/conf_parse.h"

#include "rtp-media/jpeg.h"
#include "stream/stream.h"
#include "osal/osal_sema.h"
#include "osal/osal_task.h"

#include "debug.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief pack frame f to send_list, allocated send_item from heap
 * @param f         源frame，原图的帧
 * @param r         在这里不需要用到,保持兼容
 * @param user      客户的结构体,尽量保持与原来pack的兼容,因此使用void指针
 * @retval -1 failure	不删除frame r,由外部删除
 * @retval  0 OK			不删除frame r		,外部删除
 */

static int user_pack(struct frame *r, void *d, struct frame *f)
{
  // printf("user_pack!\r\n");
  struct user *userdata = (struct user *)d;
  userdata->len = jpg_write_to_buf(userdata->picture, f);
  userdata->flag = 1;
  // printf("user_pack len:%d\r\n",userdata->len);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
static struct media_jpeg media_ops = {
    .m = {
        .get_sdp = NULL,
        .get_payload = NULL,
        .pack = NULL,
        .userpack = user_pack},
    .need_scan = 1};

void rtp_user_jpeg_register(void)
{
  media_register("user-jpeg", &media_ops);
}

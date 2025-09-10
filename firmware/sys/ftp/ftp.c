/**@file    ftp.c
 * @author  何凱帆
 * @brief   ftp支持
 * @details 提供支持函數
 */

#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "osal/osal_rand.h"

#include "debug.h"

#include "ftp.h"
#include "ftp-fs.h"

#define DEBUG_FTP 1

/*响应ftp客户端时候用这些宏即可，ftp的响应是三位数字，第一位，第二位，第三位，分别代表不同的意思*/

#define FTP_DATACONN 150

#define FTP_NOOPOK 200
#define FTP_TYPEOK 200
#define FTP_PORTOK 200
#define FTP_EPRTOK 200
#define FTP_UMASKOK 200
#define FTP_CHMODOK 200
#define FTP_EPSVALLOK 200
#define FTP_STRUOK 200
#define FTP_MODEOK 200
#define FTP_PBSZOK 200
#define FTP_PROTOK 200
#define FTP_OPTSOK 200
#define FTP_ALLOOK 202
#define FTP_FEAT 211
#define FTP_STATOK 211
#define FTP_SIZEOK 213
#define FTP_MDTMOK 213
#define FTP_STATFILE_OK 213
#define FTP_SITEHELP 214
#define FTP_HELP 214
#define FTP_SYSTOK 215
#define FTP_GREET 220
#define FTP_GOODBYE 221
#define FTP_ABOR_NOCONN 225
#define FTP_TRANSFEROK 226
#define FTP_ABOROK 226
#define FTP_PASVOK 227
#define FTP_EPSVOK 229
#define FTP_LOGINOK 230
#define FTP_AUTHOK 234
#define FTP_CWDOK 250
#define FTP_RMDIROK 250
#define FTP_DELEOK 250
#define FTP_RENAMEOK 250
#define FTP_PWDOK 257
#define FTP_MKDIROK 257

#define FTP_GIVEPWORD 331
#define FTP_RESTOK 350
#define FTP_RNFROK 350

#define FTP_IDLE_TIMEOUT 421
#define FTP_DATA_TIMEOUT 421
#define FTP_TOO_MANY_USERS 421
#define FTP_IP_LIMIT 421
#define FTP_IP_DENY 421
#define FTP_TLS_FAIL 421
#define FTP_BADSENDCONN 425
#define FTP_BADSENDNET 426
#define FTP_BADSENDFILE 451

#define FTP_BADCMD 500
#define FTP_BADOPTS 501
#define FTP_COMMANDNOTIMPL 502
#define FTP_NEEDUSER 503
#define FTP_NEEDRNFR 503
#define FTP_BADPBSZ 503
#define FTP_BADPROT 503
#define FTP_BADSTRU 504
#define FTP_BADMODE 504
#define FTP_BADAUTH 504
#define FTP_NOSUCHPROT 504
#define FTP_NEEDENCRYPT 522
#define FTP_EPSVBAD 522
#define FTP_DATATLSBAD 522
#define FTP_LOGINERR 530
#define FTP_NOHANDLEPROT 536
#define FTP_FILEFAIL 550
#define FTP_NOPERM 550
#define FTP_UPLOADFAIL 553

extern struct netif *p_netif;

/**********************************************/
enum
{
  DCMD_NONE,
  DCMD_QUIT,
  DCMD_ABOR,
  DCMD_PASV,
  DCMD_LIST,
  DCMD_LIST_ROOT,
  DCMD_RETR,
  DCMD_STOR,
};

typedef struct ftp_session
{
  // 控制连接
  int ctrl_sfd; // sock handle

  char cmd[1024];
  char existname[255];
  char path[255];
  char *arg;

  // 数据连接
  struct sockaddr_in data_addr;
  int data_cfd;
  int data_dfd; // tran data with cline

  // FTP协议状态标志
  char is_ascii;

  // 文件状态
  DWORD file_offest; // for REST & RETR command, 32bit, 4GB max

  wait_queue_head_t datachan_done;
  wait_queue_head_t datachan_wait;
  DWORD datachan_cmd;
  void *datachan_arg;
  char type;
  char datachan_done_cmd;
} session_t;

typedef struct ftpcmd
{
  const char *cmd;
  void (*cmd_handler)(struct ftp_session *);
} ftpcmd_t;

/**********************************************/

void virtualThumbdir(session_t *sess, char *path)
{
  //printf("virtualThumbdir:%s\r\n", path);
  if (path[0] == '/' && path[1] == '2')
  {
    sess->type = 1;
    path[1] = '1'; // 转到实际的目录
    printf("path is thumbdir:%s!!!\r\n", path);
  }
  else
  {
    sess->type = 0;
  }
  memcpy(sess->path, path, strlen(path) + 1);
}

void virtualThumbdirFile(session_t *sess, char *path)
{
  printf("virtualThumbdirFile:%s\r\n", path);
  if (path[0] == '/' && path[1] == '2')
  {
    sess->type = 1;
    path[1] = '1'; // 转到实际的目录
    printf("path is thumbdir:%s!!!\r\n", path);
  }
  else
  {
    sess->type = 0;
  }
}

void ftp_reply(int fd, int status, const char *text)
{
  char buf[1024] = {0};
  sprintf(buf, "%d %s \r\n", status, text);
  //printf("ftp_reply:%s\n", buf);
  write(fd, buf, strlen(buf));
}

void welcome_cline(int fd)
{
  ftp_reply(fd, FTP_GREET, "Welcome to FTP server of vision");
}

void trim_req(session_t *sess)
{
  char *str;
  for (str = sess->cmd; *str && *str != '\r' && *str != '\n'; ++str)
    ;
  *str = 0;
}

/********************************** Command  **********************************/
void do_quit(session_t *sess)
{
  ftp_reply(sess->ctrl_sfd, FTP_GOODBYE, "Goodbye ");
}

void do_user(session_t *sess)
{
  ftp_reply(sess->ctrl_sfd, FTP_GIVEPWORD, "Please specify the password ");
}

void do_pass(session_t *sess)
{
  ftp_reply(sess->ctrl_sfd, FTP_LOGINOK, "Login successful.");
}

void do_syst(session_t *sess)
{
  ftp_reply(sess->ctrl_sfd, FTP_SYSTOK, "UNIX Type: L8");
}

// todo: getcwd
void do_pwd(session_t *sess)
{
  char buf[256];
  // f_chdrive("1:");
  // ftp_getcwd( buf, 255 );
  // p_info("pwd111 %s\r\n", buf+1);
  // ftp_reply(sess->ctrl_sfd, FTP_PWDOK,buf+1);

  if (sess->path[0] == 0)
  {
    buf[0] = '\"';
    buf[1] = '/';
    buf[2] = '\"';
    buf[3] = '\0';
    memcpy(sess->path, "/", 2);
    ftp_reply(sess->ctrl_sfd, FTP_PWDOK, buf);
  }
  else
  {
    ftp_getcwd(buf, 128);
    ftp_reply(sess->ctrl_sfd, FTP_PWDOK, buf);
  }

  // p_info("pwd %s\r\n", buf);
}

void do_cwd(session_t *sess)
{
  char *dir = sess->cmd + 4;
  trim_req(sess);

  virtualThumbdir(sess, dir);

  if (ftp_setcwd2(sess->path) < 0)
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "Failed to change directory.");
    p_info("Failed to change directory.\r\n");
  }
  else
  {
    ftp_reply(sess->ctrl_sfd, FTP_CWDOK, "Directory successfully changed.");
    memcpy(sess->path, dir, strlen(dir) + 1);
  }
}

void do_type(session_t *sess)
{
  char *arg = sess->cmd + 5;
  if (arg[0] == 'A' || arg[1] == 'a')
  {
    sess->is_ascii = 1;
    ftp_reply(sess->ctrl_sfd, FTP_TYPEOK, "Switching to ASCII mode.");
  }
  else if (arg[0] == 'I' || arg[1] == 'i')
  {
    sess->is_ascii = 0;
    ftp_reply(sess->ctrl_sfd, FTP_TYPEOK, "Switching to Binary mode.");
  }
  else
    ftp_reply(sess->ctrl_sfd, FTP_BADCMD, "Unrecognised TYPE command.");
}

void do_rest(session_t *sess)
{
  char *arg = sess->cmd + 5;
  char c;
  DWORD file_offest = 0;

  while ((c = *arg))
  {
    if (c == '\r' || c == 'n')
      break;
    else if (c >= 0x30 && c <= 0x39)
    {
      file_offest = file_offest * 10 + (c - '0');
      ++arg;
      continue;
    }
    p_info("REST syntax error in parameter.\n");
    sess->file_offest = 0;
    ftp_reply(sess->ctrl_sfd, FTP_BADOPTS, "REST syntax error in parameter.");
    return;
  }
  p_info("REST set position at %lu\n", file_offest);
  sess->file_offest = file_offest;
  ftp_reply(sess->ctrl_sfd, FTP_RESTOK, "REST set OK.");
}

void do_feat(session_t *sess)
{
  char features[64] = {0};
  char *rest = "REST STREAM";
  sprintf(features, "%d-Features:\n %s\n%d end\r\n", FTP_FEAT, rest, FTP_FEAT);
  write(sess->ctrl_sfd, features, strlen(features));
}

void do_mkd(session_t *sess)
{
  char buf[256];
  char directoryLen;
  int res;
  directoryLen = strlen(sess->cmd) - 4 - 2;
  // buf[0] = '\'';
  // buf[1] = '/';
  memcpy(buf, &sess->cmd[4], directoryLen);
  // buf[2+directoryLen]='\'';
  buf[directoryLen] = 0;
  p_info("mkd %s\r\n", buf);
  // ftp_reply( sess->ctrl_sfd, FTP_BADCMD, "Unknown command." );
  res = osal_fmkdir(buf);
  if (res == 0)
  {
    ftp_reply(sess->ctrl_sfd, FTP_MKDIROK, buf);
  }
  else
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "create fail.");
  }
}

void do_rmd(session_t *sess)
{
  char buf[256];
  char directoryLen;
  int res = 1;
  directoryLen = strlen(sess->cmd) - 4 - 2;
  memcpy(buf, &sess->cmd[4], directoryLen);
  // buf[2+directoryLen]='\'';
  buf[directoryLen] = 0;
  res = osal_unlink(buf);
  if (res == 0)
  {
    ftp_reply(sess->ctrl_sfd, FTP_MKDIROK, buf);
  }
  else
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "Cannot delete directory.");
  }
}

void do_dele(session_t *sess)
{
  char buf[256];
  char directoryLen;
  int res = 1;
  directoryLen = strlen(sess->cmd) - 5 - 2;
  memcpy(buf, &sess->cmd[5], directoryLen);
  buf[directoryLen] = 0;
  p_info("dele:%s\t%dname'len byte\r\n", buf, strlen(buf));
  res = osal_unlink(buf);
  if (res == 0)
  {
    ftp_reply(sess->ctrl_sfd, FTP_DELEOK, buf);
  }
  else
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "File unavailable.");
  }
}

void do_stor(session_t *sess)
{
  void *dir;
  char buf[256];
  char directoryLen;
  void *fp;
  directoryLen = strlen(sess->cmd) - 5 - 2;
  memcpy(buf, &sess->cmd[5], directoryLen);
  buf[directoryLen] = '\0';
  if (INVALID_SOCKET == sess->data_cfd)
  {
    ftp_reply(sess->ctrl_sfd, FTP_BADPROT, "Bad sequence of commands.");
    p_err("LIST but no PASV\r\n");
    return;
  }
  // trim_req(sess);

  fp = osal_fopen(buf, "wb");
  if (!fp)
  {
    ftp_reply(sess->ctrl_sfd, FTP_UPLOADFAIL, "Cannot create file.");
    p_err("Cannot open file:%s\r\n", buf);
    return;
  }

  p_info("do_stor");

  ftp_reply(sess->ctrl_sfd, FTP_DATACONN, "Here ready transport file.");

  sess->datachan_arg = fp;
  sess->datachan_cmd = DCMD_STOR;
  sess->datachan_done_cmd = 0;
  wake_up(&sess->datachan_wait);
  wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);
}

// 查找是否有这个名称的文件
u8 existfile(void *dir, void *filename)
{
  void *finfo;
  char *name;
  printf("exist filename:%s\n", filename);
  while (finfo = osal_readdir(dir))
  {
    name = osal_dirent_name(finfo);
    if (!name)
    {
      return 0;
    }

    if (!strcmp(name, filename))
    {
      return 1;
    }
  }
  return 0;
}

void do_rnfr(session_t *sess)
{
  void *dir;
  char directoryLen;
  char buf[256];
  directoryLen = strlen(sess->cmd) - 5 - 2;
  memcpy(buf, &sess->cmd[5], directoryLen);
  buf[directoryLen] = '\0';

  dir = osal_opendir("");

  if (existfile(dir, buf))
  {
    ftp_reply(sess->ctrl_sfd, FTP_RNFROK, "Target exists.");
    memcpy(sess->existname, buf, directoryLen + 1);
  }
  else
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "NO Target exists.");
    sess->existname[0] = 0;
  }
}

void do_rnto(session_t *sess)
{
  char directoryLen;
  char buf[256];
  char res;
  directoryLen = strlen(sess->cmd) - 5 - 2;
  memcpy(buf, &sess->cmd[5], directoryLen);
  buf[directoryLen] = '\0';
  printf("sess:%s\tnewname:%s\n", sess->existname, buf);
  if (!sess->existname[0])
  {
    ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "No Recv Command rnfr.");
  }
  else
  {
    res = osal_rename(sess->existname, buf);
    if (!res)
    {
      ftp_reply(sess->ctrl_sfd, FTP_RENAMEOK, "RNTO command success.");
      sess->existname[0] = 0;
    }
    else
    {
      printf("rename fail:%d!\r\n", res);
      ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "RNTO command fail.");
    }
  }
}

/********************************** DataChan **********************************/
static void datachan_end(session_t *sess, int status, const char *text)
{
  if (INVALID_SOCKET != sess->data_dfd)
    osal_socket_close(sess->data_dfd);
  sess->data_dfd = INVALID_SOCKET;
  if (INVALID_SOCKET != sess->data_cfd)
    osal_socket_close(sess->data_cfd);
  sess->data_cfd = INVALID_SOCKET;

  ftp_reply(sess->ctrl_sfd, status, text);

  sess->datachan_cmd = DCMD_NONE;
  sess->datachan_arg = NULL;
}

static void datachan_close(session_t *sess)
{
  if (INVALID_SOCKET != sess->data_dfd)
    osal_socket_close(sess->data_dfd);
  sess->data_dfd = INVALID_SOCKET;
  if (INVALID_SOCKET != sess->data_cfd)
    osal_socket_close(sess->data_cfd);
  sess->data_cfd = INVALID_SOCKET;
  sess->datachan_cmd = DCMD_NONE;
  sess->datachan_arg = NULL;
}

#define B_LEN 8 * 1024
static void *datachan_thread(session_t *sess)
{
  while (1)
  {
    wait_event_interruptible(&sess->datachan_wait, sess->datachan_cmd);

    if (DCMD_QUIT == sess->datachan_cmd)
    {
      datachan_close(sess);
      printf("datachan_thread exit\r\n");
      break;
    }

    if (DCMD_ABOR == sess->datachan_cmd)
    {
      datachan_end(sess, FTP_ABOROK, "ABOR OK.");
    }

    if (DCMD_PASV == sess->datachan_cmd)
    {
      int i;
      struct sockaddr_in addr;
      int fd;

      sess->datachan_cmd = DCMD_NONE;
      sess->datachan_done_cmd = 1;
      wake_up(&sess->datachan_done);

      i = sizeof(addr);
      if ((fd = osal_socket_accept(sess->data_cfd, (struct sockaddr *)&addr, &i)) < 0)
      {
        p_warn("error accepting FTP connection: %s", strerror(errno));
        osal_socket_close(sess->data_cfd);
        sess->data_cfd = INVALID_SOCKET;
      }
      else
      {
        p_info("accepted connection from %s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        if (osal_set_tcp_nodelay(fd, 1) < 0)
        {
          ERR_STR strerr;
          //          osal_socket_strerr(strerr, socket_errno);
          //          p_info("error setting TCP_NODELAY on socket: %s", strerr);
        }

        sess->data_dfd = fd;
      }
      continue;
    }

    if (DCMD_LIST == sess->datachan_cmd && INVALID_SOCKET != sess->data_dfd)
    {
      void *dir = sess->datachan_arg;
      void *finfo;
      char buf[128];
      unsigned int len;
      printf("sess->type:%d\r\n", sess->type);
      while ((finfo = osal_readdir(dir)))
      {
        len = ftp_formatdir(finfo, buf, 128);
        // p_info("%s", buf);
        if (len)
          if (osal_socket_send(sess->data_dfd, buf, len, 0) < 0)
            break;
      }

      osal_closedir(dir);
      datachan_end(sess, FTP_TRANSFEROK, "Directory send OK.");
    }

    if (DCMD_LIST_ROOT == sess->datachan_cmd && INVALID_SOCKET != sess->data_dfd)
    {
      // 虚拟磁盘的数据
      char buf0[128] = "drw-------    2 1000     1000        0 Jan 01 2017 0\r\n";
      char buf1[128] = "drw-------    2 1000     1000        0 Jan 01 2017 1\r\n";
      // char buf2[128] = "drw-------    2 1000     1000        0 Jan 01 2017 2\r\n";
      printf("%s", buf0);
      printf("%s", buf1);
      // printf("%s",buf2);
      osal_socket_send(sess->data_dfd, buf0, strlen(buf0), 0);
      osal_socket_send(sess->data_dfd, buf1, strlen(buf1), 0);
      // osal_socket_send( sess->data_dfd, buf2, strlen(buf2), 0 );
      datachan_end(sess, FTP_TRANSFEROK, "Directory send OK.");
    }

    if (DCMD_STOR == sess->datachan_cmd && INVALID_SOCKET != sess->data_dfd)
    {
      void *fp = sess->datachan_arg;
      char *buf;
      unsigned int len;
      int res;
      unsigned int remain;
      buf = osal_malloc(B_LEN);
      while (1)
      {
        remain = len & 0x03;
        if (remain)
        {
          memcpy(buf, buf + len - remain, remain);
        }
        // 1024为了提高写卡速度,不需要拷贝
        len = osal_socket_recv(sess->data_dfd, buf + remain, B_LEN - remain, 0);
        len += remain;
        if (len == 0)
        {
          // osal_free(buf);
          osal_fclose(fp);
          datachan_end(sess, FTP_TRANSFEROK, "Directory send OK.");
          break;
        }
        else if (len < 0)
        {
          // osal_free(buf);
          osal_fclose(fp);
          printf("some err fail in transport file.");
          datachan_end(sess, FTP_FILEFAIL, "some err fail in transport file.");
          break;
        }
        else
        {
          osal_fwrite(buf, 1, len & (~0x03), fp);
        }
      }
      // osal_fclose(fp);
      osal_free(buf);
      printf("stor finish!\r\n");
    }

    if (DCMD_RETR == sess->datachan_cmd && INVALID_SOCKET != sess->data_dfd)
    {
      DWORD len;
      void *fil = sess->datachan_arg;
      char *buf1 = osal_malloc(B_LEN + 0x10);           // 申请大点的buf
      uint8_t *buf = ((uint32_t)buf1 + 0x10) & (~0x0f); // 进行16byte对齐
      printf("DCMD_RETR sess->type:%d\r\n", sess->type);

      if (buf1)
      {
        while ((len = osal_fread(buf, 1, B_LEN, fil)))
        {
          if (osal_socket_send(sess->data_dfd, buf, len, 0) < 0)
            break;
          if (len < B_LEN || DCMD_RETR != sess->datachan_cmd)
            break;
        }
        osal_free(buf1);

        osal_fclose(fil);

        // 發送了ABOR，要有兩個rtp_reply
        if (DCMD_ABOR == sess->datachan_cmd)
        {
          p_dbg("abor");
          ftp_reply(sess->ctrl_sfd, FTP_ABOROK, "ABOR OK.");
        }
        datachan_end(sess, FTP_TRANSFEROK, "Transfer complete.");
      }
      else
        datachan_end(sess, FTP_BADSENDFILE, "Transfer memory error.");
    }

    // printf("%s end\n",__FUNCTION__);
    sess->datachan_done_cmd = 1;
    wake_up(&sess->datachan_done);

    // printf("%s:%d\n",__FUNCTION__,sess->datachan_done_cmd);
  }
  sess->datachan_done_cmd = 1;
  wake_up(&sess->datachan_done);
  // thread exit
  printf("thread [%s] stack size:%d,use:%d\n",pcTaskGetName(NULL),uxTaskGetStackSize(NULL),uxTaskGetStackSize(NULL)-uxTaskGetStackHighWaterMark(NULL));
  osal_thread_delete(osal_thread_myself());
  return NULL;
}

/************************************ File ************************************/
static void do_abor(session_t *sess)
{
  p_info("ABORT\r\n");
  sess->datachan_cmd = DCMD_ABOR;
  sess->datachan_done_cmd = 0;
  wake_up(&sess->datachan_wait);
  wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);

  // ftp_reply(sess->ctrl_sfd, FTP_ABOROK, "ABOR OK.");
}

static void do_noop(session_t *sess)
{
  p_info("noop\r\n");
  ftp_reply(sess->ctrl_sfd, FTP_NOOPOK, "Nice to NOOP you!");
}

static void do_list(session_t *sess)
{
  void *dir;
  int res;
  char *path = sess->path;
  // list 判断原来是否为虚拟的一个映射目录，是的话就还原
  if (sess->type == 1)
  {
    path[1] = '2';
  }

  if (INVALID_SOCKET == sess->data_cfd)
  {
    ftp_reply(sess->ctrl_sfd, FTP_BADPROT, "Bad sequence of commands.");
    p_err("LIST but no PASV");
    return;
  }
  trim_req(sess);
  // 将虚拟磁盘的部分去掉
  virtualThumbdir(sess, sess->path);
  path = virdisk(sess->path);
  printf("do_list:%s\tpath:%s\r\n", sess->path, path);
  // res = ftp_setcwd2(sess->path);
  // ftp_setcwd(sess->path);
  if (strlen(sess->path) > 1 || path[0] != '/')
  {
    dir = osal_opendir("");
    if (!dir)
    {
      ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "Cannot open directory.");
      p_err("Cannot open directory\r\n");
      return;
    }
    p_info("do_list\r\n");
    ftp_reply(sess->ctrl_sfd, FTP_DATACONN, "Here comes the directory listing.");
    sess->datachan_arg = dir;
    sess->datachan_cmd = DCMD_LIST;
    sess->datachan_done_cmd = 0;
    wake_up(&sess->datachan_wait);
    wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);
  }
  else if (strlen(path) == 1 && path[0] == '/')
  {
    printf("root sess->path:%s\t len:%d\n", sess->path, strlen(sess->path));
    ftp_reply(sess->ctrl_sfd, FTP_DATACONN, "Here comes the directory listing.");
    // sess->datachan_arg = dir;
    sess->datachan_cmd = DCMD_LIST_ROOT;
    sess->datachan_done_cmd = 0;
    wake_up(&sess->datachan_wait);
    wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);
  }

  else
  {
    printf("sess->path:%s len:%d\tres:%d\n", sess->path, strlen(sess->path), res);
    ftp_reply(sess->ctrl_sfd, FTP_BADSENDCONN, "Not found path.");
    // sess->datachan_arg = dir;
    // sess->datachan_cmd = DCMD_LIST_ROOT;
    // wake_up (&sess->datachan_wait);
  }
}

static void do_retr(session_t *sess)
{
  void *finfo;
  void *fil;
  DWORD fsize = 0;
  DWORD headfsize = 0;
  char *path = sess->cmd + 5;
  char buf[128];

  if (INVALID_SOCKET == sess->data_cfd)
  {
    ftp_reply(sess->ctrl_sfd, FTP_BADPROT, "Bad sequence of commands.");
    p_err("LIST but no PASV");
    return;
  }
  trim_req(sess);

  virtualThumbdirFile(sess, path);
  // 转换成实际对应fatfs的路径
  path = virdisk(path);

  fil = osal_fopen(path, "rb");
  if (!fil)
  {
    // ftp_reply(sess->ctrl_sfd, FTP_FILEFAIL, "Failed to open file.");
    datachan_end(sess, FTP_FILEFAIL, "Failed to open file.");
    p_err("Failed to open file");
    return;
  }

  if (sess->type == 0)
  {
    fsize = osal_fsize(fil);

    if (sess->is_ascii)
      sprintf(buf, "Opening ASCII mode data connection for %s (%u bytes).", path, fsize);
    else
      sprintf(buf, "Opening BINARY mode data connection for %s (%u bytes).", path, fsize);
    p_info("%s\n", buf);
    ftp_reply(sess->ctrl_sfd, FTP_DATACONN, buf);

    osal_fseek(fil, sess->file_offest);
    p_info("RETR file at position %ld\n", sess->file_offest);

    sess->datachan_arg = fil;
    sess->datachan_cmd = DCMD_RETR;
    sess->datachan_done_cmd = 0;
    wake_up(&sess->datachan_wait);
    wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);
  }
  else if (sess->type == 1)
  {
    headfsize = ftp_thumb(fil);
    // osal_fseek(fil, 0);
    fsize = osal_fsize(fil) - headfsize;
    if (sess->is_ascii)
      sprintf(buf, "Opening ASCII mode data connection for %s (%u bytes).", path, fsize);
    else
      sprintf(buf, "Opening BINARY mode data connection for %s (%u bytes).", path, fsize);
    p_info("%s\n", buf);
    ftp_reply(sess->ctrl_sfd, FTP_DATACONN, buf);

    // osal_fseek(fil, sess->file_offest);
    // p_info("RETR file at position %ld\n", sess->file_offest );

    sess->datachan_arg = fil;
    sess->datachan_cmd = DCMD_RETR;
    sess->datachan_done_cmd = 0;
    wake_up(&sess->datachan_wait);
    wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);
  }
}

// todo: DATA_PORT需要動態生成，並進行管理
#define DATA_PORT 8888
#define DATA_PORT_START 30000
#define DATA_PORT_END 50000

static void do_pasv(session_t *sess)
{
  char resp[64];
  unsigned int addr;
  unsigned short port;
  struct sockaddr_in saddr;
  socklen_t namelen = sizeof(saddr);
  ERR_STR strerr;
  // printf("rand:%d\r\n",(osal_random()%899+sess->ctrl_sfd)*(osal_random()%10+sess->ctrl_sfd%10)+sess->ctrl_sfd+DATA_PORT_START);

  while (port < DATA_PORT_START || port > DATA_PORT_END)
  {
    port = (osal_random() % 899 + sess->ctrl_sfd) * (osal_random() % 10 + sess->ctrl_sfd % 10) + sess->ctrl_sfd + DATA_PORT_START;
  }

  printf("do_pasv port:%d\r\n", port);
  if (INVALID_SOCKET == sess->data_cfd)
  {
    sess->data_cfd = osal_socket_open(SOCK_STREAM);
    sess->data_addr.sin_family = AF_INET;
    sess->data_addr.sin_port = htons(port); /* host to net, short */
    sess->data_addr.sin_addr.s_addr = INADDR_ANY;
    memset(sess->data_addr.sin_zero, 0, 8);
    // FTP_BADSENDCONN
    if (osal_set_reuseaddr(sess->data_cfd, 1) < 0)
    {
      //      osal_socket_strerr(strerr, socket_errno);
      //      p_warn("ignoring error on setsockopt: %s", strerr);
      ftp_reply(sess->ctrl_sfd, FTP_BADSENDCONN, "reuseaddr err.");
    }
    if (osal_socket_bind(sess->data_cfd, (struct sockaddr *)&sess->data_addr, sizeof(struct sockaddr)) < 0)
    {
      //      osal_socket_strerr(strerr, socket_errno);
      //      p_err("unable to bind to tcp socket: %s\tsocket port:%d\r\n", strerr,sess->data_addr.sin_port);
      osal_socket_close(sess->data_cfd);
      ftp_reply(sess->ctrl_sfd, FTP_BADSENDCONN, "unable to bind to tcp.");
      return;
    }
    if (osal_socket_listen(sess->data_cfd, 1) < 0)
    {
      //      osal_socket_strerr(strerr, socket_errno);
      //      p_err("error when attempting to listen on tcp socket: %s\r\n", strerr);
      ftp_reply(sess->ctrl_sfd, FTP_BADSENDCONN, "attempting to listen on tcp socket err.");
      osal_socket_close(sess->data_cfd);
      return;
    }
    sess->datachan_cmd = DCMD_PASV;
    sess->datachan_done_cmd = 0;
    wake_up(&sess->datachan_wait); // for accept data_cfd
    wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);

    getsockname(sess->ctrl_sfd, (struct sockaddr *)&saddr, &namelen);
    addr = saddr.sin_addr.s_addr;    // addr需要來自ctrl_sfd；data_cfd的地址是INADDR_ANY，不能用
    port = sess->data_addr.sin_port; // port在這裏是已知的
    sprintf(resp,
            "Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
            (addr >> 0) & 0xFF,
            (addr >> 8) & 0xFF,
            (addr >> 16) & 0xFF,
            (addr >> 24) & 0xFF,
            (port >> 0) & 0xFF,
            (port >> 8) & 0xFF);

    ftp_reply(sess->ctrl_sfd, FTP_PASVOK, resp);
    p_info("resp:%s", resp);

    sess->file_offest = 0;
  }
  else
  {
    // ftp_reply(sess->ctrl_sfd, FTP_BADPROT, "Two PASV");
    // p_err("Two PASV\r\n" );
    getsockname(sess->ctrl_sfd, (struct sockaddr *)&saddr, &namelen);
    addr = saddr.sin_addr.s_addr;    // addr需要來自ctrl_sfd；data_cfd的地址是INADDR_ANY，不能用
    port = sess->data_addr.sin_port; // port在這裏是已知的
    sprintf(resp,
            "Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
            (addr >> 0) & 0xFF,
            (addr >> 8) & 0xFF,
            (addr >> 16) & 0xFF,
            (addr >> 24) & 0xFF,
            (port >> 0) & 0xFF,
            (port >> 8) & 0xFF);

    ftp_reply(sess->ctrl_sfd, FTP_PASVOK, resp);
    p_err("PASV already creat\r\n");
    // printf("resp:%s\r\n",resp);
  }
}

/*********************************** Thread ***********************************/
static ftpcmd_t ctrl_cmds[] = {
    {"QUIT", do_quit},
    {"USER", do_user},
    {"PASS", do_pass},
    {"SYST", do_syst},
    {"PWD", do_pwd},
    {"CWD", do_cwd},
    {"XCWD", do_cwd},
    {"TYPE", do_type},
    {"PASV", do_pasv},
    {"LIST", do_list},
    {"RETR", do_retr},
    {"REST", do_rest},
    {"FEAT", do_feat},
    {"ABOR", do_abor},
    {"MKD", do_mkd},
    {"RMD", do_rmd},
    {"DELE", do_dele},
    {"STOR", do_stor},
    {"noop", do_noop},
    {"RNFR", do_rnfr},
    {"RNTO", do_rnto},
    {NULL, NULL}};

static void ftp_thread(int fd)
{
  session_t *sess;

  p_info("Welcome");
  welcome_cline(fd);

  sess = (session_t *)osal_malloc(sizeof(session_t));
  if (sess == NULL)
  {
    p_err("malloc error");
    osal_socket_close(fd);
    osal_thread_delete(osal_thread_myself());
    return;
  }
  sess->ctrl_sfd = fd;
  sess->is_ascii = 0;

  sess->data_dfd = INVALID_SOCKET;
  sess->data_cfd = INVALID_SOCKET;
  sess->datachan_cmd = DCMD_NONE;
  sess->datachan_done_cmd = 0;
  sess->existname[0] = 0;
  sess->path[0] = 0;
  init_waitqueue_head(&sess->datachan_wait);
  init_waitqueue_head(&sess->datachan_done);
  osal_thread_create(datachan_thread, (void *)sess, 15, 0, 4096, "ftpdata");

  //   avi_recstop();
  while (1)
  {
    int i;
    int iRecvLen = osal_socket_recv(fd, sess->cmd, 1023, 0);
    if (iRecvLen <= 0)
    { // cannot recv data or cline exit
      p_err("cline exit");
      break;
    }
    sess->cmd[iRecvLen] = 0;

    // printf("recv success\n");

    // handle commands
    for (i = 0; ctrl_cmds[i].cmd; ++i)
    {
      const char *cmd = ctrl_cmds[i].cmd;
      char *req = sess->cmd;
      if (cmd[0] == req[0] && cmd[1] == req[1] && cmd[2] == req[2] &&
          (cmd[3] == req[3] || req[3] == '\r' || req[3] == '\n' || req[3] == ' '))
      {
        if (ctrl_cmds[i].cmd_handler)
        {
          //p_info("req %s", req);
          (*ctrl_cmds[i].cmd_handler)(sess);
        }
        else
        {
          ftp_reply(sess->ctrl_sfd, FTP_COMMANDNOTIMPL, "Unimplement command.");
          p_info("Unimplement %s", req);
        }
        break;
      }
    }

    if (!ctrl_cmds[i].cmd)
    {
      ftp_reply(sess->ctrl_sfd, FTP_BADCMD, "Unknown command.");
      p_info("Unknown %s", sess->cmd);
    }
  }

  // 等待datachan結束
  sess->datachan_cmd = DCMD_QUIT;
  sess->datachan_done_cmd = 0;
  wake_up(&sess->datachan_wait);
  wait_event_interruptible(&sess->datachan_done, sess->datachan_done_cmd);

  osal_socket_close(fd);
  osal_free(sess);
  // thread exit
  //    avi_restart();
  printf("dele ftp_thread success!!!\r\n");
  printf("thread [%s] stack size:%d,use:%d\n",pcTaskGetName(NULL),uxTaskGetStackSize(NULL),uxTaskGetStackSize(NULL)-uxTaskGetStackHighWaterMark(NULL));
  osal_thread_delete(osal_thread_myself());
  return;
}

/********************* Initial ********************/
static void do_accept(struct event_info *ei, void *d)
{
  SOCK_HDL fd;
  unsigned int i;
  struct sockaddr_in addr;
  ERR_STR strerr;
  struct conn *c;

  i = sizeof(addr);
  if ((fd = osal_socket_accept((int)d, (struct sockaddr *)&addr, &i)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_warn("error accepting TCP connection: %s", strerr);
    return;
  }
  p_verb("accepted connection from %s:%d\r\n",
         inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

  if (1)
  {
    unsigned char mac[8];
    osal_socket_getmac(fd, mac);
    p_info("MAC %02x-%02x-%02x-%02x-%02x-%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }

  //  if(osal_socket_block(fd, 0) < 0) {
  //    osal_socket_strerr(strerr, socket_errno);
  //    p_info("error setting O_NONBLOCK on socket: %s", strerr);
  //  }

  if (osal_set_tcp_nodelay(fd, 1) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_info("error setting TCP_NODELAY on socket: %s", strerr);
  }
  //if(videoIsRecording())
	recordUnintMsg();
  osal_thread_create(ftp_thread, fd, 15, 0, 4096, "ftp");
}

static int ftp_listen(int port)
{
  struct sockaddr_in addr;
  SOCK_HDL fd;
  ERR_STR strerr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(port);

  if ((fd = osal_socket_open(SOCK_STREAM)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("error creating listen socket: %s\r\n", strerr);
    return -1;
  }

  if (osal_set_reuseaddr(fd, 1) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_warn("ignoring error on setsockopt: %s\r\n", strerr);
  }
  if (osal_socket_bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("unable to bind to tcp socket: %s\r\n", strerr);
    osal_socket_close(fd);
    return -1;
  }
  if (osal_socket_listen(fd, 5) < 0)
  {
    //    osal_socket_strerr(strerr, socket_errno);
    //    p_err("error when attempting to listen on tcp socket: %s\r\n", strerr);
    osal_socket_close(fd);
    return -1;
  }

  eloop_add_fd(fd, 0, 0, do_accept, (void *)fd);

  p_info("listening on ftp port %s:%d\r\n", inet_ntoa(addr.sin_addr), port);

  return 0;
}

/********************* GLOBAL CONFIGURATION DIRECTIVES ********************/
int config_ftp(int port)
{
  if (port <= 0 || port > 65535)
  {
    p_err("invalid listen port %d", port);
    return -1;
  }

  return ftp_listen(port);
}

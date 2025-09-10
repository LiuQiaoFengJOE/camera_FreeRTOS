#include <stdint.h>
#include "config1x.h"

struct boost_t
{
  unsigned long rambase;
  unsigned long entry;
  unsigned char filename[12];
};

// 自解壓引擎的頭部，注意在此處定義的長度
struct unzip_hdr_t
{
  unsigned long dummy[3];
  unsigned long inlen;
  struct boost_t next;
};

struct ziped_hdr_t
{
  unsigned long dummy[3];
  unsigned long entry;
  unsigned long in;
  unsigned long inlen;
  unsigned long out;
  unsigned long outlen;
};

struct ziped_str_t
{
  unsigned long dummy[3];
  char entry[4];
  char in[4];
  char inlen[4];
  char out[4];
  char outlen[4];
};

typedef void (*put_func)(int ch);

void dumphex(put_func put, uint32_t val)
{
  val &= 0xF;
  if (val == 0x0)
    put('0');
  if (val == 0x1)
    put('1');
  if (val == 0x2)
    put('2');
  if (val == 0x3)
    put('3');
  if (val == 0x4)
    put('4');
  if (val == 0x5)
    put('5');
  if (val == 0x6)
    put('6');
  if (val == 0x7)
    put('7');
  if (val == 0x8)
    put('8');
  if (val == 0x9)
    put('9');
  if (val == 0xA)
    put('A');
  if (val == 0xB)
    put('B');
  if (val == 0xC)
    put('C');
  if (val == 0xD)
    put('D');
  if (val == 0xE)
    put('E');
  if (val == 0xF)
    put('F');
}

void dumpword(put_func put, uint32_t val)
{
  put(' ');
  dumphex(put, val >> 28);
  dumphex(put, val >> 24);
  dumphex(put, val >> 20);
  dumphex(put, val >> 16);
  dumphex(put, val >> 12);
  dumphex(put, val >> 8);
  dumphex(put, val >> 4);
  dumphex(put, val);
}

void dumpstr(put_func put, uint8_t *str)
{
  while (*str)
    put(*str++);
}

void dump_newline(put_func put)
{
  put('\r');
  put('\n');
}

void dump_param(put_func put, uint32_t val, char *str)
{
  put(' ');
  dumpstr(put, str); // see start.S
  dumpword(put, val);
}

void dump_params(put_func put, struct ziped_hdr_t *ziped, struct ziped_str_t *str)
{
  dump_newline(put);
  dump_param(put, ziped->entry, str->entry);

  dump_newline(put);
  dump_param(put, ziped->in, str->in);
  dump_param(put, ziped->inlen, str->inlen);
  dump_newline(put);
  dump_param(put, ziped->out, str->out);
  dump_param(put, ziped->outlen, str->outlen);

  dump_newline(put);
}

extern struct ziped_str_t ziped_str;
// uint32_t do_unzip(put_func r3, uint32_t r4, uint32_t r5, uint32_t *outlen)
uint32_t do_unzip(struct ziped_hdr_t *ziped, uint32_t base, uint32_t r5, put_func put)
{
  uint32_t ret;
  struct ziped_hdr_t work;
  uint32_t outlen;

  {
    work.entry = ziped->entry;
    work.in = ziped->in;
    work.inlen = ziped->inlen;
    work.out = ziped->out;
    work.outlen = ziped->outlen;
  }

  dumpword(put, work.in);
  dumpword(put, work.inlen);

  wdt_disable();
  icache_init();
  dcache_init();

  /* icache 可以init/unzip/dis/init
   * dcache 可以init/unzip/dis/writeback/enable
   *          或init/unzip/dis/flush/enable
   * 但dcache不可以在enable之前做invalid
   */
  dump_params(put, &work, (struct ziped_str_t *)(((char *)&ziped_str) + base));
  ret = lzo1x_decompress(work.in, work.inlen, work.out, &work.outlen, 0);
  dump_params(put, &work, (struct ziped_str_t *)(((char *)&ziped_str) + base));
  dumpword(put, ret);
  dump_newline(put);

  dcache_disable();
  dcache_flush_all();
  //  dcache_writeback_all ();

  icache_disable();

  return ret ? ~0 : work.entry;
}

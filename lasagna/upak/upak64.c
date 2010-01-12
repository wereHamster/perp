/* upak64.c
** upak, portable storage for unsigned integers
** wcm, 2004.12.29 - 2009.07.19
** ===
*/
#include <stdint.h>

#include "uchar.h"
#include "upak.h"



uchar_t *
upak64_pack(uchar_t s[8], uint64_t u)
{
  s[0] = u & 255;
  u >>= 8;
  s[1] = u & 255;
  u >>= 8;
  s[2] = u & 255;
  u >>= 8;
  s[3] = u & 255;
  u >>= 8;
  s[4] = u & 255;
  u >>= 8;
  s[5] = u & 255;
  u >>= 8;
  s[6] = u & 255;
  s[7] = u >> 8;

  return s;
}


uint64_t
upak64_unpack(const uchar_t s[8])
{
  uint64_t  u = 0L;

  u =   s[7];
  u <<= 8;
  u +=  s[6];
  u <<= 8;
  u +=  s[5];
  u <<= 8;
  u +=  s[4];
  u <<= 8;
  u +=  s[3];
  u <<= 8;
  u +=  s[2];
  u <<= 8;
  u +=  s[1];
  u <<= 8;
  u +=  s[0];

  return u;
}


/* eof (upak64.c) */

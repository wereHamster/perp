/* upak32.c
** upak, portable storage for unsigned integers
** wcm, 2004.12.29 - 2009.07.27
** ===
*/
#include <stdint.h>

#include "uchar.h"
#include "upak.h"


uchar_t *
upak32_pack(uchar_t s[4], uint32_t u)
{
  s[0] = u & 255;
  u >>= 8;
  s[1] = u & 255;
  u >>= 8;
  s[2] = u & 255;
  s[3] = u >> 8;

  return s;
}


uint32_t
upak32_unpack(const uchar_t s[4])
{
  uint32_t  u = 0;

  u =   s[3];
  u <<= 8;
  u +=  s[2];
  u <<= 8;
  u +=  s[1];
  u <<= 8;
  u +=  s[0];

  return u;
}


/* eof (upak32.c) */

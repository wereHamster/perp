/* upak16.c
** upak, portable storage for unsigned integers
** wcm, 2004.12.29 - 2009.07.19
** ===
*/
#include <stdint.h>

#include "uchar.h"
#include "upak.h"


uchar_t *
upak16_pack(uchar_t s[2], uint16_t u)
{
  s[0] = u & 255;
  s[1] = u >> 8;

  return s;
}


uint16_t
upak16_unpack(const uchar_t s[2])
{
  uint16_t  u = 0;

  u = (unsigned char) s[1];
  u <<= 8;
  u += (unsigned char) s[0];

  return u;
}

/* eof (upak16.c) */

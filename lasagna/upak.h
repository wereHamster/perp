/* upak.h
** upak, portable storage for unsigned integers
** wcm, 2004.12.29 - 2009.07.27
** ===
*/
#ifndef UPAK_H
#define UPAK_H 1

#include <stddef.h>
#include <stdint.h>

#include "uchar.h"

/*
**  upak:  portable storage format for unsigned integers
**  (an implementation of djb uint* utilities)
**
**  for n-byte unsigned integer u, find u0, u1, u2, u3, ..., such that
**
**      u = u0 + (u1*2^8) + (u2*2^16) + (u3*2^32) ...
**
**  then store u in buffer buf[0..(n-1)]:
**
**      buf[0] = u0, buf[1] = u1, buf[2] = u2, buf[3] = u3, ...
**
** 
**  example for 32-bit unsigned integer types (4 bytes):
**
**  uint32_t  u;
**  uchar_t   s[4];
**
**  upak32_pack(s, u);    // pack s from u
**  u = upak32_unpack(s); // unpack u from s
*/

/* upakNN_*() common interface:
** 
** upakNN_pack():
**   pack uintNN_t integer u into buffer b
**   return b
**
** upakNN_unpack():
**   return uintNN_t integer unpacked from buffer b
*/

/* upak16_*(), for uint16_t (caller supplies buf[2]): */
extern uchar_t * upak16_pack(uchar_t *buf, uint16_t u);
extern uint16_t upak16_unpack(const uchar_t *buf);

/* upak32_*(), for uint32_t (caller supplies buf[4]: */
extern uchar_t * upak32_pack(uchar_t *buf, uint32_t u);
extern uint32_t upak32_unpack(const uchar_t *buf);

/* upak64_*(), for uint64_t (caller supplies buf[8]: */
extern uchar_t * upak64_pack(uchar_t *buf, uint64_t u);
extern uint64_t upak64_unpack(const uchar_t *buf);


/* upak_*():
**   variable argument utilities:
**   under control of specification string in fmt
**   pack/unpack variable number of arguments
**
**   characters interpreted in fmt:
**     'b'  1 byte  unsigned char
**     's'  2 byte  uint16_t
**     'd'  4 byte  uint32_t
**     'L'  8 byte  uint64_t
**
**   return: number of bytes packed/unpacked from buf
*/

/* upak_pack()
**   pack variable uintNN arguments specified in fmt into buf
**   packed into consecutive position from beginning of buf
**
**   return: number of bytes packed into buf
**
**   note: caller must supply buf of sufficient size!
*/
extern size_t upak_pack(uchar_t *buf, char *fmt, ...);

/* upak_unpack()
**   unpack from buf the variable number of packed integers specified in fmt
**   into the matching number of variable integer arguments
**
**   return: number of bytes unpacked from buf
*/
extern size_t upak_unpack(uchar_t *buf, char *fmt, ...);


#endif /* UPAK_H */
/* eof: upak.h */

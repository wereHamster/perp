/* nfmt_uint32_pad0.c
** stringify uint32_t numerical value
** pad with '0' on left to fill width
** wcm, 2004.04.19 - 2009.08.03
** ===
*/
#include <stddef.h>
#include <stdint.h>

#include "nfmt.h"

/*
** djb classic version:
** stringify decimal n into s of width w
** '0' pad on left to fill width
** s is _not_ nul terminated
** return number of characters in s
**
** customary usage to nul-terminate s:
**   char  s[NFMT_SIZE];
**   s[nfmt_uint32_pad0_(s, n)] = '\0';
*/

size_t
nfmt_uint32_pad0_(char *s, uint32_t n, size_t w)
{
  size_t   len;

  len = nfmt_uint32_(NULL, n);
  while(len < w){
      if(s){ *s = '0'; ++s; }
      ++len;
  }
  if(s){
      nfmt_uint32_(s, n);
  }

  return len;
}
          
char *
nfmt_uint32_pad0(char *s, uint32_t n, size_t w)
{
  s[nfmt_uint32_pad0_(s,n,w)] = '\0';
  return s;
}

/* eof: nfmt_uint32_pad0.c */

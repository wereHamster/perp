/* nfmt.h
** format numeric types
** wcm, 2004.04.19 - 2009.08.03
** ===
*/
#ifndef NFMT_H
#define NFMT_H 1

#include <stddef.h>    /* sizet_t */
#include <stdint.h>

/*
** size of char buffer to hold maximum uint128_t value + nul
** (planning for 16 byte integers ...):
*/
#define NFMT_SIZE  40

/*
** stringify unsigned decimal integer n into s
** s is nul-terminated
** return pointer to s:
*/
extern char *nfmt_uint32(char *s, uint32_t n);
extern char *nfmt_uint64(char *s, uint64_t n);

/*
** stringify unsigned decimal integer n into s for field width w
** pad with '0' on left if/as necessary to fill w
** s is nul-terminated
** return pointer to s
*/
extern char *nfmt_uint32_pad0(char *s, uint32_t n, size_t w);
extern char *nfmt_uint64_pad0(char *s, uint64_t n, size_t w); 


/*
** stringify decimal integer n into s
** s is *not* nul-terminated
** return number of characters in s
**
** djb classic usage is:
**   char  s[NFMT_UINT32];
**   s[nfmt_uint32_(s,n)]='\0';
**
** if s is NULL, return number of characters to convert n
*/
extern size_t nfmt_uint32_(char *s, uint32_t n);
extern size_t nfmt_uint64_(char *s, uint64_t n);


/*
** stringify unsigned decimal integer n into s for field width w
** pad with '0' on left if/as necessary to fill w
** s is *not* nul-terminated
** return number of characters in s
**
** if s is NULL, return number of characters to convert n
*/
extern size_t nfmt_uint32_pad0_(char *s, uint32_t n, size_t w);
extern size_t nfmt_uint64_pad0_(char *s, uint64_t n, size_t w);


#endif /* NFMT_H */
/* EOF (nfmt.h) */

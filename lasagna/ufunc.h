/* ufunc.h
** ufunc, unsigned integer utilities
** wcm, 2010.11.16 - 2010.11.16
** ===
*/
#ifndef UFUNC_H
#define UFUNC_H 1

#include <stdint.h>

/* ufunc_u32add()
**   update u with u += add
**
**   return:
**     0 : no error, updated value loaded into u
**    -1 : overflow, errno set ERANGE
*/
extern int ufunc_u32add(uint32_t *u, uint32_t add);

#endif /* UFUNC_H */
/* eof: ufunc.h */

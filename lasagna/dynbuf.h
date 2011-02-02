/* dynbuf.h
** dynamic (growable) buffer
** wcm, 2004.04.21 - 2010.05.29
** ===
*/
#ifndef DYNBUF_H
#define DYNBUF_H

#include <stddef.h>  /* size_t */

#define DYNBUF_INITSIZE  1

struct dynbuf {
  void    *buf;
  size_t   n;   /* actual size allocated  */
  size_t   p;   /* current position (aka "length", "used") */
}; 
typedef struct dynbuf  dynbuf;

/* constructor for malloc'ed dynbuf: */
extern dynbuf * dynbuf_new(void); 

/* destructor for malloc'ed dynbuf: */
extern void dynbuf_free(dynbuf *);

/* dynbuf_INIT()
** dynbuf static initialization
** usage:
**   dynbuf dyn = dynbuf_INIT();
**
** should be preferred to:  dynbuf d = {0,0,0};
*/
#define dynbuf_INIT() {NULL, 0, 0}

/* release internal buffer only: */
extern void dynbuf_freebuf(dynbuf *);

/* no memory release, just set d->p = 0 */
extern void dynbuf_clear(dynbuf *);
#define dynbuf_CLEAR(d) ((d)->p = 0)

/* accessors: */
extern dynbuf * dynbuf_buf(dynbuf *);
extern size_t dynbuf_len(dynbuf *);
/* accessor macros: */
#define dynbuf_BUF(d) ((d)->buf)
#define dynbuf_LEN(d) ((d)->p)

/* if necessary, grow to accomodate total need */
/* return -1 error, 0 no error */
extern int dynbuf_need(dynbuf *, size_t need);

/* if necessary, grow to accomodate additional add */
/* return -1 error, 0 no error */
extern int dynbuf_grow(dynbuf *, size_t add);

/* dynbuf_put*() functions:
**   "put" means _append_ to dynbuf
**
**   return -1 on error, 0 no error
*/
extern int dynbuf_put(dynbuf *to, const dynbuf *from);
extern int dynbuf_putbuf(dynbuf *, const void *buf, size_t len);

/* dynbuf_puts()
** str is NUL terminated,
** NUL is _not_ copied into dynbuf
*/
extern int dynbuf_puts(dynbuf *, const char *str);

/* dynbuf_putc()
** append single char/byte
*/
#define dynbuf_putc(d,c)  dynbuf_putbuf((d),&(c),1)

/* append '\0' to buffer: */
extern int dynbuf_putnul(dynbuf *);


/* "copy" buf into dynbuf, overwriting previous contents: */
extern int dynbuf_copy(dynbuf *to, const dynbuf *from);
extern int dynbuf_copybuf(dynbuf *, const void *buf, size_t len);

/*
** str is NUL terminated,
** NUL is _not_ copied into dynbuf
*/
extern int dynbuf_copys(dynbuf *, const char *str);



#endif /* DYNBUF_H */

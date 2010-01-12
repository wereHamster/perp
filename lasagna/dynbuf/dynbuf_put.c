/* dynbuf_put.c
** wcm, 2004.04.20 - 2009.09.09
** ===
*/
#include <stddef.h>
#include "dynbuf.h"

#include "buf.h"
#include "cstr.h"


int
dynbuf_putbuf(dynbuf *d, const void *buf, size_t len)
{
    unsigned char *b;

    if(!d->n) return dynbuf_copybuf(d, buf, len);

    if(dynbuf_grow(d, (len + 1)) != 0)
        return -1;  /* error! */

    buf_copy((d->buf + d->p), buf, len);

    d->p += len;
    b = d->buf; b[d->p] = 'Q'; /* "offensive programming" */

    return 0; /* no error */
}


int
dynbuf_put(dynbuf *to, const dynbuf *from)
{
    return dynbuf_putbuf(to, from->buf, from->n);
}


int
dynbuf_puts(dynbuf *d, const char *str)
{
    return dynbuf_putbuf(d, str, cstr_len(str));
}


int
dynbuf_putnul(dynbuf *d)
{
    return dynbuf_putbuf(d, "\0", 1);
}

/* eof (dynbuf_put.c) */

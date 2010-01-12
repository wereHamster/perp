/*
** dynbuf.c
** wcm, 2004.04.20 - 2004.04.20
** ===
*/
#include <stdlib.h>
#include "dynbuf.h"

/* accessors: */

dynbuf *
dynbuf_buf(dynbuf *d)
{
    return d->buf;
}

size_t
dynbuf_len(dynbuf *d)
{
    return d->p;
}

/* that's all, folks! */

/*
** dynbuf_grow.c
** wcm, 2004.04.20 - 2004.04.29
** ===
*/
#include <stddef.h>
#include "dynbuf.h"


int
dynbuf_grow(dynbuf *d, size_t add)
{
    if(d->buf)
        add += d->p;

    return dynbuf_need(d, add);
}

/* that's all, folks! */

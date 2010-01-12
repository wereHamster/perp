/*
** dynbuf_need.c
** wcm, 2004.04.20 - 2004.04.20
** ===
*/
#include <stddef.h>
#include <stdlib.h>

#include "dynbuf.h"

int
dynbuf_need(dynbuf *d, size_t need)
{
    size_t have = d->n;

    if(!d->buf || (have < need)){
        void *buf = NULL;

        need += (need >> 2) + 8;
        if((buf = realloc(d->buf, need)) == NULL)
            return -1;

        /* else: */
        d->buf = buf;
        d->n   = need;
        return 0;  /* no error */
    }

    /* else, ok already: */
    return 0;  /* no error */
}

/* that's all, folks! */

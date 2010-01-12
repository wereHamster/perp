/*
** dynbuf_freebuf.c
** wcm, 2004.05.01 - 2004.05.01
** ===
*/
#include <stdlib.h>
#include "dynbuf.h"

/* release internal buffer only: */

void
dynbuf_freebuf(dynbuf *d)
{
    if(d){
        if(d->buf) {
            free(d->buf);
            d->buf = NULL;
        }
        d->n = 0;
        d->p = 0;
    }
    return;
}

/* that's all, folks! */

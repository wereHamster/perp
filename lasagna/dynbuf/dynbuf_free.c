/*
** dynbuf_free.c
** wcm, 2004.04.20 - 2004.04.20
** ===
*/
#include <stdlib.h>
#include "dynbuf.h"


void
dynbuf_free(dynbuf *d)
{
    if(d){
        if(d->buf) free(d->buf);
        free(d);
    }
    return;
}

/* that's all, folks! */

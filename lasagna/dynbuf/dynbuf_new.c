/*
** dynbuf_new.c
** wcm, 2004.04.20 - 2004.04.20
** ===
*/
#include <stdlib.h>
#include "dynbuf.h"


dynbuf *
dynbuf_new()
{
    dynbuf *d = (dynbuf *)malloc(sizeof(dynbuf));

    if(d){
        d->buf = (void *)malloc(DYNBUF_INITSIZE);
        if(d->buf != NULL){
            d->n = DYNBUF_INITSIZE;
            d->p = 0;
        }
        else{
            free(d);
            d = NULL;
        }
    }
    return d;
} 

/* that's all, folks! */

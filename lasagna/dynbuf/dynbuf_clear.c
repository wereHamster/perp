/*
** dynbuf_clear.c
** wcm, 2004.04.20 - 2004.04.20
** ===
*/
#include "dynbuf.h"

void
dynbuf_clear(dynbuf *d)
{
    d->p = 0;
    return;
}

/* that's all, folks! */

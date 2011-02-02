/* hdb_dynget.c
** wcm, 2010.05.29 - 2010.12.14
** ===
*/

/* lasagna: */
#include "dynbuf.h"
#include "uchar.h"

/* libhdb: */
#include "hdb.h"


/* hdb_dynget()
**   assumed to follow successful hdb_find()
**
**   note:
**     operation on dynbuf is "append"
*/
int
hdb_dynget(struct hdb *H, dynbuf *B)
{
    return hdb_dynread(H, B, hdb_dlen(H), hdb_dpos(H));
}

/* eof (hdb_dynget.c) */

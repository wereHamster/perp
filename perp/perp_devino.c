/* perp_devino.c
** wcm, 2009.11.12 - 2009.11.13
** ===
*/

#include <stddef.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tx64.h"
#include "uchar.h"
#include "upak.h"

#include "perplib.h"

/* base64 encoding vector for perp_devino: */
static
const char     tx64vec[] = "0123456789"
    "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "}{" "~";

/* perp_devino():
**
** obtain unique filename for shadow directory of a perpetrate service
** based on the stat dev/ino of the service definition directory
**
** cast dev and ino of service definition directory into 8-byte integers
** load these into a 16-byte buffer
** base64 encode the 16-byte buffer into a 22-byte string
** prefix with leading underscore and append terminating nul
** (24 bytes total, including terminating nul)
**
** the dev and ino of a service definition persists if service is simply renamed
** ie, its control directory is still functional and identifiable to clients
**
** return pointer to dest
*/

char          *perp_devino(char *dest, struct stat *st)
{
   uchar_t        binbuf[16];   /* packing for two 8-byte integers */

   upak_pack(binbuf, "LL", (uint64_t) st->st_dev, (uint64_t) st->st_ino);
   tx64_encode(&dest[1], (const char *) binbuf, sizeof binbuf, tx64vec,
               TX64_NOPAD);
   /* add leading underscore and terminating nul: */
   dest[0] = '_';
   dest[23] = '\0';

   return dest;
}

/* eof: perp_devino.c */

/* perp_ctlpath.c
** wcm, 2009.11.16 - 2009.11.16
** ===
*/
#include <stddef.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cstr.h"

#include  "perp_common.h"
#include  "perplib.h"

const char    *perp_ctlpath(struct stat *st)
{
   static char    ctlpath[] = "./" PERP_CONTROL "/" SV_DEVINO;
   size_t         n;

   n = cstr_len("./" PERP_CONTROL "/");
   perp_devino(&ctlpath[n], st);

   return (const char *) ctlpath;
}

/* eof: perp_ctlpath.c */

/* perp_ready.c
** wcm, 2009.11.13 - 2009.11.16
** ===
*/
#include <unistd.h>
#include <fcntl.h>

#include "cstr.h"

#include "perp_common.h"
#include "perplib.h"

/* perp_ready()
**   checks "readiness" of a perpetrate supervisor
**
**   test:
**     CTL_IN fifo may be opened for writing
**     fails if an active process does not have fifo open for read
**
**   * must be called from within the base directory PERP_BASE
**   * requires adequate user privilege (usually root)
*/
int perp_ready(const char *ctlpath)
{
   char           path[] = "./" PERP_CONTROL "/" SV_DEVINO "/" CTL_IN;
   int            fd;

   cstr_vcopy(path, ctlpath, "/" CTL_IN);
   if ((fd = open(path, (O_WRONLY | O_NONBLOCK))) == -1) {
      /* leave errno for caller to inspect, return error: */
      return -1;
   }

   close(fd);
   return 0;
}

/* eof: perp_ready.c */

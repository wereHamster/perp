/* perp_binstat.c
** wcm, 2009.11.12 - 2009.11.16
** ===
*/
#include <stddef.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "cstr.h"
#include "uchar.h"

#include "perp_common.h"
#include "perplib.h"


/* perp_binstat()
**   reads binary-encoded status from service directory ctlpath
**
**   params:
**     ctlpath: path to service control directory relative to PERP_BASE
**
**   return:
**     pointer to internal static binary-encoded status buffer
**     on failure: NULL, errno set
*/

const uchar_t *perp_binstat(const char *ctlpath)
{
   static uchar_t buf[BINSTAT_SIZE];
   char           path[] = "./" PERP_CONTROL "/" SV_DEVINO "/" STATUS_BIN;
   int            fd;
   ssize_t        r;
   int            terrno;

   cstr_vcopy(path, ctlpath, "/" STATUS_BIN);

   if ((fd = open(path, O_RDONLY)) == -1) {
      return NULL;
   }

   do {
      r = read(fd, buf, sizeof buf);
   } while ((r == -1) && (errno == EINTR));

   terrno = errno;
   close(fd);
   errno = terrno;

   if (r == -1)
      return NULL;

   if (r < sizeof buf) {
      errno = EPROTO;
      return NULL;
   }

   return (const uchar_t *) buf;
}

/* eof: perp_binstat.c */

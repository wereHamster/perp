/* perp_statread.c
** wcm, 2009.11.12 - 2009.11.13
** ===
*/

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "uchar.h"

#include "perp_common.h"
#include "perplib.h"

/* perp_statread()
**   read binary-encoded status from fd into static buffer
**   close(fd)
**
**   return pointer to static binary-encoded status buffer
**   on failure, return NULL and set errno
*/

const uchar_t *perp_statread(int fd)
{
   static uchar_t buf[BINSTAT_SIZE];
   ssize_t        r;
   int            terrno;

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

/* eof: perp_statread.c */

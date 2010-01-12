/* perp_common.h
** common defines for perp apps
** wcm, 2008.01.23 - 2009.12.09
** ===
*/
#ifndef PERP_COMMON_H
#define PERP_COMMON_H 1

/* release version string: */
#ifndef PERP_VERSION
#define PERP_VERSION "0.00"
#endif

/* default PERP_BASE directory: */
#ifndef PERP_BASE_DEFAULT
#define PERP_BASE_DEFAULT  "/etc/perp"
#endif

/* maximum number of perpetrate services per perpd instance: */
#ifndef PERP_MAX
#define PERP_MAX 400
#endif


/*
** other path/file names:
*/

/* base service control directory
**   (relative to PERP_BASE; usually configured as a symlink):
*/
#define PERP_CONTROL  ".control"

/* perpboot directory
**   (relative to PERP_BASE):
*/
#define PERP_BOOT ".boot"

/* perpd control directory
**   (relative to PERP_CONTROL):
*/
#define PERPD_CONTROL ".perpd"

/* device/inode encoded identifier for a service control directory
**   a leading underscore, followed by 22-character devino identifier
**   devino string buffer is 24 bytes total inclusive of terminal nul
**
**   note:
**     #define used as space-holder for fixed-length devino buffers
**
**   (relative to PERP_CONTROL):
*/
#define SV_DEVINO  "_nnnnnnnnnnnnnnnnnnnnnn"

/* pidlock filename
** (relative to both PERP_CONTROL/SV_DEVINO/ and PERPD_CONTROL/):
*/
#define PIDLOCK  "pid.lock"

/* name of fifo for input
**   note: also used to by clients to test supervisor ready
**   (relative to PERP_CONTROL/SV_DEVINO/):
*/
#define CTL_IN  "ctl.0"

/* name of fifo for output
**   (relative to PERP_CONTROL/SV_DEVINO/):
*/
#define CTL_OUT "ctl.1"

/* name of lock file for perpetrate control clients:
**    (relative to PERP_CONTROL/SV_DEVINO/):
*/
#define CTL_LOCK "ctl.lock"

/* filenames for binary-encoded status
**   (relative to PERP_CONTROL/SV_DEVINO/):
*/
#define STATUS_BIN  "status.bin"
#define STATUS_TMP  "status.tmp"


/* maximum packet size in control protocol
** (255 + 3, and then some)
*/
#define PACKET_SIZE  260


/* default attributes for colorized perpls: */
#ifndef PERPLS_COLORS_DEFAULT
#define PERPLS_COLORS_DEFAULT  \
  "df=00:na=00:an=01:ar=01;33:ap=01;33:ad=01;34:wu=01;33:wd=01;33:er=01;31"
#endif


/*
** other common macros:
*/
#include <unistd.h>
#define  die(e) \
    _exit((e))


/*
** common stderr macros:
**
** source prerequisites:
**   #define EPUT_DEVOUT  //(or EPUTS_OUTVEC or EPUTS_IOQ)
**   #include "eputs.h"
**
*/
#ifdef  EPUTS_DEFINED
#define  usage() \
    eputs("usage: ", progname, prog_usage)

#define  version() \
    eputs(progname, ": version: ", PERP_VERSION)

#define  die_usage() \
    { usage(); die(100); }

/* exclude the following definitions if using loggit.h: */
#ifndef LOGGIT_DEFINED
#include "sysstr.h"
#define  fatal(e, ...) \
    { eputs(progname, ": fatal: ", __VA_ARGS__); die((e)); }

#define  fatal_syserr(...) \
    fatal(111, __VA_ARGS__, ": ", \
          sysstr_errno_mesg(errno), " (", \
          sysstr_errno(errno), ")" )

#define  fatal_usage(...) \
    { eputs(progname, ": usage error: ", __VA_ARGS__); die_usage(); }
#endif /* LOGGIT_DEFINED */
#endif /* EPUTS_DEFINED */


#endif /* PERP_COMMON_H */
/* eof (perp_common.h) */

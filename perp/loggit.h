/* loggit.h
** macros for stderr logging
** intended for "daemons": perpd, perpetrate, tinylog
** wcm, 2009.12.09 - 2009.12.09
** ===
*/
#ifndef LOGGIT_H
#define LOGGIT_H 1

/* includes for messaging on errno: */
#include <errno.h>
#include "sysstr.h"


/* XXX, not idempotent:
**   perp project headers need to be ordered:
**
**   #include "eputs.h"       //optional: default eputs() defined below
**   #include "loggit.h"      //this file, for logging daemon progs  
**   #include "perp_common.h" //required
*/

/* loggit macro usage requirements:
**   definition for eputs()
**   definition for LOGGIT_FMT
** 
**   for example:
** 
**   #define eputs(...) \
**     outvec_vputs(&OUTVEC_STDERR, __VA_ARGS__, "\n")
** 
**   #define LOGGIT_FMT \
**     progname, "[", progpid, "]: "
*/


/*
** a default eputs():
*/
#ifndef EPUTS_DEFINED
#  ifdef eputs
#    undef eputs
#  endif
#  include "outvec.h"
#  define eputs(...) \
     outvec_vputs(&OUTVEC_STDERR, __VA_ARGS__, "\n")
#  define EPUTS_DEFINED 1
#endif


/*
** logging macros:
*/
#define loggit(...) \
  eputs(LOGGIT_FMT, __VA_ARGS__)

/* logging to stderr, prefix with priority "pri": */
#define loggit_pri(pri, ...) \
  eputs((pri), ": ", LOGGIT_FMT, __VA_ARGS__)

#define log_alert(...) \
  loggit_pri("alert", __VA_ARGS__)

#define log_error(...) \
  loggit_pri("error", __VA_ARGS__)

#define log_warning(...) \
  loggit_pri("warning", __VA_ARGS__)

#define log_info(...) \
  loggit(__VA_ARGS__)

#ifndef NDEBUG
#  define log_debug(...) \
       loggit_pri("debug", __VA_ARGS__)
#  define log_trace(...) \
       loggit_pri("debug", "trace: ", __func__, "(): ", __VA_ARGS__)
#else
#  define log_trace(...)
#  define log_debug(...)
#endif /* NDEBUG */

/* define guard for perp_common.h macros (yes, LOGGIT_H redundant): */
#define LOGGIT_DEFINED 1

#define fatal_usage(...) \
  { log_error("usage error: ", __VA_ARGS__); die_usage(); }

#define fatal(...) \
  { log_error("fatal: ", __VA_ARGS__); die(111); }

#define fatal_syserr(...) \
  {\
      log_error("fatal: ", __VA_ARGS__, ": ", \
                sysstr_errno_mesg(errno), " (", \
                sysstr_errno(errno), ")"); \
      die(111);\
  }

#define warn_syserr(...) \
  log_warning(__VA_ARGS__, ": ", \
               sysstr_errno_mesg(errno), " (", \
               sysstr_errno(errno), ")")

/* other common stderr macros are provided by perp_common.h (or tinylog.h):
**
**   usage()
**   version()
**   die_usage()
*/

#endif /* LOGGIT_H */
/* eof: loggit.h */

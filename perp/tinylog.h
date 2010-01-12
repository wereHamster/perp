/* tinylog.h
** tinylog configuration header
** wcm, 2008.10.09 - 2009.12.09
** ===
*/
#ifndef TINYLOG_H
#define TINYLOG_H 1

#include <errno.h>

#include "ioq.h"
#include "ioq_std.h"
#include "sysstr.h"


#ifndef TINYLOG_VERSION
#define TINYLOG_VERSION  "0.1.0"
#endif

/* what to name single concurrency lock file: */
#ifndef PIDLOCK
#define PIDLOCK "pid.lock"
#endif

/* where to find gzip executable:
** (may also be set with TINYLOG_ZIP in the environment)
*/
#ifndef TINYLOG_ZIP
#define TINYLOG_ZIP  "/usr/bin/gzip"
#endif

/* extension for zipped files: */
#ifndef ZIP_EXT
#define ZIP_EXT  ".Z"
#endif


/* macros: */
#define die(e) \
  _exit((e))

#define usage() \
  {\
    eputs("usage: ", progname, prog_usage); \
  }

#define version() \
  {\
    eputs(progname, ": version: ", TINYLOG_VERSION); \
  }

#define die_usage() \
  { usage(); die(100); }


#endif /* TINYLOG_H */
/* eof: tinylog.h */

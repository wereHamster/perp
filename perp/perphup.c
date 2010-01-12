/* perphup.c
** perphup: trigger rescan in perpd
** wcm, 2009.11.11 - 2009.12.06
** ===
*/

/* standard library: */
#include <stdlib.h>

/* unix: */
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/* lasagna: */
#include "nextopt.h"
#include "pidlock.h"
#include "sysstr.h"

/* perp: */
#define  EPUTS_DEVOUT  1
#include "eputs.h"
#include "perp_common.h"

static const char *progname = NULL;
static const char prog_usage[] = " [-hV] [-q] [basedir]";


#define  report_ok(...) \
  {\
      if(verbose){\
          eputs(progname, ": ",  __VA_ARGS__);\
      }\
  }


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVq");
   int            verbose = 1;
   const char    *basedir = NULL;
   pid_t          lockpid;

   progname = nextopt_progname(&nopt);
   while ((opt = nextopt(&nopt))) {
      char           optc[2] = { nopt.opt_got, '\0' };
      switch (opt) {
      case 'h':
         usage();
         die(0);
         break;
      case 'V':
         version();
         die(0);
         break;
      case 'q':
         verbose = 0;
         break;
      case ':':
         fatal_usage("missing argument for option -", optc);
         break;
      case '?':
         if (nopt.opt_got != '?') {
            fatal_usage("invalid option -", optc);
         }
         /* else fallthrough: */
      default:
         die_usage();
         break;
      }
   }

   argc -= nopt.arg_ndx;
   argv += nopt.arg_ndx;

   basedir = *argv;

   if (!basedir)
      basedir = getenv("PERP_BASE");
   if (!basedir || (basedir[0] == '\0'))
      basedir = PERP_BASE_DEFAULT;

   if (chdir(basedir) != 0) {
      fatal_syserr("failure chdir() to ", basedir);
   }

   if (chdir(PERP_CONTROL) == -1) {
      fatal_syserr("failure chdir() to ", basedir, "/", PERP_CONTROL);
   }

   if (chdir(PERPD_CONTROL) == -1) {
      fatal_syserr("failure chdir() to ", basedir, "/", PERP_CONTROL, "/",
                   PERPD_CONTROL);
   }

   lockpid = pidlock_check(PIDLOCK);
   if (lockpid == -1) {
      fatal_syserr("failure checking lock on ", PIDLOCK);
   }
   if (lockpid == 0) {
      fatal(111, "perpd not running on ", basedir, ": no lock found on ",
            PIDLOCK);
   }

   if (kill(lockpid, SIGHUP) == -1) {
      fatal_syserr("failure kill() on SIGHUP to perpd");
   }

   if (verbose) {
      report_ok("rescan triggered on ", basedir);
   }

   return 0;
}


/* eof (perphup.c) */

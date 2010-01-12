/* perpok.c
** perpok: check perpetrate supervisor
** wcm, 2009.11.09 - 2009.12.06
** ===
*/

/* standard library: */
#include <stdlib.h>

/* unix: */
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* lasagna: */
#include "cstr.h"
#include "nextopt.h"
#include "nscan.h"
#include "sysstr.h"
#include "tain.h"
#include "uchar.h"

/* perp: */
#define  EPUTS_DEVOUT
#include "eputs.h"
#include "perp_common.h"
#include "perplib.h"

static const char *progname = NULL;
static const char prog_usage[] = " [-hV] [-b basedir] [-u secs] [-v] sv";


#define  report_fail(...) \
  {\
      if(verbose){\
          eputs(progname, ": ", svdir, ": fail: ", __VA_ARGS__);\
      }\
      die(1);\
  }

#define  report_ok(...) \
  {\
      if(verbose){\
          eputs(progname, ": ", svdir, ": ok: ",  __VA_ARGS__);\
      }\
      die(0);\
  }


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVb:u:v");
   int            verbose = 0;
   uint32_t       arg_upsecs = 0;
   const char    *nscan_end;
   const char    *basedir = NULL;
   const char    *svdir = NULL;
   struct stat    st;
   const char    *ctlpath;
   const uchar_t *binstat;
   perpstat_t     perpstat;
   uchar_t        flags;
   tain_t         now;

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
      case 'b':
         basedir = nopt.opt_arg;
         break;
      case 'u':
         arg_upsecs = nscan_uint32(nopt.opt_arg, &nscan_end);
         if (*nscan_end != '\0') {
            fatal_usage("numeric argument required for option -", optc);
         }
         break;
      case 'v':
         ++verbose;
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

   svdir = argv[0];
   if (svdir == NULL) {
      fatal_usage("missing argument");
   }

   if (!basedir)
      basedir = getenv("PERP_BASE");
   if (!basedir)
      basedir = ".";

   if (chdir(basedir) != 0) {
      fatal_syserr("fail chdir() to ", basedir);
   }

   if (stat(svdir, &st) == -1) {
      fatal_syserr("fail stat() on service directory ", svdir);
   }

   if (!S_ISDIR(st.st_mode)) {
      fatal_usage("argument not a directory: ", svdir);
   }

   if (!(S_ISVTX & st.st_mode)) {
      report_fail("service directory not activated");
   }

   /* get path to service control directory (relative to cwd): */
   ctlpath = perp_ctlpath(&st);

   if (perp_ready(ctlpath) == -1) {
      report_fail("supervisor not running\n");
   }

   if (arg_upsecs == 0) {
      /* all done: */
      report_ok("supervisor running ok");
   }

   /*
    ** else: extended testing...
    */

   /* read binary-encoded status: */
   binstat = perp_binstat(ctlpath);
   if (binstat == NULL) {
      if (errno == EPROTO) {
         report_fail("bad status format found in ", STATUS_BIN);
      } else {
         report_fail("error reading status in ", STATUS_BIN, ": ",
                     sysstr_errno(errno));
      }
   }

   /* decode status: */
   perp_statload(&perpstat, binstat);

   if (perpstat.subsv[SUBSV_MAIN].pid == 0) {
      report_fail("service not running (pid is 0)");
   }

   flags = perpstat.subsv[SUBSV_MAIN].flags;
   if (subsv_isreset(flags)) {
      report_fail("service resetting");
   }
   if (subsv_iswant(flags)) {
      report_fail("service wants down");
   }

   /* test uptime compared to now: */
   tain_now(&now);
   if ((perp_uptime(&now, &perpstat.subsv[SUBSV_MAIN].when)) < arg_upsecs) {
      report_fail("service uptime not met");
   }

   /* else okiedokeyshmokey: */
   report_ok("service uptime ok");

   /* not reached: */
   return 0;
}


/* eof: perpok.c */

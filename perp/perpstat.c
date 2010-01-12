/* perpstat.c
** perpstat: report current service status from persistent process supervisor
** wcm, 2008.01.23 - 2010.01.05
** ===
*/

/* standard library: */
#include <stdlib.h>
#include <stdio.h>

/* unix: */
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* lasagna: */
#include "nextopt.h"
#include "sysstr.h"
#include "tain.h"
#include "uchar.h"

/* perp: */
#include "perp_common.h"
#include "perplib.h"


static const char *progname = NULL;
static const char prog_usage[] = " [-hV] [-b basedir] sv [sv ...]";


/*
** stderr macros (using stdio/printf):
*/

#define  usage() \
    fprintf(stderr, "usage: %s%s\n", progname, prog_usage)

#define  version() \
    fprintf(stderr, "%s: version: %s\n", progname, PERP_VERSION)

#define die_usage() \
    { usage(); die(100); }

#define  fatal(...) \
    {\
      fflush(stdout);\
      fprintf(stderr, "%s: fatal: ", progname);\
      fprintf(stderr, __VA_ARGS__);\
      die(111);\
    }

#define  fatal_usage(...) \
    {\
      fflush(stdout);\
      fprintf(stderr, "%s: usage error: ", progname);\
      fprintf(stderr, __VA_ARGS__);\
      die_usage();\
    }

#define  fatal_syserr(...) \
    {\
      fflush(stdout);\
      fprintf(stderr, "%s: fatal: ", progname);\
      fprintf(stderr, __VA_ARGS__);\
      fprintf(stderr, ": %s (%s)\n", sysstr_errno_mesg(errno), sysstr_errno(errno));\
      die(111);\
    }

/*
** prototypes in scope:
*/
static void    report(char *report, perpstat_t * perpstat, tain_t * now);


/* TODO:
** break uptime report units at:
** 
**   seconds    0  - 900   [15 minutes]     900 seconds
**   minutes   15  - 480   [8 hours]     28,800 seconds
**   hours      8  -  96   [4 days]     345,100 seconds
**   days       4  -   n
**
*/

static
void report(char *service, perpstat_t * P, tain_t * now)
{
   pid_t          pid;
   uint32_t       up;
   unsigned char  flags;
   int            haslog = 0;

   /* about supervisor: */
   up = perp_uptime(now, &P->super.when);
   flags = P->super.flags;
   printf("\n  super: up %d seconds [pid %d]", up, P->super.pid);

   haslog = super_haslog(flags);

   /* XXX, TODO:
    ** other supervisor flags eg, flagexit)
    */

   /* about main: */
   pid = P->subsv[SUBSV_MAIN].pid;
   up = perp_uptime(now, &P->subsv[SUBSV_MAIN].when);
   flags = P->subsv[SUBSV_MAIN].flags;
   printf("\n   main:");
   if (pid == 0) {
      printf(" down %d seconds", up);
      if (subsv_iswant(flags))
         printf(", want up!");
      if (subsv_isonce(flags))
         printf(", flagged once");
   } else if ((up < 1) && !(subsv_isreset(flags))) {
      /* munge uptime < 1 sec to wantup: */
      printf(" down, want up!");
      if (subsv_isonce(flags))
         printf(", flagged once");
   } else {
      printf(" %s %d seconds [pid %d]",
             (subsv_isreset(flags) ? "resetting" : "up"), up, pid);
      if (subsv_iswant(flags))
         printf(", want down!");
      if (subsv_ispause(flags))
         printf(", paused");
      if (subsv_isonce(flags))
         printf(", flagged once");
   }

   /* about log: */
   printf("\n    log:");
   if (!(haslog)) {
      printf(" no log\n");
      return;
   }

   pid = P->subsv[SUBSV_LOG].pid;
   up = perp_uptime(now, &P->subsv[SUBSV_LOG].when);
   flags = P->subsv[SUBSV_LOG].flags;
   if (pid == 0) {
      printf(" down %d seconds", up);
      if (subsv_iswant(flags))
         printf(", want up!");
      if (subsv_isonce(flags))
         printf(", flagged once");
   } else if ((up < 1) && !(subsv_isreset(flags))) {
      /* munge uptime < 1 sec to wantup: */
      printf(" down, want up!");
      if (subsv_isonce(flags))
         printf(", flagged once");
   } else {
      printf(" %s %d seconds [pid %d]",
             (subsv_isreset(flags) ? "resetting" : "up"), up, pid);
      if (subsv_iswant(flags))
         printf(", want down!");
      if (subsv_ispause(flags))
         printf(", paused");
      if (subsv_isonce(flags))
         printf(", flagged once");
   }
   printf("\n");

   return;
}


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVb:");
   const char    *basedir = NULL;
   tain_t         now;

   progname = nextopt_progname(&nopt);
   while ((opt = nextopt(&nopt))) {
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
      case ':':
         fatal_usage("missing argument for option -%c\n", nopt.opt_got);
         break;
      case '?':
         if (nopt.opt_got != '?') {
            fatal_usage("invalid option -%c\n", nopt.opt_got);
         }
         /* else fallthrough: */
      default:
         die_usage();
         break;
      }
   }
   argc -= nopt.arg_ndx;
   argv += nopt.arg_ndx;

   if (!*argv) {
      fatal_usage("missing argument\n");
   }

   if (!basedir)
      basedir = getenv("PERP_BASE");
   if (!basedir)
      basedir = ".";

   if (chdir(basedir) != 0) {
      fatal_syserr("fail chdir() to %s", basedir);
   }

   /* uptimes compared to now: */
   tain_now(&now);

   /* loop through service directory arguments and display status report: */
   for (; *argv != NULL; ++argv) {
      struct stat    st;
      const char    *ctlpath;
      const uchar_t *binstat;
      perpstat_t     perpstat;

      printf("%s:", *argv);
      if (stat(*argv, &st) == -1) {
         printf(" directory not found\n");
         continue;
      }

      if (!S_ISDIR(st.st_mode)) {
         printf(" not a directory\n");
         continue;
      }

      if (!(S_ISVTX & st.st_mode)) {
         printf(" service not activated\n");
         continue;
      }

      ctlpath = perp_ctlpath(&st);

      if (perp_ready(ctlpath) == -1) {
         if ((errno == ENXIO) || (errno == ENOENT)) {
            printf(" supervisor not running\n");
         } else {
            printf(" supervisor not ready: %s\n", sysstr_errno(errno));
         }
         continue;
      }

      binstat = perp_binstat(ctlpath);
      if (binstat == NULL) {
         if (errno == EPROTO) {
            printf(" oops, bad status format!\n");
         } else {
            printf(" error reading status: %s\n", sysstr_errno(errno));
         }
         continue;
      }

      /* decode binary status to perpstat_t object: */
      perp_statload(&perpstat, binstat);

      /* display results for service: */
      report(*argv, &perpstat, &now);
   }

   return 0;
}


/* eof (perpstat.c) */

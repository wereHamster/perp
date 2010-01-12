/* catlimits.c
** display current resource limits
** wcm, 2009.09.08 - 2009.09.18
** ===
*/
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>

#include "ioq.h"
#include "ioq_std.h"
#include "nfmt.h"
#include "rlimit.h"

#include "runtools_common.h"


#define PROGNAME  "catlimits"
static const char progname[] = PROGNAME;
static const char prog_usage[] = "usage: " PROGNAME;

static const char *resources[] = {
   "RLIMIT_AS",
   "RLIMIT_CORE",
   "RLIMIT_CPU",
   "RLIMIT_DATA",
   "RLIMIT_FSIZE",
   "RLIMIT_LOCKS",
   "RLIMIT_MEMLOCK",
   "RLIMIT_NOFILE",
   "RLIMIT_NPROC",
   "RLIMIT_RSS",
   "RLIMIT_SBSIZE",
   "RLIMIT_STACK",
   NULL
};


int main(int argc, char *argv[], char *envp[])
{
   int            opt;
   int            i;

   /* suppress getopt() error message: */
   opterr = 0;
   while ((opt = getopt(argc, argv, "hV")) != -1) {
      char           optc[2] = { (char) optopt, '\0' };
      switch (opt) {
      case 'V':
         version();
         die(0);
         break;
      case 'h':
         usage();
         die(0);
         break;
      case '?':
         if (optopt != '?') {
            eputs(progname, ": usage error: invalid option: -", optc);
         }
         /* fallthrough: */
      default:
         die_usage();
         break;
      }
   }

   argc -= optind;
   argv += optind;

   for (i = 0; resources[i] != NULL; ++i) {
      const char    *resource = resources[i];
      int            r = rlimit_lookup(resource);
      char           nfmt[NFMT_SIZE];
      struct rlimit  rlim;
      if (r == -1) {
         ioq_vputs(ioq1, resource, "\t[not provided on this platform]\n");
         continue;
      }
      getrlimit(r, &rlim);
      ioq_vputs(ioq1, resource, "\t", rlimit_mesg(r), ": ");
      if (rlim.rlim_cur == RLIM_INFINITY) {
         ioq_vputs(ioq1, "unlimited\n");
      } else {
         ioq_vputs(ioq1, nfmt_uint32(nfmt, rlim.rlim_cur), "\n");
      }
   }
   ioq_flush(ioq1);

   return 0;
}


/* eof: catlimits.c */

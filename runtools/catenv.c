/* catenv.c
** display current environ
** wcm, 2009.09.08 - 2009.09.17
** ===
*/
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "ioq.h"
#include "ioq_std.h"

#include "runtools_common.h"


#define PROGNAME  "catenv"
static const char progname[] = PROGNAME;
static const char prog_usage[] = "usage: " PROGNAME;


int main(int argc, char *argv[], char *envp[])
{
   int            opt;

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

   while (*envp) {
      ioq_vputs(ioq1, *envp, "\n");
      ++envp;
   }
   ioq_flush(ioq1);

   return 0;
}


/* eof: catenv.c */

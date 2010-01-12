/* runpause.c
** exec prog after pause
** wcm, 2009.09.23 - 2009.12.15
** ===
*/
#include <errno.h>
#include <unistd.h>

#include "execvx.h"
#include "nextopt.h"
#include "nscan.h"
#include "sig.h"

#include "runtools_common.h"

static const char *progname = NULL;
static const char prog_usage[] = " [-hV] [-L label] secs program [args ...]";


static
void signal_catcher(int signal)
{
   return;
}


int main(int argc, char *argv[], char *envp[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVL:");
   uint32_t       secs;
   const char    *nscan_end;

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
      case 'L':                /* label string ignored */
         break;
      case ':':
         fatal_usage("missing argument for option -", optc);
         break;
      case '?':
         if (nopt.opt_got != '?') {
            fatal_usage("invalid option: -", optc);
         }
         /* else fallthrough: */
      default:
         die_usage();
         break;
      }
   }

   argc -= nopt.arg_ndx;
   argv += nopt.arg_ndx;

   if (argc < 2) {
      fatal_usage("missing required argument(s)");
   }

   secs = nscan_uint32(argv[0], &nscan_end);
   if (*nscan_end != '\0') {
      fatal_usage("bad numeric argument for secs: ", argv[0]);
   }
   ++argv;

   /* catch SIGALRM on pause()/sleep() without termination: */
   sig_catch(SIGALRM, signal_catcher);

   if (secs == 0)
      pause();
   else
      sleep(secs);

   sig_uncatch(SIGALRM);
   errno = 0;

   /* execvx() provides path search for prog */
   execvx(argv[0], argv, envp, NULL);

   /* uh oh: */
   fatal_syserr("unable to run ", argv[0]);

   /* not reached: */
   return 0;
}


/* eof: runpause.c */

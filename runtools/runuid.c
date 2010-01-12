/* runuid.c
** exec prog with permissions of user uid
** (djb setuidgid facility)
** wcm, 2009.09.08 - 2009.12.15
** ===
*/

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

#include "execvx.h"
#include "nextopt.h"

#include "runtools_common.h"

static const char *progname = NULL;
static const char prog_usage[] = " [-hV] account program [args ...]";


int main(int argc, char *argv[], char *envp[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, "hV");
   struct passwd *pw = NULL;
   const char    *acct = NULL;
   const char    *prog = NULL;
   uid_t          uid;
   gid_t          gid;

   progname = nextopt_progname(&nopt);
   while ((opt = nextopt(&nopt))) {
      char           optc[2] = { nopt.opt_got, '\0' };
      switch (opt) {
      case 'V':
         version();
         die(0);
         break;
      case 'h':
         usage();
         die(0);
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

   if (getuid() != 0) {
      fatal_usage("not running as super user");
   }

   if (argc < 2) {
      fatal_usage("missing required argument(s)");
   }

   acct = argv[0];
   ++argv;
   prog = argv[0];

   pw = getpwnam(acct);
   if (pw == NULL) {
      fatal(111, "no account found for user ", acct);
   }

   gid = pw->pw_gid;
   uid = pw->pw_uid;

   if (setgroups(1, &gid) == -1) {
      fatal_syserr("failure setgroups()");
   }

   if (setgid(gid) == -1) {
      fatal_syserr("failure setgid()");
   }

   if (setuid(uid) == -1) {
      fatal_syserr("failure setuid()");
   }

   /* execvx() provides path search for prog */
   execvx(prog, argv, envp, NULL);

   /* uh oh: */
   fatal_syserr("unable to run ", prog);

   /* not reached: */
   return 0;
}


/* eof: runpgrp.c */

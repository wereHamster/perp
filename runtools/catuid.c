/* catuid.c
** display current uid, pid, etc
** wcm, 2009.09.21 - 2009.12.29
** ===
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
/* getsid() not posix: */
extern pid_t   getsid(pid_t pid);

#include "runtools_common.h"


#define PROGNAME  "catuid"
static const char progname[] = PROGNAME;
static const char prog_usage[] = "usage: " PROGNAME "-hv";


#define eputf(...) \
  {\
    static char  s[500]; \
    snprintf(s, sizeof (s), __VA_ARGS__); \
    eputs(s); \
  }

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


   eputf("UID:\t%d", getuid());
   eputf("EUID:\t%d", geteuid());
   eputf("GID:\t%d", getgid());
   eputf("EGID:\t%d", getegid());
   eputf("PID:\t%d", getpid());
   eputf("PPID:\t%d", getppid());
   eputf("PGID:\t%d", getpgrp());
   eputf("SID:\t%d", getsid(0));
   eputf("umask:\t%o", umask(0));

   return 0;
}


/* eof: catuid.c */

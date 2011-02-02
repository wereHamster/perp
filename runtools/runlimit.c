/* runlimit.c
** exec prog with resource limits 
** (djb softlimit facility)
** wcm, 2009.09.08 - 2011.01.31
** ===
*/
#include <unistd.h>
#include <sys/resource.h>

#include "execvx.h"
#include "nextopt.h"
#include "nuscan.h"
#include "rlimit.h"

#include "runtools_common.h"

static const char *progname = NULL;
static const char prog_usage[] =
  "[-hV]"
  " [-a availmem] [-c coresize] [-d databytes] [-f filesize]"
  " [-l lockbytes] [-m membytes] [-o openfiles] [-p processes]"
  " [-r rssbytes] [-s stackbytes] [-t cpusecs]"
  " program [args ...]";

static int verbose = 0;
#define warn(...) \
  {\
    if(verbose) eputs(progname, "warning: ", __VA_ARGS__); \
  }

static void do_limit(int resource, const char *optc, const char *arg);


static
void
do_limit(int resource, const char *optc, const char *arg)
{
  struct rlimit  r;
  uint32_t       u;
  const char    *z;

  if(getrlimit(resource, &r) == -1){
      fatal_syserr("failure getrlimit() for option -", optc);
  }

  if((arg[0] == '_') || (arg[0] == '^') || (arg[0] == '=')){
      /* special processing for underscore '_', '^', and '=':
      **   set resource upto hardlimit
      */
      r.rlim_cur = r.rlim_max;
  } else {
      z = nuscan_uint32(&u, arg);
      if(*z != '\0'){
          fatal_usage("non-numeric argument for option -", optc, ": ", arg);
      }
      r.rlim_cur = (u < r.rlim_max ? u : r.rlim_max);
  }
  
  if(setrlimit(resource, &r) == -1){
      fatal_syserr("failure setrlimit() for option -", optc);
  }

  return;
}


#define try_rlimit(def) \
  {\
    int r = rlimit_lookup((def)); \
    if(r == -1) {\
        warn("option -", optc, ": ", (def), " not supported on this system"); \
    } else {\
        do_limit(r, optc, nopt.opt_arg); \
    }\
  }


int
main(int argc, char *argv[], char *envp[])
{
  nextopt_t  nopt = nextopt_INIT(argc, argv, ":hVva:c:d:f:l:m:o:p:r:s:t:");
  char       opt;

  progname = nextopt_progname(&nopt);
  while((opt = nextopt(&nopt))){
      char optc[2] = {nopt.opt_got, '\0'};
      switch(opt){
      case 'V': version(); die(0); break;
      case 'h': usage(); die(0); break;
      case 'v': ++verbose; break;
      case 'a':
          /* total available memory */
          /* rlimit.h enforces synonymity of RLIMIT_AS and RLIMIT_VMEM: */
          try_rlimit("RLIMIT_AS");
          break;
      case 'c':
          /* coresize (set to 0 to disable corefiles) */
          try_rlimit("RLIMIT_CORE");
          break;
      case 'd':
          /* maximum data segment */
          try_rlimit("RLIMIT_DATA");
          break;
      case 'f':
          /* maximum file size */
          try_rlimit("RLIMIT_FILE");
          break;
      case 'l':
          /* maximum mlock(): */ 
          try_rlimit("RLIMIT_MEMLOCK");
          break;
      case 'm':
          /* all of _AS/_VMEM _DATA _STACK _MEMLOCK */
          try_rlimit("RLIMIT_AS");
          try_rlimit("RLIMIT_DATA");
          try_rlimit("RLIMIT_STACK");
          try_rlimit("RLIMIT_MEMLOCK");
          break;
      case 'o':
          /* maximum open files */
          try_rlimit("RLIMIT_NOFILE");
          break;
      case 'p':
          /* maximum number of processes per uid */
          try_rlimit("RLIMIT_NPROC");
          break;
      case 'r':
          /* rss resident set size bytes */
          try_rlimit("RLIMIT_RSS");
          break;
      case 's':
          /* maximum stack size */
          try_rlimit("RLIMIT_STACK");
          break;
      case 't':
          /* maximum cpu time (seconds) */
          try_rlimit("RLIMIT_CPU");
          break;
      case ':':
          fatal_usage("missing argument for option -", optc);
          break;
      case '?':
          if(nopt.opt_got != '?'){
              fatal_usage("invalid option: -", optc);
          }
          /* else fallthrough: */
      default :
          die_usage(); break; 
      }
  }

  argc -= nopt.arg_ndx;
  argv += nopt.arg_ndx;

  if(argv[0] == NULL){
      fatal_usage("missing program argument");
  }

  /* execvx() provides path search for prog */
  execvx(argv[0], argv, envp, NULL);

  /* uh oh: */
  fatal_syserr("unable to run ", argv[0]);

  /* not reached: */
  return 0;
}


/* eof: runlimit.c */

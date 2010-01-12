/* perpd.c
** perpd: persistent process scanning daemon
** wcm, 2008.01.03 - 2009.12.09
** ===
*/

/* standard library headers: */
#include <stdlib.h>
#include <stdarg.h>

/* unix standard headers: */
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

/* lasagna headers: */
#include "cstr.h"
#include "fd.h"
#include "nextopt.h"
#include "nfmt.h"
#include "nscan.h"
#include "newenv.h"
#include "nextopt.h"
#include "outvec.h"
#include "pidlock.h"
#include "sig.h"
#include "sigset.h"
#include "sysstr.h"
#include "tain.h"

/* macros for logging perp daemons to stderr: */
#include "loggit.h"
/* common defines for perp system: */
#include "perp_common.h"

/* access environment: */
extern char  **environ;

/*
** logging variables in scope:
*/
static const char *progpid = NULL;
static const char *progname = NULL;
static const char prog_usage[] = " [-hV] [-a secs] [basedir]";

/*
** configure loggit.h macros:
*/
#define LOGGIT_FMT \
  progname, "[", progpid, "]: "

/*
** objects:
*/

/* sv object (for each perpetrate supervisor instance): */
struct sv {
   dev_t          dev;
   ino_t          ino;
   pid_t          pid;
   int            cull;
};

/* perpd object (this perpd instance): */
struct perpd {
   const char    *basedir;      /* directory scanning */
   uint32_t       autoscan;     /* scanning interval (secs) */
   struct tain    when;         /* my uptime timestamp */
   int            fdpidlock;    /* fd for pidlock file */
   sigset_t       sigset;       /* sigset to block/unblock during nanosleep() */
   int            n;            /* number of sv processes in sv[] */
   int            max;          /* PERP_MAX */
   struct sv      sv[PERP_MAX]; /* sv processes */
};


/*
**  variables in scope:
*/
static pid_t   mypid = 0;
static int     selfpipe[2];
static int     flagterm = 0;
static int     flaghup = 0;
static int     fdmax = 0;


/*
** declarations in scope:
*/
static void    sigtrap(int sig);
static void    selfpipe_ping(void);
static void    sv_init(struct sv *sv);
static void    perpd_init(struct perpd *perpd, const char *basedir,
                          uint32_t autoscan);
static void    setup_control(struct perpd *perpd);
static int     run_child(struct perpd *perpd, char *prog[]);
static const struct stat *check_sv(char *svdir);
static void    run_sv(struct perpd *perpd, int i, char *svdir);
static void    check_children(struct perpd *perpd);
static void    perpd_scan(struct perpd *perpd);
static void    main_loop(struct perpd *perpd);
static void    perpd_shutdown(struct perpd *perpd);


/*
** definitions:
*/

static
void sigtrap(int sig)
{
   switch (sig) {
   case SIGINT:
      log_debug("got SIGINT");
      flagterm = 1;
      break;
   case SIGTERM:
      log_debug("got SIGTERM");
      flagterm = 1;
      break;
   case SIGHUP:
      log_debug("got SIGHUP");
      flaghup = 1;
      break;
   case SIGCHLD:
      break;
   default:
      break;
   }

   selfpipe_ping();
   return;
}


static
void selfpipe_ping(void)
{
   int            terrno = errno;
   int            w;

   do {
      w = write(selfpipe[1], "!", 1);
   } while ((w == -1) && (errno == EINTR));

   errno = terrno;
   return;
}


/* object init methods: */
static
void sv_init(struct sv *sv)
{
   sv->dev = 0;
   sv->ino = 0;
   sv->pid = 0;
   sv->cull = 0;

   return;
}


/* perpd_init()
**   initialize perpd object
**   setup base directory
**   block signals
**   ...
*/
static
void perpd_init(struct perpd *perpd, const char *basedir, uint32_t autoscan)
{
   int            fd;

   /* initialize assignments: */
   perpd->basedir = basedir;
   perpd->autoscan = autoscan;
   tain_now(&perpd->when);
   perpd->fdpidlock = -1;

   sigset_fill(&perpd->sigset);
   perpd->n = 0;
   perpd->max = (int) PERP_MAX;

   if (chdir(basedir) != 0) {
      fatal_syserr("failure chdir() to base directory ", basedir);
   }

   /* block signals: */
   sigset_block(&perpd->sigset);

   /* redirect stdin: */
   if ((fd = open("/dev/null", O_RDWR)) == -1) {
      fatal_syserr("failure on open() for /dev/null");
   }
   fd_move(0, fd);

   /* selfpipe: */
   if (pipe(selfpipe) == -1) {
      fatal_syserr("failure pipe() for selfpipe");
   }
   fd_cloexec(selfpipe[0]);
   fd_nonblock(selfpipe[0]);
   fd_cloexec(selfpipe[1]);
   fd_nonblock(selfpipe[1]);

   /* setup for intentional mode on file creation: */
   umask(0);

   return;
}


/* setup_control()
**   the control directory "shadows" the service directory
**   cwd is base directory on entry/exit
*/
static
void setup_control(struct perpd *perpd)
{
   int            fd = -1;
   int            fdbase;

   /* setup for return to base directory: */
   if ((fdbase = open(".", O_RDONLY)) == -1) {
      fatal_syserr("failure open() on base directory");
   }

   /* initialize .control directory: */
   if (mkdir(PERP_CONTROL, 0700) == -1) {
      char           pathbuf[256];
      int            n;
      /* permit configuration using dangling symlink: */
      if ((n = readlink(PERP_CONTROL, pathbuf, sizeof pathbuf)) != -1) {
         if (n < (sizeof pathbuf)) {
            pathbuf[n] = '\0';
            mkdir(pathbuf, 0700);       /* ignore failure */
         } else {
            fatal(111,
                  "error readlink() on base control directory: symlink name too long");
         }
      }
      /* ignoring other errors */
   }

   if (chdir(PERP_CONTROL) != 0) {
      fatal_syserr("failure chdir() to ", PERP_CONTROL);
   }

   /* initialize .perpd within control directory: */
   if (mkdir(PERPD_CONTROL, 0700) != 0) { /* ignore failure: */ ;
   }

   if (chdir(PERPD_CONTROL) != 0) {
      fatal_syserr("failure chdir() to ", PERPD_CONTROL,
                   " control directory");
   }

   /* initialize pidlock (for single server instance): */
   fd = pidlock_set(PIDLOCK, mypid, PIDLOCK_NOW);
   if (fd == -1) {
      fatal_syserr("failure open/lock/write pidlock file ", PIDLOCK);
   }
   fd_cloexec(fd);
   perpd->fdpidlock = fd;

   /* restore cwd: */
   if (fchdir(fdbase) == -1) {
      fatal_syserr("failure fchdir() to base directory");
   }
   close(fdbase);

   return;
}


/* run_child()
**   setup child process and exec() prog:
**
**       prog[0]: "perpetrate"
**       prog[1]: "<dir>"
**
**   called after fork()
**
**   return:
**     should only return on error of exec()
*/
static
int run_child(struct perpd *perpd, char *prog[])
{
   int            i;

   /* set PERP_BASE: */
   if (newenv_set("PERP_BASE", perpd->basedir) == -1) {
      fatal_syserr("(in child) failure setting environment for ",
                   prog[0], " ", prog[1]);
   }

   /* close extraneous file descriptors: */
   for (i = 3; i < fdmax; ++i)
      close(i);

   /* setsid() to put child in new process group: */
   setsid();

   /* clear signal handlers from child process: */
   sig_uncatch(SIGINT);
   sig_uncatch(SIGTERM);
   sig_uncatch(SIGCHLD);
   sig_uncatch(SIGHUP);
   sig_uncatch(SIGPIPE);
   sigset_unblock(&perpd->sigset);

   return newenv_run(prog, environ);
}


/* check_sv()
**   check svdir for valid service directory: name, isdir, and sticky
**   called by perpd_scan()
**   returns:
**     success: pointer to static struct stat object for svdir
**     failure: NULL
*/
static
const struct stat *check_sv(char *svdir)
{
   /* persistent stat object! */
   static struct stat st;

   /* ignore: */
   if (svdir[0] == '.')
      return NULL;

   /* get the stat: */
   if (stat(svdir, &st) == -1) {
      warn_syserr("failure stat() on ", svdir);
      /* clear errno: */
      errno = 0;
      return NULL;
   }

   /* check stat for directory and sticky bit: */
   if (!((S_ISDIR(st.st_mode)) && (st.st_mode & S_ISVTX))) {
      return NULL;
   }

   /* okay then: */
   return (const struct stat *) &st;
}


/* run_sv()
**   fork()/exec() supervisor for service directory
*/
static
void run_sv(struct perpd *perpd, int i, char *svdir)
{
   char          *prog[] = { "perpetrate", svdir, NULL };
   pid_t          child;

   log_debug("starting supervisor on service directory: ", svdir);
   child = fork();
   if (child == -1) {
      warn_syserr("failure fork() to supervise service ", svdir);
      return;
   }
   if (child == 0) {
      /* child: */
      run_child(perpd, prog);
      fatal_syserr("(in child) failure execvp() on ", prog[0], " ", prog[1]);
   }

   /* parent: */
   perpd->sv[i].pid = child;
   perpd->sv[i].cull = 0;

   return;
}


/* check_children()
**   waitpid() for any terminated child processes
*/
static
void check_children(struct perpd *perpd)
{
   pid_t          p;
   int            wstat;
   int            i;

   for (;;) {
      do {
         p = waitpid(-1, &wstat, WNOHANG);
      } while ((p == -1) && (errno == EINTR));

      if (p <= 0) {
         /* no child or error: */
         break;
      }

      /* a perpetrate child has exited, set its pid 0: */
      for (i = 0; i < perpd->n; ++i) {
         if (perpd->sv[i].pid == p) {
            perpd->sv[i].pid = 0;
            break;
         }
      }
   }

   return;
}


/* perpd_scan()
**   loop over cwd: add new services; cull old services
**   (provides the core directory scanning logic)
*/
static
void perpd_scan(struct perpd *perpd)
{
   DIR           *dir;
   struct dirent *d;
   char          *svdir;
   const struct stat *st;
   int            i;
   int            terrno;

   if ((dir = opendir(".")) == NULL) {
      warn_syserr("failure opendir() for service scan");
      return;
   }

   /* flag all for cull (active services will unflag cull): */
   for (i = 0; i < perpd->n; ++i) {
      perpd->sv[i].cull = 1;
   }

   /* reset errno before scanning: */
   errno = 0;
   /* scan: */
   for (;;) {
      if ((d = readdir(dir)) == NULL) {
         break;
      }

      svdir = d->d_name;
      if ((st = check_sv(svdir)) == NULL) {
         /* skip this dirent: */
         continue;
      }

      /* scan existing services for index with this dev/ino: */
      for (i = 0; i < perpd->n; ++i) {
         if ((perpd->sv[i].ino == st->st_ino) &&
             (perpd->sv[i].dev == st->st_dev)) {
            /* keeper: */
            perpd->sv[i].cull = 0;
            if (perpd->sv[i].pid == 0) {
               /* restart supervisor: */
               run_sv(perpd, i, svdir);
            }
            break;
         }
      }

      /* add new service: */
      if (i == perpd->n) {
         if (!(perpd->n < perpd->max)) {
            log_warning("unable to add new service ", svdir,
                        ": too many services");
            continue;
         }
         sv_init(&perpd->sv[i]);
         perpd->sv[i].dev = st->st_dev;
         perpd->sv[i].ino = st->st_ino;
         perpd->sv[i].pid = 0;
         /* start supervisor: */
         run_sv(perpd, i, svdir);
         ++perpd->n;
         log_debug("adding service ", svdir);
      }
   }
   terrno = errno;
   closedir(dir);
   errno = terrno;

   if (errno) {
      warn_syserr("failure readdir() while scanning base directory");
      /* XXX, if readdir() has failed, cull state is unstable! */
      return;
   }

   /* remove any sv still flagged for cull: */
   for (i = 0; i < perpd->n; ++i) {
      if (perpd->sv[i].cull == 1) {
         if (perpd->sv[i].pid != 0) {
            kill(perpd->sv[i].pid, SIGTERM);
            kill(perpd->sv[i].pid, SIGCONT);
         }
         --perpd->n;
         perpd->sv[i] = perpd->sv[perpd->n];
         log_debug("removing service flagged for cull");
      }
   }

   return;
}


static
void main_loop(struct perpd *perpd)
{
   struct pollfd  pfd[1];
   int            poll_interval;
   char           nbuf[NFMT_SIZE];
   int            last_count = 0;
   char           c;
   int            e;

   /* initialize poll(): */
   pfd[0].fd = selfpipe[0];
   pfd[0].events = POLLIN;
   poll_interval = (perpd->autoscan ? (perpd->autoscan * 1000) : -1);

   /* initial scan: */
   perpd_scan(perpd);

   for (;;) {

      if (last_count != perpd->n) {
         log_info("monitoring ", nfmt_uint32(nbuf, perpd->n),
                  " services ...");
         last_count = perpd->n;
      }

      /* poll() while signals unblocked: */
      sigset_unblock(&perpd->sigset);
      {
         do {
            e = poll(pfd, 1, poll_interval);
         } while ((e == -1) && (errno == EINTR));
      }
      sigset_block(&perpd->sigset);

      if (e == -1) {
         warn_syserr("failure poll() in main loop");
         continue;
      }

      /* consume selfpipe: */
      while ((read(selfpipe[0], &c, 1)) == 1) {
         /* empty */ ;
      }

      /* all done? */
      if (flagterm)
         break;

      /* dead children? */
      check_children(perpd);

      /* rescan: */
      perpd_scan(perpd);
   }

   return;
}


/* perpd_shutdown()
**   terminate existing supervisors
**   waitpid() for them
*/
static
void perpd_shutdown(struct perpd *perpd)
{
   int            i;
   int            e;

   log_debug("starting shutdown sequence ...");
   /* terminate supervisors: */
   for (i = 0; i < perpd->n; ++i) {
      if (perpd->sv[i].pid > 0) {
         kill(perpd->sv[i].pid, SIGTERM);
         kill(perpd->sv[i].pid, SIGCONT);
      }
   }

   /* collect them: */
   /* XXX, make this an option? or don't do this for some signals? */
   for (i = 0; i < perpd->n; ++i) {
      do {
         e = waitpid(perpd->sv[i].pid, NULL, 0);
      } while ((e == -1) && (errno == EINTR));
   }

   return;
}


int main(int argc, char *argv[])
{
   static char    pidbuf[NFMT_SIZE];
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVa:");
   const char    *basedir = NULL;
   uint32_t       arg_autoscan = 0;
   const char    *nscan_end;
   struct perpd   perpd;

   /* pid for pidlock and logging: */
   mypid = getpid();
   progpid = nfmt_uint32(pidbuf, (uint32_t) mypid);
   /* progname for logging: */
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
      case 'a':
         arg_autoscan = nscan_uint32(nopt.opt_arg, &nscan_end);
         if (*nscan_end != '\0') {
            fatal_usage("non-numeric argument found for option -", optc, ": ",
                        arg_autoscan);
         }
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

   basedir = argv[0];

   if (!basedir)
      basedir = getenv("PERP_BASE");
   if (!basedir || (basedir[0] == '\0'))
      basedir = PERP_BASE_DEFAULT;

   if (basedir[0] != '/') {
      fatal_usage("base directory not defined as absolute path: ", basedir);
   }

   log_info("starting on ", basedir, " ...");

   /* initialize perpd, block signals, close fds ...: */
   perpd_init(&perpd, basedir, arg_autoscan);
   /* initialize control directory and acquire pidlock: */
   setup_control(&perpd);

   /*
    ** no fatals beyond this point!
    */

   /* setup signal handlers: */
   sig_catch(SIGINT, &sigtrap);
   sig_catch(SIGTERM, &sigtrap);
   sig_catch(SIGHUP, &sigtrap);
   sig_catch(SIGCHLD, &sigtrap);

   /* scan: */
   main_loop(&perpd);

   /* shutdown: */
   perpd_shutdown(&perpd);

   log_info("terminating normally on ", basedir);
   return 0;
}


/* eof (perpd.c) */

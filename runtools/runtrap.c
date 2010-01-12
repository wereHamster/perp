/* runtrap.c
** exec a prog and forward any signals for it to a trapper
** wcm, 2009.11.22 - 2009.12.15
** ===
*/

/* behavior issues:
**
**  ? runtrap hangs in do_trap() if trapper hangs:
**  -->  user error: failure in configuration or expression of trapper
**
**  ? runtrap itself will not exit unless/until do_trap(SIGTERM) causes
**    child termination:
**  -->  if thats what child in fact does, behavior is consistent
**  -->  otherwise: user error as above
**
*/

#include <stdlib.h>

#include <unistd.h>
#include <poll.h>
#include <sys/wait.h>

#include "cstr.h"
#include "execvx.h"
#include "fd.h"
#include "nextopt.h"
#include "nfmt.h"
#include "sig.h"
#include "sigset.h"
#include "sysstr.h"
#include "tain.h"

#include "runtools_common.h"

/* environ: */
extern char  **environ;

static const char *progname = NULL;
static const char prog_usage[] = " [-hV] id trapper child [args ...]";


#define warn(...) \
    eputs(progname, ": warning: ", __VA_ARGS__)


struct child {
   char         **argv;
   pid_t          pid;
   tain_t         when;
};
typedef struct child child_t;


/* variables in scope: */
static int     my_sigpipe[2];
static int     flag_term = 0;
static int     last_signal = 0;
static sigset_t my_sigset;
static char   *my_trap_path;
static char   *my_trap_id;
static child_t my_child;

/* prototypes in scope: */
static void    sigpipe_ping(void);
static void    sigtrap(int signal);
static void    setup(void);
static void    child_exec(void);
static void    child_wait(void);
static void    do_trap(int signal);
static void    main_loop(void);


static
void sigpipe_ping(void)
{
   int            terrno = errno;
   int            w;

   do {
      w = write(my_sigpipe[1], "!", 1);
   } while ((w == -1) && (errno == EINTR));

   errno = terrno;
   return;
}


static
void sigtrap(int signal)
{
   /* discard any signals if already got SIGTERM: */
   if (flag_term) {
      /* XXX, emit warning diagnostic */
      sigpipe_ping();
      return;
   }

   switch (signal) {
   case SIGCHLD:
      signal = last_signal;
      break;
   case SIGTERM:
      /* if we catch SIGTERM:
       **   assume exit is desired and setup to terminate
       */
      flag_term = 1;
      /* fallthrough: */
   default:
      /* pass signal to trapper: */
      do_trap(signal);
      break;
   }

   last_signal = signal;
   sigpipe_ping();
   return;
}


static
void setup(void)
{
   int            i = 0;

   if (pipe(my_sigpipe) == -1) {
      fatal_syserr("failure setting up selfpipe");
   }

   for (i = 0; i < 2; ++i) {
      fd_cloexec(my_sigpipe[i]);
      fd_nonblock(my_sigpipe[i]);
   }

   sigset_fill(&my_sigset);
   sigset_block(&my_sigset);

   sig_catch(SIGTERM, &sigtrap);
   sig_catch(SIGINT, &sigtrap);
   sig_catch(SIGCHLD, &sigtrap);

   /* catching these signals to pass to program: */
   sig_catch(SIGALRM, &sigtrap);
   sig_catch(SIGCONT, &sigtrap);
   sig_catch(SIGHUP, &sigtrap);
   sig_catch(SIGQUIT, &sigtrap);
   sig_catch(SIGTSTP, &sigtrap);
   sig_catch(SIGUSR1, &sigtrap);
   sig_catch(SIGUSR2, &sigtrap);

   sig_ignore(SIGPIPE);
   return;
}


static
void do_trap(int signo)
{
   pid_t          pid;
   char          *trap_argv[7];
   char           fmt_pid[NFMT_SIZE];
   char           fmt_signo[NFMT_SIZE];
   int            wstat;

   /* invariant: */
   trap_argv[0] = my_trap_path;
   trap_argv[1] = "trap";
   trap_argv[2] = my_trap_id;
   /* variant: */
   trap_argv[3] = nfmt_uint32(fmt_pid, my_child.pid);
   trap_argv[4] = nfmt_uint32(fmt_signo, signo);
   trap_argv[5] = (char *) sysstr_signal(signo);
   /* */
   trap_argv[6] = NULL;

   /* discard signal if no child:
    **   note: the kill() test may be ineffective
    **   also: race conditions are still possible
    */
   if ((my_child.pid == 0) || (kill(my_child.pid, 0) == -1)) {
      warn("child not running, discarding signal ", trap_argv[5]);
      return;
   }

   warn("do_trap() on signal ", trap_argv[5]);

   while ((pid = fork()) == -1) {
      warn("failure on fork() for runtrap ", my_trap_id, ": ", my_trap_path);
      sleep(2);
   }

   /* child: */
   if (pid == 0) {
      /* reset default signal handlers, unblock: */
      sig_default(SIGTERM);
      sig_default(SIGCHLD);
      sig_default(SIGALRM);
      sig_default(SIGCONT);
      sig_default(SIGHUP);
      sig_default(SIGINT);
      sig_default(SIGQUIT);
      sig_default(SIGTSTP);
      sig_default(SIGUSR1);
      sig_default(SIGUSR2);
      sig_default(SIGPIPE);
      sigset_unblock(&my_sigset);
      /* do it: */
      execvx(trap_argv[0], (char *const *) trap_argv, environ, NULL);
      /* uh oh: */
      fatal_syserr("failure on exec of trap ", trap_argv[0]);
   }

   /* XXX:
    **   ? possible timeout/failsafe if trapper fails to complete
    **   ? including SIGTERM directly to child pid if SIGTERM && flag_term
    */

   /* parent: block for completion of trapper */
   while (waitpid(pid, &wstat, 0) != pid) {
      if (errno != EINTR) {
         warn("failure waitpid() on trapper: ", sysstr_errno(errno));
         break;
      }
   }
   /* XXX, emit wstat diagnostic to stderr? */
   warn("trapper completion");

   return;
}


static
void child_exec(void)
{
   pid_t          pid;
   tain_t         now;
   tain_t         when_ok;
   char         **argv = my_child.argv;

   tain_now(&now);
   tain_load(&when_ok, 1, 0);
   tain_plus(&when_ok, &my_child.when, &when_ok);
   if (tain_less(&now, &when_ok)) {
      warn("pausing for respawn of ", argv[0], " ...");
      sleep(1);
   }

   while ((pid = fork()) == -1) {
      warn("failure on fork() while starting ", argv[0]);
      sleep(2);
   }

   /* child: */
   if (pid == 0) {
      /* reset default signal handlers, unblock: */
      sig_default(SIGTERM);
      sig_default(SIGCHLD);
      sig_default(SIGALRM);
      sig_default(SIGCONT);
      sig_default(SIGHUP);
      sig_default(SIGINT);
      sig_default(SIGQUIT);
      sig_default(SIGTSTP);
      sig_default(SIGUSR1);
      sig_default(SIGUSR2);
      sig_default(SIGPIPE);
      sigset_unblock(&my_sigset);
      /* do it: */
      execvx(argv[0], argv, environ, NULL);
      /* uh oh: */
      fatal_syserr("failure on exec of ", argv[0]);
   }

   /* parent: */
   my_child.pid = pid;
   tain_now(&my_child.when);

   return;
}



static
void child_wait(void)
{
   pid_t          pid;
   int            wstat;

   while ((pid = waitpid(-1, &wstat, WNOHANG)) > 0) {
      if (pid == my_child.pid) {
         my_child.pid = 0;
         /* XXX, emit some diagnostic for wstat? */
      }
   }

   return;
}


static
void main_loop(void)
{
   struct pollfd  pfd[1];
   int            e = 0;
   char          *c;

   pfd[0].fd = my_sigpipe[0];
   pfd[0].events = POLLIN;

   for (;;) {
      /* terminal condition: */
      if (flag_term && (my_child.pid == 0)) {
         /* all done: */
         break;
      }

      /* start/restart child: */
      if (my_child.pid == 0) {
         child_exec();
      }

      /* note:
       **   if do_trap(SIGTERM) doesn't kill child
       **   ie: if ((flag_term) && (my_child.pid != 0))
       **   we hang
       */

      /* poll on sigpipe: */
      sigset_unblock(&my_sigset);
      do {
         e = poll(pfd, 1, -1);
      } while ((e == -1) && (errno == EINTR));
      sigset_block(&my_sigset);

      /* consume sigpipe: */
      while (read(my_sigpipe[0], &c, 1) == 1);

      /* consume dead children: */
      child_wait();
   }

   return;
}


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hV");

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

   if (argc < 3) {
      fatal_usage("missing required arguments");
   }

   /* trapper: */
   my_trap_id = *argv++;
   my_trap_path = *argv++;

   /* child: */
   my_child.argv = argv;
   my_child.pid = 0;
   tain_load(&my_child.when, 0, 0);

   /* initialize sigpipe[], etc: */
   setup();
   /* da bidness: */
   main_loop();

   return 0;
}


/* eof: runtrap.c */

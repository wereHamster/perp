/* perpctl.c
** perpctl: perpetrate supervisor control interface
** wcm, 2008.01.23 - 2010.01.05
** ===
*/

/* standard library headers: */
#include <stdlib.h>

/* unix headers: */
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* lasagna headers: */
#include "buf.h"
#include "fd.h"
#include "nextopt.h"
#include "padlock.h"
#include "pidlock.h"
#include "pkt.h"
#include "sig.h"
#include "sysstr.h"
#include "uchar.h"
#include "upak.h"

/* local headers: */
#define  EPUTS_OUTVEC  1
#include "eputs.h"
#include "perp_common.h"
#include "perplib.h"

static const char *progname = NULL;
static const char prog_usage[] =
    " [-hV] [-b basedir] [-L] [DUXduopcahikqtw12] sv [sv ...]";


/* prototypes in scope: */
static int     do_control(const char *svdir, const pkt_t pkt);


/* do_control():
** for service named in svdir
**   send command packet to input fifo
**   read response packet from output fifo
**
** notes:
**   cwd is preset to service control directory
**
** return:
**   0 : ok
**  -1 : error, diagnostic on stderr
*/
static
int do_control(const char *svdir, const pkt_t pkt)
{
   int            ctl_lock;
   int            fifo_0, fifo_1;
   pkt_t          reply;
   int            n;
   int            terrno;

   /* open/acquire client access lock: */
   ctl_lock = open(CTL_LOCK, O_WRONLY);
   if (ctl_lock == -1) {
      eputs(svdir, ": failure on open() for client lock ", CTL_LOCK, ": ",
            sysstr_errno(errno));
      return -1;
   }
   if (padlock_exlock(ctl_lock, PADLOCK_WAIT) == -1) {
      eputs(svdir, ": failure acquiring lock on client lock ", CTL_LOCK, ": ",
            sysstr_errno(errno));
      close(ctl_lock);
      return -1;
   }

   /* open control fifos: */
   fifo_0 = open(CTL_IN, O_WRONLY);
   if (fifo_0 == -1) {
      eputs(svdir, ": failure on open() for input fifo ", CTL_IN, ": ",
            sysstr_errno(errno));
      close(ctl_lock);
      return -1;
   }
   fifo_1 = open(CTL_OUT, O_RDONLY);
   if (fifo_1 == -1) {
      eputs(svdir, ": failure on open for output fifo ", CTL_OUT, ": ",
            sysstr_errno(errno));
      close(fifo_0);
      close(ctl_lock);
      return -1;
   }

   /* send command packet: */
   n = pkt_write(fifo_0, pkt);
   if (n == -1) {
      int            terrno = errno;
      eputs(svdir, ": failure packet_write() for command packet: ",
            sysstr_errno(terrno));
      close(fifo_1);
      close(fifo_0);
      close(ctl_lock);
      return -1;
   }

   /* get reply packet: */
   n = pkt_read(fifo_1, reply);

   /* done with connection: */
   terrno = errno;
   close(fifo_1);
   close(fifo_0);
   close(ctl_lock);
   errno = terrno;

   if (n == -1) {
      eputs(svdir, ": failure on pkt_read() for command reply: ",
            sysstr_errno(errno));
      return -1;
   }

   if (pkt_proto(reply) != 1) {
      eputs(svdir, ": protocol error found in server response");
      return -1;
   }

   if ((pkt_type(reply) == 'E') && (pkt_size(reply) == 4)) {
      uchar_t       *payload = pkt_data(reply);
      int            err = (int) upak32_unpack(payload);

      /* error reported in reply packet? */
      if (err != 0) {
         eputs(svdir, ": server reply to command packet with error: ",
               sysstr_errno(err));
         return -1;
      }
   } else {
      eputs(svdir, ": protocol error found in server repsonse");
      return -1;
   }

   /* success: */
   eputs(svdir, ": ok");
   return 0;
}


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVb:L");
   int            logflag = 0;
   pkt_t          pkt = pkt_INIT(1, 'C', 1);    /* a "command" packet */
   uchar_t       *cmd = pkt_data(pkt);
   const char    *basedir = NULL;
   int            fd_base;
   int            errs = 0;

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
      case 'L':
         logflag = 1;
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

   if (!*argv) {
      eputs(progname, ": usage error: missing arguments");
      die_usage();
   }

   /* next argv is the control command (can be taken from first letter): */
   switch (cmd[0] = *argv[0]) {
   case 'd':
   case 'u':
   case 'o':
   case 'p':
   case 'c':
   case 'a':
   case 'h':
   case 'i':
   case 'k':
   case 'q':
   case 't':
   case 'w':
   case '1':
   case '2':
      if (logflag) {
         /* control command for log service: */
         eputs("shifting command for log service");
         cmd[0] += 0x7f;
      }
      break;
   case 'D':
   case 'U':
   case 'X':
      if (logflag) {
         fatal_usage("meta-command '", *argv,
                     "' may not be used with option -L");
      }
      break;
   default:
      fatal_usage("unknown control command '", *argv, "'");
      break;
   }

   --argc;
   ++argv;
   if (!*argv) {
      fatal_usage("missing argument: no service(s) specified");
   }
#if 0
   {
      cmd[1] = '\0';
      eputs(progname, ": sending command ", cmd);
   }
#endif

   if (basedir == NULL)
      basedir = getenv("PERP_BASE");
   if ((basedir == NULL) || (basedir[0] == '\0'))
      basedir = ".";

   if (chdir(basedir) != 0) {
      fatal_syserr("unable to chdir() to ", basedir);
   }

   if ((fd_base = open(".", O_RDONLY | O_NONBLOCK)) == -1) {
      fatal_syserr("unable to open() on ", basedir);
   }

   /* ignore SIGPIPE: */
   sig_ignore(SIGPIPE);

   /* loop through service directory argument(s) and send control packet: */
   for ( /* */ ; *argv != NULL; ++argv) {
      struct stat    st;
      const char    *ctlpath;
      int            e;

      if (stat(*argv, &st) != 0) {
         eputs(*argv, ": failure stat() on service directory: ",
               sysstr_errno(errno));
         ++errs;
         continue;
      }
      if (!S_ISDIR(st.st_mode)) {
         eputs(*argv, ": not a directory");
         ++errs;
         continue;
      }

      ctlpath = perp_ctlpath(&st);
      if (perp_ready(ctlpath) == -1) {
         if ((errno == ENXIO) || (errno == ENOENT)) {
            eputs(*argv, ": supervisor not running");
         } else {
            eputs(*argv, ": failure checking supervisor: ",
                  sysstr_errno(errno));
         }
         ++errs;
         continue;
      }

      if (chdir(ctlpath) == -1) {
         eputs(*argv, ": failure chdir() to service control directory: ",
               sysstr_errno(errno));
         ++errs;
         continue;
      }

      /* the business: */
      e = do_control(*argv, pkt);
      if (e != 0)
         ++errs;
      if (fchdir(fd_base) == -1) {
         fatal_syserr("failure fchdir() to base directory");
      }
   }

   return (errs ? 111 : 0);
}


/* eof (perpctl.c) */

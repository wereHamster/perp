/* perpls.c
** perpls: perp persistent process lister
** wcm, 2008.01.23 - 2010.01.05
** ===
*/

/* standard library headers: */
#include <stdlib.h>

/* unix headers: */
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* lasagna headers: */
#include "buf.h"
#include "cstr.h"
#include "dynstuf.h"
#include "ioq.h"
#include "ioq_std.h"
#include "nextopt.h"
#include "nfmt.h"
#include "padlock.h"
#include "pidlock.h"
#include "sig.h"
#include "sysstr.h"
#include "tain.h"
#include "upak.h"

/* local headers: */
#define  EPUTS_IOQ
#include "eputs.h"
#include "perp_common.h"
#include "perplib.h"


const char    *progname = NULL;
const char     prog_usage[] = " [-hV] [-b basedir] [-cGgrt] [sv ...]";

/* timestamp in scope: */
tain_t         now;

/* status display panel: */
#define  PERP_PANEL  "[- --- ---]"

/* "colorized" perpls capbilities: */
enum cap_id {
   CAP_DF = 0,
   CAP_NA,
   CAP_AN, CAP_AR, CAP_AP, CAP_AD,
   CAP_WU, CAP_WD,
   CAP_ER,
   CAP_NU
};

/* storage for a capability attribute string: */
#define CAP_ATTR_MAX  15

/* colorized perpls capability record: */
struct lscap {
   enum cap_id    cap_id;
   const char    *cap_key;
   char           attr[CAP_ATTR_MAX + 1];
};

/* colorized perpls capability table: */
static struct lscap captab[] = {
   /* note:
    ** C99 designated initializers could/should be used here, eg:
    **
    **     [CAP_DF] = { CAP_DF, "df", "0\0" },
    **     [CAP_NA] = { CAP_NA, "na", "0\0" },
    **     ...
    */

   /* INITIALIZE IN cap_id ORDER: */

   /* default capability:     */ {CAP_DF, "df", "0\0"},
   /* not active:             */ {CAP_NA, "na", "0\0"},
   /* active/normal:          */ {CAP_AN, "an", "0\0"},
   /* active/resetting:       */ {CAP_AR, "ar", "0\0"},
   /* active/paused:          */ {CAP_AP, "ap", "0\0"},
   /* active/down:            */ {CAP_AD, "ad", "0\0"},
   /* want up:                */ {CAP_WU, "wu", "0\0"},
   /* want down:              */ {CAP_WD, "wd", "0\0"},
   /* error:                  */ {CAP_ER, "er", "0\0"},
   /* NULL SENTINAL:          */ {CAP_NU, NULL, "\0"}

   /* note:
    ** the CAP_DF/"df" provides a user-definable default capability
    ** that is, what to use when a capability is not otherwise defined
    */
};

/* LSCAPS: number of items in captab[], _not_ including null sentinal: */
#define LSCAPS ((sizeof captab / sizeof(struct lscap)) - 1)

/* PADMAX: maximum field size for svstat->name: */
#define PADMAX  80

/* svstat: */
struct svstat {
   int            is_active;
   enum cap_id    cap_id;
   char          *name;
   char          *panel;
   pid_t          pid_main;
   uint32_t       uptime_main;
   int            has_log;
   pid_t          pid_log;
   uint32_t       uptime_log;
   int            sys_errno;
   const char    *errmsg;
};


/* stdout macros using ioq: */
#define putv(...) \
        ioq_vputs(ioq1, __VA_ARGS__)
#define puts(...) \
        ioq_vputs(ioq1, __VA_ARGS__, "\n")

/* stderr: */
#define barf(...) \
    { eputs(progname, " barf: ", __VA_ARGS__); die(120); }


/* prototypes in scope: */
static struct svstat *svstat_init(struct svstat *svstat, const char *dir);
static void    svstat_free(struct svstat *svstat);
static const char *cap_lookup(const char *db, const char *key,
                              size_t * attr_len);
static void    captab_init(const char *db);
static void    status_unpack(struct svstat *svstat, const perpstat_t * S);
static void    svstat_fill(struct svstat *svstat);
static const char *name_pad(const char *name, size_t width);

/* qsort comparison functions: */
typedef int    (*cmp_func_t) (const void *, const void *);
static int     cmp_byname(const void *a, const void *b);
static int     cmp_byuptime(const void *a, const void *b);


/*
**  definitions
*/

/* svstat_init()
** svstat constructor/initialization:
** initialize existing svstat, or allocate a new one if NULL
**   strdup(dir) into svstat->name if dir non-NULL
** return:
**   pointer to initialized (possibly new) svstat
**   NULL on allocation failure
*/
static
struct svstat *svstat_init(struct svstat *svstat, const char *svdir)
{
   struct svstat *S = svstat;
   int            new = 0;
   char          *panel = NULL;
   char          *name = NULL;

   if (S == NULL) {
      S = (struct svstat *) malloc(sizeof(struct svstat));
      if (S == NULL)
         return NULL;
      /* else: */
      new = 1;
   }

   panel = cstr_dup(PERP_PANEL);
   if (panel == NULL) {
      goto fail;
   }

   if (svdir != NULL) {
      name = cstr_dup(svdir);
      if (name == NULL)
         goto fail;
   }

   S->is_active = 0;
   S->cap_id = CAP_NA;
   S->name = name;
   S->panel = panel;
   S->pid_main = -1;
   S->uptime_main = 0;
   S->has_log = 0;
   S->pid_log = -1;
   S->uptime_log = 0;
   S->sys_errno = EOK;

   return S;

 fail:
   if (panel)
      free(panel);
   if (name)
      free(name);
   if (new)
      free(S);
   return NULL;
}


/*
** svstat destructor:
*/
static
void svstat_free(struct svstat *st)
{
   if (st) {
      if (st->name)
         free(st->name);
      if (st->panel)
         free(st->panel);
      free(st);
   }

   return;
}


/* cap_lookup()
** search db for capability matching key
** if found:
**     if attr_len is supplied (non-NULL), set to length of attribute
**     return start position of relevant attribute within db
** not found:
**     return NULL
**     no change to attr_len
*/
static
const char    *cap_lookup(const char *db, const char *key, size_t * attr_len)
{
   const char    *d0;
   const char    *attr = NULL;
   size_t         key_len;

   if ((db == NULL) || (key == NULL) || (*key == '\0'))
      return NULL;

   key_len = cstr_len(key);

   d0 = db;
   while (*d0 != '\0') {
      size_t         k = 0;
      const char    *d = d0;

      while ((*d != '\0') && (*d != '='))
         ++d;

      if (*d == '\0')
         /* reached end of db: */
         break;

      /* else: */
      /* save key length of current cap: */
      k = d - d0;
      /* increment beyond '=' : */
      ++d;

      if ((k == key_len) && (cstr_ncmp(key, d0, k) == 0)) {
         /* cap found: */
         /* attribute for cap begins at d: */
         attr = d;
         if (attr_len != NULL) {
            /* get length of attribute: */
            while ((*d != '\0') && (*d != ':'))
               ++d;
            /* setup for return: */
            *attr_len = d - attr;
         }
         /* all done: */
         break;
      }

      /* else: */
      /* cap not found in this loop: */
      /* increment to next key in db: */
      while ((*d != '\0') && (*d != ':'))
         ++d;

      /* increment beyond ':' : */
      if (*d == ':')
         ++d;

      /* if *d=='\0', reached end of db ... */
      d0 = d;
   }

   return attr;
}


/* captab_init()
**   initialize global captab with attributes extracted from db
*/
static
void captab_init(const char *db)
{
   const char    *attr;
   size_t         len = 0;
   int            i;

   /* initialize defined capabilities: */
   for (i = 0; i < LSCAPS; ++i) {
      if (captab[i].cap_id != i) {
         /* internal consistency check: */
         barf("barfing on programmer error in captab_init()");
      }
      attr = cap_lookup(db, captab[i].cap_key, &len);
      if (attr && (len < CAP_ATTR_MAX)) {
         ((char *) buf_copy(captab[i].attr, attr, len))[len] = '\0';
      } else {
         /* fill with default attribute: */
         cstr_copy(captab[i].attr, captab[CAP_DF].attr);
      }
   }

   return;
}


/* status_unpack():
**   fill svstat record from service data found in perpstat
*/
static
void status_unpack(struct svstat *svstat, const perpstat_t * S)
{
   int            flags = S->super.flags;
   int            haslog = super_haslog(flags);

   /* main: */
   svstat->pid_main = S->subsv[SUBSV_MAIN].pid;
   svstat->uptime_main = perp_uptime(&now, &S->subsv[SUBSV_MAIN].when);

   flags = S->subsv[SUBSV_MAIN].flags;
   svstat->panel[3] = subsv_isup(flags) ? '+' : '.';
   svstat->panel[4] = svstat->panel[3];
   svstat->panel[5] = svstat->panel[3];
   if (subsv_iswant(flags))
      svstat->panel[3] = '!';
   if (subsv_isup(flags)) {
      if (subsv_isonce(flags))
         svstat->panel[4] = 'o';
      if (subsv_ispause(flags))
         svstat->panel[5] = 'p';
      if (subsv_isreset(flags))
         svstat->panel[5] = 'r';
      /* munge uptime < 1 sec to wantup: */
      if ((svstat->uptime_main < 1) && !(subsv_isreset(flags))) {
         svstat->pid_main = 0;
         svstat->panel[3] = '!';
         svstat->panel[4] = '.';
         svstat->panel[5] = '.';
      }
   }

   if (!haslog) {
      return;
   }

   /* log: */
   svstat->has_log = 1;
   svstat->pid_log = S->subsv[SUBSV_LOG].pid;
   svstat->uptime_log = perp_uptime(&now, &S->subsv[SUBSV_LOG].when);

   flags = S->subsv[SUBSV_LOG].flags;
   svstat->panel[7] = subsv_isup(flags) ? '+' : '.';
   svstat->panel[8] = svstat->panel[7];
   svstat->panel[9] = svstat->panel[7];
   if (subsv_iswant(flags))
      svstat->panel[7] = '!';
   if (subsv_isup(flags)) {
      if (subsv_isonce(flags))
         svstat->panel[8] = 'o';
      if (subsv_ispause(flags))
         svstat->panel[9] = 'p';
      if (subsv_isreset(flags))
         svstat->panel[9] = 'r';
      /* munge uptime < 1 sec to wantup: */
      if ((svstat->uptime_log < 1) && !(subsv_isreset(flags))) {
         svstat->pid_log = 0;
         svstat->panel[7] = '!';
         svstat->panel[8] = '.';
         svstat->panel[9] = '.';
      }
   }

   return;
}


/* svstat_fill():
**   fill svstat with read of perpetrate server status
*/
static
void svstat_fill(struct svstat *svstat)
{
   const char    *svdir = svstat->name;
   const char    *ctlpath;
   struct stat    st;
   const uchar_t *binstat;
   perpstat_t     perpstat;

   if (stat(svdir, &st) == -1) {
      svstat->panel[1] = 'E';
      svstat->sys_errno = errno;
      svstat->errmsg = "failure stat() on service directory";
      return;
   }

   if (!S_ISDIR(st.st_mode)) {
      svstat->panel[1] = 'E';
      svstat->sys_errno = ENOTDIR;
      svstat->errmsg = "not a directory";
      return;
   }

   if (!(st.st_mode & S_ISVTX)) {
      /* sticky bit not set; service not activated: */
      svstat->panel[1] = '-';
      return;
   }

   /* service considered "active" if sticky bit set: */
   svstat->panel[1] = '+';

   ctlpath = perp_ctlpath(&st);

   /* supervisor running? */
   if (perp_ready(ctlpath) == -1) {
      svstat->panel[1] = 'E';
      svstat->sys_errno = errno;
      if ((errno == ENXIO) || (errno == ENOENT)) {
         svstat->errmsg = "supervisor not running";
      } else {
         svstat->errmsg = "failure checking supervisor";
      }
      return;
   }

   /* get binary-encoded status: */
   binstat = perp_binstat(ctlpath);
   if (binstat == NULL) {
      svstat->panel[1] = 'E';
      svstat->sys_errno = errno;
      svstat->errmsg = "failure reading status";
      return;
   }

   perp_statload(&perpstat, binstat);
   status_unpack(svstat, &perpstat);

   return;
}


/* svstat_setcap()
**   setup svstat->cap_id based on svstat
*/
static
void svstat_setcap(struct svstat *svstat)
{
   char          *panel = svstat->panel;

   switch (panel[1]) {
   case 'E':
      svstat->cap_id = CAP_ER;
      return;
   case '-':
      svstat->cap_id = CAP_NA;
      return;
   case '+':
      svstat->cap_id = CAP_AN;
      break;
   default:
      svstat->cap_id = CAP_ER;
      return;
   }

   if (panel[3] == '!') {
      if (panel[4] == '.')
         svstat->cap_id = CAP_WU;
      else
         svstat->cap_id = CAP_WD;
      return;
   }

   if (panel[7] == '!') {
      if (panel[8] == '.')
         svstat->cap_id = CAP_WU;
      else
         svstat->cap_id = CAP_WD;
      return;
   }

   if (panel[4] == '.') {
      svstat->cap_id = CAP_AD;
      return;
   }

   switch (panel[5]) {
   case 'p':
      svstat->cap_id = CAP_AP;
      return;
   case 'r':
      svstat->cap_id = CAP_AR;
      return;
   case '+':
   default:
      svstat->cap_id = CAP_AN;
   }

   return;
}


static
int cmp_byname(const void *a, const void *b)
{
   /* fun with pointers: */
   struct svstat *sv1 = *(struct svstat **) a;
   struct svstat *sv2 = *(struct svstat **) b;

   return cstr_cmp(sv1->name, sv2->name);
}


static
int cmp_byuptime(const void *a, const void *b)
{
   /* fun with pointers: */
   struct svstat *sv1 = *(struct svstat **) a;
   struct svstat *sv2 = *(struct svstat **) b;

#if 0
   /* note:
    ** we want default sort to be "youngest" first
    **
    ** if tain_less(sv1, sv2) true:
    **    the start time of sv1 was actually farther in the past
    **    ie: sv1 "older/greater" than sv2
    **    ie: return 1
    **
    ** we want:
    **   "older" case to return 1
    **   "younger" case to return -1
    */

   if (tain_less(&sv1->tain_main, &sv2->tain_main))
      return 1;

   if (tain_less(&sv2->tain_main, &sv1->tain_main))
      return -1;
#endif

   if (sv1->uptime_main < sv2->uptime_main)
      return -1;

   if (sv1->uptime_main > sv2->uptime_main)
      return 1;

   /* else: */
   return cstr_cmp(sv1->name, sv2->name);
}


/* name_pad()
**   right pad "name" with whitespace upto width
**   note: return is pointer to internal static buffer
*/
static
const char    *name_pad(const char *name, size_t width)
{
   static char    padbuf[PADMAX + 1];
   const char    *s = name;
   char          *p = &padbuf[0];

   if (width > PADMAX) {
      return NULL;
   }

   while (width && *s) {
      *p = *s;
      ++p;
      ++s;
      --width;
   }

   while (width) {
      *p = ' ';
      ++p;
      --width;
   }

   *p = '\0';

   return (const char *) padbuf;
}


int main(int argc, char *argv[])
{
   char           opt;
   nextopt_t      nopt = nextopt_INIT(argc, argv, ":hVb:cGgKrt");
   const char    *basedir = NULL;
   struct svstat *svstat = NULL;
   struct dynstuf *svstuf = NULL;
   dynstuf_cmp_t  sort_by = NULL;
   const char    *cap_db = NULL;
   int            use_color = 1;
   int            opt_G = 0;    /* explicit want colorized */
   int            opt_K = 0;    /* undocumented show color */
   int            opt_r = 0;    /* reverse display order */
   size_t         width = 0;
   int            i = 0;

   progname = nextopt_progname(&nopt);
   while ((opt = nextopt(&nopt))) {
      char           optc[2] = { (char) nopt.opt_got, '\0' };
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
      case 'c':
         basedir = ".";
         break;
      case 'G':
         opt_G = 1;
         use_color = 1;
         break;
      case 'g':
         opt_G = 0;
         use_color = 0;
         break;
      case 'K':
         opt_K = 1;
         break;
      case 'r':
         opt_r = 1;
         break;
      case 't':
         sort_by = &cmp_byuptime;
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

   /* using color? */
   if (use_color) {
      cap_db = getenv("PERPLS_COLORS");
      switch (opt_G) {
      case 1:
         /* colorized output explicitly requested
          ** use_color even if:
          **   PERPLS_COLORS not defined in environment
          **   isatty(stdout) == 0
          */
         if ((cap_db == NULL) || (cap_db[0] == '\0')) {
            cap_db = PERPLS_COLORS_DEFAULT;
            if (cap_db[0] == '\0') {
               /* not compiled with color support: */
               cap_db = NULL;
               use_color = 0;
            }
         }
         break;
      default:
         /* colorize by default
          ** use_color only if:
          **     PERPLS_COLORS defined in environment
          **     isatty(stdout) == 1
          */
         if ((cap_db == NULL) || (cap_db[0] == '\0') || (isatty(1) == 0)) {
            cap_db = NULL;
            use_color = 0;
         }
      }
   }

   /* initialize captab: */
   if (use_color) {
      captab_init(cap_db);
   }

   /* display color capabilities and exit (undocumented): */
   if (use_color && opt_K) {
      eputs(progname, ": color capabilities: ", cap_db);
      die(0);
   }

   /* initialize svstuf: */
   svstuf = dynstuf_new();
   if (svstuf == NULL) {
      fatal(111, "allocation failure");
   }

   if (basedir == NULL)
      basedir = getenv("PERP_BASE");
   if ((basedir == NULL) || (basedir[0] == '\0'))
      basedir = ".";

   /* all work from basedir: */
   if (chdir(basedir) != 0) {
      fatal_syserr("unable to chdir() to ", basedir);
   }

   /* ignore SIGPIPE: */
   sig_ignore(SIGPIPE);

   if (*argv != NULL) {
      /* initialize svstat list from command line: */
      while (*argv != NULL) {
         svstat = svstat_init(NULL, *argv);
         if (svstat == NULL) {
            fatal(111, "allocation failure");
         }
         if (dynstuf_push(svstuf, svstat) != 0) {
            fatal(111, "allocation failure");
         }
         ++argv;
      }
   } else {
      /* initialize svstat list from current directory: */
      DIR           *dir;
      struct dirent *d;

      if ((dir = opendir(".")) == NULL) {
         fatal_syserr("failure opendir() on base directory ", basedir);
      }

      while ((d = readdir(dir)) != NULL) {
         struct stat    st;

         /* skip entries beginning with '.': */
         if (d->d_name[0] == '.') {
            continue;
         }

         /* skip non-directory entries: */
         if ((stat(d->d_name, &st) != 0) || (!S_ISDIR(st.st_mode))) {
            continue;
         }

         svstat = svstat_init(NULL, d->d_name);
         if (svstat == NULL) {
            fatal(111, "allocation failure");
         }
         if (dynstuf_push(svstuf, svstat) != 0) {
            fatal(111, "allocation failure");
         }
      }

      /* sort directory list by name: */
      if (sort_by == NULL)
         sort_by = &cmp_byname;
   }

   /* uptimes compared to now: */
   tain_now(&now);

   /* svstat_fill() for each svstat, scanning for longest name: */
   for (i = 0; i < dynstuf_ITEMS(svstuf); ++i) {
      size_t         w;
      svstat = (struct svstat *) dynstuf_get(svstuf, i);
      svstat_fill(svstat);
      w = cstr_len(svstat->name);
      if (w > width)
         width = w;
   }

   /* apply sort: */
   if (sort_by != NULL) {
      dynstuf_sort(svstuf, sort_by);
   }

   /* reverse order: */
   if (opt_r == 1) {
      dynstuf_reverse(svstuf);
   }

   /* print results: */
   for (i = 0; i < dynstuf_ITEMS(svstuf); ++i) {
      const char    *panel;
      const char    *name;
      char          *attr = NULL;
      char           up_main[NFMT_SIZE], pid_main[NFMT_SIZE];
      char           up_log[NFMT_SIZE], pid_log[NFMT_SIZE];

      svstat = dynstuf_get(svstuf, i);
      if (use_color) {
         svstat_setcap(svstat);
         attr = captab[svstat->cap_id].attr;
      }
      panel = svstat->panel;
      name = name_pad(svstat->name, width);

      if (use_color && (attr != NULL)) {
         /* colorizing name: */
         putv(panel, "  ", "\033[00m\033[", attr, "m", name, "\033[00m");
      } else {
         putv(panel, "  ", name);
      }

      switch (panel[1]) {
      case 'E':
         puts("  error: ", svstat->errmsg, " [",
              sysstr_errno(svstat->sys_errno), "]");
         break;
      case '+':
         puts("  ",
              /* uptime: */
              "uptime: ", ((panel[4] == '+') || (panel[4] == 'o'))
              ? nfmt_uint32(up_main, svstat->uptime_main)
              : "-", "s/", ((panel[8] == '+') || (panel[8] == 'o'))
              ? nfmt_uint32(up_log, svstat->uptime_log)
              : "-", "s",
              /* pids: */
              "  pids: ", ((panel[4] == '+') || (panel[4] == 'o'))
              ? nfmt_uint32(pid_main, svstat->pid_main)
              : "-", "/", ((panel[8] == '+') || (panel[8] == 'o'))
              ? nfmt_uint32(pid_log, svstat->pid_log)
              : "-");
         break;
      case '-':
         puts("");
         break;
      default:
         puts("  ???");
         break;
      }
   }

   /* flush stdout: */
   ioq_flush(ioq1);
   dynstuf_free(svstuf, (void (*)(void *)) &svstat_free);

   return 0;
}


/* eof (perpls.c) */

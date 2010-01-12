/* perplib.h
** objects and utility functions
** wcm, 2009.11.12 - 2009.11.18
** ===
*/
#ifndef PERPLIB_H
#define PERPLIB_H  1

#include <stddef.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tain.h"
#include "uchar.h"

/* perp system defines and build configuration: */
#include "perp_common.h"


/* binstat: binary-encoded status buffer
**
** block             bytes   type*     comment
** ================  =====   =======   =====================
** binstat[0 .. 3]      4    pid_t     perpetrate supervisor
** binstat[4 .. 15]    12    tain_t    uptime supervisor
** binstat[16]          1    byte      flags supervisor
** binstat[17]          1    byte      reserved
**
** binstat[18 .. 21]    4    pid_t     subservice main
** binstat[22 .. 33]   12    tain_t    uptime main
** binstat[34]          1    byte      flags main
** binstat[35]          1    byte      reserved
**
** binstat[36 .. 39]    4    pid_t     subservice log
** binstat[40 .. 51]   12    tain_t    uptime log
** binstat[52]          1    byte      flags log
** binstat[53]          1    byte      reserved
** -----------------
** 54 bytes total
**
** unsigned integer types packed into binstat[], see upak.h
*/ 

/* size of binary-encoded status buffer: */
#define BINSTAT_SIZE  54
/* binstat_t object: */
typedef uchar_t  binstat_t[BINSTAT_SIZE];

/*
** convenience macros for decoding binstat:
** nomenclature:
**   SUPER: flags for perpetrate supervisor
**   SUBSV: flags for main/log subservice
*/

/* bit definitions within supervisor flags byte: */
#define  SUPER_UP        0x1
#define  SUPER_HASLOG    0x2
#define  SUPER_EXITFLAG  0x80

/* bit definitions within subservice flags byte: */
#define  SUBSV_UP        0x1
#define  SUBSV_RESET     0x2
#define  SUBSV_WANT      0x4
#define  SUBSV_PAUSE     0x8
#define  SUBSV_ONCE      0x40

/* boolean macros for evaluating supervisor flags byte: */
#define super_isup(f) \
    ((f) & SUPER_UP)

#define super_haslog(f) \
    ((f) & SUPER_HASLOG)

#define super_exitflag(f) \
    ((f) & SUPER_EXITFLAG)

/* boolean macros for evaluating a subsv flags byte: */
#define subsv_isup(f) \
    ((f) & SUBSV_UP)

#define subsv_isreset(f) \
    ((f) & SUBSV_RESET)

#define subsv_iswant(f) \
    ((f) & SUBSV_WANT)

#define subsv_ispause(f) \
    ((f) & SUBSV_PAUSE)

#define subsv_isonce(f) \
    ((f) & SUBSV_ONCE)


/* perpstat_t object:
**   status container for supervisor and main/log pair of subservices
**   unpacked from binary-encoded status buffer
**   flags bytes interpreted as within binstat
*/
struct perpstat {
  /* super: perpetrate supervisor process */
  struct {
      uint32_t  pid;
      tain_t    when;
      uchar_t   flags;
  } super;
  /*main/log pair of subservice processes: */
  struct {
      uint32_t  pid;
      tain_t    when;
      uchar_t   flags;
  } subsv[2];
};
typedef struct perpstat  perpstat_t;

/* subsv[] indices in perpstat_t: */
enum subsv_id {
  SUBSV_MAIN = 0,
  SUBSV_LOG  = 1
};
typedef enum subsv_id subsv_id;

/* perp_binstat()
**   reads binary-encoded status from service directory ctlpath
**
**   params:
**     ctlpath: path to service control directory relative to PERP_BASE
**
**   return:
**     pointer to internal static binary-encoded status buffer
**     on failure: NULL, errno set
*/
extern const uchar_t *perp_binstat(const char *ctlpath);

/* perp_ctlpath()
**   return path to service control directory relative to PERP_BASE
**
**   ie: "./" PERP_CONTROL "/" SV_DEVINO
**
**   params:
**     stat: pointer to results of stat() for the svdir
**
**   return:
**     pointer to internal static string buffer 
*/
extern const char *perp_ctlpath(struct stat *st);

/* perp_devino()
**   encode device/inode of a stat() on svdir into dest
**
**   params:
**     dest: pointer to a buffer for the encoded result
**     st:   pointer to results of stat() for the svdir
**
**   return:
**     pointer to dest buffuer 
**
**   note:
**     caller must supply dest buffer of suffient size
**     (24 bytes, including terminating nul)
**     see also #define SV_DEVINO in perp_common.h
*/
extern char *perp_devino(char *dest, struct stat *st);

/* perp_ready()
**   check "readiness" of a perpetrate supervisor
**   test:
**     open() for write succeeds on CTL_IN fifo
**     (fails if an active process does not have fifo open for read)
**
**   params:
**     ctlpath: path to service control directory relative to PERP_BASE
**
**   return:
**     0 : perpetrate supervisor is running and ready
**    -1 : not running or error, errno set
**
**   notes:
**     must be called from within the base directory PERP_BASE
**     test requires adequate user privilege (usually root)
*/
extern int perp_ready(const char *ctlpath);

/* perp_statload()
**   load binary-encoded status into perpstat_t object
**
**   params:
**     S       : pointer to destination perpstat_t object
**     binstat : pointer to binary-encoded status buffer
**
**   return:
**     pointer to S
*/
extern perpstat_t *perp_statload(perpstat_t *S, const uchar_t *binstat);

/* perp_uptime()
**   difference between now and when
**   return seconds
**
**   note:
**     return value cast to 4-byte unsigned integer
**     (uptimes up to 136 years)
*/
extern uint32_t perp_uptime(const tain_t *now, const tain_t *when);


#endif /* PERPLIB_H */
/* eof: perplib.h */

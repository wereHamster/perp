/* eputs.h
** base macro definitions for stderr output
** wcm, 2009.11.30 - 2009.12.19
** ===
*/
#ifndef EPUTS_H
#define EPUTS_H 1

/*
** libasanga: stderr in three(!) flavors
*/

/* eputs()
**   write one or more non-null argument strings to stderr
**
**   notes:
**     appends newline
**     flushed as required for immediate output
**
**   application usage:
**   // select eputs() flavor:
**   #define EPUTS_DEVOUT
**   // include this file:
**   #include "eputs.h"
**   // build additional macros as desired:
**   #define  barf(...)  {eputs("barf: ", __VA_ARGS__); _exit(111); }
*/


/* devout:
**   the most simple, small, ...and inefficient
**   no data copying
**   however: makes one write() syscall for each arg!
**
**   useful and convenient for basic stderr requirements
*/
#ifdef EPUTS_DEVOUT
#ifdef eputs
#  undef eputs
#endif
#include "devout.h"
#define eputs(...) \
  devout(2, __VA_ARGS__, "\n");
#define  EPUTS_DEFINED  1
#endif /* EPUTS_DEVOUT */


/* outvec:
**   provides a "slightly buffered" effect with writev()
**   no data copying
**
**   if the program doesn't require the fully-buffered output capability of ioq,
**   this is probably your best all-around stderr writer
*/
#ifdef EPUTS_OUTVEC
#ifdef eputs
#  undef eputs
#endif
#include "outvec.h"
#define eputs(...) \
  outvec_vputs(&OUTVEC_STDERR, __VA_ARGS__, "\n")
#define  EPUTS_DEFINED  1
#endif /* EPUTS_OUTVEC */


/* ioq:
**   fully buffered output
**   copies args into statically-allocated internal buffer
**
**   use this stderr writer whenever you are already using the ioq module
*/ 
#ifdef EPUTS_IOQ
#ifdef eputs
#  undef eputs
#endif
#include "ioq.h"
#include "ioq_std.h"
#define eputs(...) \
  {ioq_flush(ioq1); ioq_vputs(ioq2, __VA_ARGS__, "\n"); ioq_flush(ioq2); }
#define  EPUTS_DEFINED  1
#endif /* EPUTS_IOQ */


#endif /* EPUTS_H */
/* eof: eputs.h */

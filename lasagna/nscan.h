/* nscan.h
** scan numeric string into numeric value
** wcm, 2008.10.09 - 2009.09.20
** ===
*/
#ifndef NSCAN_H
#define NSCAN_H 1

#include <stddef.h>    /* sizet_t */
#include <stdint.h>


/*
** scan numeric nul-terminated string in s
** return numeric value
** if end is not NULL:
**   on return it points to the address in s following the last
**   valid character scanned into result
**
** snippet:
**
**   const char *e;
**   uint32_t    u = 0;
**
**   u = nscan_uint32(mystr, &e);
**   if(*e != '\0'){
**       printf("bad string input: %s\n", mystr);
**   }
*/

/* nscan_uint32()
**   scan decimal string s
*/
extern uint32_t nscan_uint32(const char *s, const char **end);

/* nscan_uint32o()
**   scan octal string s
*/
extern uint32_t nscan_uint32o(const char *s, const char **end);

#endif /* NSCAN_H */
/* EOF (nscan.h) */

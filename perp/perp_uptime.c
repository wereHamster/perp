/* perp_uptime.c
** wcm, 2009.11.13 - 2009.11.13
** ===
*/
#include <stdint.h>

#include "tain.h"

#include "perplib.h"

/* perp_uptime()
**   difference between now and when
**   return seconds
**
**   note:
**     return value cast to 4-byte integer
**     (uptimes up to 136 years)
*/

uint32_t perp_uptime(const tain_t * now, const tain_t * when)
{
   tain_t         temp;

   /* usually, when happened sometime before now: */
   if (tain_less(when, now))
      tain_minus(&temp, now, when);
   /* but occasionally, when updated since now: */
   else
      tain_minus(&temp, when, now);

   return (uint32_t) temp.sec;
}

/* eof: perp_uptime.c */

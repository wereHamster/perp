/* perp_statload.c
** wcm, 2009.11.13 - 2009.11.13
** ===
*/
#include <stddef.h>

#include "tain.h"
#include "uchar.h"
#include "upak.h"

#include "perp_common.h"
#include "perplib.h"

/* perp_statload()
**   load binary-encoded status into perpstat_t structure
*/

perpstat_t    *perp_statload(perpstat_t * S, const uchar_t * b)
{
   enum subsv_id  subsv;
   size_t         offset;
   int            haslog;

   /*
    ** magic numbers:
    ** see perp_common.h for binstat description
    */

   /* supervisor: */
   S->super.pid = upak32_unpack(&b[0]);
   tain_unpack(&S->super.when, &b[4]);
   haslog = super_haslog((S->super.flags = b[16]));

   /* main and log subservice: */
   for (subsv = SUBSV_MAIN; subsv < 2; ++subsv) {
      if ((subsv == SUBSV_LOG) && !(haslog))
         break;

      offset = (subsv == SUBSV_MAIN) ? 18 : 36;

      S->subsv[subsv].pid = upak32_unpack(&b[offset]);
      tain_unpack(&S->subsv[subsv].when, &b[offset + 4]);
      S->subsv[subsv].flags = b[offset + 16];
   }

   return S;
}

/* eof: perp_statload.c */

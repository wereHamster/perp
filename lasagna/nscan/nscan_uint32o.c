/* nscan_uint32o.c
** scan octal numeric string into numeric value
** wcm, 2008.10.09 - 2009.09.20
** ===
*/
#include <stddef.h>
#include <inttypes.h>

#include "nscan.h"


uint32_t
nscan_uint32o(const char *s, const char **end)
{
  uint32_t  result = 0;
  uint32_t  n = 0;

  while((n = *s - '0') < 8){
      result = result * 8 + n;
      ++s;
  }

  if(end != NULL)
      *end = s;

  return result;
}


/* EOF (nscan_uint32o.c) */

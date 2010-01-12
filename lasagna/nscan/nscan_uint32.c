/* nscan_uint32.c
** scan numeric string into numeric value
** wcm, 2008.10.09 - 2008.10.09
** ===
*/
#include <stddef.h>
#include <inttypes.h>

#include "nscan.h"


uint32_t
nscan_uint32(const char *s, const char **end)
{
  uint32_t  result = 0;
  uint32_t  n = 0;

  while((n = *s - '0') < 10){
      result = result * 10 + n;
      ++s;
  }

  if(end != NULL)
      *end = s;

  return result;
}


/* EOF (nscan_uint32.c) */

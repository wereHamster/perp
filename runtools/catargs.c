/* catargs.c
** output argv in runargs format
** wcm, 2009.09.08 - 2009.09.18
** ===
*/
#include <stdlib.h>
#include <unistd.h>

#include "ioq.h"
#include "ioq_std.h"

int main(int argc, char *argv[])
{
   while (*argv) {
      ioq_vputs(ioq1, *argv, "\n");
      ++argv;
   }
   ioq_flush(ioq1);

   return 0;
}


/* eof: catargs.c */

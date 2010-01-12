/* sig_block.c
** signal operations
** wcm, 2008.01.04 - 2008.01.28
** ===
*/

#include <stdlib.h>
#include <signal.h>

#include "sig.h"


void sig_block(int sig)
{
  sigset_t ss;

  sigemptyset(&ss);
  sigaddset(&ss, sig);
  sigprocmask(SIG_BLOCK, &ss, (sigset_t *) 0);

  return;
}


/* EOF (sig_block.c) */

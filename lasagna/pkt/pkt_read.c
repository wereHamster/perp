/* pkt_read.c
** pkt: tiny packet protocol
** wcm, 2009.07.29 - 2009.07.29
** ===
*/

#include <unistd.h>
#include <errno.h>

#include "uchar.h"
#include "buf.h"
#include "pkt.h"


int
pkt_read(int fd, pkt_t K)
{
    ssize_t  r;
    size_t   n;

    do{
        r = read(fd, K, sizeof(pkt_t));
    }while((r == -1) && (errno == EINTR));

    if(r == -1){
        /* error during read(), errno set */
        return -1;
    }

    if(r < 3){
        /* must read at least 3 header bytes: */ 
        errno = EPROTO;
        return -1;
    }

    n = (size_t)K[2];
    if(r != (n + 3)){
        /* length mismatch (protocol error or short read): */ 
        errno = EPROTO;
        return -1;
    }

    return 0;
}

/* eof: pkg_read.c */

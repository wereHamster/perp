/* pkt_write.c
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
pkt_write(int fd, const pkt_t K)
{
    uchar_t  *b = (uchar_t *)K;
    ssize_t   w;
    size_t    to_write = (size_t)K[2];

    /* writing 3 header bytes: */
    to_write += 3;
    while(to_write > 0){

        do{
            w = write(fd, b, to_write);
        }while((w == -1) && (errno == EINTR));

        if(w == -1)
            return -1;

        if(w == 0)
            continue;

        b += w;
        to_write -= w;
    }

    return 0; 
}


/* eof: pkt_write.c */

/* ioq_putfile.c
** wcm, 2010.01.07 - 2010.06.25
** ===
*/
/* stdlib: */
#include <errno.h>
/* unix: */
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

/* lasagna: */
#include "buf.h"
#include "cstr.h"
#include "uchar.h"

/* ioq module: */
#include "ioq.h"


/* ioq_putfile()
**   append contents of filename to internal buffer
**   calls ioq_flush() as needed to handle all bytes of file
**   return
**     0 : success, no error
**    -1 : open(), stat(), mmap(), or write() error, errno set
**
**   notes:
**     uses mmap()
**     does not perform final ioq_flush()
*/

int
ioq_putfile(ioq_t *ioq, const char *filename)
{
    int           fd = 0;
    struct  stat  st;
    size_t        len = 0;
    void         *map = NULL;
    int           terrno;

    if((fd = open(filename, O_RDONLY)) == -1){
        return -1;
    }

    if(fstat(fd, &st) == -1){
        goto fail;
    }

    len = st.st_size;
    if(len == 0){
        close(fd);
        return 0;
    }

    map = (uchar_t *)mmap(0, len, PROT_READ, MAP_SHARED, fd, 0);
    if(map == MAP_FAILED){
        goto fail;
    }

    if(ioq_put(ioq, (const uchar_t *)map, len) == -1){
        goto fail;
    }

    /* success: */
    munmap(map, len);
    close(fd);
    return 0; 

fail:
    terrno = errno;
    if(fd) close(fd);
    if(map) munmap(map, len);
    errno = terrno;
    return -1;
}


/* eof: ioq_putfile.c */

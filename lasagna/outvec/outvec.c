/* outvec.c
** output using writev()
** wcm, 2008.01.04 - 2009.10.07
** ===
*/

#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/uio.h>

#include "outvec.h"
#include "cstr.h"

/*
** predefine outvec objects for stdout and stderr:
*/

/* slightly buffered stdout (flushme not set): */
#ifdef IOV_MAX
#  define _VEC_STDOUT_SIZE IOV_MAX
#else
#  warning "IOV_MAX not defined; setting _VEC_STDOUT_SIZE 16"
#  define _VEC_STDOUT_SIZE 16
#endif
static struct iovec _VEC_STDOUT[_VEC_STDOUT_SIZE];
struct outvec OUTVEC_STDOUT = outvec_INIT(0, _VEC_STDOUT, _VEC_STDOUT_SIZE, 0);
 
/* unbuffered stderr (flushme set): */ 
#define _VEC_STDERR_SIZE  16
static struct iovec _VEC_STDERR[_VEC_STDERR_SIZE];
struct outvec OUTVEC_STDERR = outvec_INIT(1, _VEC_STDERR, _VEC_STDERR_SIZE, 1);


/*
** outvec methods:
*/

int
outvec_flush(struct outvec *vec)
{
  struct iovec  *v = vec->vec;
  size_t         nvec = vec->n;
  ssize_t        w = 0;
  size_t         i = 0;

  /*
  ** partial writev() handling adapted from algorithm published by
  ** Girish Venkatachalam
  */
  while(i < nvec){
      do{
          w = writev(vec->fd, &v[i], nvec - i);
      }while((w == -1) &&
             ((errno == EINTR) || (errno == EAGAIN)));

      if(w == -1){
          /* error (errno set by writev()): */
          return -1;
      } 

      /* check/handle for partial writes: */
      if(w > 0){
          while((i < nvec) && (w >= v[i].iov_len)){
              w -= v[i].iov_len;
              ++i;
          }
          if(w > 0){
              v[i].iov_base = (char *)v[i].iov_base + w;
              v[i].iov_len -= w;
          }
      }
  }

  /* if here, all flushed: */
  vec->n = 0;
  return 0;
}


int
outvec_puts(struct outvec *vec, const char *s)
{
    if(!(vec->n < vec->max)){
        if(outvec_flush(vec) == -1){
            return -1;
        }
    }

    vec->vec[vec->n].iov_base = (void *)s;
    vec->vec[vec->n].iov_len = cstr_len(s);
    ++vec->n;

    return 0;
}


int
outvec_vputs_(struct outvec *vec, const char *s0, ...)
{
  va_list      ap;
  const char  *s = s0;

  va_start(ap, s0);
  do{
      if(outvec_puts(vec, s) == -1){
          va_end(ap);
          return -1;
      }
      s = va_arg(ap, const char *);
  }while(s != NULL);
  va_end(ap);

  return (vec->flushme ? outvec_flush(vec) : 0);
}      


/* eof: outvec.c */

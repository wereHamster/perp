/* pkt.h
** pkt: tiny packet protocol
** wcm, 2009.07.29 - 2009.11.24
** ===
*/
#ifndef PKT_H
#define PKT_H 1

#include <stddef.h>
#include <stdint.h>

#include "uchar.h"

/*
** concept:
**   pkt_t is an object type designed to implement a simple packet protocol
** 
**   pkt[258]:
**   index          bytes   comment
**   =============  =====   =========================================== 
**   pkt[0]            1    P   protocol id
**   pkt[1]            1    T   packet type id
**   pkt[2]            1    N   size of data payload
**   pkt[3 .. 257]              packet data payload of N bytes (max 255)
**   -------------
**   258 bytes total
**
**   P:  intended to signal the protocol version of pkt 
**   T:  intended to signal a "type" for the data in pkt
**
**   note:
**   pkt only provides the datatype, accessor, & read/write utilities
**   it remains for the application to design the actual protocol
*/

#define PKT_SIZE  258
typedef uchar_t  pkt_t[PKT_SIZE];


/* accessor macros (think "get"): */
#define  pkt_proto(k)  (uchar_t)(k)[0]
#define  pkt_type(k)   (uchar_t)(k)[1]
#define  pkt_size(k)   (uint8_t)(k)[2]
/* get pointer to start of data block within packet: */
#define  pkt_data(k)   (uchar_t *)&(k)[3]

/* assignment macros: */
#define  pkt_setproto(k,P)  (k)[0]=(uchar_t)(P)
#define  pkt_settype(k,T)   (k)[1]=(uchar_t)(T)
#define  pkt_setsize(k,N)   (k)[2]=(uchar_t)(N)

/* initialize packet with protocol, type id, and size (no data): */
#define  pkt_init(k,P,T,N) \
  { pkt_setproto((k),(P)); pkt_settype((k),(T)); pkt_setsize((k),(N)); }

/* static pkt_t variable initialization
** idiom:
**   pkt_t  pkt = pkt_INIT(P, T, N); 
*/ 
#define  pkt_INIT(P,T,N)  {(P), (T), (N)}

/* pkt_load():
**   initialize pkt K with protocol P and type T
**   copy n bytes from buf into K[3] and assign n to K[2]
**
**   return
**     0 : no error
**    -1 : problem with input, errno set
**             ERANGE: n > 255
**
**   note: there are other means of writing into the payload of the
**   pkt_t without breaking the abstract interface; see the notes at
**   the bottom of this file
*/ 
extern int pkt_load(pkt_t K, uchar_t P, uchar_t T, uchar_t *buf, size_t n);

/* pkt_read():
**   from file descriptor fd, read() into pkt K
**   (performs single read() from fd, at most 258 bytes)
**   assumes reading an incoming pkt
**   checks size field (K[2]) of incoming pkt correlates with bytes read
**
**   return
**     0 : no error
**    -1 : error, errno set
**         read() error
**         EPROTO if read() size mismatch with K[2]
*/ 
extern int pkt_read(int fd, pkt_t K); 

/* pkt_write():
**   write() pkt K to file descriptor fd
**   perform successive write() as necessary for K[2]+3 bytes
**
**   return
**     0 : no error
**    -1 : write() error, errno set (some bytes may have been written)
*/
extern int pkt_write(int fd, const pkt_t K);


/* C programming comments
**
** when an array like pkt_t is declared as a parameter to a function,
** the compiler treats the parameter as a pointer to the first element
** of the array
**
** that is, within functions defined with pkt_t array parameters:
**
**   myfunc(pkt_t K, ...){ ... }
**    
** the body of the function sees K as though defined:
**
**   myfunc(uchar_t *K, ...){ ... }
**
** and as though the caller had used:
**
**   caller(void){
**       pkt_t  K;
**       myfunc(&K[0], ...);
**   }
**
**
** we try to avoid the temptation of breaking the interface and
** hacking directly into the pkt_t, eg K[3]
**
** here is an example of reading/writing the data payload of a pkt_t
** without breaking the abstract interface:
**
**     myfunc(pkt_t K, ...){
**         uchar_t  *data = pkt_data(K);
**         if((pkt_type(K) == 'T') && (data[0] == 'X')) ...
**         ...
**    }
**
*/

#endif /* PKT_H */
/* eof: pkt.h */

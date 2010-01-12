# Makefile
# makfile for libasagna
# wcm, 2008.01.04 - 2010.01.07
# ===

include ../conf.mk
CFLAGS += -I.

all: libasagna.a

## libasagna modules:
LIB_OBJS=\
 $(BUF_OBJS) \
 $(CSTR_OBJS) \
 $(DEVOUT_OBJS) \
 $(DYNBUF_OBJS) \
 $(DYNSTR_OBJS) \
 $(DYNSTUF_OBJS) \
 $(EXECVX_OBJS) \
 $(FD_OBJS) \
 $(IOQ_OBJS) \
 $(NEWENV_OBJS) \
 $(NEXTOPT_OBJS) \
 $(NFMT_OBJS) \
 $(NSCAN_OBJS) \
 $(OUTVEC_OBJS) \
 $(PACKET_OBJS) \
 $(PADLOCK_OBJS) \
 $(PIDLOCK_OBJS) \
 $(PKT_OBJS) \
 $(RLIMIT_OBJS) \
 $(SIG_OBJS) \
 $(SYSSTR_OBJS) \
 $(TAIN_OBJS) \
 $(TX64_OBJS) \
 $(UPAK_OBJS) \


##
## buf:
##
BUF_OBJS=\
 buf_cmp.o \
 buf_copy.o \
 buf_fill.o \
 buf_ndx.o \
 buf_rcopy.o \
 buf_rndx.o \

buf_cmp.o : buf/buf_cmp.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_cmp.c

buf_copy.o : buf/buf_copy.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_copy.c

buf_fill.o : buf/buf_fill.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_fill.c

buf_ndx.o : buf/buf_ndx.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_ndx.c

buf_rcopy.o : buf/buf_rcopy.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_rcopy.c

buf_rndx.o : buf/buf_rndx.c buf.h
	$(CC) $(CFLAGS) -c buf/buf_rndx.c


##
## cstr:
##
CSTR_OBJS=\
 cstr_cat.o \
 cstr_cmp.o \
 cstr_cmpi.o \
 cstr_contains.o \
 cstr_copy.o \
 cstr_dup.o \
 cstr_lcat.o \
 cstr_lcpy.o \
 cstr_len.o \
 cstr_ltrim.o \
 cstr_ncmp.o \
 cstr_pos.o \
 cstr_rpos.o \
 cstr_rtrim.o \
 cstr_trim.o \
 cstr_vcat_.o \
 cstr_vcopy_.o \
 cstr_vlen_.o \

cstr_cat.o : cstr/cstr_cat.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_cat.c

cstr_cmp.o : cstr/cstr_cmp.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_cmp.c

cstr_cmpi.o : cstr/cstr_cmpi.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_cmpi.c

cstr_contains.o : cstr/cstr_contains.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_contains.c

cstr_copy.o : cstr/cstr_copy.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_copy.c

cstr_dup.o : cstr/cstr_dup.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_dup.c

cstr_lcat.o : cstr/cstr_lcat.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_lcat.c

cstr_lcpy.o : cstr/cstr_lcpy.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_lcpy.c

cstr_len.o : cstr/cstr_len.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_len.c

cstr_ltrim.o : cstr/cstr_ltrim.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_ltrim.c

cstr_ncmp.o : cstr/cstr_ncmp.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_ncmp.c

cstr_pos.o : cstr/cstr_pos.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_pos.c

cstr_rpos.o : cstr/cstr_rpos.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_rpos.c

cstr_rtrim.o : cstr/cstr_rtrim.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_rtrim.c

cstr_trim.o : cstr/cstr_trim.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_trim.c

cstr_vcat_.o : cstr/cstr_vcat_.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_vcat_.c

cstr_vcopy_.o : cstr/cstr_vcopy_.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_vcopy_.c

cstr_vlen_.o : cstr/cstr_vlen_.c cstr.h
	$(CC) $(CFLAGS) -c cstr/cstr_vlen_.c


##
## devout:
##
DEVOUT_OBJS=\
  devout.o \

devout.o : devout/devout.c devout.h
	$(CC) $(CFLAGS) -c devout/devout.c


##
## dynbuf:
##
DYNBUF_OBJS=\
 dynbuf.o \
 dynbuf_clear.o \
 dynbuf_copy.o \
 dynbuf_free.o \
 dynbuf_freebuf.o \
 dynbuf_grow.o \
 dynbuf_need.o \
 dynbuf_new.o \
 dynbuf_put.o \
 
dynbuf.o : dynbuf/dynbuf.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf.c

dynbuf_clear.o : dynbuf/dynbuf_clear.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_clear.c

dynbuf_copy.o : dynbuf/dynbuf_copy.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_copy.c

dynbuf_free.o : dynbuf/dynbuf_free.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_free.c

dynbuf_freebuf.o : dynbuf/dynbuf_freebuf.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_freebuf.c

dynbuf_grow.o : dynbuf/dynbuf_grow.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_grow.c

dynbuf_need.o : dynbuf/dynbuf_need.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_need.c

dynbuf_new.o : dynbuf/dynbuf_new.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_new.c

dynbuf_put.o : dynbuf/dynbuf_put.c dynbuf.h
	$(CC) $(CFLAGS) -c dynbuf/dynbuf_put.c


##
## dynstr:
##
DYNSTR_OBJS=\
 dynstr.o \
 dynstr_clear.o \
 dynstr_copy.o \
 dynstr_copys.o \
 dynstr_free.o \
 dynstr_freestr.o \
 dynstr_grow.o \
 dynstr_need.o \
 dynstr_new.o \
 dynstr_put.o \
 dynstr_putc.o \
 dynstr_putn.o \
 dynstr_puts.o \
 dynstr_set.o \
 dynstr_vputs_.o \

dynstr.o : dynstr/dynstr.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr.c

dynstr_clear.o : dynstr/dynstr_clear.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_clear.c

dynstr_copy.o : dynstr/dynstr_copy.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_copy.c

dynstr_copys.o : dynstr/dynstr_copys.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_copys.c

dynstr_free.o : dynstr/dynstr_free.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_free.c

dynstr_freestr.o : dynstr/dynstr_freestr.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_freestr.c

dynstr_grow.o : dynstr/dynstr_grow.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_grow.c

dynstr_need.o : dynstr/dynstr_need.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_need.c

dynstr_new.o : dynstr/dynstr_new.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_new.c

dynstr_put.o : dynstr/dynstr_put.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_put.c

dynstr_putc.o : dynstr/dynstr_putc.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_putc.c

dynstr_putn.o : dynstr/dynstr_putn.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_putn.c

dynstr_puts.o : dynstr/dynstr_puts.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_puts.c

dynstr_set.o : dynstr/dynstr_set.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_set.c

dynstr_vputs_.o : dynstr/dynstr_vputs_.c dynstr.h
	$(CC) $(CFLAGS) -c dynstr/dynstr_vputs_.c


##
## dynstuf:
##
DYNSTUF_OBJS=\
 dynstuf_find.o \
 dynstuf_free.o \
 dynstuf_get.o \
 dynstuf_grow.o \
 dynstuf_init.o \
 dynstuf_isempty.o \
 dynstuf_items.o \
 dynstuf_new.o \
 dynstuf_peek.o \
 dynstuf_pop.o \
 dynstuf_push.o \
 dynstuf_replace.o \
 dynstuf_reverse.o \
 dynstuf_set.o \
 dynstuf_slots.o \
 dynstuf_sort.o \
 dynstuf_stuf.o \
 dynstuf_visit.o \
 
dynstuf_find.o : dynstuf/dynstuf_find.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_find.c

dynstuf_free.o : dynstuf/dynstuf_free.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_free.c

dynstuf_get.o : dynstuf/dynstuf_get.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_get.c

dynstuf_grow.o : dynstuf/dynstuf_grow.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_grow.c

dynstuf_init.o : dynstuf/dynstuf_init.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_init.c

dynstuf_isempty.o : dynstuf/dynstuf_isempty.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_isempty.c

dynstuf_items.o : dynstuf/dynstuf_items.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_items.c

dynstuf_new.o : dynstuf/dynstuf_new.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_new.c

dynstuf_peek.o : dynstuf/dynstuf_peek.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_peek.c

dynstuf_pop.o : dynstuf/dynstuf_pop.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_pop.c

dynstuf_push.o : dynstuf/dynstuf_push.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_push.c

dynstuf_replace.o : dynstuf/dynstuf_replace.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_replace.c
 
dynstuf_reverse.o : dynstuf/dynstuf_reverse.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_reverse.c

dynstuf_set.o : dynstuf/dynstuf_set.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_set.c

dynstuf_slots.o : dynstuf/dynstuf_slots.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_slots.c

dynstuf_sort.o : dynstuf/dynstuf_sort.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_sort.c

dynstuf_stuf.o : dynstuf/dynstuf_stuf.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_stuf.c

dynstuf_visit.o : dynstuf/dynstuf_visit.c dynstuf.h
	$(CC) $(CFLAGS) -c dynstuf/dynstuf_visit.c


##
## execvx:
##
EXECVX_OBJS=\
 execvx.o \

execvx.o : execvx/execvx.c execvx.h
	$(CC) $(CFLAGS) -c execvx/execvx.c


##
## fd:
##
FD_OBJS=\
 fd_blocking.o \
 fd_cloexec.o \
 fd_dupe.o \
 fd_move.o \
 fd_nonblock.o \

fd_blocking.o : fd/fd_blocking.c fd.h
	$(CC) $(CFLAGS) -c fd/fd_blocking.c

fd_cloexec.o : fd/fd_cloexec.c fd.h
	$(CC) $(CFLAGS) -c fd/fd_cloexec.c

fd_dupe.o : fd/fd_dupe.c fd.h
	$(CC) $(CFLAGS) -c fd/fd_dupe.c

fd_move.o : fd/fd_move.c fd.h
	$(CC) $(CFLAGS) -c fd/fd_move.c

fd_nonblock.o : fd/fd_nonblock.c fd.h
	$(CC) $(CFLAGS) -c fd/fd_nonblock.c


##
## ioq:
##
IOQ_OBJS=\
  ioq_get.o \
  ioq_getln.o \
  ioq_init.o \
  ioq_put.o \
  ioq_putfile.o \
  ioq_stderr.o \
  ioq_stdin.o \
  ioq_stdout.o \
  ioq_vputs_.o

ioq_get.o : ioq/ioq_get.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_get.c

ioq_getln.o : ioq/ioq_getln.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_getln.c

ioq_init.o : ioq/ioq_init.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_init.c

ioq_put.o : ioq/ioq_put.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_put.c

ioq_putfile.o : ioq/ioq_putfile.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_putfile.c

ioq_stderr.o : ioq/ioq_stderr.c ioq.h ioq_std.h
	$(CC) $(CFLAGS) -c ioq/ioq_stderr.c

ioq_stdin.o : ioq/ioq_stdin.c ioq.h ioq_std.h
	$(CC) $(CFLAGS) -c ioq/ioq_stdin.c

ioq_stdout.o : ioq/ioq_stdout.c ioq.h ioq_std.h
	$(CC) $(CFLAGS) -c ioq/ioq_stdout.c

ioq_vputs_.o : ioq/ioq_vputs_.c ioq.h
	$(CC) $(CFLAGS) -c ioq/ioq_vputs_.c

##
## newenv:
##
NEWENV_OBJS=\
  newenv.o \

newenv.o : newenv/newenv.c newenv.h
	$(CC) $(CFLAGS) -c newenv/newenv.c

##
## nextopt:
##
NEXTOPT_OBJS=\
  nextopt.o \
  nextopt_progname.o \

nextopt.o : nextopt/nextopt.c nextopt.h
	$(CC) $(CFLAGS) -c nextopt/nextopt.c

nextopt_progname.o : nextopt/nextopt_progname.c nextopt.h
	$(CC) $(CFLAGS) -c nextopt/nextopt_progname.c


##
## nfmt:
##
NFMT_OBJS=\
 nfmt_uint32.o \
 nfmt_uint32_pad0.o \
 nfmt_uint64.o \
 nfmt_uint64_pad0.o \

nfmt_uint32.o : nfmt/nfmt_uint32.c nfmt.h
	$(CC) $(CFLAGS) -c nfmt/nfmt_uint32.c

nfmt_uint32_pad0.o : nfmt/nfmt_uint32_pad0.c nfmt.h
	$(CC) $(CFLAGS) -c nfmt/nfmt_uint32_pad0.c

nfmt_uint64.o : nfmt/nfmt_uint64.c nfmt.h
	$(CC) $(CFLAGS) -c nfmt/nfmt_uint64.c

nfmt_uint64_pad0.o : nfmt/nfmt_uint64_pad0.c nfmt.h
	$(CC) $(CFLAGS) -c nfmt/nfmt_uint64_pad0.c


##
## nscan:
##
NSCAN_OBJS=\
 nscan_uint32.o \
 nscan_uint32o.o \
 
nscan_uint32.o : nscan/nscan_uint32.c nscan.h
	$(CC) $(CFLAGS) -c nscan/nscan_uint32.c

nscan_uint32o.o : nscan/nscan_uint32o.c nscan.h
	$(CC) $(CFLAGS) -c nscan/nscan_uint32o.c


##
## outvec:
##
OUTVEC_OBJS=\
 outvec.o \

outvec.o : outvec/outvec.c outvec.h
	$(CC) $(CFLAGS) -c outvec/outvec.c


##
## packet:
##
PACKET_OBJS=\
 packet_read.o \
 packet_write.o \

packet_read.o : packet/packet_read.c packet.h
	$(CC) $(CFLAGS) -c packet/packet_read.c

packet_write.o : packet/packet_write.c packet.h
	$(CC) $(CFLAGS) -c packet/packet_write.c


##
## padlock:
##
PADLOCK_OBJS=\
 padlock_exbyte.o \
 padlock_exlock.o \
 padlock_extest.o \
 padlock_fcntl.o \
 padlock_shbyte.o \
 padlock_shlock.o \
 padlock_shtest.o \
 padlock_unbyte.o \
 padlock_unlock.o \


padlock_exbyte.o : padlock/padlock_exbyte.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_exbyte.c

padlock_exlock.o : padlock/padlock_exlock.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_exlock.c

padlock_extest.o : padlock/padlock_extest.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_extest.c

padlock_fcntl.o : padlock/padlock_fcntl.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_fcntl.c

padlock_shbyte.o : padlock/padlock_shbyte.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_shbyte.c

padlock_shlock.o : padlock/padlock_shlock.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_shlock.c

padlock_shtest.o : padlock/padlock_shtest.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_shtest.c

padlock_unbyte.o : padlock/padlock_unbyte.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_unbyte.c

padlock_unlock.o : padlock/padlock_unlock.c padlock.h
	$(CC) $(CFLAGS) -c padlock/padlock_unlock.c


##
## pidlock:
##
PIDLOCK_OBJS=\
  pidlock_check.o \
  pidlock_set.o \

pidlock_check.o : pidlock/pidlock_check.c pidlock.h
	$(CC) $(CFLAGS) -c pidlock/pidlock_check.c

pidlock_set.o : pidlock/pidlock_set.c pidlock.h
	$(CC) $(CFLAGS) -c pidlock/pidlock_set.c



##
## pkt:
##
PKT_OBJS=\
 pkt_load.o \
 pkt_read.o \
 pkt_write.o\

pkt_load.o : pkt/pkt_load.c pkt.h
	$(CC) $(CFLAGS) -c pkt/pkt_load.c

pkt_read.o : pkt/pkt_read.c pkt.h
	$(CC) $(CFLAGS) -c pkt/pkt_read.c

pkt_write.o : pkt/pkt_write.c pkt.h
	$(CC) $(CFLAGS) -c pkt/pkt_write.c

##
## rlimit:
##
RLIMIT_OBJS=\
 rlimit.o \

## special builds for rlimit:
## mk_rlimit (rlimit_defs generator):
rlimit/mk_rlimit : rlimit/mk_rlimit.c
	$(CC) $(CFLAGS) -o $@ rlimit/mk_rlimit.c

## generate rlimit_defs table:
rlimit/rlimit_defs.c.in : rlimit/mk_rlimit
	./rlimit/mk_rlimit > rlimit/rlimit_defs.c.in

rlimit.o : rlimit/rlimit.c \
  rlimit/rlimit_defs.c.in \
  rlimit/rlimit_private.h \
  rlimit.h
	$(CC) $(CFLAGS) -c rlimit/rlimit.c


##
## sig:
##
SIG_OBJS=\
 sig_block.o \
 sig_catch.o \
 sig_catchr.o \
 sig_unblock.o \

sig_block.o : sig/sig_block.c sig.h
	$(CC) $(CFLAGS) -c sig/sig_block.c

sig_catch.o : sig/sig_catch.c sig.h
	$(CC) $(CFLAGS) -c sig/sig_catch.c

sig_catchr.o : sig/sig_catchr.c sig.h
	$(CC) $(CFLAGS) -c sig/sig_catchr.c

sig_unblock.o : sig/sig_unblock.c sig.h
	$(CC) $(CFLAGS) -c sig/sig_unblock.c


##
## sigset:
##

# note:
#   no object files for sigset
#   sigset consists only of the header file "sigset.h"


##
## sysstr:
##
SYSSTR_OBJS=\
 sysstr_errno.o \
 sysstr_signal.o \

## special builds for sysstr:
## mysysstr sysno table generator:
sysstr/mksysstr : sysstr/mksysstr.c
	$(CC) $(CFLAGS) -o $@ sysstr/mksysstr.c

## generate errno table:
sysstr/sysstr_errno.c.in : sysstr/mksysstr sysstr.h
	./sysstr/mksysstr errno > $@

## generate signal table:
sysstr/sysstr_signo.c.in : sysstr/mksysstr sysstr.h
	./sysstr/mksysstr signo > $@

sysstr_errno.o : sysstr/sysstr_errno.c \
  sysstr/sysno_lookup.c.in \
  sysstr/sysstr_errno.c.in \
  sysstr/sysstr_private.h \
  sysstr.h
	$(CC) $(CFLAGS) -c sysstr/sysstr_errno.c

sysstr_signal.o : sysstr/sysstr_signal.c \
  sysstr/sysno_lookup.c.in \
  sysstr/sysstr_signo.c.in \
  sysstr/sysstr_private.h \
  sysstr.h
	$(CC) $(CFLAGS) -c sysstr/sysstr_signal.c


##
## tain:
##
TAIN_OBJS=\
 tain_assign.o \
 tain_iszero.o \
 tain_less.o \
 tain_load.o \
 tain_load_msecs.o \
 tain_load_utc.o \
 tain_minus.o \
 tain_now.o \
 tain_pack.o \
 tain_packhex.o \
 tain_pause.o \
 tain_plus.o \
 tain_tai_pack.o \
 tain_tai_unpack.o \
 tain_to_float.o \
 tain_to_msecs.o \
 tain_to_utc.o \
 tain_unpack.o \
 tain_unpackhex.o \

tain_assign.o : tain/tain_assign.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_assign.c

tain_iszero.o : tain/tain_iszero.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_iszero.c

tain_less.o : tain/tain_less.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_less.c

tain_load.o : tain/tain_load.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_load.c

tain_load_msecs.o : tain/tain_load_msecs.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_load_msecs.c

tain_load_utc.o : tain/tain_load_utc.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_load_utc.c

tain_minus.o : tain/tain_minus.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_minus.c

tain_now.o : tain/tain_now.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_now.c

tain_pack.o : tain/tain_pack.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_pack.c

tain_packhex.o : tain/tain_packhex.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_packhex.c

tain_pause.o : tain/tain_pause.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_pause.c

tain_plus.o : tain/tain_plus.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_plus.c

tain_tai_pack.o : tain/tain_tai_pack.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_tai_pack.c

tain_tai_unpack.o : tain/tain_tai_unpack.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_tai_unpack.c

tain_to_float.o : tain/tain_to_float.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_to_float.c

tain_to_msecs.o : tain/tain_to_msecs.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_to_msecs.c

tain_to_utc.o : tain/tain_to_utc.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_to_utc.c

tain_unpack.o : tain/tain_unpack.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_unpack.c

tain_unpackhex.o : tain/tain_unpackhex.c tain.h
	$(CC) $(CFLAGS) -c tain/tain_unpackhex.c

##
## tx64:
##
TX64_OBJS=\
 base64_vec.o \
 tx64_encode.o \

base64_vec.o : tx64/base64_vec.c tx64.h
	$(CC) $(CFLAGS) -c tx64/base64_vec.c

tx64_encode.o : tx64/tx64_encode.c tx64.h
	$(CC) $(CFLAGS) -c tx64/tx64_encode.c

 
##
## upak:
##
UPAK_OBJS=\
 upak.o \
 upak16.o \
 upak32.o \
 upak64.o \

upak.o : upak/upak.c upak.h
	$(CC) $(CFLAGS) -c upak/upak.c

upak16.o : upak/upak16.c upak.h
	$(CC) $(CFLAGS) -c upak/upak16.c

upak32.o : upak/upak32.c upak.h
	$(CC) $(CFLAGS) -c upak/upak32.c

upak64.o : upak/upak64.c upak.h
	$(CC) $(CFLAGS) -c upak/upak64.c


## libasagna:
libasagna.a: $(LIB_OBJS)
	ar cr $@ $(LIB_OBJS)
	ranlib $@

## misc targets:
TARGETS: Makefile
	grep '^[a-z].*:' Makefile | cut -d':' -f1 >TARGETS

clean: TARGETS
	rm -f `cat TARGETS` TARGETS

### EOF (Makefile)

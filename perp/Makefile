# Makefile
# makfile for project perp:
# a persistent process supervisor and utility suite
# wcm, 2008.01.04 - 2009.12.28
# ===

# build configuration:
include ../conf.mk
CFLAGS += -I. -I../lasagna
LDFLAGS += -L../lasagna -lasagna

PERPAPPS = \
  perpboot \
  perpctl \
  perpd \
  perpetrate \
  perphup \
  perpls \
  perpstat \
  perpok \
  sissylog \
  tinylog \

## default target:
all: $(PERPAPPS) perp-setup


PERP_HEADERS = \
  eputs.h \
  perp_common.h \
  perplib.h \

## perpapps:
perpboot: perpboot.o
	$(CC) $(CFLAGS) -o $@ perpboot.o -L. $(LDFLAGS)

perpboot.o: perpboot.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perpboot.c

perpctl: perpctl.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpctl.o -L. -lperp $(LDFLAGS)

perpctl.o: perpctl.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perpctl.c

perpd: perpd.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpd.o -L. -lperp $(LDFLAGS)

perpd.o: perpd.c $(PERP_HEADERS) loggit.h
	$(CC) $(CFLAGS) -c perpd.c

perpetrate: perpetrate.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpetrate.o -L. -lperp $(LDFLAGS)

perpetrate.o: perpetrate.c $(PERP_HEADERS) loggit.h
	$(CC) $(CFLAGS) -c perpetrate.c

perphup: perphup.o
	$(CC) $(CFLAGS) -o $@ perphup.o -L. $(LDFLAGS)

perphup.o: perphup.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perphup.c

perpls: perpls.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpls.o -L. -lperp $(LDFLAGS)

perpls.o: perpls.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perpls.c

perpok: perpok.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpok.o -L. -lperp $(LDFLAGS)

perpok.o: perpok.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perpok.c

perpstat: perpstat.o libperp.a
	$(CC) $(CFLAGS) -o $@ perpstat.o -L. -lperp $(LDFLAGS)

perpstat.o: perpstat.c $(PERP_HEADERS)
	$(CC) $(CFLAGS) -c perpstat.c

sissylog.o: sissylog.c sissylog.h
	$(CC) $(CFLAGS) -c sissylog.c

sissylog: sissylog.o
	$(CC) $(CFLAGS) -o $@ sissylog.o  $(LDFLAGS)

tinylog.o: tinylog.c tinylog.h loggit.h
	$(CC) $(CFLAGS) -c tinylog.c

tinylog: tinylog.o
	$(CC) $(CFLAGS) -o $@ tinylog.o  $(LDFLAGS)


## perp-setup (not in PERPAPPS):
perp-setup: perp-setup.in
	cat perp-setup.in | sed -e 's}@@SBIN@@}$(SBIN)}g' > $@
	chmod 0755 $@


##
## libperp:
##
LIBPERP_OBJS=\
 perp_binstat.o \
 perp_ctlpath.o \
 perp_devino.o \
 perp_ready.o \
 perp_statload.o \
 perp_uptime.o \

perp_binstat.o: perp_binstat.c perp_common.h perplib.h
	$(CC) $(CFLAGS) -c perp_binstat.c

perp_ctlpath.o: perp_ctlpath.c perp_common.h perplib.h
	$(CC) $(CFLAGS) -c perp_ctlpath.c

perp_devino.o: perp_devino.c perp_common.h perplib.h
	$(CC) $(CFLAGS) -c perp_devino.c

perp_ready.o: perp_ready.c perp_common.h perplib.h
	$(CC) $(CFLAGS) -c perp_ready.c

perp_statload.o: perp_statload.c perp_common.h perplib.h
	$(CC) $(CFLAGS) -c perp_statload.c

perp_uptime.o: perp_uptime.c perplib.h
	$(CC) $(CFLAGS) -c perp_uptime.c


libperp.a: $(LIBPERP_OBJS) perplib.h
	ar cr $@ $(LIBPERP_OBJS)
	ranlib $@


## install targets:
install-man:
	install -d -m 0755 $(DESTDIR)$(MAN)/man5
	@for f in ./man/*.5 ; do\
           echo "install -m 0644 $${f} $(DESTDIR)$(MAN)/man5";\
           install -m 0644 $${f} $(DESTDIR)$(MAN)/man5 ;\
        done
	install -d -m 0755 $(DESTDIR)$(MAN)/man8
	@for f in ./man/*.8 ; do\
           echo "install -m 0644 $${f} $(DESTDIR)$(MAN)/man8";\
           install -m 0644 $${f} $(DESTDIR)$(MAN)/man8 ;\
        done

install-sbin: $(PERPAPPS) perp-setup tinylog_run
	install -d -m 0755  $(DESTDIR)$(SBIN)
	@for f in $(PERPAPPS) perp-setup tinylog_run ; do\
           echo "install -m 0755 $${f} $(DESTDIR)$(SBIN)";\
           install -m 0755 $${f} $(DESTDIR)$(SBIN);\
         done

install: install-sbin install-man

## XXX, deprecated:
## use perp-setup post-installer configuration instead
install-perpboot: install
	install -d -m 0755 $(DESTDIR)$(ETC)/perp
	install -d -m 0755 $(DESTDIR)$(ETC)/perp/.boot
	@for f in rc.log rc.perp ; do\
	  if test ! -f $(DESTDIR)$(ETC)/perp/.boot/$${f} ; then\
	      echo "install -m 0755 boot/$${f} $(DESTDIR)$(ETC)/perp/.boot/$${f}";\
	      install -m 0755 boot/$${f} $(DESTDIR)$(ETC)/perp/.boot/$${f};\
	  else\
	      echo "install -m 0644 boot/$${f} $(DESTDIR)$(ETC)/perp/.boot/$${f}.new";\
	      install -m 0644 boot/$${f} $(DESTDIR)$(ETC)/perp/.boot/$${f}.new;\
	  fi\
	done
	@if ! test -d $(DESTDIR)$(ETC)/perp/.control -o -h $(DESTDIR)$(ETC)/perp/.control ; then\
	  echo "ln -fs /var/run/perp $(DESTDIR)$(ETC)/perp/.control";\
	  ln -fs /var/run/perp $(DESTDIR)$(ETC)/perp/.control;\
	fi


## misc targets:

TARGETS: Makefile
	grep '^[a-z].*:' Makefile | cut -d':' -f1 >TARGETS

clean: TARGETS
	rm -f `cat TARGETS` TARGETS

strip: $(PERPAPPS)
	strip $(PERPAPPS)


.PHONY: all clean install install-man install-sbin install-perpboot strip

### EOF (Makefile)

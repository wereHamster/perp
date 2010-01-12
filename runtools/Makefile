# Makefile
# makfile for runtools:
# utilities for perpetrate runscripts
# wcm, 2008.01.04 - 2009.12.20
# ===

# build configuration:
include ../conf.mk
CFLAGS += -I../lasagna
LDFLAGS += -L../lasagna -lasagna

RUNTOOLS = \
  runargv0 \
  runargs \
  rundetach \
  rundeux \
  runenv \
  runfile \
  runlimit \
  runlock \
  runpause \
  runsession \
  runtrap \
  runtool \
  runuid \

CATTOOLS = \
  catargs \
  catenv \
  catlimits \
  catuid \

## default target:
all: $(RUNTOOLS) $(CATTOOLS)

## runtools:
runargs.o: runargs.c runtools_common.h
	$(CC) $(CFLAGS) -c runargs.c

runargs: runargs.o
	$(CC) $(CFLAGS) -o $@ runargs.o $(LDFLAGS)

runargv0.o: runargv0.c runtools_common.h
	$(CC) $(CFLAGS) -c runargv0.c

runargv0: runargv0.o
	$(CC) $(CFLAGS) -o $@ runargv0.o $(LDFLAGS)

rundetach.o: rundetach.c runtools_common.h
	$(CC) $(CFLAGS) -c rundetach.c

rundetach: rundetach.o
	$(CC) $(CFLAGS) -o $@ rundetach.o $(LDFLAGS)

rundeux.o: rundeux.c runtools_common.h
	$(CC) $(CFLAGS) -c rundeux.c

rundeux: rundeux.o
	$(CC) $(CFLAGS) -o $@ rundeux.o $(LDFLAGS)

runenv.o: runenv.c runtools_common.h
	$(CC) $(CFLAGS) -c runenv.c

runenv: runenv.o
	$(CC) $(CFLAGS) -o $@ runenv.o $(LDFLAGS)

runfile.o: runfile.c runtools_common.h
	$(CC) $(CFLAGS) -c runfile.c

runfile: runfile.o
	$(CC) $(CFLAGS) -o $@ runfile.o $(LDFLAGS)

runlimit.o: runlimit.c runtools_common.h
	$(CC) $(CFLAGS) -c runlimit.c

runlimit: runlimit.o
	$(CC) $(CFLAGS) -o $@ runlimit.o $(LDFLAGS)

runlock.o: runlock.c runtools_common.h
	$(CC) $(CFLAGS) -c runlock.c

runlock: runlock.o
	$(CC) $(CFLAGS) -o $@ runlock.o $(LDFLAGS)

runpause.o: runpause.c runtools_common.h
	$(CC) $(CFLAGS) -c runpause.c

runpause: runpause.o
	$(CC) $(CFLAGS) -o $@ runpause.o $(LDFLAGS)

runsession.o: runsession.c runtools_common.h
	$(CC) $(CFLAGS) -c runsession.c

runsession: runsession.o
	$(CC) $(CFLAGS) -o $@ runsession.o $(LDFLAGS)

runtool.o: runtool.c runtools_common.h
	$(CC) $(CFLAGS) -c runtool.c

runtool: runtool.o
	$(CC) $(CFLAGS) -o $@ runtool.o $(LDFLAGS)

runtrap: runtrap.o
	$(CC) $(CFLAGS) -o $@ runtrap.o $(LDFLAGS)

runtrap.o: runtrap.c runtools_common.h
	$(CC) $(CFLAGS) -c runtrap.c

runuid: runuid.o
	$(CC) $(CFLAGS) -o $@ runuid.o $(LDFLAGS)

runuid.o: runuid.c runtools_common.h
	$(CC) $(CFLAGS) -c runuid.c

## cattools:
catargs: catargs.o
	$(CC) $(CFLAGS) -o $@ catargs.o $(LDFLAGS)

catargs.o: catargs.c runtools_common.h
	$(CC) $(CFLAGS) -c catargs.c

catenv: catenv.o
	$(CC) $(CFLAGS) -o $@ catenv.o $(LDFLAGS)

catenv.o: catenv.c runtools_common.h
	$(CC) $(CFLAGS) -c catenv.c

catlimits: catlimits.o
	$(CC) $(CFLAGS) -o $@ catlimits.o $(LDFLAGS)

catlimits.o: catlimits.c runtools_common.h
	$(CC) $(CFLAGS) -c catlimits.c

catuid: catuid.o
	$(CC) $(CFLAGS) -o $@ catuid.o $(LDFLAGS)

catuid.o: catuid.c runtools_common.h
	$(CC) $(CFLAGS) -c catuid.c


## misc targets:
TARGETS: Makefile
	grep '^[a-z].*:' Makefile | cut -d':' -f1 >TARGETS

clean: TARGETS
	rm -f `cat TARGETS` TARGETS

install: install-sbin install-man

install-man:
	install -d -m 0755 $(DESTDIR)$(MAN)/man8
	@for f in ./man/*.8 ; do\
	  echo "install -m 0644 $${f} $(DESTDIR)$(MAN)/man8" ;\
	  install -m 0644 $${f} $(DESTDIR)$(MAN)/man8 ;\
	done

install-sbin: $(RUNTOOLS)
	install -d -m 0755 $(DESTDIR)$(SBIN)
	@for f in $(RUNTOOLS) ; do\
	  echo "install -m 0755 $${f} $(DESTDIR)$(SBIN)";\
	  install -m 0755 $${f} $(DESTDIR)$(SBIN);\
	done

strip: $(RUNTOOLS) $(CATTOOLS)
	strip  $(RUNTOOLS) $(CATTOOLS)


.PHONY: all clean install install-man install-sbin strip

### EOF (Makefile)

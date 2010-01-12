# Makefile
# makfile for project perp:
# a persistent process supervisor and utility suite
# includes perp, runtools, and libasagna
# wcm, 2008.01.04 - 2009.12.05
# ===

# build configuration (not used in this makefile):
#include ./conf.mk

PROJLIBS = \
 ./_lasagna.done \

PROJAPPS = \
 ./_perpapps.done \
 ./_runtools.done \

## default target:
all: ./_all.done
./_all.done: $(PROJLIBS) $(PROJAPPS)
	touch $@

## lasagna:
lasagna: ./_lasagna.done
./_lasagna.done: conf.mk
	cd lasagna && $(MAKE)
	touch $@

## perp:
perpapps: ./_perpapps.done
./_perpapps.done: conf.mk $(PROJLIBS)
	cd perp && $(MAKE)
	touch $@

## runtools:
runtools: ./_runtools.done
./_runtools.done: conf.mk $(PROJLIBS)
	cd runtools && $(MAKE)
	touch $@

## misc targets:
clean:
	rm -f ./_perpapps.done; cd perp && $(MAKE) clean
	rm -f ./_runtools.done; cd runtools && $(MAKE) clean	

cleanlib:
	rm -f ./_lasagna.done; cd lasagna && $(MAKE) clean

cleanall: cleanlib clean
	rm -f ./_all.done


install: $(PROJAPPS)
	cd perp && $(MAKE) install
	cd runtools && $(MAKE) install

strip:
	cd perp && $(MAKE) strip
	cd runtools && $(MAKE) strip

.PHONY: all lasagna perpapps runtools clean cleanall cleanlib install strip


### EOF (Makefile)

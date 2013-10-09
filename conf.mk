# conf.mk
# project build/install configs
# wcm, 2009.09.14 - 2013.01.08
# ===

## build configuration, standard gcc + libc:
CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -DNDEBUG -O2
#CFLAGS = -Wall -Wextra -Wshadow -O2

## build configuration, dietlibc:
#CC = diet -Os gcc
#CFLAGS = -Wall -Wextra -Wshadow -DNDEBUG

## strip configuration
STRIP = strip
#STRIP = /some/other/stripper

## install configuration:
BINDIR = /usr/bin
SBINDIR = /usr/sbin
MANDIR  = /usr/share/man

### EOF (conf.mk)

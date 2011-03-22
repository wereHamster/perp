# conf.mk
# project build/install configs
# wcm, 2009.09.14 - 2011.03.22
# ===

## build configuration, standard gcc + libc:
CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -DNDEBUG -O2
#CFLAGS = -Wall -Wextra -Wshadow -O2

## build configuration, dietlibc:
#CC = diet -Os gcc
#CFLAGS = -Wall -Wextra -Wshadow -DNDEBUG

## install configuration:
BINDIR = /usr/bin
SBINDIR = /usr/sbin
MANDIR  = /usr/share/man

### EOF (conf.mk)

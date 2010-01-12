# conf.mk
# project build/install configs
# wcm, 2009.09.14 - 2009.12.29
# ===

## build configuration, standard gcc + libc:
CC = gcc
CFLAGS = -Wall -O2

## build configuration, dietlibc:
#CC = diet -Os gcc
#CFLAGS = -Wall

## install configuration:
SBIN = /usr/sbin
ETC  = /etc
MAN  = /usr/share/man

### EOF (conf.mk)

VERSION = 0.1.0

CC = gcc
SRC = par.c config.c util.c

CPPFLAGS = -DVERSION=\"$(VERSION)\" # WTF?
CFLAGS = -std=c99 -pedantic -Wall -O3 $(CPPFLAGS)

build:
	$(CC) $(CFLAGS) $(SRC) -o par

.PHONY: build
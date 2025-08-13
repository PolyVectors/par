VERSION = $(shell test -d .git && git describe --always 2>/dev/null)

CC = gcc
SRC = par.c config.c util.c

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -pedantic -Wall -Wextra -O3 -lX11 $(CPPFLAGS)

build:
	$(CC) $(CFLAGS) $(SRC) -o par

.PHONY: build
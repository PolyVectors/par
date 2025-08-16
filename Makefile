VERSION = $(shell test -d .git && git describe --always 2>/dev/null)

CC = tcc
SRC = par.c bar.c config.c util.c

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -Wall -Werror -lX11 $(CPPFLAGS)

build:
	$(CC) $(CFLAGS) $(SRC) -o par

.PHONY: build

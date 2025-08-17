VERSION = $(shell test -d .git && git describe --always 2>/dev/null)

CC = zig
SRC = par.c bar.c config.c util.c

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -lc -lX11 -O ReleaseSmall -fsingle-threaded $(CPPFLAGS)

build:
	$(CC) build-exe $(CFLAGS) $(SRC)

.PHONY: build

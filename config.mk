VERSION = $(shell test -d .git && git describe --always 2>/dev/null)

CC = zig build-exe

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -I/usr/include/freetype2 \
		-lc -lX11 -lXft \
		-O ReleaseSmall -fsingle-threaded \
		$(CPPFLAGS)

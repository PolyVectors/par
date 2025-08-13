VERSION = 0.1.0

CC = gcc
SRC = par.c config.c util.c

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -pedantic -Wall -Wextra -O3 $(CPPFLAGS)

build:
	$(CC) $(CFLAGS) $(SRC) -o par

.PHONY: build
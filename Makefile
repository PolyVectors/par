include config.mk

SRC = par.c bar.c parse.c util.c

build:
	$(CC) $(CFLAGS) $(SRC)

.PHONY: build

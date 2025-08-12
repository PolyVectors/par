CC = gcc
SRC = par.c config.c

build:
	$(CC) -std=c99 $(SRC) -o par

.PHONY: build
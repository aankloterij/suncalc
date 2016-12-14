CC=gcc
CFLAGS=-Wall -Wextra
CLIBS=-lm

all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c -o bin/suncalc $(CLIBS)

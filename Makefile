CC=gcc
CFLAGS=-Wall -Wextra
CLIBS=-lm

all:
	$(CC) $(CFLAGS) src/main.c -o bin/suncalc $(CLIBS)

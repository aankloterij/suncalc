CC=gcc
CFLAGS=-Wall -Wextra -lm

all:
	$(CC) $(CFLAGS) src/main.c -o main

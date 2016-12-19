CC=gcc
CFLAGS=-Wall -Wextra
CLIBS=-lm

all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c -o bin/suncalc $(CLIBS)

filmpjes:
	rm -rf plots && mkdir plots
	./imggen.py
	ffmpeg -framerate $2 -pattern_type glob -i 'plots/*.png' out.mp4

CC=gcc
CFLAGS=-Wall -Wextra
CLIBS=-lm

all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c -o bin/suncalc $(CLIBS)

filmpjes:
	# Maak de map plots voor als die nog niet bestaat, en zorg ervoor dat deze leeg is.
	rm -rf plots && mkdir plots
	./imggen.py
	ffmpeg -framerate 16 -pattern_type glob -i 'plots/*.png' out.mp4

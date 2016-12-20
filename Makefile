CC=gcc

# Dit is voor debugging, zodat debugging met gdb makkelijker word
CFLAGS=-Wall -Wextra -g

# Dit wil je niet aan hebben tijdens het debuggen
# want dan geeft hij geen warnings voor domme dingen
# CFLAGS=-O3
CLIBS=-lm

all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c -o bin/suncalc $(CLIBS)

install: all
	cp bin/suncalc /usr/bin/suncalc

filmpjes: all
	# Maak de map plots voor als die nog niet bestaat, en zorg ervoor dat deze leeg is.
	rm -rf plots && mkdir plots

	./imggen.py

	ffmpeg -framerate 16 -pattern_type glob -i 'plots/*.png' out.mp4

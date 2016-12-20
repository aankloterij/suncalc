# suncalc

Dit is een repository die alle code bevat om de hoek van de zon en de stand van zonnepanelen te bepalen.

### Inhoud
Deze repository bevat onder andere de volgende bestanden, die elk voor een ander doel dienen.

#### `suncalc/suncalc.ino`
De Arduino sketch om de serial uit te lezen en de motoren aan te sturen. Upload dit naar de Arduino met de Arduino IDE

#### `src/main.c`
Het C programma om de positie van de zon te berekenen.
Dit programma is de basis van alle andere software, aangezien alle Python scripts en dus indirect ook het Arduino programma hier gebruik van maken.

Gebruik `make` om dit te compilen

Hieronder is te zien hoe je het programma kan gebruiken (ook te zien met `bin/suncalc -h`)
```
Usage: suncalc [-p timestamp] [-t timestamp] [-c timestamp]
  -c                       hetzelfde als -t maar zonder timestamp, en geeft maar 1
                           resultaat voor gegeven tijd
  -p                       print standen van paneel
  -t                       geef een lijst met x, y, z, timestamp gegevens
                           vanaf de gegeven timestamp totdat de zon onder gaat
  -h                       laat deze help zien en exit
  -v                       print de versie en exit
```

#### `sunplot.py`
Laat gegevens van `src/main.c` zien in een 3D grafiek met de huidige positie van de zon.

#### `imggen.py`
Render de grafiek van `sunplot.py` een aantal keer en sla deze op in images, die vervolgens weer achter elkaar kunnen zetten om een geanimeerde grafiek te maken, zie bijvoorbeeld [deze video](https://youtu.be/TODOFIX) op 36FPS, waarbij dus 1 seconde precies 1 uur is, en [deze video](https://youtu.be/TODOFIX) die 2 keer zo traag gaat als de vorige om het effect beter te laten zien.

Als je de juiste packages geinstalleerd hebt (zie Dependencies) kun je `make filmpjes` gebruiken om een animatie van de grafiek te krijgen.

#### `suncalc-web/index.html`
Hier is de basis versie waarmee alles op gebaseerd is. Van deze versie weten we zeker dat hij werkt.

### Dependencies
Om dit te gebruiken heb je onder andere de volgende programmas nodig:
- `python3`
- `gcc`
- `arduino`
- `git`
- `make`
- `ffmpeg`
- `sl`
- `python3-matplotlib`

Ook heb je chrome, chromium, firefox of een andere **internet browser** nodig (dus niet IE) om `suncalc-web/index.html` uit te kunnen voeren, maar de meeste computers zullen dit al wel hebben.

Vrijwel alle zijn gemakkelijk te installeren met `apt-get install` als je Ubuntu gebruikt of met `pacman -S` als je voor de thug life hebt gekozen `B-)` jwz g.
```
# apt-get install python3 gcc arduino git make ffmpeg sl python3-matplotlib
```

`matplotlib` is zowel via de package manager als via `pip` beschikbaar, maar `pip` is aanbevolen in verband met nieuwere versies.
```
# pip3 install matplotlib
```

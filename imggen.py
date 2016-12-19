#!/usr/bin/python3

from matplotlib import pyplot as plt
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
import datetime
import subprocess
import time


def getCurrentTime():
	'''
	Geeft huidige timestamp, makkelijk om commands te maken.
	'''

	return int(time.time())

def command(command):
	'''
	Voert een shell-commando uit. Dit wordt gebruikt voor het verkrijgen
	van de informatie over de zon uit onze C-binary (zie bin/suncalc)
	'''

	print("Executing '%s' ..." % command)
	return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.readlines()

def getCurrentSunPos():
	'''
	Geeft de huidige positie van de zon op de
	timestamp 'getCurrentTimestamp()' volgens de binary in 'bin/'
	gemaakt door 'src/main.c'
	'''

	sunpos = command("bin/suncalc -c %d" % getCurrentTime())

	x, y, z = sunpos[0].decode("utf-8").strip().split("\t");

	return float(x), float(y), float(z)

# Dit is een lijst met resultaten die in de grafiek de curve van de zon
# over de gehele dag vormen. Gewoon een lijst met punten dus.
xvals, yvals, zvals = [], [], []

# Voor de presentatie (op donderdag 22 december, 2016) gaan we een animatie maken
# dus nemen we een timestamp aan het begin van deze dag en plotten we alle
# resultaten waar de zon nog op is. De resultaten zijn volgens het src/main.c script
# 100 seconden van elkaar af. Dit lever ongeveer 250 tot 300 verschillende situaties
# waar de zon op is.
dag = datetime.datetime.fromtimestamp(1482371634); # Dit is op de 22e, 1 uur 's nachts

# Dit verkrijgt gewoon dezelfde timestamp als het getal hierboven
# maar ik wil niet hetzelfde getal ergens 2x in zetten.
now = dag.strftime("%s")

# Haal de gegevens op uit de binary in 'bin/' en gebruik de gekozen timestamp
data = command("bin/suncalc -t %s" % now)

for line in data:
	# Parse de x, y, en z coordinaten voor het verloop van de gehele dag.
	# Hier doen we nog niks met de animatie, dit genereert een lijst met
	# elke positie van de zon op die dag, net zoals in sunplot.py
	# Hier hebben we de timestamp die erbij komt dus nog niet nodig.
	x, y, z, overbodigeTime = line.decode("utf-8").strip().split('\t')

	# Deze 3 'lists' met gegevens blijven gedurende het hele script hetzelfde.
	xvals.append(float(x))
	yvals.append(float(y))
	zvals.append(float(z))

for line in data:
	# Hier parsen we dezelfde data nog een keer, maar nu gaan we dus voor elke
	# regel een nieuwe plot maken. Elke regel representateert hier de "huidige"
	# positie van de zon. Hier hebben we dus wel de timestamp nodig om de
	# correcte titel van de grafiek te genereren.
	figure = plt.figure(figsize=(24, 13.5), dpi=80); # 24x13.5 inch @ 80 dpi = 1920x1080 pixels.

	# extraheer de gegevens uit elke regel.
	lx, ly, lz, time = line.decode("utf-8").strip().split('\t')

	# Maak een datetime object van de gevonden timestamp. Hierop gebruiken we
	# strftime om dit te formatten in de titel.
	dag = datetime.datetime.fromtimestamp(int(time))

	# Dit is dus de titel. Het tijd gedeelte is in het formaat Uur:minuut dag(getal) maand(1e 3 letters), jaar (2016)
	figure.suptitle("Positie van de zon op %s" % dag.strftime("%H:%M %d %b, %Y"), fontsize=14)

	# Maak een nieuwe plot
	ax = figure.add_subplot(111, projection='3d')
	ax.axis('equal') # met gelijke x en y assen. (y is hier dus op de grond, z is verticaal)


	# Plot de gegevens
	remmers = ax.scatter(0, 0, 0, color='green', label='Positie van remmers\' huis') # remmers' huis
	sunLine = ax.plot(xvals, zvals, yvals, label='Traject van de zon') # zon curve

	# Plot de 'huidige' positie van de zon.
	liveSun = ax.scatter(float(lx), float(lz), float(ly), label='Huidige positie van de zon', color='red')


	# Maak een legenda
	plt.legend()

	# Stel labels in, nogmaals Z en Y zijn omgedraaid omdat ik wil dat de
	# y-as verticaal loopt.
	ax.set_xlabel("X")
	ax.set_ylabel("Z")
	ax.set_zlabel("Y")

	# Sla dit figuur op op de locatie 'plots/<timestamp>.png'
	figure.savefig("plots/%s.png" % dag.strftime("%s"), bbox_inches='tight')

	# Sluit het plot weer. Dit is nodig om ram te besparen, omdat we hier
	# ongeveer 250 tot 400 (afhankelijk van de lengte van de dag) plots maken.
	plt.close()

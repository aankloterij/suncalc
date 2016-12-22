#!/usr/bin/python3

from matplotlib import pyplot as plt
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
import datetime

from suncalc_utils import *

# Dit is een lijst met resultaten die in de grafiek de curve van de zon
# over de gehele dag vormen. Gewoon een lijst met punten dus.
xvals, yvals, zvals = [], [], []

now = int(datetime.date.today().strftime('%s')) # Huidige timestamp
data = command("bin/suncalc -t %d" % now) # Veel regels data in het formaat x, y, z, timestamp.

for line in data:
	# Parse alle data. Alle gegevens zijn in het onderstaande formaat gegeven:
	# x\ty\tz\ttimestamp\n
	# x\ty\tz\ttimestamp\n
	# ..
	# Individuele regels zijn geschijden door een \n, en de data in die regels
	# zijn gescheiden door een tab (\t)
	x, y, z, timestampvoorshit = line.decode("utf-8").strip().split('\t')

	# Voeg de data toe aan de lijsten
	xvals.append(float(x))
	yvals.append(float(y))
	zvals.append(float(z))

# Verkrijg de huidige positie van de zon (current-x, current-y, current-z)
cx, cy, cz = getCurrentSunPos()

# plt.ion() # Dit werkt niet op max' PC. anders hadden we een interactief ding kunnen maken.

# Maak een nieuw matplotlib figure.
figure = plt.figure()

# Titel
figure.suptitle("Positie van de zon op %s" % datetime.datetime.now().strftime("%H:%M %d %b, %Y"), fontsize=14)

# Nieuwe grafiek
ax = figure.add_subplot(111, projection='3d')

# We willen dat de x- en y-as gelijk zijn.
ax.axis('equal')

# Plot de gegevens in de grafiek.
remmers = ax.scatter(0, 0, 0, color='green', label='Positie van remmers\' huis') # remmers' huis
sunLine = ax.plot(xvals, zvals, yvals, label='Traject van de zon') # zon curve
liveSun = ax.scatter(cx, cz, cy, color='red', s=60, label='Huidige positie van de zon') # current zon

# Genereer een legenda
plt.legend()

# In matplotlib is de z-as verticaal, maar ik wil dat de y-as verticaal is.
# hierom heb ik dus de z- en y-as omgewisseld, zodat het lijkt alsof
# De z-as de diepte in loopt, en de y-as omhoog.
# Eigenlijk plotten we dus de hoogte op de z-as. Dit is ook waarom
# we in de plots hierboven de z waarden op de y-positie zetten, en de y-waarden
# op de z-positie.
ax.set_xlabel("X")
ax.set_ylabel("Z")
ax.set_zlabel("Y")

# Laat de grafiek zien. Dit kan niet woren afgesloten met ctrl+c, en het crasht
# als je ctrl+z gebruikt. Sluit daarom het scherm om dit script te terminaten.
plt.show()

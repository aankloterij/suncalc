#!/usr/bin/python3

from matplotlib import pyplot as plt
import matplotlib
from mpl_toolkits.mplot3d import Axes3D
import datetime
import subprocess
import time

def getCurrentTime():
	return int(time.time())

def command(command):
	print("Executing '%s' ..." % command)
	return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.readlines()

def getCurrentSunPos():
	sunpos = command("bin/suncalc -c %d" % getCurrentTime())

	x, y, z = sunpos[0].decode("utf-8").strip().split("\t");

	return float(x), float(y), float(z)

xvals, yvals, zvals = [], [], []

dag = datetime.datetime.fromtimestamp(1482371634); # Dit is op de 22e, 1 uur 's nachts
now = dag.strftime("%s");

data = command("bin/suncalc -t %s" % now)

for line in data:
	x, y, z, bstime = line.decode("utf-8").strip().split('\t')

	xvals.append(float(x))
	yvals.append(float(y))
	zvals.append(float(z))

# cx, cy, cz = getCurrentSunPos()

# plt.ion()

for line in data:

	figure = plt.figure(figsize=(24, 13.5), dpi=80);

	lx, ly, lz, time = line.decode("utf-8").strip().split('\t')
	dag = datetime.datetime.fromtimestamp(int(time))


	figure.suptitle("Positie van de zon op %s" % dag.strftime("%H:%M %d %b, %Y"), fontsize=14)

	ax = figure.add_subplot(111, projection='3d')
	ax.axis('equal')

	remmers = ax.scatter(0, 0, 0, color='green', label='Positie van remmers\' huis') # remmers' huis
	sunLine = ax.plot(xvals, zvals, yvals, label='Traject van de zon') # zon curve

	liveSun = ax.scatter(float(lx), float(lz), float(ly), label='Huidige positie van de zon', color='red')

	plt.legend()

	ax.set_xlabel("X")
	ax.set_ylabel("Z")
	ax.set_zlabel("Y")

	figure.savefig("plots/%s.png" % dag.strftime("%s"), bbox_inches='tight')
	plt.close()

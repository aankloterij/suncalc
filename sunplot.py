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

now = datetime.date.today().strftime("%s")
data = command("bin/suncalc -t %s" % now)

for line in data:
	x, y, z = line.decode("utf-8").strip().split('\t')

	xvals.append(float(x))
	yvals.append(float(y))
	zvals.append(float(z))

cx, cy, cz = getCurrentSunPos()

# plt.ion()

figure = plt.figure()

ax = figure.add_subplot(111, projection='3d')
ax.axis('equal')

ax.scatter(0, 0, 0, color='green') # remmers' huis
ax.plot(xvals, zvals, yvals) # zon curve
liveSun = ax.scatter(cx, cz, cy, color='red', s=60) # current zon

ax.set_xlabel("X")
ax.set_ylabel("Z")
ax.set_zlabel("Y")

plt.show()
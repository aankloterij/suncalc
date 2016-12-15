from matplotlib import pyplot
import pylab
from mpl_toolkits.mplot3d import Axes3D
import datetime
import subprocess
import time

def getCurrentTime():
	return int(time.time())

def getCurrentSunPos():
	sunpos = subprocess.Popen("bin/suncalc -c %d", getCurrentTime(), shell=True, stdout=subprocess.PIPE).stdout.readlines()

	x, y, z = sunpos[0].decode("utf-8").strip().split("\t");

	return x, y, z

fig = pylab.figure()
ax = Axes3D(fig)

xvals, yvals, zvals = [], [], []

now = datetime.date.today().strftime("%s")
data = subprocess.Popen("bin/suncalc -t %d".format(now), shell=True, stdout=subprocess.PIPE).stdout.readlines()

for line in data:
	x, y, z = line.decode("utf-8").strip().split('\t')

	xvals.append(float(x))
	yvals.append(float(y))
	zvals.append(float(z))

# Positie remmers' huis
xvals.append(0)
yvals.append(0)
zvals.append(0)


ax.scatter(xvals, zvals, yvals)

cx, cy, cz = getCurrentSunPos()

liveSun = ax.scatter(cx, cz, cx)

ax.set_xlabel("X")
ax.set_ylabel("Z")
ax.set_zlabel("Y")

pyplot.show()

while True:
	sleep(5)

	nx, ny, nz = getCurrentSunPos()

	liveSun.set_xdata(nx)
	liveSun.set_zdata(ny)
	liveSun.set_ydata(nz)

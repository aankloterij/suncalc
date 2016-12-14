from matplotlib import pyplot
import pylab
from mpl_toolkits.mplot3d import Axes3D
import datetime
import subprocess


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

xvals.append(0)
yvals.append(0)
zvals.append(0)

ax.scatter(xvals, zvals, yvals)
ax.set_xlabel("X")
ax.set_ylabel("Z")
ax.set_zlabel("Y")

pyplot.show()

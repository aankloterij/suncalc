#!/usr/bin/python3
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

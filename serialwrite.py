#!/usr/bin/python3

import serial
import suncalc_utils
import time
import subprocess

from suncalc_utils import *


# Verander dit maybe idk jwz g swag yolo
with serial.Serial("/dev/ttyACM0", 9600) as device:

    if not device.is_open:
        print("Werkt niet")

    while True:
        data = command("bin/suncalc -p %d" % getCurrentTime())

        print(data[0])

        device.write(data[0])

        time.sleep(5)

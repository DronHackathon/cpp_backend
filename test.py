#!python
import force_api as f
import time as t

f.open("/dev/ttyACM0")
while 1:
    print f.recognize()
f.close()

import serial
from time import time
ser = serial.Serial('/dev/ttyUSB1',115200)  # open serial port

t = time()
avg = 0.0
while True:
    if(ord(ser.read(1))==36):
        avg = 0.99*avg + 0.01*(time()-t)
        print(1./avg)
        t = time()

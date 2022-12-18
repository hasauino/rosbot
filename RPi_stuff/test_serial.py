import serial
from time import sleep

ser = serial.Serial('/dev/ttyS0', 115200)

while True:
    print("reading a line ...")
    rec = ser.read()
    print(f"Done. received: {rec}")
    sleep(0.01)

import argparse
import collections
import logging
import signal
import time

import matplotlib.pyplot as plt
import numpy as np
import serial

condition = True


def handler(_, __):
    global condition
    condition = False


def main():
    global condition
    while condition:
        try:
            ser.reset_input_buffer()
            rec = ser.readline()
            data_str = rec.decode("utf-8").strip()
            angle = float(data_str)
        except:
            continue
        print(angle)
        y_data.append(angle)
        x_data.append(time.time() - t0)
        ax.set_xlim([x_data[0], x_data[-1]])
        ax.plot(x_data, y_data, 'b')
        plt.draw()
        plt.pause(0.001)


if __name__ == "__main__":
    t0 = time.time()
    ser = serial.Serial('/dev/ttyUSB0', 115200)

    fig = plt.figure(0, [10, 5])
    ax = fig.add_subplot(1, 1, 1)
    ax.grid()

    x_data = collections.deque([], maxlen=100)
    y_data = collections.deque([], maxlen=100)

    signal.signal(signal.SIGTERM, handler)
    signal.signal(signal.SIGINT, handler)

    parser = argparse.ArgumentParser(
        prog='Reader',
        description='Test reading serial data')
    parser.add_argument('-l', '--log-level', type=str, default="info")
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG)
    logger = logging.getLogger("main")
    logger.setLevel(args.log_level.upper())

    main()

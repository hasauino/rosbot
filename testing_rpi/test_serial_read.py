import serial
import struct
import argparse
import logging

ser = serial.Serial('/dev/ttyS0', 115200)
FIRST_BYTE = 0x7B
SECOND_BYTE = 0x37

def main():
    while True:
        logger.debug("reading a line ...")
        rec = ser.read()[0]
        if rec != FIRST_BYTE:
            continue
        rec = ser.read()[0]
        if rec != SECOND_BYTE:
            continue
        length = ser.read()[0]
        data = ser.read(length)
        checksum = ser.read()[0]
        delta_s_r = struct.unpack('f', data[0:4])[0]
        delta_s_l = struct.unpack('f', data[4:8])[0]
        w_r = delta_s_r = struct.unpack('f', data[8:12])[0]
        w_l = delta_s_r = struct.unpack('f', data[12:16])[0]
        ax = struct.unpack('h', data[16:18])[0]
        ay = struct.unpack('h', data[18:20])[0]
        az = struct.unpack('h', data[20:22])[0]
        gx = struct.unpack('h', data[22:24])[0]
        gy = struct.unpack('h', data[24:26])[0]
        gz = struct.unpack('h', data[26:28])[0]
        temp = struct.unpack('f', data[28:32])[0]
        logger.info(f"{w_l}  ------o-o----- {w_r}")



if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    prog = 'Reader',
                    description = 'Test reading serial data')
    parser.add_argument('-l', '--log-level', type=str, default="info")
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG)
    logger = logging.getLogger("main")
    logger.setLevel(args.log_level.upper())

    main()

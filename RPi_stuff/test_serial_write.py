import serial
import struct
import argparse
import logging
import curses

ser = serial.Serial('/dev/ttyS0', 115200)
FIRST_BYTE = 0x7B
SECOND_BYTE = 0x37
VEL_COMMAND_ID = 0x00
HEAD_COMMAND_ID = 0x03

HEAD_INCREMENT = 40
DEFAULT_V = 0.05
DEFAULT_W = 0.05

def speed_command(v, w):
    msg = bytearray([FIRST_BYTE, SECOND_BYTE])
    msg.append(9)  # data length
    msg.append(VEL_COMMAND_ID)  # command id
    msg.extend(list(struct.pack('f', v)))
    msg.extend(list(struct.pack('f', w)))
    msg.append(0)  # checksum (dummy)
    ser.write(msg)

def head_command(angle):
    msg = bytearray([FIRST_BYTE, SECOND_BYTE])
    msg.append(2)  # data length
    msg.append(HEAD_COMMAND_ID)  # command id
    msg.append(angle)
    msg.append(0)  # checksum (dummy)
    ser.write(msg)

def on_up():
    speed_command(DEFAULT_V, 0)

def on_down():
    speed_command(-DEFAULT_V, 0)

def on_left():
    speed_command(0, DEFAULT_W)

def on_right():
    speed_command(0, -DEFAULT_W)

def on_w():
    head_command(90 + HEAD_INCREMENT)

def on_s():
    head_command(90 - HEAD_INCREMENT)

def on_space():
    speed_command(0, 0)
    head_command(90)

def main():
    stdscr = curses.initscr()
    curses.cbreak()
    stdscr.keypad(1)
    stdscr.addstr(0, 0,"Hit 'q' to quit. Arrow keys to move robot, w/q to move head, space to reset")
    stdscr.refresh()
    key = ''
    while key != ord('q'):
        key = stdscr.getch()
        stdscr.addch(20,25,key)
        stdscr.refresh()
        if key == curses.KEY_UP: 
            stdscr.addstr(2, 10, "Up")
            on_up()
        elif key == curses.KEY_DOWN: 
            stdscr.addstr(4, 10, "Down")
            on_down()
        if key == curses.KEY_RIGHT: 
            stdscr.addstr(3, 20, "Right")
            on_right()
        elif key == curses.KEY_LEFT: 
            stdscr.addstr(3, 0, "Left")
            on_left()
        elif key == 32:  # space 
            stdscr.addstr(3, 10, "Reset")
            on_space()
        elif key == 119:  # s
            stdscr.addstr(2, 40, "Head -")
            on_s()
        elif key == 115:  # w
            stdscr.addstr(4, 40, "Head +") 
            on_w()     
    curses.endwin()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    prog = 'Writer',
                    description = 'Test reading serial data')
    parser.add_argument('-l', '--log-level', type=str, default="info")
    args = parser.parse_args()
    logging.basicConfig(level=logging.DEBUG)
    logger = logging.getLogger("main")
    logger.setLevel(args.log_level.upper())
    main()

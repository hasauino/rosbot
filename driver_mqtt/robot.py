import logging
import struct
import threading
import time

import serial

FIRST_BYTE = 0x7B
SECOND_BYTE = 0x37
VEL_COMMAND_ID = 0x00
HEAD_COMMAND_ID = 0x03
ZERO_HEAD = 90.0


class Robot:

    def __init__(
        self,
        serial_port="/dev/ttyS0",
        baudrate=115200,
        timeout=1.0,
        heartbeat=0.5,
        serial_writer_rate=10,
        safe_mode=True,
    ) -> None:
        self.serial_reader = serial.Serial(serial_port,
                                           baudrate,
                                           exclusive=False,
                                           timeout=timeout)
        self.serial_writer = serial.Serial(serial_port,
                                           baudrate,
                                           exclusive=False,
                                           write_timeout=timeout)
        self.heartbeat = heartbeat
        self.is_running = True
        self.imu_temp = None
        self.w = None
        self.v = None
        self.x = None
        self.y = None
        self.theta = None
        self.head_angle = ZERO_HEAD
        self.set_speed(0.0, 0.0)
        self.set_head(self.head_angle)
        self.cmd_v = 0.0
        self.cmd_w = 0.0
        self.speed_timestamp = time.time()
        self.head_timestamp = time.time()
        self.heartbeat = heartbeat
        self.serial_writer_rate = serial_writer_rate
        self.safe_mode = safe_mode
        self.reader_thread = threading.Thread(target=self.reader, daemon=True)
        self.reader_thread.start()
        self.writer_thread = threading.Thread(target=self.writer, daemon=True)
        self.writer_thread.start()

    def set_speed(self, v, w):
        self.speed_timestamp = time.time()
        self.cmd_v = v
        self.cmd_w = w

    def set_head(self, angle):
        self.head_timestamp = time.time()
        self.head_angle = angle

    def stop(self):
        self.is_running = False
        if self.serial_writer.isOpen():
            self._send_speed(0.0, 0.0)
            self._send_head(ZERO_HEAD)
        if self.reader_thread.is_alive:
            self.reader_thread.join()
        if self.writer_thread.is_alive:
            self.writer_thread.join()

    def writer(self):
        logging.info("Starting serial writer thread")
        while self.is_running:
            if self.safe_mode:
                if (time.time() - self.speed_timestamp) > self.heartbeat:
                    self.set_speed(0.0, 0.0)
                if (time.time() - self.head_timestamp) > self.heartbeat:
                    self.set_head(ZERO_HEAD)
            self._send_speed(self.cmd_v, self.cmd_w)
            self._send_head(self.head_angle)
            time.sleep(1.0 / self.serial_writer_rate)
        logging.info("Stopping serial writer thread")

    def reader(self):
        logging.info("Starting serial reader thread")
        while self.is_running:
            logging.debug("wating serial msg..")
            rec = self.serial_reader.read()[0]
            if rec != FIRST_BYTE:
                continue
            rec = self.serial_reader.read()[0]
            if rec != SECOND_BYTE:
                continue
            logging.debug("Got a serial msg.. Reading it..")
            length = self.serial_reader.read()[0]
            data = self.serial_reader.read(length)
            checksum = self.serial_reader.read()[0]
            if not self.check_data(data, checksum):
                logging.warning(
                    "corrupted data received... will ignore this message")
                continue
            delta_s_r = struct.unpack('f', data[0:4])[0]
            delta_s_l = struct.unpack('f', data[4:8])[0]
            w_r = struct.unpack('f', data[8:12])[0]
            w_l = struct.unpack('f', data[12:16])[0]
            ax = struct.unpack('h', data[16:18])[0]
            ay = struct.unpack('h', data[18:20])[0]
            az = struct.unpack('h', data[20:22])[0]
            gx = struct.unpack('h', data[22:24])[0]
            gy = struct.unpack('h', data[24:26])[0]
            gz = struct.unpack('h', data[26:28])[0]
            self.update_states(delta_s_r, delta_s_l, w_r, w_l, ax, ay, az, gx,
                               gy, gz)
            self.imu_temp = struct.unpack('f', data[28:32])[0]
            logging.debug("Finished reading serial message")
        logging.info("Stopping serial reading thread")

    def _send_speed(self, v, w):
        msg = bytearray([FIRST_BYTE, SECOND_BYTE])
        msg.append(9)  # data length
        msg.append(VEL_COMMAND_ID)  # command id
        msg.extend(list(struct.pack('f', v))[::-1])
        msg.extend(list(struct.pack('f', w))[::-1])
        msg.append(0)  # checksum (dummy)
        self.serial_writer.write(msg)

    def _send_head(self, angle):
        msg = bytearray([FIRST_BYTE, SECOND_BYTE])
        msg.append(2)  # data length
        msg.append(HEAD_COMMAND_ID)  # command id
        msg.append(int(angle))
        msg.append(0)  # checksum (dummy)
        self.serial_writer.write(msg)

    def check_data(self, data, checksum):
        # TODO: checksum
        return True

    def update_states(self, delta_s_r, delta_s_l, w_r, w_l, ax, ay, az, gx, gy,
                      gz):
        pass

    def __del__(self):
        self.stop()
        self.serial_reader.close()
        self.serial_writer.close()
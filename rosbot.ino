#include "Robot.h"
#include "communication.h"
#include "commands.h"
#include "IMU.h"

Robot robot;
IMU imu;
Receiver receiver;
// Command msgs definitions
Transmitter transmitter;
Velocity vel_msg;
StartStream start_msg;
Head head_msg;
byte* msg;
int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  robot.init();
  imu.init();
  Serial.begin(115200);
}

void loop() {
  if (receiver.available()) {
    msg = receiver.latest_msg();

    // counter from 3 (after msg length byte, and before the last checksum byte)
    for (int i = 3; i < msg[2] + 2;) {
      switch (msg[i]) {
        case vel_msg.ID:
          vel_msg.deserialize(msg[i + 1]); //serialize the following bytes (equal to vel_msg.length bytes) starting from i+1
          i += vel_msg.length;
          break;

        case start_msg.ID:
          start_msg.deserialize(msg[i + 1]); //serialize the following bytes (equal to vel_msg.length bytes) starting from i+1
          i += start_msg.length;
          break;

        case head_msg.ID:
          head_msg.deserialize(msg[i + 1]); //serialize the following bytes (equal to vel_msg.length bytes) starting from i+1
          i += head_msg.length;
          break;

        default:
          i++;
      }
    }
  }
  robot.set_speed(vel_msg.v, vel_msg.w);
  robot.set_head(head_msg.angle);
  
  // Send odometry, IMU
  if (start_msg.is_enabled && transmitter.check_rate()) {
    transmitter.push(robot.delta_s_r());
    transmitter.push(robot.delta_s_l());
    transmitter.push(robot.get_rightspeed());
    transmitter.push(robot.get_leftspeed());
    imu.getData(ax, ay, az, gx, gy, gz);
    transmitter.push(ax);
    transmitter.push(ay);
    transmitter.push(az);
    transmitter.push(gx);
    transmitter.push(gy);
    transmitter.push(gz);
    transmitter.push(imu.getTemp());
    transmitter.send();
  }
}


void serialEvent() {
  // add saftey check for communication loss
  // if no byte is received within 200 ms, set everything to zero (stop motors)
  receiver.update(Serial.read());
}

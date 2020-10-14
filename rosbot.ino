#include "Robot.h"
#include "communication.h"
#include "commands.h"

Robot robot;
Receiver receiver;
Transmitter transmitter;
Velocity vel_msg;
byte* msg;

void setup() {
  Serial.begin(115200);
  robot.init();
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

        default:
          i++;
      }
    }
  }
  robot.set_speed(vel_msg.v, vel_msg.w);

  // Send odometry
  transmitter.push(robot.get_rightposition());
  transmitter.push(robot.get_leftposition());
  transmitter.push(robot.get_rightspeed());
  transmitter.push(robot.get_leftspeed());
  transmitter.send();
}


void serialEvent() {
  // add saftey check for communication loss
  // if no byte is received within 200 ms, set everything to zero (stop motors)
  receiver.update(Serial.read());
}

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




  transmitter.push(float(10.5));
  transmitter.push(byte(0xCC));
  transmitter.push(float(2.35));
  transmitter.push(int(0xBBAA));

  transmitter.send();

  /*byte d2[3] = {0xA4, 0xA5, 0xA6};
    transmitter._push_data(d2[0], 3);
    transmitter.send();*/
}


void serialEvent() {
  // add saftey check for communication loss
  // if no byte is received within 200 ms, set everything to zero (stop motors)
  receiver.update(Serial.read());
}

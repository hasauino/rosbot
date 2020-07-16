#include "Robot.h"
#include "communication.h"
#include "commands.h"

Robot robot;
Receiver receiver;
Velocity vel_msg;
byte* msg;

void setup() {
  Serial.begin(115200);
  robot.init();
}

void loop() {
  if (receiver.available()) {
    Serial.println("yes, available!");
    msg = receiver.latest_msg();

    // counter from 3 (after msg length byte, and before the last checksum byte)
    for (int i = 3; i < msg[2] + 2;) {
      switch (msg[i]) {
        case vel_msg.ID:
          Serial.println("yes, velocity!");
          vel_msg.deserialize(msg[i + 1]); //serialize the following bytes (equal to vel_msg.length bytes) starting from i+1
          i += vel_msg.length;
          break;

        default:
          Serial.println("not velocity!");
          i++;
      }
    }
  }
  //robot.set_speed(vel_msg.v, vel_msg.w);
  Serial.print(vel_msg.v); Serial.print("  |  "); Serial.println(vel_msg.w);
  delay(100);
}


void serialEvent() {
  // add saftey check for communication loss
  // if no byte is received within 200 ms, set everything to zero (stop motors)
  receiver.update(Serial.read());
}

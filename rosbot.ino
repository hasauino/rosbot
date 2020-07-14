#include "Robot.h"
#include "communication.h"

Robot robot;
Receiver receiver;
float v = 0, w = 0;
byte* msg;

void setup() {
  Serial.begin(115200);
  robot.init();
}

void loop() {
  //robot.set_speed(v, w);
  if (receiver.available()) {
    msg = receiver.latest_msg();
    for (int i = 0; i < 10; i++) {
      Serial.write(msg[i]);
    }
  }
}


void serialEvent() {
  receiver.update(Serial.read());
}

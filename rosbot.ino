#include "Robot.h"
#include "communication.h"

Robot robot;
float v = 0, w = 0;
byte message[50];
int index = 0;

void setup() {
  Serial.begin(115200);
  robot.init();
}

void loop() {
  robot.set_speed(v, w);
}


void serialEvent() {
  if (index > 1) {
    message[index++] = Serial.read();
  }
  else if (index == 1) {
    if (Serial.read() == MSG_SECOND_BYTE) {
      message[index++] = MSG_SECOND_BYTE;
      Serial.write(1);
    }
    else {
      index = 0;
      message[0] = 0; // wrong msg, reset first byte
    }
  }
  else if (index == 0) {
    if (Serial.read() == MSG_FIRST_BYTE) {
      message[index++] = MSG_FIRST_BYTE;
      Serial.write(0);
    }
  }

  if (index > 2) {
    if (index > message[2] + 2) {
      for (int i=0; i<index;i++){
        Serial.write(message[i]);
        }
      index = 0; // end of msg, reset index
      message[0] = 0; // end of msg, reset first byte
      message[1] = 0; // reset second byte
    }
  }


}//serialEvent

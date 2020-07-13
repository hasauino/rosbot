#include "Robot.h"

Robot robot;
float v = 0, w = 0;
void setup() {
  Serial.begin(115200);
  robot.init();
}

void loop() {
  char rec = Serial.read();
  switch (rec) {
    case '1':
      v = 0.0;
      break;
    case '2':
      v = 0.2;
      break;
    case '3':
      v = 0.3;
      break;
    case '4':
      v = 0.4;
      break;
    case '5':
      v = 0.5;
      break;
    case '6':
      v = 0.6;
      break;
    case '7':
      v = 0.7;
      break;
    case '8':
      v = 0.8;
      break;
    case '9':
      v = 0.9;
      break;
  }

  robot.set_speed(v, w);
}

#include "Motor.h"
#include "pinout.h"


#define SETPOINT 0

Motor right_motor(RIGHT_MOTOR_PINS);
Motor left_motor(LEFT_MOTOR_PINS);
float setpoint = 0;
void setup() {
  Serial.begin(115200);
  right_motor.init(3);
  //left_motor.init(10);
}

void loop() {
  char rec = Serial.read();
  switch (rec) {
    case '1':
      setpoint = 0.0;
      break;
    case '2':
      setpoint = 50.0;
      break;
    case '3':
      setpoint = 100.0;
      break;
    case '4':
      setpoint = 150.0;
      break;
    case '5':
      setpoint = 200.0;
      break;
  }



  right_motor.set_speed(rad2rpm(setpoint));
  Serial.print(setpoint);
  Serial.print(",");
  Serial.println(rad2rpm(right_motor.get_speed()));
}

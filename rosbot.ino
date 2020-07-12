#include "Motor.h"
#include "helpers.h"
#include "pinout.h"

#include <AutoPID.h>

double speed = 0.0;
double setPoint = 200.0;
double output = 0.0;
AutoPID myPID(&speed, &setPoint, &output, -100, 100, 0.5, 5.0, 0.0);

Motor right_motor(RIGHT_MOTOR_PINS);
Motor left_motor(LEFT_MOTOR_PINS);


void setup() {
Serial.begin(115200);
right_motor.init(10);
//left_motor.init(10);
myPID.setTimeStep(20);
}

void loop() {
//left_motor.set_power(0);

speed = rad2rpm(right_motor.get_speed());
myPID.run();
right_motor.set_power(output);
}

#include "Motor.h"
#include "helpers.h"
#include "pinout.h"



Motor motor(C1_RIGHT, C2_RIGHT, PWM_RIGHT, M1_RIGHT, M2_RIGHT, STBY_RIGHT);


void setup() {
motor.init(10);
}

void loop() {
motor.set_power(20);
}

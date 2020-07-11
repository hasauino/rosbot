#include "Motor.h"
#include "helpers.h"
#include "pinout.h"



Motor right_motor(RIGHT_MOTOR_PINS);
Motor left_motor(LEFT_MOTOR_PINS);


void setup() {
Serial.begin(115200);
right_motor.init(10);
left_motor.init(10);
}

void loop() {
right_motor.set_power(0);
left_motor.set_power(0);

Serial.print( rad2deg(left_motor.get_position()) );
Serial.print( "   |    " );
Serial.println( rad2deg(right_motor.get_position()) );
delay(100);
}

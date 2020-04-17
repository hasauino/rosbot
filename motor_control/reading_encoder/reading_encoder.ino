#include "Motor.h"
#include "helpers.h"
#include <AutoPID.h>

float data[1];
double speed = 0.0;
double setPoint = 20.0;
double output = 0.0;
AutoPID myPID(&speed, &setPoint, &output, -100, 100, 1.0, 10.0, 0.0);


Motor motor;
Plotter plot(100);



void setup() {
plot.begin(115200);  
motor.init(10);
myPID.setTimeStep(1);
}

void loop() {
motor.set_power(200);

data[0] = motor.get_speed();
plot.update(data, 1);
}

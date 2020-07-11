#include "Motor.h"
#include "helpers.h"
#include <AutoPID.h>

float data[2];
double speed = 0.0;
double setPoint = 20.0;
double output = 0.0;
AutoPID myPID(&speed, &setPoint, &output, -100, 100, 1.0, 20.0, 0.0);


Motor motor;
Plotter plot(100);



void setup() {
//plot.begin(115200);  
motor.init(10);
//myPID.setTimeStep(1);
}

void loop() {
  motor.set_power(0);

//setPoint = 250*sin(millis()*0.001*0.1*2*PI);
//  
//speed = rad2rpm(motor.get_speed());
//myPID.run();
//motor.set_power(output);
//
//data[0] = speed;
//data[1] = setPoint;
//plot.update(data, 2);

}

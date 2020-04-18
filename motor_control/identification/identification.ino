#include "Motor.h"
#include "helpers.h"
#include <AutoPID.h>

float data[3];
double speed = 0.0;
double setPoint = 20.0;
double output = 0.0;
float power = 0.0;
AutoPID myPID(&speed, &setPoint, &output, -100, 100, 1.0, 10.0, 0.0);



Motor motor;
Plotter plot(100);

float freq=0.1;
float interval;
unsigned long t=0, t0;

void setup() {
plot.begin(115200);  
motor.init(10);
myPID.setTimeStep(1);
}

void loop() {
while(Serial.read()!=10);
t0 = micros();
while (freq<=10){
interval = 3000000.0*(1/freq);
t = micros();
while((micros() - t) < interval){
  
data[1] = 100*sin((micros()-t)*0.000001*2.0*PI*freq);
motor.set_power(data[1]);
data[0] = motor.get_speed();
data[2] = freq;

plot.update(data, 3);
}
freq += 0.1;
}
  
motor.set_power(0);
}

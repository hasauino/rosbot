#include "Motor.h"
#include "helpers.h"

Motor motor;
Plotter plot(100,2);

void setup() {
plot.begin(115200);  
motor.init(10);
motor.set_power(100);
}

void loop() {
plot.data[0] = 100.0;
plot.data[1] = rad2rpm(motor.get_speed());
plot.update();
}

#include "IMU.h"

#define ACC_SENSITIVITY 1670.132518 // LBS / (m/s2)
#define GYRO_SENSITIVITY 131.0 // LBS / (degree/s)

IMU imu;

int16_t ax, ay, az;
int16_t gx, gy, gz;




void setup() {
  Wire.begin();
  imu.init();
  Serial.begin(115200);
}

void loop() {
  imu.getData(ax, ay, az, gx, gy, gz);


  Serial.println(imu.getTemp());


  delay(100);

}

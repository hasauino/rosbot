#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"


MPU6050 imu;

int16_t ax, ay, az;
int16_t gx, gy, gz;


#define LED_PIN 13


void setup() {
  Wire.begin();
  Serial.begin(115200);

  imu.initialize();
  imu.setXAccelOffset(-3165);
  imu.setYAccelOffset(-778);
  imu.setZAccelOffset(1410);
  imu.setXGyroOffset(25);
  imu.setYGyroOffset(-26);
  imu.setZGyroOffset(-47);

  Serial.println("Testing device connections...");
  Serial.println(imu.testConnection() ?
                 "MPU6050 connection successful" : "MPU6050 connection failed");

}

void loop() {
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);


  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  delay(100);

}

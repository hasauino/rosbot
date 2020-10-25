#include "I2Cdev.h"
#include "MPU6050_mod.h"
#include "Wire.h"

#define ACC_SENSITIVITY 1670.132518 // LBS / (m/s2)
#define GYRO_SENSITIVITY 131.0 // LBS / (degree/s)
#define TEMP_SENSITIVITY 340.0
#define TEMP_OFFSET 35.0 // degree C
class IMU {

  public:
    void init() {
      Wire.begin();
      imu.initialize();
      imu.setXAccelOffset(-3165);
      imu.setYAccelOffset(-778);
      imu.setZAccelOffset(1408);
      imu.setXGyroOffset(25);
      imu.setYGyroOffset(-27);
      imu.setZGyroOffset(-47);

      imu.CalibrateAccel(6, true);
      imu.CalibrateGyro(6);
      imu.PrintActiveOffsets();


      Serial.println("Testing device connections...");
      Serial.println(imu.testConnection() ?
                     "MPU6050 connection successful" : "MPU6050 connection failed");

    }

    void getData(int16_t &_ax, int16_t &_ay, int16_t &_az, int16_t &_gx, int16_t  &_gy, int16_t &_gz) {
      imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      _ax = ax;
      _ay = ay;
      _az = az;
      _gx = gx;
      _gy = gy;
      _gz = gz;
    }

    float getAccelX() {
      return imu.getAccelerationX() / ACC_SENSITIVITY;
    }
    float getAccelY() {
      return imu.getAccelerationY() / ACC_SENSITIVITY;
    }
    float getAccelZ() {
      return imu.getAccelerationZ() / ACC_SENSITIVITY;
    }
    float getGyroX() {
      return imu.getRotationX() / GYRO_SENSITIVITY;
    }
    float getGyroY() {
      return imu.getRotationY() / GYRO_SENSITIVITY;
    }
    float getGyroZ() {
      return imu.getRotationZ() / GYRO_SENSITIVITY;
    }

    float getTemp() {
      return (imu.getTemperature() / TEMP_SENSITIVITY) + TEMP_OFFSET;
    }

  private:
    MPU6050_MOD imu;
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
};

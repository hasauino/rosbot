#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

class MPU6050_MOD: public MPU6050 {

  public:
    void CalibrateAccel(uint8_t Loops, bool invert_z = false) {

      float kP = 0.3;
      float kI = 20;
      float x;
      x = (100 - map(Loops, 1, 5, 20, 0)) * .01;
      kP *= x;
      kI *= x;
      PID( 0x3B, kP, kI,  Loops, invert_z);
    }

    void PID(uint8_t ReadAddress, float kP, float kI, uint8_t Loops, bool invert_z = false) {
      uint8_t SaveAddress = (ReadAddress == 0x3B) ? ((getDeviceID() < 0x38 ) ? 0x06 : 0x77) : 0x13;

      int16_t  Data;
      float Reading;
      int16_t BitZero[3];
      uint8_t shift = (SaveAddress == 0x77) ? 3 : 2;
      float Error, PTerm, ITerm[3];
      int16_t eSample;
      uint32_t eSum ;
      Serial.write('>');
      for (int i = 0; i < 3; i++) {
        I2Cdev::readWords(MPU6050_ADDRESS_AD0_LOW, SaveAddress + (i * shift), 1, (uint16_t *)&Data); // reads 1 or more 16 bit integers (Word)
        Reading = Data;
        if (SaveAddress != 0x13) {
          BitZero[i] = Data & 1;                     // Capture Bit Zero to properly handle Accelerometer calibration
          ITerm[i] = ((float)Reading) * 8;
        } else {
          ITerm[i] = Reading * 4;
        }
      }
      for (int L = 0; L < Loops; L++) {
        eSample = 0;
        for (int c = 0; c < 100; c++) {// 100 PI Calculations
          eSum = 0;
          for (int i = 0; i < 3; i++) {
            I2Cdev::readWords(MPU6050_ADDRESS_AD0_LOW, ReadAddress + (i * 2), 1, (uint16_t *)&Data); // reads 1 or more 16 bit integers (Word)
            Reading = Data;
            if (invert_z) {
              if ((ReadAddress == 0x3B) && (i == 2)) Reading -= -16384; //remove Gravity
            } else {
              if ((ReadAddress == 0x3B) && (i == 2)) Reading -= 16384; //remove Gravity
            }

            Error = -Reading;
            eSum += abs(Reading);
            PTerm = kP * Error;
            ITerm[i] += (Error * 0.001) * kI;       // Integral term 1000 Calculations a second = 0.001
            if (SaveAddress != 0x13) {
              Data = round((PTerm + ITerm[i] ) / 8);    //Compute PID Output
              Data = ((Data) & 0xFFFE) | BitZero[i];  // Insert Bit0 Saved at beginning
            } else Data = round((PTerm + ITerm[i] ) / 4); //Compute PID Output
            I2Cdev::writeWords(MPU6050_ADDRESS_AD0_LOW, SaveAddress + (i * shift), 1, (uint16_t *)&Data);
          }
          if ((c == 99) && eSum > 1000) {         // Error is still to great to continue
            c = 0;
            Serial.write('*');
          }
          if ((eSum * ((ReadAddress == 0x3B)?.05 : 1)) < 5) eSample++; // Successfully found offsets prepare to  advance
          if ((eSum < 100) && (c > 10) && (eSample >= 10)) break;   // Advance to next Loop
          delay(1);
        }
        Serial.write('.');
        kP *= .75;
        kI *= .75;
        for (int i = 0; i < 3; i++) {
          if (SaveAddress != 0x13) {
            Data = round((ITerm[i] ) / 8);    //Compute PID Output
            Data = ((Data) & 0xFFFE) | BitZero[i]; // Insert Bit0 Saved at beginning
          } else Data = round((ITerm[i]) / 4);
          I2Cdev::writeWords(MPU6050_ADDRESS_AD0_LOW, SaveAddress + (i * shift), 1, (uint16_t *)&Data);
        }
      }
    }
};

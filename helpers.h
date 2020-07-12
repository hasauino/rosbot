#ifndef HELPERS_H
#define HELPERS_H

#define rad2deg(x) x*180.0/PI
#define rad2rpm(x) x*(30.0/PI)
#define rpm2rad(x) x*2.0*PI/60.0
#include <stdarg.h>
#include <Arduino.h>


#define RIGHT_MOTOR_PINS C1_RIGHT, C2_RIGHT, PWM_RIGHT, M1_RIGHT, M2_RIGHT, STBY_RIGHT
#define LEFT_MOTOR_PINS C1_LEFT, C2_LEFT, PWM_LEFT, M1_LEFT, M2_LEFT, STBY_LEFT

#define MAX_VAL_TIME 4294967296

// class used to reutrn duration in microseconds between calls, even if an overflow occurs to micros()
class MicrosecondsTimer {
  private:
    unsigned long elapsed_us = 0;
    unsigned long temp;
  public:
    MicrosecondsTimer() {
    }
    void init() {
      elapsed_us = micros();
    }
    unsigned long duration() {
      if (elapsed_us > micros()) {
        temp = MAX_VAL_TIME - elapsed_us + micros();
      }
      else {
        temp = micros() - elapsed_us;
      }
      elapsed_us = micros();
      return temp;
    }
};


class Plotter {
  private:
    unsigned long timer;
    int del_t;
  public:
    Plotter(int rate) {
      del_t = 1000.0 / float(rate);
    }
    void  begin(unsigned long baud) {
      Serial.begin(baud);
      timer = millis();
    }
    void update(float data[], int size) {
      if (millis() - timer >= del_t) {
        timer = millis();
        Serial.print("$");
        for (int i = 0; i < size - 1; ++i) {
          Serial.print(data[i]);
          Serial.print(" ");
        }
        Serial.print(data[size - 1]);
        Serial.print(";");
        Serial.println();
      }
    }


};//Plotter class

#endif

#define rad2deg(x) x*180.0/PI
#define rad2rpm(x) x*(30.0/PI)
#include <stdarg.h>


#define RIGHT_MOTOR_PINS C1_RIGHT, C2_RIGHT, PWM_RIGHT, M1_RIGHT, M2_RIGHT, STBY_RIGHT
#define LEFT_MOTOR_PINS C1_LEFT, C2_LEFT, PWM_LEFT, M1_LEFT, M2_LEFT, STBY_LEFT

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
        for (int i = 0; i < size-1; ++i) {
          Serial.print(data[i]);
          Serial.print(" ");
        }
        Serial.print(data[size-1]);
        Serial.print(";");
      Serial.println();
      }
    }


};//Plotter class

#define rad2deg(x) x*180.0/PI
#define rad2rpm(x) x*(30.0/PI)
#include <stdarg.h>

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

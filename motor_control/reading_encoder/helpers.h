#define rad2deg(x) x*180.0/PI
#define rad2rpm(x) x*(30.0/PI)
#include <stdarg.h>

class Plotter {
  private:
    unsigned long timer;
    int del_t;
    int num_args;
  public:
    float *data;
    Plotter(int rate, unsigned int _num_args) {
      del_t = 1000.0 / float(rate);
      num_args = _num_args;
    }
    void  begin(unsigned long baud) {
      Serial.begin(baud);
      timer = millis();
    }
    void update() {
      if (millis() - timer >= del_t) {
        timer = millis();
        Serial.print("$");
        for (int i = 0; i < num_args-1; ++i) {
          Serial.print(data[i]);
          Serial.print(" ");
        }
        Serial.print(data[num_args-1]);
        Serial.print(";");
      }
    }


};//Plotter class

#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#define GEAR_ENCODER_RATIO 880.0 // 20 (gear ration)x4 (quar encoder) x11 (encoder resolution)
#define MAX_VAL_TIME 4294967296

class Motor {
  private:
    int M1, M2, PWM, STBY;
    Encoder encoder;
    unsigned long elapsed_us = 0;
    unsigned long speed_timer = 0;
    unsigned long speed_sampling_time = 0;
    long count = 0;
    float delta_angle = 0.0;
    float speed = 0.0;
    unsigned long temp;
    unsigned long us_duration(unsigned long &val);
  public:
    Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY);
    // smapling time in milliseconds
    void init(int _speed_sampling_time = 10);
    long get_count();
    float get_position();
    float get_speed();
    void set_power(float pwr);
};//Motor class
#endif

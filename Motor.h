#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "helpers.h"
#define GEAR_ENCODER_RATIO 880.0 // 20 (gear ration)x4 (quar encoder) x11 (encoder resolution)
#include <AutoPID.h>

#define KP 0.0
#define KI 50.0
#define KD 0.0
#define MAX_OUT 100
#define MIN_OUT 0


class Motor {
  private:
    int M1, M2, PWM, STBY;
    Encoder encoder;
    unsigned long speed_timer = 0;
    unsigned long speed_sampling_time = 0;
    long count = 0;
    float delta_angle = 0.0;
    double speed = 0.0;
    double speed_setpoint = 0.0;
    double output = 0.0;
    AutoPID speedPID;
    MicrosecondsTimer speed_microtimer;
  public:
    Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY);
    // smapling time in milliseconds
    void init(int _speed_sampling_time = 10);
    long get_count();
    float get_position();
    float get_speed();
    void set_power(double pwr);

    void set_speed(float _speed_setpoint) {
    get_speed();
    speed_setpoint = rpm2rad(_speed_setpoint);
    speedPID.run();
    set_power(output);
    }

};//Motor class
#endif

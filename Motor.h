#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "helpers.h"
#include <AutoPID.h>

#define GEAR_ENCODER_RATIO 880.0 // 20 (gear ration)x4 (quar encoder) x11 (encoder resolution)
#define KP 0.0
#define KI 50.0
#define KD 0.0
#define MAX_OUT 100
#define MIN_OUT 0

class Sample
{
private:
  unsigned long sampling_time{0};
  unsigned long timer{0};
  MicrosecondsTimer microtimer;

public:
  double speed = 0.0;
  float delta_angle = 0.0;
  long count = 0;

  void init(unsigned long _sampling_time)
  {
    sampling_time = _sampling_time;
    timer = millis();
    microtimer.init();
  }
  void reset()
  {
    speed = 0.0;
    count = 0;
    delta_angle = 0.0;
  }

  void update(Encoder &encoder)
  {
    if (millis() - timer >= sampling_time)
    {
      delta_angle = ((encoder.read() - count) / GEAR_ENCODER_RATIO) * 2.0 * PI;
      count = encoder.read();
      speed = (delta_angle / microtimer.duration()) * 1000000.0;
      timer = millis();
    }
  }
};

class Motor
{

private:
  int M1, M2, PWM, STBY;
  Encoder encoder;
  Sample fast_sample;
  Sample slow_sample;
  double abs_speed = 0.0;
  double speed_setpoint = 0.0;
  double output = 0.0;
  AutoPID speedPID;

  long get_count();

public:
  Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY);

  // sampling time in milliseconds
  void init(int _sampling_time = 10, int control_step_time = 5);

  float get_position();

  float delta_theta(bool fast = false);

  float get_speed(bool fast = false);

  void set_power(double pwr);

  void set_speed(float _speed_setpoint);

  void reset();

private:
  void update();
}; // Motor class

#endif

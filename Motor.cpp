#include "Motor.h"

Motor::Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY)
    : encoder(C1, C2),
      speedPID(&abs_speed, &speed_setpoint, &output, MIN_OUT, MAX_OUT, KP, KI, KD)
{
  M1 = _M1;
  M2 = _M2;
  PWM = _PWM;
  STBY = _STBY;
}

void Motor::init(int _sampling_time, int control_step_time)
{
  encoder.write(0);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(PWM, OUTPUT);
  digitalWrite(M2, LOW);
  digitalWrite(M1, LOW);
  digitalWrite(PWM, LOW);
  digitalWrite(STBY, HIGH);
  speed_microtimer.init();
  /*
  a sampling time of 100 ms (10 hz) results in error of 0.43 % in computed travel distance at robot speed of 1 m/s
  a sampling time of 50  ms (20 hz) results in error of 0.86 % in computed travel distance at robot speed of 1 m/s
  */
  sampling_time = _sampling_time;
  speedPID.setTimeStep(control_step_time);
}

long Motor::get_count()
{
  return encoder.read();
}

float Motor::get_position()
{
  return (encoder.read() / GEAR_ENCODER_RATIO) * 2.0 * PI;
}

void Motor::sample()
{
  if (millis() - speed_timer >= sampling_time)
  {
    delta_angle = ((encoder.read() - count) / GEAR_ENCODER_RATIO) * 2.0 * PI;
    count = encoder.read();
    speed = (delta_angle / speed_microtimer.duration()) * 1000000.0;
    speed_timer = millis();
  }
}

float Motor::delta_theta()
{
  sample();
  return delta_angle;
}

float Motor::get_speed()
{
  sample();
  return speed;
}

void Motor::set_power(double pwr)
{
  if (pwr > 0)
  {
    digitalWrite(M2, HIGH);
    digitalWrite(M1, LOW);
  }
  else if (pwr < 0)
  {
    digitalWrite(M2, LOW);
    digitalWrite(M1, HIGH);
  }
  if (abs(pwr) <= 100)
  {
    analogWrite(PWM, 255 * (abs(pwr) / 100.00));
  }
  else
  {
    analogWrite(PWM, 255);
  }
}

void Motor::set_speed(float _speed_setpoint)
{
  abs_speed = abs(get_speed());
  speed_setpoint = abs(_speed_setpoint);
  // reset PID (I and D terms) upon stopping the motor
  if (speed_setpoint == 0)
  {
    speedPID.reset();
  }
  speedPID.run();
  set_power(output * sign(_speed_setpoint));
}

void Motor::reset()
{
  encoder.write(0);
  count = 0;
  delta_angle = 0.0;
  speed = 0.0;
  abs_speed = 0.0;
  speed_setpoint = 0.0;
  output = 0.0;
  speedPID.reset();
}

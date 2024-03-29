#include "Motor.h"



Motor::Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY)
  : encoder(C1, C2),
    speedPID(&abs_speed, &speed_setpoint, &output, MIN_OUT, MAX_OUT, KP, KI, KD) {
  M1 = _M1;
  M2 = _M2;
  PWM = _PWM;
  STBY = _STBY;
}


void Motor::init(int _speed_sampling_time = 10) {
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
  speed_sampling_time = _speed_sampling_time;
  speedPID.setTimeStep(2 * speed_sampling_time);
}

long Motor::get_count() {
  return encoder.read();
}

float Motor::get_position() {
  return (encoder.read() / GEAR_ENCODER_RATIO) * 2.0 * PI;
}

float Motor::delta_theta() {
  _delta_angle = ((encoder.read() - _count) / GEAR_ENCODER_RATIO) * 2.0 * PI;
  _count = encoder.read();
  return _delta_angle;
}

float Motor::get_speed() {
  if (millis() - speed_timer >= speed_sampling_time) {
    delta_angle = ((encoder.read() - count) / GEAR_ENCODER_RATIO) * 2.0 * PI;
    count = encoder.read();
    speed = (delta_angle / speed_microtimer.duration()) * 1000000.0;
    speed_timer = millis();
  }
  return speed;
}

void Motor::set_power(double pwr) {
  if (pwr > 0) {
    digitalWrite(M2, HIGH);
    digitalWrite(M1, LOW);
  }
  else if (pwr < 0) {
    digitalWrite(M2, LOW);
    digitalWrite(M1, HIGH);
  }
  if (abs(pwr) <= 100) {
    analogWrite(PWM, 255 * (abs(pwr) / 100.00));
  }
  else {
    analogWrite(PWM, 255);
  }
}

void Motor::set_speed(float _speed_setpoint) {
  abs_speed = abs(get_speed());
  speed_setpoint = abs(_speed_setpoint);
  // reset PID (I and D terms) upon stopping the motor
  if (speed_setpoint == 0){speedPID.reset();}
  speedPID.run();
  set_power(output * sign(_speed_setpoint));
}


void Motor::reset() {
  encoder.write(0);
  count = 0;
  delta_angle = 0.0;
  speed = 0.0;
  abs_speed = 0.0;
  speed_setpoint = 0.0;
  output = 0.0;
  speedPID.reset();
}

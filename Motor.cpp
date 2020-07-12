#include "Motor.h"



unsigned long Motor::us_duration(unsigned long &val) {
  if (val > micros()) {
    temp = MAX_VAL_TIME - val + micros();
  }
  else {
    temp = micros() - val;
  }
  val = micros();
  return temp;
}


Motor::Motor(int C1, int C2, int _PWM, int _M1, int _M2, int _STBY): encoder(C1, C2) {
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
  elapsed_us = micros();
  speed_sampling_time = _speed_sampling_time;
}

long Motor::get_count() {
  return encoder.read();
}

float Motor::get_position() {
  return (encoder.read() / GEAR_ENCODER_RATIO) * 2.0 * PI;
}

float Motor::get_speed() {
  if (millis() - speed_timer >= speed_sampling_time) {
    delta_angle = ((encoder.read() - count) / GEAR_ENCODER_RATIO) * 2.0 * PI;
    count = encoder.read();
    speed = (delta_angle / us_duration(elapsed_us)) * 1000000.0;
    speed_timer = millis();
  }
  return speed;
}

void Motor::set_power(float pwr) {
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

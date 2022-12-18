#include "Robot.h"

Robot::Robot() : right_motor(RIGHT_MOTOR_PINS),
                 left_motor(LEFT_MOTOR_PINS) {}

void Robot::init()
{
  right_motor.init(ENCODER_SAMPLING, MOTOR_CONTROL_LOOP_DURATION);
  left_motor.init(ENCODER_SAMPLING, MOTOR_CONTROL_LOOP_DURATION);
  head.attach(HEAD_SERVO);
  head.write(HEAD_INIT);
}

void Robot::set_speed(float v, float w)
{
  wr = -(v + 0.5 * ROBOT_WIDTH * w) / WHEEL_RADIUS; // v/w (negative since motor rotation is opposite to robot motion)
  wl = (v - 0.5 * ROBOT_WIDTH * w) / WHEEL_RADIUS;
  right_motor.set_speed(wr);
  left_motor.set_speed(wl);
}

void Robot::set_rightspeed(float w)
{
  right_motor.set_speed(w);
}

void Robot::set_leftspeed(float w)
{
  left_motor.set_speed(w);
}

float Robot::get_rightspeed()
{
  return right_motor.get_speed();
}

float Robot::get_leftspeed()
{
  return left_motor.get_speed();
}

float Robot::get_leftposition()
{
  return left_motor.get_position();
}

float Robot::get_rightposition()
{
  return right_motor.get_position();
}

float Robot::delta_s_r()
{
  return right_motor.delta_theta() * WHEEL_RADIUS;
}

float Robot::delta_s_l()
{
  return left_motor.delta_theta() * WHEEL_RADIUS;
}

void Robot::set_head(int _angle)
{
  if (_angle != head_angle && _angle <= 180 && _angle >= 0)
  {
    head_angle = _angle;
    head.write(head_angle);
  }
}

void Robot::reset()
{
  right_motor.reset();
  left_motor.reset();
}

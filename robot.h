#ifndef
#define ROBOT_H

#include "Motor.h"
#include "pinout.h"

#define ENCODER_SPEED_SAMPLING 3 // milliseconds, do not update speed if get_speed is called faster than this
#define ROBOT_WIDTH 0.12 // meters (120 mm)
#define WHEEL_RADIUS 0.02783619955

class Robot {

  private:
    Motor right_motor;
    Motor left_motor;
    float wr = 0.0;
    float wl = 0.0;

  public:
    Robot():
      right_motor(RIGHT_MOTOR_PINS),
      left_motor(LEFT_MOTOR_PINS) {
      right_motor.init(ENCODER_SPEED_SAMPLING);
      left_motor.init(ENCODER_SPEED_SAMPLING);
    }

    /* set robot speed. Takes: v (m/s linear speed in forward direction),
       and w (rad/s, angular speed around z axis).
    */
    void set_speed(float v, float w) {
      wr = -(v + 0.5 * ROBOT_WIDTH * w)/WHEEL_RADIUS; // v/w (negative since motor rotation is opposite to robot motion)
      wl = (v - 0.5 * ROBOT_WIDTH * w)/WHEEL_RADIUS;
      right_motor.set_speed(wr);
      left_motor.set_speed(wl);
    }

}




#endif

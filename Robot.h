#ifndef ROBOT_H
#define ROBOT_H

#include <Servo.h>
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
    int head_angle = 90;
    Servo head;

  public:
    Robot();

    void init();
    /* set robot speed. Takes: v (m/s linear speed in forward direction),
       and w (rad/s, angular speed around z axis).
    */
    void set_speed(float v, float w);

    void set_rightspeed(float w);
    void set_leftspeed(float w);
    float get_rightspeed();
    float get_leftspeed();
    float get_leftposition();
    float get_rightposition();
    float delta_s_r();
    float delta_s_l();
    void set_head(int _angle);

    // reset motors and odometry
    void reset();

};




#endif

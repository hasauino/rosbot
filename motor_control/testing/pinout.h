/* Pins availability
//Pin:      0   1   2   3   4   5   6   7   8   9   10    11    12    13    A0    A1    A2    A3    A4    A5    A6    A7
//state:    X   X   X   X   X   X   X   X   X   X   X     X     X     X     X     X                 F     F     F     F

/*
Right Motor
*/
#define PWM_RIGHT 5 //motor PWM terminal. PWM freq 980 Hz
#define M2_RIGHT 10 //motor ccw pin
#define M1_RIGHT 11
#define STBY_RIGHT 4
#define C1_RIGHT 2 //Encoder 1st pin
#define C2_RIGHT 7 //Encoder 1st pin
/*
Left Motor
*/
#define PWM_LEFT 6 //motor PWM terminal. PWM freq 980 Hz
#define M2_LEFT 12 //motor ccw pin
#define M1_LEFT 13
#define STBY_LEFT A1
#define C1_LEFT 3 //Encoder 1st pin
#define C2_LEFT 8 //Encoder 1st pin
/*
Head Servo
*/
#define HEAD_SERVO 9
/*
Battery voltage
*/
#define BATTERY_VOLTAGE A0
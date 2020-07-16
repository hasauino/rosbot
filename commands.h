#ifndef COMMANDS_H
#define COMMANDS_H

#include "protocol.h"

class Velocity {
  private:
    union {
      float asFloat;
      byte asBytes[4];
    } V;
    union {
      float asFloat;
      byte asBytes[4];
    } W;
  public:
    static const byte length = 9;
    static const byte ID = VELOCITY_COMMAND;
    float v = 0.0, w = 0.0;
    void deserialize(byte &data) {
      V.asBytes[0] = 0x3E;//(&data)[0];
      V.asBytes[1] = 0x99;//(&data)[1];
      V.asBytes[2] = 0x99;//(&data)[2];
      V.asBytes[3] = 0x9A;//(&data)[3];
      W.asBytes[0] = (&data)[4];
      W.asBytes[1] = (&data)[5];
      W.asBytes[2] = (&data)[6];
      W.asBytes[3] = (&data)[7];

      v = V.asFloat;
      w = W.asFloat;
    }
};

//-------------------



#endif

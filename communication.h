/*
  Serial Protocol
  Message format:
  MSG_First_Byte  MSG_Second_Byte   MSG_Length    MSG_Type  <argument bytes..> Checksum
*/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "protocol.h"
#include <Arduino.h>

class Receiver {

  private:
    /*
      location1 and location 2 will both be used interchangably to store new bytes being buffered
      and the last message.
      "buffer" points to one of these array locations, once the received sequence of bytes form a complete message,
      it will switch to point to the other location, where the original location will be pointed at by the "lates_message".
      So they swap everytime after a complete messgae is received.
    */
    byte location1[MSG_BUFFER_LENGTH];
    byte location2[MSG_BUFFER_LENGTH];
    byte* buffer = location1;
    byte* latest_message = location2;
    int index = 0;
    bool is_there_new_msg = false;
    // temporary variables (ex: lpop counters ...etc)
    int i = 0;


  public:
    Receiver() {}

    void update(int received_byte) {

      if (index > 1) {
        buffer[index++] = received_byte;
      }


      else if (index == 1) {
        if (received_byte == MSG_SECOND_BYTE) {
          buffer[index++] = MSG_SECOND_BYTE;
        }
        else {
          index = 0;
          buffer[0] = 0; // wrong msg, reset first byte
        }
      }


      else if (index == 0) {
        if (received_byte == MSG_FIRST_BYTE) {
          buffer[index++] = MSG_FIRST_BYTE;
        }
      }

      if (index > 2) {
        if (index > buffer[2] + 2) {
          // if checksum is correct --- TODO
          if (true) {
            is_there_new_msg = true;
            if (buffer == location1) {
              buffer = location2;
              latest_message = location1;
            } else {
              buffer = location1;
              latest_message = location2;
            }

          }

          index = 0; // end of msg, reset index
          buffer[0] = 0; // end of msg, reset first byte
          buffer[1] = 0; // reset second byte
        }
      }
    }//update_buffer

    byte* latest_msg() {
      is_there_new_msg = false;
      return latest_message;
    }

    bool available() {
      return is_there_new_msg;
    }
};


class Transmitter {

  private:
    unsigned int rate = 20; // transmission rate in Hz
    byte index = 0;
    unsigned long timer = 0;
    byte buffer[MSG_BUFFER_LENGTH];
    union {
      float asFloat;
      byte asBytes[4];
    } float_variable;

    byte int_serialized[2];

    byte byte_serialized[1];

    byte* serialize(float var) {
      float_variable.asFloat = var;
      return float_variable.asBytes;
    }
    byte* serialize(int var) {
      int_serialized[0] = var;
      int_serialized[1] = var>>8;
      return int_serialized;
    }

    byte* serialize(byte var) {
      byte_serialized[0] = var;
      return byte_serialized;
    }    

    void push_data(byte &data, unsigned int length) {
      for (int i = 1; i <= length; i++) {
        buffer[index] = (&data)[index];
        index++;
      }
    }


  public:
    void _push_data(byte &data, unsigned int length) {
      for (int i = 1; i <= length; i++) {
        buffer[index] = (&data)[index];
        index++;
      }
    }
    
    Transmitter() {}

    void push(float var) {
      byte* data = serialize(var);
      push_data(data[0], 4); // this is CPU dependent (float assumed 4 bytes in size)
    }

    void push(int var) {
      byte* data = serialize(var);
      push_data(data[0], 2); // this is CPU dependent (int assumed 2 bytes in size)
    }
    void push(byte var) {
      byte* data = serialize(var);
      push_data(data[0], 1);
    }

    void flush() {
      index = 0;
    }
    void send() {
      if (millis() - timer > 1000.0 / float(rate)) {
        timer = millis();
        Serial.write(MSG_FIRST_BYTE);
        Serial.write(MSG_SECOND_BYTE);
        Serial.write(index);
        for (int i = 0; i < index; i++) {
          Serial.write(buffer[i]);
        }
        Serial.write(0); //TODO: Checksum
      }
      flush();
    }
};
#endif

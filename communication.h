/*
  Serial Protocol
  Message format:
  MSG_First_Byte  MSG_Second_Byte   MSG_Length    MSG_Type  <argument bytes..> Checksum
*/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define MSG_FIRST_BYTE 123 // first byte in received message (0x7B)
#define MSG_SECOND_BYTE 55 // second byte in received message (0x37)
#define MSG_BUFFER_LENGTH 50

/* commands */
#define SET_VELOCITY 'V' // message type is command

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

#endif

/*
  Serial Protocol
  Message format:
  MSG_First_Byte  MSG_Second_Byte   MSG_Length    MSG_Type  <argument bytes..> Checksum
*/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define MSG_FIRST_BYTE 123 // first byte in received message
#define MSG_SECOND_BYTE 55 // second byte in received message

/* commands */
#define SET_VELOCITY 'V' // message type is command

/*
  class Receiver() {
  private:

  public:
  Receiver(){}
  }
*/
#endif

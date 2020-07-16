#ifndef PROTOCOL_H
#define PROTOCOL_H


#define MSG_FIRST_BYTE 0x7B // first byte in received message
#define MSG_SECOND_BYTE 0x37 // second byte in received message
#define MSG_BUFFER_LENGTH 50

/* command IDs */
#define VELOCITY_COMMAND 0x00
#define HEAD_COMMAND 0x01
#endif

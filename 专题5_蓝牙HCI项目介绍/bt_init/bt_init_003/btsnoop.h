#ifndef BTSNOOP_H_H_H
#define BTSNOOP_H_H_H


#include <stdint.h>


#define TRANSPORT_TYPE_CMD 1
#define TRANSPORT_TYPE_ACL 2
#define TRANSPORT_TYPE_SCO 3
#define TRANSPORT_TYPE_EVT 4


uint8_t btsnoop_open(uint8_t *file_naname);
uint8_t btsnoop_close(void);
uint8_t btsnoop_write(uint8_t type,uint8_t in,uint8_t *data,uint16_t data_len);


#endif



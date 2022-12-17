#ifndef UART_H_H_H
#define UART_H_H_H

#include <stdint.h>

void uart_bt_open(void);
void uart_bt_close(void);
int uart_bt_send(uint8_t *data, uint16_t len);
int uart_bt_read(uint8_t *data, uint16_t len);

#endif
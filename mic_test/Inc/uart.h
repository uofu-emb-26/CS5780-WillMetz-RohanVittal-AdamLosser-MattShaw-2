#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void transmit_byte(uint8_t byte);

extern UART_HandleTypeDef huart3;

#endif /* UART_H */
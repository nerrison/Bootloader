#ifndef RECEIVE_UART_H
#define RECEIVE_UART_H

#include <stdint.h>

#define RX_BUFFER_SIZE 256U

void RX_Init(void);
void RX_Start(void);
uint16_t RX_Process(uint8_t **data);

extern uint8_t rx_Buffer[RX_BUFFER_SIZE];

#endif
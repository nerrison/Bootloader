#ifndef STM32F4XX_IT_H
#define STM32F4XX_IT_H

#include <stdint.h>

uint8_t UART_DataReady(void);

void UART_GetReceivedData(uint8_t **data, uint16_t *length);

#endif
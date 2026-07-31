#include "usart1.h"
#include "update_process.h"
#include "stm32f4xx.h"
#include "usart1_irq.h"
#include "memory_map.h"

static uint8_t *rxData;
static uint16_t rxLength;
static volatile uint8_t rxReady;

uint8_t UART_DataReady(void)
{
    return rxReady;
}

void UART_GetReceivedData(uint8_t **data, uint16_t *length)
{
    *data = rxData;
    *length = rxLength;
    rxReady = 0;
}

void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_SR_IDLE)
    {
        volatile uint32_t temp;

        temp = USART1->SR;
        temp = USART1->DR;
        (void)temp;

        rxLength = RX_Process(&rxData);

        rxReady = 1;
    }
}
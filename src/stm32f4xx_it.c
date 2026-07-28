#include "receive_uart.h"
#include "flash_writer.h"
#include "stm32f4xx.h"
#include "memory_map.h"

void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_SR_IDLE)
    {
        volatile uint32_t temp;

        temp = USART1->SR;
        temp = USART1->DR;
        (void)temp;

        uint8_t *data;
        uint16_t length;

        length = RX_Process(&data);

         Flash_Write(
            data,
            length
        );
    }
}
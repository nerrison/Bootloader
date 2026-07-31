#include "stm32f4xx.h"

#define UART_BAUD 0x008B


void Init_GPIOD_Clock()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}


void Config_GPIOD_MODE()
{
    /* PD12 */
    GPIOD->MODER &= ~(3U << (12 * 2));
    GPIOD->MODER |=  (1U << (12 *2));
}

void GPIOD_Init()
{
    Init_GPIOD_Clock();
    Config_GPIOD_MODE();
}

void led_on()
{
    GPIOD->BSRR = (1U << 12);
}

void led_off()
{
    GPIOD->BSRR = (1U << (12 + 16));
}


void Init_GPIOA_Clock()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

void Config_GPIOA_Mode()
{
    /* PA9*/
    GPIOA->MODER &= ~(3U << (9 * 2));
    GPIOA->MODER |= (2U << (9 * 2));

    GPIOA->OSPEEDR |= (3U << (9 * 2));

    GPIOA->AFR[1] &= ~(0xF << 4);

    GPIOA->AFR[1] |= (7 << 4);

    GPIOA->PUPDR &= ~(3U << (9 * 2));
}

void USART1_Config()
{
    USART1->CR1 = 0;

    
    USART1->BRR = UART_BAUD;

    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void USART1_Init(void)
{
    Init_GPIOA_Clock();

    Config_GPIOA_Mode();

    USART1_Config();
}

void USART1_SendChar(char c)
{
    while(!(USART1->SR & USART_SR_TXE));

    USART1->DR = c;
}


void USART1_SendString(const char *str)
{
    while(*str)
    {
        USART1_SendChar(*str);
        str++;
    }
}

#define DELAY 10000000U

int main()
{
    GPIOD_Init();
    USART1_Init();

    while(1)
    {
        volatile uint32_t i; 

        led_on();
        USART1_SendString("LED ON \r\n");

        for (i = 0; i < DELAY; i++);

        led_off();
        USART1_SendString("LED OFF \r\n");
        for ( i = 0; i < DELAY; i++);
    }

}
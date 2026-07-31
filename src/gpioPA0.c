#include "stm32f4xx.h"


static void GPIOA_ClockEnable()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

static void GPIOA_Init()
{
    /* input mode*/
    GPIOA->MODER &= ~(3U << (0 * 2));

    /* pull-up */
    GPIOA->PUPDR &= ~(3U << (0 * 2));
    GPIOA->PUPDR |=  (1U << (0 * 2));
}

void ButtonInit(void)
{
    GPIOA_ClockEnable();
    GPIOA_Init();
}

uint8_t  ButtonPressed(void)
{
    return (GPIOA->IDR & GPIO_IDR_ID0) != 0;
}
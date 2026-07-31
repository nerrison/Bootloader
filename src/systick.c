#include "stm32f4xx.h"
#include "systick.h"
#include "usart1.h"

static volatile uint32_t ticks = 0;


void SysTickInit(void)
{

    SysTick->CTRL = 0;

    SysTick->LOAD = (SystemCoreClock / 1000) - 1;

    SysTick->VAL = 0;

    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk |
        SysTick_CTRL_ENABLE_Msk;

}


void SysTick_Handler(void)
{
    ticks++;
}


uint32_t SysTickGetTick(void)
{
    return ticks;
}
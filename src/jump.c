#include "stm32f4xx.h"

#include "jump.h"
#include "memory_map.h"

typedef void (*pFunction)(void);

uint8_t Application_IsValid(void)
{
    uint32_t stack = *(volatile uint32_t *)APPLICATION_ADDRESS;

    return ((stack >= 0x20000000U) &&
            (stack <= 0x20020000U));
}

void JUMP_TO_Program(void)
{
    uint32_t appStack = *(uint32_t *)APPLICATION_ADDRESS;
    uint32_t appEntry = *(uint32_t *)(APPLICATION_ADDRESS + 4);

    /* Disable interrupts */
    __disable_irq();

    /* Stop SysTick */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* Disable all NVIC interrupts */
    for (uint32_t i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* Change vector table */
    SCB->VTOR = APPLICATION_ADDRESS;
    __DSB();
    __ISB();

    /* Set application's stack pointer */
    __set_MSP(appStack);

    /* Jump to application's Reset_Handler */
    ((pFunction)appEntry)();
}
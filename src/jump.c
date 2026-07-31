#include "stm32f4xx.h"

#include "jump.h"
#include "memory_map.h"

typedef void (*pFunction)(void);


void JumpToProgram(void)
{
    uint32_t programStack = *(uint32_t *)PROGRAM_ADDRESS;
    uint32_t programEntry = *(uint32_t *)(PROGRAM_ADDRESS + 4);

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
    SCB->VTOR = PROGRAM_ADDRESS;
    __DSB();
    __ISB();

    /* Set PROGRAM's stack pointer */
    __set_MSP(programStack);

    /* Jump to PROGRAM's Reset_Handler */
    ((pFunction)programEntry)();
}
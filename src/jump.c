/* src/jump.c */
#include "stm32f4xx_hal.h"

#include "memory_map.h"
#include "jump.h"


/* function jump to program*/
void JUMP_TO_Program(void)
{
    uint32_t appStack;
    uint32_t appEntry;

    appStack = *(volatile uint32_t*)APPLICATION_ADDRESS;
    appEntry = *(volatile uint32_t*)(APPLICATION_ADDRESS + 4U);

    /* Function pointer */
    void (*jump)(void);

    jump = (void (*)(void))appEntry;
    
    /* Disable interrupts */
    __disable_irq();


    /* Stop SysTick */
    SysTick->CTRL  = 0;
    SysTick->LOAD  = 0;
    SysTick->VAL   = 0;

    /* Deinitialize peripherals */
    HAL_DeInit();

    /* Relocate the vector table (System Control Block - Vector Table Offset Register)*/
    SCB->VTOR = APPLICATION_ADDRESS;


    /* Set the main stack pointer (MSP) */
    __set_MSP(appStack);

    


    /* jump to program*/
    jump();

}
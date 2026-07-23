/* src/main.c */

#include "stm32f4xx_hal.h"

#include "memory_map.h"
#include "jump.h"

int main(void)
{
    SCB->VTOR = BOOTLOADER_ADDRESS;
    HAL_Init();


    uint32_t appStack = *(volatile uint32_t*)APPLICATION_ADDRESS;

    if((appStack >= 0x20000000) && (appStack <= 0x20020000))
    {

       JUMP_TO_Program();
    }

 
    while(1)
    {

    }

    
}

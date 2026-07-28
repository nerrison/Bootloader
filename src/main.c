#include "stm32f4xx_hal.h"

#include "jump.h"



int main(void)
{
    HAL_Init();


    if (Application_IsValid())
    {
        JUMP_TO_Program();
    }

    /*
     * No application found.
     * Stay in bootloader.
     */

    while (1)
    {
        /* Bootloader code */

        HAL_Delay(500);
    }
}
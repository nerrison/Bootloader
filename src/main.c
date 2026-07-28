#include "stm32f4xx.h"

#include "jump.h"
#include "verify.h"


int main(void)
{
    
//    if(Update_Requet){
//        RX_Init();
//        Rx_Start();
//        Flash_Init();
//        Flash_Erase_App();
//    }
  
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
        
    }
}
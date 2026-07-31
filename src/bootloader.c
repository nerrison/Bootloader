#include "stm32f4xx.h"

#include "jump.h"
#include "button_pressed.h"
#include "usart1.h"
#include "usart1_irq.h"
#include "verify.h"
#include "update_process.h"
#include "systick.h"


static uint8_t *data;
static uint16_t length;


typedef enum
{
    BOOT_WAIT,
    BOOT_UPDATE,
    BOOT_JUMP

}BootState_t;


static BootState_t bootState;

#define DELAY 10000U

void BootShowBanner(void)
{
    USART1_SendString("\033[2J");
    USART1_SendString("\033[H");

    USART1_SendString("BOOTLOADER v0.1\r\n");
    USART1_SendString("ARM Cortex-M4\r\n");
    USART1_SendString("STM32F407VGTX\r\n\r\n");
}


int main(void)
{
    ButtonInit();

    USART1_Init();

    SysTickInit();

    BootShowBanner();


    uint32_t startTime = SysTickGetTick();


    bootState = BOOT_WAIT;



    while(1)
    {

        switch(bootState)
        {

            case BOOT_WAIT:

                if(ButtonPressed())
                {
                    USART1_SendString("UPDATE MODE\r\n");

                    USART1_RX_Start();

                    bootState = BOOT_UPDATE;

                    break;
                }


                if((SysTickGetTick() - startTime) >= DELAY)
                {
                    USART1_SendString("10 SECONDS PASSED\r\n");
                    
                    if(ProgramIsValid())
                    {
                        USART1_SendString("PROGRAM FOUND\r\n");

                        

                        bootState = BOOT_JUMP;
                    }
                    else
                    {
                        USART1_SendString("NO PROGRAM\r\n");

                        USART1_RX_Start();

                        bootState = BOOT_UPDATE;
                    }
                }


                break;



            case BOOT_UPDATE:


                if(UART_DataReady())
                {
                    UART_GetReceivedData(&data, &length);

                    UpdateProcess(data,length);
                }


                break;



            case BOOT_JUMP:

                JumpToProgram();

                break;



            default:

                bootState = BOOT_UPDATE;

                break;
        }
    }
}
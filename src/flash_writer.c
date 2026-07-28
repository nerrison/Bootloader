#include "stm32f4xx.h"
#include "flash_writer.h"

#include "memory_map.h"




static void Flash_Unlock(void)
{
    if(FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}


void Flash_Lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}


void Flash_Init(void)
{
    Flash_Unlock();

    FLASH->ACR |= FLASH_ACR_PRFTEN;
    FLASH->ACR |= FLASH_ACR_ICEN;
    FLASH->ACR |= FLASH_ACR_DCEN;
}


static void Flash_Wait_Busy(void)
{
    while(FLASH->SR & FLASH_SR_BSY);
}


void Flash_Erase_App(void)
{
    Flash_Unlock();

    Flash_Wait_Busy();


    FLASH->CR &= ~FLASH_CR_SNB;

    FLASH->CR |= FLASH_CR_SER;

    FLASH->CR |= (2 << FLASH_CR_SNB_Pos);


    FLASH->CR |= FLASH_CR_STRT;


    Flash_Wait_Busy();


    FLASH->CR &= ~FLASH_CR_SER;
}


static uint32_t address = APPLICATION_ADDRESS;

void Flash_Write(uint8_t *data, uint16_t length)
{
    Flash_Unlock();

    for(uint16_t i = 0; i < length; i += 4)
    {
        uint32_t word = 0xFFFFFFFF;

        if(i < length)
            word &= ~(0xFF << 0), word |= data[i];

        if(i + 1 < length)
            word |= data[i+1] << 8;

        if(i + 2 < length)
            word |= data[i+2] << 16;

        if(i + 3 < length)
            word |= data[i+3] << 24;


        Flash_Wait_Busy();

        FLASH->CR &= ~FLASH_CR_PSIZE;
        FLASH->CR |= FLASH_CR_PSIZE_1;   // 32-bit programming

        FLASH->CR |= FLASH_CR_PG;

        *(__IO uint32_t*)address = word;

        Flash_Wait_Busy();

        FLASH->CR &= ~FLASH_CR_PG;

        address += 4;
    }
}
#ifndef FLASH_WRITER_H
#define FLASH_WRITER_H

#include "stdint.h"

void Flash_Init(void);

void Flash_Erase_App(void);

void Flash_Write( uint8_t *data, uint16_t length);

void Flash_Lock(void);

#endif
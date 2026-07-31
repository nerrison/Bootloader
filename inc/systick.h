#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void SysTickInit(void);
uint32_t SysTickGetTick(void);
void SysTick_Handler(void);

#endif
#include "verify.h"
#include "memory_map.h"

uint8_t Application_IsValid(void)
{
    uint32_t stack = *(volatile uint32_t *)APPLICATION_ADDRESS;

    return ((stack >= SRAM_START) &&
            (stack <= SRAM_END));
}
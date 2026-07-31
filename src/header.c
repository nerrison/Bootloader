#include "header.h"
#include "memory_map.h"
#include <string.h>

#define MAGIC_VALUE  0xDEADBEEF

static ImageHeader_t header;


uint8_t HeaderReceived(uint8_t *data, uint16_t length)
{
    if(length < sizeof(ImageHeader_t))
    {
        return 0;
    }

    memcpy(&header, data, sizeof(ImageHeader_t));


    if(header.magic != MAGIC_VALUE)
    {
        return 0;
    }
    if(header.image_size == 0)
    {
        return 0;
    }

    if(header.image_size > PROGRAM_FLASH_SIZE)
    {
        return 0;
    }

    return 1;
}

const ImageHeader_t *HeaderGet(void)
{
    return &header;
}
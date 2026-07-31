#include "flash_writer.h"
#include "update_process.h"
#include "memory_map.h"
#include "header.h"
#include "jump.h"
#include "verify.h"


typedef enum{
    UPDATE_WAIT_HEADER,
    UPDATE_RECEIVE_PAYLOAD,
    UPDATE_VERIFY,
    UPDATE_FINISHED,
    UPDATE_ERROR
}UpdateState_t;


static volatile UpdateState_t updateState = UPDATE_WAIT_HEADER;

static uint32_t bytesReceived = 0;



void UpdateProcess(uint8_t *data, uint16_t length)
{

    switch(updateState)
    {

    case UPDATE_WAIT_HEADER:

    if(HeaderReceived(data, length))
    {
        const ImageHeader_t *header = HeaderGet();

        if(header->image_size <= PROGRAM_FLASH_SIZE)
        {
            FlashEraseProgram();

            bytesReceived = 0;

            updateState = UPDATE_RECEIVE_PAYLOAD;
        }
        else
        {
            updateState = UPDATE_ERROR;
        }
    }

    break;


    case UPDATE_RECEIVE_PAYLOAD:
    {
        const ImageHeader_t *header = HeaderGet();

        uint32_t remaining;

        remaining = header->image_size - bytesReceived;

        if(length > remaining)
        {
            length = remaining;
        }

        if(FlashWrite(data, length))
        {
            bytesReceived += length;

            if(bytesReceived >= header->image_size)
            {
                updateState = UPDATE_VERIFY;
            }
        }
        else
        {

            updateState = UPDATE_ERROR;
        }

        break;
    }


    case UPDATE_VERIFY:

        if(CRC_Check_OK())
        {
            updateState = UPDATE_FINISHED;
        }
        else
        {

            updateState = UPDATE_ERROR;
        }

        break;


    case UPDATE_FINISHED:


        JumpToProgram();

        break;


    case UPDATE_ERROR:

        while(1);

        break;


    default:

        updateState = UPDATE_ERROR;

        break;
    }
}
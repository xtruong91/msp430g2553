/*
 * hc06.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "debug.h"
#include "hc06.h"
#include "isr.h"
#include "uart.h"

static void recvDataHandle(void *args);
uart_config_t g_config = {R_9600};
CallBack _cbRecv = NULL;

int8_t
HC06_init()
{
    uart_init(&g_config);
    setObserver(recvDataHandle);
    return 0;
}

void
HC06_send(int8_t* buffer)
{
    if(buffer == NULL)
        return;
    uart_puts(buffer);
}

RetCode parseMessage(rbd_t rbID)
{
    int8_t data;

    // get preamble byte
    if(ring_buffer_get(rbID, &data) == FALSE)
    {
        return INVALID_RX;
    }

    if(PREAMBLE != data)
    {
        return INVALID_PREAMBLE;
    }

    // get message ID byte;
    if(ring_buffer_get(rbID, &data) == FALSE)
    {
        return INVALID_RX;
    }

    // get length byte;
    if(ring_buffer_get(rbID, &data) == FALSE)
    {
        return INVALID_RX;
    }

    uint8_t length = data - 0x30;
    if(length <= 0)
    {
        return INVALID_LENGTH;
    }

    int8_t *buffer = (int8_t *)malloc(length);
    uint8_t i;
    for(i = 0; i < length; i++)
    {
        if(ring_buffer_get(rbID, &buffer[i]) == FALSE)
        {
            return INVALID_RX;
        }
    }

    if(_cbRecv != NULL)
    {
       _cbRecv((void*)buffer);
    }

    // remove remaining byte inside ringbuffer
    while(ring_buffer_get(rbID, &data) == TRUE);

    free(buffer);

    return VALID;

}


void recvDataHandle(void *args)
{
    rbd_t *rbID = (rbd_t*)(args);

    RetCode result = parseMessage(*rbID);

    UARTprintf("%c", result);
}

void HC06_setObserver(CallBack cbRecv)
{
    _cbRecv = cbRecv;
}


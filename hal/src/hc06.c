/*
 * hc06.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "hc06.h"
#include "isr.h"
#include "uart.h"
#include "debug.h"

static void recvDataHandle(void *args);
uart_config_t config = {R_9600};

int8_t
HC06_init()
{
    uart_init(&config);
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


void recvDataHandle(void *args)
{
    rbd_t *rbID = (rbd_t*)(args);
    int8_t data;

    while(ring_buffer_get(*rbID,&data) == TRUE)
    {
        UARTprintf("%c", data);
    }

}


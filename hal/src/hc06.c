/*
 * hc06.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "hc06.h"
#include "isr.h"
#include "uart.h"

int8_t
HC06_init()
{
    uart_config_t config = {9600};
    return uart_init(&config);
}

void
HC06_send(int8_t* buffer)
{
    if(buffer == '\0')
        return;
    uart_puts(buffer);
}
void HC06_enableRxISR(void (*cbRxHandler)(void* args)){
    uart_enableRXInt(cbRxHandler);
}



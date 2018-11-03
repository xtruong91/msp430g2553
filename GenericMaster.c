/*
 * GenericMaster.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#include "CompileOption.h"
#include "clock.h"
#include "adc.h"
#include "uart.h"
#include "bq32000.h"
#include "hc06.h"


#if (BOARD_VERSION == BOARD_VERSION_MASTER)

void isr_uartrx(void *args);



int main()
{
    CLKInit(CLK_1MHZ);

//    uart_config_t config = {9600};
//    if(uart_init(&config) != 0)
//        return -1;
//    uart_enableRXInt(isr_uartrx);
    HC06_init();
    HC06_send("Initialize UART successfully! \n");

    // we have two option 1. handle at routine function or handle with ringbuffer
    adc_init(CHANNEL_0);
    HC06_send("Initialize ADC successfully! \n");

    BQ32000_init();
    HC06_send("Initialized BQ3200! \n ");

    SDateTime sDateTime;
    while(1)
    {
        sDateTime = BQ32000_readDateTime();
        uart_putnum(sDateTime.second, 0, 0);
        HC06_send("\r\n");
        delay_ms(1000);
        //uart_puts("push data up PC \n");
    }
}

void isr_uartrx(void *args)
{
    int8_t *data = (int8_t*)args;
    uart_putchar(*data);
}


#endif



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
#include "pwm.h"
#include "isr.h"


#if (BOARD_VERSION == BOARD_VERSION_MASTER)

void isr_uartrx(void *args);

int main()
{
    CLKInit(CLK_1MHZ);

    uart_config_t config = {9600};
    if(uart_init(&config) != 0)
        return -1;
    uart_enableInt();
    uart_puts("Initialize UART successfully! \n");

    // we have two option 1. handle at routine function or handle with ringbuffer
//    isr_config isrConfig = {0, 1, isr_uartrx};
//    subscribe(&isrConfig);

    adc_init(CHANNEL_0);
    uart_puts("Initialize ADC successfully! \n");

    //pwm_init();
    int8_t c;
    while(1)
    {
        //uart_puts("push data up PC \n");
        c = uart_getchar();
        if(c != -1)
        {
            uart_putchar(c);
        }
        delay_ms(1000);
    }
}

void isr_uartrx(void *args)
{
    int8_t *data = (int8_t*)args;
    uart_putchar(*data);
}


#endif



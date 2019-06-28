/*
 * CentralCircuit.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#include "centralcircuit.h"

void isr_uartrx(void *args);
void readDateTime();
void blinkLed();

int
CentralCircuit_init()
{
    // setup clock_system
    clk_init(CLK_1MHZ);
    // set pin functionality
    pin_mode(LED, OUTPUT);
#if (DEBUG_EN > 0)
    // enable debug module
    UARTStdioConfig(UART_BAUDRATE);
    UARTprintf("Initialized Debug module \n");
    UARTprintf("d value: %d \n ", -12344);
    UARTprintf("s String: %s \n", "test unit");
#endif

#if (SX1278_EN > 0)
    sx1278_init();
    sx1278_enableRxISR(isr_uartrx);
    blinkLed();
#endif

#if (HC06 > 0)
    HC06_init();
    uart_puts("Initialize UART successfully! \n");
#endif

#ifdef HC595
    HC595_init();
    uart_puts("Initialize HC595 successfully! \n");
#endif

#if (ADC_EN > 0)
    // we have two option 1. handle at routine function or handle with ringbuffer
    adc_init(CHANNEL_0);
    uart_puts("Initialize ADC successfully! \n");
#endif

#if (I2C_EN > 0)
    BQ32000_init();
    uart_puts("Initialized BQ3200! \n ");
#endif

    return 0;
}

int
CentralCircuit_run()
{
    int8_t index;
    for(index = 0; index < 10; index++)
    {
        sx1278_send(&index, 1);
        UARTprintf("Tx:%d \n", index);
        blinkLed();
    }
    return 0;
}


void
readDateTime()
{
    SDateTime sDateTime = BQ32000_readDateTime();
    UARTprintf("Second: %d \n", sDateTime.second);
    delay_ms(1000);
}


void isr_uartrx(void *args)
{
    char *data = (char*)(args);
    UARTprintf("Rx data: %s \n", data);
}

void blinkLed()
{
    digital_write(LED, LOW);
    delay_ms(1000);
    digital_write(LED, HIGH);
    delay_ms(1000);
}




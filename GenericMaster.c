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
#include "hc595.h"
#include "sx1278.h"


#if (BOARD_VERSION == BOARD_VERSION_LoRaMASTER)


//#define UART
//#define HC595
//#define ADC
#define SX1278
void isr_uartrx(void *args);
void readDateTime();
void displayNumber();


int main()
{
    CLKInit(CLK_1MHZ);

#ifdef UART
    uart_config_t config = {9600};
    uart_init(&config);
    uart_enableRXInt(isr_uartrx);
    uart_puts("Initialize SX1278 successfully! \n");
#endif

#ifdef SX1278
    sx1278_init();
    sx1278_enableRxISR(isr_uartrx);
    uart_puts("Initialize SX1278 successfully! \n");
#endif

#ifdef HC06
    HC06_init();
    uart_puts("Initialize UART successfully! \n");
#endif

#ifdef HC595
    HC595_init();
    uart_puts("Initialize HC595 successfully! \n");
#endif

#ifdef ADC
    // we have two option 1. handle at routine function or handle with ringbuffer
    adc_init(CHANNEL_0);
    uart_puts("Initialize ADC successfully! \n");
#endif

#ifdef I2C
    BQ32000_init();
    uart_puts("Initialized BQ3200! \n ");
#endif

    while(1){
        //displayNumber();
    }
}

void readDateTime(){
    SDateTime sDateTime = BQ32000_readDateTime();
    uart_putnum(sDateTime.second, 0, 0);
    uart_puts("\r\n");
    delay_ms(1000);
}

void displayNumber(){
    int i = 0;
    for(i = 0; i < 10 ; i++){
        HC595_sendNumber(i);
        delay_ms(1000);
    }
}

void isr_uartrx(void *args)
{
    int8_t *data = (int8_t*)args;
    uart_putchar(*data);
}


#endif



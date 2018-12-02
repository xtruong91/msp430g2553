/*
 * GenericMaster.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */
#include "ConfigChip.h"
#include "clock.h"
#include "bq32000.h"
#include "hc06.h"
#include "hc595.h"
#include "sx1278.h"
#include "debug.h"
#include "gpio.h"

#if (BOARD_VERSION == BOARD_VERSION_LoRaMASTER)
void isr_uartrx(void *args);
void readDateTime();
void displayNumber();

int main(){
    CLKInit(CLK_1MHZ);
#ifdef DEBUG_EN
    UARTStdioConfig(UART_BAUDRATE);;
    UARTprintf("d value: %d \n ", -12344);
    UARTprintf("s String: %s \n", "test unit");
#endif
    pin_mode(P2_2, OUTPUT);
    pin_mode(P2_3, OUTPUT);
#ifdef SX1278_EN
    sx1278_init();
    sx1278_enableRxISR(isr_uartrx);
    digital_write(P2_2, HIGH);
    delay_ms(2000);
    digital_write(P2_2, LOW);
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

#ifdef I2C_EN
    BQ32000_init();
    uart_puts("Initialized BQ3200! \n ");
#endif
    int8_t index;
    while(1){
        for(index = 0; index < 10; index++){
            digital_write(P2_2, HIGH);
            sx1278_send(&index, 1);
            delay_ms(1000);
            digital_write(P2_2, LOW);
            delay_ms(1000);
        }

    }
}

void readDateTime(){
    SDateTime sDateTime = BQ32000_readDateTime();
    UARTprintf("Second: %d \n", sDateTime.second);
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
    //int8_t *data = (int8_t*)args;
    //uart_putchar(*data);
    digital_write(P2_3, HIGH);
    delay_ms(1000);
    digital_write(P2_3, LOW);
}


#endif



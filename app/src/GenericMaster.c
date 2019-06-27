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

#define LED P1_0

#if (BOARD_VERSION == BOARD_VERSION_LoRaMASTER)
void isr_uartrx(void *args);
void readDateTime();
void blinkLed();

int main(){
    clk_init(CLK_1MHZ);
    pin_mode(LED, OUTPUT);

#if (DEBUG_EN > 0)
    UARTStdioConfig(UART_BAUDRATE);;
//    UARTprintf("d value: %d \n ", -12344);
//    UARTprintf("s String: %s \n", "test unit");
    UARTprintf("Initialized Debug module \n");
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
    int8_t index;
    while(1){
        for(index = 0; index < 10; index++){
            sx1278_send(&index, 1);
            UARTprintf("Tx:%d \n", index);
            blinkLed();
        }
    }
}

void readDateTime(){
    SDateTime sDateTime = BQ32000_readDateTime();
    UARTprintf("Second: %d \n", sDateTime.second);
    delay_ms(1000);
}


void isr_uartrx(void *args)
{
    char *data = (char*)(args);
    UARTprintf("Rx data: %s \n", data);
}

void blinkLed(){
    digital_write(LED, LOW);
    delay_ms(1000);
    digital_write(LED, HIGH);
    delay_ms(1000);
}

#endif



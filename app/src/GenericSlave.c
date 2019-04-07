/*
 * GenericSlave.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 *      use Timer send message each one second;
 */

#include "ConfigChip.h"
#include "sx1278.h"
#include "clock.h"
#include "gpio.h"
#include "debug.h"
#include "hc595.h"
#include "hcsr04.h"
#include "hc06.h"

#if (BOARD_VERSION == BOARD_VERSION_LoRaSLAVE)

void blinkLed(void);

static void cbRxISR(void *args){
    blinkLed();
}



int main(){
    CLKInit(CLK_1MHZ);

    pin_mode(P1_0, OUTPUT); // led for debug;
#if (DEBUG_EN > 0)
    UARTStdioConfig(UART_BAUDRATE);;
    UARTprintf("d value: %d \n ", -12344);
    UARTprintf("s String: %s \n", "test unit");
#endif

#if (SX1278_EN > 0)
    sx1278_init();
    sx1278_enableRxISR(cbRxISR);
    blinkLed();
#endif

#if (HCSR04_EN > 0)
    HCSR04_init(0);
    blinkLed();
    //UARTprintf("Initialized HCSR04 module \n");
#endif

#if (HC595_EN > 0)
    HC595_init(P2_5);
    blinkLed();
    HC595_sendNumber(30);
#endif

#if (HC06_EN > 0)

#endif

    unsigned int distance;
    while(1){

        distance = getDistance();
        UARTprintf("Distance: %d \n", distance);
        HC595_sendNumber(distance);
        sx1278_send((int8_t*)&distance, sizeof(distance));
        HC06_send((int8_t*)&distance);
        blinkLed();
        delay_ms(1000);
    }
}

void blinkLed(){
    digital_write(P1_0, HIGH);
    delay_ms(1000);
    digital_write(P1_0, LOW);
    delay_ms(1000);
}

#endif


/*
 * GenericSlave.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 * Hardware component:
 * 1. MSP430G2553 launch pad board.
 * 2. Bluetooth slave module -> receive command from android device
 * 3. HC595 IC -> display distance parameter on LED
 * 4. HC-SR04 sensor -> measure distance to obstacle
 * 5. SX1278 module -> transceiver data with the control board.
 *
 */

#include "ConfigChip.h"
#include "sx1278.h"
#include "clock.h"
#include "gpio.h"
#include "debug.h"
#include "hc595.h"
#include "hcsr04.h"
#include "hc06.h"

#if (BOARD_VERSION == BOARD_VERSION_REMOTECAR)

void blinkLed(void);

static void cbRxISR(void *args){
    blinkLed();
}


unsigned int distance;
int main(){
    // setup clock for MCU
    CLKInit(CLK_1MHZ);
    pin_mode(P1_0, OUTPUT); // led for debug;

#if (DEBUG_EN > 0)
    UARTStdioConfig(UART_BAUDRATE);;
    UARTprintf("Configure UART module with baudrate = %d \n",UART_BAUDRATE);
#endif

#if (SX1278_EN > 0)
    sx1278_init();
    sx1278_enableRxISR(cbRxISR);
    UARTprintf("Initialized LoRa module! \n");
#endif

#if (HCSR04_EN > 0)
    HCSR04_init(0);
    UARTprintf("Initialized Sonar sensor! \n");
#endif

#if (HC595_EN > 0)
    HC595_init(P2_5);
    HC595_sendNumber(0);
    UARTprintf("Initialized LED7SEG module! \n");
#endif

#if (HC06_EN > 0)
    HC06_init();
    UARTprintf("Initialized Bluetooth module \n");
#endif

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


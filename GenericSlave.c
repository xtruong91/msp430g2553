/*
 * GenericSlave.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 *      use Timer send message each one second;
 */

#include "CompileOption.h"
#include "sx1278.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"

#if (BOARD_VERSION == BOARD_VERSION_LoRaSLAVE)

static void cbRxISR(void *args){
    digital_write(8, HIGH);
    delay_ms(1000);
    digital_write(8, LOW);
}
int main(){

    CLKInit(CLK_1MHZ);

    pin_mode(0, OUTPUT);
    pin_mode(8, OUTPUT);

    digital_write(0, HIGH);
    sx1278_init();
    sx1278_enableRxISR(cbRxISR);
    uart_puts("Initialized SX1278 Salve successfully! \n");
    int8_t data[2] =  {'0', '1'};
    while(1){
        digital_write(0, LOW);
        delay_ms(1000);
        sx1278_send(data, 2);
        digital_write(0, HIGH);
        delay_ms(1000);
    }
}
#endif

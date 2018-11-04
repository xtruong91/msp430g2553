/*
 * sx1278.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */
#include "sx1278.h"
#include "uart.h"
#include "isr.h"
#include "gpio.h"

void sx1278_init(){

    pin_mode(9, OUTPUT);
    pin_mode(10, OUTPUT);
    uart_config_t config = {9600};
    uart_init(&config);
    // default set normal mode;
    digital_write(9, LOW);
    digital_write(10, LOW);
}

void sx1278_setMode(ELoraMode mode){
    switch(mode){
        case Normal:{
            digital_write(9, LOW);
            digital_write(10, LOW);
            break;
        }
        case Wake_up:{
            digital_write(9, LOW);
            digital_write(10, HIGH);
            break;
        }
        case Power_saving:{
            digital_write(9, HIGH);
            digital_write(10, LOW);
            break;
        }
        case Sleep:{
            digital_write(9, HIGH);
            digital_write(10, HIGH);
            break;
        }
        default:{
            break;
        }
    }
}
void sx1278_enableRxISR(void (*cbRxHandler)(void *args)){
    uart_enableRXInt(cbRxHandler);
}

void sx1278_disableRxISR(){
    uart_disableRXInt();
}

void sx1278_send(const int8_t *data, uint8_t length){
    int index;
    for(index = 0; index < length; index++){
        uart_putchar(data[index]);
    }
}




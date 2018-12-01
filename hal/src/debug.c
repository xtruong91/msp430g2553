/*
 * Debug.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "debug.h"
#include "uart.h"

void Debug_init(){
    uart_config_t config = {9600};
    uart_init(&config);
}

void Debug_enableRxISR(void (*cbRxISR)(void *args)){
    uart_enableRXInt(cbRxISR);
}
void Debug_printfs(const int8_t *String){
    uart_puts(String);
}





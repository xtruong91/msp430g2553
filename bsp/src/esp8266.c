/*
 * esp8266.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "esp8266.h"
#include "spi.h"


void esp8266_init(){
    spi_config spiConfig = {SLAVE,ModuleB, RSPI_1MHz};
    spi_init(&spiConfig);
}

void esp8266_enableRxISR(void (*RxHandler)(void *args)){
    spi_enableRxISR(RxHandler);
}

void esp8266_disableRxISR(){
    spi_disableRxISR();
}

void esp8266_send(const int8_t *buffer, uint8_t length){
    spi_puts(buffer, length);
}

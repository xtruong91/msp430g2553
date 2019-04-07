/*
 * WifiSlave.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "CompileOption.h"
#include "clock.h"
#include "debug.h"
#include "esp8266.h"

#if (BOARD_VERSION == BOARD_VERSION_WifiSLAVE)

static void cbRxISR(void *args){
    Debug_printfs("Received date from UART \n");
}

int main(){
    CLKInit(CLK_1MHZ);

    Debug_init();
    Debug_printfs("Initialized Debug module \n");
    Debug_enableRxISR(cbRxISR);

    esp8266_init();
    Debug_printfs("Initialized ESP8266 module \n");

    while(1){

    }
}

#endif



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
#include "wifinode.h"

void callbackUART(void *args)
{
    //Debug_printfs("Received date from UART \n");
}

int
WifiNode_init()
{
    clk_init(CLK_1MHZ);

//    Debug_init();
//    Debug_printfs("Initialized Debug module \n");
//    Debug_enableRxISR(cbRxISR);
//
//    esp8266_init();
//    Debug_printfs("Initialized ESP8266 module \n");
    return 0;
}

int
WifiNode_run()
{
    return 0;
}

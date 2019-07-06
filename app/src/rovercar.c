/*
 * rovercar.c
 *
 *  Created on: Jul 6, 2019
 *      Author: truongtx
 */

#include "rovercar.h"
#include "hcsr04.h"
#include "hc595.h"
#include "hc06.h"
#include "debug.h"
#include "clock.h"


Ret Car_init()
{
    // initialize system clock

    clk_init(CLK_1MHZ);
    // set Trigger(T) - Pin 2.4, Echo - Pin 2.0
    HCSR04_init();
    // initialize 7seg led; MOSI - P1.7, SCK -  P1.5, STRB - P1.0
    HC595_init(STRTB);
    // setup Bluetooth.TXD -P1.1(RXD),RXD - P1.2(TXD)
    //HC06_init();
#ifdef DEBUG
    UARTStdioConfig(BAUDRATE);
#endif
    return SUCCESS;
}
/*
 * Run in while loop
 * */
unsigned int num = 0;

Ret Car_run()
{

    num = getDistance();

    HC595_sendNumber(num);
#ifdef DEBUG
    UARTprintf("Distance: %d \n", num);
#endif
//    HC06_send("distance:  \n");
//    HC06_send(distance/10 + 0x30);
//    HC06_send(distance%10 + 0x30);

    return SUCCESS;
}

Ret Car_stop()
{

}




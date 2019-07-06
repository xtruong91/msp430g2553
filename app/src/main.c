/*
 * main.c
 *
 *  Created on: Jun 28, 2019
 *      Author: truongtx
 */

#include "rovercar.h"

int main()
{
    Car_init();

    while(1)
    {
        Car_run();
        delay_ms(1000);
    }
}

/*
 * main.c
 *
 *  Created on: Jun 28, 2019
 *      Author: truongtx
 */

#include "rovercar.h"
#define APPLED

int main()
{

#ifdef APPCAR
    Car_init();
#elif defined APPLED
    LedMatrix_init();
#endif


    while(1)
    {

#ifdef APPCAR
       Car_run();
#elif defined APPLED
       LedMatrix_run();
#endif

    }
}

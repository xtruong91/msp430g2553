/*
 * clock.c
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
*/

#include "clock.h"
#include "watchdog.h"
#include <msp430.h>


void CLKInit(eCLK clk)
{
    watchdog_disable();
    switch(clk)
    {
        case CLK_1MHZ:
        {
            if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_1MHZ;                    // Set range
            DCOCTL = CALDCO_1MHZ;                     // Set DCO step + modulation */
            break;
        }
        case CLK_8MHZ:
        {
            if (CALBC1_8MHZ==0xFF)                    // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_8MHZ;                    // Set range
            DCOCTL = CALDCO_8MHZ;                     // Set DCO step + modulation */
            break;
        }

        case CLK_12MHZ:
        {
            if (CALBC1_12MHZ==0xFF)                   // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_12MHZ;                   // Set range
            DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation*/
            break;
        }

        case CLK_16MHZ:
        {
            if (CALBC1_16MHZ==0xFF)                   // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_16MHZ;                   // Set range
            DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/
            break;
        }
        default:
        {
            break;
        }
    }
}

// Delay for t microseconds
// The delay is not exact when t is too small
//*****************************************************************************
void delay_us (int t)
{
    int i;
    for (i = 0; i<t; i++ )
        _delay_cycles(MCLK_F);
}

//*****************************************************************************
// Delay for t milliseconds
//*****************************************************************************
void delay_ms (int t)
{
    int i;
    for (i = 0; i<t; i++ )
        _delay_cycles(MCLK_F*1000);
}


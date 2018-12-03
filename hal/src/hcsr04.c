/*
 * hcsr04.c
 *
 *  Created on: Dec 2, 2018
 *      Author: truongtx
 */

#include "hcsr04.h"
#include "gpio.h"
#include <msp430.h>

unsigned int up_counter;
volatile unsigned int distance_cm;

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TimerA0(void)
{
    if (TA1CCTL0 & CCI)            // Raising edge
    {
        up_counter = TA1CCR0;      // Copy counter to variable
    }
    else                        // Falling edge
    {
        // Formula: Distance in cm = (Time in uSec)/58
        distance_cm = (TA1CCR0 - up_counter)/58;
    }
    TA1CTL &= ~TAIFG;           // Clear interrupt flag - handled
}

void HCSR04_init(uint8_t ssTrig){
    /* Set P2.3 to input direction (echo)
      input for Timer A1 - Compare/Capture input */
    P2DIR &= ~BIT0;
    // Select P2.3 as timer trigger input select (echo from sensor)
    P2SEL = BIT0;

    /* set P2.4 to output direction (trigger) */
    P2DIR |= BIT4;
    P2OUT &= ~BIT4;                 // keep trigger at low

    /* Timer A1 configure to read echo signal:
    Timer A Capture/Compare Control 1 =>
    capture mode: 1 - both edges +
    capture sychronize +
    capture input select 0 => P2.3 (CCI1A) +
    capture mode +
    capture compare interrupt enable */
    TA1CCTL0 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;

    /* Timer A Control configuration =>
    Timer A clock source select: 1 - SMClock +
    Timer A mode control: 2 - Continous up +
    Timer A clock input divider 0 - No divider */
    TA1CTL |= TASSEL_2 + MC_2 + ID_0 + TACLR;

    // Global Interrupt Enable
    _BIS_SR(GIE);
}

unsigned int
getDistance(){
    P2OUT ^= BIT4;              // assert
    __delay_cycles(10);         // 10us wide
    P2OUT ^= BIT4;              // deassert
    __delay_cycles(60000);      // 60ms measurement cycle
    return distance_cm;
}


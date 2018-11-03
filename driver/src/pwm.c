/*
 * pwm.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */
#include "pwm.h"
#include <msp430.h>

BOOL pwm_init()
{
    P1DIR |= BIT2; //Set pin 1.2 to the output direction.
    P1SEL |= BIT2; //Select pin 1.2 as our PWM output.
    TA0CCTL1 = OUTMOD_7;
    TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    return 1;
}
/*
 * set period timer;
 * */
void pwm_setperiod(uint16_t period)
{
    TA0CCR0 = period;
}
/*
 *  set duty cycle
 * */
void pwm_setdutycycle(uint16_t dutyCycle)
{
    TA0CCR1 = dutyCycle;
}




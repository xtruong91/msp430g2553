/*
 * adc.c
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 */
#include "adc.h"
#include <msp430.h>

void adc_init(EADCChannel pin)
{
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    if(pin > 15)
    {
        ADC10CTL1 = INCH_0;                       // input A1
        ADC10AE0 |= INCH_0;                         // PA.1 ADC option select
    }else
    {
        ADC10CTL1 = 1 << pin;                       // input A1
        ADC10AE0 |= 1 << pin;                         // PA.1 ADC option select
    }
}
uint32_t adc_start()
{
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
    return ADC10MEM;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}





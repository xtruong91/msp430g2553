/*
 * adc.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 * ADC 10 Bit, 8 channel, auto-scan one channel DMA
 * It means indicate 2^10 value from input signal
 *
 * ADC10 Registers
 * 1. ADC10 input enable register0 (ADC10AE0 at 04Ah address)
 * 2. ADC10 input enable register 1 (ADC10AE1 at 04Bh address)
 * 3. ADC10 control register 0 (ADC10CTL0 at 01B0h address)
 * 4. ADC10 control register 1 (ADC10CTL1 at 01B2h address)
 */

#ifndef DRIVERS_INCLUDE_ADC_H_
#define DRIVERS_INCLUDE_ADC_H_

#include "type.h"

typedef enum{
    CHANNEL_0 = 0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3
}EADCChannel;

/*
 * ADC 10 bit analog to digital conversion
 * @param: pin
 * */
extern void adc_init(EADCChannel pin);
/*
 * call each get adc value;
 * @return: ADC value;
 * */
extern uint32_t adc_start();


#endif /* DRIVER_ADC_H_ */

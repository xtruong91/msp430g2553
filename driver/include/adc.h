/*
 * adc.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

#include "type.h"

typedef enum{
    CHANNEL_0 = 0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3
}EADCChannel;
extern void adc_init(EADCChannel pin);
extern uint32_t adc_start();


#endif /* DRIVER_ADC_H_ */

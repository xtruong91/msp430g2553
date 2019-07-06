/*
 * hc595.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 *  MSP430 master communicate with the HC595 by SPI protocol;
 *
 *  MOSI - P1.7
 *  SCK -  P1.4
 *  STRB - the other GPIO
 */

#ifndef HAL_INCLUDE_HC595_H_
#define HAL_INCLUDE_HC595_H_

#include "type.h"
#include "spi.h"


/*
 * ssPin: STRB pin used for latch pin in HC595;
 * */
void HC595_init(uint8_t ssPin);
// Send number through down LED7Seg
void HC595_sendNumber(const uint16_t number);



#endif /* HAL_INCLUDE_HC595_H_ */

/*
 * hc595.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#ifndef HAL_INCLUDE_HC595_H_
#define HAL_INCLUDE_HC595_H_

#include "type.h"
#include "spi.h"



void HC595_init();
// Send number through down LED7Seg
void HC595_sendNumber(const uint16_t number);



#endif /* HAL_INCLUDE_HC595_H_ */

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

const uint8_t code7seg[11] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x3F};

BOOL hc595_init();
// Send number through down LED7Seg
BOOL hc595_sendNo(const uint16_t number);
BOOL hc595_sendCommand(const int8_t command);
BOOL hc595_sendString(const int8_t* str);



#endif /* HAL_INCLUDE_HC595_H_ */

/*
 * debug.h
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#ifndef HAL_INCLUDE_DEBUG_H_
#define HAL_INCLUDE_DEBUG_H_

#include "type.h"

void Debug_init();
void Debug_enableRxISR(void (*cbRxISR)(void *args));
void Debug_printfs(const int8_t *String);



#endif /* HAL_INCLUDE_DEBUG_H_ */

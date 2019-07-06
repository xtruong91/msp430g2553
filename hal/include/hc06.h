/*
 * hc06.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *      9600, N, 8, 1.
 *      Pairing code: 1234.
 *      voltage: 3V3-5V DC
 *      current: 20-30mA
 *      baudRate: 9600
 *
 *
 */

#ifndef HAL_INCLUDE_HC06_H_
#define HAL_INCLUDE_HC06_H_

#include "type.h"
#include "uart.h"


int8_t HC06_init();

void HC06_send(int8_t* buffer);


#endif /* HAL_INCLUDE_HC06_H_ */

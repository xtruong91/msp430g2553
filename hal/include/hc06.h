/*
 * hc06.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *      9600, N, 8, 1.
 *      Pairing code: 1234.
 *      voltage: 3V3-5V DC
 *      current: 20-30mA
 *      baudRate: 200，2400，4800，9600, 19200，38400，57600，115200
 *
 *
 */

#ifndef HAL_INCLUDE_HC06_H_
#define HAL_INCLUDE_HC06_H_

#include "type.h"


int8_t HC06_init();
void HC06_send(const int8_t* buffer);
void HC06_enableRxISR(void (*cbRxHandler)(void* args));

#endif /* HAL_INCLUDE_HC06_H_ */

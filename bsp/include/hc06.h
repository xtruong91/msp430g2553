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
 *  protocol receive data via bluetooth
 *
 *   | Preamble(!) | ID | Length | Data | CRC |
 */

#ifndef BSP_INCLUDE_HC06_H_
#define BSP_INCLUDE_HC06_H_

#include "uart.h"
#include "type.h"

#define PREAMBLE    '!'

typedef enum
{
    INVALID_RX,
    INVALID_PREAMBLE,
    INVALID_LENGTH,
    VALID
}RetCode;

int8_t HC06_init();

void HC06_send(int8_t* buffer);

void HC06_setObserver(CallBack cbRecv);


#endif /* BSP_INCLUDE_HC06_H_ */

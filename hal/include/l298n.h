/*
 * l298n.h
 *
 *  Created on: Jul 6, 2019
 *      Author: truongtx
 *      L298N module use for controlling motor with
 *      Control voltage - 5-> 36 V
 *      Current max: 2A
 *      Signal voltage: +5V - 7V
 *      Signal current: 0 - 36 mA
 *      P: 20W
 *      4 input:
 *      INA - Pin 2.1
 *      INB - Pin 2.2
 *      INC - Pin 2.3
 *      IND - Pin 2.4
 * when ENA = 1 -> INA = 1, INB=0 -> turn clockwise
 *              -> INA = 0, INB = 1-> turn anti-closewise
 *              -> INA = INB -> Stop
 *
 */

#ifndef HAL_INCLUDE_L298N_H_
#define HAL_INCLUDE_L298N_H_

#include "type.h"

typedef enum
{
    Left,
    Right,
    Up,
    Down,
    Stop
}Navigation;

typedef struct
{
    uint8_t INA;
    uint8_t INB;
    uint8_t INC;
    uint8_t IND;
}L298Config;

extern void L298_init(L298Config* config);

extern void L298_drive(Navigation nav);



#endif /* HAL_INCLUDE_L298N_H_ */

/*
 * hcsr04.h
 *
 *  Created on: Dec 2, 2018
 *      Author: truongtx
 *  HC-SR04
 *  Voltage: 5V (DC)
 *  Angel: 30
 *  distance range: 2 - 450 cm
 *  minimum distance: 3 mm
 *  Pin: VCC, Trig(T), Echo, GND
 */

#ifndef HAL_INCLUDE_HCSR04_H_
#define HAL_INCLUDE_HCSR04_H_


#include "type.h"

extern void HCSR04_init(uint8_t ssTrig);

extern unsigned int getDistance();



#endif /* HAL_INCLUDE_HCSR04_H_ */

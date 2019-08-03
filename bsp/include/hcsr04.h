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
 *
 *  Trig(T) - Pin 2.4
 *  Echo  - Pin 2.0
 */

#ifndef BSP_INCLUDE_HCSR04_H_
#define BSP_INCLUDE_HCSR04_H_


#include "type.h"

/*
 * Initialize the pin which have external pulse edge interrupt routine
 * */
extern void HCSR04_init();

extern unsigned int getDistance();



#endif /* BSP_INCLUDE_HCSR04_H_ */

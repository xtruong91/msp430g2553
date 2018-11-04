/*
 * button.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_GPIO_H_
#define DRIVER_GPIO_H_
#include "type.h"

#define OUTPUT      1
#define INPUT       0
#define HIGH        1
#define LOW         0

extern void pin_init(CallBack cbFunction);
/*
 * Pin 1.0 - pin 0, 1.1 - pin 1, ... pin 2.0 - pin 8, pin 2.1 - pin 9, pin 2.2 - pin 10
 * */
extern void pin_mode(uint8_t pin, uint8_t mode);
extern void digital_write(uint8_t pin, uint8_t value);


#endif /* DRIVER_GPIO_H_ */

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
extern void pin_mode(uint8_t pin, uint8_t mode);
extern void digital_write(uint8_t pin, uint8_t value);


#endif /* DRIVER_GPIO_H_ */

/*
 * gpio.h
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

#define P1_0        0
#define P1_1        1
#define P1_2        2
#define P1_3        3
#define P1_4        4
#define P1_5        5
#define P1_6        6
#define P1_7        7
#define P2_0        8
#define P2_1        9
#define P2_2        10
#define P2_3        11
#define P2_4        12
#define P2_5        13



/*
 * Configure for input/output
 * */
extern void pin_init();
/*
 * Pin 1.0 - pin 0, 1.1 - pin 1, ... pin 2.0 - pin 8, pin 2.1 - pin 9, pin 2.2 - pin 10
 * */
extern void pin_mode(uint8_t pin, uint8_t mode);
extern void digital_write(uint8_t pin, uint8_t value);

extern void attachInterrupt(CallBack IRCallback);


#endif /* DRIVER_GPIO_H_ */

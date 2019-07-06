/*
 * rovercar.h
 *
 *  Created on: Jun 28, 2019
 *      Author: truongtx
 */

#ifndef APP_INCLUDE_ROVERCAR_H_
#define APP_INCLUDE_ROVERCAR_H_

#include "helper.h"
#include "gpio.h"
#include "l298n.h"


#define STRTB       P1_0
#define BAUDRATE    9600

#define DEBUG


extern L298Config l298Config;
/*
 * Initialize all module;
 * @Return: Ret is failure or success
 * */
Ret Car_init();
/*
 * Run in while loop
 * */
Ret Car_run();

Ret Car_stop();


#endif /* APP_INCLUDE_ROVERCAR_H_ */

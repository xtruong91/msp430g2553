/*
 * pwm.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_PWM_H_
#define DRIVER_INCLUDE_PWM_H_
#include "type.h"

/*
 * initialize pwm module
 * */
BOOL pwm_init();
/*
 * set period timer;
 * */
void pwm_setperiod(uint16_t period);
/*
 *  set duty cycle
 * */
void pwm_setdutycycle(uint16_t period);




#endif /* DRIVER_INCLUDE_PWM_H_ */

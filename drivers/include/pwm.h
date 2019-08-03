/*
 * pwm.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef DRIVERS_INCLUDE_PWM_H_
#define DRIVERS_INCLUDE_PWM_H_
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




#endif /* DRIVERS_INCLUDE_PWM_H_ */

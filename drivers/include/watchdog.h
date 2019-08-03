/*
 * watchdog.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef DRIVERS_INCLUDE_WATCHDOG_H_
#define DRIVERS_INCLUDE_WATCHDOG_H_

/**
 * \brief Disable the watchdog timer module
 */
void watchdog_disable(void);

/**
 * \brief Enable the watchdog timer module
 * The watchdog timeout is set to an interval of 32768 cycles
 */
void watchdog_enable(void);

/**
 * \brief Pet the watchdog
 */
void watchdog_pet(void);



#endif /* DRIVERS_INCLUDE_WATCHDOG_H_ */

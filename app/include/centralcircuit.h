/*
 * centralcircuit.h
 *
 *  Created on: Jun 28, 2019
 *      Author: truongtx
 * For test all modules in the project.
 */

#ifndef APP_INCLUDE_CENTRALCIRCUIT_H_
#define APP_INCLUDE_CENTRALCIRCUIT_H_

#include "clock.h"
#include "ConfigChip.h"

#include "bq32000.h"
#include "hc06.h"
#include "hc595.h"
#include "sx1278.h"
#include "debug.h"
#include "gpio.h"
#include "clock.h"

#define LED P1_0

extern int CentralCircuit_init();

extern int CentralCircuit_run();


#endif /* APP_INCLUDE_CENTRALCIRCUIT_H_ */

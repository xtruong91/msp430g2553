/*
 * clock.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_CLOCK_H_
#define DRIVER_INCLUDE_CLOCK_H_

#include "type.h"


// These definitions help you to notice the system clocks
#define MCLK_F 1 // frequency of Master Clock in MHz
// You must change it to the right MCLK frequency that you configure through
// "Config_Clocks" function
// This definition affect delay functions
#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in Hz
// You must change it to the right SMCLK frequency that you configure through
// "Config_Clocks" function

typedef enum {
    CLK_1MHZ = 0,
    CLK_8MHZ,
    CLK_12MHZ,
    CLK_16MHZ
}eCLK;

/*
 * Initialize kind of source clock
 * @param: clk source clock
 * */
void clk_init(eCLK clk);

void delay_ms (int t);
void delay_us (int t);


#endif /* DRIVER_CLOCK_H_ */

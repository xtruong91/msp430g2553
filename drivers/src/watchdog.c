/*
 * watchdog.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#include <drivers/include/watchdog.h>
#include <msp430.h>

/**
 * \brief Disable the watchdog timer module
 */
void watchdog_disable(void)
{
    /* Hold the watchdog */
    WDTCTL = WDTPW + WDTHOLD;
}

/**
 * \brief Enable the watchdog timer module
 * The watchdog timeout is set to an interval of 32768 cycles
 */
void watchdog_enable(void)
{
    /* Read the watchdog interrupt flag */
    if (IFG1 & WDTIFG) {
        /* Clear if set */
        IFG1 &= ~WDTIFG;
    }

    watchdog_pet();
}

/**
 * \brief Pet the watchdog
 */
void watchdog_pet(void)
{
    /**
     * Enable the watchdog with following settings
     *   - sourced by ACLK
     *   - interval = 32786 / 12000 = 2.73s
     */
    WDTCTL = WDTPW + (WDTSSEL | WDTCNTCL);
}



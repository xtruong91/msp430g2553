/*
 * ConfigChip.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 *
 *      Change the compiling options
 */

#ifndef CONFIGCHIP_H_
#define CONFIGCHIP_H_

#include <msp430.h>
#include "type.h"
#include "ConfigChip.h"

#if(BOARD_VERSION == BOARD_VERSION_SLAVE1)
    //CPU clock rate
    #define FCY 1000000L
    //UART port
    #define UART_EN
    #define UART_BAUDRATE   115200
    // SPI protocol for transfer with master/slave
    #undef SPI1_EN
    #define SPI2_EN
    //I2C protocol used for transfer data with slave;
    #undef I2C_EN
#elif (BOARD_VERSION == BOARD_VERSION_SLAVE2)
        //CPU clock rate
    #define FCY 1000000L
    //UART port
    #define UART_EN     1
    #define UART_BAUDRATE   115200
    // SPI protocol for transfer with master/slave
    #define SPI1_EN     0
    #define SPI2_EN     0
    //I2C protocol used for transfer data with slave;
    #define I2C_EN     0
#elif (BOARD_VERSION == BOARD_VERSION_MASTER1)
        //CPU clock rate
    #define FCY 1000000L
    //UART port
    #define UART_EN     0
    #define UART_BAUDRATE   115200
    // SPI protocol for transfer with master/slave
    #define SPI1_EN     0
    #define SPI2_EN     0
    //I2C protocol used for transfer data with slave;
    #define I2C_EN     0
#elif (BOARD_VERSION == BOARD_VERSION_MASTER2)
    //CPU clock rate
    #define FCY 1000000L
    //UART port
    #define UART_EN     1
    #define UART_BAUDRATE   115200
    // SPI protocol for transfer with master/slave
    #define SPI1_EN     0
    #define SPI2_EN     0
    //I2C protocol used for transfer data with slave;
    #define I2C_EN     0
#else
    #error You forget to define the clock source the MSP430
#endif




#endif /* CONFIGCHIP_H_ */

/*
 * spi.h
 *
 *  Created on: Sep 19, 2018
 *      Author: truongtx
 */

//                    MSP430G2xx3
//                 -----------------
//             /|\|              XIN|-
//              | |                 |
//              --|RST          XOUT|-
//                |                 |
//                |             P1.2|-> Data Out (UCA0SIMO)
//                |                 |
//          LED <-|P1.0         P1.1|<- Data In (UCA0SOMI)
//                |                 |
//  Slave reset <-|P1.5         P1.4|-> Serial Clock Out (UCA0CLK)

#ifndef DRIVER_INCLUDE_SPI_H_
#define DRIVER_INCLUDE_SPI_H_

#include "type.h"
#include "ring_buffer.h"

typedef struct {
    BOOL master; // true: set master, otherwise set slave
    int8_t mode; // 0: configure SPIA, otherwise configure for SPIB
    uint16_t baudrate;

}spi_config;
// ring-buffer for usca1 module
rbd_t g_rbd1;
char g_rbmem1[BUFFER_LENGTH];

//ring buffer for usca2 module;
rbd_t g_rbd2;
char g_rbmem2[BUFFER_LENGTH];
/*
* \brief Initialize the SPI peripheral
* \param[in] config - the SPI configuration
* \return 0 on success, -1 otherwise
*/

BOOL spi_init(spi_config* config);

/*Enable interrupt receive data and transmit data*/
void spi_enableInt();

/*Disable interrupt*/
void spi_disableInt();

int8_t spi_getchar(void);

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
BOOL spi_putchar(int8_t c);

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
BOOL spi_puts(const int8_t *str);

#endif /* DRIVER_SPI_H_ */

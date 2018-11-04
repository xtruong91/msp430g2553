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

typedef enum{
    MASTER,
    SLAVE
}ESpiMode;

typedef enum{
    ModuleA,
    ModuleB
}ESpiType;

typedef enum{
    RSPI_100KHz,  //SMCLK/10
    RSPI_500KHz, // SMCLK/2
    RSPI_1MHz    // SMCLK
}ESpiBaudRate;

typedef struct {
    ESpiMode eMode;
    ESpiType eType;
    ESpiBaudRate eBaudRate;
}spi_config;


/*
* \brief Initialize the SPI peripheral
* \param[in] config - the SPI configuration
* \return 0 on success, -1 otherwise
*/

void spi_init(spi_config* config);

/*Enable interrupt receive data and transmit data*/
void spi_enableRxISR(void (*cbRxHandler)(void *args));
void spi_disableRxISR();

int8_t spi_getc(void);
int8_t spi_gets(int8_t *buffer, uint8_t length);

void spi_putc(const int8_t data);
int8_t spi_puts(const int8_t *buffer, uint8_t length);

#endif /* DRIVER_SPI_H_ */

/*
 * spi.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#include "spi.h"
#include <msp430.h>

static int8_t _mode;

BOOL spi_init(spi_config* config)
{
    if(config == NULL)
        return -1;
    _mode = config->mode;

    if(_mode == 0)
    {
        P1SEL = BIT1 + BIT2 + BIT4;
        P1SEL2 = BIT1 + BIT2 + BIT4;
        UCA0CTL0 |= UCCKPL + UCMSB + UCSYNC;  // 3-pin, 8-bit SPI master
        if(config->master == 1)
        {
            UCA0CTL0 |= UCMST;
        }
        UCA0CTL1 |= UCSSEL_2;                     // SMCLK
        UCA0BR0 |= 0x02;                          // /2
        UCA0BR1 = 0;                              //
        UCA0MCTL = 0;                             // No modulation
        UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }else
    {
        P1SEL |= BIT5 + BIT6 + BIT7;
        P1SEL2 |= BIT5 + BIT6 + BIT7;
        /*  UCSYNC     Sync-Mode
         *  UCCKPH     Mode: Clock Phase
         *  UCCKPL     Mode: Clock Polarity
         *  UCMST      Mode: Master Select
         *  UC7BIT     Mode: Data Bits  0:8-bits / 1:7-bits
         *  UCMSB      Mode: MSB first  0:LSB / 1:MSB
         *  UCMST      Mode: Master Select
         *  UCMODE_0   Mode: USCI Mode: 0; 3pin SPI default
         *  UCMODE_1    Mode: USCI Mode: 1; 4pin SPI
         *  UCMODE_2    Mode: USCI Mode: 2; 4pin SPI
         */
        UCB0CTL0 |= UCCKPL + UCMSB + UCSYNC;  // 3-pin, 8-bit SPI master
        if(config->master == 1)
        {
            UCB0CTL0 |= UCMST;
        }
        /*UCSSEL_1 ACLK
         *UCSSEL_2 SMCLK
         *UCSSEL_3 SMCLK
         */
        UCB0CTL1 |= UCSSEL_2;                     // SMCLK
        UCB0BR0 |= 0x02;                          // /2
        UCB0BR1 = 0;                              //
        UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }
    return 0;
}

/*Enable interrupt receive data and transmit data*/
void spi_enableInt()
{
    if(_mode == 0){
        IE2 |= UCA0RXIE;
    }else
    {
        IE2 |= UCB0RXIE;
    }
    __bis_SR_register(GIE);
}


/*Disable interrupt*/
void spi_disableInt()
{
    if(_mode == 0){
        IE2 &= ~UCA0RXIE;
    }else
    {
        IE2 &= ~UCB0RXIE;
    }
}

int8_t spi_getchar(void)
{
    int8_t retval = -1, c = -1;

    if(_mode == 0)
    {
        if (ring_buffer_get(g_rbd1, &c) == 0) {
            retval = (int8_t) c;
        }
        return retval;
    }else
    {
        if (ring_buffer_get(g_rbd2, &c) == 0) {
            retval = (int8_t) c;
        }
        return retval;
    }
}

/**
 * \brief Write a character to SPI
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
BOOL spi_putchar(int8_t data)
{
    if(_mode == 0)
    {
        while (!(IFG2 & UCA0TXIFG));          // USCI_A0 TX buffer ready?
        UCA0TXBUF = data;                     // Send next value
    }else
    {
        while (!(IFG2 & UCB0TXIFG));          // USCI_A0 TX buffer ready?
        UCB0TXBUF = data;                     // Send next value
    }
    return 0;
}

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
BOOL spi_puts(const int8_t *str)
{
    int8_t status = -1;
    if(str != NULL)
    {
        status = 0;
        while(*str != NULL)
        {
            if(_mode == 0)
            {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCA0TXIFG));

                /* Transmit data */
                UCA0TXBUF = *str;

                /*  If there is a line-feed, add a carriage return */
                if (*str == '\n') {
                    /* Wait for the transmit buffer to be ready */
                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = '\r';
                }
            }else
            {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCB0TXIFG));

                /* Transmit data */
                UCB0TXBUF = *str;

                /*  If there is a line-feed, add a carriage return */
                if (*str == '\n') {
                    /* Wait for the transmit buffer to be ready */
                    while (!(IFG2 & UCB0TXIFG));
                    UCB0TXBUF = '\r';
                }
            }
            str++;
        }
    }
    return status;
}



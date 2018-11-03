/*
 * i2c.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#include "i2c.h"
#include <msp430.h>

static int _transmit(const int8_t dev, const uint8_t *buf, size_t nbytes);
static int _receive(const int8_t dev, uint8_t *buf, size_t nbytes);
static int _check_ack(const int8_t dev);

/**
 * \brief Initialize the I2C peripheral
 * \return 0 on success, -1 otherwise
 */
BOOL i2c_init(const i2c_config* config)
{
    P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    /* Ensure USCI_B0 is in reset before configuring */
    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCMODE_3 | UCSYNC;

    if(config->master == 1)
    {
        UCB0CTL0 |= UCMST;
        UCB0I2CSA = config->address;
    }
    else
    {
        UCB0I2COA = config->address;
        UCB0I2CIE |= UCSTTIE;                     // Enable STT interrupt
    }
    /* Set USCI_B0 to master mode I2C mode */


    /**
     * Configure the baud rate registers for 100kHz when sourcing from SMCLK
     * where SMCLK = 1MHz
     *     UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
           UCB0BR1 = 0;
     */
    UCB0BR0 = 10;
    UCB0BR1 = 0;

    /* Take USCI_B0 out of reset and source clock from SMCLK */
    UCB0CTL1 |= UCSSEL_2;
    UCB0CTL1 &= ~UCSWRST;
    return 0;
}

void i2c_enableInt()
{
    IE2 |= UCB0TXIE;                          // Enable TX interrupt
    __bis_SR_register(GIE);
}
/*
 * Disable global interrupt
 * */
void i2c_disableInt()
{
    IE2 &= ~UCB0TXIE;
}

/**
 * \brief Perform an I2C transfer
 * \param[in] dev - the I2C slave device
 * \param[in/out] data - data structure containing the buffers
 * \return 0 on success, -1 otherwise
 */
BOOL i2c_transfer(const int8_t dev, struct i2c_data *data)
{
    int err = 0;

    /* Set the slave device address */
    UCB0I2CSA = dev;

    /* Transmit data is there is any */
    if (data->tx_len > 0) {
        err = _transmit(dev, (const uint8_t *) data->tx_buf, data->tx_len);
    }

    /* Receive data is there is any */
    if ((err == 0) && (data->rx_len > 0)) {
        err = _receive(dev, (uint8_t *) data->rx_buf, data->rx_len);
    } else {
        /* No bytes to receive send the stop condition */
        UCB0CTL1 |= UCTXSTP;
    }

    return err;
}

void i2c_gets(uint8_t addreg,uint8_t* buffer, uint8_t length)
{
    while (UCB0CTL1 & UCTXSTP);                // Loop until I2C STT is sent
    UCB0CTL1 |= UCTR + UCTXSTT;                // I2C TX, start condition

    while (!(IFG2&UCB0TXIFG));
    UCB0TXBUF = addreg;                        // Address start

    while (!(IFG2&UCB0TXIFG));

    UCB0CTL1 &= ~UCTR;                         // I2C RX
    UCB0CTL1 |= UCTXSTT;                       // I2C start condition
    IFG2 &= ~UCB0TXIFG;                        // Clear USCI_B0 TX int flag

    while (UCB0CTL1 & UCTXSTT);                // Loop until I2C STT is sent
    while (!(IFG2&UCB0RXIFG));
    unsigned char i;
    for( i=0;i<=length;i++)
     {
       *(buffer+i-1)= UCB0RXBUF;
        while (!(IFG2&UCB0RXIFG));
     }
    UCB0CTL1 |= UCTXSTP;                       // I2C stop condition after 1st TX
}

void i2c_puts(uint8_t addRegister,const uint8_t *data, uint8_t length)
{
    while (UCB0CTL1 & UCTXSTP);                // Loop until I2C STT is sent
    UCB0CTL1 |= UCTR + UCTXSTT;                // I2C TX, start condition

    while (!(IFG2&UCB0TXIFG));
    UCB0TXBUF = addRegister;

    while (!(IFG2&UCB0TXIFG));
    unsigned char i;
    for( i=0;i<length;i++)
     {
        UCB0TXBUF= *(data+i) ;
        while (!(IFG2&UCB0TXIFG));
     }
    UCB0CTL1 |= UCTXSTP;                       // I2C stop condition after 1st TX
    IFG2 &= ~UCB0TXIFG;                        // Clear USCI_B0 TX int flag
}

void i2c_putc(uint8_t addRes,const uint8_t c)
{
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition

    while (!(IFG2&UCB0TXIFG));
    UCB0TXBUF = addRes;

    while (!(IFG2&UCB0TXIFG));
    UCB0TXBUF = c;

    UCB0CTL1 |= UCTXSTP;                       // I2C stop condition after 1st TX
    IFG2 &= ~UCB0TXIFG;                        // Clear USCI_B0 TX int flag
}

int8_t i2c_getc()
{
    if(UCSTTIFG)
        return UCB0RXBUF;
    else
        return 0;
}
/**
 * \brief Check for ACK/NACK and handle NACK condition if occured
 * \param[in] dev - the I2C slave device
 * \return 0 if slave ACK'd, -1 if slave NACK'd
 */
static int _check_ack(const int8_t dev)
{
    int err = 0;

    /* Check for ACK */
    if (UCB0STAT & UCNACKIFG) {
        /* Stop the I2C transmission */
        UCB0CTL1 |= UCTXSTP;

        /* Clear the interrupt flag */
        UCB0STAT &= ~UCNACKIFG;

        /* Set the error code */
        err = -1;
    }

    return err;
}

/**
 * \brief Transmit data to the slave device
 * \param[in] dev - the I2C slave device
 * \param[in] buf - the buffer of data to transmit
 * \param[in] nbytes - the number of bytes to transmit
 * \return 0 on success, -1 otherwise
 */
static int _transmit(const int8_t dev, const uint8_t *buf, size_t nbytes)
{
    int err = 0;

    /* Send the start condition */
    UCB0CTL1 |= UCTR | UCTXSTT;

    /* Wait for the start condition to be sent and ready to transmit interrupt */
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

    /* Check for ACK */
    err = _check_ack(dev);

    /* If no error and bytes left to send, transmit the data */
    while ((err == 0) && (nbytes > 0)) {
        UCB0TXBUF = *buf;
        while ((IFG2 & UCB0TXIFG) == 0) {
            err = _check_ack(dev);
            if (err < 0) {
                break;
            }
        }

        buf++;
        nbytes--;
    }

    return err;
}

/**
 * \brief Receive data from the slave device
 * \param[in] dev - the I2C slave device
 * \param[in] buf - the buffer to store the received data
 * \param[in] nbytes - the number of bytes to receive
 * \return 0 on success, -1 otherwise
 */
static int _receive(const int8_t dev, uint8_t *buf, size_t nbytes)
{
    int err = 0;

    /* Send the start and wait */
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;

    /* Wait for the start condition to be sent */
    while (UCB0CTL1 & UCTXSTT);

    /*
     * If there is only one byte to receive, then set the stop
     * bit as soon as start condition has been sent
     */
    if (nbytes == 1) {
        UCB0CTL1 |= UCTXSTP;
    }

    /* Check for ACK */
    err = _check_ack(dev);

    /* If no error and bytes left to receive, receive the data */
    while ((err == 0) && (nbytes > 0)) {
        /* Wait for the data */
        while ((IFG2 & UCB0RXIFG) == 0);

        *buf = UCB0RXBUF;
        buf++;
        nbytes--;

        /*
         * If there is only one byte left to receive
         * send the stop condition
         */
        if (nbytes == 1) {
            UCB0CTL1 |= UCTXSTP;
        }
    }

    return err;
}




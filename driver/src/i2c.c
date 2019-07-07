/*
 * i2c.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#include "i2c.h"
#include "isr.h"
#include <msp430.h>

rbd_t g_rbusca2 = 2;
rb_attr_t g_rbusca2Attr = {sizeof(i2cbuffer[0]), ARRAY_SIZE(i2cbuffer), i2cbuffer};

static int _transmit(const int8_t dev, const uint8_t *buf, size_t nbytes);
static int _receive(const int8_t dev, uint8_t *buf, size_t nbytes);
static int _check_ack(const int8_t dev);

/**
 * \brief Initialize the I2C peripheral
 * \return 0 on success, -1 otherwise
 */
void i2c_init(const i2c_config* config)
{
    P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    /* Ensure USCI_B0 is in reset before configuring */
    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCMODE_3 | UCSYNC;

    if(config->mode == MASTER){
        UCB0CTL0 |= UCMST; // I2C Master, synchronous mode
        UCB0I2CSA = config->addrSlave; //// Set slave address
    }
    else{
        UCB0I2COA = config->addrSlave; //Own Address
        UCB0I2CIE |= UCSTTIE;                     // Enable STT interrupt
    }
    /* Set USCI_B0 to master mode I2C mode */
    if(config->baudRate == R_400K){
        UCB0BR0 = 40;                             // fSCL = SMCLK/40 = ~400kHz
        UCB0BR1 = 0;
    }else{
        UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
        UCB0BR1 = 0;
    }
    /* Take USCI_B0 out of reset and source clock from SMCLK */
    UCB0CTL1 |= UCSSEL_2;
    UCB0CTL1 &= ~UCSWRST;

    IE2 |= UCB0TXIE + GIE;                          // Enable TX interrupt
}

/*
 * Disable global interrupt
 * */
void i2c_disableRxISR()
{
    IE2 &= ~UCB0TXIE;
}

void i2c_setAddress(uint8_t address){
    UCB0CTL1 |= UCSWRST;
    UCB0I2CSA = address;                           // Set slave address
    UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
}

uint8_t i2c_getc(uint8_t addReg)
{
    while (UCB0CTL1 & UCTXSTP);                 // wait for I2C STT set flag
    UCB0CTL1 |= UCTR + UCTXSTT;                 // I2C TX,START

    while (!(IFG2&UCB0TXIFG));                  // wait for  completing transfer
    UCB0TXBUF = addReg;                        // the first send address

    while (!(IFG2&UCB0TXIFG));                  // wait for completing

    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C RESTART
    IFG2 &= ~UCB0TXIFG;                     // clear USCI_B0 TX flag

    while (UCB0CTL1 & UCTXSTT);             // Cho den khi I2C STT duoc gui di
    UCB0CTL1 |= UCTXSTP;                    // Gui bit STOP
    return UCB0RXBUF;
}

uint8_t i2c_gets(uint8_t addReg,uint8_t* buffer, uint8_t length){
    unsigned char i=0;
    while (UCB0CTL1 & UCTXSTP);             // Loop until I2C STT is sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

    while (!(IFG2&UCB0TXIFG));
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //Neu bao loi
    UCB0TXBUF = addReg;                          // Dia chi luu gia tri Seconds

    while (!(IFG2&UCB0TXIFG));
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //Neu bao loi

    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    while (UCB0CTL1 & UCTXSTT);             // Loop until I2C STT is sent
    for(i=0;i<(length-1);i++)
    {
        while (!(IFG2&UCB0RXIFG));
        IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
        buffer[i] = UCB0RXBUF;
    }
    while (!(IFG2&UCB0RXIFG));
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition after 1st TX
    buffer[length-1] = UCB0RXBUF;
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    return 0;
}

uint8_t i2c_putc(uint8_t addreg,const uint8_t data){
    while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

    while (!(IFG2&UCB0TXIFG));              // Cho cho bit START gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //Neu bao loi thì thoat khoi ham
    UCB0TXBUF = addreg;                    // Gui dia chi thanh ghi can ghi


    while (!(IFG2&UCB0TXIFG));              // Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //Neu bao loi thì thoat khoi ham
    UCB0TXBUF = data;                       // Gui du lieu

    while (!(IFG2&UCB0TXIFG));              // Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //Neu bao loi thì thoat khoi ham
    UCB0CTL1 |= UCTXSTP;                    // Gui bit STOP
    IFG2 &= ~UCB0TXIFG;                     // Xoa co USCI_B0 TX
    return 0;
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

    return TRUE;
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
    while ((err == 0) && (nbytes > 0))
    {
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




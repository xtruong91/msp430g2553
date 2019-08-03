/*
 * i2c.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 *      //                                /|\  /|\
//               MSP430G2xx3      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P1.7/UCB0SDA|<-|---+->|P1.7/UCB0SDA  XIN|-
//            |                 |  |      |                 | 32kHz
//           -|XOUT             |  |      |             XOUT|-
//            |     P1.6/UCB0SCL|<-+----->|P1.6/UCB0SCL     |
//            |                 |         |             P1.0|--> LED
 */

#ifndef DRIVERS_INCLUDE_I2C_H_
#define DRIVERS_INCLUDE_I2C_H_

#include <stdlib.h>
#include "type.h"

#define I2CBUFF_LENGTH 100

int8_t i2cbuffer[I2CBUFF_LENGTH];
/* I2C slave device structure */

typedef enum{
    MASTER,
    SLAVE
}EMode;

typedef enum{
    R_100K, // standard mode: 100 kbit/s
    R_400K, // fast mode: 400 kbit/s
    R_10K,  // low-speed mode: 10 kbit/s
    R_1M,   // fast mode plus 1Mbit
    R_3_4M  // high speed mode 3.5Mbit/s
}EBaudRate;

typedef struct
{
    uint8_t addrSlave; // set address of slave
    EBaudRate baudRate;
    EMode mode; //1: if set master mode, otherwise set slave mode
}i2c_config;

/* I2C transaction data */
struct i2c_data
{
    const void *tx_buf;
    size_t tx_len;
    void *rx_buf;
    size_t rx_len;
};

/**
 * \brief Initialize the I2C peripheral
 * \return 0 on success, -1 otherwise
 */
void i2c_init(const i2c_config* config);
void i2c_enableRxISR(void (*cbRxHandler)(void *args));
void i2c_disableRxISR();
void i2c_setAddress(uint8_t address);

uint8_t i2c_getc(uint8_t addReg);
uint8_t i2c_gets(uint8_t addReg,uint8_t* buffer, uint8_t length);

uint8_t i2c_putc(uint8_t addreg,const uint8_t data);
void i2c_puts(uint8_t addreg,const uint8_t *data, uint8_t length);

/**
 * \brief Perform an I2C transfer
 * \param[in] dev - the I2C slave device
 * \param[in/out] data - data structure containing the buffers
 * \return 0 on success, -1 otherwise
 */
BOOL i2c_transfer(const int8_t dev, struct i2c_data *data);


#endif /* DRIVERS_INCLUDE_I2C_H_ */

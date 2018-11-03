/*
 * i2c.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_I2C_H_
#define DRIVER_INCLUDE_I2C_H_

#include <stdlib.h>
#include "type.h"

/* I2C slave device structure */

typedef struct
{
    uint8_t address; // set address
    BOOL master; //1: if set master mode, otherwise set slave mode
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
BOOL i2c_init(const i2c_config* config);
void i2c_gets(uint8_t addreg,uint8_t* buffer, uint8_t length);
void i2c_puts(uint8_t addreg,const uint8_t *data, uint8_t length);
void i2c_putc(uint8_t addreg,const uint8_t c);
int8_t i2c_getc();
/*
 * Enable global interrupt;
 * */
void i2c_enableInt();
/*
 * Disable global interrupt
 * */
void i2c_disableInt();
/**
 * \brief Perform an I2C transfer
 * \param[in] dev - the I2C slave device
 * \param[in/out] data - data structure containing the buffers
 * \return 0 on success, -1 otherwise
 */
BOOL i2c_transfer(const int8_t dev, struct i2c_data *data);


#endif /* DRIVER_INCLUDE_I2C_H_ */

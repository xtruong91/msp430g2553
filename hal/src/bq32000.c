/*
 * BQ32000.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */

#include "bq32000.h"
#include "i2c.h"
#include <msp430.h>

// Initialize default value for BQ32000

void BQ32000_init(){
    i2c_config config = {BQ32000,R_100K, MASTER};
    i2c_init(&config);

    while(i2c_putc(CALCFG1_REG, 0x40));

    while((i2c_getc(CALCFG1_REG) & 0x40) != 0x40){
        while(i2c_putc(CALCFG1_REG, 0x40));
    }

    while(i2c_putc(SFKEY1_REG, 0x5E));
    while(i2c_putc(SFKEY2_REG, 0xC7));
    while(i2c_putc(SFR_REG, 0x01));
}

SDateTime BQ32000_readDateTime(){
    uint8_t buffer[8];
    i2c_gets(SEC_REG, buffer, 8);

    buffer[1] &= 0x7F;
    buffer[2] &= 0x7F;
    buffer[3] &= 0x3F;
    buffer[5] &= 0x3F;
    buffer[6] &= 0x1F;

    SDateTime sDateTime;
    sDateTime.second = ((buffer[1]>>4)*10 +  (buffer[1] & 0x0F));
    sDateTime.minute = ((buffer[2]>>4)*10 + (buffer[2] & 0x0F));
    sDateTime.hour   = ((buffer[3]>>4)*10 + (buffer[3] & 0x0F));
    sDateTime.day    = ((buffer[5]>>4)*10 + (buffer[5] & 0x0F)) ;
    sDateTime.month  = ((buffer[6]>>4)*10 + (buffer[6] & 0x0F));
    sDateTime.year   = ((buffer[7]>>4)*10 + (buffer[7] & 0x0F));
    return sDateTime;
}






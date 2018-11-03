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


static void _reset();
static int8_t _startRead(int8_t address);
static int8_t _startWrite(int8_t address, int8_t data);
static void _stop();
static int8_t _send(int8_t c);
const DateTime datetime = {0x30,0x00, 0x00, 0x00, 0x01, 0x11, 0x01, 0x14, 0x10};

BOOL BQ32000_init()
{
    i2c_config config = {BQ32000, 1};
    if(i2c_init(&config) != 0)
        return -1;
    _reset();
    i2c_putc(BQ32000, 0x00);
    _send(datetime.second);
    _send(datetime.minute);
    _send(datetime.hour);
    _send(datetime.day);
    _send(datetime.month);
    _send(datetime.year);
    _send(datetime.control);
    _stop();
    return 0;
}

int8_t BQ32000_readData(int8_t *buffer)
{
    int8_t aux;
    _reset();
    _startWrite(BQ32000,0x00);
    _startRead(BQ32000);

    for(aux = 0; aux < 7; aux++, buffer++){//6
       *buffer = i2c_getc();
    }

    UCB0CTL1 |= UCTXNACK;
    _stop();
    *buffer = i2c_getc();
    __delay_cycles(15);//15
    _stop();
    return aux;
}
int8_t BQ32000_writeData(const int8_t *data, int8_t length);

static void _reset()
{
    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;
    UCB0CTL1 = UCSSEL_2+UCSWRST;
    UCB0BR0 = 12;
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;
}

static int8_t _startRead(int8_t address)
{
    while((UCB0STAT & BUSY) != 0);
    UCB0I2CSA = address;
    UCB0CTL1 &=~UCTR;
    UCB0CTL1 |=UCTXSTT;
    while((UCB0CTL1  & UCTXSTT) != 0);
    return ((UCB0STAT & UCNACKIFG) != 0)? 0 : 1;
}
static int8_t _startWrite(int8_t address, int8_t data)
{
    while((UCB0STAT & BUSY) != 0);
    UCB0I2CSA = address;
    UCB0CTL1 |= UCTR + UCTXSTT;
    UCB0TXBUF = data;
    while(!(IFG2 & UCB0TXIFG));
    return ((UCB0STAT & UCNACKIFG) != 0)? 0 : 1;
}

static void _stop()
{
    while((UCB0STAT & BUSY) != 0);
    UCB0CTL1 |= UCTXSTP;
}

static int8_t _send(int8_t c)
{
    while((UCB0STAT & BUSY) != 0);
    UCB0TXBUF = c;
    while(!(IFG2 & UCB0TXIFG));
    return ((UCB0STAT & UCNACKIFG) != 0)? 0 : 1;
}






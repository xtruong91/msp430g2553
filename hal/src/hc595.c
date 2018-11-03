/*
 * hc595.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */
#include "hc595.h"
#include "gpio.h"
#include "clock.h"

BOOL hc595_init()
{
    // Set master mode, with configure SPI A and baudrate 1MHz
    spi_config config = {1, 0, 1000};

    // set 2.0 pin to latch pin
    pin_mode(8, OUTPUT);

    return spi_init(&config);
}

// Send number through down LED7Seg
BOOL hc595_sendNo(const uint16_t number)
{
    digital_write(8, LOW);
    if( spi_putchar(code7seg[number % 10]) != 0)
        return -1;
    if( spi_putchar(code7seg[(number/10)%10]) != 0)
        return -1;
    if( spi_putchar(code7seg[(number/100) % 10]) != 0)
        return -1;
    if( spi_putchar(code7seg[number/10000]) != 0)
        return -1;
    delay_ms(100);
    digital_write(8, HIGH);
    return 0;
}
BOOL hc595_sendCommand(const int8_t command)
{
    //TODO
    return 0;
}
BOOL hc595_sendString(const int8_t* str)
{
    //TODO
    return 0;
}





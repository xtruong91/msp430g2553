/*
 * hc595.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 */
#include "hc595.h"
#include "gpio.h"
#include "clock.h"
#include "spi.h"

const uint8_t code7seg[11] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x3F};

void HC595_init(){
    // Set master mode, with configure SPI A and baudrate 500KHz
    spi_config config = {MASTER, ModuleB , RSPI_500KHz};
    // set 2.0 pin to latch pin
    pin_mode(8, OUTPUT);
    spi_init(&config);
}

// Send number through down LED7Seg
void HC595_sendNumber(const uint16_t number)
{
    digital_write(8, LOW);
    spi_putc(code7seg[number % 10]);
    spi_putc(code7seg[(number/10)%10]);
    spi_putc(code7seg[(number/100) % 10]);
    spi_putc(code7seg[number/10000]);
    digital_write(8, HIGH);
}





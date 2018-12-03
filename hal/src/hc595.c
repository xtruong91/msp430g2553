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
//Bit co logic 0 sang      0    1    2   3     4    5    6    7   8     9        -    *C    F
//unsigned char code7seg[14]={0x28,0x7D,0x32,0x34,0x65,0xA4,0xA0,0x3D,0x20,0x24,0xFF,0xF7,0x8A,0xA3};

uint8_t g_ssPin;
void HC595_init(uint8_t ssPin){
    g_ssPin = ssPin;
    // Set master mode, with configure SPI A and baudrate 500KHz
    spi_config config = {MASTER, ModuleB , RSPI_500KHz};
    // set 2.0 pin to latch pin
    pin_mode(g_ssPin, OUTPUT);
    spi_init(&config);
}

// Send number through down LED7Seg
void HC595_sendNumber(const uint16_t number)
{
    uint16_t temp = number;
    digital_write(g_ssPin, LOW);
    spi_putc(code7seg[temp % 10]);
    temp /=10;
    spi_putc(code7seg[temp %10]);
    temp /=10;
    spi_putc(code7seg[temp % 10]);
    temp /=10;
    spi_putc(code7seg[temp]);
    digital_write(g_ssPin, HIGH);
}





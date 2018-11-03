/*
 * sx1278.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *      transmit and receive data via LoRa
 *
 *       *      Module Lora SX1278  MSP430G2553
        VCC     3.3VDC
        GND     GND
        M1  Any GPIO set to P2.0 / GND

        RXD Pin 1.1
        TXD Pin 1.2

        M0  Any GPIO set to 2.1 / GND
 */

#ifndef HAL_INCLUDE_SX1278_H_
#define HAL_INCLUDE_SX1278_H_

typedef enum {
    Normal,
    Wake_up,
    Power_saving,
    Sleep
}EMode;

void sx1278_init(EMode mode);
void sx1278_send(const int8_t* data, uint8_t length);
void sx1278_receive(int8_t* data, uint8_t length);



#endif /* HAL_INCLUDE_SX1278_H_ */

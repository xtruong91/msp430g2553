/*
 * sx1278.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *      transmit and receive data via LoRa
 *
 *       *      Module Lora SX1278  MSP430G2553
 *      f = 433MHz (420-150MHz)
 *      R = 10Km (3000m -no obstacle)
 *      support FSK  FSK, GFSK, MSK, GMSK, LoRaTM
 *      Votage = 3.3V (1.8V - 3.7V)
 *      P = 20dBm - 10mW
 *      P = 127dB
 *      CRC 256 byte
 *      R = 1200- 115200 bps.
 *      temperture =  -40- 85.
 *
 *      Map MSP430 and SX1278
        VCC     3.3VDC
        GND     GND
        M1  Any GPIO set to P2.1 & P2.2
        RXD Pin 1.1
        TXD Pin 1.2


 */

#ifndef HAL_INCLUDE_SX1278_H_
#define HAL_INCLUDE_SX1278_H_

#include "type.h"

typedef enum {
    Normal,         //Serial open and wireless open, transparent transmission
    Wake_up,        // Serial open and wireless open
    Power_saving,   //   Serial close and wireless wake-up mode
    Sleep           //Sleep, and can receive parameter setting command
}ELoraMode;

extern void sx1278_init();
extern void sx1278_setMode(ELoraMode mode);
extern void sx1278_enableRxISR(void (*cbRxHandler)(void *args));
extern void sx1278_disableRxISR();
extern void sx1278_send(const int8_t *data, uint8_t length);



#endif /* HAL_INCLUDE_SX1278_H_ */

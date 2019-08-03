/*
 * esp8266.h
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *      connect with wifi esp8266 via SPI
 *      Specification of ESP8266
 *      ESP8266 wifi SoC wifi 2.4 GHz, standard: 802.11 b/g/n
 *      communicate with GPIO by 3.3V
 *      flash 4MB
 *      encrypt: WPA/WPA2
 *      integrate TCP/IP
 *
 *      Mapping between ESP8266 - MSP430
 *      pin D5(HSCLK) - pin 1.5 (UCB0CLK)
 *      pin D7(HMOSI) - pin 1.7(UCB0SIMO)
 *      pin D6(HMISO) - pin 1.6 (UCB0SOMI)
 *
 */

#ifndef BSP_INCLUDE_ESP8266_H_
#define BSP_INCLUDE_ESP8266_H_

#include "type.h"

void esp8266_init();

void esp8266_enableRxISR(void (*RxHandler)(void *args));
void esp8266_disableRxISR();
void esp8266_send(const int8_t *buffer, uint8_t length);

#endif /* BSP_INCLUDE_ESP8266_H_ */

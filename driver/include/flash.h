/*
 * flash.h
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_FLASH_H_
#define DRIVER_INCLUDE_FLASH_H_

void flash_Init(void);
void flash_EraseSingleSegments(unsigned char * addr);
void flash_WriteByte(unsigned char * addr, unsigned char data);

#endif /* DRIVER_INCLUDE_FLASH_H_ */

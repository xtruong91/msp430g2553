/*
 * helper.h
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 */

#ifndef BOOTLOADER_INCLUDE_HELPER_H_
#define BOOTLOADER_INCLUDE_HELPER_H_

#define RESPONE_SUCCESS     1
#define RESPONE_FAIL        0

typedef struct
{
    unsigned int length;
    unsigned int type;
    unsigned int address;
    unsigned char data[16];
} ihex_format_t;

extern unsigned char hexStringToBin(const char *str, ihex_format_t *bin);


#endif /* BOOTLOADER_INCLUDE_HELPER_H_ */

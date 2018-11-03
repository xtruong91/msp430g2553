/*
 * type.h
 *
 *  Created on: Sep 18, 2018
 *      Author: truongtx
 */

#ifndef DRIVER_INCLUDE_TYPE_H_
#define DRIVER_INCLUDE_TYPE_H_

#include <stdint.h>
//typedef char BOOL;
//typedef char int8_t;
//typedef unsigned char uint8_t;
//typedef int int16_t;
//typedef unsigned int uint16_t;
//typedef unsigned long uint32_t;
//typedef long int32_t;

#define  BOOL int
//#define int8_t char
//#define uint8_t unsigned char
//#define int16_t int
//#define uint16_t  unsigned int
//#define uint32_t unsigned long
//#define int32_t long

#define TRUE            0
#define FALSE           -1


/* A union type for byte or word access for 16 bit values.  */
typedef union _union16 {
    uint16_t u16;
    uint8_t u8[2];
} union16;

/* A union type for byte, word, or dword access for 32 bit values.  */
typedef union _union32 {
    uint32_t u32;

    struct {
        uint16_t ls16;
        uint16_t ms16;
    } u16;
    uint8_t u8[4];
} union32;

/*----------------------------------------------------------------------*/

typedef union _UINT8 {

      uint8_t _uint8;           //uint8 viewed as an uint8

      uint8_t u8;               //uint8 viewed as an uint8
      struct {
            unsigned b0:        //bit 0 (the LSb) of the uint8
                1;
            unsigned b1:        //bit 1 of the uint8
                1;
            unsigned b2:        //bit 2 of the uint8
                1;
            unsigned b3:        //bit 3  of the uint8
                1;
            unsigned b4:        //bit 4  of the uint8
                1;
            unsigned b5:        //bit 5  of the uint8
                1;
            unsigned b6:        //bit 6  of the uint8
                1;
            unsigned b7:        //bit 7 (MSb) of the uint8
                1;
      };
} UINT8;


typedef void (*CallBack)(void *args);


#endif /* DRIVER_TYPE_H_ */

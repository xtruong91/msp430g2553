/*
 * type.h
 *
 *  Created on: Sep 18, 2018
 *      Author: truongtx
 */

#ifndef UTILS_INCLUDE_TYPE_H_
#define UTILS_INCLUDE_TYPE_H_

/***************************** Include Files *********************************/
#include <stdint.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/*-------------------------------------------------------------
 * NOTE: stdint.h define
 * typedef signed char int8_t;
 * typedef signed int int16_t;
 * typedef signed long int int32_t;
 * typedef signed long long int int64_t;

 * typedef unsigned char uint8_t;
 * typedef unsigned int uint16_t;
 * typedef unsigned long int uint32_t;
 * typedef unsigned long long int uint64_t;

-------------------------------------------------------------*/
#define uint8   uint8_t
#define uint16  uint16_t
#define uint32  uint32_t
#define uint64  uint64_t

#define int8    int8_t
#define int16   int16_t
#define int32   int32_t
#define int64   int64_t

/* Other definitions    */
#ifndef BYTE
#define BYTE        uint8_t
#define BYTE_MAX (  255)
#endif

#ifndef WORD
#define WORD        uint16_t
#define WORD_MAX    (65535U)
#endif

#ifndef DWORD
#define DWORD       uint32_t
#define DWORD_MAX   (4294967295UL)
#endif

#ifndef LPVOID
#define LPVOID  void*
#endif

#ifndef LPBYTE
#define LPBYTE  BYTE*
#endif

#ifndef LPWORD
#define LPWORD  WORD*
#endif

#ifndef LPDWORD
#define LPDWORD  DWORD*
#endif
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

/* A union type for byte, word, or dword access for 64 bit values.  */
typedef union _union64 {
  uint32_t u32[2];
  uint16_t u16[4];
  uint8_t u8[8];
} union64;

typedef union _unionfloat32{
    float f32;

    uint8_t u8[4];
} unionfloat32;

typedef union _unionfloat64{
    float f64;

    uint8_t u8[8];
} unionfloat64;

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


typedef union _UINT16 {

      uint16_t _uint16;                 //uint16 viewed as an uint16

      uint16_t u16;                     //uint16 viewed as an uint16
      struct {
            uint8_t u8Lsb;              //LSB (uint8) of the uint16
            uint8_t u8Msb;              //MSB (uint8) of the uint16
      };
      struct {
            uint8_t u8Bytes[2];         //The uint16 viewed as an array of two (2) uint8s
      };
      struct {

            unsigned b0:                //bit 0 (the LSb) of the uint16
                1;
            unsigned b1:                //bit 1 of the uint16
                1;
            unsigned b2:                //bit 2 of the uint16
                1;
            unsigned b3:                //bit 3 of the uint16
                1;
            unsigned b4:                //bit 4 of the uint16
                1;
            unsigned b5:                //bit 5 of the uint16
                1;
            unsigned b6:                //bit 6 of the uint16
                1;
            unsigned b7:                //bit 7 of the uint16
                1;
            unsigned b8:                //bit 8 of the uint16
                1;
            unsigned b9:                //bit 9 of the uint16
                1;
            unsigned b10:               //bit 10 of the uint16
                1;
            unsigned b11:               //bit 11 of the uint16
                1;
            unsigned b12:               //bit 12 of the uint16
                1;
            unsigned b13:               //bit 13 of the uint16
                1;
            unsigned b14:               //bit 14 of the uint16
                1;
            unsigned b15:               //bit 15 of the uint16
                1;
      };
} UINT16;

#define LSB(a)      ((a).u8Bytes[0])

#define MSB(a)      ((a).u8Bytes[1])
/*----------------------------------------------------------------------*/
typedef union _UINT32 {

      uint32_t _uint32;                         //uint32 viewed as an uint32

      uint32_t u32;                             //uint32 viewed as an uint32
      struct {
            uint8_t u8LoLsb;                    //The LSB of the least-signficant uint16 in the 32-bit data
            uint8_t u8LoMsb;                    //The MSB of the least-signficant uint16 in the 32-bit data
            uint8_t u8HiLsb;                    //The LSB of the most-signficant uint16 in the 32-bit data
            uint8_t u8HiMsb;                    //The MSB of the most-signficant uint16 in the 32-bit data
      };
      struct {
            uint16_t u16LoWord;                 //The least-significant uint16 in the 32-bit data
            uint16_t u16HiWord;                 //The most-significant uint16 in the 32-bit data
      };
      struct {
            uint16_t u16Words[2];               //The uint32 viewed as an array of two (2) uint16s
      };
      struct {
            uint8_t u8Bytes[4];                 //The uint32 viewed as an array of four (4) uint8s
      };
      struct {
            unsigned b0:
                1;
            unsigned b1:
                1;
            unsigned b2:
                1;
            unsigned b3:
                1;
            unsigned b4:
                1;
            unsigned b5:
                1;
            unsigned b6:
                1;
            unsigned b7:
                1;
            unsigned b8:
                1;
            unsigned b9:
                1;
            unsigned b10:
                1;
            unsigned b11:
                1;
            unsigned b12:
                1;
            unsigned b13:
                1;
            unsigned b14:
                1;
            unsigned b15:
                1;
            unsigned b16:
                1;
            unsigned b17:
                1;
            unsigned b18:
                1;
            unsigned b19:
                1;
            unsigned b20:
                1;
            unsigned b21:
                1;
            unsigned b22:
                1;
            unsigned b23:
                1;
            unsigned b24:
                1;
            unsigned b25:
                1;
            unsigned b26:
                1;
            unsigned b27:
                1;
            unsigned b28:
                1;
            unsigned b29:
                1;
            unsigned b30:
                1;
            unsigned b31:
                1;
      };
} UINT32;

#define LOWER_WORD(a)   ((a).u16Words[0])

#define UPPER_WORD(a)   ((a).u16Words[1])

#define LOWER_LSB(a)    ((a).u8Bytes[0])

#define LOWER_MSB(a)    ((a).u8Bytes[1])

#define UPPER_LSB(a)    ((a).u8Bytes[2])

#define UPPER_MSB(a)    ((a).u8Bytes[3])

/*----------------------------------------------------------------------*/
typedef enum _BOOL {
      FALSE = 0,
      TRUE
} BOOL;

/*
#ifndef bool
typedef enum _bool {
      false = 0,
      true
} bool;
#endif
*/

#define OK              TRUE

#ifndef NULL
#define NULL            0
#endif

#define NULLPTR         0               //An uninitialized pointer

#define NULLIDX         0xFF            //An unitialized index value


#define BIT10           0x0400          //Mask to represent bit 10
#define BIT11           0x0800          //Mask to represent bit 11
#define BIT12           0x1000          //Mask to represent bit 12
#define BIT13           0x2000          //Mask to represent bit 13
#define BIT14           0x4000          //Mask to represent bit 14
#define BIT15           0x8000          //Mask to represent bit 15 (the MSb of an uint16)

/*----------------------------------------------------------------------*/

/***************** Macros (Inline Functions) Definitions *********************/


#define BIT_SET_MASK(var, mask)             ((var) |= (mask))

#define BIT_CLEAR_MASK(var, mask)           ((var) &= (~(mask)))

#define BIT_TOGGLE_MASK(var, mask)          ((var) ^= (mask))

#define IS_BIT_SET_MASK(var, mask)          (((var) & (mask)))

#define IS_BIT_CLEAR_MASK(var, mask)        ((~(var) & (mask)))

#define BIT_SET(var, bitnum)             ((var) |= (1 << (bitnum)))

#define BIT_CLEAR(var, bitnum)           ((var) &= (~(1 << (bitnum))))

#define BIT_TOGGLE(var, bitnum)          ((var) ^= (1 << (bitnum)))

#define IS_BIT_SET(var, bitnum)          ((var) & (1 << (bitnum)))

#define IS_BIT_CLEAR(var, bitnum)        (~(var) & ((1 << (bitnum))))
/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*****************************************************************************/



typedef void (*CallBack)(void *args);


#endif /* DRIVER_TYPE_H_ */

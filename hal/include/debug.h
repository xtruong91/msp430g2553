/*
 * debug.h
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#ifndef HAL_INCLUDE_DEBUG_H_
#define HAL_INCLUDE_DEBUG_H_

#include "type.h"
#include <stdarg.h>

// Prototype for the function that is called when an invalid argument is passed
// to an API.  This is only used when doing a DEBUG build.
//
//*****************************************************************************
extern void __error__(char *pcFilename, uint32_t ui32Line);

#ifdef DEBUG
#define ASSERT(expr) do                                                       \
                     {                                                        \
                         if(!(expr))                                          \
                         {                                                    \
                             __error__(__FILE__, __LINE__);                   \
                         }                                                    \
                     }                                                        \
                     while(0)
#else
#define ASSERT(expr)
#endif


extern void UARTStdioConfig(uint16_t ui32Baud);
extern int UARTgets(char *pcBuf, uint16_t ui32Len);
extern unsigned char UARTgetc(void);
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
extern void UARTprintf(const char *pcString, ...);
extern void UARTvprintf(const char *pcString, va_list vaArgP);
extern int UARTwrite(const char *pcBuf, uint32_t ui32Len);


#endif /* HAL_INCLUDE_DEBUG_H_ */

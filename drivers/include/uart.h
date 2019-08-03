/*
 * uart.h
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 *      //                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.1/UCA0RXD|<------------
 */

#ifndef DRIVERS_INCLUDE_UART_H_
#define DRIVERS_INCLUDE_UART_H_

#include "type.h"
#include "ring_buffer.h"

#define BUFFER_LENGTH   128

/* Macro to ignore unused parameters */
#define IGNORE(p)         ((void) p)

typedef enum
{
    R_9600 = 9600,
    R_19200 = 19200
}Baudrate;

int8_t buffer[BUFFER_LENGTH];

typedef struct
{
    Baudrate baud;
} uart_config_t;


extern uart_config_t g_config;

void setObserver(CallBack callback);

/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int8_t uart_init(uart_config_t *config);

/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int8_t uart_getchar(void);

extern void uart_getc(char *ch);
extern void uart_getchars(char *str, unsigned int length);

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int8_t uart_putchar(int8_t c);

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int8_t uart_puts(const int8_t *str);

/*
 * write a number to UART;
 * */
void uart_putnum(uint32_t val, uint8_t dec, uint8_t neg);




#endif /* DRIVERS_INCLUDE_UART_H_ */

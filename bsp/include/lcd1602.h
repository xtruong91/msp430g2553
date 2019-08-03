/*
 * lcd1602.h
 *
 *  Created on: Oct 17, 2018
 *
 *  Description: Use SPI interface for transfer data between MCU and LCD1602
 *      Author: truongtx
 *      VSS:  ground
 *      VDD: Power supply for logic operating
 *      V0: Adjusting supply voltage for LCD driving
 *      RS: Signal for selecting registers
 *      - 1: Data register (for read and write)
 *      - 0: Instruction Register
 *      R/W: 1 - ReadMode, 0 - Write mode.
 *      E: An enable signal for writing or reading data.
 *      DB0 - DB7: Data bus.
 *      LED+ +5V
 *
 *      Mapping HC595 and LCD pin
 *      R/S - Q0
 *      R/W - Q1
 *      E   - Q2
 *
 *      DB4 - Q4
 *      DB5 - Q5
 *      DB6 - Q6
 *      DB7 - Q7
 *
 *
 */

#ifndef BSP_INCLUDE_LCD1602_H_
#define BSP_INCLUDE_LCD1602_H_

#include "gpio.h"
#include "type.h"

#define LCDLATCH P1_0

#define RS      P2_0
#define RW      P2_1
#define EN       P2_2

/* Display ON/OFF Control definitions */
#define DON          0x0F  /* Display on      */
#define DOFF         0x0B  /* Display off     */
#define CURSOR_ON    0x0F  /* Cursor on       */
#define CURSOR_OFF   0x0D  /* Cursor off      */
#define BLINK_ON     0x0F  /* Cursor Blink    */
#define BLINK_OFF    0x0E  /* Cursor No Blink */

/* Cursor or Display Shift definitions */
#define SHIFT_CUR_LEFT     0x04  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT    0x05  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT    0x06  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT   0x07  /* Display shifts to the right */

/* Function Set definitions */
#define FOUR_BIT    0x2C  /* 4-bit Interface               */
#define EIGHT_BIT   0x3C  /* 8-bit Interface               */
#define LINE_5X7    0x30  /* 5x7 characters, single line   */
#define LINE_5X10   0x34  /* 5x10 characters               */
#define LINES_5X7   0x38  /* 5x7 characters, multiple line */


//*****************************************************************************
// REGISTER DEFINITION
//*****************************************************************************
union reg
{    unsigned char _byte;            // byte declaration
     struct bit
     {
       unsigned char b0:1; //bit 0
       unsigned char b1:1; //bit 1
       unsigned char b2:1; //bit 2
       unsigned char b3:1; //bit 3
       unsigned char b4:1; //bit 4
       unsigned char b5:1; //bit 5
       unsigned char b6:1; //bit 6
       unsigned char b7:1; //bit 7
     } _bit;
};
/****************************************************************************
* FUNCTIONS 'S PROTOTYPES
******************************************************************************/
// For further description, see LCD.c
/*
 * Send a byte of data (rs == 0) or command (rs == 0) to lcd
 * */
void lcd_put_byte(unsigned char rs, unsigned char data);

void lcd_init();
void lcd_clear(void);
void lcd_gotoxy(unsigned char col, unsigned char row);
void lcd_putc(char c);
void lcd_puts(const char* s);
void lcd_clr_puts (const char* s1 , const char* s2);
void lcd_put_num (unsigned long val, char dec, unsigned char neg);


#endif /* BSP_INCLUDE_LCD1602_H_ */

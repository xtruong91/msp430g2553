/*
 * lcd1602.c
 *
 *  Created on: Jul 16, 2019
 *      Author: truongtx
 */
#include <bsp/include/lcd1602.h>
#include <drivers/include/clock.h>
#include <drivers/include/spi.h>

void
lcd_put_byte(unsigned char rs, unsigned char data)
{
    //unsigned char result;
    if(rs)
    {
        digital_write(RS, HIGH);
    }
    digital_write(EN, LOW);
    delay_us(20);


    //result = data & 0xF0;
    // send the high nibble
    digital_write(LCDLATCH, LOW);
    spi_putc(data);
    digital_write(LCDLATCH, HIGH);

    delay_us(20);
    digital_write(EN, HIGH);
    delay_us(20);

    digital_write(EN, LOW);
    delay_us(20);


    // send the low nibble
//    digital_write(LCDLATCH, LOW);
//    spi_putc((data & 0x0F) << 4);
//    digital_write(LCDLATCH, HIGH);
//
//    delay_us(20);
//    digital_write(EN, HIGH);
//    delay_us(20);
//    digital_write(EN, LOW);
}

void
lcd_init()
{
    spi_config config = {MASTER, ModuleB, RSPI_500KHz};
    spi_init(&config);

    pin_mode(LCDLATCH, OUTPUT);

    pin_mode(RS, OUTPUT);
    pin_mode(RW, OUTPUT);
    pin_mode(EN, OUTPUT);

    digital_write(RS, LOW);
    digital_write(RW, LOW);
    digital_write(EN, LOW);

    digital_write(LCDLATCH, LOW);
    spi_putc(0x30);
    digital_write(LCDLATCH, HIGH);

    delay_ms(100);

    // reset LCD
    lcd_put_byte(0,0x30);
    delay_ms(50);
    lcd_put_byte(0,0x30);
    delay_ms(50);
    lcd_put_byte(0,0x32);
    delay_ms(200);                // delay for LCD reset

//    delay_ms(2);     // wait for LCD
//    lcd_put_byte(0,EIGHT_BIT & LINES_5X7);            // Set LCD type
//    delay_ms(2);     // wait for LCD

    lcd_put_byte(0,DOFF&CURSOR_OFF&BLINK_OFF);        // display off
    delay_ms(2);     // wait for LCD
    lcd_put_byte(0,DON&CURSOR_OFF&BLINK_OFF);        // display on
    delay_ms(2);     // wait for LCD

    lcd_put_byte(0,0x01);              // clear display and move cursor to home
    delay_ms(2);     // wait for LCD
    lcd_put_byte(0,SHIFT_CUR_LEFT);                // cursor shift mode
    delay_ms(2);     // wait for LCD
    lcd_put_byte(0,0x01);              // clear display and move cursor to home
    delay_ms(2);     // wait for LCD

}

void
lcd_backlight(unsigned char on)
{

}

void
lcd_clear(void)
{
    lcd_put_byte(0, 0x01);
    delay_ms(2);
}

void
lcd_gotoxy(unsigned char col, unsigned char row)
{
    unsigned char address;

    if(row!=0)
        address=0x40;
    else
        address=0;

    address += col;
    lcd_put_byte(0,0x80|address);
    delay_ms(2);
}

void
lcd_putc(char c)
{
    switch(c)
    {
        case '\f':
            lcd_put_byte(0, 0x01);
            delay_ms(2);     // wait for LCD
            break;
        case '\n':
            lcd_gotoxy(0, 0x01);
            break;
        default:
            lcd_put_byte(1, c);
            delay_ms(2);     // wait for LCD
            break;
    }
}

void
lcd_puts(const char* s)
{
    while(*s)
    {
        lcd_putc(*s++);
    }
}

void
lcd_clr_puts (const char* s1 , const char* s2)
{
    lcd_clear ();
    lcd_puts (s1);
    lcd_gotoxy (0,1);
    lcd_puts (s2);
}

void
lcd_put_num (unsigned long val, char dec, unsigned char neg)
{
    char i, j, digit,k;
    long total;
    long temp;
    for (i = 0, total = val; total > 0;i++) // count number
        total /= 10;
        total = i;

    if (neg !=0 )
      lcd_putc ('-');

    if ((total - dec) <=0)  // if total < dec put 0.xxx
        lcd_putc('0');
    else
    {

      for (i=0; i< (total-dec); i++)
      {
        temp = 1;
        for (j=1;j<(total-i);j++)
            temp *=10;
        digit = (val/temp)%10;
        lcd_putc(digit + 0x30);
      }
    }
    if (dec > 0)
    {
      lcd_putc('.');
      if ( (dec - total)> 0)    // ex: val = 55; dec = 3 put 0.055
      {
        for ( i = 0; i < (dec-total);i++)
          lcd_putc('0');
        k = 0;
      }
      else
        k = total - dec;
      for (i=k; i< total; i++)
      {
       temp = 1;
       for (j=1;j<(total-i);j++)
           temp *=10;
       digit = (val/temp)%10;
       lcd_putc(digit + 0x30);
      }
    }
}


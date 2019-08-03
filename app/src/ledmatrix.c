/*
 * lebmatrix.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 *      use Timer send message each one second;
 */

#include "debug.h"
#include "lcd1602.h"
#include "clock.h"
#include "spi.h"
#include "uart.h"
#include "ConfigChip.h"
#include "uart.h"
#include "lebmatrix.h"



/*
 *Handle message with end message which '.'
 * */

void handleUartRecv(void *args)
{
    rbd_t *rbID = (rbd_t*)args;
    char data;
    int k;

    while(ring_buffer_get(*rbID, &data) == TRUE)
    {
        for(k = 0; k < sizeof(character); k++)
        {
            if(data == character[k])
            {
                blinkImage(characterHEX[k],1,30,30);
                //scrollImage(characterHEX[k]);
                break;
            }
        }
    }

    UARTprintf("Handled data \n");
}

Ret
LedMatrix_init()
{
    // initialize system clock;
    clk_init(CLK_1MHZ);

    // Init LED
    // Set master mode, with configure SPI A and baudrate 500KHz
    spi_config config = {MASTER, ModuleB , RSPI_500KHz};
    spi_init(&config);

    // set 1.0 pin to latch pin
    pin_mode(LATCH, OUTPUT);

    // Initialize UART;
    uart_init(&g_config);
    setObserver(handleUartRecv);

    lcd_init();


    // initialize debug module;
#if (DEBUG_EN > 0)
    UARTStdioConfig(UART_BAUDRATE);;
#endif

    return SUCCESS;
}

void test()
{
    digital_write(LATCH, LOW);

    spi_putc(0x00); //wired column with GND

    digital_write(LATCH, HIGH);

    delay_ms(2000);

    digital_write(LATCH, LOW);

    spi_putc(0xFF); //wired column with GND

    digital_write(LATCH, HIGH);

    delay_ms(2000);


}


Ret
LedMatrix_run()
{

//    int i;
//    for(i = 0; i < sizeof(character); i++)
//    {
//        blinkImage(characterHEX[i], 1, 30, 30);
//    }
//    displayImage(offLED,60);
//    //delay_ms(1000);
//    displayImage(onLED,60);

    lcd_clear();
    lcd_puts ("Xuan truong \n");
    //lcd_gotoxy (0,1);
    //lcd_put_num(12345, 3, 0);

    delay_ms(1000);

    //test();
    //initImage();
    return SUCCESS;
}


void
blinkImage(uint8 image[], int times, int on, int off)
{
    int i;
    for(i = 0; i < times; i++)
    {
        displayImage(image, on);
        clearImage(off);
    }
}

void
displayImage(uint8 image[],int duration)
{
    int hold, a;
    for(hold = 0; hold < duration; hold++)
    {
      for(a = 0; a < 8; a++)
      {
        digital_write(LATCH, LOW);

        uint8 rotate = ~image[a];
        spi_putc(rotate); // column2;
        spi_putc(row[a]); // row & column;

        digital_write(LATCH, HIGH);
        delay_ms(1);
      }
    }
}

void
initImage()
{
    //digital_write(LATCH, LOW);
    int index;
    for(index = 0; index < 8; index++)
    {
        digital_write(LATCH, LOW);

        spi_putc(0x00); //wired column with GND
        digital_write(LATCH, HIGH);

        digital_write(LATCH, LOW);
        spi_putc(0xFF); // wired row with VCC;

        digital_write(LATCH, HIGH);
        delay_ms(1);
    }

    delay_ms(1000);
    // turn off;

    for(index = 0; index < 8; index++)
    {
        digital_write(LATCH, LOW);

        spi_putc(0xFF); //wired column with GND
        spi_putc(0x00); // wired row with VCC;

        digital_write(LATCH, HIGH);
        delay_ms(1);
    }
    delay_ms(1000);
}

void
clearImage(int duration)
{
    int hold, a;
    for(hold=0;hold<duration;hold++)
    {
      for(a=0;a<8;a++)
      {
          digital_write(LATCH, LOW);

          spi_putc(0xFF);   //column
          spi_putc(row[a]); //row

          digital_write(LATCH, HIGH);
          delay_ms(1);
      }
    }
}

void scrollImage(uint8 image[])
{
    int shift,a;
    for(shift = 0; shift < 9; shift++)
    {
        for(a = 0; a < 8; a++)
        {
            digital_write(LATCH, LOW);
            spi_putc(~(image[a] << shift));
            spi_putc(row[a]);
            digital_write(LATCH, HIGH);
            delay_ms(1);
        }
    }
}



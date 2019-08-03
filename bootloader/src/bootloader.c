/*
 * bootloader.c
 *
 *  Created on: Apr 7, 2019
 *      Author: truongtx
 */

#include <drivers/include/flash.h>
#include <drivers/include/uart.h>
#include "bootloader.h"
#include "helper.h"
#include "type.h"

int8_t *menuStr = "------------MSP430 BOOT MENU------------\r\n"
        " (D):Download Program From Serial\r\n"
        " (E):Erase Flash\r\n"
        " (R):Reboot\r\n";

int8_t *commandStr = "command:";
int8_t *commandErrorStr = "command error!\r\n";

typedef int (*ApplicationMainFunc)();

const ApplicationMainFunc * const appMainFunc = (ApplicationMainFunc*) 0xCA1E;

unsigned int __attribute__((section(".mode_flag"))) isBootMode;

unsigned char currentState;
unsigned int count;
unsigned int preCount;

static void initSystemClock(void);
static void initTimer(void);
static void disableTimer(void);
static char checkApplication(void);
static void eraseApplication(void);
static void downloadProgram(void);
static void reboot(void);

void bootloader_main()
{
    char command[5] = { 0 };
    isBootMode = 1;
    initSystemClock();
    initTimer();
    uart_config_t config = {R_9600};
    uart_init(&config);
    flash_init();
    _EINT();

    currentState = START_STATE;
    count = 0;
    preCount = count;
    uart_puts("Press Any Key to Interrupt Boot!\r\n");

    while (1)
    {
        switch (currentState)
        {
            case START_STATE:
                if (count - preCount < 6 && (IFG2 & UCA0RXIFG))
                {
                    uart_getc((char *) 0x00);
                    currentState = MENU_STATE;
                    uart_puts(menuStr);
                    uart_puts(commandStr);
                    disableTimer();
                    _DINT();
                }
                else if (count - preCount > 6)
                {
                    currentState = START_APPLICATION_STATE;
                    disableTimer();
                    _DINT();
                }
                break;

            case MENU_STATE:
                uart_getchars(command, 5);
                if (command[1] != '\0')
                {
                    uart_puts(commandErrorStr);
                    uart_puts(commandStr);
                }
                else
                {
                    switch (command[0])
                    {
                        case 'D':
                        case 'd':
                            downloadProgram();
                            uart_puts(commandStr);
                            break;
                        case 'E':
                        case 'e':
                            eraseApplication();
                            uart_puts(commandStr);
                            break;
                        case 'R':
                        case 'r':
                            __delay_cycles(500000);
                            reboot();
                            break;
                        default:
                            uart_puts(commandErrorStr);
                            uart_puts(commandStr);
                            break;
                    }
                }
                break;

            case START_APPLICATION_STATE:
                if (checkApplication() == 1)
                {
                    currentState = END_STATE;
                    isBootMode = 0;
                    (*appMainFunc)();
                } else {
                    currentState = MENU_STATE;
                    uart_puts("Boot Error, No Application Found!\r\n");
                    uart_puts(menuStr);
                    uart_puts(commandStr);
                }
                break;
        }
    }
}

static void initSystemClock(void)
{
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}

static void initTimer(void)
{
    TA0CTL |= 0x01 << 2;
    TA0CTL = (0x02 << 8) + (0x03 << 6) + (0x01 << 4); //125000
    TA0CCTL0 |= 0x01 << 4;
    TA0CCR0 = 62500; //0.5s
}

static void disableTimer(void)
{
    TA0CTL |= 0x01 << 2;
    TA0CTL &= ~(0x03 << 4);
    TA0CCTL0 &= ~(0x01 << 4);
}

static char checkApplication(void)
{
    if ((unsigned int) (*appMainFunc) == 0xFFFF)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static void eraseApplication(void)
{
    unsigned char *ptr;

    _DINT();
    uart_puts("Erase Start\r\n");

    for (ptr = APP_SEGMENT_START; ptr <= APP_SEGMENT_END; ptr += 0x200)
    {
        flash_eraseSegment((SegmentAddr)ptr);
        uart_putchar('.');
    }

    uart_puts("\r\nErase Complete!\r\n");
}

static void downloadProgram(void)
{
    ihex_format_t format;
    unsigned int index;
    char buffer[43];
    char command = 0;
    char length;
    char temp;

    uart_putchar('n');

    while (1)
    {
        uart_getc(&command);
        switch (command)
        {
            case 's':
                uart_getc(&length);

                /*
                 * Get length of data transfer from flasher
                 */
                for (index = 0; index < length; index++)
                {
                    uart_getc(&temp);
                    buffer[index] = temp;
                }

                /*
                 * Parse the Intel Hex format
                 */
                if (!hexStringToBin(buffer, &format))
                {
                    uart_putchar('e');
                    return;
                }

                /*
                 * Checking the address of application
                 */
                if (((unsigned char *) (format.address)) < APP_ROM_START
                        || ((unsigned char *) (format.address)) > APP_ROM_END)
                {
                    uart_putchar('e');
                    return;
                }

                /*
                 * Type 0x00 ==> Data record
                 * Just write data record to flash
                 */
                if (format.type == 0)
                {
                    flash_writeBlock((SegmentAddr)format.address,(char*)format.data, format.length);
                }

                uart_putchar('n');
                break;

            case 'f':
                uart_putchar('n');
                return;
        }
    }
}

static void reboot(void)
{
    /* Write a wrong value to force reset (PUC is generated) */
    WDTCTL = 0x00;
}

/*
 * lebmatrix.c
 *
 *  Created on: Oct 17, 2018
 *      Author: truongtx
 *
 *      use Timer send message each one second;
 */

#include "ConfigChip.h"
#include "clock.h"
#include "gpio.h"
#include "debug.h"
#include "hc595.h"
#include "uart.h"
#include "lebmatrix.h"



/*
 *Handle message with end message which '.'
 * */
static void handleUartRecv(void *args)
{
    rbd_t *rbID = (rbd_t*)args;
    UARTprintf("Received: %s \n", *data);
}

Ret
LedMatrix_init()
{
    // initialize system clock;
    clk_init(CLK_1MHZ);

    // Init HC595
    HC595_init(P1_0);

    // Initialize UART;
    uart_init(&g_config);
    setObserver(handleUartRecv);

    // initialize debug module;
#if (DEBUG_EN > 0)
    UARTStdioConfig(UART_BAUDRATE);;
#endif

    return SUCCESS;
}

uint8_t ledValue = 0;

int8_t rx;
Ret
LedMatrix_run()
{
    //UARTprintf("value: %x \n", ledValue);
    HC595_sendNumber(ledValue);
    delay_ms(1000);
    ledValue++;

    return SUCCESS;
}

void
LedMatrix_blinkLed(void)
{
    digital_write(P1_0, HIGH);
    delay_ms(1000);
    digital_write(P1_0, LOW);
    delay_ms(1000);
}



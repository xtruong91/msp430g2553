/*
 * isr.c
 *
 *  Created on: Sep 25, 2018
 *      Author: truongtx
 */
#include <drivers/include/isr.h>
#include <msp430.h>
#include "ConfigChip.h"
#include "type.h"

// notify changed ringbuffer to the observer;
void notify()
{
    if(ISRCallback != NULL)
        ISRCallback((void*)&g_rbuart);
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
#if defined ( UART_EN ) || defined (SPI1_EN)
    if(UC0IFG & UCA0TXIFG)
    {
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    }
#endif // end of USCI_A0 interrupt

#if defined (SPI2_EN) || defined(I2C_EN)
    if( UC0IFG & UCB0TXIFG )
    {
        UC0IE &= ~UCB0TXIE; // Disable USCI_B0 TX interrupt
    }
#endif // end of USCI_B0 interrupt
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
#if defined ( UART_EN ) || defined (SP1_EN)
    // receive data through UART
    if( UC0IFG & UCA0RXIFG )
    {
        const int8_t data = UCA0RXBUF;
        /*Clear the interrupt flag*/
        IFG2 &= ~UCA0RXIFG;
        /*push data into the buffer if overflow buffer -> stop receive*/
        if(ring_buffer_put(g_rbuart, &data) == FALSE)
        {
            return;
        }
        // receive enough data -> notify all observers.
        if(data == ENDMARKED)
        {
            notify();
        }
    }
#endif // end of module UCA0

#if !defined ( SPI2_EN )
    // defined SPI2
    if( UC0IFG & UCB0RXIFG)
    {
        /*Clear the interrupt flag*/
        IFG2 &= ~UCB0RXIFG;
        const int8_t data = UCB0RXBUF;

        if(ring_buffer_put(g_rbusca2, &data) == FALSE)
        {
            return;
        }

        if(data == ENDMARKED)
        {
            notify();
        }
    }
#elif !defined (I2C_EN)
    if (UCB0STAT & UCSTTIFG)
    {
        i2c_rx_address = 0;
        UCB0STAT &= ~UCSTTIFG;
    }
    if(UCB0STAT & UCSTPIFG)
    {
        UCB0STAT &= ~UCSTPIFG;
    }
    /*Clear the interrupt flag*/
    IFG2 &= ~UCB0RXIFG;
    const int8_t data = UCB0RXBUF;
    ring_buffer_put(g_rbusca2, &data);
    if(data == ENDMARKED)
    {
        notify();
    }
}
#endif // end of UCB0 module;
}



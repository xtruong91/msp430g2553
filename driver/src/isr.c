/*
 * isr.c
 *
 *  Created on: Sep 25, 2018
 *      Author: truongtx
 */
#include <msp430.h>
#include "isr.h"
#include "ConfigChip.h"
#include "type.h"

static CallBack _callback[3] = {NULL, NULL, NULL};

void subscribe(const isr_config *config)
{
    if(config == NULL)
        return;
    switch(config->module){
        case RX_UART:{
            _callback[0] = config->cbFunction;
            break;
        }
        case RX_SPI:{
            _callback[1] = config->cbFunction;
            break;
        }
        case RX_I2C:{
            _callback[2] = config->cbFunction;
            break;
        }
        default:{
            break;
        }
    }
}

void unsubscribe(const isr_config *config)
{
    if(config == NULL)
        return ;
    switch(config->module){
        case RX_UART:{
            _callback[0] = NULL;
            break;
        }
        case RX_SPI:{
            _callback[1] = NULL;
            break;
        }
        case RX_I2C:{
            _callback[2] = NULL;
            break;
        }
        default:{
            break;
        }
    }
}




#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
#if defined ( UART_EN ) || defined (SPI1_EN)
    if(UC0IFG & UCA0TXIFG)
    {
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
//        if(_callbackTX[0] != NULL)
//            _callbackTX[0](0);
    }
#endif // end of USCI_A0 interrupt

#if defined (SPI2_EN) || defined(I2C_EN)
    if( UC0IFG & UCB0TXIFG )
    {
        UC0IE &= ~UCB0TXIE; // Disable USCI_B0 TX interrupt
//        if(_callbackTX[1] != NULL)
//            _callbackTX[1](0);
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
#ifdef RINGBUFF
        ring_buffer_put(g_rbd1, &data);
#endif
        if(_callback[0] != NULL)
        {
            _callback[0]((void*)&data);
        }
    }
#endif // end of module UCA0

#if defined ( SPI2_EN )
    // defined SPI2
    if( UC0IFG & UCB0RXIFG)
    {
        /*Clear the interrupt flag*/
        IFG2 &= ~UCB0RXIFG;
        const int8_t data = UCB0RXBUF;
#ifdef RINGBUFF
        ring_buffer_put(g_rbd2, &data);
#endif
        if(_callback[1] != NULL)
        {
            _callback[1]((void*)&data);
        }
    }
#elif defined (I2C_EN)
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
    ring_buffer_put(g_rbd2, &data);
    if(_callback[2] != NULL)
    {
        _callback[2]((void*)&data);
    }
}
#endif // end of UCB0 module;
}



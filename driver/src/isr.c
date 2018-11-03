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

static CallBack _callbackTX[2] = {NULL, NULL};
static CallBack _callbackRX[2] = {NULL, NULL};

BOOL subscribe(const isr_config *config)
{
    if(config == NULL)
        return -1;
    if(config->mode == 0)
    {
        if(config->RxHandler == 1){
            _callbackRX[0]= config->CallBack;
        }else
        {
            _callbackTX[0] = config->CallBack;
        }
    }else
    {
        if(config->RxHandler == 1){
            _callbackRX[1]= config->CallBack;
        }else
        {
            _callbackTX[1] = config->CallBack;
        }
    }
    return 0;
}

BOOL unsubscribe(const isr_config *config)
{
    if(config == NULL)
        return -1;
    if(config->mode == 0)
    {
        if(config->RxHandler == 1){
            _callbackRX[0]= NULL;
        }else
        {
            _callbackTX[0] = NULL;
        }
    }else
    {
        if(config->RxHandler == 1){
            _callbackRX[1]= NULL;
        }else
        {
            _callbackTX[1] = NULL;
        }
    }
    return 0;
}




#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
#if defined ( UART_EN ) || defined (SPI1_EN)
    if(UC0IFG & UCA0TXIFG)
    {
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
        if(_callbackTX[0] != NULL)
            _callbackTX[0](0);
    }
#endif // end of USCI_A0 interrupt

#if defined (SPI2_EN) || defined(I2C_EN)
    if( UC0IFG & UCB0TXIFG )
    {
        UC0IE &= ~UCB0TXIE; // Disable USCI_B0 TX interrupt
        if(_callbackTX[1] != NULL)
            _callbackTX[1](0);
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
        ring_buffer_put(g_rbd1, &data);
        if(_callbackRX[0] != NULL)
        {
            _callbackRX[0]((void*)&data);
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
        ring_buffer_put(g_rbd2, &data);
        if(_callbackRX[1] != NULL)
        {
            _callbackRX[1]((void*)&data);
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
    if(_callbackRX[1] != NULL)
    {
        _callbackRX[1]((void*)&data);
    }
}
#endif // end of UCB0 module;
}



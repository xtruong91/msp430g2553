/*
 * spi.c
 *
 *  Created on: Oct 16, 2018
 *      Author: truongtx
 */

#include <msp430.h>
#include "spi.h"
#include "isr.h"


static ESpiType _eType;
static isr_config _isrConfig;
void spi_init(spi_config* config)
{
    _eType = config->eType;
    if(config->eType == ModuleB){
        P1SEL |= BIT5 + BIT6 + BIT7;
        P1SEL2 |= BIT5 + BIT6 + BIT7;
        /*  UCSYNC     Sync-Mode, UCCKPH     Mode: Clock Phase, UCCKPL     Mode: Clock Polarity
         *  UCMST      Mode: Master Select, UC7BIT     Mode: Data Bits  0:8-bits / 1:7-bits
         *  UCMSB      Mode: MSB first  0:LSB / 1:MSB, UCMST      Mode: Master Select
         *  UCMODE_0   Mode: USCI Mode: 0; 3pin SPI default, UCMODE_1    Mode: USCI Mode: 1; 4pin SPI
         *  UCMODE_2    Mode: USCI Mode: 2; 4pin SPI
         */
        UCB0CTL0 |= UCCKPL + UCMSB + UCSYNC;  // 3-pin, 8-bit SPI master
        if(config->eMode == MASTER)
        {
            UCB0CTL0 |= UCMST; //master select
        }
        /*UCSSEL_1 ACLK
         *UCSSEL_2 SMCLK
         *UCSSEL_3 SMCLK
         */
        UCB0CTL1 |= UCSSEL_2;                     // SMCLK
        switch(config->eBaudRate){
            case RSPI_100KHz:{
                UCB0BR0 |= 0x0A;                          // /10
                UCB0BR1 = 0;
                break;
            }
            case RSPI_500KHz:{
                UCB0BR0 |= 0x02;                          // /2
                UCB0BR1 = 0;
                break;
            }
            case RSPI_1MHz:{
                UCB0BR0 |= 0x01;                          // /1
                UCB0BR1 = 0;
                break;
            }
            default:{
                UCB0BR0 |= 0x05;                          // /5
                UCB0BR1 = 0;
                break;
            }
        }
        UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }else{
        P1SEL = BIT1 + BIT2 + BIT4;
        P1SEL2 = BIT1 + BIT2 + BIT4;
        UCA0CTL0 |= UCCKPL + UCMSB + UCSYNC;  // 3-pin, 8-bit SPI master
        if(config->eMode == MASTER)
        {
            UCA0CTL0 |= UCMST;
        }
        UCA0CTL1 |= UCSSEL_2;                     // SMCLK

        switch(config->eBaudRate){
            case RSPI_100KHz:{
                UCA0BR0 |= 0x0A;                          // /10
                UCA0BR1 = 0;
                break;
            }
            case RSPI_500KHz:{
                UCB0BR0 |= 0x02;                          // /2
                UCA0BR1 = 0;
                break;
            }
            case RSPI_1MHz:{
                UCA0BR0 |= 0x01;                          // /1
                UCA0BR1 = 0;
                break;
            }
            default:{
                UCA0BR0 |= 0x05;                          // /5
                UCA0BR1 = 0;
                break;
            }
        }
        UCA0MCTL = 0;                             // No modulation
        UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }
}

/*Enable interrupt receive data and transmit data*/
void spi_enableRxISR(void (*cbRxHandler)(void *args))
{
    if(_eType == ModuleA){
        IE2 |= UCA0RXIE;
    }else
    {
        IE2 |= UCB0RXIE;
    }
    __bis_SR_register(GIE);
    _isrConfig.module       = RX_SPI;
    _isrConfig.cbFunction   = cbRxHandler;
    subscribe(&_isrConfig);
}


/*Disable interrupt*/
void spi_disableRxISR()
{
    if(_eType == ModuleA){
        IE2 &= ~UCA0RXIE;
    }else{
        IE2 &= ~UCB0RXIE;
    }
    unsubscribe(&_isrConfig);
}

static int8_t spiA_getc(){
    while (!(IFG2 & UCA0RXIFG));              // USCI_A0 RX buffer ready?
    const int8_t data = UCA0RXBUF;
    return data;
}
static int8_t spiB_getc(){
    while (!(IFG2 & UCB0RXIFG));              // USCI_A0 RX buffer ready?
    const int8_t data = UCB0RXBUF;
    return data;
}

int8_t spi_getc(void)
{
    if(_eType == ModuleA){
        return spiA_getc();
    }else{
        return spiB_getc();
    }
}

int8_t spi_gets(int8_t *buffer, uint8_t length){
    int8_t index;
    for(index = 0; index < length - 1; index++){
        buffer[index] = spi_getc();
    }
    return length;
}


static void spiA_putc(const int8_t data){
    while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = data;
}
static void spiB_putc(const int8_t data){
    while (!(IFG2 & UCB0TXIFG));              // USCI_B0 TX buffer ready?
    UCB0TXBUF = data;
}

void spi_putc(const int8_t data){
    if(_eType == ModuleA){
        spiA_putc(data);
    }else{
        spiB_putc(data);
    }
}

int8_t spi_puts(const int8_t *buffer, uint8_t length){
    int8_t index;
    for(index = 0; index < length-1; index++){
        spiA_putc(buffer[index]);
    }
    return length;
}



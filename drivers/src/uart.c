#include <drivers/include/isr.h>
#include <drivers/include/uart.h>
#include "ring_buffer.h"
#include <stdint.h>
#include <stddef.h>
#include <msp430.h>

rbd_t g_rbuart = 1;

rb_attr_t g_rbuartAttr = {sizeof(buffer[0]), ARRAY_SIZE(buffer), buffer};

CallBack ISRCallback = NULL;

void setObserver(CallBack callback)
{
    ISRCallback = callback;
}

/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int8_t uart_init(uart_config_t *config)
{
    int status = -1;
    // USCI should be in reset before configuring - only configrure once
    if(UCA0CTL1 & UCSWRST)
    {
        P1SEL |= BIT1 + BIT2 ;                     // Set P1.1 as RXD
        P1SEL2 |= BIT1 + BIT2 ;                    // Set P1.2 as TXD
        /* Set clock source to SMCLK */
        UCA0CTL1 |= UCSSEL_2;

        if(config->baud == R_19200 )
        {
            UCA0BR0 = 52;                             // 1MHz 19200
            UCA0BR1 = 0;                              // 1MHz 19200
        }else
        {
            UCA0BR0 = 104;                            // 1MHz 9600
            UCA0BR1 = 0;                              // 1MHz 9600
        }
        UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
        UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

        // init ringbuffer;
        /* Initialize the ring buffer */
        if (ring_buffer_init(&g_rbuart, &g_rbuartAttr) == TRUE)
        {
            /* Enable rx interrupts */
            IE2 |= UCA0RXIE;
            __bis_SR_register(GIE);
        }
    }

    return status;
}


void uart_getc(char *ch)
{
    while (!(IFG2 & UCA0RXIFG));
    *ch = UCA0RXBUF;
    IFG2 &= ~UCA0RXIFG;
}

void
uart_getchars(char *str, unsigned int length)
{
    unsigned int index;
    char temp;

    index = 0;
    temp = 0;

    while (1)
    {
        uart_getc(&temp);
        if (temp == '\r')
        {
            uart_putchar('\n');
            break;
        }

        if (index < length)
        {
            str[index] = temp;
            index++;
        }
    }

    str[index] = '\0';
}

/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int8_t
uart_getchar(void)
{
    int retval = -1;
#ifdef RINGBUFF
    int c = -1;
    if (ring_buffer_get(g_rbd1, &c) == 0) {
        retval = (int) c;
    }
#else
    while(!(IFG2&UCA0RXIFG));   // Wait until USCI_A0 RX receive a complete character
    retval =  UCA0RXBUF; // assign RX buffer to function 's name
#endif
    return retval;
}

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int8_t uart_putchar(int8_t c)
{
    /* Wait for the transmit buffer to be ready */
    while (!(IFG2 & UCA0TXIFG));

    /* Transmit data */
    UCA0TXBUF = (char ) c;

    return 0;
}

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int8_t
uart_puts(const int8_t *str)
{
    int status = -1;

    if (str != NULL) {
        status = 0;

        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(IFG2 & UCA0TXIFG));

            /* Transmit data */
            UCA0TXBUF = *str;

            /*  If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = '\r';
            }

            str++;
        }
    }

    return status;
}

void uart_putnum(uint32_t val, uint8_t dec, uint8_t neg){
    char i, j, digit,k;
    long total;
    long temp;
    for (i = 0, total = val; total > 0;i++) // count number
        total /= 10;
        total = i;

    if (neg !=0 )
        uart_putchar ('-');

    if ((total - dec) <=0)  // if total < dec put 0.xxx
        uart_putchar('0');
    else
    {

      for (i=0; i< (total-dec); i++)
      {
        temp = 1;
        for (j=1;j<(total-i);j++)
            temp *=10;
        digit = (val/temp)%10;
        uart_putchar(digit + 0x30);
      }
    }
    if (dec > 0)
    {
        uart_putchar('.');
      if ( (dec - total)> 0)    // ex: val = 55; dec = 3 put 0.055
      {
        for ( i = 0; i < (dec-total);i++)
            uart_putchar('0');
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
       uart_putchar(digit + 0x30);
      }
    }
}


#include "uart.h"
#include "ring_buffer.h"
#include "isr.h"
#include <stdint.h>
#include <stddef.h>
#include <msp430.h>

struct baud_value
{
    uint32_t baud;
    uint16_t UCAxBR0;
    uint16_t UCAxBR1;
    uint16_t UCAxMCTL;
};

#ifdef RINGBUFF
/* Table of baud rate register values from reference manual (SLAU144) */
static const struct baud_value _baud_tbl[] = {
    {9600, 104, 0, 0x2}
};

/* RX ring bufer */
extern rbd_t g_rbd1;
extern char g_rbmem1[BUFFER_LENGTH];

#endif
/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int8_t uart_init(uart_config_t *config)
{
    int status = -1;

    P1SEL |= BIT1 + BIT2 ;                     // Set P1.1 as RXD
    P1SEL2 |= BIT1 + BIT2 ;                    // Set P1.2 as TXD
#ifdef RINGBUFF
    /* USCI should be in reset before configuring - only configure once */
    if (UCA0CTL1 & UCSWRST) {
        size_t i;

        /* Set clock source to SMCLK */
        UCA0CTL1 |= UCSSEL_2;

        /* Find the settings from the baud rate table */
        for (i = 0; i < ARRAY_SIZE(_baud_tbl); i++) {
            if (_baud_tbl[i].baud == config->baud) {
                break;
            }
        }

        if (i < ARRAY_SIZE(_baud_tbl)) {
            rb_attr_t attr = {sizeof(g_rbmem1[0]), ARRAY_SIZE(g_rbmem1), g_rbmem1};

            /* Set the baud rate */
            UCA0BR0 = _baud_tbl[i].UCAxBR0;
            UCA0BR1 = _baud_tbl[i].UCAxBR1;
            UCA0MCTL = _baud_tbl[i].UCAxMCTL;

            /* Initialize the ring buffer */
            if (ring_buffer_init(&g_rbd1, &attr) == 0) {
                /* Enable the USCI peripheral (take it out of reset) */
                UCA0CTL1 &= ~UCSWRST;

                /* Enable rx interrupts */
                IE2 |= UCA0RXIE;

                status = 0;
            }
        }
    }
#else
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    if(config->baud == 19200 ){
        UCA0BR0 = 52;                             // 1MHz 19200
        UCA0BR1 = 0;                              // 1MHz 19200
    }else{
        UCA0BR0 = 104;                            // 1MHz 9600
        UCA0BR1 = 0;                              // 1MHz 9600
    }
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    status = 0;
#endif

    return status;
}

static isr_config isrConfig;
void uart_enableRXInt(void (*cbRxHandler)(void *args))
{
    IE2 |= UCA0RXIE;
    __bis_SR_register(GIE);

    isrConfig.module = RX_UART;
    isrConfig.cbFunction = cbRxHandler;
    subscribe(&isrConfig);
}

/*Disable interrupt*/
void uart_disableRXInt()
{
    IE2 &= ~UCA0RXIE;
    unsubscribe(&isrConfig);
}
/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int8_t uart_getchar(void)
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
int8_t uart_puts(const int8_t *str)
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


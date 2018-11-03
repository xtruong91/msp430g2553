#include "uart.h"
#include "ring_buffer.h"
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

/* Table of baud rate register values from reference manual (SLAU144) */
static const struct baud_value _baud_tbl[] = {
    {9600, 104, 0, 0x2}
};

/* RX ring bufer */
extern rbd_t g_rbd1;
extern char g_rbmem1[BUFFER_LENGTH];

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

    return status;
}

void uart_enableInt()
{
    IE2 |= UCA0RXIE;
    __bis_SR_register(GIE);
}

/*Disable interrupt*/
void uart_disableInt()
{
    IE2 &= ~UCA0RXIE;
}
/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int8_t uart_getchar(void)
{
    int retval = -1;
    int c = -1;

    if (ring_buffer_get(g_rbd1, &c) == 0) {
        retval = (int) c;
    }

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


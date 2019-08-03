/*
 * Debug.c
 *
 *  Created on: Nov 4, 2018
 *      Author: truongtx
 */

#include "debug.h"
#include "uart.h"

static const char *g_pcHex = "0123456789abcdef";

void UARTStdioConfig(uint16_t ui32Baud)
{
    Baudrate baudrate = (Baudrate)ui32Baud;
    uart_config_t config = {baudrate};
    uart_init(&config);
}
int UARTgets(char *pcBuf, uint16_t ui32Len){
    ASSERT(pcBuf != 0);
    ASSERT(ui32Len != 0);
    uint16_t ui32Count = 0;
    int8_t cChar;
    static int8_t bLastWasCR = 0;
    // Adjust the length back by 1 to leave space for the trailing
    // null terminator.
    ui32Len--;
    while(1){
        // Read the next character from the console.
        cChar = uart_getchar();
        // See if the backspace key was pressed.
        if(cChar == '\b'){
            // If there are any characters already in the buffer, then delete
            // the last.
            if(ui32Count){
                // Rub out the previous character.
                UARTwrite("\b \b", 3);
                // Decrement the number of characters in the buffer.
                ui32Count--;
            }
            // Skip ahead to read the next character.
            continue;
        }
        // If this character is LF and last was CR, then just gobble up the
        // character because the EOL processing was taken care of with the CR.
        if((cChar == '\n') && bLastWasCR){
            bLastWasCR = 0;
            continue;
        }
        // See if a newline or escape character was received.
        if((cChar == '\r') || (cChar == '\n') || (cChar == 0x1b)){
            // If the character is a CR, then it may be followed by a LF which
            // should be paired with the CR.  So remember that a CR was
            // received.
            //
            if(cChar == '\r'){
                bLastWasCR = 1;
            }
            // Stop processing the input and end the line.
            break;
        }
        // Process the received character as long as we are not at the end of
        // the buffer.  If the end of the buffer has been reached then all
        // additional characters are ignored until a newline is received.
        if(ui32Count < ui32Len){
            // Store the character in the caller supplied buffer.
            pcBuf[ui32Count] = cChar;
            // Increment the count of characters received.
            ui32Count++;
            // Reflect the character back to the user.
            uart_putchar(cChar);
        }
    }
    // Add a null termination to the string.
    pcBuf[ui32Count] = 0;
    UARTwrite("\r\n", 2);
    return(ui32Count);
}

unsigned char UARTgetc(void){
    return (unsigned char)uart_getchar();
}

void UARTprintf(const char *pcString, ...){
    va_list vaArgP;
    // Start the varargs processing.
    va_start(vaArgP, pcString);
    UARTvprintf(pcString, vaArgP);
    // We're finished with the varargs now.
    va_end(vaArgP);
}
//! A simple UART based vprintf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param vaArgP is a variable argument list pointer whose content will depend
//! upon the format string passed in \e pcString.
//!
//! This function is very similar to the C library <tt>vprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments in the variable arguments list must match the
//! requirements of the format string.  For example, if an integer was passed
//! where a string was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
void UARTvprintf(const char *pcString, va_list vaArgP){
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;
    ASSERT(pcString != 0);
    while(*pcString){
        // Find the first non-% character, or the end of the string.
        for(ui32Idx = 0;
            (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++){}
        // Write this portion of the string.
        UARTwrite(pcString, ui32Idx);
        // Skip the portion of the string that was written.
        pcString += ui32Idx;
        // See if the next character is a %.
        if(*pcString == '%'){
            // Skip the %.
            pcString++;
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            ui32Count = 0;
            cFill = ' ';
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
again:
            // Determine how to handle the next character.
            switch(*pcString++){
                // Handle the digit characters.
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':{
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    if((pcString[-1] == '0') && (ui32Count == 0)){
                        cFill = '0';
                    }
                    // Update the digit count.
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';
                    // Get the next character.
                    goto again;
                }
                // Handle the %c command.
                case 'c':{
                    // Get the value from the varargs.
                    ui32Value = va_arg(vaArgP, uint32_t);
                    // Print out the character.
                    UARTwrite((char *)&ui32Value, 1);
                    // This command has been handled.
                    break;
                }
                // Handle the %d and %i commands.
                case 'd':
                case 'i':{
                    // Get the value from the varargs.
                    ui32Value = va_arg(vaArgP, int16_t);
                    // Reset the buffer position.
                    ui32Pos = 0;
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    if((int32_t)ui32Value < 0){
                        // Make the value positive.
                        ui32Value = -(int32_t)ui32Value;
                        // Indicate that the value is negative.
                        ui32Neg = 1;
                    }
                    else{
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        ui32Neg = 0;
                    }
                        // Set the base to 10.
                        ui32Base = 10;
                        // Convert the value to ASCII.
                        goto convert;
                 }
                // Handle the %s command.
                case 's':{
                    // Get the string pointer from the varargs.
                    pcStr = va_arg(vaArgP, char *);
                    // Determine the length of the string.
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++){}
                    // Write the string.
                    UARTwrite(pcStr, ui32Idx);
                    // Write any required padding spaces
                    if(ui32Count > ui32Idx){
                        ui32Count -= ui32Idx;
                        while(ui32Count--){
                            UARTwrite(" ", 1);
                        }
                    }
                    // This command has been handled.
                    break;
                }
                case 'u':{
                    // Get the value from the varargs.
                    ui32Value = va_arg(vaArgP, uint32_t);
                    // Reset the buffer position.
                    ui32Pos = 0;
                    // Set the base to 10.
                    ui32Base = 10;
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    ui32Neg = 0;
                    // Convert the value to ASCII.
                    goto convert;
                }
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                case 'x':
                case 'X':
                case 'p':{
                    // Get the value from the varargs.
                    ui32Value = va_arg(vaArgP, uint32_t);
                    // Reset the buffer position.
                    ui32Pos = 0;
                    // Set the base to 16.
                    ui32Base = 16;
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    ui32Neg = 0;
                    // Determine the number of digits in the string version of
                    // the value.
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--){}
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    if(ui32Neg){
                        ui32Count--;
                    }
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    if(ui32Neg && (cFill == '0')){
                        // Place the minus sign in the output buffer.
                        pcBuf[ui32Pos++] = '-';
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        ui32Neg = 0;
                    }
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    if((ui32Count > 1) && (ui32Count < 16)){
                        for(ui32Count--; ui32Count; ui32Count--){
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }
                    // If the value is negative, then place the minus sign
                    // before the number.
                    if(ui32Neg){
                        // Place the minus sign in the output buffer.
                        pcBuf[ui32Pos++] = '-';
                    }
                    // Convert the value into a string.
                    for(; ui32Idx; ui32Idx /= ui32Base){
                        pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }
                    // Write the string.
                    UARTwrite(pcBuf, ui32Pos);
                    // This command has been handled.
                    break;
                }
                // Handle the %% command.
                case '%':{
                    // Simply write a single %.
                    UARTwrite(pcString - 1, 1);
                    // This command has been handled.
                    break;
                }
                // Handle all other commands.
                default:{
                    // Indicate an error.
                    UARTwrite("ERROR", 5);
                    // This command has been handled.
                    break;
                }
            }
        }
    }
}

int
UARTwrite(const char *pcBuf, uint32_t ui32Len){
    // Check for valid UART base address, and valid arguments.
    ASSERT(pcBuf != 0);
    unsigned int uIdx;
    // Send the characters
    for(uIdx = 0; uIdx < ui32Len; uIdx++){
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        if(pcBuf[uIdx] == '\n'){
            uart_putchar('\r');
        }
        // Send the character to the UART output.
        uart_putchar(pcBuf[uIdx]);
    }
    return(uIdx);
}


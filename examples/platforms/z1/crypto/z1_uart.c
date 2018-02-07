#include <iomacros.h>
#include <msp430f2617.h>
#include <stdarg.h>
#include <stdio.h>

#include "z1_uart.h"
#include "common.h"


char to_hex(uint8_t h)
{
    switch(h) {
        case 0x0: { return '0'; }
        case 0x1: { return '1'; }
        case 0x2: { return '2'; }
        case 0x3: { return '3'; }
        case 0x4: { return '4'; }
        case 0x5: { return '5'; }
        case 0x6: { return '6'; }
        case 0x7: { return '7'; }
        case 0x8: { return '8'; }
        case 0x9: { return '9'; }
        case 0xA: { return 'A'; }
        case 0xB: { return 'B'; }
        case 0xC: { return 'C'; }
        case 0xD: { return 'D'; }
        case 0xE: { return 'E'; }
        case 0xF: { return 'F'; }
        default:  { return '!'; }
   }
}

void uart_write_hex(uint8_t h)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = to_hex(h >> 4);
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = to_hex(0x0F & h);
}

void uart_write_n(char * s, uint8_t n)
{
    for(uint8_t i = 0; i < n; i++)
    {
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = s[i];
    }
}

void uart_write(char * s)
{
    uint8_t i = 0;
 
    while(*s != '\0' && i < 255)
    {
        i++;
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = *s++;
    }
}

void uart_init(void)
{
	/* Select UART for P3.4-5 */
	P3SEL |= 0x30;
	UCA0CTL0 = 0x00;
	/* SMCLK */
	UCA0CTL1 = UCSSEL_2;
	/* Tested divider for 115200 */
	UCA0BR0 = 0x44;
	UCA0BR1 = 0x00;
	/* No modulation */
	UCA0MCTL = (UCBRS_0 | UCBRF_0);
	/* Turn on */
	UCA0CTL1 &= ~UCSWRST;
}

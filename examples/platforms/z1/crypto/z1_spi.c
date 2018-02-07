#include <msp430f2617.h>
#include <stdio.h>
#include <stddef.h>

#include "z1_spi.h"
#include "common.h"


uint8_t spi_send_receive(uint8_t m)
{
	while(!(IFG2 & UCB0TXIFG)) ;

	UCB0TXBUF = m;

	while(!(IFG2 & UCB0RXIFG)) ;

	return UCB0RXBUF;
}

/*void spi_send(uint8_t m)
{
	while(!(IFG2 & UCB0TXIFG)) ;

	UCB0TXBUF = m;
}*/

void spi_init()
{
	/* Select SPI for P3.1-3 */
	P3SEL |= BV(1) | BV(2) | BV(3);
	P3DIR |= BV(1) | BV(3);
	P3DIR &= ~BV(2);
	/* Turn off SPI */
	UCB0CTL1 = UCSWRST;
	/* Synchronous, MSB first, Master */
	UCB0CTL0 = UCCKPH | UCSYNC | UCMST | UCMSB;
	/* Select SubMaster clk */
	UCB0CTL1 |= UCSSEL_2;
	/* Clock Prescaler */
	UCB0BR0 = 0x02;
	UCB0BR1 = 0x00;
	/* Turn on SPI */
	UCB0CTL1 &= ~UCSWRST;
}

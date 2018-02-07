#include <iomacros.h>
#include <msp430f2617.h>
#include <stdarg.h>
#include <stdio.h>

#include "z1_spi.h"
#include "z1_uart.h"
#include "z1_cc2420.h"
#include "common.h"

uint8_t cc2420_status()
{
	/* CS low */
	P3OUT &= ~0x01;

	uint8_t ret = spi_send_receive(SREG | SWRITE | CC2420_SNOP);

	/* CS high */
	P3OUT |= 0x01;
	return ret;
}

void cc2420_strobe(enum cc2420_strobe s)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SREG | SWRITE | s);

	/* CS high */
	P3OUT |= 0x01;
}

uint16_t cc2420_reg_read(enum cc2420_register reg)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SREG | SREAD | (reg & 0x3F));
	uint8_t ret1 = spi_send_receive(SREG | SWRITE | CC2420_SNOP);
	uint8_t ret0 = spi_send_receive(SREG | SWRITE | CC2420_SNOP);

	/* CS high */
	P3OUT |= 0x01;
	return (ret1 << 8) | (ret0);
}

void cc2420_reg_write(enum cc2420_register reg, uint16_t m)
{
	uint8_t buffer[3];
	buffer[0] = reg;
	buffer[1] = (uint8_t) (m >> 8);
	buffer[2] = (uint8_t) m;

	/* CS low */
	P3OUT &= ~0x01;

	for(uint8_t i = 0; i < 3; i++) {
		spi_send_receive(buffer[i]);
	}

	/* CS high */
	P3OUT |= 0x01;
}

void cc2420_ram_read_msb(enum cc2420_address addr, uint8_t *m, uint16_t l)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SRAM | ((addr >> 1) & 0x7F));
	spi_send_receive(((addr >> 1) & 0xC0) | 0x20);

	for(uint16_t i = 0; i < l; i++) {
		m[i] = spi_send_receive(0x00);
	}

	/* CS high */
	P3OUT |= 0x01;
}

void cc2420_ram_write_msb(enum cc2420_address addr, uint8_t *m, uint16_t l)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SRAM | ((addr >> 1) & 0x7F));
	spi_send_receive((addr >> 1) & 0xC0);

	for(uint16_t i = 0; i < l; i++) {
		spi_send_receive(m[i]);
	}

	/* CS high */
	P3OUT |= 0x01;
}

void cc2420_ram_read_lsb(enum cc2420_address addr, uint8_t *m, uint16_t l)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SRAM | (addr & 0x7F));
	spi_send_receive(((addr >> 1) & 0xC0) | 0x20);

	for(uint16_t i = l; i > 0; i--) {
		m[i-1] = spi_send_receive(0x00);
	}

	/* CS high */
	P3OUT |= 0x01;
}

void cc2420_ram_write_lsb(enum cc2420_address addr, uint8_t *m, uint16_t l)
{
	/* CS low */
	P3OUT &= ~0x01;

	spi_send_receive(SRAM | (addr & 0x7F));
	spi_send_receive((addr >> 1) & 0xC0);

	for(uint16_t i = l; i > 0; i--) {
		spi_send_receive(m[i-1]);
	}

	/* CS high */
	P3OUT |= 0x01;
}

void cc2420_init()
{
	/* Set CS (P3.0) to high */
	P3DIR |= BV(0);
	P3OUT |= BV(0);

	P3REN |= BV(2);
	P3OUT |= BV(2);

	/* Set vreg and res high
	   https://github.com/herrfz/openwsn-fw/blob/master/bsp/chips/cc2420/radio.c */
	P4DIR |= BV(5) | BV(6);
	/* VREG startub is 0.6ms = 0x12C0 */
	P4OUT |= BV(5);
	/* TODO */
	__delay_cycles(5*SYSFREQ/1000);

	/* RESET */
	P4OUT &= ~BV(6);
	/* TODO */
	__delay_cycles(5*SYSFREQ/1000);
	P4OUT |= BV(6);

	/*cc2420_reg(0x10, 0x0000);
	__delay_cycles(0.005*SYSFREQ);
	cc2420_reg(0x10, 0xF800);
	__delay_cycles(0.005*SYSFREQ);
	*/

	cc2420_strobe(CC2420_SXOSCON);

	uint8_t tries = 0;
	while(!(cc2420_status() & BV(CC2420_XOSC16M_STABLE)))
	{
		if(tries++ == 255) break;
	}
}

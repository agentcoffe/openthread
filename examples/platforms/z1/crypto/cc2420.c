#include <iomacros.h>
#include <msp430f2617.h>
#include <stdarg.h>
#include <stdio.h>

#include "z1_spi.h"
#include "z1_uart.h"
#include "z1_cc2420.h"
#include "common.h"


#define DCO_8MHZ		0x60
#define BCS1_8MHZ		0x8D
#define BCS2_8MHZ		0x00
#define BCS3_8MHZ		0x00

#define DCO_12MHZ		0x60
#define BCS1_12MHZ		0x8E
#define BCS2_12MHZ		0x00
#define BCS3_12MHZ		0x00

#define DCO_16MHZ		0x60
#define BCS1_16MHZ		0x8F
#define BCS2_16MHZ		0x00
#define BCS3_16MHZ		0x00


struct cc2420_nonce {
	uint8_t flags;
	uint8_t source_address[8];
	uint32_t frame_count;
	uint8_t key_seq_count;
	uint16_t block_count;
};

uint8_t t_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
uint8_t t_msg[] = "No life till leather, We are gonna kick some ass tonight, We got the metal madness";  // 83 with \0
uint8_t t_ath[] = "Metallica"; // 10

void encrypt_test(uint8_t *key, uint8_t *msg, uint8_t msg_len, uint8_t *auth, uint8_t auth_len)
{
	if(msg_len > 128)
		return;

	if(((auth_len % 2 ) != 0) ||
	    (auth_len > 16) ||
		(auth_len < 4))
		return;

	/* Set key */
	//cc2420_ram_write_lsb(CC2420RAM_KEY1, key, 16);
	cc2420_ram_write_lsb(CC2420RAM_KEY0, key, 16);

	/* Set nonce
	*  | 1:Flags | 8:Source Addr | 4:FrameC | 1:KeyC | 2:BlockC |
	*
	* Flags:
	*  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	*  |        0      | A |    L'     |
	*/

	uint8_t L = 0;
	uint8_t A = (auth_len == 0 ? 0 : 1);
	uint8_t M = auth_len;
	struct cc2420_nonce nonce = {
		.source_address = {0xFB, 0x13, 0x3D, 0xBD, 0xE0, 0xDB, 0x51, 0x67}
	};

	/*for(len = msg_len; len; len >>= 8)
		L++;

	 * The c2420 allows max. 128 bytes, which
	 * corresponds to an L of 1 but
	 * the min. L allowed is 2, so set L to 2.
	 */
	L = 2;

	/* Since the msp430 is little endian
	 * the ints can simply be transferred to
	 * the cc2420. (all security related data
	 * is stored little endian there. P.45)
	 */
	nonce.flags = ((A << 3) | (L-1));
	nonce.frame_count = 0;
	nonce.key_seq_count = 0x00;
	nonce.block_count = 0x0001;

	//cc2420_ram_write_msb(CC2420RAM_TXNONCE, (uint8_t*) &nonce, sizeof(nonce));

	/* Layout of SECCTRL0
	 * | 15 : 10 | 9 | 8 || 7 | 6 | 5 | 4 : 2 | 1 : 0 |
	 * |    0    | 1 | 1 || 1 | 1 | 0 |   M'  | CCM(3)|
	 *
	 *         0x03              0xC0 | (M' << 2) | 0x03
	 *
	 * Not sure if M' needs to be set here or in SECCTRL1.SEC_TXL
	 * the datasheet is not very specific, so setting both.
	 */
	// TODO: use enums of cc2420.h
	//cc2420_reg_write(CC2420_SECCTRL0, 0x03C3 | (((M - 2) >> 1) << 2));

	/* Layout of SECCTRL0
	 * | 15 | 14 : 8 || 7 | 6 : 0 |
	 * |  0 |  l(a)  || 0 |   0   |
	 *     l(a) << 8     0x00
	 */
	//cc2420_reg_write(CC2420_SECCTRL1, 0x0000 | (auth_len << 8));

	/* Writing the data.
	 * TXFIFO + |         0         | 1 : auth_len-1 | auth_len : auth_len+msg_len |
	 *          | auth_len+msg_len-2|      auth      |         msg                 |
	 */
	uint8_t len = 0xBA;//(auth_len+msg_len) << 1;
	//uart_write("\n::");
	//uart_write_hex(len);
	//cc2420_ram_write_lsb(CC2420RAM_TXFIFO, &len, 1);
	//cc2420_ram_write_lsb(CC2420RAM_TXFIFO+1, auth, auth_len);
	//cc2420_ram_write_lsb(CC2420RAM_TXFIFO+auth_len+1, msg, msg_len);
	cc2420_ram_write_lsb(CC2420RAM_SABUF, msg, msg_len);

	uint8_t cipher[auth_len+msg_len];

	//cc2420_ram_read_lsb(CC2420RAM_TXFIFO+1, cipher, sizeof(cipher));

	//uart_write_n(cipher, sizeof(cipher));
	//uart_write("\n---\n");

	//cc2420_strobe(CC2420_STXENC);
	cc2420_strobe(CC2420_SAES);

	P1DIR &= ~(BV(2) | BV(3));

	while((cc2420_status() & BV(CC2420_ENC_BUSY)))
	{
		__delay_cycles(0.25*SYSFREQ);
	}

	//cc2420_ram_read_lsb(CC2420RAM_TXFIFO+1, cipher, sizeof(cipher));
	cc2420_ram_read_lsb(CC2420RAM_SABUF, cipher, sizeof(cipher));

	for(uint8_t i = 0; i < sizeof(cipher); i++)
		uart_write_hex(cipher[i]);
	uart_write("\n###\n");
}

int main(void)
{
	WDTCTL = (WDTPW | WDTHOLD); // Stop WDT

	/* Set clk speed */
	DCOCTL = 0x00;
	/* Turn XT2 off */
	BCSCTL1 = BCS1_8MHZ;
	BCSCTL2 = BCS2_8MHZ;
	BCSCTL3 = BCS3_8MHZ;
	DCOCTL = DCO_8MHZ;

	/* green led */
	P5DIR |= BV(6);
	P5OUT |= BV(6);
 
	/* init */
	uart_init();
	spi_init();
	cc2420_init();


	uint8_t j = 0;
	char s[2] = {'\n', '\0'};
	uint16_t ret;

	while(1) {
		P5OUT ^= BV(6);
		__delay_cycles(0.5*SYSFREQ);
		//uart_write_hex(cc2420_status());
		//uart_write(s);

		__delay_cycles(0.5*SYSFREQ);

		encrypt_test(t_key, t_msg, sizeof(t_msg), t_ath, sizeof(t_ath));

		/*ret = cc2420_reg_read(0x1E);
		uart_write_hex(ret >> 8);
		uart_write_hex(ret);
		uart_write(s);
		ret = cc2420_reg_read(0x1F);
		uart_write_hex(ret >> 8);
		uart_write_hex(ret);
		uart_write(s);
		*/

		//__delay_cycles(0.0005*SYSFREQ);
		//ret = cc2420_status();
		//uart_write_hex(ret);
		//uart_write(s);
	}
	return 0;
}

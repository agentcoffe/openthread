#ifndef CC2420_H
#define CC2420_H

#define SWRITE		0x00
#define SREAD		0x40

#define SREG		0x00
#define SRAM		0x80

/*
 * All constants are from the Chipcon CC2420 Data Sheet that at one
 * point in time could be found at
 * http://www.chipcon.com/files/CC2420_Data_Sheet_1_4.pdf
 *
 * The page numbers below refer to pages in this document.
 *
 *  Created on: 11/02/2011
 *      Author: Lior
 */

/* Page 27. */
enum cc2420_status_byte {
	CC2420_XOSC16M_STABLE 				= 6,
	CC2420_TX_UNDERFLOW					= 5,
	CC2420_ENC_BUSY						= 4,
	CC2420_TX_ACTIVE					= 3,
	CC2420_LOCK							= 2,
	CC2420_RSSI_VALID					= 1,
};

/* Page 27. */
enum cc2420_memory_size {
	CC2420_RAM_SIZE						= 368,
	CC2420_FIFO_SIZE					= 128,
};

/* Page 29. */
enum cc2420_address {
	CC2420RAM_TXFIFO					= 0x000,
	CC2420RAM_RXFIFO					= 0x080,
	CC2420RAM_KEY0						= 0x100,
	CC2420RAM_RXNONCE					= 0x110,
	CC2420RAM_SABUF						= 0x120,
	CC2420RAM_KEY1						= 0x130,
	CC2420RAM_TXNONCE					= 0x140,
	CC2420RAM_CBCSTATE					= 0x150,
	CC2420RAM_IEEEADDR					= 0x160,
	CC2420RAM_PANID						= 0x168,
	CC2420RAM_SHORTADDR					= 0x16A,
};

/* Page 60. */
enum cc2420_strobe {
	CC2420_SNOP							= 0x00,
	CC2420_SXOSCON						= 0x01,
	CC2420_STXCAL						= 0x02,
	CC2420_SRXON						= 0x03,
	CC2420_STXON						= 0x04,
	CC2420_STXONCCA						= 0x05,
	CC2420_SRFOFF						= 0x06,
	CC2420_SXOSCOFF						= 0x07,
	CC2420_SFLUSHRX						= 0x08,
	CC2420_SFLUSHTX						= 0x09,
	CC2420_SACK							= 0x0A,
	CC2420_SACKPEND						= 0x0B,
	CC2420_SRXDEC						= 0x0C,
	CC2420_STXENC						= 0x0D,
	CC2420_SAES							= 0x0E,
};

/* Page 60. */
enum cc2420_register {
	/* 0x0F not used */
	CC2420_MAIN							= 0x10,
	CC2420_MDMCTRL0						= 0x11,
	CC2420_MDMCTRL1						= 0x12,
	CC2420_RSSI							= 0x13,
	CC2420_SYNCWORD						= 0x14,
	CC2420_TXCTRL						= 0x15,
	CC2420_RXCTRL0						= 0x16,
	CC2420_RXCTRL1						= 0x17,
	CC2420_FSCTRL						= 0x18,
	CC2420_SECCTRL0						= 0x19,
	CC2420_SECCTRL1						= 0x1A,
	CC2420_BATTMON						= 0x1B,
	CC2420_IOCFG0						= 0x1C,
	CC2420_IOCFG1						= 0x1D,
	CC2420_MANFIDL						= 0x1E,
	CC2420_MANFIDH						= 0x1F,
	CC2420_FSMTC						= 0x20,
	CC2420_MANAND						= 0x21,
	CC2420_MANOR						= 0x22,
	CC2420_AGCCTRL						= 0x23,
	CC2420_AGCTST0						= 0x24,
	CC2420_AGCTST1						= 0x25,
	CC2420_AGCTST2						= 0x26,
	CC2420_FSTST0						= 0x27,
	CC2420_FSTST1						= 0x28,
	CC2420_FSTST2						= 0x29,
	CC2420_FSTST3						= 0x2A,
	CC2420_RXBPFTST						= 0x2B,
	CC2420_FSMSTATE						= 0x2C,
	CC2420_ADCTST						= 0x2D,
	CC2420_DACTST						= 0x2E,
	CC2420_TOPTST						= 0x2F,
	CC2420_RESERVED						= 0x30,
	/* 0x31 - 0x3D not used */
	CC2420_TXFIFO						= 0x3E,
	CC2420_RXFIFO						= 0x3F,
};

/* Page 69. */
enum cc2420_secctrl0 {
	CC2420_SECCTRL0_NO_SECURITY			= 0x0000,
	CC2420_SECCTRL0_CBC_MAC				= 0x0001,
	CC2420_SECCTRL0_CTR					= 0x0002,
	CC2420_SECCTRL0_CCM					= 0x0003,

	CC2420_SECCTRL0_SEC_M_IDX			= 2,

	CC2420_SECCTRL0_RXKEYSEL0			= 0x0000,
	CC2420_SECCTRL0_RXKEYSEL1			= 0x0020,

	CC2420_SECCTRL0_TXKEYSEL0			= 0x0000,
	CC2420_SECCTRL0_TXKEYSEL1			= 0x0040,

	CC2420_SECCTRL0_SAKEYSEL0			= 0x0000,
	CC2420_SECCTRL0_SAKEYSEL1			= 0x0080,

	CC2420_SECCTRL0_SEC_CBC_HEAD		= 0x0100,
	CC2420_SECCTRL0_RXFIFO_PROTECTION	= 0x0200,
};

uint8_t cc2420_status(void);
void cc2420_strobe(enum cc2420_strobe);
uint16_t cc2420_reg_read(enum cc2420_register);
void cc2420_reg_write(enum cc2420_register, uint16_t);
void cc2420_ram_read_msb(enum cc2420_address, uint8_t*, uint16_t);
void cc2420_ram_write_msb(enum cc2420_address, uint8_t*, uint16_t);
void cc2420_ram_read_lsb(enum cc2420_address, uint8_t*, uint16_t);
void cc2420_ram_write_lsb(enum cc2420_address, uint8_t*, uint16_t);

void cc2420_init(void);

#endif /* CC2420_H */

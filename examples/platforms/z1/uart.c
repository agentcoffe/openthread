/**
 * @file
 * @brief
 *   This file includes the platform abstraction for UART communication.
 */


#include <stdint.h>
#include <stddef.h>

#include <openthread/types.h>
#include <openthread/platform/uart.h>

#include "platform-z1.h"
#include "utils/code_utils.h"

/* These values are not in enums because the MSP430 runs a
16 bit architecture, and a 16 bit int is not big enough to
hold some of these values. In c++11 the size of an enum can
be specified, but not in c.*/
#define kPlatformClock          32000000
#define kBaudRate               115200
#define kReceiveBufferSize      128

void processReceive(void);
void processTransmit(void);

static const uint8_t *txBuffer = NULL;
static uint16_t txBufferLength = 0;

static uint8_t txDone = 0;

/**
 *     +------+ +------+ +------+ +------+
 *     | xxxx | | char | | char | | xxxx |
 * ... +------+ +------+ +------+ +------+ ...
 *                 ^                 ^
 *               mHead             mTail
 */

typedef struct rxRingBuffer
{
    // The data buffer
    uint8_t mData[kReceiveBufferSize];
    // The index of the next free location
    uint16_t mHead;
    // The index of the last written item
    uint16_t mTail;
} rxRingBuffer_t;

static rxRingBuffer_t rxBuffer;

/**
 * @addtogroup plat-uart
 *
 * @brief
 *   This module includes the platform abstraction for UART communication.
 *
 * @{
 *
 */

/**
 * Enable the UART.
 *
 * @retval OT_ERROR_NONE    Successfully enabled the UART.
 * @retval OT_ERROR_FAILED  Failed to enabled the UART.
 *
 */
otError otPlatUartEnable(void)
{
    otError error = OT_ERROR_NONE;

    /* Enable rx/tx pins */
    P3SEL |= 0x30;
    /* 8N1 */
    UCA0CTL0 = 0x00;
    /* Select clock source */
    UCA0CTL1 |= UCSSEL_2;
    /* Select 115200 Baud under 8MHZ */
    UCA0BR0 = 0x45;
    UCA0BR1 = 0x00;
    /* Set the BITCLK modulation */
    UCA0MCTL = UCBRS2;
    /* Disable software reset */
    UCA0CTL1 &= ~UCSWRST;
    /* Enable rx interrupt */
    IE2 |= UCA0RXIE;

    UCA0TXBUF = 0xF5;

    return error;
}

/**
 * Disable the UART.
 *
 * @retval OT_ERROR_NONE    Successfully disabled the UART.
 * @retval OT_ERROR_FAILED  Failed to disable the UART.
 *
 */
otError otPlatUartDisable(void)
{
    otError error = OT_ERROR_NONE;

    return error;
}

/**
 * Send bytes over the UART.
 *
 * @param[in] aBuf        A pointer to the data buffer.
 * @param[in] aBufLength  Number of bytes to transmit.
 *
 * @retval OT_ERROR_NONE    Successfully started transmission.
 * @retval OT_ERROR_FAILED  Failed to start the transmission.
 *
 */
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    otError error = OT_ERROR_NONE;

    otEXPECT_ACTION(txBuffer == NULL, error = OT_ERROR_BUSY);

    txBuffer = aBuf;
    txBufferLength = aBufLength;

    IE2 |= UCA0TXIE;

exit:
    return error;
}

/**
 * The UART driver calls this method to notify OpenThread that the requested bytes have been sent.
 *
 * extern void otPlatUartSendDone(void);
 *
 */

void processTransmit(void)
{
    if (txDone == 1)
    {
        txDone = 0;
        otPlatUartSendDone();
    }
}

__interrupt_vec(USCIAB0TX_VECTOR)
void tx_finished_isr(void)
{
    otEXPECT(txBuffer != NULL);

    if (txBufferLength > 0)
    {
        UCA0TXBUF = *txBuffer;
        txBuffer++;
        txBufferLength--;
    }

    if (txBufferLength == 0)
    {
        /* Transmit finished, set flag and turn isr off */
        txDone = 1;
        IE2 &= ~UCA0TXIE;
    }

exit:
    return;
}

/**
 * The UART driver calls this method to notify OpenThread that bytes have been received.
 *
 * @param[in]  aBuf        A pointer to the received bytes.
 * @param[in]  aBufLength  The number of bytes received.
 *
 * extern void otPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength);
 *
 */

void processReceive(void)
{
    // Copy tail to prevent multiple reads
    uint16_t tail = rxBuffer.mTail;

    // If the data wraps around, process the first part
    if (rxBuffer.mHead > tail)
    {
        otPlatUartReceived(&(rxBuffer.mData[rxBuffer.mHead]), kReceiveBufferSize - rxBuffer.mHead);
        // Reset the buffer mHead back to zero.
        rxBuffer.mHead = 0;
    }

    // For any data remaining, process it
    if (rxBuffer.mHead != tail)
    {
        otPlatUartReceived(&(rxBuffer.mData[rxBuffer.mHead]), tail - rxBuffer.mHead);
        // Set mHead to the local tail we have cached
        rxBuffer.mHead = tail;
    }
}

__interrupt_vec(USCIAB0RX_VECTOR)
void rx_finished_isr(void)
{
    uint8_t byte = UCA0RXBUF;
    // We can only write if incrementing mTail doesn't equal mHead
    if (rxBuffer.mHead != (rxBuffer.mTail + 1) % kReceiveBufferSize)
    {
        rxBuffer.mData[rxBuffer.mTail] = byte;
        rxBuffer.mTail = (rxBuffer.mTail + 1) % kReceiveBufferSize;
    }
 
    return;
}

void z1UartProcess(void)
{
    processTransmit();
    processReceive();
}

/**
 * @}
 *
 */

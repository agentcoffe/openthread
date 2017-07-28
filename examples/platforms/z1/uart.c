/**
 * @file
 * @brief
 *   This file includes the platform abstraction for UART communication.
 */


#include <stdint.h>

#include <openthread/types.h>

#include <openthread/platform/uart.h>

#include "common/code_utils.hpp"


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

exit:
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

exit:
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

exit:
    return error;
}

/**
 * The UART driver calls this method to notify OpenThread that the requested bytes have been sent.
 *
 * extern void otPlatUartSendDone(void);
 *
 */

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
{}

void processTransmit(void)
{}

void z1UartProcess(void)
{}

/**
 * @}
 *
 */

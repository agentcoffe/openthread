
/**
 * @file
 * @brief
 *   This file includes platform abstractions for miscellaneous behaviors.
 */


#include <stdint.h>

#include <openthread/types.h>


/**
 * @addtogroup plat-misc
 *
 * @brief
 *   This module includes platform abstractions for miscellaneous behaviors.
 *
 * @{
 *
 */

/**
 * This function performs a software reset on the platform, if supported.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 */
void otPlatReset(otInstance *aInstance)
{}

/**
 * Enumeration of possible reset reason codes.
 *
 * These are in the same order as the Spinel reset reason codes.
 *
 * typedef enum
 * {
 *     OT_PLAT_RESET_REASON_POWER_ON       = 0,
 *     OT_PLAT_RESET_REASON_EXTERNAL       = 1,
 *     OT_PLAT_RESET_REASON_SOFTWARE       = 2,
 *     OT_PLAT_RESET_REASON_FAULT          = 3,
 *     OT_PLAT_RESET_REASON_CRASH          = 4,
 *     OT_PLAT_RESET_REASON_ASSERT         = 5,
 *     OT_PLAT_RESET_REASON_OTHER          = 6,
 *     OT_PLAT_RESET_REASON_UNKNOWN        = 7,
 *     OT_PLAT_RESET_REASON_WATCHDOG       = 8,
 * 
 *     OT_PLAT_RESET_REASON_COUNT,
 * } otPlatResetReason;
 * 
 */

/**
 * This function returns the reason for the last platform reset.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 */
otPlatResetReason otPlatGetResetReason(otInstance *aInstance)
{
    return OT_PLAT_RESET_REASON_EXTERNAL;
}

/**
 * This function provides a platform specific implementation for assert.
 *
 * @param[in] aFilename    The name of the file where the assert occurred.
 * @param[in] aLineNumber  The line number in the file where the assert occurred.
 *
 */
void otPlatAssertFail(const char *aFilename, int aLineNumber)
{}

/**
 * This function performs a platform specific operation to wake the host MCU.
 * This is used only for NCP configurations.
 *
 */
void otPlatWakeHost(void)
{}

/**
 * @}
 *
 */

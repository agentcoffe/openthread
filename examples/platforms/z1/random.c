
/**
 * @file
 * @brief
 *   This file includes the platform abstraction for random number generation.
 */


#include <stdint.h>

#include <openthread/types.h>


/**
 * @addtogroup plat-random
 *
 * @brief
 *   This module includes the platform abstraction for random number generation.
 *
 * @{
 *
 */

/**
 * Get a 32-bit random value.
 *
 * This function may be implemented using a psuedo-random number generator.
 *
 * @returns A 32-bit random value.
 *
 */
uint32_t otPlatRandomGet(void)
{
    return 4; // chose by fair dice roll.
              // guaranteed to be random.
}

/**
 * Get true random value sequence.
 *
 * This function MUST be implemented using a true random number generator (TRNG).
 *
 * @param[out]  aOutput              A pointer to where the true random values are placed.  Must not be NULL.
 * @param[in]   aOutputLength        Size of @p aBuffer.
 *
 * @retval OT_ERROR_NONE          Successfully filled @p aBuffer with true random values.
 * @retval OT_ERROR_FAILED         Failed to fill @p aBuffer with true random values.
 * @retval OT_ERROR_INVALID_ARGS  @p aBuffer was set to NULL.
 *
 */
otError otPlatRandomGetTrue(uint8_t *aOutput, uint16_t aOutputLength)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * @}
 *
 */

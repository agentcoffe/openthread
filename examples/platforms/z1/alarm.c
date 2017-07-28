
/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the millisecond alarm service.
 */


#include <stdint.h>

#include <openthread/types.h>


/**
 * @addtogroup plat-alarm
 *
 * @brief
 *   This module includes the platform abstraction for the alarm service.
 *
 * @{
 *
 */

/**
 * Set the alarm to fire at @p aDt milliseconds after @p aT0.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aT0        The reference time.
 * @param[in] aDt        The time delay in milliseconds from @p aT0.
 */
void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
}

/**
 * Stop the alarm.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 */
void otPlatAlarmMilliStop(otInstance *aInstance)
{
}

/**
 * Get the current time.
 *
 * @returns The current time in milliseconds.
 */
uint32_t otPlatAlarmMilliGetNow(void)
{
    return 0;
}

/**
 * Signal that the alarm has fired.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * extern void otPlatAlarmMilliFired(otInstance *aInstance);
 *
 */

/**
 * Signal diagnostics module that the alarm has fired.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * extern void otPlatDiagAlarmFired(otInstance *aInstance);
 *
 */


/**
 * @}
 *
 */

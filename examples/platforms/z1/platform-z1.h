#ifndef PLATFORM_Z1_H_
#define PLATFORM_Z1_H_

#include <stdint.h>

#include <openthread/types.h>

#include "msp430f2617-reg.h"

// Global OpenThread instance structure
extern otInstance *sInstance;

/**
 * This function initializes the alarm service used by OpenThread.
 *
 */
void z1AlarmInit(void);

/**
 * This function performs alarm driver processing.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void z1AlarmProcess(otInstance *aInstance);

/**
 * This function initializes the radio service used by OpenThread.
 *
 */
void z1RadioInit(void);

/**
 * This function performs radio driver processing.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void z1RadioProcess(otInstance *aInstance);

/**
 * This function initializes the random number service used by OpenThread.
 *
 */
void z1RandomInit(void);

/**
 * This function performs UART driver processing.
 *
 */
void z1UartProcess(void);


#endif // PLATFORM_Z1_H_

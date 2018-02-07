
/**
 * @file
 * @brief
 *   This file defines the flash interface used by settings.cpp.
 */


#include <stdint.h>

#include <openthread/types.h>


/**
 * Perform any initialization for flash driver.
 *
 * @retval ::OT_ERROR_NONE    Initialize flash driver success.
 * @retval ::OT_ERROR_FAILED  Initialize flash driver fail.
 */
otError utilsFlashInit(void)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Get the size of flash that can be read/write by the caller.
 * The usable flash size is always the multiple of flash page size.
 *
 * @returns The size of the flash.
 */
uint32_t utilsFlashGetSize(void)
{
    return 0;
}

/**
 * Erase one flash page that include the input address.
 * This is a non-blocking function. It can work with utilsFlashStatusWait to check when erase is done.
 *
 * The flash address starts from 0, and this function maps the input address to the physical address of flash for erasing.
 * 0 is always mapped to the beginning of one flash page.
 * The input address should never be mapped to the firmware space or any other protected flash space.
 *
 * @param[in]  aAddress  The start address of the flash to erase.
 *
 * @retval OT_ERROR_NONE           Erase flash operation is started.
 * @retval OT_ERROR_FAILED         Erase flash operation is not started.
 * @retval OT_ERROR_INVALID_ARGS    aAddress is out of range of flash or not aligend.
 */
otError utilsFlashErasePage(uint32_t aAddress)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
  * Check whether flash is ready or busy.
  *
  * @param[in]  aTimeout  The interval in milliseconds waiting for the flash operation to be done and become ready again.
  *                       zero indicates that it is a polling function, and returns current status of flash immediately.
  *                       non-zero indicates that it is blocking there until the operation is done and become ready, or timeout expires.
  *
  * @retval OT_ERROR_NONE           Flash is ready for any operation.
  * @retval OT_ERROR_BUSY           Flash is busy.
  */
otError utilsFlashStatusWait(uint32_t aTimeout)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Write flash. The write operation only clears bits, but never set bits.
 *
 * The flash address starts from 0, and this function maps the input address to the physical address of flash for writing.
 * 0 is always mapped to the beginning of one flash page.
 * The input address should never be mapped to the firmware space or any other protected flash space.
 *
 * @param[in]  aAddress  The start address of the flash to write.
 * @param[in]  aData     The pointer of the data to write.
 * @param[in]  aSize     The size of the data to write.
 *
 * @returns The actual size of octets write to flash.
 *          It is expected the same as aSize, and may be less than aSize.
 *          0 indicates that something wrong happens when writing.
 */
uint32_t utilsFlashWrite(uint32_t aAddress, uint8_t *aData, uint32_t aSize)
{
    return 0;
}

/**
 * Read flash.
 *
 * The flash address starts from 0, and this function maps the input address to the physical address of flash for reading.
 * 0 is always mapped to the beginning of one flash page.
 * The input address should never be mapped to the firmware space or any other protected flash space.
 *
 * @param[in]   aAddress  The start address of the flash to read.
 * @param[Out]  aData     The pointer of buffer for reading.
 * @param[in]   aSize     The size of the data to read.
 *
 * @returns The actual size of octets read to buffer.
 *          It is expected the same as aSize, and may be less than aSize.
 *          0 indicates that something wrong happens when reading.
 */
uint32_t utilsFlashRead(uint32_t aAddress, uint8_t *aData, uint32_t aSize)
{
    return 0;
}


/**
 * @file
 * @brief
 *   This file includes the platform abstraction for dynamic memory allocation.
 */



/**
 * @addtogroup plat-memory
 *
 * @brief
 *   This module includes the platform abstraction for dynamic memory allocation.
 *
 * @{
 *
 */

/*
 * OpenThread only requires dynamic memory allocation when supporting multiple simultaneous instances, for MbedTls.
 */

/**
 * Dynamically allocates new memory. On platforms that support it, should just redirect to calloc. For
 * those that don't support calloc, should support the same functionality:
 *
 *   "The calloc() function contiguously allocates enough space for count objects that are size bytes of
 *   memory each and returns a pointer to the allocated memory. The allocated memory is filled with bytes
 *   of value zero."
 *
 * This function is available and can ONLY be used only when support for multiple OpenThread instances is enabled.
 *
 * @param[in] aNum   The number of blocks to allocate
 * @param[in] aSize  The size of each block to allocate
 *
 * @retval void*  The pointer to the front of the memory allocated
 * @retval NULL   Failed to allocate the memory requested.
 */
void *otPlatCAlloc(size_t aNum, size_t aSize)
{}

/**
 * Frees memory that was dynamically allocated.
 *
 * This function is available and can ONLY be used only when support for multiple OpenThread instances is enabled.
 *
 * @param[in] aPtr  A pointer the memory blocks to free. The pointer may be NULL.
 */
void otPlatFree(void *aPtr)
{}

/**
 * @}
 *
 */

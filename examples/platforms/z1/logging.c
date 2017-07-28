
/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the debug log service.
 */


#include <stdint.h>


/**
 * @addtogroup plat-logging
 *
 * @brief
 *   This module includes the platform abstraction for the debug log service.
 *
 * @{
 *
 */

/**
 * Log levels.
 *
 * #define OT_LOG_LEVEL_NONE  0  ///< None
 * #define OT_LOG_LEVEL_CRIT  1  ///< Critical
 * #define OT_LOG_LEVEL_WARN  2  ///< Warning
 * #define OT_LOG_LEVEL_INFO  3  ///< Info
 * #define OT_LOG_LEVEL_DEBG  4  ///< Debug
 * 
 * typedef uint8_t otLogLevel;
 * 
 */

/**
 * This enum represents log regions.
 *
 * typedef enum otLogRegion
 * {
 *     OT_LOG_REGION_API      = 1,  ///< OpenThread API
 *     OT_LOG_REGION_MLE      = 2,  ///< MLE
 *     OT_LOG_REGION_ARP      = 3,  ///< EID-to-RLOC mapping.
 *     OT_LOG_REGION_NET_DATA = 4,  ///< Network Data
 *     OT_LOG_REGION_ICMP     = 5,  ///< ICMPv6
 *     OT_LOG_REGION_IP6      = 6,  ///< IPv6
 *     OT_LOG_REGION_MAC      = 7,  ///< IEEE 802.15.4 MAC
 *     OT_LOG_REGION_MEM      = 8,  ///< Memory
 *     OT_LOG_REGION_NCP      = 9,  ///< NCP
 *     OT_LOG_REGION_MESH_COP = 10, ///< Mesh Commissioning Protocol
 *     OT_LOG_REGION_NET_DIAG = 11, ///< Network Diagnostic
 *     OT_LOG_REGION_PLATFORM = 12, ///< Platform
 *     OT_LOG_REGION_COAP     = 13, ///< CoAP
 * } otLogRegion;
 * 
 */

/**
 * This function outputs logs.
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aLogRegion  The log region.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ...         Arguments for the format specification.
 *
 */
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
}

/**
 * @}
 *
 */

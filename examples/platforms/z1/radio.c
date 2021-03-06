
/**
 * @file
 * @brief
 *   This file defines the radio interface for OpenThread.
 *
 */


#include <openthread/config.h>
#include <openthread/openthread.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/diag.h>
#include <openthread/platform/platform.h>
#include <openthread/platform/radio.h>

#include "platform-z1.h"
#include "common/logging.hpp"
#include "utils/code_utils.h"

enum
{
    IEEE802154_MIN_LENGTH = 5,
    IEEE802154_MAX_LENGTH = 127,
    IEEE802154_ACK_LENGTH = 5,
    IEEE802154_FRAME_TYPE_MASK = 0x7,
    IEEE802154_FRAME_TYPE_ACK = 0x2,
    IEEE802154_FRAME_PENDING = 1 << 4,
    IEEE802154_ACK_REQUEST = 1 << 5,
    IEEE802154_DSN_OFFSET = 2,
};


/**
 * @addtogroup plat-radio
 *
 * @brief
 *   This module includes the platform abstraction for radio communication.
 *
 * @{
 *
 */

/**
 * @defgroup radio-types Types
 *
 * @brief
 *   This module includes the platform abstraction for a radio frame.
 *
 * @{
 *
 * enum
 * {
 *     OT_RADIO_FRAME_MAX_SIZE      = 127,                             ///< aMaxPHYPacketSize (IEEE 802.15.4-2006)
 *     OT_RADIO_CHANNEL_MIN         = 11,                              ///< 2.4 GHz IEEE 802.15.4-2006
 *     OT_RADIO_CHANNEL_MAX         = 26,                              ///< 2.4 GHz IEEE 802.15.4-2006
 *     OT_RADIO_SUPPORTED_CHANNELS  = 0xffff << OT_RADIO_CHANNEL_MIN,  ///< 2.4 GHz IEEE 802.15.4-2006
 *     OT_RADIO_SYMBOLS_PER_OCTET   = 2,                               ///< 2.4 GHz IEEE 802.15.4-2006
 *     OT_RADIO_BIT_RATE            = 250000,                          ///< 2.4 GHz IEEE 802.15.4 (kilobits per second)
 * 
 *     OT_RADIO_BITS_PER_OCTET  = 8,      ///< Number of bits per octet
 *     OT_RADIO_SYMBOL_TIME     = ((OT_RADIO_BITS_PER_OCTET / OT_RADIO_SYMBOLS_PER_OCTET) * 1000000) / OT_RADIO_BIT_RATE,
 * 
 *     OT_RADIO_LQI_NONE        = 0,      ///< LQI measurement not supported
 *     OT_RADIO_RSSI_INVALID    = 127,    ///< Invalid or unknown RSSI value
 * };
 *
 */

/**
 * This enum represents radio capabilities.
 *
 * typedef enum otRadioCaps
 * {
 *     OT_RADIO_CAPS_NONE              = 0,  ///< None
 *     OT_RADIO_CAPS_ACK_TIMEOUT       = 1,  ///< Radio supports AckTime event
 *     OT_RADIO_CAPS_ENERGY_SCAN       = 2,  ///< Radio supports Energy Scans
 *     OT_RADIO_CAPS_TRANSMIT_RETRIES  = 4,  ///< Radio supports transmission retry logic with collision avoidance (CSMA).
 *     OT_RADIO_CAPS_CSMA_BACKOFF      = 8,  ///< Radio supports CSMA backoff for frame transmission (but no retry).
 * } otRadioCaps;
 *
 */

/**
 * This structure represents an IEEE 802.15.4 radio frame.
 *
 * typedef struct otRadioFrame
 * {
 *     uint8_t  *mPsdu;            ///< The PSDU.
 *     uint8_t  mLength;           ///< Length of the PSDU.
 *     uint8_t  mChannel;          ///< Channel used to transmit/receive the frame.
 *     int8_t   mPower;            ///< Transmit/receive power in dBm.
 *     uint8_t  mLqi;              ///< Link Quality Indicator for received frames.
 *     uint8_t  mMaxTxAttempts;    ///< Max number of transmit attempts for an outbound frame.
 *     bool     mSecurityValid: 1; ///< Security Enabled flag is set and frame passes security checks.
 *     bool     mDidTX: 1;         ///< Set to true if this frame sent from the radio. Ignored by radio driver.
 *     bool     mIsARetx: 1;       ///< Set to true if this frame is a retransmission. Should be ignored by radio driver.
 * #if OPENTHREAD_ENABLE_RAW_LINK_API
 *     uint32_t mMsec;             ///< The timestamp when the frame was received (milliseconds).
 *     uint16_t mUsec;             ///< The timestamp when the frame was received (microseconds, the offset to mMsec).
 * #endif
 * } otRadioFrame;
 * 
 */

/**
 * This structure represents the state of a radio.
 * Initially, a radio is in the Disabled state.
 *
 * typedef enum otRadioState
 * {
 *     OT_RADIO_STATE_DISABLED = 0,
 *     OT_RADIO_STATE_SLEEP    = 1,
 *     OT_RADIO_STATE_RECEIVE  = 2,
 *     OT_RADIO_STATE_TRANSMIT = 3,
 * } otRadioState;
 *
 */

/**
 * The following are valid radio state transitions:
 *
 *                                    (Radio ON)
 *  +----------+  Enable()  +-------+  Receive() +---------+   Transmit()  +----------+
 *  |          |----------->|       |----------->|         |-------------->|          |
 *  | Disabled |            | Sleep |            | Receive |               | Transmit |
 *  |          |<-----------|       |<-----------|         |<--------------|          |
 *  +----------+  Disable() +-------+   Sleep()  +---------+   Receive()   +----------+
 *                                    (Radio OFF)                 or
 *                                                        signal TransmitDone
 */

/**
 * @}
 *
 */

/**
 * @defgroup radio-config Configuration
 *
 * @brief
 *   This module includes the platform abstraction for radio configuration.
 *
 * @{
 *
 */

/**
 * Get the factory-assigned IEEE EUI-64 for this interface.
 *
 * @param[in]  aInstance   The OpenThread instance structure.
 * @param[out] aIeeeEui64  A pointer to where the factory-assigned IEEE EUI-64 will be placed.
 *
 */
void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64)
{}

/**
 * Set the PAN ID for address filtering.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aPanId     The IEEE 802.15.4 PAN ID.
 *
 */
void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanId)
{}

/**
 * Set the Extended Address for address filtering.
 *
 * @param[in] aInstance         The OpenThread instance structure.
 * @param[in] aExtendedAddress  A pointer to the IEEE 802.15.4 Extended Address.
 *
 */
void otPlatRadioSetExtendedAddress(otInstance *aInstance, uint8_t *aExtendedAddress)
{}

/**
 * Set the Short Address for address filtering.
 *
 * @param[in] aInstance      The OpenThread instance structure.
 * @param[in] aShortAddress  The IEEE 802.15.4 Short Address.
 *
 */
void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress)
{}

/**
 * @}
 *
 */

/**
 * @defgroup radio-operation Operation
 *
 * @brief
 *   This module includes the platform abstraction for radio operations.
 *
 * @{
 *
 */

/**
 * Get current state of the radio.
 *
 * This function is not required by OpenThread. It may be used for debugging and/or application-specific purposes.
 *
 * @note This function may be not implemented. It does not affect OpenThread.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @return  Current state of the radio.
 */
otRadioState otPlatRadioGetState(otInstance *aInstance)
{
    return OT_RADIO_STATE_DISABLED;
}

/**
 * Enable the radio.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @retval OT_ERROR_NONE     Successfully enabled.
 * @retval OT_ERROR_FAILED   The radio could not be enabled.
 */
otError otPlatRadioEnable(otInstance *aInstance)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Disable the radio.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @retval OT_ERROR_NONE  Successfully transitioned to Disabled.
 */
otError otPlatRadioDisable(otInstance *aInstance)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Check whether radio is enabled or not.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @returns TRUE if the radio is enabled, FALSE otherwise.
 *
 */
bool otPlatRadioIsEnabled(otInstance *aInstance)
{
    return false;
}

/**
 * Transition the radio from Receive to Sleep.
 * Turn off the radio.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @retval OT_ERROR_NONE          Successfully transitioned to Sleep.
 * @retval OT_ERROR_BUSY          The radio was transmitting
 * @retval OT_ERROR_INVALID_STATE The radio was disabled
 */
otError otPlatRadioSleep(otInstance *aInstance)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Transitioning the radio from Sleep to Receive.
 * Turn on the radio.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 * @param[in]  aChannel   The channel to use for receiving.
 *
 * @retval OT_ERROR_NONE          Successfully transitioned to Receive.
 * @retval OT_ERROR_INVALID_STATE The radio was disabled or transmitting.
 */
otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Enable/Disable source address match feature.
 *
 * The source address match feature controls how the radio layer decides the "frame pending" bit for acks sent in
 * response to data request commands from children.
 *
 * If disabled, the radio layer must set the "frame pending" on all acks to data request commands.
 *
 * If enabled, the radio layer uses the source address match table to determine whether to set or clear the "frame
 * pending" bit in an ack to a data request command.
 *
 * The source address match table provides the list of children for which there is a pending frame. Either a short
 * address or an extended/long address can be added to the source address match table.
 *
 * @param[in]  aInstance   The OpenThread instance structure.
 * @param[in]  aEnable     Enable/disable source address match feature.
 */
void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable)
{}

/**
 * Add a short address to the source address match table.
 *
 * @param[in]  aInstance      The OpenThread instance structure.
 * @param[in]  aShortAddress  The short address to be added.
 *
 * @retval OT_ERROR_NONE      Successfully added short address to the source match table.
 * @retval OT_ERROR_NO_BUFS   No available entry in the source match table.
 */
otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Add an extended address to the source address match table.
 *
 * @param[in]  aInstance    The OpenThread instance structure.
 * @param[in]  aExtAddress  The extended address to be added.
 *
 * @retval OT_ERROR_NONE      Successfully added extended address to the source match table.
 * @retval OT_ERROR_NO_BUFS   No available entry in the source match table.
 */
otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const uint8_t *aExtAddress)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Remove a short address from the source address match table.
 *
 * @param[in]  aInstance      The OpenThread instance structure.
 * @param[in]  aShortAddress  The short address to be removed.
 *
 * @retval OT_ERROR_NONE        Successfully removed short address from the source match table.
 * @retval OT_ERROR_NO_ADDRESS  The short address is not in source address match table.
 */
otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Remove an extended address from the source address match table.
 *
 * @param[in]  aInstance    The OpenThread instance structure.
 * @param[in]  aExtAddress  The extended address to be removed.
 *
 * @retval OT_ERROR_NONE        Successfully removed the extended address from the source match table.
 * @retval OT_ERROR_NO_ADDRESS  The extended address is not in source address match table.
 */
otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const uint8_t *aExtAddress)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * Clear all short addresses from the source address match table.
 *
 * @param[in]  aInstance   The OpenThread instance structure.
 *
 */
void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance)
{}

/**
 * Clear all the extended/long addresses from source address match table.
 *
 * @param[in]  aInstance   The OpenThread instance structure.
 *
 */
void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance)
{}

/**
 * The radio driver calls this method to notify OpenThread of a received frame.
 *
 * @param[in]  aInstance The OpenThread instance structure.
 * @param[in]  aFrame    A pointer to the received frame or NULL if the receive operation failed.
 * @param[in]  aError    OT_ERROR_NONE when successfully received a frame, OT_ERROR_ABORT when reception
 *                       was aborted and a frame was not received, OT_ERROR_NO_BUFS when a frame could not be
 *                       received due to lack of rx buffer space.
 *
 * extern void otPlatRadioReceiveDone(otInstance *aInstance, otRadioFrame *aFrame, otError aError);
 *
 */


/**
 * The radio transitions from Transmit to Receive.
 * This method returns a pointer to the transmit buffer.
 *
 * The caller forms the IEEE 802.15.4 frame in this buffer then calls otPlatRadioTransmit() to request transmission.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @returns A pointer to the transmit buffer.
 *
 */

/* NOTE: I am not sure where we need this, but it cannot be in a function scope. So probably global. Double check with other implementations. */
otRadioFrame otRadioTXBuff;

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    return &otRadioTXBuff;
}

/**
 * This method begins the transmit sequence on the radio.
 *
 * The caller must form the IEEE 802.15.4 frame in the buffer provided by otPlatRadioGetTransmitBuffer() before
 * requesting transmission.  The channel and transmit power are also included in the otRadioFrame structure.
 *
 * The transmit sequence consists of:
 * 1. Transitioning the radio to Transmit from Receive.
 * 2. Transmits the psdu on the given channel and at the given transmit power.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aFrame     A pointer to the frame that will be transmitted.
 *
 * @retval OT_ERROR_NONE          Successfully transitioned to Transmit.
 * @retval OT_ERROR_INVALID_STATE The radio was not in the Receive state.
 */
otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * The radio driver calls this method to notify OpenThread that the transmission has started.
 *
 * @note  This function should be called by the same thread that executes all of the other OpenThread code. It should
 *        not be called by ISR or any other task.
 *
 * @param[in]  aInstance  A pointer to the OpenThread instance structure.
 * @param[in]  aFrame     A pointer to the frame that is being transmitted.
 *
 *
 * extern void otPlatRadioTxStarted(otInstance *aInstance, otRadioFrame *aFrame);
 *
 */


/**
 * The radio driver calls this method to notify OpenThread that the transmission has completed,
 * this callback pass up the ACK frame, new add platforms should use this callback function.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 * @param[in]  aFrame     A pointer to the frame that was transmitted.
 * @param[in]  aAckFrame  A pointer to the ACK frame, NULL if no ACK was received.
 * @param[in]  aError     OT_ERROR_NONE when the frame was transmitted, OT_ERROR_NO_ACK when the frame was
 *                        transmitted but no ACK was received, OT_ERROR_CHANNEL_ACCESS_FAILURE when the transmission
 *                        could not take place due to activity on the channel, OT_ERROR_ABORT when transmission was
 *                        aborted for other reasons.
 *
 * extern void otPlatRadioTxDone(otInstance *aInstance, otRadioFrame *aFrame, otRadioFrame *aAckFrame,
 *                              otError aError);
 *
 */


/**
 * The radio driver calls this method to notify OpenThread that the transmission has completed,
 * this function is going to be deprecated, new add platfroms should not use this callback function.
 *
 * @param[in]  aInstance      The OpenThread instance structure.
 * @param[in]  aFrame         A pointer to the frame that was transmitted.
 * @param[in]  aFramePending  TRUE if an ACK frame was received and the Frame Pending bit was set.
 * @param[in]  aError         OT_ERROR_NONE when the frame was transmitted, OT_ERROR_NO_ACK when the frame was
 *                            transmitted but no ACK was received, OT_ERROR_CHANNEL_ACCESS_FAILURE when the transmission
 *                            could not take place due to activity on the channel, OT_ERROR_ABORT when transmission was
 *                            aborted for other reasons.
 *
 * extern void otPlatRadioTransmitDone(otInstance *aInstance, otRadioFrame *aFrame, bool aFramePending,
 *                                    otError aError);
 *
 */


/**
 * Get the most recent RSSI measurement.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @returns The RSSI in dBm when it is valid.  127 when RSSI is invalid.
 */
int8_t otPlatRadioGetRssi(otInstance *aInstance)
{
    return 0;
}

/**
 * Get the radio capabilities.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @returns The radio capability bit vector. The stack enables or disables some functions based on this value.
 */
otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    return OT_RADIO_CAPS_NONE;
}

/**
 * Set the radio Tx power used for auto-generated frames.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aPower     The Tx power to use in dBm.
 *
 */
void otPlatRadioSetDefaultTxPower(otInstance *aInstance, int8_t aPower)
{}

/**
 * Get the status of promiscuous mode.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @retval true   Promiscuous mode is enabled.
 * @retval false  Promiscuous mode is disabled.
 */
bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
    return false;
}

/**
 * Enable or disable promiscuous mode.
 *
 * @param[in]  aInstance The OpenThread instance structure.
 * @param[in]  aEnable   A value to enable or disable promiscuous mode.
 */
void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable)
{}

/**
 * The radio driver calls this method to notify OpenThread diagnostics module that the transmission has completed.
 *
 * @param[in]  aInstance      The OpenThread instance structure.
 * @param[in]  aFrame         A pointer to the frame that was transmitted.
 * @param[in]  aError         OT_ERROR_NONE when the frame was transmitted, OT_ERROR_CHANNEL_ACCESS_FAILURE when the
 *                            transmission could not take place due to activity on the channel, OT_ERROR_ABORT when
 *                            transmission was aborted for other reasons.
 *
 * extern void otPlatDiagRadioTransmitDone(otInstance *aInstance, otRadioFrame *aFrame, otError aError);
 *
 */


/**
 * The radio driver calls this method to notify OpenThread diagnostics module of a received frame.
 *
 * @param[in]  aInstance The OpenThread instance structure.
 * @param[in]  aFrame    A pointer to the received frame or NULL if the receive operation failed.
 * @param[in]  aError    OT_ERROR_NONE when successfully received a frame, OT_ERROR_ABORT when reception
 *                       was aborted and a frame was not received, OT_ERROR_NO_BUFS when a frame could not be
 *                       received due to lack of rx buffer space.
 *
 * extern void otPlatDiagRadioReceiveDone(otInstance *aInstance, otRadioFrame *aFrame, otError aError);
 *
 */


/**
 * This method begins the energy scan sequence on the radio.
 *
 * @param[in] aInstance      The OpenThread instance structure.
 * @param[in] aScanChannel   The channel to perform the energy scan on.
 * @param[in] aScanDuration  The duration, in milliseconds, for the channel to be scanned.
 *
 * @retval OT_ERROR_NONE             Successfully started scanning the channel.
 * @retval OT_ERROR_NOT_IMPLEMENTED  The radio doesn't support energy scanning.
 */
otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration)
{
    otError error = OT_ERROR_NONE;

exit:
    return error;
}

/**
 * The radio driver calls this method to notify OpenThread that the energy scan is complete.
 *
 * @param[in]  aInstance           The OpenThread instance structure.
 * @param[in]  aEnergyScanMaxRssi  The maximum RSSI encountered on the scanned channel.
 *
 * extern void otPlatRadioEnergyScanDone(otInstance *aInstance, int8_t aEnergyScanMaxRssi);
 *
 */


/**
 * Get the radio receive sensitivity value.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 *
 * @returns The radio receive sensitivity value in dBm.
 */
int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance)
{
    return 0;
}

/**
 * @}
 *
 */

/**
 * @}
 *
 */

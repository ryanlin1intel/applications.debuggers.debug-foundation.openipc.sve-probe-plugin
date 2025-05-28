/////////////////////////<Source Code Embedded Notices>/////////////////////////
//
// INTEL CONFIDENTIAL
// Copyright (C) Intel Corporation All Rights Reserved.
//
// The source code contained or described herein and all documents related to
// the source code ("Material") are owned by Intel Corporation or its suppliers
// or licensors. Title to the Material remains with Intel Corporation or its
// suppliers and licensors. The Material contains trade secrets and proprietary
// and confidential information of Intel or its suppliers and licensors. The
// Material is protected by worldwide copyright and trade secret laws and
// treaty provisions. No part of the Material may be used, copied, reproduced,
// modified, published, uploaded, posted, transmitted, distributed, or disclosed
// in any way without Intel's prior express written permission.
//
// No license under any patent, copyright, trade secret or other intellectual
// property right is granted to or conferred upon you by disclosure or delivery
// of the Materials, either expressly, by implication, inducement, estoppel or
// otherwise. Any license under such intellectual property rights must be
// express and approved by Intel in writing.
//
/////////////////////////<Source Code Embedded Notices>/////////////////////////
///  @file
///
///  @brief Contains types used throughout the PROBE and its interfaces
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include <stddef.h>

#if __has_include("ErrorCodes.h")
#include <ErrorCodes.h>
#else
#include <Foundation/Error/ErrorCodes.h>
#endif

typedef uint32_t OpenIPC_DeviceId;

///
/// @brief A value representing an invalid device ID.
#define OpenIPC_INVALID_DEVICE_ID           ((OpenIPC_DeviceId)(-1))

//! Consistent boolean size.
typedef uint8_t PPI_bool;
//! Consistent character.
typedef char PPI_char;
//! Consistent reference id.
typedef uint32_t PPI_RefId;

//! Here so that we have a consistent enum size
#define DECLARE_ENUM(_NAME_) typedef uint32_t _NAME_; enum

//! Interface type definition
DECLARE_ENUM(PPI_EInterfaceType)
{
    PPI_interfaceTypePins = 0,
    PPI_interfaceTypeJtag         = 1,
    PPI_interfaceTypeI2c          = 2,
    PPI_interfaceTypeXdpObs       = 3,
    PPI_interfaceTypeDciDvCGP     = 4,  // BSSB (not used but exists on USB descriptors)
    PPI_interfaceTypeDciDvCDfx    = 5,  // BSSB
    PPI_interfaceTypeDciDvCTrace  = 6,  // BSSB
    PPI_interfaceTypeDciDbCGP     = 7,  // direct USB
    PPI_interfaceTypeDciDbCDfx    = 8,  // direct USB
    PPI_interfaceTypeDciDbCTrace  = 9,  // direct USB
    PPI_interfaceTypeMipiPtiTrace = 10, //
    PPI_interfaceTypeDciRaw       = 11, // This takes direct control of the Dfx endpoint, blocking all other traffic
    PPI_interfaceTypeDciPackets   = 12, // This shares the Dfx endpoint with peers like JTAG and Pins.
    PPI_interfaceTypeDciUsbKmd    = 13, // direct USB - kernel mode debug interface (aka GP1)
    PPI_interfaceTypeHtiTrace     = 14, // High-Speed Trace Interface
    PPI_interfaceTypeStatePort    = 15, // Generic interface for addressable fixed size access.
    PPI_interfaceTypeMemory       = 16, // Generic interface for addressable dynamic size access.
    PPI_interfaceTypeSneakPeek    = 17, // Interface for sending/receiving SneakPeek packets
    PPI_interfaceTypePort         = 18, // Interface for sending/receiving arbitrary blobs of bytes.
    PPI_interfaceMAX
};

//! Interface human-readable name
static const PPI_char* const c_interfaceTypeString[PPI_interfaceMAX] = {
    "Pins",
    "Jtag",
    "I2C",
    "XDP_OBS",
    "DCI_OOB_DMA",
    "DCI_OOB_DFX",
    "DCI_OOB_TRACE",
    "DCI_USB_DMA",
    "DCI_USB_DFX",
    "DCI_USB_TRACE",
    "MIPI_PTI",
    "DCI_RAW",
    "DCI_PACKETS",
    "DCI_USB_KMD",
    "HTI",
    "StatePort",
    "Memory",
    "SneakPeek",
    "Port"
};

//! enumeration value of probe plugin event type: these events are generally logical
DECLARE_ENUM(PPI_PluginEvent)
{
    // probe events
    PPI_probeAdded    = 0x100, // A probe was added, value is the probe ref id
    PPI_probeRemoved  = 0x101, // A probe was removed, value is reserved, device id is either 0, for no mapping, or the OpenIPC DEvice id of the probe ow
    PPI_probePower    = 0x102, // A probe power event was detect, 1 is power on, 0 is power off
    PPI_probeReset    = 0x103, // Probe was reset, value is reserved
    PPI_probeError    = 0x104, // Probe had an error, value is the OpenIPC_Error value
    PPI_probeReconfig = 0x105, // The probe has undergone a reconfiguration.
    // generic interface events
    PPI_interfaceArbitration = 0x200, // Interface might have multiple clients talking to it;
    PPI_interfaceError = 0x201,       // Interface saw an error, value is the OpenIPC_Error value
    // debug port events (re: XDPA)
    PPI_targetPower     = 0x300,       // Interface saw a target power event (1 is target is turned on, 0 is target is turned off)
    PPI_targetReset     = 0x301,       // Interface saw a target reset; value is 1 when reset occured, 0 otherwise (e.g., it's logical and not physical).
    PPI_targetPrdy      = 0x302,       // Interface saw a PRDy, value is 1 if PRDY went high, 0 is PRDY went low (e.g., this is a logical value)
    PPI_targetSyncIn    = 0x303,       //
    PPI_targetBclk_xdpa = 0x304,       // Interface saw a bclk change (legacy XDP port)
    PPI_targetRclk_xdpa = 0x305,       // Interface saw a rclk change (legacy XDP port)
    PPI_targetLaIn_xdpa = 0x306,       // Interface saw a LaIn change (legacy XDP port)
    PPI_targetSleepStall_xdpa = 0x307, // Interface saw a sleepStall change (legacy XDP port)
    PPI_targetStatus = 0x308,          // Interface saw Debug Event (BPK bpk_fsta_trigev)
    PPI_targetPmodePacket = 0x400,     // Interface saw a pmode packet, value is the pmode packet value.		// note: leave payload undecoded, since encoding is Si specific, and breaking into separate events and sending them up discretely would imply false time/order
    //PPI_targetPchCsmeReset
    //PPI_targetPchIshReset
    //PPI_targetPchIeReset
    //PPI_targetC10
    // DCI events
    PPI_targetDciPacket = 0x401, // Interface saw a DCI packet, value is the packet value; this is not the full DCI packet, just the status payload that cannot be decoded in a more abstract fashion.		// note: leave payload undecoded, since encoding is Si specific, and breaking into separate events and sending them up discretely would imply false time/order
    // Next are the trigger response occurence events (support depends on the probe type)
    PPI_triggerResponse = 0x500, // The payload is: [32] 0 if the trigger-response triggered, 1 if it un-triggered [31:0] the identifier associated with a trigger-response
    PPI_targetCpuPower  = 0x600,
    //PPI_targetCpuReset	// is this the same as DCI's "CPU/Warm Reset"?
    //PPI_targetNcReset
    PPI_targetGlobalReset = 0x603,
    //PPI_targetS5OrSoi3
    //PPI_targetS4OrSoi2
    //PPI_targetS3OrSoi1
    PPI_statePortValueChanged = 0x700,
};

const PPI_PluginEvent PPI_TYPE_EVENTS[] = {
    PPI_probeAdded,
    PPI_probePower,
    PPI_probeReset,
    PPI_probeError,
    PPI_probeRemoved,
    PPI_interfaceArbitration,
    PPI_interfaceError,
    PPI_targetPower,
    PPI_targetReset,
    PPI_targetPrdy,
    PPI_targetSyncIn,
    PPI_targetBclk_xdpa,
    PPI_targetRclk_xdpa,
    PPI_targetLaIn_xdpa,
    PPI_targetSleepStall_xdpa,
    PPI_targetPmodePacket,
    PPI_targetDciPacket,
    PPI_triggerResponse,
    PPI_targetCpuPower };

#define PPI_TYPE_EVENTS_LENGTH 19

//! Plugin callback event handler
typedef void (* PluginEventCallbackHandler)(
    OpenIPC_DeviceId deviceId,    // device ID scope of probe or interface, -1 for "probeAdded"
    PPI_PluginEvent  pluginEvent, // The event that occurred
    uint64_t         value        // This is the value associated with the event, the MSB is reserved and is currently not part of the value.
    );

//! Notification level
DECLARE_ENUM(PPI_NotificationLevel)
{
    PPI_traceNotification,
    PPI_debugNotification,
    PPI_infoNotification,
    PPI_warningNotification,
    PPI_errorNotification,
    PPI_noNotification,
};

//! Logging stream definition
DECLARE_ENUM(PPI_Stream)
{
    PPI_API_STREAM,                            //! This is for logging the in/out parameters of the probe API method calls.
    PPI_INTERNAL_STREAM,                       //! This is for any internal, unstructured logging per plugin. PluginLogCallbackHandler uses this stream
    PPI_HARDWARE_COMMUNICATION_STREAM,         //! This is for any direct communication with external hardware; this is expected to be the *lowest* level possible, and the format varies per plugin.
    PPI_INTERFACE_OPERATION_STREAM,            //! This is for public logging of interface data (e.g., JTAG, I2C, Pins, etc.). The type and format of the log is dependent on the device ID type.
    PPI_BACKEND_HARDWARE_COMMUNICATION_STREAM, //! This is another hardware communication stream; it should be used by any additional agents (e.g., the probe plugin connects to another agent to do work on its behalf) to log information
    // Any additional streams are added here!
    PPI_LAST_STREAM //! A fake stream that corresponds to 1 past the last valid stream; used for checking inside the implementation and any clients
};

//! Notification callback handler type
typedef void (* PluginNotificationCallbackHandler)(OpenIPC_DeviceId deviceId, PPI_NotificationLevel level, OpenIPC_Error error, const char* message);
//! Log callback handler type
typedef void (* PluginLogCallbackHandler)(OpenIPC_DeviceId deviceId, PPI_NotificationLevel level, const char* message);
//! Log callback type that comprehends multiple 'streams' of data PluginStreamLogCallbackHandler(d, PPI_INTERNAL_STREAM, l, cp) is the same as PluginLogCallbackHandler(d, l, cp)
typedef void (* PluginStreamLogCallbackHandler)(OpenIPC_DeviceId deviceId, PPI_Stream stream, PPI_NotificationLevel level, const char* message);

//! plugin API version number
typedef uint32_t PPI_PluginApiVersion;
//! Current SDK/API version
const PPI_PluginApiVersion PPI_PLUGIN_API_VERSION = 24;
//! Version 25 adds S0i1_0 and S0i1_1 stalls
//! Version 24 adds PPI_interfaceTypePort for any generic interface port.
//! Version 23 fixes default implementation of PPI_ProbeBeginInitialize to return not implemented.
//! Version 22 adds new methods for stepwise initialization of probes/interfaces.
//! Version 21 adds a new field to PPI_PluginInfo to indicate whether the plug-in requires lock notifications.
//! Version 20 adds new methods to get available probe types and create static probes.
//! Version 19 adds a new method, PPI_Bundle_InterfaceSetConfig, to append interface configs to a bundle.
//! Version 18 adds a new field to PPI_JTAG_StateGotoOptions to indicate if a trigger timeout should stop the bundle execute.
//! Version 17 adds a new method to append JTAG interface padding changes within a bundle.
//! Version 16 adds virtual pins for new SOix.y and IVRx PMC Handshake trigger responses.
//! Version 15 adds a method to get the details of a PPI_statePortValueChanged event (which is replacing the PPI_mailboxIosfSb and PPI_mailboxIosfP events)
//! Version 14 adds a new logger stream, PPI_BACKEND_HARDWARE_COMMUNICATION_STREAM
//! Version 13 adds a new interface type: stateport with several functions for usage (see StateportOperations.h)
//! Version 12 adds a new interface port enum for HTI.
//! Version 11 adds a method to report remote connection information
//! Version 10 adds a method to set an input argument to a device action.
//! Version 9 adds a JTAG trigger to the State-based API
//! Version 8 adds device configuration strings, actions and adds some extra interface types. Also, allows for both read and write interfaces
//! Version 7 adds a method to check if an event can be relied on from the plugin implementation
//! Version 6 adds some additional logging functions that enable a client to route certain types of logging messages according to "streams"
//! Version 5 modifies the PluginLogCallbackHandler function to not take an error; it also modifies the PPI_Pins_TypeEncode to include more virtual pins and adds a few functions. An additional API for TriggerResponses has been added as well.
//! Version 4 adds functions to set the current log/notification level in a plugin
//! Version 3 adds a probeInfo field to PPI_ProbeInfo

#if defined(__cplusplus)
    const size_t PPI_MAX_INFO_LEN = 512;
    const size_t PPI_MAX_PROBE_TYPE_LEN = 512;
#else
    #define PPI_MAX_INFO_LEN 512
    #define PPI_MAX_PROBE_TYPE_LEN 512
#endif

/**
* @brief structure to hold information regarding plugin information.
*/
typedef struct
{
    //! @brief API version implemented by the plugin
    PPI_PluginApiVersion pluginInterfaceVersion;
    //! @brief true if and only if the plugin determines that it is compatible with the client's API version.
    PPI_bool clientInterfaceSupported;
    //! @brief Null-terminated string.
    PPI_char pluginName[PPI_MAX_INFO_LEN];
    //! @brief Indicates whether the probe needs to be notified of locking bundles containing no payload.
    PPI_bool requiresLockNotifications;
} PPI_PluginInfo;

/**
* @brief structure to hold information regarding plugin capabilities.
*/
typedef struct
{
    //! Does this plugin support the auto config to identify connected probes with a default configuration.
    uint32_t supportAuto : 1;
    //! Does this plugin support dynamic initialization.
    uint32_t supportDynamicInitialization : 1;
    uint32_t reserved : 30;
} PPI_PluginCapabilities;

/**
* @brief structure to hold information regarding probe information.
*/
typedef struct
{
    //! Probe type
    PPI_char type[PPI_MAX_INFO_LEN];
    //! Unique identifier for this probe; must be unique to this plugin
    PPI_char uniqueIdentifier[PPI_MAX_INFO_LEN];
    //! @brief Used by probe to return information that might be useful for logging purposes (FW revision, HW revision, etc.); format determined by probeVendor
    PPI_char probeInfo[PPI_MAX_INFO_LEN];
} PPI_ProbeInfo;

typedef struct
{
    //! This is for a plugin to indicate that it is a remote connection
    PPI_bool isRemote;
    //! This is for a plugin to indicate that the connection is secure.
    PPI_bool isSecure;
} PPI_RemoteConnectionInformation;

/**
* @brief structure to hold information regarding JTAG interface capabilities.
*/
typedef struct
{
    //! Does the interface support using the TRST pin?
    uint32_t supportTRST : 1;
    //! Does the interface support cycling to TLR for a TAP reset. Note: supportTRST | supportTLR == 1
    uint32_t supportTLR : 1;
    //! Does the interface support staying in PauDR in a register interface
    uint32_t supportPauDRInRegisterInterface : 1;
} PPI_InterfaceJTAGCapabilities;

/**
* @brief structure to hold information regarding Pins interface capabilities.
*/
typedef struct
{
    //! Do not use, may be replaced later
    uint32_t reserved;
} PPI_InterfacePinsCapabilities;

/**
* @brief structure to hold information regarding I2C interface capabilities.
*/
typedef struct
{
    /**This is the minimum delay for any Pins Operation. */
    uint32_t minimumtimeGraularityInMicroSeconds;
} PPI_InterfaceI2CCapabilities;

/**
* @brief structure to hold information regarding Trace interface capabilities.
*/
typedef struct
{
    //! Do not use, may be replaced later
    uint32_t reserved;
} PPI_InterfaceTraceCapabilities;

//! Trace port access mode
DECLARE_ENUM(PPI_Trace_PortAccessMode)
{
    PPI_Trace_PortAccessRead,
    PPI_Trace_PortAccessWrite,
    PPI_Trace_PortAccessReadWrite, // Generally useful (and supported) only for debug interfaces
};

//! Callback invoked when accessing a probe interface has started
typedef void (* PluginInterfaceAccessStartedCallbackHandler)(OpenIPC_DeviceId deviceId, uint64_t inputBitCount, uint64_t outputBitCount);

//! Callback invoked when accessing a probe interface has completed
typedef void (* PluginInterfaceAccessCompletedCallbackHandler)(OpenIPC_DeviceId deviceId, uint64_t inputBitCount, uint64_t outputBitCount);

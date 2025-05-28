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
///  @brief Contains methods for obtaining instances of the Probe interfaces
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ProbeTypes.h"

//! Export ifdef
#if defined(HOST_WINDOWS) || defined(_WIN32)
    #if defined(PROBEPLUGIN_EXPORTS)
        #define PROBEPLUGIN_API __declspec(dllexport)
    #else
        #define PROBEPLUGIN_API __declspec(dllimport)
    #endif
#else
    #define PROBEPLUGIN_API __attribute__ ((visibility("default")))
#endif

//////////////////////////////////////////////////////////////////////////////
///
/// @brief Plugins are used in the context of the Probe Abstraction framework.
///        IProbePlugin describes the ability to create instances
///        of probes and interfaces.
///
//////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginGetInfo
//
/// @brief Get plugin-specific information and check version compatibility
///
/// @note This may be called before the plugin is initialized.
///
/// @param[out] info Contains information specific to this plugin.
/// @param[in] clientInterfaceVersion The client's interface version
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginGetInfo(PPI_PluginApiVersion clientInterfaceVersion, PPI_PluginInfo* info);
typedef OpenIPC_Error (* PPI_PluginGetInfo_TYPE)(PPI_PluginApiVersion clientInterfaceVersion, PPI_PluginInfo* info);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginInitialize
//
/// @brief Initializes the plugin and allows the caller to provide vendor
/// specific
///
/// @param[in] vendorinit
///     Plugin specific string populated by the plugin.
///@param[in] pluginId
///     The plugin reference id to be used for this plugin.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Plugin_Already_Initialized
///     The plugin has been previously initialized.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_PluginInitialize(PPI_RefId pluginId, const PPI_char* vendorinit);
typedef OpenIPC_Error (* PPI_PluginInitialize_TYPE)(PPI_RefId pluginId, const PPI_char* vendorinit);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginDeinitialize
///
/// @brief Deinitializes the plugin, allowing the plugin to cleanup it's probes
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_PluginDeinitialize(void);
typedef OpenIPC_Error (* PPI_PluginDeinitialize_TYPE)(void);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginGetCapabilities
///
/// @brief Queries the plugin to determine some of the supported features.
///
/// @note This may be called before the plugin is initialized.
///
/// @param[out] capabilities Contains information specific to this plugin.
///
/// @pre @p capabilities is not null and is zero initialized.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_PluginGetCapabilities(PPI_PluginCapabilities* capabilities);
typedef OpenIPC_Error (* PPI_PluginGetCapabilities_TYPE)(PPI_PluginCapabilities* capabilities);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginGetProbeTypes
///
/// @brief Provides a list of possible probe types.
///
/// Returns a list of the static probe types supported by the plugin
///
/// @param[in] maxProbeTypes
///    The maximum number of probe types that can be safely returned in probeTypes.
///
/// @param[out] probeTypes
///    Populated with the list of probe types supported by the plugin.
///
/// @param[out] probeTypeCount
///    Populated with the total number of probes types supported. If this is > maxProbeTypes, the client should increase the buffer size and re-call.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Plugin_Not_Initialized
///     The Plugin has not been initialized.
///
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_PluginGetProbeTypes(uint32_t maxProbeTypes, PPI_char const** probeTypes, uint32_t* probeTypeCount);
typedef OpenIPC_Error (* PPI_PluginGetProbeTypes_TYPE)(uint32_t maxProbeTypes, PPI_char const** probeTypes, uint32_t* probeTypeCount);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginCreateStaticProbe
///
/// @brief Creates a static probe for this plugin.
///s
/// Create a of the specified type, and returns the probe ref id.
///
/// @param[in] probeType
///    The type of probe to create.
///
/// @param[out] refId
///    Populated with the probe reference id created.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Plugin_Not_Initialized
///     The Plugin has not been initialized.
///
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_PluginCreateStaticProbe(const PPI_char probeType[PPI_MAX_PROBE_TYPE_LEN], PPI_RefId* refId);
typedef OpenIPC_Error (* PPI_PluginCreateStaticProbe_TYPE)(const PPI_char probeType[PPI_MAX_PROBE_TYPE_LEN], PPI_RefId* refId);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeGetRefIds
///
/// @brief Provides a list of probe reference identification numbers
///
/// Each plugin contains 1 or more probes. PPI_ProbeGetRefIds() returns
/// a list of the probes held by the plugin.
///
/// @param[in] maxIds
///    The maximum number of reference ids that can be safely returned in refids.
///
/// @param[out] refIds
///    Populated with the list of probe reference ids held by the plugin.
///
/// @param[out] probeCount
///    Populated with the total number of probes available. If this is > maxIds, the client should increase the buffer size and re-call.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Plugin_Not_Initialized
///     The Plugin has not been initialized.
///
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeGetRefIds(uint32_t maxIds, PPI_RefId* refIds, uint32_t* probeCount);
typedef OpenIPC_Error (* PPI_ProbeGetRefIds_TYPE)(uint32_t maxIds, PPI_RefId* refIds, uint32_t* probeCount);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeGetInfo
///
/// @brief Returns probe specific information in @a type
///
///
/// @param[in] refId
///     The reference id of the probe to get information about.
/// @param[out] info
///     Probe-specific structure, filled in by the plugin.
///
/// @pre The plugin is initialized.
/// @pre @p info is not null and is zero initialized.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a refId is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeGetInfo(PPI_RefId refId, PPI_ProbeInfo* info);
typedef OpenIPC_Error (* PPI_ProbeGetInfo_TYPE)(PPI_RefId refId, PPI_ProbeInfo* info);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeBeginInitialization
///
/// @brief Begins stepwise initialization of the specified probe, assigning
///        the @a probeID to the probe instance. Subsequent probe interaction
///        is done based on the probe ID.
///
/// @note Once stepwise initialization is started, PPI_ProbeInitialize() can
///       no longer be called on the probe (these different initialization
///       paths cannot be mixed and matched). The following functions are
///       operable on the probe before finishing initialization:
///           - PPI_ProbeSpecifyVendorInitString()
///           - PPI_DeviceListConfigString()
///           - PPI_DeviceGetConfigDescription()
///           - PPI_DeviceListConfigValues()
///           - PPI_DeviceSetConfig()
///           - PPI_DeviceGetConfig()
///       The vendor init string specified and any device config values set
///       prior will take effect once initialization is finished for the probe
///       using PPI_ProbeFinishInitialization(). The recommended order of
///       operations is:
///           1. PPI_ProbeBeginInitialization()
///           2. PPI_DeviceListConfigStrings()/PPI_DeviceListConfigValues()
///           3. PPI_ProbeSpecifyVendorInitString()
///           4. PPI_DeviceGetConfig()/PPI_DeviceSetConfig()
///           5. PPI_ProbeFinishInitialization()
///
/// @param[in] probe_refid
///     The reference ID of the probe to begin initialization and assign the
///     device ID to.
/// @param[in] probeID
///     The unique identification number the plugin will assign to this probe
///     provided in @a probe_refid. Future calls to the probe will use this
///     identifier.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference ID @a probe_refid is not valid.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeBeginInitialization(PPI_RefId probe_refid, OpenIPC_DeviceId probeID);
typedef OpenIPC_Error (* PPI_ProbeBeginInitialization_TYPE)(PPI_RefId probe_refid, OpenIPC_DeviceId probeID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeSpecifyVendorInitString
///
/// @brief Specifies the probe specific initialization string for the probe.
///
/// @note Initialization must have been started for the probe using
///       PPI_ProbeBeginInitialization(). The vendor init string may
///       override device config values set prior to specifying the init
///       string.
///
/// @param[in] probeID
///     The unique identification number for the probe to specify the init
///     string for.
/// @param[in] vendorinit
///     Probe specific string populated by the client.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe ID @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeSpecifyVendorInitString(OpenIPC_DeviceId probeID, const PPI_char* vendorinit);
typedef OpenIPC_Error (* PPI_ProbeSpecifyVendorInitString_TYPE)(OpenIPC_DeviceId probeID, const PPI_char* vendorinit);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeFinishInitialization
///
/// @brief Initializes the probe using the vendor init string (if specified
///        using PPI_ProbeSpecifyVendorInitString()) and any device config
///        values set prior on the probe.
///
/// @note Initialization must have been started for the probe using
///       PPI_ProbeBeginInitialization().
///
/// @param[in] probeID
///     The unique identification number for the probe to finish
///     initialization for.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe ID @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeFinishInitialization(OpenIPC_DeviceId probeID);
typedef OpenIPC_Error (* PPI_ProbeFinishInitialization_TYPE)(OpenIPC_DeviceId probeID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeInitialize
///
/// @brief Initializes the probe with the vendor specific information and
///     assigns the @a probeID to the probe instance.  Subsequent probe interaction
///     is done based on the probeID.
///
/// @note Logically, PPI_ProbeInitialize() is equivalent to:
///           1. PPI_ProbeBeginInitialization()
///           2. PPI_ProbeSpecifyVendorInitString()
///           3. PPI_ProbeFinishInitialization()
///
/// @param[in] probe_refid
///     The reference id of the probe to initialize.  If this
///     id is not valid on this plugin, @ref OpenIPC_Error_Invalid_Device_ID
///     is returned.
/// @param[in] probeID
///     The unique identification number the plugin will assign to this probe
///     provided in @a refid. Future calls to the plugin will use this identifier.
/// @param[in] vendorinit
///     Probe specific string populated by the client.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probe_refid is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeInitialize(PPI_RefId probe_refid, OpenIPC_DeviceId probeID, const PPI_char* vendorinit);
typedef OpenIPC_Error (* PPI_ProbeInitialize_TYPE)(PPI_RefId probe_refid, OpenIPC_DeviceId probeID, const PPI_char* vendorinit);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ProbeDeInitialize
///
/// @brief De initializes the probe and allows the plugin to reclaim resources.
///
/// @param[in] probeID
///     The unique identification number for the probe to deinitialize
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_ProbeDeInitialize(OpenIPC_DeviceId probeID);
typedef OpenIPC_Error (* PPI_ProbeDeInitialize_TYPE)(OpenIPC_DeviceId probeID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetRefIds
///
/// @brief Each probe holds a list of interfaces.  InterfaceGetRefIds provides
///     the list of interface present for the given @a probeID.
///
/// @param[in] probeID
///     The unique identification number for the probe to get interface
///     information from.  @a probeID is set during @a ProbeInitialize
/// @param[in] maxIds
///     Maximum number of interface reference ids that can be returned in @a refIds
/// @param[out] refIds
///     An array of interface reference ids that are associated with this probe.
/// @param[out] interfacecount
///     The number of interface reference ids in the probe, if this is > maxIds, then the client should increase the refIds buffer size and re-call.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetRefIds(OpenIPC_DeviceId probeID, uint8_t maxIds, PPI_RefId* refIds, uint32_t* interfacecount);
typedef OpenIPC_Error (* PPI_InterfaceGetRefIds_TYPE)(OpenIPC_DeviceId probeID, uint8_t maxIds, PPI_RefId* refIds, uint32_t* interfacecount);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetType
///
/// @brief Returns the interface type for the given @a probeID and
///     @a interface_refid.
///
/// @param[in] probeID
///     The unique identification number for the probe to get interface
///     information from.  @a probeID is set during @a ProbeInitialize
/// @param[in] interface_refid
///     Specifies the interface to get information about.
/// @param[out] interface_type
///     Indicates the type of interface.  See also @a EInterfaceType
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
/// @deprecated
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetType(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_EInterfaceType* interface_type);
typedef OpenIPC_Error (* PPI_InterfaceGetType_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_EInterfaceType* interface_type);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetInfoJTAG
///
/// @brief Returns the interface type for the given @a probeID and
///     @a interface_refid.
///
/// @param[in] probeID
///     The unique identification number for the probe to get interface
///     information from.  @a probeID is set during @a ProbeInitialize
/// @param[in] interface_refid
///     Specifies the interface to get information about.
/// @param[out] capabilities
///     Indicates the capabilities of this interface.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a interfaceID is not valid (either invalid or the wrong type)
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetInfoJTAG(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfaceJTAGCapabilities* capabilities);
typedef OpenIPC_Error (* PPI_InterfaceGetInfoJTAG_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfaceJTAGCapabilities* capabilities);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetInfoPins
///
/// @brief Returns the interface type for the given @a probeID and
///     @a interface_refid.
///
/// @param[in] probeID
///     The unique identification number for the probe to get interface
///     information from.  @a probeID is set during @a ProbeInitialize
/// @param[in] interface_refid
///     Specifies the interface to get information about.
/// @param[out] capabilities
///     Indicates the capabilities of this interface.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a interfaceID is not valid (either invalid or the wrong type)
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetInfoPins(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfacePinsCapabilities* capabilities);
typedef OpenIPC_Error (* PPI_InterfaceGetInfoPins_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfacePinsCapabilities* capabilities);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetInfoI2C
///
/// @brief Returns the interface type for the given @a probeID and
///     @a interface_refid.
///
/// @param[in] probeID
///     The unique identification number for the probe to get interface
///     information from.  @a probeID is set during @a ProbeInitialize
/// @param[in] interface_refid
///     Specifies the interface to get information about.
/// @param[out] capabilities
///     Indicates the capabilities of this interface.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a interfaceID is not valid (either invalid or the wrong type)
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetInfoI2C(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfaceI2CCapabilities* capabilities);
typedef OpenIPC_Error (* PPI_InterfaceGetInfoI2C_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, PPI_InterfaceI2CCapabilities* capabilities);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceBeginInitialization
///
/// @brief Begins stepwise initialization of the specified interface, assigning
///        the @a interfaceID to the interface instance. Subsequent interface
///        interaction is done based on the interface ID.
///
/// @note Once stepwise initialization is started, PPI_InterfaceInitialize()
///       can no longer be called on the interface (these different
///       initialization paths cannot be mixed and matched). The following
///       functions are operable on the interface before finishing
///       initialization:
///           - PPI_InterfaceSpecifyVendorInitString()
///           - PPI_DeviceListConfigString()
///           - PPI_DeviceGetConfigDescription()
///           - PPI_DeviceListConfigValues()
///           - PPI_DeviceSetConfig()
///           - PPI_DeviceGetConfig()
///       The vendor init string specified and any device config values set
///       prior will take effect once initialization is finished for the
///       interface using PPI_InterfaceFinishInitialization(). The recommended
///       order of operations is:
///           1. PPI_InterfaceBeginInitialization()
///           2. PPI_DeviceListConfigStrings()/PPI_DeviceListConfigValues()
///           3. PPI_InterfaceSpecifyVendorInitString()
///           4. PPI_DeviceGetConfig()/PPI_DeviceSetConfig()
///           5. PPI_InterfaceFinishInitialization()
///
/// @param[in] probeID
///     The unique identification number for the probe to which the interface
///     to initialize is associated.
/// @param[in] interface_refid
///     The reference ID of the interface to begin initialization and assign the
///     device ID to.
/// @param[in] interfaceID
///     The unique identification number the plugin will assign to this
///     interface provided in @a interface_refid. Future calls to the interface
///     will use this identifier.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe ID @a probeID is not valid or the interface reference ID
///     @a interface_refid is not valid.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceBeginInitialization(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, OpenIPC_DeviceId interfaceID);
typedef OpenIPC_Error (* PPI_InterfaceBeginInitialization_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, OpenIPC_DeviceId interfaceID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceSpecifyVendorInitString
///
/// @brief Specifies the probe specific initialization string for the interface.
///
/// @note Initialization must have been started for the interface using
///       PPI_InterfaceBeginInitialization(). The vendor init string may
///       override device config values set prior to specifying the init
///       string.
///
/// @param[in] interfaceID
///     The unique identification number for the interface to specify the init
///     string for.
/// @param[in] vendorinit
///     Probe specific string populated by the client.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The interface ID @a interfaceID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceSpecifyVendorInitString(OpenIPC_DeviceId interfaceID, const PPI_char* vendorinit);
typedef OpenIPC_Error (* PPI_InterfaceSpecifyVendorInitString_TYPE)(OpenIPC_DeviceId interfaceID, const PPI_char* vendorinit);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceFinishInitialization
///
/// @brief Initializes the interface using the vendor init string (if specified
///        using PPI_InterfaceSpecifyVendorInitString()) and any device config
///        values set prior on the interface.
///
/// @note Initialization must have been started for the interface using
///       PPI_InterfaceBeginInitialization().
///
/// @param[in] interfaceID
///     The unique identification number for the interface to finish
///     initialization for.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The interface ID @a interfaceID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceFinishInitialization(OpenIPC_DeviceId interfaceID);
typedef OpenIPC_Error (* PPI_InterfaceFinishInitialization_TYPE)(OpenIPC_DeviceId interfaceID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceInitialize
///
/// @brief Initializes the interface and allows the caller to provide vendor specific information
///
/// @note Logically, PPI_InterfaceInitialize() is equivalent to:
///           1. PPI_InterfaceBeginInitialization()
///           2. PPI_InterfaceSpecifyVendorInitString()
///           3. PPI_InterfaceFinishInitialization()
///
/// @param[in] probeID
///     The unique identification number for the probe to which the interface to initialize is associated.
/// @param[in] interface_refid
///     Specifies the interface to initialize.
/// @param[in] interfaceID
///     The unique identification number to assign to this interface.
///     Future calls to the interface will use this identifier.probe
/// @param[out] vendorinit
///     A vendor specific initialization string for initializing the interface
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceInitialize(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, OpenIPC_DeviceId interfaceID, const PPI_char* vendorinit);
typedef OpenIPC_Error (* PPI_InterfaceInitialize_TYPE)(OpenIPC_DeviceId probeID, PPI_RefId interface_refid, OpenIPC_DeviceId interfaceID, const PPI_char* vendorinit);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceDeInitialize
///
/// @brief Deinitializes the interface and allows cleanup. Note: one should be able to implement a "reset" using an InterfaceDeinitialize and a InterfaceInitialize
///
/// @param[in] interfaceID
///     The unique identification number to assign to this interface.
///     Future calls to the interface will use this identifier.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceDeInitialize(OpenIPC_DeviceId interfaceID);
typedef OpenIPC_Error (* PPI_InterfaceDeInitialize_TYPE)(OpenIPC_DeviceId interfaceID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceConfigString
///
/// @brief PPI_InterfaceConfigString is a mechanism to return the current interface configuration strings.
///
///     Conceptually, this is equivalent to calling DeviceListConfigString with the additional check that the device is an interface
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] configTypeLength
///     Length of the configType buffer
/// @param[out] configType
///     A array of pointers to the configuration strings; the array is client managed, but the const strings are managed by the plugin.
/// @param[out] numberOfConfigTypes
///     The actual number of configTypes (not just those returned, if *numberOfConfigTypes > configTypeLength, the client must increase its buffer size!)
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceListConfigString(OpenIPC_DeviceId interfaceID, uint32_t configTypeLength, PPI_char const** configType, uint32_t* numberOfConfigTypes);
typedef OpenIPC_Error (* PPI_InterfaceListConfigString_TYPE)(OpenIPC_DeviceId interfaceID, uint32_t configTypeLength, PPI_char const** configType, uint32_t* numberOfConfigTypes);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceListLockInterfacePeers
///
/// @brief PPI_InterfaceListLockInterfacePeers is a mechanism to list interfaces equivalence classes under LOCK/RELEASE.
///
///     PPI_InterfaceListLockInterfacePeers is a mechanism to determine which other interfaces are impacted by a LOCK on one.
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] peerInterfacesLength
///     Length of the peerInterfaces buffer
/// @param[out] peerInterfaces
///     An array of OpenIPC_DeviceID: these are identifiers which have the property that a lock on one is a lock on all and interfaceID is one of them.
/// @param[out] numberOfPeerInterfaces
///     The actual number of peerInterfaces (not just those returned, if *numberOfPeerInterfaces > configTypeLength, the client must increase its buffer size!)
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceListLockInterfacePeers(OpenIPC_DeviceId interfaceID, uint32_t peerInterfacesLength, OpenIPC_DeviceId* peerInterfaces, uint32_t* numberOfPeerInterfaces);
typedef OpenIPC_Error (* PPI_InterfaceListLockInterfacePeers_TYPE)(OpenIPC_DeviceId interfaceID, uint32_t peerInterfacesLength, OpenIPC_DeviceId* peerInterfaces, uint32_t* numberOfPeerInterfaces);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceSetConfig
///
/// @brief PPI_InterfaceSetConfig allows the setting of interface specific configurations.
///
///     PPI_InterfaceSetConfig allows the caller to send in a string and value combination to the interface.
///     For example, a JTAG interface may have a parameter for the TCLK frequency.   @a allows
///     the caller to set the current TCLK frequency value for the interface.
///     Conceptually, this is equivalent to calling DeviceSetConfig with the additional check that the device is an interface
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] configType
///     A null terminated string that specifies what to configure.
/// @param[in] value
///     A string value to set for the given @a configType
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a interfaceID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceSetConfig(OpenIPC_DeviceId interfaceID, const PPI_char* configType, const PPI_char value[PPI_MAX_INFO_LEN]);
typedef OpenIPC_Error (* PPI_InterfaceSetConfig_TYPE)(OpenIPC_DeviceId interfaceID, const PPI_char* configType, const PPI_char value[PPI_MAX_INFO_LEN]);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceGetConfig
///
/// @brief PPI_InterfaceGetConfig allows the reading of an interface specific configuration.
///
///     PPI_InterfaceGetConfig allows the caller to read value for a specified setting
///     For example, a JTAG interface may have a parameter for the TCLK frequency, @a allows
///     the caller to retrieve the current TCLK frequency value for the interface.
///     Conceptually, this is equivalent to calling DeviceGetConfig with the additional check that the device is an interface
//
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] configType
///     A null terminated string that specifies what configuration setting to retrieve
/// @param[out] value
///     A string value to set for the given @a configType
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceGetConfig(OpenIPC_DeviceId interfaceID, const PPI_char* configType, PPI_char value[PPI_MAX_INFO_LEN]);
typedef OpenIPC_Error (* PPI_InterfaceGetConfig_TYPE)(OpenIPC_DeviceId interfaceID, const PPI_char* configType, PPI_char value[PPI_MAX_INFO_LEN]);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceSetConfig
///
/// @brief PPI_DeviceSetConfig allows the setting of device specific configurations.
///
///     PPI_DeviceSetConfig allows the caller to send in a string and value combination to the device.
///     For example, a JTAG interface may have a parameter for the TCLK frequency.   @a allows
///     the caller to set the current TCLK frequency value for the interface.
///
/// @note When using stepwise initialization (see PPI_ProbeBeginInitialization()
///       and PPI_InterfaceBeginInitialization()) setting a device config may
///       override config values set in the specified vendor init string.
///
/// @param[in] deviceID
///     The unique identification number of the device, set during @a InterfaceInitialize or ProbeInitialize. If set to OpenIPC_INVALID_DEVICE_ID, then this targets the plugin.
/// @param[in] configType
///     A null terminated string that specifies what to configure.
/// @param[in] value
///     A string value to set for the given @a configType
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a deviceID is not valid
/// @return OpenIPC_Error_Probe_Config_Not_Defined
///     The specified @a configType does not correspond to a valid
///     configuration value.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceSetConfig(OpenIPC_DeviceId deviceID, const PPI_char* configType, const PPI_char value[PPI_MAX_INFO_LEN]);
typedef OpenIPC_Error (* PPI_DeviceSetConfig_TYPE)(OpenIPC_DeviceId deviceID, const PPI_char* configType, const PPI_char value[PPI_MAX_INFO_LEN]);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceGetConfigDescription
///
/// @brief Gets the description of a device specific configuration.
///
/// @param[in] deviceID
///     The unique identification number of the interface, set during
///     @a InterfaceInitialize or ProbeInitialize. If set to
///     OpenIPC_INVALID_DEVICE_ID, then this targets the plugin.
/// @param[in] configType
///     A null terminated string that specifies what configuration setting
///     to retrieve the description of.
/// @param[out] description
///     Returns the description of the device specific configuration.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The specified @a deviceID is not valid.
/// @return OpenIPC_Error_Probe_Config_Not_Defined
///     The specified @a configType does not correspond to a valid
///     configuration value.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceGetConfigDescription(OpenIPC_DeviceId deviceID, const PPI_char* configType, PPI_char description[PPI_MAX_INFO_LEN]);
typedef OpenIPC_Error (* PPI_DeviceGetConfigDescription_TYPE)(OpenIPC_DeviceId deviceID, const PPI_char* configType, PPI_char description[PPI_MAX_INFO_LEN]);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceListConfigValues
///
/// @brief Gets a list of the possible values of the device specific
///        configuration (if applicable).
///
/// @param[in] deviceID
///     The unique identification number of the interface, set during
///     @a InterfaceInitialize or ProbeInitialize. If set to
///     OpenIPC_INVALID_DEVICE_ID, then this targets the plugin.
/// @param[in] configType
///     A null terminated string that specifies what configuration setting
///     to retrieve the possible values for.
/// @param[in] valuesLength
///     Length of the @a values buffer.
/// @param[out] values
///     A array of pointers to the values; the array is client managed,
///     but the const strings are managed by the plugin.
/// @param[out] numberOfValues
///     The actual number of values (not just those returned, if
///     *numberOfValues > valuesLength, the client must increase its buffer
///     size!).
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The specified @a deviceID is not valid.
/// @return OpenIPC_Error_Probe_Config_Not_Defined
///     The specified @a configType does not correspond to a valid
///     configuration value.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceListConfigValues(OpenIPC_DeviceId deviceID, const PPI_char* configType, uint32_t valuesLength, PPI_char const** values, uint32_t* numberOfValues);
typedef OpenIPC_Error (* PPI_DeviceListConfigValues_TYPE)(OpenIPC_DeviceId deviceID, const PPI_char* configType, uint32_t valuesLength, PPI_char const** values, uint32_t* numberOfValues);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceGetConfig
///
/// @brief PPI_DeviceGetConfig allows the reading of an device specific configuration.
///
///     PPI_DeviceGetConfig allows the caller to read value for a specified setting
///     For example, a JTAG interface may have a parameter for the TCLK frequency, @a allows
///     the caller to retrieve the current TCLK frequency value for the interface.
///
/// @param[in] deviceID
///     The unique identification number of the interface, set during @a InterfaceInitialize or ProbeInitialize. If set to OpenIPC_INVALID_DEVICE_ID, then this targets the plugin.
/// @param[in] configType
///     A null terminated string that specifies what configuration setting to retrieve
/// @param[out] value
///     A string value to set for the given @a configType
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a deviceID is not valid
/// @return OpenIPC_Error_Probe_Config_Not_Defined
///     The specified @a configType does not correspond to a valid
///     configuration value.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceGetConfig(OpenIPC_DeviceId deviceID, const PPI_char* configType, PPI_char value[PPI_MAX_INFO_LEN]);
typedef OpenIPC_Error (* PPI_DeviceGetConfig_TYPE)(OpenIPC_DeviceId deviceID, const PPI_char* configType, PPI_char value[PPI_MAX_INFO_LEN]);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceListConfigString
///
/// @brief PPI_DeviceListConfigString is a mechanism to return the current device configuration strings.
///
///
/// @param[in] deviceID
///     The unique identification number of the interface, set during @a InterfaceInitialize or ProbeInitialize. If set to OpenIPC_INVALID_DEVICE_ID, then this targets the plugin.
/// @param[in] configTypeLength
///     Length of the configType buffer
/// @param[out] configType
///     A array of pointers to the configuration strings; the array is client managed, but the const strings are managed by the plugin.
/// @param[out] numberOfConfigTypes
///     The actual number of configTypes (not just those returned, if *numberOfConfigTypes > configTypeLength, the client must increase its buffer size!)
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a deviceID is not valid
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceListConfigString(OpenIPC_DeviceId deviceID, uint32_t configTypeLength, PPI_char const** configType, uint32_t* numberOfConfigTypes);
typedef OpenIPC_Error (* PPI_DeviceListConfigString_TYPE)(OpenIPC_DeviceId deviceID, uint32_t configTypeLength, PPI_char const** configType, uint32_t* numberOfConfigTypes);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceOperationCancel
///
/// @brief Cancels commands queued to a interface
///
/// Attempts to cancel the operations executing on a interface.
/// @param [in] interfaceID interface to cancel all commands .
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceOperationCancel(OpenIPC_DeviceId interfaceID);
typedef OpenIPC_Error (* PPI_InterfaceOperationCancel_TYPE)(OpenIPC_DeviceId interfaceID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginRegisterEventHandler
///
/// @brief Function to register an event handler
///
/// @note This may be called before the plugin is initialized.
///
/// This function allows a client to register an event handler; unlike most functions, this one can be called *before* a plugin is instantiated,
/// though no information will be posted until the PPI_PluginInitialize operation is called. Although optional, it is highly encouraged to register
/// prior to calling PPI_PluginInitialize to capture all events that occur during and immediately after Plugin Initialization.
/// @param [in] eventHandlerFunction The function the plugin should call when an event has occurred. Note: we recommend that these callbacks do not call into the API as error messages, etc. are more difficult to handle in this case.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginRegisterEventHandler(PluginEventCallbackHandler eventHandlerFunction);
typedef OpenIPC_Error (* PPI_PluginRegisterEventHandler_TYPE)(PluginEventCallbackHandler eventHandlerFunction);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginEventGenerated
///
/// @brief Function to provide information to a client on whether to expect particular events to be generated.
///
/// This function provides information to a client on whether the plugin implementation guarantees that a particular
/// event will be generated for each plugin event type. Basically, this is to let a client know if the client can rely on
/// this event being generated or if the client should use other means to ensure that no events are missed.
/// A return of false here does *not* mean that events of that type will not be generated, just that the client cannot rely on all
/// instances of that physical occurrence generating an PPI_PluginEvent.
///
/// @param [in] pluginEvent The plugin event of interest
/// @param [in] probe The probe of interest.
/// @param [out] pluginGeneratesEvent Is set to true iff the plugin ensures that this event is always generated when the corresponding target event occurs.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
/// @retval OpenIPC_Error_Null_Pointer if pluginGeneratesEvent is passed in as NULL
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginEventGenerated(OpenIPC_DeviceId probe, PPI_PluginEvent pluginEvent, PPI_bool* pluginGeneratesEvent);
typedef OpenIPC_Error (* PPI_PluginEventGenerated_TYPE)(OpenIPC_DeviceId probe, PPI_PluginEvent pluginEvent, PPI_bool* pluginGeneratesEvent);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginRegisterNotificationHandler
///
/// @brief Function to set a Notification handler
///
/// @note This may be called before the plugin is initialized.
///
/// This function allows a client to register a notification handler; unlike most functions, this one can be called *before* a plugin is instantiated,
/// though no information will be posted until the PPI_PluginInitialize operation is called. Although optional, it is highly encouraged to register
/// prior to calling PPI_PluginInitialize to capture all notifications that occur during and immediately after Plugin Initialization.
/// @param [in] notificationHandlerFunction The function the plugin should call when notification is needed. Note: we recommend that these callbacks do not call into the API as error messages, etc. are more difficult to handle in this case.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginRegisterNotificationHandler(PluginNotificationCallbackHandler notificationHandlerFunction);
typedef OpenIPC_Error (* PPI_PluginRegisterNotificationHandler_TYPE)(PluginNotificationCallbackHandler notificationHandlerFunction);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetLogEventHandler
///
/// @brief Function to set a log callback function
///
/// @note This may be called before the plugin is initialized.
///
/// This function allows a client to register a logger; unlike most functions, this one can be called *before* a plugin is instantiated,
/// though no information will be posted until the PPI_PluginInitialize operation is called. Although optional, it is highly encouraged to register
/// prior to calling PPI_PluginInitialize to capture all logs that occur during and immediately after Plugin Initialization.
/// @param [in] logCallBackFunction The function the plugin should call when logging is needed.  Note: we recommend that these callbacks do not call into the API as error messages, etc. are more difficult to handle in this case.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginSetLogEventHandler(PluginLogCallbackHandler logCallBackFunction);
typedef OpenIPC_Error (* PPI_PluginSetLogEventHandler_TYPE)(PluginLogCallbackHandler logCallBackFunction);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetStreamLogEventHandler
///
/// @brief Function to set a log callback function that comprehends different streams
///
/// @note This may be called before the plugin is initialized.
///
/// This function allows a client to register a logger; unlike most functions, this one can be called *before* a plugin is instantiated,
/// though no information will be posted until the PPI_PluginInitialize operation is called. Although optional, it is highly encouraged to register
/// prior to calling PPI_PluginInitialize to capture all logs that occur during and immediately after Plugin Initialization.
/// @param [in] logCallBackFunction The function the plugin should call when logging is needed.  Note: we recommend that these callbacks do not call into the API as error messages, etc. are more difficult to handle in this case.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginSetStreamLogEventHandler(PluginStreamLogCallbackHandler logCallBackFunction);
typedef OpenIPC_Error (* PPI_PluginSetStreamLogEventHandler_TYPE)(PluginStreamLogCallbackHandler logCallBackFunction);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetNotificationRequiredForLogCallBack
///
/// @brief This function provides a way to instruct the plugin to mask log messages below a certain notification level.
///        If this function is not provided, the default is to report no log messages.
/// @param [in] level The notification level and more urgent messages that should be returned. For example, PPI_errorNotification indicates that only errors should be returned.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval  IPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginSetNotificationRequiredForLogCallBack(PPI_NotificationLevel level);
typedef OpenIPC_Error (* PPI_PluginSetNotificationRequiredForLogCallBack_TYPE)(PPI_NotificationLevel level);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginUsesLoggerStream
///
/// @brief This function provides the ability to query if a particular logging stream is used by a plugin.
///
/// @param [in] stream The stream to query.
/// @param [out] usesLoggerStream Is True iff the plugin uses this stream.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginUsesLoggerStream(PPI_Stream stream, PPI_bool* usesLoggerStream);
typedef OpenIPC_Error (* PPI_PluginUsesLoggerStream_TYPE)(PPI_Stream stream, PPI_bool* usesLoggerStream);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetNotificationRequiredForStreamLogCallBack
///
/// @brief This function provides a way to instruct the plugin to mask log messages below a certain notification level for a given stream.
///        If this function is not provided, the default is to report no log messages.
/// @param [in] level The notification level and more urgent messages that should be returned. For example, PPI_errorNotification indicates that only errors should be returned.
/// @param [in] stream The stream to apply this filter to.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval  IPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginSetNotificationRequiredForStreamLogCallBack(PPI_Stream stream, PPI_NotificationLevel level);
typedef OpenIPC_Error (* PPI_PluginSetNotificationRequiredForStreamLogCallBack_TYPE)(PPI_Stream stream, PPI_NotificationLevel level);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetNotificationRequiredForNotificationCallBack
///
/// @brief This function provides a way to instruct the plugin to mask notification messages below a certain notification level.
///        If this function is not provided, the default is to report all notification messages.
/// @param [in] level The notification level and more urgent messages that should be returned. For example, PPI_errorNotification indicates that only errors should be returned.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval  IPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_PluginSetNotificationRequiredForNotificationCallBack(PPI_NotificationLevel level);
typedef OpenIPC_Error (* PPI_PluginSetNotificationRequiredForNotificationCallBack_TYPE)(PPI_NotificationLevel level);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginGetLastError
///
/// @brief Function to obtain more information on the last error (with specific error information).
///        The contents may not be preserved if any other operation call (e.g., not PPI_GetLastError) is made to the plugin even
///        if the other operation does not return an error.
///
/// @param [in] error The error code obtained from the last call.
/// @return (const char*) The error message.
///
PROBEPLUGIN_API const char* PPI_PluginGetLastError(OpenIPC_Error error);
typedef const char* (* PPI_PluginGetLastError_TYPE)(OpenIPC_Error error);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_GetRemoteInformation
///
/// @brief Function to obtain information about the probe plugin implementation. Basically, this allows a probe plugin to be a good citizen and report if it is using a remote connection and if that
///        connection is secure.
/// @param [out] connectionInformation populated by the plugin with info regarding if this plugin is secure and/or remote.
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval  IPC_Error_No_Error Indicates success.
///
PROBEPLUGIN_API OpenIPC_Error PPI_GetRemoteInformation(PPI_RemoteConnectionInformation* connectionInformation);
typedef OpenIPC_Error (* PPI_GetRemoteInformation_TYPE)(PPI_RemoteConnectionInformation* connectionInformation);

#if defined(__cplusplus)
    } // extern "C"
#endif

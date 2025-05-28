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
///  @brief Contains methods for setting trigger responses.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ProbePlugin.h"
#include "PinOperations.h"

#if defined(__cplusplus)
    extern "C" {
#endif

/// @brief The main opaque type for denoting any given trigger-response
///
///  The model for a trigger-response is a temporal implication: a pin state
///  occurrence implies another pin state occurrence (either positive or negative);
///  timing is not explicit in this API, but may be configured directly on the probe.
///
///  State-wise, this is a tuple of:
///  trigger : PPI_Pins_TypeEncode
///  triggerOnAssert : PPI_bool
///  response : PPI_Pins_TypeEncode
///  respondAsAssert : PPI_bool
///  identifier : uint32_t
///
///  This is treated as an opaque data type, i.e., a client cannot know how it is implemented internally,
///  only observe what happens when interacting via API functions.
///  The functions that can be called to see what this trigger response includes are here:
///  PPI_TriggerGetTrigger
///  PPI_TriggerGetResponse
///
///  Each trigger-response can be enabled or disabled. Some of these may be observable "directly" through the probe.
///  An example of this is a full-featured probe plugin that interacts with an agent that can perform JTAG scans.
///  In order to support this use model, there is additional set of triggers that are observable.
///  Observable trigger cause an event to fire when they occur; this event encodes which trigger-response has happened;
///  The identifier (which is unique within an interface) is used as the event payload.
typedef struct PPI_InterfaceTriggerResponse PPI_InterfaceTriggerResponse;

// Function for interacting with the trigger-response

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_TriggerGetTrigger
//
/// @brief This function queries a trigger-response for the trigger pin
///
/// @param [in] triggerResponse The trigger-response as provided by this component.
/// @param [out] pin Populated with the pin encoding of the trigger.
/// @param [out] onAssert Populated with the polarity of the trigger
/// @param [out] identifier Populated with the identifier of this trigger-response, this identifier is used to determine in a PPI_triggerResponse event which trigger-response has occurred.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Parameter if triggerResponse is not valid
/// @retval OpenIPC_Error_Null_Pointer if pin is null or onAssert is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_TriggerGetTrigger(const PPI_InterfaceTriggerResponse* triggerResponse, PPI_Pins_TypeEncode* pin, PPI_bool* onAssert, uint32_t* identifier);
typedef OpenIPC_Error (* PPI_TriggerGetTrigger_TYPE)(const PPI_InterfaceTriggerResponse* triggerResponse, PPI_Pins_TypeEncode* pin, PPI_bool* onAssert, uint32_t* identifier);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_TriggerGetResponse
//
/// @brief This function queries a trigger-response for the response pin
///
/// @param [in] triggerResponse The trigger-response as provided by this component.
/// @param [out] pin Populated with the pin encoding of the response.
/// @param [out] respondAsAssert Populated with the polarity of the response (either assertion or deassertion)
/// @param [out] identifier Populated with the identifier of this trigger-response, this identifier is used to determine in a PPI_triggerResponse event which trigger-response has occurred.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Parameter if triggerResponse is not valid
/// @retval OpenIPC_Error_Null_Pointer if pin is null or onAssert is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_TriggerGetResponse(const PPI_InterfaceTriggerResponse* triggerResponse, PPI_Pins_TypeEncode* pin, PPI_bool* respondAsAssert, uint32_t* identifier);
typedef OpenIPC_Error (* PPI_TriggerGetResponse_TYPE)(const PPI_InterfaceTriggerResponse* triggerResponse, PPI_Pins_TypeEncode* pin, PPI_bool* respondAsAssert, uint32_t* identifier);

// Functions for managing trigger-responses supported by the probe.

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceListTriggerResponses
//
/// @brief Allows a client to determine the TriggerResponse sequences implemented by a probe interface.
///
/// The mental model for this function is that it allows for a client to obtain pointers to static TriggerResponse objects
/// that the plugin provides. These trigger responses are expected to be static per probe/interface. The expected implementation
/// of the plugin is to simply provide copies to its trigger structures or whatever is needed as part of PPI_InterfaceTriggerResponse
/// to identify the trigger.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] triggerResponseSupportedLength The number of entries in triggerResponseSupported; if this equals 0 then the number of entries should be written into triggerResponsePopulated.
/// @param [out] triggerResponseSupported The trigger response types that are supported by device. This is an array with a size equal to triggerResponseSupportedLength passed in. The implementer owns the PPI_InterfaceTriggerResponse instances, thus the client need not manage that memory.
/// @param [out] triggerResponsePopulated The number of trigger responses written; if this number equals the listLength, this should be recalled with a larger triggerResponseSupportedLength
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support trigger responses
/// @retval OpenIPC_Error_Null_Pointer if triggerResponsePopulated is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceListTriggerResponses(OpenIPC_DeviceId device, uint32_t triggerResponseSupportedLength, PPI_InterfaceTriggerResponse** triggerResponseSupported, uint32_t* triggerResponsePopulated);
typedef OpenIPC_Error (* PPI_InterfaceListTriggerResponses_TYPE)(OpenIPC_DeviceId device, uint32_t triggerResponseSupportedLength, PPI_InterfaceTriggerResponse** triggerResponseSupported, uint32_t* triggerResponsePopulated);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceListObservableTriggerResponses
//
/// @brief Allows a client to determine which trigger response occurrences the probe interface can observe.
///
/// The mental model for this function is that it allows for a client to obtain pointers to static TriggerResponse objects
/// that the plugin provides. These trigger responses are expected to be static per probe/interface. The expected implementation
/// of the plugin is to simply provide copies to its trigger structures or whatever is needed as part of PPI_InterfaceTriggerResponse
/// to identify the trigger.
///
/// For many types of probes, the ability to determine that a trigger response has occurred
/// depends on the ability to query the target at a fundamentally deep level, so the probe
/// plugin cannot directly observe this (though clients of this plugin may be using JTAG to
/// determine the state of the target indirectly). For other probe types, the probe itself
/// provides a direct mechanism to observe that a trigger has occurred (for example, if the probe
/// has built-in knowledge of the target that it is connected to and can master JTAG without control
/// of the probe plugin). This function provides a mechanism for
/// a client to determine which trigger occurrences can be detected by this probe/interface.
/// Note: a client can assume that the result of this list has the same or fewer items
/// than PPI_InterfaceListTriggerResponse.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] triggerResponseSupportedLength The number of entries in triggerResponsesSupported, this should be bigger than 0.
/// @param [out] triggerResponseSupported The trigger response types that are supported by device. This is an array with a size equal to triggerResponseSupportedLength passed in. The implementer owns the PPI_InterfaceTriggerResponse instances, thus the client need not manage that memory.
/// @param [out] triggerResponsePopulated The number of trigger response written; if this number equals the listLength, this should be recalled with a larger triggerResponseSupportedLength
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support trigger responses
/// @retval OpenIPC_Error_Null_Pointer if any of the pointers are null
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceListObservableTriggerResponses(OpenIPC_DeviceId device, uint32_t triggerResponseSupportedLength, PPI_InterfaceTriggerResponse** triggerResponseSupported, uint32_t* triggerResponsePopulated);
typedef OpenIPC_Error (* PPI_InterfaceListObservableTriggerResponses_TYPE)(OpenIPC_DeviceId device, uint32_t triggerResponseSupportedLength, PPI_InterfaceTriggerResponse** triggerResponseSupported, uint32_t* triggerResponsePopulated);

/// @brief This enum indicates if a trigger response is to be observed (if observable). Otherwise this value has no effect.
DECLARE_ENUM(PPI_TRIGGER_RESPONSE_OPTIONS_ET)
{
    PPI_Trigger_DEFAULT = 0x0,                            /// The default is to not ignore an observable trigger-response.
    PPI_Trigger_Ignore_Observable_Trigger_Response = 0x1, /// One can selectively ignore an observable trigger-response
};

/// @brief Supports optional arguments to the PPI_InterfaceTriggerResponseIsSet and PPI_InterfaceTriggerResponseSet functions.
typedef struct
{
    /// Default is PPI_Trigger_DEFAULT
    PPI_TRIGGER_RESPONSE_OPTIONS_ET options;
} PPI_TriggerResponseSetOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTriggerResponseIsSet
//
/// @brief This function queries to see if a trigger response has been set.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] triggerResponse This is the trigger response to configure
/// @param [out] triggerResponseSet Equivalent to if the trigger response is armed in the probe
/// @param [out] optionsSet If this is non-null, it is populated with the current options for this trigger.
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support trigger responses
/// @retval OpenIPC_Error_Null_Pointer if any of the pointers are null
/// @retval OpenIPC_Error_Invalid_Parameter if triggerResponse does not point to a valid triggerResponse
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTriggerResponseIsSet(OpenIPC_DeviceId device, const PPI_InterfaceTriggerResponse* triggerResponse, PPI_bool* triggerResponseSet, PPI_TriggerResponseSetOptions* optionsSet);
typedef OpenIPC_Error (* PPI_InterfaceTriggerResponseIsSet_TYPE)(OpenIPC_DeviceId device, const PPI_InterfaceTriggerResponse* triggerResponse, PPI_bool* triggerResponseSet, PPI_TriggerResponseSetOptions* optionsSet);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTriggerResponseIsSet
//
/// @brief This function sets a trigger response to be either enabled or disabled
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] triggerResponse This is the trigger response to configure
/// @param [in] triggerResponseSet Sets or clears the trigger response (e.g., triggerResponseSet iff triggerResponse is enabled in the probe plugin after this call.) If the trigger response is set *and* the break has been hit, this will rearm (and release) the break.
/// @param [in] optionsSet If this is non-null, the options specified in the structure are used, otherwise defaults are used.
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support trigger responses
/// @retval OpenIPC_Error_Null_Pointer if any of the pointers are null
/// @retval OpenIPC_Error_Invalid_Parameter if triggerResponse does not point to a valid triggerResponse
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTriggerResponseSet(OpenIPC_DeviceId device, PPI_InterfaceTriggerResponse* triggerResponse, PPI_bool triggerResponseSet, const PPI_TriggerResponseSetOptions* optionsSet);
typedef OpenIPC_Error (* PPI_InterfaceTriggerResponseSet_TYPE)(OpenIPC_DeviceId device, PPI_InterfaceTriggerResponse* triggerResponse, PPI_bool triggerResponseSet, const PPI_TriggerResponseSetOptions* optionsSet);

#if defined(__cplusplus)
    } // extern "C"
#endif

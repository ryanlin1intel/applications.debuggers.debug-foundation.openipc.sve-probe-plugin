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

#if defined(__cplusplus)
    extern "C" {
#endif

//
//  The model of an action is a capability that is advertised by a plugin.
//      This capability is specific to a probe/interface within a plugin and can be run at any time.
//      Parameters are not directly supported, but are indirectly supported with the Device*Config operations.
//      After an action is run, any messages can be accessed (though these are not designed to be machine parseable).
//
//  The lifetime for any of the returned PPI_Action pointers is for the lifetime of an interface. Basically, the
//  actions pointer must be valid until an interface is deinitialized. Additionally, after any interface initialization, a client
//  must call PPI_DeviceListActions to determine the valid actions for that interface.
//

typedef struct PPI_Action PPI_Action;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceListActions
//
/// @brief PPI_DeviceListActions provides a mechanism to list all of the actions that a particular device has available.
///
/// @param [in] deviceID The device id to query.
/// @param [in] actionsLength The number of entries in the @a actions buffer
/// @param [out] actions The produced list of actions. This can be NULL. Only the buffer's memory is owned by the client, everything else is owned by the plugin and has a valid lifetime until the interface is deinitialized.
/// @param [out] numberOfActions The total number of actions for this device. This will always be written and cannot be NULL.
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly
///
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceListActions(OpenIPC_DeviceId deviceID, uint32_t actionsLength, PPI_Action** actions, uint32_t* numberOfActions);
typedef OpenIPC_Error (* PPI_DeviceListActions_TYPE)(OpenIPC_DeviceId deviceID, uint32_t actionsLength, PPI_Action** actions, uint32_t* numberOfActions);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ActionGetName
//
/// @brief PPI_ActionGetName provides a mechanism to determine the name of a particular action (provided by the plugin).
///
/// @param [in] action The action to query.
/// @param [in] valueLength The number of characters in the @a value buffer.
/// @param [out] value The buffer to be in the description.
///
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly
///
PROBEPLUGIN_API OpenIPC_Error PPI_ActionGetName(const PPI_Action* action, uint32_t valueLength, PPI_char* value);
typedef OpenIPC_Error (* PPI_ActionGetName_TYPE)(const PPI_Action* action, uint32_t valueLength, PPI_char* value);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ActionGetDescription
//
/// @brief PPI_ActionGetDescription provides a mechanism to determine the description of a particular action (provided by the plugin).
///
/// @param [in] action The action to query.
/// @param [in] valueLength The number of characters in the @a value buffer.
/// @param [out] value The buffer to be in the description.
///
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly
///
PROBEPLUGIN_API OpenIPC_Error PPI_ActionGetDescription(const PPI_Action* action, uint32_t valueLength, PPI_char* value);
typedef OpenIPC_Error (* PPI_ActionGetDescription_TYPE)(const PPI_Action* action, uint32_t valueLength, PPI_char* value);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ActionSetArgument
//
/// @brief PPI_ActionSetArgument provides a mechanism to set the parameter of an action. By default, this is set to a NULL pointer.
///
/// @param [in] action The action to set the parameter.
/// @param [in] value The null-terminated buffer.
///
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly
///
PROBEPLUGIN_API OpenIPC_Error PPI_ActionSetArgument(PPI_Action* action, const PPI_char* value);
typedef OpenIPC_Error (* PPI_ActionSetArgument_TYPE)(PPI_Action* action, const PPI_char* value);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_DeviceActionRun
//
/// @brief PPI_DeviceActionRun runs an action
///
/// @param [in] deviceID The device id to query.
/// @param [in] action The action to run on a device.
/// @param [in] messageLength The length of the buffer pointed to by message in bytes.
/// @param [out] message The buffer to be filled in with any messages generated by the action.
///
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly and the action ran successfully
///
PROBEPLUGIN_API OpenIPC_Error PPI_DeviceActionRun(OpenIPC_DeviceId deviceID, const PPI_Action* action, uint32_t messageLength, PPI_char* message);
typedef OpenIPC_Error (* PPI_DeviceActionRun_TYPE)(OpenIPC_DeviceId deviceID, const PPI_Action* action, uint32_t messageLength, PPI_char* message);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_ActionIsVisible
//
/// @brief PPI_ActionIsVisible Queries an action to see if it should be visible to clients
///
/// @param [in] action The action to query.
/// @param [out] isVisible Is true iff the action should be listed for clients.
///
/// @return OpenIPC_Error
/// @retval OpenIPC_Error_No_Error The out parameters were populated correctly and the action ran successfully
///
PROBEPLUGIN_API OpenIPC_Error PPI_ActionIsVisible(const PPI_Action* action, PPI_bool* isVisible);
typedef OpenIPC_Error (* PPI_ActionIsVisible_TYPE)(const PPI_Action* action, PPI_bool* isVisible);

#if defined(__cplusplus)
    } // extern "C"
#endif

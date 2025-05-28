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
///  @brief Contains Enumeration Declarations for JTAG-based triggering functions.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "JTAGDefinitions.h"

#if defined(__cplusplus)
    extern "C" {
#endif

///
/// @brief struct to hold JTAG trigger options.
///
typedef struct
{
    /** This indicates that the probe should not rearm the triggered scan. By default, this is false. */
    PPI_bool doNotRearm;
} PPI_JTAG_TriggerOptions;

///
/// This is the definition of the triggerOccuredCallBack that is called when a trigger occurs.
/// @param occurrenceCount the number of times this trigger has been executed since the last callback.
typedef void (* triggerOccuredCallBack)(uint64_t occurrenceCount);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_RegisterTrigger
///
/// Registers a bundle to be executed on a particular trigger occurrence.
///
/// Registers a bundle to be executed on a particular trigger occurrence with respect to an interface.
/// @param [in,out] handle Can be a bundle handle; LOCK and RELEASE are not allowed.
/// @param [in] jtagInterface A handle to a JTAG interface to run the operation on
/// @param [in] callback A call-back to indicate that a trigger occurred (with number of times included) and a pointer to the handle (for retriggering if needed).
/// @param [in] minimumTimeBetweenCallbacks_us is the minimum time between two consecutive callbacks.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @param [in] trigger The trigger that this bundle should be executed on.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle or it is a LOCK or RELEASE
/// @retval OpenIPC_Error_Probe_Invalid_JTAG_Device if jtagInterface is not a valid JTAG interface
/// @retval OpenIPC_Error_Probe_Invalid_Parameter if callback is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_RegisterTrigger(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId jtagInterface, PPI_JTAG_TRIGGER_TYPE_ET trigger, triggerOccuredCallBack callback, uint64_t minimumTimeBetweenCallbacks_us, const PPI_JTAG_TriggerOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_RegisterTrigger_TYPE)(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId jtagInterface, PPI_JTAG_TRIGGER_TYPE_ET trigger, triggerOccuredCallBack callback, uint64_t minimumTimeBetweenCallbacks_us, const PPI_JTAG_TriggerOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_UnregisterTrigger
///
/// Unregisters a bundle to executed on a trigger (that has already been scheduled).
///
/// Unregisters a bundle to executed on a trigger (that has already been scheduled).
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_UnregisterTrigger(PPI_ProbeBundleHandle handle);
typedef OpenIPC_Error (* PPI_JTAG_UnregisterTrigger_TYPE)(PPI_ProbeBundleHandle handle);

#if defined(__cplusplus)
    } // extern "C"
#endif

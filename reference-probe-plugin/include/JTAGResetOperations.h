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
///  @brief Contains Bundle based JTAG functions for modifying JTAG parameters.
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
/// @brief Structure to hold any additional JTAG parameters required.
///
typedef struct
{
    /// Do not use! Will be replaced with another field in subsequent versions of the API.
    uint32_t reserved;
} PPI_JTAG_ParametersOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_SetParameters
///
/// @brief Sets parameters of the JTAG interface in the context of a bundle handle.
///
/// Sets parameters of the JTAG interface in the context of a bundle handle.
/// @param [in] jtagInterface A handle to a JTAG interface to run the operation on
/// @param [in] numberOfClocksInTlr The number of clocks to remain in TLR
/// @param [in] mechanismToEnterTLR Indicates how to enter TRST (via pin or state machine).
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_SetParameters(OpenIPC_DeviceId jtagInterface, uint64_t numberOfClocksInTlr, PPI_JTAG_RESET_OPTIONS_ET mechanismToEnterTLR, PPI_JTAG_ParametersOptions* options);
typedef OpenIPC_Error (* PPI_JTAG_SetParameters_TYPE)(OpenIPC_DeviceId jtagInterface, uint64_t numberOfClocksInTlr, PPI_JTAG_RESET_OPTIONS_ET mechanismToEnterTLR, PPI_JTAG_ParametersOptions* options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_GetParameters
///
/// @brief Gets parameters of the JTAG interface in the context of a bundle handle.
///
/// Gets parameters of the JTAG interface in the context of a bundle handle.
/// @param [in] jtagInterface A handle to a JTAG interface to run the operation on
/// @param [out] numberOfClocksInTlr The number of clocks to remain in TLR.
/// @param [out] mechanismToEnterTLR Indicates how to enter TRST (via pin or state machine). Default is by the state machine.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
/// @retval OpenIPC_Error_Probe_Invalid_Parameter if number_of_clocks_in_trst, mechanismToEnterTLR or options is null
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_GetParameters(OpenIPC_DeviceId jtagInterface, uint64_t* numberOfClocksInTlr, PPI_JTAG_RESET_OPTIONS_ET* mechanismToEnterTLR, PPI_JTAG_ParametersOptions* options);
typedef OpenIPC_Error (* PPI_JTAG_GetParameters_TYPE)(OpenIPC_DeviceId jtagInterface, uint64_t* numberOfClocksInTlr, PPI_JTAG_RESET_OPTIONS_ET* mechanismToEnterTLR, PPI_JTAG_ParametersOptions* options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_EnterTLR
///
/// @brief Resets JTAG state machine (immediately, or later as part of a bundle).
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_EnterTLR(PPI_ProbeBundleHandle handle);
typedef OpenIPC_Error (* PPI_JTAG_EnterTLR_TYPE)(PPI_ProbeBundleHandle handle);

/// Delay options for waiting on a trigger instead of a delay
DECLARE_ENUM(PPI_Delay_Options_ET) {
    Delay_Option_Default = 0x000,                   // Indicates that the delay operation should perform a hard wait
    Delay_Option_Wait_For_Trigger = 0x001,          // Indicates that the delay operation should wait for a trigger and error after the delay time if the trigger does not occur
    Delay_Option_Wait_For_Trigger_Or_Delay = 0x002, // Indicates that the delay operation should wait for a trigger or delay, whichever comes first
};

///
/// @brief Options for JTAG delay operations.
///
typedef struct
{
    PPI_Delay_Options_ET triggerOption; /// @brief Provides a mechanism to indicate if waiting for a trigger is desired.
    PPI_JTAG_TRIGGER_TYPE_ET trigger;   /// @brief Provides a machenaism to indicate which trigger to wait for (only valid it triggerOptions indicates waiting for a trigger).
} PPI_JTAG_DelayOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_Delay
///
/// @brief  Adds a delay to be executed in a bundle. The delay behavior will differ depending on probe capabilities. On those that can stop TCK, it will wait for numberOfCLocks / TCK.
///         Otherwise, TMS and TDI are maintained while TCK is toggled. This behavior is indicated by the JTAG capabilities structure. The options structure provides parameters to provide
///         a mechanism to wait for a trigger instead (and error out after a delay).
///
/// Adds a delay to be executed in a bundle; this delay is in terms of the number of TCKs clocks to wait.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] numberOfClocks The number of TCKs to wait
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_Delay(PPI_ProbeBundleHandle handle, uint32_t numberOfClocks, PPI_JTAG_DelayOptions* options);
typedef OpenIPC_Error (* PPI_JTAG_Delay_TYPE)(PPI_ProbeBundleHandle handle, uint32_t numberOfClocks, PPI_JTAG_DelayOptions* options);

#if defined(__cplusplus)
    } // extern "C"
#endif

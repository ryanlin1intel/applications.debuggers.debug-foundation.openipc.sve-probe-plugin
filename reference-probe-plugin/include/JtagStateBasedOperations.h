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
///  @brief Contains Functions for bundle-based JTAG functions that use a State-based model.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "JTAGResetOperations.h"

#if defined(__cplusplus)
    extern "C" {
#endif

///
/// @brief Structure to hold any additional JTAG Shift State parameters required.
///
typedef struct
{
    /// Default is JtagOption_TDI_Default.
    PPI_JTAG_TDI_TDO_OPTIONS_ET TdiTdoOptions;
    /// Sets the slot to use on a save or load. Default is PPI_SLOT_HANDLE_INVALID
    PPI_SlotHandle savedSlot;
} PPI_JTAG_StateShiftOptions;

///
/// @brief Structure to hold any additional JTAG Goto State parameters required.
///
typedef struct
{
    /// This indicates if we should wait for a trigger after arriving at the specified state. By default, this is false.
    PPI_bool waitForTrigger;
    /// This indicates that the waitForTrigger should error out. By default, this is false.
    /// Logically the InterfaceConfig string "Jtag.TriggerScanExitOnTimeout" is "or'd" with this value.
    PPI_bool errorOnTimeout;
} PPI_JTAG_StateGotoOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_GoToState
///
/// @brief Perform a goto state jtag operation (immediately, or later as part of a bundle).
///
/// Perform a goto state jtag operation (immediately, or later as part of a bundle).
/// @param handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] gotoState The JTAG state to shift to
/// @param [in] numberOfClocksInState The number of clocks to stay in a destination state (only valid for stable states)
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_GoToState(PPI_ProbeBundleHandle handle, JtagStateEncode gotoState, uint32_t numberOfClocksInState, const PPI_JTAG_StateGotoOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_GoToState_TYPE)(PPI_ProbeBundleHandle handle, JtagStateEncode gotoState, uint32_t numberOfClocksInState, const PPI_JTAG_StateGotoOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_StateIRShift
///
/// @brief Performs a transition to the ShIR state.
///
/// Performs a transition to the ShIR state and writes the given data; the ending state is in Ex1IR and is transitioned on the last TCK.
/// The case of back-to-back IR shifts requires that the JTAG state machine sequence transitions through Update-IR between the shifts.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] shiftLengthBits The length of the IR shift.
/// @param [in] inBits A buffer of data to be sent in TDI. This is consumed by the call (so it is safe to re-use). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDI is pulled from a slot.
/// @param [out] outBits A buffer to save the TDO write-back. This is produced by the bundle execution (so it is not safe to reuse). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDO is saved to a slot.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_StateIRShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_StateIRShift_TYPE)(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_StateDRShift
/// @brief Performs a transition to the ShDR state.
///
/// Performs a transition to the ShDR state and writes the given data; the ending state is in Ex1IR and is transitioned on the last TCK.
/// The case of back-to-back DR shifts requires that the JTAG state machine sequence transitions through Update-DR between the shifts.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] shiftLengthBits The length of the DR shift.
/// @param [in] inBits A buffer of data to be sent in TDI. This is consumed by the call (so it is safe to re-use). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDI is pulled from a slot.
/// @param [out] outBits A buffer to save the TDO write-back. This is produced by the bundle execution (so it is not safe to reuse). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDO is saved to a slot.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_StateDRShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_StateDRShift_TYPE)(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_StateShiftOptions* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

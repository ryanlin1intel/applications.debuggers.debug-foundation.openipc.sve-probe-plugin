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
///  @brief Contains Declarations for JTAG-based functions that use a Register based interface.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "JTAGResetOperations.h"

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_RegisterShiftCapabilities
///
/// @brief Allows for a capabilities query of the Register JTAG capabilities.
///
/// Allows for a capabilities query of the Register JTAG capabilities.
/// @param [in] jtagInterface A handle to a JTAG interface to run the operation on
/// @param [out] supportPauseStates Results of the query to indicate if the operation supports the Pause states. This can be NULL (but little will happen in that case).
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_RegisterShiftCapabilities(OpenIPC_DeviceId jtagInterface, PPI_bool* supportPauseStates);
typedef OpenIPC_Error (* PPI_JTAG_RegisterShiftCapabilities_TYPE)(OpenIPC_DeviceId jtagInterface, PPI_bool* supportPauseStates);

///
/// @brief struct to hold JTAG register based options.
///
typedef struct
{
    /// Default is: JtagOption_TDI_Default.
    PPI_JTAG_TDI_TDO_OPTIONS_ET tdiTdoOptions;
    /// Sets the slot to use on a save or load. Default is PPI_SLOT_HANDLE_INVALID
    PPI_SlotHandle savedSlot;
    /// Indicates that the implementation should stop in Pause, instead of RTI. Default is False
    PPI_bool stopInPauseNotRunTestIdle;
} PPI_JTAG_RegisterOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_IRRegisterShift
///
/// @brief Perform a jtag operation (immediately, or later as part of a bundle).
///
/// Performs a transition from RTI (or a Pause state) to ShIR, writes in_bits, then transitions to either RTI or Pause-IR.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] shiftLengthBits The length of the IR shift.
/// @param [in] inBits A buffer of data to be sent in TDI. This is consumed by the call (so it is safe to re-use). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDI is pulled from a slot.
/// @param [out] outBits A buffer to save the TDO write-back. This may be NULL, in that case no data is returned to the client. The caller must allocate sufficient space in tdo otherwise and the tdo buffer will not contain valid data until after the bundle is executed.  The TdiTdoOptions field also governs if the TDO is saved to a slot.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_IRRegisterShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_RegisterOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_IRRegisterShift_TYPE)(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_RegisterOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_DRRegisterShift
///
/// @brief Perform a jtag operation (immediately, or later as part of a bundle).
///
/// Performs a transition from RTI (or a Pause state) to ShDR, writes in_bits, then transitions to either RTI or Pause-IR.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] shiftLengthBits The length of the DR shift.
/// @param [in] inBits A buffer of data to be sent in TDI. This is consumed by the call (so it is safe to re-use). This may be NULL, in which case, the options structure defines the data shifted in (by the TdiTdoOptions field). The TdiTdoOptions field also governs if the TDI is pulled from a slot.
/// @param [out] outBits A buffer to save the TDO write-back. This may be NULL, in that case no data is returned to the client. The caller must allocate sufficient space in tdo otherwise and the tdo buffer will not contain valid data until after the bundle is executed.  The TdiTdoOptions field also governs if the TDO is saved to a slot.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_DRRegisterShift(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_RegisterOptions* const options);
typedef OpenIPC_Error (* PPI_JTAG_DRRegisterShift_TYPE)(PPI_ProbeBundleHandle handle, uint32_t shiftLengthBits, const uint8_t* const inBits, uint8_t* outBits, const PPI_JTAG_RegisterOptions* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

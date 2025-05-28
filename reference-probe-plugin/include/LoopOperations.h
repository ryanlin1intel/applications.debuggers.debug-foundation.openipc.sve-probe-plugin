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
///  @brief Contains Functions for using the looping service with bundles.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BundleOperations.h"

#if defined(__cplusplus)
    extern "C" {
#endif

///
/// @brief Structure to hold any additional loop parameters required.
///
typedef struct
{
    /// This indicates if the loop should continue on executing new iterations on a timeout error; by default it is false.
    PPI_bool continueOnTimeoutError;
} PPI_Loop_LoopWithBreakOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetNotificationRequiredForStreamLogCallBack
///
/// @brief Constructs a Loop appended to the end of a given handle.
///
/// Constructs a Loop appended to the end of a handle, given a handle to a body.
/// This also breaks on any comparison success (and those buffers will always
/// have the last iteration worth of data).
/// This is not required to be implemented; clients are responsible for emulation of this feature if it's not.
/// Like slots and bundles, this is highly recommended for be implemented for full featured probes for optimal performance.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in,out] body The operations to execute as part of the loop body; body.has_been_run = True. This must be a true bundle handle
/// @param [in] maxNumberOfIterations The maximum number of iterations to run.
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval Probe_Invalid_Parameter if body is not a valid bundle handle.
///
PROBEPLUGIN_API OpenIPC_Error PPI_Loop_LoopBreakOnComparisonSuccess(PPI_ProbeBundleHandle handle, PPI_ProbeBundleHandle body, uint32_t maxNumberOfIterations, const PPI_Loop_LoopWithBreakOptions* const options);
typedef OpenIPC_Error (* PPI_Loop_LoopBreakOnComparisonSuccess_TYPE)(PPI_ProbeBundleHandle handle, PPI_ProbeBundleHandle body, uint32_t maxNumberOfIterations, const PPI_Loop_LoopWithBreakOptions* const options);

///
/// @brief Structure to hold any additional loop parameters required.
///
typedef struct
{
    /// Do not use! May be replaced with another field in subsequent versions of the API.
    uint32_t reserved;
} PPI_Loop_Options;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PluginSetNotificationRequiredForStreamLogCallBack
///
/// @brief Constructs a Loop appended to the end of a given handle.
///
/// Constructs a Loop appended to the end of a handle, given a handle to a body,
/// and some information on how to collect writeback.
/// This is not required to be implemented; clients are responsible for emulation of this feature if it's not.
/// Like slots and bundles, this is highly recommended for be implemented for full featured probes for optimal performance.
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in,out] body The operations to execute as part of the loop body; body.has_been_run = True.
/// @param [in] numberOfIterations The number of iterations to run.
/// @param [in] bufferLengthInBytes Length of the buffer passed in to store write-back. If it is too small, buffer will act as a circular one.
/// @param [out] bufferForIterations The buffer to store the captured writeback data; this data is bit packed and only includes data that is saved in a writeback buffer within the body (e.g., for JTAG, TDO is not null).
/// @param [out] currentBit The next bit to be written (so this is the oldest bit in the circular buffer).
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval Probe_Invalid_Parameter if body is not a valid bundle handle.
///
PROBEPLUGIN_API OpenIPC_Error PPI_Loop_CaptureAll(PPI_ProbeBundleHandle handle, PPI_ProbeBundleHandle body, uint32_t numberOfIterations, uint32_t bufferLengthInBytes, uint8_t* bufferForIterations, uint32_t* currentBit, const PPI_Loop_Options* const options);
typedef OpenIPC_Error (* PPI_Loop_CaptureAll_TYPE)(PPI_ProbeBundleHandle handle, PPI_ProbeBundleHandle body, uint32_t numberOfIterations, uint32_t bufferLengthInBytes, uint8_t* bufferForIterations, uint32_t* currentBit, const PPI_Loop_Options* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

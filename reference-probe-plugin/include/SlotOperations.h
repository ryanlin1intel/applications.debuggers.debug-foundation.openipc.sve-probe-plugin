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
///  @brief Contains Functions for creating/manipulating the slot instances.
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
/// This slot handle gives an ability to refer to a particular handle.
///
typedef void* PPI_SlotHandle;
//! An invalid slot handle
const PPI_SlotHandle PPI_SLOT_HANDLE_INVALID = (PPI_SlotHandle)0;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Slot_Allocate
///
/// @brief Allocates a slot for the plugin.
///
/// Allocates a slot for the probe; returns PPI_SLOT_HANDLE_INVALID if unable to.
/// This slot is valid as long as the bundle is, e.g., implicitly the slot is free'd after the bundle is released.
/// This is not required to be implemented; clients are responsible for emulation of slot behavior if this is the case. As with bundles,
/// for optimal performance on a full featured probe, this is highly recommended to be implemented.
/// @param handle the bundle to tie this slot to
/// @param bitSize the number of bits the slot should have.
/// @returns PPI_SLOT_HANDLE_INVALID iff unable to allocate a new bundle handle.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API PPI_SlotHandle PPI_Slot_Allocate(PPI_ProbeBundleHandle handle, uint64_t bitSize);
typedef PPI_SlotHandle (* PPI_Slot_Allocate_TYPE)(PPI_ProbeBundleHandle handle, uint64_t bitSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Slot_Free
///
/// @brief Frees a slot. Handle is no longer a valid slot.
///
/// Clears and Frees a bundle handle. Required if PPI_Slot_Allocate is implemented.
/// @param [in,out] handle The handle to free. It is set to PPI_SLOT_HANDLE_INVALID upon returning.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Slot_Free(PPI_SlotHandle* handle);
typedef OpenIPC_Error (* PPI_Slot_Free_TYPE)(PPI_SlotHandle* handle);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Slot_Size
///
/// @brief Allows for one to query the number of bits contained in a slot.
///
/// Gets the size of a slot. Required if PPI_Slot_Allocate is implemented.
/// @param [in] handle The handle to query.
/// @param [out] bitSize The number of bits in the handle.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Slot_Size(PPI_SlotHandle handle, uint32_t* bitSize);
typedef OpenIPC_Error (* PPI_Slot_Size_TYPE)(PPI_SlotHandle handle, uint32_t* bitSize);

///
/// @brief structure to hold any additional options needed for slot modifications.
///
typedef struct
{
    // Do not use! Will be replaced with another field in subsequent versions of the API.
    uint32_t reserved;
} PPI_Slot_ModificationOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Slot_Modification
///
/// @brief Performs a modification of data contained in a slot.
///
/// Performs a modification of data contained in a slot; the data in all slots is reset upon a release. Required if PPI_Slot_Allocate is implemented.
/// @param handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param savedSlot The saved slot number to modify
/// @param numberOfBits The length of the mask and valueToOrIn arrays (this must match the length of the bit string stored in the slot).
/// @param mask The mask to apply, passing in NULL implies a mask of all 0's (e.g., keep nothing).
/// @param valueToOrIn The value to 'or' with the slot's value, passing in NULL means to 'or' in 0.
/// @param options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle.
/// @retval OpenIPC_Error_Invalid_Slot if the slot is invalid
/// @retval OpenIPC_Error_Invalid_Slot_length if the numberOfBits does not match the slot length.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Slot_Modification(PPI_ProbeBundleHandle handle, PPI_SlotHandle savedSlot, uint32_t numberOfBits, const uint8_t* const mask, const uint8_t* const valueToOrIn, PPI_Slot_ModificationOptions* options);
typedef OpenIPC_Error (* PPI_Slot_Modification_TYPE)(PPI_ProbeBundleHandle handle, PPI_SlotHandle savedSlot, uint32_t numberOfBits, const uint8_t* const mask, const uint8_t* const valueToOrIn, PPI_Slot_ModificationOptions* options);

/// An enumeration for the types of comparisons
DECLARE_ENUM(PPI_Slot_COMPARISON_ET) {
    Comparison_Match_Any_Zeros = 0x000, /// Indicates that the match and mask field should be all 0's
    Comparison_Mask_Ones = 0x001,       /// Indicates that the mask field should be all 1's
    Comparison_Match_Any_Ones = 0x002   /// Indicates that the match field should be all 1's
};

///
/// The ComparisonOptions Data structure for passing in options to the SlotComparisonToConstant function.
///
typedef struct
{
    /// Option that holds the method of comparison (to 0's, or 1's). Default is Comparison_Match_Any_Zeros
    PPI_Slot_COMPARISON_ET option;
    /// Option to indicate if one should stop executing the bundle if the comparison fails. Default is False.
    PPI_bool exitBundleOnComparisonFailure;
} PPI_Slot_ComparisonOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Slot_ComparisonToConstant
///
/// Performs a comparison with values stored in a slot.
///
/// Performs a comparison with values stored in a slot; match, mask are
/// parameters. Required if PPI_Slot_Allocate is implemented.
/// @param handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param savedSlot The saved slot number to compare
/// @param numberOfBits The length of the slot and the length of tdo_mask/tdo_match variables if defined. Must be equal to the length in the slot.
/// @param mask A buffer for the mask of the saved data. If null this value is governed by options.option
/// @param match A buffer for the match of the saved data. If null this value is governed by options.option
/// @param comparisonResult is set to (savedSlot & mask) == (match & mask)
/// @param options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Slot if the number of bits specified in the call do not match the number in the slot.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Slot_ComparisonToConstant(PPI_ProbeBundleHandle handle, PPI_SlotHandle savedSlot, uint32_t numberOfBits, const uint8_t* const mask, const uint8_t* const match, PPI_bool* comparisonResult, const PPI_Slot_ComparisonOptions* const options);
typedef OpenIPC_Error (* PPI_Slot_ComparisonToConstant_TYPE)(PPI_ProbeBundleHandle handle, PPI_SlotHandle savedSlot, uint32_t numberOfBits, const uint8_t* const mask, const uint8_t* const match, PPI_bool* comparisonResult, const PPI_Slot_ComparisonOptions* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

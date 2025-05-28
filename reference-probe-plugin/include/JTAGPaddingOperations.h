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
///  @brief Contains Declarations for JTAG-based functions to modify the shift paddings (usable by the state and register-based JTAG).
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "JTAGDefinitions.h"

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_GetInterfacePadding
///
/// @brief Gets the padding on an interface, e.g., gets the padding at the point in the flow and populates the out-going parameters immediately.
///
/// Gets the pre/post padding on an interface (applicable to only register and state-based methods).
///
/// Note: when the JTAG interface is initialized, all padding bits *must* be set to 0.
///
/// @param [in] device Must be an interface that supports JTAG.
/// @param [out] irPaddingNearTDI Produces the number of padding IR bits between TDI and the TAP. Not invalid if NULL is given.
/// @param [out] irPaddingNearTDO Produces the number of padding IR bits between the TAP and TDO. Not invalid if NULL is given.
/// @param [out] drPaddingNearTDI Produces the number of padding DR bits between TDI and TAP. Not invalid if NULL is given.
/// @param [out] drPaddingNearTDO Produces the number of padding DR bits between the TAP and TDO. Not invalid if NULL is given.
/// @param [out] drValueConstantOne Produces the current DR padding bit value; if this is True we are using 1's, else we are using 0's.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_GetInterfacePadding(OpenIPC_DeviceId device, uint32_t* irPaddingNearTDI, uint32_t* irPaddingNearTDO, uint32_t* drPaddingNearTDI, uint32_t* drPaddingNearTDO, PPI_bool* drValueConstantOne);
typedef OpenIPC_Error (* PPI_JTAG_GetInterfacePadding_TYPE)(OpenIPC_DeviceId device, uint32_t* irPaddingNearTDI, uint32_t* irPaddingNearTDO, uint32_t* drPaddingNearTDI, uint32_t* drPaddingNearTDO, PPI_bool* drValueConstantOne);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_SetInterfacePadding
///
/// @brief Sets the padding on an interface, e.g., sets the padding for *every* scan after this until overridden.
///
/// Sets the pre/post padding on an interface (applicable to only register and state-based methods).
///
/// Note: when the JTAG interface is initialized, all padding bits *must* be set to 0.
///
/// @param [in,out] device Must be an interface that supports JTAG.
/// @param [in] irPaddingNearTDI The number of padding IR bits between TDI and the TAP.
/// @param [in] irPaddingNearTDO The number of padding IR bits between the TAP and TDO.
/// @param [in] drPaddingNearTDI The number of padding DR bits between TDI and TAP
/// @param [in] drPaddingNearTDO The number of padding DR bits between the TAP and TDO.
/// @param [in] drValueConstantOne If this is True, TDI will be padded with 1's, otherwise, 0's will be used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_SetInterfacePadding(OpenIPC_DeviceId device, uint32_t irPaddingNearTDI, uint32_t irPaddingNearTDO, uint32_t drPaddingNearTDI, uint32_t drPaddingNearTDO, PPI_bool drValueConstantOne);
typedef OpenIPC_Error (* PPI_JTAG_SetInterfacePadding_TYPE)(OpenIPC_DeviceId device, uint32_t irPaddingNearTDI, uint32_t irPaddingNearTDO, uint32_t drPaddingNearTDI, uint32_t drPaddingNearTDO, PPI_bool drValueConstantOne);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_GetCurrentBundlePadding
///
/// @brief Gets the delta of padding on an interface, e.g., gets the padding at the point in the bundle construction flow and populates the out-going parameters immediately.
///
/// Gets the pre/post padding on an interface (applicable to only register and state-based methods).
///
/// Note: Interface padding on interfaces is preserved across bundle execute operations. E.g., if bundle b has a net of 8 IR padding bits added, and interface i has 0 interface padding bits, then after executing b, i still has no padding bits.
///
/// @param [in] bundle Must be a current bundle with JTAG operations on it
/// @param [out] irPaddingNearTDI Produces the number of the delta padding IR bits between TDI and the TAP at the end of bundle. Not invalid if NULL is given.
/// @param [out] irPaddingNearTDO Produces the number of the delta padding IR bits between the TAP and TDO at the end of bundle. Not invalid if NULL is given.
/// @param [out] drPaddingNearTDI Produces the number of the delta padding DR bits between TDI and TAP at the end of bundle. Not invalid if NULL is given.
/// @param [out] drPaddingNearTDO Produces the number of the delta padding DR bits between the TAP and TDO at the end of bundle. Not invalid if NULL is given.
/// @param [out] drValueConstantOne Produces the current DR padding bit value at the end of bundle; if this is True we are using 1's, else we are using 0's.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_GetCurrentBundlePadding(PPI_ProbeBundleHandle bundle, int32_t* irPaddingNearTDI, int32_t* irPaddingNearTDO, int32_t* drPaddingNearTDI, int32_t* drPaddingNearTDO, PPI_bool* drValueConstantOne);
typedef OpenIPC_Error (* PPI_JTAG_GetCurrentBundlePadding_TYPE)(PPI_ProbeBundleHandle bundle, int32_t* irPaddingNearTDI, int32_t* irPaddingNearTDO, int32_t* drPaddingNearTDI, int32_t* drPaddingNearTDO, PPI_bool* drValueConstantOne);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_JTAG_UpdateBundlePadding
///
/// @brief updates the padding on an interface within the context of a bundle execution, e.g., sets the padding for *every* scan after this in the bundle until overridden.
///
/// Updates the pre/post padding on an interface (applicable to only register and state-based methods). Logically, every shift operation appended will have extra bits corresponding to the pre/post bits appended.
/// If the total number of padding bits is negative, it will act like the padding bits are 0.
///
/// Note: when the bundle terminates execution, the interface padding will be set to what it was before the bundle executed.
///
/// @param [in,out] bundle Bundle to include the update of the interface padding
/// @param [in] irPaddingNearTDI The delta of number of padding IR bits between TDI and the TAP.
/// @param [in] irPaddingNearTDO The delta of number of padding IR bits between the TAP and TDO.
/// @param [in] drPaddingNearTDI The delta of number of padding DR bits between TDI and TAP
/// @param [in] drPaddingNearTDO The delta of number of padding DR bits between the TAP and TDO.
/// @param [in] drValueConstantOne If this is True, TDI will be padded with 1's, otherwise, 0's will be used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Probe_JTAG_Device if jtagInterface is not a valid JTAG interface
///
PROBEPLUGIN_API OpenIPC_Error PPI_JTAG_UpdateBundlePadding(PPI_ProbeBundleHandle bundle, int32_t irPaddingNearTDI, int32_t irPaddingNearTDO, int32_t drPaddingNearTDI, int32_t drPaddingNearTDO, PPI_bool drValueConstantOne);
typedef OpenIPC_Error (* PPI_JTAG_UpdateBundlePadding_TYPE)(PPI_ProbeBundleHandle bundle, int32_t irPaddingNearTDI, int32_t irPaddingNearTDO, int32_t drPaddingNearTDI, int32_t drPaddingNearTDO, PPI_bool drValueConstantOne);

#if defined(__cplusplus)
    } // extern "C"
#endif

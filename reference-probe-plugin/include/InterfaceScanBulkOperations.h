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
///  @brief Contains Methods for bulk interface operations.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ProbePlugin.h>

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: InterfaceScan
///
/// @brief InterfaceScan is a transport like method for receiving blocks of data and returning blocks of data.
///
///     InterfaceScan is a transport like method for receiving blocks of data and returning blocks of data.
///     In the case of JTAG protocols the caller bundles JTAG data and provides it in the inputdword argument.
///
///     Return data from the JTAG scans is provided to the call in the outputdwords argument.
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] input
///     A byte array of input, formatted for the targeted interface.
/// @param[in] inputdwords
///     The number of dwords in the @a input parameter.
/// @param[out] output
///     A pointer to the location that the return data will be copied to; this may be NULL only if maxoutputdwords is 0.
/// @param[in] maxoutputdwords
///     The maximum number of dwords that can be safely copied to @a output.
/// @param[out] outputdwords
///     The number of dwords actually copied to @a output.
///
/// @return (IPC_ErrorCode): A code indicating success or failure.
/// @retval IPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The probe reference id @a probeID is not valid
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceScan(OpenIPC_DeviceId interfaceID, const uint32_t* input, uint32_t inputdwords, uint32_t* output, uint32_t maxoutputdwords, uint32_t* outputdwords);
typedef OpenIPC_Error (* PPI_InterfaceScan_TYPE)(OpenIPC_DeviceId interfaceID, const uint32_t* input, uint32_t inputdwords, uint32_t* output, uint32_t maxoutputdwords, uint32_t* outputdwords);

#if defined(__cplusplus)
    } // extern "C"
#endif

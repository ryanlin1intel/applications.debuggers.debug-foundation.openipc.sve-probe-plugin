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
///  @brief Contains methods for interacting at a low level with I2C interfaces.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "I2cDefinitions.h"

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_I2C_DirectRead
///
/// @brief This function allows for the direct read of an I2C bus.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] deviceId the device Id to target on the I2C bus (this is the first byte written onto the bus).
/// @param [in] bufferLength The length of the readBuffer in uint32_t
/// @param [out] readBuffer The buffer to populate with the data read. This may be null; if it is, do not return the read information.
/// @param [out] addressAck True iff the address was ack'd; this may be null, if so then do not return addressAck information.
/// @param [in] forceStop An indication to perform a forceStop between transactions
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_I2C_DirectRead(PPI_ProbeBundleHandle handle, uint8_t deviceId, uint32_t bufferLength, uint8_t* readBuffer, PPI_bool forceStop, PPI_bool* addressAck, const PPI_I2C_Options* const options);
typedef OpenIPC_Error (* PPI_I2C_DirectRead_TYPE)(PPI_ProbeBundleHandle handle, uint8_t deviceId, uint32_t bufferLength, uint8_t* readBuffer, PPI_bool forceStop, PPI_bool* addressAck, const PPI_I2C_Options* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_I2C_DirectWrite
///
/// @brief This function allows for the direct write of an I2C bus.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] deviceId the device Id to target on the I2C bus (the first byte written onto the bus).
/// @param [in] bufferLength The length of the writeBuffer in uint32_t
/// @param [in] writeBuffer The buffer with the data to write
/// @param [in] forceStop An indication to perform a forceStop between transactions
/// @param [out] addressAck Is true iff the address was ack'd; this may be null, if so then do not return addressAck information.
/// @param [out] lastDataAck Must null or be the size of the write + 1 in bytes and will contain all of the ACK/NACK from each of the scans; if it is null then do not return lastDataAck information.
/// @param [in] options The options structure; this can be null, in that case defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_I2C_DirectWrite(PPI_ProbeBundleHandle handle, uint8_t deviceId, uint32_t bufferLength, const uint8_t* const writeBuffer, PPI_bool forceStop, PPI_bool* addressAck, uint32_t* lastDataAck, const PPI_I2C_Options* const options);
typedef OpenIPC_Error (* PPI_I2C_DirectWrite_TYPE)(PPI_ProbeBundleHandle handle, uint8_t deviceId, uint32_t bufferLength, const uint8_t* const writeBuffer, PPI_bool forceStop, PPI_bool* addressAck, uint32_t* lastDataAck, const PPI_I2C_Options* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

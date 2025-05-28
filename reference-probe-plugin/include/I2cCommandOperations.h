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
///  @brief Contains methods for interacting with I2C devices at a high level.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "I2cDefinitions.h" // for the probe itself

#if defined(__cplusplus)
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_I2C_CommandRead
///
/// @brief Allows for a high-level command-type interaction with an I2C bus reads.
///
/// This function is more high level than 'raw' commands. It allows for the following scenarios:
/// 1. commandBufferLength = 0 and dataBufferLength > 0 which gives: START, send deviceID, read dataBufferLength uint32_t from device; all bytes except last are ACK'd.
/// 2. commandBufferLength > 0 and dataBufferLength = 0 which gives: START, send commandBuffer data.
/// 3. commandBufferLength > 0 and dataBufferLength > 0 which gives: START, send commandBuffer data, send repeated START, read dataBufferLength uint32_t from device; all bytes except last are ACK'd.
///
/// @param [in,out] handle The handle to use (may be a bundle, hold or release)
/// @param [in] deviceId The 8-bit device ID to access (this is the value sent across the bus)
/// @param [in] commandBuffer The 8-bit entry array of commands to write to the device (if NULL, no data will be written)
/// @param [in] commandBufferLength The length of the commandBuffer in uint8_t entries.
/// @param [out] dataBuffer The dataBuffer to write the read result; this read is generated either by a start if the commandBuffer is empty or repeated start ow.
/// @param [in] dataBufferLength The length of dataBuffer in uint32_t entries.
/// @param [in] options The options structure; this can be null, in that case defaults are used. Slots are used for the dataBuffer, not the command buffer.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Interface_I2c_Nack_At_Address_Phase on NACK on a device ID send
/// @retval OpenIPC_Error_Interface_I2c_Nack_Before_Length_Complete on NACK on writing commandBuffer
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_I2C_CommandRead(PPI_ProbeBundleHandle handle, uint8_t deviceId, const uint8_t* const commandBuffer, uint32_t commandBufferLength, uint8_t* dataBuffer, uint32_t dataBufferLength, const PPI_I2C_Options* const options);
typedef OpenIPC_Error (* PPI_I2C_CommandRead_TYPE)(PPI_ProbeBundleHandle handle, uint8_t deviceId, const uint8_t* const commandBuffer, uint32_t commandBufferLength, uint8_t* dataBuffer, uint32_t dataBufferLength, const PPI_I2C_Options* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_I2C_CommandWrite
///
/// @brief Allows for a high-level command-type interaction with an I2C bus writes.
///
/// This function is more high level than 'raw' commands. It allows for the following scenarios:
/// 1. commandBufferLength = 0 and dataBufferLength > 0 which gives: START, send deviceID, write dataBufferLength uint32_t to device; all bytes except last are NACK'd.
/// 2. commandBufferLength > 0 and dataBufferLength = 0 which gives: START, send deviceID, send commandBuffer data.
/// 3. commandBufferLength > 0 and dataBufferLength > 0 which gives: START, send deviceID, send commandBuffer data, send repeated START, read dataBufferLength uint32_t from device; all bytes except last are NACK'd.
///
/// @param [in,out] handle The handle to use (may be a bundle, hold or release)
/// @param [in] deviceId The 8-bit device ID to access
/// @param [in] commandBuffer The 8-bit entry array of commands to write to the device (if NULL, no data will be written)
/// @param [in] commandBufferLength The length of the commandBuffer in uint32_t entries.
/// @param [in] dataBuffer The dataBuffer to write ; this read is generated either by a start if the commandBuffer is empty or repeated start ow.
/// @param [in] dataBufferLength The length of dataBuffer in uint32_t entries.
/// @param [in] options The options structure; this can be null, in that case defaults are used. Slots are used for the dataBuffer, not the command buffer.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Interface_I2c_Nack_At_Address_Phase on NACK on a device ID send
/// @retval OpenIPC_Error_Interface_I2c_Nack_Before_Length_Complete on NACK on writing commandBuffer
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_I2C_CommandWrite(PPI_ProbeBundleHandle handle, uint8_t deviceId, const uint8_t* const commandBuffer, uint32_t commandBufferLength, const uint8_t* const dataBuffer, uint32_t dataBufferLength, const PPI_I2C_Options* const options);
typedef OpenIPC_Error (* PPI_I2C_CommandWrite_TYPE)(PPI_ProbeBundleHandle handle, uint8_t deviceId, const uint8_t* const commandBuffer, uint32_t commandBufferLength, const uint8_t* const dataBuffer, uint32_t dataBufferLength, const PPI_I2C_Options* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

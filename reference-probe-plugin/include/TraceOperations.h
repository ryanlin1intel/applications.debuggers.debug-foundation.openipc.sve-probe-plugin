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
//  Function: PPI_InterfaceTraceOpenWindow
///
/// @brief Opens a trace connection on the specified interface.
///
///     InterfaceTraceOpen opens a trace connection on the specified interface.
///     The trace model assumes that the specific configuration of the trace endpoint is
///     done by the initialization ( @a InterfaceInitialization ) and interface configuration
///     ( @a InterfaceSetConfig ).
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
///
/// @param[in] accessMode
///     The setting read or write to open the trace window for.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
/// @retval OpenIPC_Error_InterfacePort_Window_Already_Open
///     The window for this interface is already open.
/// @retval OpenIPC_Error_InterfacePort_AccessMode_Not_Supported
///     This interface does not support the requested access mode.
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTraceOpenWindow(OpenIPC_DeviceId interfaceID, PPI_Trace_PortAccessMode accessMode);
typedef OpenIPC_Error (* PPI_InterfaceTraceOpenWindow_TYPE)(OpenIPC_DeviceId interfaceID, PPI_Trace_PortAccessMode accessMode);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTraceCloseWindow
///
/// @brief Closes a trace connection on the specified interface.
///
///     InterfaceTraceClose closes a trace connection on the specified interface
///     which terminates all data transfer through the interface.
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
/// @retval OpenIPC_Error_InterfacePort_Window_Not_Open
///     The window for this interface is not open.
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTraceCloseWindow(OpenIPC_DeviceId interfaceID);
typedef OpenIPC_Error (* PPI_InterfaceTraceCloseWindow_TYPE)(OpenIPC_DeviceId interfaceID);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTraceIsReadDataAvailable
///
/// @brief Returns true if data is available on the interface.
///
/// @pre isDataAvailable != null
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[out] isDataAvailable
///     Returns true if data is available for reading
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTraceIsReadDataAvailable(OpenIPC_DeviceId interfaceID, PPI_bool* isDataAvailable);
typedef OpenIPC_Error (* PPI_InterfaceTraceIsReadDataAvailable_TYPE)(OpenIPC_DeviceId interfaceID, PPI_bool* isDataAvailable);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTraceIsWindowOpen
///
/// @brief Returns true if the trace window is open.
///
/// @pre isWindowOpen != null
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[out] isWindowOpen
///     Returns true if the window is open
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @retval OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTraceIsWindowOpen(OpenIPC_DeviceId interfaceID, PPI_bool* isWindowOpen);
typedef OpenIPC_Error (* PPI_InterfaceTraceIsWindowOpen_TYPE)(OpenIPC_DeviceId interfaceID, PPI_bool* isWindowOpen);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTracePortRead
///
/// @brief PPI_InterfaceTracePortRead provides the ability to read blocks of data from a previously configured trace source.
///
///     PPI_InterfaceTracePortRead allows a caller to request blocks of data by providing a buffer and requesting a maximum number of bytes
///
/// @pre output != null
/// @pre outputBytes != null
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[out] output
///     An output buffer that is allocated by the caller
/// @param[in] maxOutputBytes
///     The maximum number of bytes that can be safely written to @a output buffer.
/// @param[out] outputBytes
///     The actual number of bytes written to @a output.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
/// @retval OpenIPC_Error_InterfacePort_Window_Not_Open
///     The window for this interface is not open.
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTracePortRead(OpenIPC_DeviceId interfaceID, uint8_t* output, uint32_t maxOutputBytes, uint32_t* outputBytes);
typedef OpenIPC_Error (* PPI_InterfaceTracePortRead_TYPE)(OpenIPC_DeviceId interfaceID, uint8_t* output, uint32_t maxOutputBytes, uint32_t* outputBytes);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfaceTracePortWrite
///
/// @brief PPI_InterfaceTracePortWrite provides the ability to write blocks of data to a previously configured trace source.
///
/// @pre input != null
/// @pre inputBytes != null
///
/// @param[in] interfaceID
///     The unique identification number of the interface, set during @a InterfaceInitialize.
/// @param[in] input
///     An in buffer that is allocated by the caller
/// @param[in] maxInputBytes
///     The maximum number of bytes that can be safely read from @a input buffer.
/// @param[out] inputBytes
///     The actual number of bytes written to @a output.
///
/// @return (OpenIPC_Error): A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error
///     Indicates success.
/// @return OpenIPC_Error_Invalid_Device_ID
///     The interface deviceId @a interfaceID is not valid
/// @retval OpenIPC_Error_InterfacePort_Window_Not_Open
///     The window for this interface is not open.
///
PROBEPLUGIN_API OpenIPC_Error PPI_InterfaceTracePortWrite(OpenIPC_DeviceId interfaceID, const uint8_t* input, uint32_t maxInputBytes, uint32_t* inputBytes);
typedef OpenIPC_Error (* PPI_InterfaceTracePortWrite_TYPE)(OpenIPC_DeviceId interfaceID, const uint8_t* input, uint32_t maxInputBytes, uint32_t* inputBytes);

        #if defined(__cplusplus)
    } // extern "C"
#endif

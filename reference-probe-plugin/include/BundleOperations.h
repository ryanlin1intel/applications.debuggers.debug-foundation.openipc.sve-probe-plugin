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
///  @brief Contains methods for interacting with bundles.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ProbePlugin.h"

#if defined(__cplusplus)
    extern "C" {
#endif

// Model of the bundle handles:
//
//
// Slot is a mathematical type, where a slot is a type with two fields:
// 1. value: An integer >= 0
// 2. length: An integer >= 0.
// with constraint: value < 2^length
//
// SLOT_DEFAULT is a constant provided by the implementation that of type Slot where all items in the string have value (0,0).
//
// A non PPI_PROBE_LOCK_RELEASE nor PPI_PROBE_LOCK_HOLD bundle handle has the following fields:
// 1. slots: An string of Slot
// 2. commands: A string of commands; types commands are defined by the services
// 3. has_been_run: a boolean
// 4. If it is lock bundle, it includes the device to target.
//
// As an example, we denote a value ("(2,3)", "", False) to denote:
// a bundle handle with one slot with value 2,  length of 3 bits. No commands queued and it has never been run.
//
// In this document, a client is a software component that is using a Probe
// Abstraction API implementation, e.g., the OpenIPC.
//
typedef void* PPI_ProbeBundleHandle;

///
/// @brief A released lock.
///
const PPI_ProbeBundleHandle PPI_PROBE_LOCK_RELEASE = (PPI_ProbeBundleHandle)0;

///
/// @brief A held lock.
///
const PPI_ProbeBundleHandle PPI_PROBE_LOCK_HOLD = (PPI_ProbeBundleHandle)1;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_Allocate
///
/// @brief Allocates a bundle handle for the probe = ("", "",False).
///
/// Allocates a bundle handle for the probe; returns PROBE_LOCK_RELEASE if unable to.
/// This function is not required to be provided (in that case, the client is responsible for emulating bundle behaviour).
/// Note that for optimal performance (with a "full-featured" probe), it is *highly* recommended that one implement full bundles.
/// @returns NULL iff unable to allocate a new bundle handle.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API PPI_ProbeBundleHandle PPI_Bundle_Allocate(void);
typedef PPI_ProbeBundleHandle (* PPI_Bundle_Allocate_TYPE)(void);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Lock_Target_Interface
///
/// @brief Locks a given device id and sets the target of the PPI_PROBE_LOCK_HOLD to use the given device ID.
///
/// This operation allows for a client to 'lock' the interface from additional peers to the Probe Plugin from using it. One example is if the probe plugin
/// interacts with another debug tool that polls for run control events using JTAG. If we lock an interface, then the requirement is that the debug tool does not
/// modify the JTAG chain while the lock is set.
/// Moreover, the model is that we can have more that 1 lock (and use Bundle_Execute to release those locks), but only 1 distinguished locked interface that we can send scans to using PPI_PROBE_LOCK_HOLD.
/// @param [in] deviceInterface The device that this bundle should target. This is required of all implementations.
/// @returns OpenIPC_Error_No_Error iff it is able to lock this interface.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Lock_Target_Interface(OpenIPC_DeviceId deviceInterface);
typedef OpenIPC_Error (* PPI_Lock_Target_Interface_TYPE)(OpenIPC_DeviceId deviceInterface);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_List_Locked_Interfaces
///
/// @brief This operation returns the currently locked interfaces.
///
/// This operation returns the currently locked interfaces and returns *all* interfaces locked due to peer locking (e.g., the set of interfaces returned is closed under the symmetric, transitive, and reflexive peer relation).
/// @param [in] maxNumberOfInterfaces The size of the interfaces parameter; the implementation will not write more than this number of interfaces to the interfaces parameter in any case.
/// @param [out] interfaces The list of interfaces currently locked.
/// @param [out] numberOfInterfaces The number of interfaces that are locked; if it is greater that the maxNumberOfInterfaces the client should recall with a larger interfaces parameter.
/// @returns OpenIPC_Error_No_Error iff the operation completed successfully.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_List_Locked_Interfaces(uint32_t maxNumberOfInterfaces, OpenIPC_DeviceId* interfaces, uint32_t* numberOfInterfaces);
typedef OpenIPC_Error (* PPI_List_Locked_Interfaces_TYPE)(uint32_t maxNumberOfInterfaces, OpenIPC_DeviceId* interfaces, uint32_t* numberOfInterfaces);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_Execute
///
/// @brief Executes the commands contained in a bundle handle.
///
/// Executes the commands contained in a bundle handle, with an option to keep a API lock.
/// When executing a locked or release handle, this operation simply keeps or releases the lock. This is required of all implementations.
/// @param [in,out] handle handle containing the commands to be executed; handle.has_been_run = True after the call completes
/// @param [in] keepLock Ignored if executing a locked or release handle. Otherwise, is true iff the interface should remain locked after the handle is executed.
/// @param [in] deviceInterface the interface to run scans on
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Execute_Error on the inability to execute all or some of the bundle.
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle or if the deviceInterface is not appropriate for the scans in the bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_Execute(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId deviceInterface, PPI_bool keepLock);
typedef OpenIPC_Error (* PPI_Bundle_Execute_TYPE)(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId deviceInterface, PPI_bool keepLock);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_Free
///
/// @brief Frees a bundle handle. Handle is no longer a valid handle.
///
/// Clears and Frees a bundle handle.  This is required of all implementations (though it is almost a nop if the handle is PPI_PROBE_LOCK_RELEASE or PPI_PROBE_LOCK_HOLD).
/// @param [in,out] handle The handle to free. It is set to PPI_PROBE_LOCK_RELEASE upon returning.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_Free(PPI_ProbeBundleHandle* handle);
typedef OpenIPC_Error (* PPI_Bundle_Free_TYPE)(PPI_ProbeBundleHandle* handle);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_Clear
///
/// @brief Clears the commands queued in a bundle handle.
///
/// Clears out all commands contained within a bundle handle, i.e., handle.commands = "" and handle.slots = #handle.slots and handle.has_been_run = False.
///  This is required of all implementations (though it is a nop if the handle is PPI_PROBE_LOCK_RELEASE or PPI_PROBE_LOCK_HOLD).
/// @param [in,out] handle bundle to clear all commands.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_Clear(PPI_ProbeBundleHandle handle);
typedef OpenIPC_Error (* PPI_Bundle_Clear_TYPE)(PPI_ProbeBundleHandle handle);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_Append
///
/// @brief Logically appends a bundle on to the end of another.
///
/// Appends an existing bundle (that cannot be modified anymore) to another bundle that is modifiable. The intended use case for this functionality is to
/// build up repeated JTAG scans that are logically register access algorithms. This will enable better compression by potentially allowing implementations to
/// reuse previously declared bundles.
/// @param [in,out] destHandle bundle that will be appended.
/// @param [in,out] sourceBundle the bundle that is to be appended to destBundle; this is modified to not be modifiable after this call.
/// @param [out] outputBuffer Buffer to place any writeback of this bundle (writeback will be byte aligned per operation)
/// @param [in] outputBufferSize Length of the outputBuffer in bytes
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the destHandle or sourceBundle is not a valid probe handle
///
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_Append(PPI_ProbeBundleHandle destHandle, PPI_ProbeBundleHandle sourceBundle, uint8_t* outputBuffer, uint32_t outputBufferSize);
typedef OpenIPC_Error (* PPI_Bundle_Append_TYPE)(PPI_ProbeBundleHandle destHandle, PPI_ProbeBundleHandle sourceBundle, uint8_t* outputBuffer, uint32_t outputBufferSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_ExecuteMultiChain
///
/// @brief Executes the commands contained in a bundle handle on multiple devices.
///
/// Executes the commands contained in a bundle handle, with an option to keep a API lock.
/// When executing a locked or release handle, this operation simply keeps or releases the lock. This is required of all implementations.
/// @param [in,out] handle handle containing the commands to be executed; handle.has_been_run = True after the call completes
/// @param [in] keepLock Ignored if executing a locked or release handle. Otherwise, is true iff the interfaces should remain locked after the handle is executed.
/// @param [in] deviceInterfaces the interfaces to run scans on
/// @param [in] deviceInterfacesLength the length of the deviceInterfaces array
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Execute_Error on the inability to execute all or some of the bundle.
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle or if the deviceInterface is not appropriate for the scans in the bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_ExecuteMultiChain(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId* deviceInterfaces, uint32_t deviceInterfacesLength, PPI_bool keepLock);
typedef OpenIPC_Error (* PPI_Bundle_ExecuteMultiChain_TYPE)(PPI_ProbeBundleHandle handle, OpenIPC_DeviceId* deviceInterfaces, uint32_t deviceInterfacesLength, PPI_bool keepLock);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Bundle_InterfaceSetConfig
///
/// @brief Executes the commands contained in a bundle handle on multiple devices.
///
/// Executes the commands contained in a bundle handle, with an option to keep a API lock.
/// When executing a locked or release handle, this operation simply keeps or releases the lock. This is required of all implementations.
/// @param [in,out] handle handle containing the commands to be executed; handle.has_been_run = True after the call completes
/// @param [in] configType the config to set the value for
/// @param [in] value the value to set this the specified config to
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Operation_Not_Supported when the probe plugin does not support setting this config in a bundle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle or if the deviceInterface is not appropriate for the scans in the bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Bundle_InterfaceSetConfig(PPI_ProbeBundleHandle handle, const char* configType, const char* value);
typedef OpenIPC_Error (* PPI_Bundle_InterfaceSetConfig_TYPE)(PPI_ProbeBundleHandle handle, const char* configType, const char* value);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_MemoryRead
///
/// @brief Performs a memory read on the target.
///
/// @param [in] bundle handle to add this operation to
/// @param [in] address the address to read memory from
/// @param [in,out] buffer the location to store the memory that was read
/// @param [in,out] size the size to read and set to the actual size read
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Operation_Not_Supported when the probe plugin does not support this memory operation
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle or if the deviceInterface is not appropriate for the scans in the bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_MemoryRead(PPI_ProbeBundleHandle bundle, uint64_t address, uint8_t* buffer, uint32_t* size);
typedef OpenIPC_Error (* PPI_MemoryRead_TYPE)(PPI_ProbeBundleHandle bundle, uint64_t address, uint8_t* buffer, uint32_t* size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_MemoryWrite
///
/// @brief Performs a memory write on the target.
///
/// @param [in] bundle handle to add this operation to
/// @param [in] address the address to write memory to
/// @param [in,out] buffer the data to write to the target memory
/// @param [in,out] size the size of the data to write and set to the actual size written
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Operation_Not_Supported when the probe plugin does not support this memory operation
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle or if the deviceInterface is not appropriate for the scans in the bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_MemoryWrite(PPI_ProbeBundleHandle bundle, uint64_t address, const uint8_t* buffer, uint32_t* size);
typedef OpenIPC_Error (* PPI_MemoryWrite_TYPE)(PPI_ProbeBundleHandle bundle, uint64_t address, const uint8_t* buffer, uint32_t* size);

#if defined(__cplusplus)
    } // extern "C"
#endif

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

/// @brief The opaque type for denoting StatePort operation
///
///  This struct holds the type of operation being performed,
///  the values of any parameters, and any other information that
///  might be necessary to perform the operation.
///
///  This is treated as an opaque data type, i.e., a client cannot know how it is implemented internally,
///  only observe what happens when interacting via API functions.
///
typedef struct PPI_InterfaceStatePortOperation PPI_InterfaceStatePortOperation;

static PPI_InterfaceStatePortOperation* const INVALID_OPERATION = nullptr;

/// @brief Opaque type for the definition of a state port operation
/// This includes the fields, outputs, etc. of a particular stateport
///
typedef struct PPI_InterfaceStatePortDefinition PPI_InterfaceStatePortDefinition;

/// @brief StatePort Operation Types
/// This is a flags enum, where each operation type has just one bit set, which isn't shared with any other operation type.
///
DECLARE_ENUM(PPI_StatePort_OPERATION_TYPE_ET) {
    StatePort_OPERATION_NONE  =  0x00,
    StatePort_OPERATION_READ  =  0x01,
    StatePort_OPERATION_WRITE = 0x02,
};

/// Typedef for the error code from stateport operations.
typedef uint32_t ProbeStatePortError;
/// Definition for an "ok" operation error
const ProbeStatePortError PROBE_STATE_PORT_ERROR_OK = 0;
/// Definition for an invalid operation error
const ProbeStatePortError PROBE_STATE_PORT_ERROR_INVALID = (ProbeStatePortError)(-1);

/// @brief The opaque type for denoting a StatePort operation parameter
///
///  This struct holds the parameter name, description, and size.
///  This struct also knows what operation type it is for.
///
///  This is treated as an opaque data type, i.e., a client cannot know how it is implemented internally,
///  only observe what happens when interacting via API functions.
///
typedef struct PPI_InterfaceStatePortParameter PPI_InterfaceStatePortParameter;

// Functions for interacting with StatePort operations.

/// PPI_StatePortAllocateOperation optional arguments.
typedef struct
{
    // Empty now, but left for future expansion.
    uint8_t reserved;
} PPI_StatePortAllocateOperation_Options;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortAllocateOperation
//
/// @brief This function allocates a StatePort Operation.
///
/// Every PPI_InterfaceStatePortOperation object must be freed with a call to PPI_StatePortFreeOperation.
///
/// @pre definition != null
/// @pre operation != null
///
/// @param [in] deviceId The device id of the StatePort instance.
/// @param [in] definition The definition of the stateport.
/// @param [in] operationType The type of operation you will perform.  Can only be one type.
/// @param [out] operation Populated with a pointer to the newly allocated PPI_InterfaceStatePortOperation
/// @param [in] options Additional options (NULL for default).  Currently empty, but may be extended later.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Device_ID if deviceId is invalid
/// @retval OpenIPC_Error_Invalid_Parameter if operationType is not valid
/// @retval OpenIPC_Error_Internal_Error if unable to allocate the memory.
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortAllocateOperation(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition* definition, PPI_StatePort_OPERATION_TYPE_ET operationType, PPI_InterfaceStatePortOperation** operation, const PPI_StatePortAllocateOperation_Options* options);
typedef OpenIPC_Error (* PPI_StatePortAllocateOperation_TYPE)(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition* definition, PPI_StatePort_OPERATION_TYPE_ET operationType, PPI_InterfaceStatePortOperation** operation, const PPI_StatePortAllocateOperation_Options* options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortFreeOperation
//
/// @brief This function frees a PPI_InterfaceStatePortOperation that was allocated with PPI_StatePortAllocateOperation.
///
/// @pre operation != null
///
/// @post *operation == INVALID_OPERATION
///
/// @param [in] deviceId The device id of the StatePort instance.
/// @param [in,out] operation The PPI_InterfaceStatePortOperation to be freed.  After the procedure completes *operation == INVALID_OPERATION
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Device_ID if deviceId is invalid
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortFreeOperation(OpenIPC_DeviceId deviceId, PPI_InterfaceStatePortOperation** operation);
typedef OpenIPC_Error (* PPI_StatePortFreeOperation_TYPE)(OpenIPC_DeviceId deviceId, PPI_InterfaceStatePortOperation** operation);

/// PPI_StatePortSubmitOperation optional arguments.
typedef struct
{
    //Empty now, but left for future expansion.
    uint8_t reserved;
} PPI_StatePortAppendOperation_Options;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortAppendOperation
//
/// @brief This function appends a StatePort operation to a bundle.
///
/// @remarks this function transfers ownership of operation to the probe plugin. If this function returns OpenIPC_Error_No_Error then
/// the probe plugin will take responsibility for freeing the operation when the associated bundle is completed.
///
/// @pre operationPointer != null
///
/// @post *operationPointer == INVALID_OPERATION
///
/// @param [in] deviceId The device id of the StatePort instance.
/// @param [in] bundle The bundle you are submitting this operation to.
/// @param [in] operationPointer The StatePort Operation to want to submit. After the procedure completes, *operationPointer == INVALID_OPERATION
/// @param [in] options Additional options (NULL for default).  Currently empty, but may be extended later.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Device_ID if deviceId is invalid
/// @retval OpenIPC_Error_Invalid_Parameter if bundle is not valid
/// @retval OpenIPC_Error_Null_Pointer if operation is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortAppendOperation(OpenIPC_DeviceId deviceId, PPI_ProbeBundleHandle bundle, PPI_InterfaceStatePortOperation** operationPointer, const PPI_StatePortAppendOperation_Options* options);
typedef OpenIPC_Error (* PPI_StatePortAppendOperation_TYPE)(OpenIPC_DeviceId deviceId, PPI_ProbeBundleHandle bundle, PPI_InterfaceStatePortOperation** operationPointer, const PPI_StatePortAppendOperation_Options* options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortOperationSetParameter
//
/// @brief This function sets a parameter on an operation
///
/// @pre operation != null
/// @pre parameter != null
/// @pre value != null
///
/// @param [in] operation The StatePort Operation to want to set the parameter on.
/// @param [in] parameter The parameter that you want to set on the operation.
/// @param [in] value A byte buffer for the value you want to set.
/// @param [in] size The size of value in bytes.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Parameter if size is too large
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortOperationSetParameter(PPI_InterfaceStatePortOperation* operation, const PPI_InterfaceStatePortParameter* parameter, const uint8_t* value, uint32_t size);
typedef OpenIPC_Error (* PPI_StatePortOperationSetParameter_TYPE)(PPI_InterfaceStatePortOperation* operation, const PPI_InterfaceStatePortParameter* parameter, const uint8_t* value, uint32_t size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortOperationSetWriteValue
//
/// @brief This function sets a write value for an operation.
///
/// @pre operation != null
/// @pre buffer != null
///
/// @param [in] operation The StatePort Operation to want to set the write value for.
/// @param [in] buffer A byte buffer that holds the write value.
/// @param [in] size The size of buffer in bytes.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Parameter if size is too large
/// @retval OpenIPC_Error_Not_Supported if the stateport that created this operation does not support writes
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortOperationSetWriteValue(PPI_InterfaceStatePortOperation* operation, const uint8_t* buffer, uint32_t size);
typedef OpenIPC_Error (* PPI_StatePortOperationSetWriteValue_TYPE)(PPI_InterfaceStatePortOperation* operation, const uint8_t* buffer, uint32_t size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortOperationSetReadBuffer
//
/// @brief This function sets a read buffer for an operation.
///
/// @pre operation != null
/// @pre buffer != null
///
/// @param [in] operation The StatePort Operation to want to set the read buffer for.
/// @param [out] buffer A byte buffer that will be filled with the return value.
/// @param [in] size The size of buffer in bytes.  Must be large enough to hold the read value of the operation.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Parameter if size is too small
/// @retval OpenIPC_Error_Not_Supported if the stateport that created this operation doesn't support reads
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortOperationSetReadBuffer(PPI_InterfaceStatePortOperation* operation, uint8_t* buffer, uint32_t size);
typedef OpenIPC_Error (* PPI_StatePortOperationSetReadBuffer_TYPE)(PPI_InterfaceStatePortOperation* operation, uint8_t* buffer, uint32_t size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortOperationSetErrorBuffer
//
/// @brief This function sets an error buffer for an operation.
///
/// @pre operation != null
/// @pre errorBuffer != null
///
/// @param [in] operation The StatePort Operation to want to set the error buffer for.
/// @param [out] errorBuffer Will be set to the error value.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortOperationSetErrorBuffer(PPI_InterfaceStatePortOperation* operation, ProbeStatePortError* errorBuffer);
typedef OpenIPC_Error (* PPI_StatePortOperationSetErrorBuffer_TYPE)(PPI_InterfaceStatePortOperation* operation, ProbeStatePortError* errorBuffer);

// End of functions for interacting with StatePort operation

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetDefinitions
//
/// @brief Gets the Operation definitions for a particular stateport.
///
/// @pre definitions != null || definitionSize == 0
/// @pre numberOfDefinitions != null
///
/// @post PPI_InterfaceStatePortDefinition* copied to definitions will remain valid until PPI_InterfaceDeInitialize is called for deviceId
///
/// @param [in] deviceId The device id of the StatePort instance to query.
/// @param [out] definitions The produced list of definitions. This can be NULL. Only the buffer's memory is owned by the client, everything else is owned by the plugin and has a valid lifetime until the interface is deinitialized.
/// @param [in] definitionsSize The number of entries in the @a definitions buffer. If this is 0, then the definitions pointer can be NULL.
/// @param [out] numberOfDefinitions The total number of definitions for this device. This will always be written and cannot be NULL.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Device_ID if deviceId is invalid
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetDefinitions(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition** definitions, uint32_t definitionsSize, uint32_t* numberOfDefinitions);
typedef OpenIPC_Error (* PPI_StatePortGetDefinitions_TYPE)(OpenIPC_DeviceId deviceId, const PPI_InterfaceStatePortDefinition** definitions, uint32_t definitionsSize, uint32_t* numberOfDefinitions);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetName
//
/// @brief This function gets a string for the StatePort name.
///
/// @pre definition != null
/// @pre realSize != null
/// @pre buffer != null || bufferSize == 0
///
/// @post if buffer != null, buffer contains a lowercase null terminated string.
///
/// @param [in] definition The definition of the stateport.
/// @param [out] buffer A byte buffer that will be filled with the name (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize Will be populated with the size of the name in bytes, including the null terminator.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetName(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortGetName_TYPE)(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetDescription
//
/// @brief This function gets a string for the StatePort description.
///
/// @pre definition != null
/// @pre realSize != null
/// @pre buffer != null || bufferSize == 0
///
/// @post if buffer != null, buffer contains a null terminated string.
///
///
/// @param [in] definition The definition of the stateport.
/// @param [out] buffer A byte buffer that will be filled with the null terminated description (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize Will be populated with the size of the description in bytes, including the null terminator.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetDescription(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortGetDescription_TYPE)(const PPI_InterfaceStatePortDefinition* definition, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetCapabilites
//
/// @brief This function gets a PPI_StatePort_OPERATION_ET for all the operation types this StatePort supports.
///
/// @pre definition != null
/// @pre capabilities != null
///
/// @param [in] definition The definition of the operation.
/// @param [out] capabilities All operation types that this StatePort supports (and only those that this StatePort supports) will be set.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetCapabilites(const PPI_InterfaceStatePortDefinition* definition, PPI_StatePort_OPERATION_TYPE_ET* capabilities);
typedef OpenIPC_Error (* PPI_StatePortGetCapabilites_TYPE)(const PPI_InterfaceStatePortDefinition* definition, PPI_StatePort_OPERATION_TYPE_ET* capabilities);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetAccessSize
//
/// @brief This function gets the access size (in bits) for an operation type on this StatePort.
///
/// @pre definition != null
/// @pre size != null
///
/// @param [in] definition The definition of the stateport.
/// @param [out] size Will be set to the access size (in bits) of the specified operation type.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetAccessSize(const PPI_InterfaceStatePortDefinition* definition, uint32_t* size);
typedef OpenIPC_Error (* PPI_StatePortGetAccessSize_TYPE)(const PPI_InterfaceStatePortDefinition* definition, uint32_t* size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortGetParameters
//
/// @brief This function gets the parameters for an operation type.
///
/// @pre definition != null
/// @pre realLength != null
/// @pre parameters != null || length == 0
///
/// @post PPI_InterfaceStatePortParameter* copied to parameters will remain valid until PPI_InterfaceDeInitialize is called for deviceId
///
/// @param [in] definition The definition of the stateport.
/// @param [out] parameters An array of PPI_InterfaceStatePortParameter*, which will be populated with the parameters for the given operation type.
/// @param [in] length The length of parameters (counted in number of items as an array of PPI_InterfaceStatePortParameter*).  May be 0.
/// @param [out] realLength Will be populated with the number of parameters for this operation.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortGetParameters(const PPI_InterfaceStatePortDefinition* definition, const PPI_InterfaceStatePortParameter** parameters, uint32_t length, uint32_t* realLength);
typedef OpenIPC_Error (* PPI_StatePortGetParameters_TYPE)(const PPI_InterfaceStatePortDefinition* definition, const PPI_InterfaceStatePortParameter** parameters, uint32_t length, uint32_t* realLength);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortParameterGetName
//
/// @brief This function gets the name of a parameter.
///
/// @pre parameter != null
/// @pre realSize != null
/// @pre buffer != null || bufferSize == 0
///
/// @post if buffer != null, buffer contains a null terminated string.
///
/// @param [in] parameter The parameter that you will get the name of.
/// @param [out] buffer A byte buffer that will be filled with the name (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize Will be populated with the size of the name in bytes.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortParameterGetName(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortParameterGetName_TYPE)(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortParameterGetDescription
//
/// @brief This function gets the description of a parameter.
///
/// @pre parameter != null
/// @pre realSize != null
/// @pre buffer != null || bufferSize == 0
///
/// @post if buffer != null, buffer contains a null terminated string.
///
/// @param [in] parameter The parameter that you will get the description of.
/// @param [out] buffer A byte buffer that will be filled with the description (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize Will be populated with the size of the description in bytes.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortParameterGetDescription(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortParameterGetDescription_TYPE)(const PPI_InterfaceStatePortParameter* parameter, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortParameterGetSize
//
/// @brief This function gets the size of a parameter in bits.
///
/// @pre parameter != null
/// @pre size != null
///
/// @param [in] parameter The parameter that you will get the size of.
/// @param [out] size The size of the parameter in bits.
/// @return (OpenIPC_Error)
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortParameterGetSize(const PPI_InterfaceStatePortParameter* parameter, uint32_t* size);
typedef OpenIPC_Error (* PPI_StatePortParameterGetSize_TYPE)(const PPI_InterfaceStatePortParameter* parameter, uint32_t* size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortParameterGetDefaultValue
//
/// @brief This function gets the default value of a parameter.
///
/// @pre parameter != null
/// @pre buffer != null
///
/// @param [in] parameter The parameter that you will get the default value of.
/// @param [out] buffer Populated with the default value of the parameter.
/// @param [in] size The size of buffer in bytes. Must be large enough to hold the value (same as the parameter size).
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Not_Supported if parameter does not support a default value
/// @retval OpenIPC_Error_Invalid_Argument if size is too small
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortParameterGetDefaultValue(const PPI_InterfaceStatePortParameter* parameter, uint8_t* buffer, uint32_t size);
typedef OpenIPC_Error (* PPI_StatePortParameterGetDefaultValue_TYPE)(const PPI_InterfaceStatePortParameter* parameter, uint8_t* buffer, uint32_t size);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortErrorGetName
//
/// @brief This function gets the name of an error.
///
/// @param [in] definition The definition of the stateport.
/// @param [in] error The error that you will get the name of.
/// @param [out] buffer A byte buffer that will be filled with the name (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize If not null, will be populated with the size of the name in bytes.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Null_Pointer if definition is null
/// @retval OpenIPC_Error_Invalid_Argument if error is not valid
/// @retval OpenIPC_Error_Null_Pointer if buffer is null and bufferSize is non-zero
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortErrorGetName(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortErrorGetName_TYPE)(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortErrorGetDescription
//
/// @brief This function gets the description of an error.
///
/// @param [in] definition The definition of the stateport.
/// @param [in] error The error that you will get the description of.
/// @param [out] buffer A byte buffer that will be filled with the description (if not null).  buffer will not be filled beyond bufferSize.
/// @param [in] bufferSize The size of buffer in bytes.  May be 0.
/// @param [out] realSize If not null, will be populated with the size of the description in bytes.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Null_Pointer if definition is null
/// @retval OpenIPC_Error_Invalid_Argument if error is not valid
/// @retval OpenIPC_Error_Null_Pointer if buffer is null and bufferSize is non-zero
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortErrorGetDescription(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, char* buffer, uint32_t bufferSize, uint32_t* realSize);
typedef OpenIPC_Error (* PPI_StatePortErrorGetDescription_TYPE)(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, char* buffer, uint32_t bufferSize, uint32_t* realSize);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortErrorGetOpenIpcError
//
/// @brief This function gets the success of an error.
///
/// @param [in] definition The definition of the stateport.
/// @param [in] error The error that you will get the success of.
/// @param [out] openIpcError If error indicates success (PROBE_STATE_PORT_ERROR_OK), openIpcError is set to OpenIPC_Error_No_Error.  Otherwise, it's set to an applicable error (OpenIPC_Error_Internal_Error by default).
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Null_Pointer if definition is null
/// @retval OpenIPC_Error_Invalid_Argument if error is not valid
/// @retval OpenIPC_Error_Null_Pointer if openIpcError is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortErrorGetOpenIpcError(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, OpenIPC_Error* openIpcError);
typedef OpenIPC_Error (* PPI_StatePortErrorGetOpenIpcError_TYPE)(const PPI_InterfaceStatePortDefinition* definition, ProbeStatePortError error, OpenIPC_Error* openIpcError);

/// @brief Describes the value of a parameter for a StatePort Event
///
///  This struct holds the Parameter, and a byte buffer storing
///  the value of the Parameter for this event.
///
typedef struct PPI_InterfaceStatePortEventParameterDescription
{
    PPI_InterfaceStatePortParameter const* Parameter;
    uint32_t ValueBufferByteSize;
    uint8_t const* ValueBuffer;
} PPI_InterfaceStatePortEventParameterDescription;

/// @brief Describes a StatePort Event indicating that the value of a StatePort has changed.
///
///  This struct holds the Definition for the StatePort whose value changed, along with the
///  Parameter values used to invoke that value change, and (of course) the new value.
///  The new value is stored in a byte buffer.
///
typedef struct PPI_InterfaceStatePortValueChangedEventDescription
{
    PPI_InterfaceStatePortDefinition const* Definition;
    uint32_t ParameterArrayLength;
    PPI_InterfaceStatePortEventParameterDescription const* ParameterArray;
    uint32_t NewValueBufferByteSize;
    uint8_t const* NewValueBuffer;
} PPI_InterfaceStatePortValueChangedEventDescription;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_StatePortEventGetValueChangedEventDescription
//
/// @brief This function gets the description of an event.
///
/// This function must be called from within an event handler that is handling a PPI_statePortValueChanged
/// event. Calling this function in any other context will return an error.
///
/// If this function is successful, eventDescriptionPtr will point to a PPI_InterfaceStatePortValueChangedEventDescription
/// instance that is populated with values for the event you are processing. This instance is only valid
/// while you are in the event handler. If you need any value from the event description, you will have
/// to copy them before exiting the event handler.
///
/// @param [in] deviceId The interface that the event originated from.
/// @param [out] eventDescriptionPtr Will be set to reference a PPI_InterfaceStatePortEventDescription for the event.
/// @return (OpenIPC_Error)
/// @retval OpenIPC_Error_Invalid_Device_ID if deviceId is invalid
/// @retval OpenIPC_Error_Operation_Not_Supported if this is called from a thread that isn't servicing a PPI_statePortValueChanged event.
/// @retval OpenIPC_Error_Null_Pointer if eventDescriptionPtr is null
///
PROBEPLUGIN_API OpenIPC_Error PPI_StatePortEventGetValueChangedEventDescription(OpenIPC_DeviceId deviceId, PPI_InterfaceStatePortValueChangedEventDescription const** eventDescriptionPtr);
typedef OpenIPC_Error (* PPI_StatePortEventGetValueChangedEventDescription_TYPE)(OpenIPC_DeviceId deviceId, PPI_InterfaceStatePortValueChangedEventDescription const** eventDescriptionPtr);

#if defined(__cplusplus)
    } // extern "C"
#endif

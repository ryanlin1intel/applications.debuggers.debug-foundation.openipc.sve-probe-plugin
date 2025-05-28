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
/// @file
///
/// @page Overview_Error Overview: Error Foundation Library
///
/// The Error foundation library provides the functionality for defining and
/// handling error codes/messages.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

///
/// @brief An OpenIPC error code.
typedef unsigned int OpenIPC_Component;
typedef unsigned int OpenIPC_Error;

///
/// @brief The error code used when no error has occurred.
const OpenIPC_Error OpenIPC_Error_No_Error = 0;

/*[[[cog
import cog
import ErrorCodes

componentMap = dict()
for e in ErrorCodes.error_codes:
  if e[0] not in componentMap:
      componentMap[e[0]] = len(componentMap) + 1
      cog.outl("")
      cog.outl("const OpenIPC_Component OpenIPC_Component_{:<19} = 0x{:08X};".format(e[0], componentMap[e[0]]))
  cog.outl("const OpenIPC_Error OpenIPC_Error_{:<40} = 0x{:08X};  // {}".format(e[2], (componentMap[e[0]] << 16) + e[3], e[4]))
]]]*/

const OpenIPC_Component OpenIPC_Component_DeviceControl       = 0x00000001;
const OpenIPC_Error OpenIPC_Error_DeviceControl_Invalid_Config             = 0x00010000;  // The provided deviceConfigName is not a valid configuration setting for this device.
const OpenIPC_Error OpenIPC_Error_DeviceControl_Invalid_Config_Value       = 0x00010001;  // The provided value is not a valid value for the configuration.
const OpenIPC_Error OpenIPC_Error_DeviceControl_Invalid_Action             = 0x00010002;  // The provided deviceActionName is not a an action supported by this device.

const OpenIPC_Component OpenIPC_Component_Common              = 0x00000002;
const OpenIPC_Error OpenIPC_Error_Null_Pointer                             = 0x00020000;  // An argument is NULL when it should not be
const OpenIPC_Error OpenIPC_Error_Bad_Argument                             = 0x00020001;  // An argument did not contain an expected value
const OpenIPC_Error OpenIPC_Error_Not_Supported                            = 0x00020002;  // The function or operation is not supported
const OpenIPC_Error OpenIPC_Error_Not_Implemented                          = 0x00020003;  // The function or operation is not yet implemented
const OpenIPC_Error OpenIPC_Error_Obsolete                                 = 0x00020004;  // The function or operation is obsolete and will not work
const OpenIPC_Error OpenIPC_Error_Invalid_Device_ID                        = 0x00020005;  // The device ID does not correspond to any known device
const OpenIPC_Error OpenIPC_Error_Invalid_Address_Type                     = 0x00020006;  // The type of the address is not recognized
const OpenIPC_Error OpenIPC_Error_Invalid_Address                          = 0x00020007;  // The address is not recognized as an address
const OpenIPC_Error OpenIPC_Error_Not_A_Thread                             = 0x00020008;  // The device ID does not correspond to a thread
const OpenIPC_Error OpenIPC_Error_Internal_Error                           = 0x00020009;  // An unknown error occurred
const OpenIPC_Error OpenIPC_Error_Structure_Size_Not_Initialized           = 0x0002000A;  // Structure size field is not set
const OpenIPC_Error OpenIPC_Error_Initialization_Failed                    = 0x0002000B;  // OpenIPC failed to initialize
const OpenIPC_Error OpenIPC_Error_Not_A_GPC_Thread                         = 0x0002000C;  // The device ID does not correspond to a GPC thread
const OpenIPC_Error OpenIPC_Error_Invalid_Device                           = 0x0002000D;  // The specified device cannot be used with the operation
const OpenIPC_Error OpenIPC_Error_Invalid_Argument                         = 0x0002000E;  // The argument is not valid
const OpenIPC_Error OpenIPC_Error_No_Suitable_Port                         = 0x00020010;  // The OpenIPC server failed to find a free port to bind to
const OpenIPC_Error OpenIPC_Error_Cannot_Bind_To_Port                      = 0x00020011;  // The OpenIPC server failed to bind to the given port
const OpenIPC_Error OpenIPC_Error_Cannot_Open_File                         = 0x00020012;  // The given file cannot be opened
const OpenIPC_Error OpenIPC_Error_Already_Initialized                      = 0x00020013;  // The OpenIPC server has already been initialized
const OpenIPC_Error OpenIPC_Error_Missing_Probe                            = 0x00020016;  // MISSING_PROBE_ERROR_STRING
const OpenIPC_Error OpenIPC_Error_File_Already_Exists                      = 0x00020017;  // The given file already exists
const OpenIPC_Error OpenIPC_Error_Target_Reported_Error                    = 0x00020018;  // The target reported a target specific error

const OpenIPC_Component OpenIPC_Component_Authorization       = 0x00000003;
const OpenIPC_Error OpenIPC_Error_Authorization_Not_Initialized            = 0x00030000;  // Authorization has not been initialized
const OpenIPC_Error OpenIPC_Error_Authorization_Initialization_Failed      = 0x00030001;  // Authorization failed to initialize
const OpenIPC_Error OpenIPC_Error_Authorization_Server_Unreachable         = 0x00030002;  // Server unreachable for either network or other issues
const OpenIPC_Error OpenIPC_Error_Authorization_Credentials_Invalid        = 0x00030003;  // Invalid credentials supplied for authorization
const OpenIPC_Error OpenIPC_Error_Authorization_Failure                    = 0x00030004;  // Device could not be authorized
const OpenIPC_Error OpenIPC_Error_Authorization_Denied                     = 0x00030005;  // Credentials supplied do not allow authorization of the requested level
const OpenIPC_Error OpenIPC_Error_Authorization_Level_Not_Supported        = 0x00030006;  // Device does not support authorization to the level specified
const OpenIPC_Error OpenIPC_Error_Authorization_Device_Not_Supported       = 0x00030007;  // Device does not support authorization

const OpenIPC_Component OpenIPC_Component_Jtag                = 0x00000004;
const OpenIPC_Error OpenIPC_Error_Invalid_JtagState                        = 0x00040000;  // The specified JTAG state is invalid
const OpenIPC_Error OpenIPC_Error_Invalid_JtagWaitType                     = 0x00040001;  // The specified JTAG wait type is invalid
const OpenIPC_Error OpenIPC_Error_Invalid_JtagResetType                    = 0x00040002;  // The specified JTAG reset type is invalid
const OpenIPC_Error OpenIPC_Error_JtagChain_Not_Locked                     = 0x00040003;  // The specified JTAG chain is not locked
const OpenIPC_Error OpenIPC_Error_Bad_Chain_Position                       = 0x00040004;  // A bad position into a JTAG chain has been specified
const OpenIPC_Error OpenIPC_Error_Device_Not_In_Chain                      = 0x00040005;  // A device ID was referenced that isn't in the chain
const OpenIPC_Error OpenIPC_Error_Chain_Is_Empty                           = 0x00040006;  // The specified chain has no devices
const OpenIPC_Error OpenIPC_Error_Bad_Shift_State                          = 0x00040007;  // The specified destination TAP state is not supported
const OpenIPC_Error OpenIPC_Error_Bad_Probe_Status                         = 0x00040008;  // The probe implementation returned an unexpected error
const OpenIPC_Error OpenIPC_Error_Shift_Verify_Failure                     = 0x00040009;  // A shift operation with verification failed
const OpenIPC_Error OpenIPC_Error_IrShift_Verify_Failure                   = 0x0004000A;  // Couldn't verify an IR scan. This probably indicates TDI->TDO continuity is out-of-sync with the SW
const OpenIPC_Error OpenIPC_Error_DrShift_Verify_Failure                   = 0x0004000B;  // Couldn't verify the DR scan. This indicates either the wrong DR shift length was requested, or the TDI->TDO continuity is out-of-sync with the SW, or the DR register is not IEEE 1149.12 compliant
const OpenIPC_Error OpenIPC_Error_Unpaired_Unlock                          = 0x0004000C;  // An unlock was requested with no apparent prior lock
const OpenIPC_Error OpenIPC_Error_Chain_Already_Exists                     = 0x0004000D;  // Tap chain already exists
const OpenIPC_Error OpenIPC_Error_Tap_Serialization_Failed                 = 0x0004000E;  // Failed to serialize a TAP
const OpenIPC_Error OpenIPC_Error_Tap_Deserialization_Failed               = 0x0004000F;  // Failed to deserialize a TAP
const OpenIPC_Error OpenIPC_Error_Autodetect_Failed                        = 0x00040010;  // Unable to autodetect shift length
const OpenIPC_Error OpenIPC_Error_Bad_Shift_Length                         = 0x00040011;  // Shift length must be > 0 and <= 2^^24
const OpenIPC_Error OpenIPC_Error_Bad_State_Count                          = 0x00040012;  // Clock count for TAP state must be > 0 and <= 2^^24
const OpenIPC_Error OpenIPC_Error_Chain_Not_Defined                        = 0x00040013;  // Tap chain ID referenced is not defined
const OpenIPC_Error OpenIPC_Error_Encrypted_Debug_Not_Started              = 0x00040014;  // An enDebug session has not been started for the specified TAP device
const OpenIPC_Error OpenIPC_Error_Invalid_JtagRepeatType                   = 0x00040015;  // The specified JTAG repeat type is invalid
const OpenIPC_Error OpenIPC_Error_Jtag_Mark_Nesting_Not_Allowed            = 0x00040016;  // JTAG Mark Repeat loop does not support nested loops.
const OpenIPC_Error OpenIPC_Error_Jtag_Repeat_Empty_Loop                   = 0x00040017;  // JTAG Mark/Repeat body must have at least one JTAG operation.
const OpenIPC_Error OpenIPC_Error_Jtag_Repeat_Without_Mark                 = 0x00040018;  // JTAG Repeat operation attempted without first calling Mark.
const OpenIPC_Error OpenIPC_Error_Jtag_Compare_Failed                      = 0x00040019;  // JTAG Compare the read data was not equal to the compare value.
const OpenIPC_Error OpenIPC_Error_Jtag_Mark_Without_Repeat                 = 0x0004001A;  // JTAG Mark called without Repeat, possibly a JTAG operation in the loop body got flushed unintentionally.

const OpenIPC_Component OpenIPC_Component_OperationReceipt    = 0x00000005;
const OpenIPC_Error OpenIPC_Error_Operation_Invalid                        = 0x00050000;  // The specified operation receipt token is invalid
const OpenIPC_Error OpenIPC_Error_Operation_No_Value                       = 0x00050001;  // The specified operation receipt does not have a value associated with it
const OpenIPC_Error OpenIPC_Error_Operation_Already_Completed              = 0x00050002;  // The associated operation has already been executed and cannot be canceled
const OpenIPC_Error OpenIPC_Error_Operation_Scope_Mismatch                 = 0x00050003;  // Attempt to close a scope which was not opened

const OpenIPC_Component OpenIPC_Component_Probe               = 0x00000006;
const OpenIPC_Error OpenIPC_Error_T32_DLL_Init_Fail                        = 0x00060000;  // Error initializing LTB dll
const OpenIPC_Error OpenIPC_Error_Arium_DLL_Init_Fail                      = 0x00060001;  // Error initializing Arium dll
const OpenIPC_Error OpenIPC_Error_FTDI_DLL_Init_Fail                       = 0x00060002;  // Error initializing FTDI dll.
const OpenIPC_Error OpenIPC_Error_TPV_Library_Error                        = 0x00060003;  // Third party library error
const OpenIPC_Error OpenIPC_Error_TPV_Probe_Programming_Error              = 0x00060005;  // An error occurred while programming the probe
const OpenIPC_Error OpenIPC_Error_TPV_Probe_Connection_Error               = 0x00060006;  // Probe could not connect to the target
const OpenIPC_Error OpenIPC_Error_TPV_Probe_Transport_Not_Found_Error      = 0x00060007;  // A requested transport was not found
const OpenIPC_Error OpenIPC_Error_TPV_Probe_Transport_State_Error          = 0x00060008;  // Probe transport is not in a usable state

const OpenIPC_Component OpenIPC_Component_RunControl          = 0x00000007;
const OpenIPC_Error OpenIPC_Error_Run_Control_Plugin_Not_Found             = 0x00070000;  // The specified run control plugin does not exist
const OpenIPC_Error OpenIPC_Error_No_Run_Control_Plugins_Found             = 0x00070001;  // No run control control plugins were found
const OpenIPC_Error OpenIPC_Error_Run_Control_Not_Initialized              = 0x00070002;  // Run control is not initialized: Ensure that a run control plugin is enabled in the OpenIPC configuration
const OpenIPC_Error OpenIPC_Error_Run_Control_Disabled                     = 0x00070003;  // Run control functionality has been disabled for the targeted thread or core group

const OpenIPC_Component OpenIPC_Component_ExecutionControl    = 0x00000008;
const OpenIPC_Error OpenIPC_Error_ExecutionControl                         = 0x00080000;  // ExecutionControl

const OpenIPC_Component OpenIPC_Component_Break               = 0x00000009;
const OpenIPC_Error OpenIPC_Error_Break                                    = 0x00090000;  // Break

const OpenIPC_Component OpenIPC_Component_Breakpoint          = 0x0000000A;
const OpenIPC_Error OpenIPC_Error_Breakpoint                               = 0x000A0000;  // Breakpoint

const OpenIPC_Component OpenIPC_Component_BreakpointManagement = 0x0000000B;
const OpenIPC_Error OpenIPC_Error_BreakpointManagement                     = 0x000B0000;  // BreakpointManagement

const OpenIPC_Component OpenIPC_Component_Memory              = 0x0000000C;
const OpenIPC_Error OpenIPC_Error_Memory                                   = 0x000C0000;  // Memory

const OpenIPC_Component OpenIPC_Component_Register            = 0x0000000D;
const OpenIPC_Error OpenIPC_Error_Register                                 = 0x000D0000;  // Register

const OpenIPC_Component OpenIPC_Component_TargetManager       = 0x0000000E;
const OpenIPC_Error OpenIPC_Error_TargetManager_Not_Initialized            = 0x000E0000;  // TargetManager has not been initialized
const OpenIPC_Error OpenIPC_Error_Invalid_Device_Type                      = 0x000E0002;  // The device type is invalid
const OpenIPC_Error OpenIPC_Error_Invalid_Device_Identifier                = 0x000E0003;  // The device identifier does not correspond to a device
const OpenIPC_Error OpenIPC_Error_Invalid_Target_Domain                    = 0x000E0004;  // The device does not correspond to a target domain
const OpenIPC_Error OpenIPC_Error_Invalid_Property_Name                    = 0x000E0005;  // The property name is invalid
const OpenIPC_Error OpenIPC_Error_Lock_Failed                              = 0x000E0006;  // Failed to acquire a lock on the specified device(s)
const OpenIPC_Error OpenIPC_Error_Unexpected_Unlock                        = 0x000E0007;  // Attempted to release a lock on a device without an acquired lock
const OpenIPC_Error OpenIPC_Error_Lock_Timed_Out                           = 0x000E0008;  // A time-out occurred while waiting to acquire a lock on the specified device(s)
const OpenIPC_Error OpenIPC_Error_Cannot_Lock_Device                       = 0x000E0009;  // Cannot acquire a lock on the specified device
const OpenIPC_Error OpenIPC_Error_Device_Not_Locked                        = 0x000E000A;  // A lock has not been acquired on the specified device
const OpenIPC_Error OpenIPC_Error_Device_Topology_Update_Failed            = 0x000E000B;  // Failed to update the device topology tree
const OpenIPC_Error OpenIPC_Error_Invalid_Device_Update_Phases             = 0x000E000C;  // The specified combination of device update phases are invalid
const OpenIPC_Error OpenIPC_Error_Not_Initializing                         = 0x000E000D;  // Dynamic initialization has not been started
const OpenIPC_Error OpenIPC_Error_Already_Initializing                     = 0x000E000E;  // Dynamic initialization has already been started by another client
const OpenIPC_Error OpenIPC_Error_Unknown_Probe                            = 0x000E000F;  // The specified probe is unknown
const OpenIPC_Error OpenIPC_Error_Probe_Type_Not_Supported                 = 0x000E0010;  // The specified probe type is not supported
const OpenIPC_Error OpenIPC_Error_TAP_Controller_Not_Supported             = 0x000E0011;  // The specified TAP controller is not supported
const OpenIPC_Error OpenIPC_Error_Invalid_TAP_Controller                   = 0x000E0012;  // The specified TAP controller struct is malformed
const OpenIPC_Error OpenIPC_Error_No_Saved_Configuration                   = 0x000E0013;  // There is no saved configuration file in the user's home directory
const OpenIPC_Error OpenIPC_Error_Scenario_Conflict                        = 0x000E0014;  // The specified scenario has conflicting settings with one or more scenarios selected prior

const OpenIPC_Component OpenIPC_Component_ScanEngine          = 0x0000000F;
const OpenIPC_Error OpenIPC_Error_ScanEngine_Not_Initialized               = 0x000F0000;  // ScanEngine has not been initialized
const OpenIPC_Error OpenIPC_Error_Python_Execution_Error                   = 0x000F0001;  // An error occurred while executing Python code
const OpenIPC_Error OpenIPC_Error_Invalid_Python_Function                  = 0x000F0002;  // The given Python function does not exist

const OpenIPC_Component OpenIPC_Component_TargetData          = 0x00000010;
const OpenIPC_Error OpenIPC_Error_TargetData                               = 0x00100000;  // TargetData

const OpenIPC_Component OpenIPC_Component_StatePort           = 0x00000011;
const OpenIPC_Error OpenIPC_Error_StatePort_Timeout                        = 0x00110000;  // Timed out trying to access the state port.
const OpenIPC_Error OpenIPC_Error_StatePort_Not_An_Implicit_Address_Field  = 0x00110001;  // The addressFieldName does not correspond to an implicit address field.
const OpenIPC_Error OpenIPC_Error_StatePort_Unsupported_Device             = 0x00110002;  // The specified device does not support any state ports.
const OpenIPC_Error OpenIPC_Error_StatePort_Invalid_StatePort_Name         = 0x00110003;  // The specified device does not support the requested state port.
const OpenIPC_Error OpenIPC_Error_StatePort_Invalid_AddressField_Name      = 0x00110004;  // The AddressFieldName parameter specifies an address field that does not exist.
const OpenIPC_Error OpenIPC_Error_StatePort_Invalid_Address                = 0x00110005;  // The specified AddressFields contain an error.
const OpenIPC_Error OpenIPC_Error_StatePort_Poll_Timeout                   = 0x00110006;  // Timed out waiting for value to appear in the state port.
const OpenIPC_Error OpenIPC_Error_StatePort_Endpoint_Failure               = 0x00110007;  // Received a failure from the endpoint of the state port.

const OpenIPC_Component OpenIPC_Component_ProbeVendorAPI      = 0x00000012;
const OpenIPC_Error OpenIPC_Error_LibraryLoadFailure                       = 0x00120000;  // Unable to load the probe vendor library
const OpenIPC_Error OpenIPC_Error_LibraryProcFailure                       = 0x00120001;  // Unable to get one or more procedure addresses from the probe vendor library
const OpenIPC_Error OpenIPC_Error_DebuggerConnectFailure                   = 0x00120002;  // Unable to attach probe vendor library to the debugger
const OpenIPC_Error OpenIPC_Error_DebuggerNotFound                         = 0x00120003;  // The vendor debugger was not found at the expected location
const OpenIPC_Error OpenIPC_Error_DebuggerInvokeFailure                    = 0x00120004;  // Unable to invoke the vendor debugger
const OpenIPC_Error OpenIPC_Error_VendorApiError                           = 0x00120005;  // Error returned by the vendor's API
const OpenIPC_Error OpenIPC_Error_ProbeNotInitialized                      = 0x00120006;  // Probe has not been initialized
const OpenIPC_Error OpenIPC_Error_DeviceHasNoPortSpecified                 = 0x00120007;  // The targeted device has no ports specified in the OpenIPC config XML file
const OpenIPC_Error OpenIPC_Error_UnableToAttachToDebugger                 = 0x00120008;  // Unable to attach to the vendor debugger
const OpenIPC_Error OpenIPC_Error_LibraryVersionError                      = 0x00120009;  // The vendor library is less than the minimum required version

const OpenIPC_Component OpenIPC_Component_IpcInterface        = 0x00000013;
const OpenIPC_Error OpenIPC_Error_IpcInterface                             = 0x00130000;  // IpcInterface

const OpenIPC_Component OpenIPC_Component_BitData             = 0x00000014;
const OpenIPC_Error OpenIPC_Error_BitData_Invalid                          = 0x00140000;  // OpenIPC_BitData is not valid
const OpenIPC_Error OpenIPC_Error_BitData_Capacity_Exceeded                = 0x00140001;  // OpenIPC_BitData capacity exceeded
const OpenIPC_Error OpenIPC_Error_BitData_Bit_Range_Invalid                = 0x00140002;  // OpenIPC_BitData range invalid
const OpenIPC_Error OpenIPC_Error_BitData_Bit_Offset_Invalid               = 0x00140003;  // OpenIPC_BitData offset invalid
const OpenIPC_Error OpenIPC_Error_BitData_Destination_Exists               = 0x00140004;  // OpenIPC_BitData already has a value
const OpenIPC_Error OpenIPC_Error_BitData_Bitwise_Operation_Invalid        = 0x00140005;  // Specified bitwise operation is invalid

const OpenIPC_Component OpenIPC_Component_InterfacePort       = 0x00000015;
const OpenIPC_Error OpenIPC_Error_InterfacePort_Can_Not_Open_Pipe          = 0x00150000;  // A pipe could not be created
const OpenIPC_Error OpenIPC_Error_InterfacePort_Can_Not_Connect_Pipe       = 0x00150001;  // A pipe could not be connected
const OpenIPC_Error OpenIPC_Error_InterfacePort_Can_Not_Write_To_Pipe      = 0x00150002;  // Could not write data to pipe
const OpenIPC_Error OpenIPC_Error_InterfacePort_Window_Already_Open        = 0x00150003;  // Port already has window open
const OpenIPC_Error OpenIPC_Error_InterfacePort_Offline                    = 0x00150004;  // Port is offline
const OpenIPC_Error OpenIPC_Error_InterfacePort_State_Unknown              = 0x00150005;  // Could not determine the state of the interface port
const OpenIPC_Error OpenIPC_Error_InterfacePort_Operation_Failed           = 0x00150006;  // An interface port read or write operation failed
const OpenIPC_Error OpenIPC_Error_InterfacePort_Operation_Invalid          = 0x00150007;  // An interface port operation invalid
const OpenIPC_Error OpenIPC_Error_InterfacePort_AccessMode_Not_Supported   = 0x00150008;  // Interface port does not support the configured access mode
const OpenIPC_Error OpenIPC_Error_InterfacePort_Window_Not_Open            = 0x00150009;  // Window not open

const OpenIPC_Component OpenIPC_Component_ProbePlugin         = 0x00000016;
const OpenIPC_Error OpenIPC_Error_Plugin_Is_Not_Initialized                = 0x00160001;  // The plugin should be initialized before this operation is attempted
const OpenIPC_Error OpenIPC_Error_Probe_Bundle_Invalid                     = 0x00160002;  // The probe bundle handle is not valid
const OpenIPC_Error OpenIPC_Error_Probe_Bundle_Invalid_Handle              = 0x00160003;  // The probe bundle handle has already been executed, so no new commands can be added
const OpenIPC_Error OpenIPC_Error_Probe_Bundle_Invalid_Slot                = 0x00160004;  // The probe bundle handle does not have the indicated slot
const OpenIPC_Error OpenIPC_Error_Probe_Invalid_JTAG_Device                = 0x00160005;  // The device given is not a JTAG device.
const OpenIPC_Error OpenIPC_Error_Probe_Invalid_Parameter                  = 0x00160006;  // A parameters value is invalid.
const OpenIPC_Error OpenIPC_Error_Probe_Invalid_Size                       = 0x00160007;  // A parameter's size is insufficient to handle the output.
const OpenIPC_Error OpenIPC_Error_Plugin_Not_Initialized                   = 0x00160008;  // Unable to initialize the plug-in
const OpenIPC_Error OpenIPC_Error_Probe_Not_Initialized                    = 0x00160009;  // Unable to initialize the probe
const OpenIPC_Error OpenIPC_Error_Interface_Not_Initialized                = 0x0016000A;  // Unable to initialize the interface
const OpenIPC_Error OpenIPC_Error_Interface_Not_Supported                  = 0x0016000B;  // The requested interface is not supported by this probe type
const OpenIPC_Error OpenIPC_Error_Plugin_Not_Found                         = 0x0016000C;  // No probe plugin could be found
const OpenIPC_Error OpenIPC_Error_Writeback_Size_Mismatch                  = 0x0016000D;  // The probe API returned an unexpected amount of read data (or none at all)
const OpenIPC_Error OpenIPC_Error_Probe_Request_Timeout                    = 0x0016000E;  // A request to the probe has timed out
const OpenIPC_Error OpenIPC_Error_Probe_Config_file                        = 0x0016000F;  // A file used to configure the probe was not found
const OpenIPC_Error OpenIPC_Error_Plugin_Already_Initialized               = 0x00160010;  // Plug-in is already initialized
const OpenIPC_Error OpenIPC_Error_Probe_Already_Initialized                = 0x00160011;  // Probe is already initialized
const OpenIPC_Error OpenIPC_Error_Interface_Already_Initialized            = 0x00160012;  // Interface is already initialized
const OpenIPC_Error OpenIPC_Error_Callback_Already_Initialized             = 0x00160013;  // Callback being registered more than once
const OpenIPC_Error OpenIPC_Error_Interface_Jtag_Boundary_Scan_Timeout     = 0x00160014;  // A PRDY# signal pulse from the core was not seen after submitting a probemode assembly instruction (PIR/PDR). It is possible the pin is not wired or was not recovered properly (platform bug). If a PRDY# pin issue is suspected, please try the following workaround: ipc.config.gpc.RunControl.WaitInstructionCompletionMode = 'DeadWait' and ipc.config.gpc.RunControl.WaitInstructionTimeout= '2000'
const OpenIPC_Error OpenIPC_Error_Interface_Jtag_Mark_Return               = 0x00160015;  // Jtag Mark and Return error occurred
const OpenIPC_Error OpenIPC_Error_Interface_Jtag_Tdi_Fill                  = 0x00160016;  // Jtag TDI Fill error occurred
const OpenIPC_Error OpenIPC_Error_Interface_Jtag_Save                      = 0x00160017;  // Jtag Save error occurred
const OpenIPC_Error OpenIPC_Error_Interface_Jtag_Restore                   = 0x00160018;  // Jtag Restore error occurred
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Arbitration_Loss           = 0x00160019;  // I2c Arbitration Loss error occurred
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Nack_Before_Length_Complete = 0x0016001A;  // I2c Nack before Length Complete error occurred
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Retry_Failed               = 0x0016001B;  // I2c Retry failed
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Nack_At_Address_Phase      = 0x0016001C;  // I2c Nack at Address Phase or Last Write Data
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Fifo_Empty                 = 0x0016001D;  // I2c FIFO empty before scan complete
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Sda_Wire_Stuck_Low         = 0x0016001E;  // I2c SDA Wire Stuck Low
const OpenIPC_Error OpenIPC_Error_Interface_I2c_Sda_Wire_Stuck_High        = 0x0016001F;  // I2c SDA Wire Stuck High
const OpenIPC_Error OpenIPC_Error_Interface_Jtag                           = 0x00160020;  // Jtag Error Occurred
const OpenIPC_Error OpenIPC_Error_Interface_I2c                            = 0x00160021;  // I2c Error Occurred
const OpenIPC_Error OpenIPC_Error_Interface_Obs                            = 0x00160022;  // Obs Error Occurred
const OpenIPC_Error OpenIPC_Error_Probe_Config_Undefined                   = 0x00160023;  // Probe Config is Undefined
const OpenIPC_Error OpenIPC_Error_Interface_Pins                           = 0x00160024;  // Pins Interface error occurred
const OpenIPC_Error OpenIPC_Error_Interface_Pins_Operation_Not_Supported   = 0x00160025;  // Pin Operation is Not Supported
const OpenIPC_Error OpenIPC_Error_Interface_Pins_Pin_Not_Supported         = 0x00160026;  // Pin is Not Supported
const OpenIPC_Error OpenIPC_Error_Interface_PortAccessMode_Not_Supported   = 0x00160027;  // Port does not support the configured access mode
const OpenIPC_Error OpenIPC_Error_Interface_Port_Operation_Failed          = 0x00160028;  // Port operation failed
const OpenIPC_Error OpenIPC_Error_Interface_Port_Operation_Invalid         = 0x00160029;  // Port operation invalid
const OpenIPC_Error OpenIPC_Error_Plugin_Implementer_Defined               = 0x0016002A;  // 
const OpenIPC_Error OpenIPC_Error_Operation_Not_Supported                  = 0x0016002B;  // Operation is not supported
const OpenIPC_Error OpenIPC_Error_Target_Not_Available                     = 0x0016002C;  // Target is powered down or otherwise not available
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Server_Lost            = 0x0016002D;  // Remote Connection was lost to the server
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Client_Lost            = 0x0016002E;  // Remote Connection was lost to the client
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Unable_To_Connect      = 0x0016002F;  // Remote Connection was unable to connect to the server
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Unable_To_Listen       = 0x00160030;  // Remote Connection was unable to listen for a client
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Already_Connected      = 0x00160031;  // Remote Connection was unable to connect; it is already connected
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Unable_To_Send         = 0x00160032;  // Remote Connection was unable to send
const OpenIPC_Error OpenIPC_Error_Remote_Connection_Not_Connected          = 0x00160033;  // Remote Connection is not connected
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Invalid_Magic_Number = 0x00160034;  // Message with an invalid magic number was received
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Handshake_Already_In_Progress = 0x00160035;  // Connection handshake was already in progress
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Negeotiation_Not_Accpeted = 0x00160036;  // Negotiation not accepted at connect
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Server_Version_Not_Compatible = 0x00160037;  // Server and client versions not compatible
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Token_Required_At_Connect = 0x00160038;  // Token required at connect
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Invalid_Payload_Size = 0x00160039;  // Invalid payload size for message encountered
const OpenIPC_Error OpenIPC_Error_Remote_Plugin_Rpc_Fatal_Out_Of_Sequence  = 0x0016003A;  // Invalid message sequence
const OpenIPC_Error OpenIPC_Error_Target_Not_Responding                    = 0x0016003B;  // Target is not responding
const OpenIPC_Error OpenIPC_Error_Probe_Not_Available                      = 0x0016003C;  // Lost connection to the probe
const OpenIPC_Error OpenIPC_Error_Probe_Connection_Transient_Error         = 0x0016003D;  // Connection to the probe suffered a temporary error.

const OpenIPC_Component OpenIPC_Component_I2c                 = 0x00000017;
const OpenIPC_Error OpenIPC_Error_Unpaired_I2C_Unlock                      = 0x00170000;  // An Unlock was performed without a preceding matching Lock

const OpenIPC_Component OpenIPC_Component_InterfacePins       = 0x00000018;
const OpenIPC_Error OpenIPC_Error_Invalid_InterfacePins_Or_DebugPort       = 0x00180000;  // The device ID does not correspond to either interface pins or a debug port.
const OpenIPC_Error OpenIPC_Error_No_PinsInterface_Found_In_DebugPort      = 0x00180001;  // The specified debug port does not have any pins interface.
const OpenIPC_Error OpenIPC_Error_InterfacePins_Invalid_Pins_Signal        = 0x00180002;  // The specified pins signal is invalid.
const OpenIPC_Error OpenIPC_Error_InterfacePins_Invalid_Pins_State         = 0x00180003;  // The specified pins state is invalid.
const OpenIPC_Error OpenIPC_Error_InterfacePins_Manual_Pin_Driving_Prevented = 0x00180004;  // Manual driving of the specified pins signal is prevented.

const OpenIPC_Component OpenIPC_Component_Rpc                 = 0x00000019;
const OpenIPC_Error OpenIPC_Error_Client_Connect_Error                     = 0x00190000;  // Error connecting to server
const OpenIPC_Error OpenIPC_Error_Cannot_Find_Server_Exe                   = 0x00190001;  // Error finding server executable
const OpenIPC_Error OpenIPC_Error_Cannot_Start_Server_Exe                  = 0x00190002;  // Error starting server executable

const OpenIPC_Component OpenIPC_Component_Precondition        = 0x0000001A;
const OpenIPC_Error OpenIPC_Error_Precondition_Not_Initialized             = 0x001A0000;  // Precondition component has not been initialized
const OpenIPC_Error OpenIPC_Error_Precondition_Initialization_Failed       = 0x001A0001;  // Precondition component failed to initialize
const OpenIPC_Error OpenIPC_Error_Invalid_Precondition_Name                = 0x001A0002;  // The specified precondition does not exist for the device
const OpenIPC_Error OpenIPC_Error_Invalid_Precondition_ID                  = 0x001A0003;  // The specified precondition does not exist for the device
const OpenIPC_Error OpenIPC_Error_Invalid_Precondition_State               = 0x001A0004;  // The specified precondition state is invalid
const OpenIPC_Error OpenIPC_Error_Precondition_Already_Exists              = 0x001A0005;  // The specified precondition of the specified name already exists for the device
const OpenIPC_Error OpenIPC_Error_Precondition_Requirement_Failed          = 0x001A0006;  // The scan executed to enforce a precondition requirement has failed
const OpenIPC_Error OpenIPC_Error_Precondition_Requirement_Immutable       = 0x001A0007;  // The precondition's state cannot be manually mutated
const OpenIPC_Error OpenIPC_Error_Precondition_Scope_Already_Open          = 0x001A0008;  // A precondition scope is already open on the current thread
const OpenIPC_Error OpenIPC_Error_Precondition_Scope_Not_Open              = 0x001A0009;  // A precondition scope is not open on the current thread
const OpenIPC_Error OpenIPC_Error_Precondition_Acquisition_Failed          = 0x001A000A;  // The precondition requirement could not be acquired

const OpenIPC_Component OpenIPC_Component_PlatformControl     = 0x0000001B;
const OpenIPC_Error OpenIPC_Error_Invalid_Debugport_ID                     = 0x001B0000;  // The specified debugPortId is invalid.
const OpenIPC_Error OpenIPC_Error_Invalid_Power_Domain_Name                = 0x001B0002;  // The specified name does not correspond to a valid power domain.
const OpenIPC_Error OpenIPC_Error_PlatformControl_Invalid_Preset           = 0x001B0003;  // The given preset is invalid.
const OpenIPC_Error OpenIPC_Error_PlatformControl_Invalid_Survivability_Level = 0x001B0004;  // The given feature survivability level is invalid.

const OpenIPC_Component OpenIPC_Component_ExecutionControlPlugin = 0x0000001C;
const OpenIPC_Error OpenIPC_Error_Plugin_Is_Initialized                    = 0x001C0000;  // Plugin should be initialized before this operation is attempted.
const OpenIPC_Error OpenIPC_Error_Plugin_Is_Already_Initialized            = 0x001C0001;  // The plugin has already been initialized.
const OpenIPC_Error OpenIPC_Error_Proc_Not_Halted                          = 0x001C0002;  // Processor is not halted
const OpenIPC_Error OpenIPC_Error_Proc_Halted                              = 0x001C0003;  // Processor is already halted
const OpenIPC_Error OpenIPC_Error_Invalid_Step_Type                        = 0x001C0004;  // The step type is not recognized
const OpenIPC_Error OpenIPC_Error_Invalid_Step_Count                       = 0x001C0005;  // The step count is 0 or negative
const OpenIPC_Error OpenIPC_Error_Cannot_Step_Instruction                  = 0x001C0006;  // Stepping is not allowed when the processor is in HLT, SIPI, TPD or Shutdown state
const OpenIPC_Error OpenIPC_Error_Target_In_Reset                          = 0x001C0007;  // Target is in reset
const OpenIPC_Error OpenIPC_Error_Arium_Internal_Error                     = 0x001C0008;  // An unknown error has occurred in Arium tool, please reset the target.
const OpenIPC_Error OpenIPC_Error_LTB_Internal_Error                       = 0x001C0009;  // An unknown error has occurred in T32, please reset the target.
const OpenIPC_Error OpenIPC_Error_LTB_Attach_Error                         = 0x001C000A;  // Cannot attach with the Trace32 debugger.
const OpenIPC_Error OpenIPC_Error_LTB_Coms_Error                           = 0x001C000B;  // Cannot communicate with the Trace32 debugger.
const OpenIPC_Error OpenIPC_Error_LTB_Probe_Not_Initialized                = 0x001C000C;  // Lauterbach probe is not initialized yet.
const OpenIPC_Error OpenIPC_Error_Arium_Probe_Not_Supported                = 0x001C000D;  // Arium probe is yet to be supported
const OpenIPC_Error OpenIPC_Error_Unable_To_Switch_Current_Thread          = 0x001C000E;  // Unable to switch to the targeted thread number.
const OpenIPC_Error OpenIPC_Error_Unable_To_Read_Current_Thread            = 0x001C000F;  // Unable to read the current thread number.
const OpenIPC_Error OpenIPC_Error_Unable_To_Determine_DeviceType           = 0x001C0010;  // Unable to determine the device type from the deviceId passed in.
const OpenIPC_Error OpenIPC_Error_Unable_To_Determine_DeviceIdentifier     = 0x001C0011;  // Unable to determine the device identifier from the deviceId passed in.
const OpenIPC_Error OpenIPC_Error_Unable_To_Determine_ThreadId             = 0x001C0012;  // Unable to determine the thread Id from the deviceId passed in
const OpenIPC_Error OpenIPC_Error_Unable_To_Determine_ThreadNumber         = 0x001C0013;  // Unable to determine the thread number from the deviceId passed in
const OpenIPC_Error OpenIPC_Error_Unable_To_Find_GPC_Threads               = 0x001C0014;  // There were no general purpose computing threads found in the platform.
const OpenIPC_Error OpenIPC_Error_Requested_T32_Operation_Failed           = 0x001C0015;  // The requested operation has failed in Trace32 debugger.
const OpenIPC_Error OpenIPC_Error_Target_Already_Running                   = 0x001C0016;  // All targeted processors are already running.
const OpenIPC_Error OpenIPC_Error_Halt_Timeout                             = 0x001C0017;  // Timeout while attempting to halt processor.
const OpenIPC_Error OpenIPC_Error_Instruction_Syntax_Error                 = 0x001C0018;  // The syntax of the submitted instruction (mnemonic) is invalid.
const OpenIPC_Error OpenIPC_Error_Instruction_Fault                        = 0x001C0019;  // The CPU indicated a fault while executing the supplied instruction.

const OpenIPC_Component OpenIPC_Component_RegisterPlugin      = 0x0000001D;
const OpenIPC_Error OpenIPC_Error_Register_Invalid                         = 0x001D0000;  // Invalid register name
const OpenIPC_Error OpenIPC_Error_CPUID_Output_Register_Invalid            = 0x001D0001;  // The specified CPUID output register is not valid
const OpenIPC_Error OpenIPC_Error_MSR_Address_Invalid                      = 0x001D0002;  // Invalid MSR address specified
const OpenIPC_Error OpenIPC_Error_MSR_Access_Faulted                       = 0x001D0003;  // The CPU has faulted while accessing the msr, possibly it is either non-existent for this processor family or has read/write-only bits
const OpenIPC_Error OpenIPC_Error_Unable_To_Read_CpuId                     = 0x001D0004;  // Unable to retrieve the CPUID from T32
const OpenIPC_Error OpenIPC_Error_Register_Plugin_Not_Initialized          = 0x001D0005;  // Unable to perform operation because the register access plugin is not initialized.

const OpenIPC_Component OpenIPC_Component_BreakManagerPlugin  = 0x0000001E;
const OpenIPC_Error OpenIPC_Error_Invalid_Break_Name                       = 0x001E0000;  // No break condition with the specified name exists
const OpenIPC_Error OpenIPC_Error_Invalid_Stall_Name                       = 0x001E0001;  // No system stall with the specified name exists
const OpenIPC_Error OpenIPC_Error_Unable_To_Get_BreakCondition             = 0x001E0002;  // Unable to retrieve the break condition
const OpenIPC_Error OpenIPC_Error_Unable_To_Set_BreakCondition             = 0x001E0003;  // Unable to set the break condition
const OpenIPC_Error OpenIPC_Error_Breakpoint_ID_Exists                     = 0x001E0004;  // A breakpoint with the specified ID already exists
const OpenIPC_Error OpenIPC_Error_Breakpoint_Name_Exists                   = 0x001E0005;  // A breakpoint with the specified name already exists
const OpenIPC_Error OpenIPC_Error_Breakpoint_Software_Break_Exists         = 0x001E0006;  // A software breakpoint with specified address already exists
const OpenIPC_Error OpenIPC_Error_Invalid_Breakpoint_ID                    = 0x001E0007;  // The specified breakpoint ID does not correspond to any valid breakpoint.
const OpenIPC_Error OpenIPC_Error_Invalid_Breakpoint_Type                  = 0x001E0008;  // The specified breakpoint type is not valid.
const OpenIPC_Error OpenIPC_Error_Invalid_Breakpoint_Data_Size             = 0x001E0009;  // The specified breakpoint data size is not valid (must be 1, 2, 4, or 8).
const OpenIPC_Error OpenIPC_Error_Invalid_Breakpoint_Debug_Register        = 0x001E000A;  // The specified breakpoint debug register is not valid (must be 0, 1, 2, or 3).
const OpenIPC_Error OpenIPC_Error_BreakIdsGotExchausted                    = 0x001E000B;  // Cannot create break: no more break IDs available
const OpenIPC_Error OpenIPC_Error_UnableToInstallTheBreak                  = 0x001E000C;  // Unable to install the specified breakpoint
const OpenIPC_Error OpenIPC_Error_UnableToRemoveTheBreak                   = 0x001E000D;  // Unable to remove the specified breakpoint
const OpenIPC_Error OpenIPC_Error_UnableToEnableTheBreak                   = 0x001E000E;  // Unable to enable the specified breakpoint
const OpenIPC_Error OpenIPC_Error_UnableToDisableTheBreak                  = 0x001E000F;  // Unable to disable the specified breakpoint
const OpenIPC_Error OpenIPC_Error_Invalid_BreakpointManagement_Operation   = 0x001E0010;  // Invalid breakpoint management operation
const OpenIPC_Error OpenIPC_Error_Breakpoint_Hardware_Break_Exists         = 0x001E0011;  // A hardware breakpoint with specified address already exists

const OpenIPC_Component OpenIPC_Component_MemoryPlugin        = 0x0000001F;
const OpenIPC_Error OpenIPC_Error_Memory_Not_In_VMX_Mode                   = 0x001F0000;  // VMCS access is not allowed because the thread is not running in virtual memory mode
const OpenIPC_Error OpenIPC_Error_Memory_VMCS_Encoding_Invalid             = 0x001F0001;  // The specified VMCS encoding for a field is invalid for the given thread
const OpenIPC_Error OpenIPC_Error_Memory_Invalid_Address_String            = 0x001F0002;  // The specified string does not represent a recognized address
const OpenIPC_Error OpenIPC_Error_Memory_Invalid_Port_Size                 = 0x001F0003;  // The specified port size is invalid
const OpenIPC_Error OpenIPC_Error_LTB_Malloc_Fail                          = 0x001F0004;  // Memory allocation failed
const OpenIPC_Error OpenIPC_Error_Memory_Address_Xlat_Not_Supported        = 0x001F0005;  // The requested address translation is not yet supported by Trace32 debug tool
const OpenIPC_Error OpenIPC_Error_Memory_Invalid_Access_Width              = 0x001F0006;  // The specified memory access width is invalid
const OpenIPC_Error OpenIPC_Error_Memory_Unable_To_Load_File               = 0x001F0007;  // The specified file cannot be loaded into the memory
const OpenIPC_Error OpenIPC_Error_Memory_Unable_To_Save_File               = 0x001F0008;  // The specified file cannot be saved from the memory
const OpenIPC_Error OpenIPC_Error_Memory_Invalid_Address_Format            = 0x001F0009;  // The requested memory access operation has failed in Trace32, please check the address format
const OpenIPC_Error OpenIPC_Error_Memory_Xlat_Error                        = 0x001F000A;  // The requested memory translation failed

const OpenIPC_Component OpenIPC_Component_AsmDasmPlugin       = 0x00000020;
const OpenIPC_Error OpenIPC_Error_AsmDasm_Invalid_Instruction_Type         = 0x00200000;  // The specified instructionType parameter is invalid.
const OpenIPC_Error OpenIPC_Error_AsmDasm_Syntax_Error                     = 0x00200001;  // Syntax error occurred.
const OpenIPC_Error OpenIPC_Error_AsmDasm_Output_Buffer_Too_Small          = 0x00200002;  // The specified outputBuffer is too small.
const OpenIPC_Error OpenIPC_Error_AsmDasm_Invalid_Opcode_Error             = 0x00200003;  // Invalid opcode(s) in the specified instruction.

const OpenIPC_Component OpenIPC_Component_AsmDasm             = 0x00000021;
const OpenIPC_Error OpenIPC_Error_AsmDasm                                  = 0x00210000;  // AsmDasm

const OpenIPC_Component OpenIPC_Component_EventBroker         = 0x00000022;
const OpenIPC_Error OpenIPC_Error_EventBroker                              = 0x00220000;  // EventBroker

const OpenIPC_Component OpenIPC_Component_ProbeModeRegister   = 0x00000023;
const OpenIPC_Error OpenIPC_Error_ProbeModeRegister_Invalid_AccessType     = 0x00230000;  // AccessType must reference a ProbeModeRegister.xml ReadPdr.AccessType element.
const OpenIPC_Error OpenIPC_Error_ProbeModeRegister_Fault_Occurred         = 0x00230001;  // A submitted instruction caused a fault.
const OpenIPC_Error OpenIPC_Error_ProbeModeRegister_Not_Initialized        = 0x00230002;  // ProbeModeRegister component has not been initialized via call to Initialize.
const OpenIPC_Error OpenIPC_Error_ProbeModeRegister_UseMode_Unknown        = 0x00230003;  // UseMode for deviceId was not set.
const OpenIPC_Error OpenIPC_Error_ProbeModeRegister_Unsupported_Device_Type = 0x00230004;  // DeviceId does not correspond to a supported DeviceType/Stepping.

const OpenIPC_Component OpenIPC_Component_NamedState          = 0x00000024;
const OpenIPC_Error OpenIPC_Error_NamedState_Not_Initialized               = 0x00240000;  // NamedState has not been initialized
const OpenIPC_Error OpenIPC_Error_Invalid_State_Node_Path                  = 0x00240001;  // The specified path does not correspond to a valid state node
const OpenIPC_Error OpenIPC_Error_State_Node_Access_Type_Not_Supported     = 0x00240002;  // The state node at the specified path does not support the attempted access type
const OpenIPC_Error OpenIPC_Error_State_Node_Access_Failed                 = 0x00240003;  // Failed to read or write to the state node at the specified path
const OpenIPC_Error OpenIPC_Error_State_Node_Already_Cached                = 0x00240004;  // The state node at the specified path is already cached
const OpenIPC_Error OpenIPC_Error_State_Node_Not_Cached                    = 0x00240005;  // The state node at the specified path is not cached
const OpenIPC_Error OpenIPC_Error_State_Node_Not_Cachable                  = 0x00240006;  // The state node at the specified path is not cachable
const OpenIPC_Error OpenIPC_Error_Unflushed_State_Node                     = 0x00240007;  // A cached state node was not flushed before the associated device was unlocked

const OpenIPC_Component OpenIPC_Component_DMA                 = 0x00000025;
const OpenIPC_Error OpenIPC_Error_DMA_Initialization_Failure               = 0x00250000;  // The DMA component failed to initialize.
const OpenIPC_Error OpenIPC_Error_DMA_Config_Method_Invalid                = 0x00250001;  // The given configuration access method is invalid.
const OpenIPC_Error OpenIPC_Error_DMA_Device_Busy                          = 0x00250002;  // The DMA device is already in use (potentially by another client).
const OpenIPC_Error OpenIPC_Error_DMA_Configuration_Failed                 = 0x00250003;  // The DMA service failed to configure the DMA mechanism properly.
const OpenIPC_Error OpenIPC_Error_DMA_Transfer_Failed                      = 0x00250004;  // The DMA transfer failed.
const OpenIPC_Error OpenIPC_Error_DMA_Unrecoverable_Failure                = 0x00250005;  // The target's DMA mechanism is in an unrecoverable state.
const OpenIPC_Error OpenIPC_Error_DMA_Major_Failure                        = 0x00250006;  // The DMA mechanism is in a major failure state.
const OpenIPC_Error OpenIPC_Error_DMA_Critical_Failure                     = 0x00250007;  // The DMA mechanism is in a critical failure state.
const OpenIPC_Error OpenIPC_Error_DMA_Recovery_Operation_Failed            = 0x00250008;  // Failed to recover the DMA mechanism.

const OpenIPC_Component OpenIPC_Component_Configuration       = 0x00000026;
const OpenIPC_Error OpenIPC_Error_Invalid_Configuration                    = 0x00260000;  // OpenIPC encountered an invalid configuration file
const OpenIPC_Error OpenIPC_Error_Unknown_Configuration                    = 0x00260001;  // The specified configuration is not recognized
const OpenIPC_Error OpenIPC_Error_Not_Configured                           = 0x00260002;  // The OpenIPC server has not been configured

const OpenIPC_Component OpenIPC_Component_FeatureStaging      = 0x00000027;
const OpenIPC_Error OpenIPC_Error_Staged_Feature_Disallowed                = 0x00270000;  // A feature was used that is at a maturity stage that is configured to be disallowed.
//[[[end]]]

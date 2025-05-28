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
///  @brief Contains methods for the Pins interface.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BundleOperations.h" // for the probe itself
#include "SlotOperations.h"

#if defined(__cplusplus)
    extern "C" {
#endif

/// @brief This enum is a union of the physical pins on a XDPA/MIPI-60 connector with
///        those events defined in DCI. Additionally, there is some merging of some DCI
///        events to those that the client would care about per feedback.
///        Not all of these 'pins' are supported on every platform/probe nor are all
///        drivable/readable on every pod.
///
///        The model here is that these pins are defined *per* platform according to the
///        platform design guide assuming that we have an open chassis. Thus, any
///        remapping to maintain the semantics of that should be done by the probe plugin
///        if possible. The use of the name 'pins' is an historical accident since we are
///        starting from an open chasis model; a more accurate and inclusive
///        name might be 'signals' both to the target and to the probe/interface.
///        We also note that we've likely missed signals; in that case, we expect to add
///        additional ones as either new debug interfaces are defined or we discover that
///        the current categories are not sufficient.
///        This enum is declared so that we can use 128 bit to encode multiple pin drives/reads
DECLARE_ENUM(PPI_Pins_TypeEncode)
{
    PPI_PINS_hook0 = 0x00,        // Typically Power Good Indication
    PPI_PINS_hook1 = 0x01,        // Typically causes a cold boot
    PPI_PINS_hook2 = 0x02,        // Typically causes an early break stall in the CPU
    PPI_PINS_hook3 = 0x03,        // Typically causes an early boot stall of the platform
    PPI_PINS_hook4 = 0x04,        // Legacy
    PPI_PINS_hook5 = 0x05,        // Legacy
    PPI_PINS_hook6 = 0x06,        // Typically system reset indication from the DUT or pmode pin
    PPI_PINS_hook7 = 0x07,        // Typically causes system reset of the DUT
    PPI_PINS_hook8 = 0x08,        // Generic debug signal (MIPI-60 only)
    PPI_PINS_hook9 = 0x09,        // Generic debug signal (MIPI-60 only)
    PPI_PINS_Pod_Prsnt1_N = 0x0A, // Signals the target system that a debug tool has plugged in
    PPI_PINS_Pod_Prsnt2_N = 0x0B, // Signals the target system that a debug tool has plugged in
    PPI_PINS_Preq_N      = 0x0C,  // IA probe mode request
    PPI_PINS_Prdy_N      = 0x0D,  // IA probe mode ready
    PPI_PINS_TRST        = 0x0E,
    PPI_PINS_TMS_bit     = 0x0F,
    PPI_PINS_TDI_bit     = 0x10,
    PPI_PINS_TDO_bit     = 0x11,
    PPI_PINS_TCK_bit     = 0x12,
    PPI_PINS_PowerButton = 0x13,
    PPI_PINS_ResetButton = 0x14,
    PPI_PINS_SystemTapPowerGood = 0x15,
    PPI_PINS_CoreTapPowerGood   = 0x16,
    PPI_PINS_SystemReset   = 0x17,
    PPI_PINS_CoreReset     = 0x18,
    PPI_PINS_CorePowerGood = 0x19,
    PPI_PINS_TapReady    = 0x1A,
    PPI_PINS_PowerBreak1 = 0x1B,
    PPI_PINS_PowerBreak2 = 0x1C,
    PPI_PINS_PowerBreak3 = 0x1D,
    // These are completely virtual pins to indicate that we
    // tri-state JTAG pins or disable access to I2C
    // These pins are not intended to be used as a settable or readable pin from the pin interface--InterfaceDeinitialize is a better choice in any event for a synchronous disable--
    // rather instead as a response in a trigger response to disable JTAG/I2C access associated with the coupled JTAG/I2C interfaces.
    PPI_PINS_JtagDisable     = 0x1E,
    PPI_PINS_I2CDisable      = 0x1F,
    PPI_PINS_SystemPowerGood = 0x20,
    PPI_PINS_SystemBootStall = 0x21,       // An example of this stall is the ExI bridge stall on connect.
    PPI_PINS_C9_C10 = 0x22,                // Used for filtering PRDYs during deep sleep states
    PPI_PINS_GlobalResetEntryStall = 0x23, // Stall right before the platform undergoes a global reset
    PPI_PINS_SxEntryStall     = 0x24,      // Stall right before entering an Sx state
    PPI_PINS_SxExitStall      = 0x25,      // Stall right before exiting an Sx state
    PPI_PINS_S0ixToS0Stall    = 0x26,      // Stall right before entering S0 from an S0ix.y state
    PPI_PINS_S0i2_0EntryStall = 0x27,      // Stall right before entering S0i2.0
    PPI_PINS_S0i2_1EntryStall = 0x28,      // Stall right before entering S0i2.1
    PPI_PINS_S0i3_0EntryStall = 0x29,      // Stall right before entering S0i3.0
    PPI_PINS_S0i3_1EntryStall = 0x2A,      // Stall right before entering S0i3.1
    PPI_PINS_S0i3_2EntryStall = 0x2B,      // Stall right before entering S0i3.2
    PPI_PINS_S0i3_3EntryStall = 0x2C,      // Stall right before entering S0i3.3
    PPI_PINS_S0i3_4EntryStall = 0x2D,      // Stall right before entering S0i3.4
    PPI_PINS_IVRxToS0Stall    = 0x2E,      // Stall right before entering S0 from an IVRx state
    PPI_PINS_IVR1EntryStall   = 0x2F,      // Stall right before entering IVR1
    PPI_PINS_IVR2EntryStall   = 0x30,      // Stall right before entering IVR2
    PPI_PINS_IVR3EntryStall   = 0x31,      // Stall right before entering IVR3
    PPI_PINS_S0i2_2EntryStall = 0x32,      // Stall right before entering S0i2.2
    PPI_PINS_S0i2_3EntryStall = 0x33,      // Stall right before entering S0i2.3
    PPI_PINS_S0i1_0EntryStall = 0x34,      // Stall right before entering S0i1.0
    PPI_PINS_S0i1_1EntryStall = 0x35,      // Stall right before entering S0i1.1

    PPI_PINS_obsA_func0 = 0x7F,
    PPI_PINS_obsA_func1 = 0x7E,
    PPI_PINS_obsB_func0 = 0x7D,
    PPI_PINS_obsB_func1 = 0x7C,
    PPI_PINS_obsC_func0 = 0x7B,
    PPI_PINS_obsC_func1 = 0x7A,
    PPI_PINS_obsD_func0 = 0x79,
    PPI_PINS_obsD_func1 = 0x78,
    PPI_PINS_obsA_data0 = 0x77,
    PPI_PINS_obsA_data1 = 0x76,
    PPI_PINS_obsA_data2 = 0x75,
    PPI_PINS_obsA_data3 = 0x74,
    PPI_PINS_obsB_data0 = 0x73,
    PPI_PINS_obsB_data1 = 0x72,
    PPI_PINS_obsB_data2 = 0x71,
    PPI_PINS_obsB_data3 = 0x70,
    PPI_PINS_obsC_data0 = 0x6F,
    PPI_PINS_obsC_data1 = 0x6E,
    PPI_PINS_obsC_data2 = 0x6D,
    PPI_PINS_obsC_data3 = 0x6C,
    PPI_PINS_obsD_data0 = 0x6B,
    PPI_PINS_obsD_data1 = 0x6A,
    PPI_PINS_obsD_data2 = 0x69,
    PPI_PINS_obsD_data3 = 0x68,
    PPI_PINS_NO_ACTION  = 0x67, // This is for testing purposes... we reserve a bit here that means nothing... it's not a real trigger.

    PPI_PINS_Resource0  = 0x80,
    PPI_PINS_Resource1  = 0x81,
    PPI_PINS_Resource2  = 0x82,
    PPI_PINS_Resource3  = 0x83,
    PPI_PINS_Resource4  = 0x84,
    PPI_PINS_Resource5  = 0x85,
    PPI_PINS_Resource6  = 0x86,
    PPI_PINS_Resource7  = 0x87,
    PPI_PINS_Resource8  = 0x88,
    PPI_PINS_Resource9  = 0x89,
    PPI_PINS_Resource10 = 0x8a,
    PPI_PINS_Resource11 = 0x8b,
    PPI_PINS_Resource12 = 0x8c,
    PPI_PINS_Resource13 = 0x8d,
    PPI_PINS_Resource14 = 0x8e,
    PPI_PINS_Resource15 = 0x8f,
    PPI_PINS_Resource16 = 0x90,
    PPI_PINS_Resource17 = 0x91,
    PPI_PINS_Resource18 = 0x92,
    PPI_PINS_Resource19 = 0x93,
    PPI_PINS_Resource20 = 0x94,
    PPI_PINS_Resource21 = 0x95,
    PPI_PINS_Resource22 = 0x96,
    PPI_PINS_Resource23 = 0x97,
    PPI_PINS_Resource24 = 0x98,
    PPI_PINS_Resource25 = 0x99,
    PPI_PINS_Resource26 = 0x9a,
    PPI_PINS_Resource27 = 0x9b,
    PPI_PINS_Resource28 = 0x9c,
    PPI_PINS_Resource29 = 0x9d,
    PPI_PINS_Resource30 = 0x9e,
    PPI_PINS_Resource31 = 0x9f,
    PPI_PINS_Resource32 = 0xa0,
    PPI_PINS_Resource33 = 0xa1,
    PPI_PINS_Resource34 = 0xa2,
    PPI_PINS_Resource35 = 0xa3,
    PPI_PINS_Resource36 = 0xa4,
    PPI_PINS_Resource37 = 0xa5,
    PPI_PINS_Resource38 = 0xa6,
    PPI_PINS_Resource39 = 0xa7,
    PPI_PINS_Resource40 = 0xa8,
    PPI_PINS_Resource41 = 0xa9,
    PPI_PINS_Resource42 = 0xaa,
    PPI_PINS_Resource43 = 0xab,
    PPI_PINS_Resource44 = 0xac,
    PPI_PINS_Resource45 = 0xad,
    PPI_PINS_Resource46 = 0xae,
    PPI_PINS_Resource47 = 0xaf,
    PPI_PINS_Resource48 = 0xb0,
    PPI_PINS_Resource49 = 0xb1,
    PPI_PINS_Resource50 = 0xb2,
    PPI_PINS_Resource51 = 0xb3,
    PPI_PINS_Resource52 = 0xb4,
    PPI_PINS_Resource53 = 0xb5,
    PPI_PINS_Resource54 = 0xb6,
    PPI_PINS_Resource55 = 0xb7,
    PPI_PINS_Resource56 = 0xb8,
    PPI_PINS_Resource57 = 0xb9,
    PPI_PINS_Resource58 = 0xba,
    PPI_PINS_Resource59 = 0xbb,
    PPI_PINS_Resource60 = 0xbc,
    PPI_PINS_Resource61 = 0xbd,
    PPI_PINS_Resource62 = 0xbe,
    PPI_PINS_Resource63 = 0xbf

};

/// @brief This structure allows for multiple PPI_Pins_TypeEncode enumerations to be encoded within 1 structure.
typedef struct PPI_Pins_State
{
    uint64_t lowerBits;
    uint64_t upperBits;
} PPI_Pins_State;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_PinsGetState
///
/// @brief A function to determine if a particular pin is set.
///
/// @param [in] state The state to be used in the SetMultiple/GetMultiple pins
/// @param [in] pin The pin to check
/// @return If the pin is asserted.
///
//////////////////////////////////////////////////////////////////////////
inline bool PPI_PinsGetState(PPI_Pins_State state, PPI_Pins_TypeEncode pin)
{
    uint64_t correctField;
    if (pin < 64)
    {
        correctField = state.lowerBits;
    }
    else
    {
        correctField = state.upperBits;
        pin -= 64;
    }
    return ((correctField >> pin) & 0x1) == 1;
}

#if defined(__cplusplus)
// C doesn't support references.
    extern "C++" {
        //////////////////////////////////////////////////////////////////////////
        //  Function: PPI_PinsSetState
        ///
        /// @brief A function to set a particular pin. Available only from C++.
        ///
        /// @param [in] state The state to be used in the SetMultiple/GetMultiple pins
        /// @param [in] pin The pin state to change
        /// @param [in] value The value to assign to this pin
        /// @return A new Pins_State object that is the same as the original with that bit changed
        ///
        //////////////////////////////////////////////////////////////////////////
        inline PPI_Pins_State& PPI_PinsSetState(PPI_Pins_State& state, PPI_Pins_TypeEncode pin, bool value)
        {
            if (pin < 64)
            {
                state.lowerBits = (state.lowerBits & ~(static_cast<uint64_t>(1) << pin)) | (static_cast<uint64_t>(value) << pin);
            }
            else
            {
                pin -= 64;
                state.upperBits = (state.upperBits & ~(static_cast<uint64_t>(1) << pin)) | (static_cast<uint64_t>(value) << pin);
            }
            return state;
        }

    }
#endif

///
/// @brief Options for pins and slots.
///
DECLARE_ENUM(PPI_PINS_OPTIONS_ET)
{
    PINS_Default = 0x000,                // Indicates that no operation with slots is required.
    PINS_Save_Restore_From_Slot = 0x001, // Indicates that the operation should save to a slot on read/restore from a slot on write
};

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfacePinsQueryAliases
///
/// @brief This function allows a client to see which pins are aliases for each other.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] pin The pin to find aliases for
/// @param [in] pinListLength The number of entries in the pinList array; if 0, then numberOfPinsWritten should get the size of the list.
/// @param [out] pinList Populated with the alias list
/// @param [out] numberOfPinsWritten The number of pin aliases written
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support pins
/// @retval OpenIPC_Error_Null_Pointer if any of the pointers are null
/// @retval OpenIPC_Error_Invalid_Parameter if the pin is not supported by the interface.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfacePinsQueryAliases(OpenIPC_DeviceId device, PPI_Pins_TypeEncode pin, uint32_t pinListLength, PPI_Pins_TypeEncode* pinList, uint32_t* numberOfPinsWritten);
typedef OpenIPC_Error (* PPI_InterfacePinsQueryAliases_TYPE)(OpenIPC_DeviceId device, PPI_Pins_TypeEncode pin, uint32_t pinListLength, PPI_Pins_TypeEncode* pinList, uint32_t* numberOfPinsWritten);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfacePinsListDrivablePins
///
/// @brief This function allows for a client to determine which pins of an interface are drivable.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] pinsListLength The number of entries in pinsList; if this number equals 0, then the number of pins should be written into numberOfPinsPopulated
/// @param [out] pinsList The trigger response types that are supported by device.
/// @param [out] numberOfPinsPopulated The number of trigger response written.
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support pins
/// @retval OpenIPC_Error_Null_Pointer if the numberOfPinsPopulated is null or pinsListLength is not 0 and pinsList is null.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfacePinsListDrivablePins(OpenIPC_DeviceId device, uint32_t pinsListLength, PPI_Pins_TypeEncode* pinsList, uint32_t* numberOfPinsPopulated);
typedef OpenIPC_Error (* PPI_InterfacePinsListDrivablePins_TYPE)(OpenIPC_DeviceId device, uint32_t pinsListLength, PPI_Pins_TypeEncode* pinsList, uint32_t* numberOfPinsPopulated);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_InterfacePinsListReadablePins
///
/// @brief This function allows for a client to determine which pins of an interface are readable.
///
/// @param [in] device The interface to use (this must be a pins interface)
/// @param [in] pinsListLength The number of entries in pinsList; if this number equals 0, then the number of pins should be written into numberOfPinsPopulated
/// @param [out] pinsList The trigger response types that are supported by device.
/// @param [out] numberOfPinsPopulated The number of trigger response written.
/// @returns OpenIPC_Error
/// @retval OpenIPC_Error_Invalid_Device_Id if the interface does not support pins
/// @retval OpenIPC_Error_Null_Pointer if the numberOfPinsPopulated is null or pinsListLength is not 0 and pinsList is null.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_InterfacePinsListReadablePins(OpenIPC_DeviceId device, uint32_t pinsListLength, PPI_Pins_TypeEncode* pinsList, uint32_t* numberOfPinsPopulated);
typedef OpenIPC_Error (* PPI_InterfacePinsListReadablePins_TYPE)(OpenIPC_DeviceId device, uint32_t pinsListLength, PPI_Pins_TypeEncode* pinsList, uint32_t* numberOfPinsPopulated);

///
/// @brief Options for Pin assert operations.
///
typedef struct
{
    // Default is PINS_Default
    PPI_PINS_OPTIONS_ET options;
    // Sets the slot to use on a save or load. By default this is: PPI_SLOT_HANDLE_INVALID
    PPI_SlotHandle saved_slot;
} PPI_Pins_PinReadSetOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_SetMultiplePins
///
// @brief Sets multiple Pins.
//
// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle. Bundle handle implies that the operation queued, otherwise it is run immediately.
// @param [in] pins The bit field encoding of the pins to set, either Asserted (1) or Default (0).
// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
// @return (OpenIPC_Error) A code indicating success or failure.
// @retval OpenIPC_Error_No_Error on success
// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
// @retval OpenIPC_Error_Invalid_Parameter if the pin write is not supported by the interface.
//
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_SetMultiplePins(PPI_ProbeBundleHandle handle, PPI_Pins_State pins, const PPI_Pins_PinReadSetOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_SetMultiplePins_TYPE)(PPI_ProbeBundleHandle handle, PPI_Pins_State pins, const PPI_Pins_PinReadSetOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_SetPin
///
/// @brief Sets a Pin.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle. Bundle handle implies that the operation queued, otherwise it is run immediately.
/// @param [in] pin The encoding of the pin to set.
/// @param [in] value The value to set, either Asserted (1) or Default (0).
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Invalid_Parameter if the pin write is not supported by the interface.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_SetPin(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, PPI_bool value, const PPI_Pins_PinReadSetOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_SetPin_TYPE)(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, PPI_bool value, const PPI_Pins_PinReadSetOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_ReadPin
///
/// @brief Reads a Pin's value.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle. Bundle handle implies that the operation queued, otherwise it is run immediately.
/// @param [in] pin The encoding of the pin to assert.
/// @param [out] pinValue The read pin value, asserted (1) or deasserted (0)
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Invalid_Parameter if the pin read is not supported by the interface.
///
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_ReadPin(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, uint32_t* pinValue, const PPI_Pins_PinReadSetOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_ReadPin_TYPE)(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, uint32_t* pinValue, const PPI_Pins_PinReadSetOptions* const options);

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_ReadMultiplePins
///
/// @brief Reads multiple Pin's value.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle. Bundle handle implies that the operation queued, otherwise it is run immediately.
/// @param [in] pins The bitfield packed structure to be populated with the pin state, asserted (1) or deasserted (0)
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Invalid_Parameter if the pin read is not supported by the interface.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_ReadMultiplePins(PPI_ProbeBundleHandle handle, PPI_Pins_State* pins, const PPI_Pins_PinReadSetOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_ReadMultiplePins_TYPE)(PPI_ProbeBundleHandle handle, PPI_Pins_State* pins, const PPI_Pins_PinReadSetOptions* const options);

///
/// @brief Options for Pin pulse operations.
///
typedef struct
{
    // Default is PINS_default
    PPI_PINS_OPTIONS_ET options;
    // Sets the slot to use on a save or load. Default is PPI_SLOT_HANDLE_INVALID
    PPI_SlotHandle saved_slot;
} PPI_Pins_PinPulseOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_PulsePin
///
/// @brief Pulses a Pin.
///
/// @param [in,out] handle Can be a LOCK, RELEASE, or a bundle handle. Bundle handle implies that the operation queued, otherwise it is run immediately.
/// @param [in] pin The encoding of the pin to pulse.
/// @param [in] widthOfPulseInMicroSeconds The length of the pulse, should be > 0.
/// @param [in] startingPinState The starting asserted (1/True) or Deasserted (0/False) state of the pin. This will force the pin to have that value prior to pulsing it.
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
/// @retval OpenIPC_Error_Invalid_Parameter if the pin write is not supported by the interface.
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_PulsePin(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, PPI_bool startingPinState, uint32_t widthOfPulseInMicroSeconds, const PPI_Pins_PinPulseOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_PulsePin_TYPE)(PPI_ProbeBundleHandle handle, PPI_Pins_TypeEncode pin, PPI_bool startingPinState, uint32_t widthOfPulseInMicroSeconds, const PPI_Pins_PinPulseOptions* const options);

///
/// @brief Options for Pin delay operations.
///
typedef struct
{
    // Do not use, maybe replaced in the future
    uint32_t reserved;
} PPI_Pins_DelayOptions;

//////////////////////////////////////////////////////////////////////////
//  Function: PPI_Pins_Delay
///
/// @brief Adds a delay to be executed in a bundle.
///
/// Adds a delay to be executed in a bundle; this delay is in terms of the number of clocks to wait.
/// @param [in,out]handle Can be a LOCK, RELEASE, or a bundle handle (first 2 are immediately run, last is enqueued to the handle).
/// @param [in] timeInMicroSeconds The time to wait
/// @param [in] options Options structure, which can be null; in that case the structure's defaults are used.
/// @return (OpenIPC_Error) A code indicating success or failure.
/// @retval OpenIPC_Error_No_Error on success
/// @retval OpenIPC_Error_Probe_Bundle_Invalid when the handle is not a valid probe handle
/// @retval OpenIPC_Error_Probe_Bundle_Invalid_Handle if handle has already been executed once and the implementation does not support adding commands to a previously executed bundle
///
//////////////////////////////////////////////////////////////////////////
PROBEPLUGIN_API OpenIPC_Error PPI_Pins_Delay(PPI_ProbeBundleHandle handle, uint32_t timeInMicroSeconds, const PPI_Pins_DelayOptions* const options);
typedef OpenIPC_Error (* PPI_Pins_Delay_TYPE)(PPI_ProbeBundleHandle handle, uint32_t timeInMicroSeconds, const PPI_Pins_DelayOptions* const options);

#if defined(__cplusplus)
    } // extern "C"
#endif

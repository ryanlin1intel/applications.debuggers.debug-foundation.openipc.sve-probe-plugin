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
///  @brief Contains Enumeration Declarations for JTAG-based functions.
///
///  For additional information on obtaining and using instances, see @ref probeusage.
///
//////////////////////////////////////////////////////////////////////////////

#pragma once

// Generic headers needed for any JTAG implementation
#include "BundleOperations.h" // for the probe itself
#include "SlotOperations.h"
#include "TapStateMachineEncode.h" // for the JTAG state machine encoding

#if defined(__cplusplus)
    extern "C" {
#endif

//
// Note: These values should not be changed and every value must have at most 1 bit set.
//       It's ok to add new constants though, provided they satisfy the above constraints.
//

// Some ENUM declarations for the JTAG options (state, pins, and register-based)
/// TDI and TDO options
DECLARE_ENUM(PPI_JTAG_TDI_TDO_OPTIONS_ET) {
    JtagOption_TDI_Default  = 0x000,          // Indicates that TDI should be all 0's if TDI is NULL.
    JtagOption_TDI_All_Ones = 0x001,          // Indicates that TDI should be all 1's if TDI is NULL.
    JtagOption_TDI_Restore_From_Slot = 0x002, // Indicates that TDI should be saved to a slot.
    JtagOption_TDO_Save_To_Slot = 0x004,      // Indicates that TDI should be saved to a slot.
};
/// TMS options
DECLARE_ENUM(PPI_JTAG_TMS_OPTIONS_ET) {
    JtagOption_TMS_Default  = 0x000, // Indicates that TMS should be all 0's if TMS is NULL.
    JtagOption_TMS_All_Ones = 0x001, // Indicates that TMS should be all 1's if TMS is NULL.
    JtagOption_TMS_Last_One = 0x002  // Indicates that TMS should be all 0's except for the last shift if TMS is NULL.
};

/// Reset options.
DECLARE_ENUM(PPI_JTAG_RESET_OPTIONS_ET){
    JTAG_RESET_OPTIONS_DEFAULT = 0x0, // Use both TRST pin as well as cycle the TAP state machine
    JTAG_RESET_OPTIONS_DO_NOT_USE_TRSRT_PIN     = 0x01,
    JTAG_RESET_OPTIONS_DO_NOT_USE_STATE_MACHINE = 0x02
};

/// Trigger type options
DECLARE_ENUM(PPI_JTAG_TRIGGER_TYPE_ET) {
    JtagTriggerType_None = 0x000,           // No triggers.
    JtagTriggerType_PRDY = 0x001,           // Trigger on PRDY
    JtagTriggerType_Multi_Probe    = 0x002, // Multi-probe trigger
    JtagTriggerType_Power_Restore  = 0x004, // Power restore
    JtagTriggerType_Reset_Assert   = 0x008, // Reset was asserted
    JtagTriggerType_Reset_Deassert = 0x010, // Reset was deasserted
};

#if defined(__cplusplus)
    } // extern "C"
#endif

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
///  @brief Contains constants for the I2C services.
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

/// @brief I2C slot options
DECLARE_ENUM(PPI_I2C_OPTIONS_ET) {
    I2C_Default = 0x000,                /// Indicates that no operation with slots is required.
    I2C_Save_Restore_From_Slot = 0x001, /// Indicates that the operation should save to a slot on read/restore from a slot on write
    I2C_Do_Not_Require_Ack_On_Last_Write = 0x02,
};

/// I2C optional arguments
typedef struct
{
    /// Default is 0
    PPI_I2C_OPTIONS_ET i2cOptions;
    /// Sets the slot to use on a save or load. Default is 0.
    PPI_SlotHandle savedSlot;
} PPI_I2C_Options;

#if defined(__cplusplus)
    } // extern "C"
#endif

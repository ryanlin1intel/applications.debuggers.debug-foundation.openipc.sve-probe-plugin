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

#pragma once

// note: these enumeration values comply with SVF/XSVF standards (so don't change them!)
enum JtagStateEncode
{
    JtagTLR   = 0x00,
    JtagRTI   = 0x01,
    JtagSelDR = 0x02,
    JtagCapDR = 0x03,
    JtagShfDR = 0x04,
    JtagEx1DR = 0x05,
    JtagPauDR = 0x06,
    JtagEx2DR = 0x07,
    JtagUpdDR = 0x08,
    JtagSelIR = 0x09,
    JtagCapIR = 0x0A,
    JtagShfIR = 0x0B,
    JtagEx1IR = 0x0C,
    JtagPauIR = 0x0D,
    JtagEx2IR = 0x0E,
    JtagUpdIR = 0x0F
};

// these are the string representations of the TAP states corresponding to the enums literals in JtagStateEncode
const char* const c_statestr[] = { "TLR", "RTI", "SelDR", "CapDR", "ShfDR", "Ex1DR", "PauDR", "Ex2DR", "UpdDR", "SelIR", "CapIR", "ShfIR", "Ex1IR", "PauIR", "Ex2IR", "UpdIR" };

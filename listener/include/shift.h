////////////////////////<Source Code Embedded Notices>/////////////////////////
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
//by martin.monroy@intel.com
//shift operation code header file. 

#ifndef SHIFT_H
#define SHIFT_H

#include <iostream>
#include <vector>
#include <cassert>

//set globals used by shift
void set_value(std::vector<uint8_t> value);
void set_size(size_t size);

void SetNthBit(std::vector<uint8_t>& vec, size_t n, bool value);
bool GetNthBit(const std::vector<uint8_t>& vec, size_t n);
std::vector<uint8_t> Shift(const std::vector<uint8_t>& input, size_t bitSize);

#endif
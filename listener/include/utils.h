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

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>   // std::stringstream, std::getline
#include <iomanip>   // std::hex, std::setw, std::setfill, std::uppercase

std::string bytes_to_hex_list(const std::vector<std::uint8_t>& bytes);
static inline void trim_inplace(std::string& s);
bool parse_byte_vector(const std::string& input,std::vector<std::uint8_t>& out,std::string& error);
bool to_size_t_stoul(const std::string& s, size_t& out);

#endif
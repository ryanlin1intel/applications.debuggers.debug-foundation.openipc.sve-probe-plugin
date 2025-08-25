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

#ifndef RECEIVER_H
#define RECEIVER_H

#include <winsock2.h>
#include <windows.h>
//#undef max  if the parenthesis are not used around "max" or min then the pre processor defines for them take over. parenthesis ensure this is treated as a function object first
//#undef min
#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
#include <cstddef>
#include <vector>
#include <cstdint>
#include <sstream>   // std::stringstream, std::getline
#include <sstream>
#include <cctype>    // std::isspace
#include <cassert>
#include <algorithm> // std::find_if
#include <iomanip>   // std::hex, std::setw, std::setfill, std::uppercase
#include <comdef.h>
#include <msxml6.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "msxml6.lib")


//GLOBALS for Shift Register Operation:

std::vector<uint8_t> _value;
size_t _size;

void set_value(std::vector<uint8_t> value){
    _value = value;    
}

void set_size(size_t size){
    _size = size;    
}

void SetNthBit(std::vector<uint8_t>& vec, size_t n, bool value);
bool GetNthBit(const std::vector<uint8_t>& vec, size_t n);
std::vector<uint8_t> Shift(const std::vector<uint8_t>& input, size_t bitSize);
std::string bytes_to_hex_list(const std::vector<std::uint8_t>& bytes);
static inline void trim_inplace(std::string& s);
bool parse_byte_vector(const std::string& input,std::vector<std::uint8_t>& out,std::string& error);
bool to_size_t_stoul(const std::string& s, size_t& out);
std::string extractXMLElement(IXMLDOMDocument* doc, const std::wstring& tag);
std::string buildXMLResponseInit(const std::string& request_id);
std::string buildXMLResponseShift(const std::string& request_id, const std::vector<std::uint8_t>& shiftOutput);

#endif
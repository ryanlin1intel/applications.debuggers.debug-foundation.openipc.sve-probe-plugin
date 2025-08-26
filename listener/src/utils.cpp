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

#include "utils.h"

std::string bytes_to_hex_list(const std::vector<std::uint8_t>& bytes) {
    if (bytes.empty()) return std::string{};
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i) oss << ", ";
        // cast to unsigned so it prints as a number, not a char
        oss << "0x" << std::setw(2) << static_cast<unsigned>(bytes[i]);
    }
    return oss.str();
}


// Trim ASCII whitespace in-place (C++14)
static inline void trim_inplace(std::string& s) {
    auto not_space = [](unsigned char ch){ return std::isspace(ch) == 0; };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}

// Parse comma-separated byte literals (e.g. "0x1A, 255, FF") into 'out'.
// Accepts: 0x.. / 0X.. (hex), plain hex like "FF", or decimal like "255".
// Returns true on success; on failure returns false and sets 'error'.
bool parse_byte_vector(const std::string& input,std::vector<std::uint8_t>& out,std::string& error)
{
    out.clear();
    std::stringstream ss(input);
    std::string token;
    std::size_t index = 0;

    while (std::getline(ss, token, ',')) {
        ++index;
        trim_inplace(token);

        if (token.empty()) {
            error = "Empty element at position " + std::to_string(index);
            return false;
        }

        // Disallow signs for byte values
        if (token.front() == '+' || token.front() == '-') {
            error = "Sign not allowed for byte value at position " + std::to_string(index);
            return false;
        }

        // Choose base: autodetect if "0x"/"0X", else hex if it has hex letters, else decimal.
        int base = 10;
        if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
            base = 0; // std::stoul autodetects 0x / 0 prefix
        } else if (token.find_first_of("abcdefABCDEF") != std::string::npos) {
            base = 16;
        } // else base = 10

        // Parse using stoul with position to detect leftovers
        std::size_t pos = 0;
        unsigned long value = 0;
        try {
            value = std::stoul(token, &pos, base);
        } catch (const std::invalid_argument&) {
            error = "Invalid number at position " + std::to_string(index) + " (token: '" + token + "')";
            return false;
        } catch (const std::out_of_range&) {
            error = "Value out of range at position " + std::to_string(index) + " (token: '" + token + "')";
            return false;
        }

        if (pos != token.size()) {
            error = "Unexpected characters in token at position " + std::to_string(index) +
                    " (near: '" + token.substr(pos) + "')";
            return false;
        }
        if (value > 0xFFul) {
            error = "Value exceeds byte range at position " + std::to_string(index) +
                    " (got " + std::to_string(value) + ")";
            return false;
        }

        out.push_back(static_cast<std::uint8_t>(value));
    }

    // Treat empty input as success with empty vector; change if your policy differs.
    return true;
}


bool to_size_t_stoul(const std::string& s, size_t& out) {
    try {
        unsigned long long v = std::stoull(s);          // handles leading/trailing spaces, base prefixes, +/-
        if (v > (std::numeric_limits<size_t>::max)())   // make sure it fits
            return false;
        out = static_cast<size_t>(v);
        return true;
    } catch (...) {
        return false;
    }
}
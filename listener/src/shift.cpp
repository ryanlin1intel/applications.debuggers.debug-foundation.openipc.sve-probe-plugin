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
//by martin.monroy@intel.com

#include "shift.h"

//globals used by shift
std::vector<uint8_t> _value;
size_t _size;

void set_value(std::vector<uint8_t> value){
    _value = value;    
}

void set_size(size_t size){
    _size = size;    
}


void SetNthBit(std::vector<uint8_t>& vec, size_t n, bool value)
{
    if (value)
    {
        vec[n / 8] |= (1 << (n % 8)); // Set bit
    }
    else
    {
        vec[n / 8] &= ~(1 << (n % 8)); // Clear bit
    }
}

bool GetNthBit(const std::vector<uint8_t>& vec, size_t n)
{
    return ((vec[n / 8] >> (n % 8)) & 1) == 1;
}

std::vector<uint8_t> Shift(const std::vector<uint8_t>& input, size_t bitSize)
{
    assert(input.size() == (bitSize + 7) / 8);
    std::vector<uint8_t> output;
    if (bitSize < 1)
    {
        return output;
    }
    if (bitSize == _size)
    {
        // Exact shift
        output = std::move(_value);
        _value = input;
        return output;
    }

    output.resize((bitSize + 7) / 8, 0);
    if (bitSize > _size)
    {
        // Over-shift

        const size_t bitCountOverBy = bitSize - _size;

        // output[0:_size] = _value[0:_size]
        std::copy_n(_value.begin(), (_size + 7) / 8, output.begin());
        // output[_size:bitsize] = input[0:bitsize - _size]
        for (size_t bitOffset = 0; bitOffset < bitCountOverBy; bitOffset++)
        {
            SetNthBit(output, _size + bitOffset, GetNthBit(input, bitOffset));
        }
        // _value[0:_size] = input[(bitsize - _size): bitsize]
        for (size_t bitOffset = 0; bitOffset < _size; bitOffset++)
        {
            SetNthBit(_value, bitOffset, GetNthBit(input, bitOffset + bitCountOverBy));
        }
    }
    else
    {
        // Under-shift
        const size_t bitCountUnderBy = _size - bitSize;
        // output[0:bitSize] = _value[0:bitSize]
        std::copy_n(_value.begin(), (bitSize + 7) / 8, output.begin());

        // _value[0:_size-bitSize] = _value[bitSize:_size]
        for (size_t bitOffset = 0; bitOffset < bitCountUnderBy; bitOffset++)
        {
            SetNthBit(_value, bitOffset, GetNthBit(_value, bitOffset + bitSize));
        }

        // _value[_size-bitSize:_size] = input[0:bitSize]
        for (size_t bitOffset = 0; bitOffset < bitSize; bitOffset++)
        {
            // Filling remaining value bits from input
            SetNthBit(_value, bitSize + bitOffset, GetNthBit(input, bitOffset));
        }
    }
    return output;
}